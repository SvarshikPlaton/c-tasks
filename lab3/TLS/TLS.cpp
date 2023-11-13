#include "stdafx.h"

DWORD tlsIndex;

struct ThreadContext {
	int id;
	std::vector<int> data;
};

int randomInteger(int lowerBound, int upperBound) {
	return lowerBound + std::rand() % (upperBound - lowerBound);
}

int findGCD(int a, int b) {
	while (b != 0) {
		int temp = b;
		b = a % b;
		a = temp;
	}

	return a;
}

void calculateGCD(ThreadContext* ctx) {
	std::vector<int> data = ctx->data;
	int gcd = data[0];

	for (int i = 1; i < data.size(); ++i) {
		gcd = findGCD(gcd, data[i]);
	}

	if (!TlsSetValue(tlsIndex, (LPVOID)gcd)) {
		std::cerr << std::format("Error: TlsSetValue failed for {}\n", ctx->id);
	}
}

DWORD gcdThreadEntryPoint(LPVOID param) {
	ThreadContext* ctx = (ThreadContext*)param;
	calculateGCD(ctx);

	int result = (int)TlsGetValue(tlsIndex);
	std::cout << std::format("[{}] Done: {}.\n", ctx->id, result);
	return 0;
}

int main(int argc, char** argv)
{
	if (argc < 2) {
		std::cerr << "Error: Number of threads missing.\n";
		return 1;
	}

	int threadCount = std::atoi(argv[1]);
	std::vector<ThreadContext> threads(threadCount);
	HANDLE* threadHandles = new HANDLE[threadCount];

	if (threadCount < 2 || threadCount > 64) {
		std::cerr << "Error: Invalid number of threads (2-64).\n";
		return 1;
	}

	std::cout << std::format("Number of threads: {}\n", threadCount);
	std::srand(std::time(nullptr));

	tlsIndex = TlsAlloc();
	if (tlsIndex == TLS_OUT_OF_INDEXES) {
		std::cerr << "Error: TLS allocation error.\n";
		return 1;
	}

	std::vector<std::vector<int>> data(threadCount);
	for (int i = 0; i < threadCount; ++i) {
		data[i] = std::vector<int>(randomInteger(5, 10));
		for (int j = 0; j < data[i].size(); ++j) {
			data[i][j] = randomInteger(10, 100);
		}

		threads[i].id = i;
		threads[i].data = data[i];

		threadHandles[i] = CreateThread(NULL, 0, gcdThreadEntryPoint, &threads[i], 0, NULL);
		if (threadHandles[i] == INVALID_HANDLE_VALUE) {
			std::cerr << std::format("Failed to create thread {}.\n", i);
		}
	}

	WaitForMultipleObjects(threadCount, threadHandles, TRUE, INFINITE);


	TlsFree(tlsIndex);
	delete[] threadHandles;
}