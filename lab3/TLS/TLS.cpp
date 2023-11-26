#include "stdafx.h"

DWORD tlsDividers;
DWORD tlsSum;

struct ThreadContext {
	int id;
	std::vector<int> data;
};

std::string joinArray(int* data, int size) {
	std::stringstream stream;

	for (int i = 0; i < size - 1; ++i) {
		stream << data[i] << ", ";
	}

	stream << data[size - 1];
	return stream.str();
}

int randomInteger(int lowerBound, int upperBound) {
	return lowerBound + std::rand() % (upperBound - lowerBound);
}

int findGreatestDivisor(int number) {
	int divider = number;
	do
	{
		divider--;
	} while (number % divider != 0);
	return divider;
}

void calculateGD(ThreadContext* ctx) {
	std::vector<int> data = ctx->data;

	int* divisors = (int*)LocalAlloc(LPTR, data.size() * sizeof(int));
	int sum = 0;
	if (!TlsSetValue(tlsDividers, divisors)) {
		std::cerr << std::format("Error: TlsSetValue failed for {}\n", ctx->id);
		return;
	}
	if (!TlsSetValue(tlsSum, (LPVOID)sum)) {
		std::cerr << std::format("Error: TlsSetValue failed for {}\n", ctx->id);
		return;
	}

	for (int i = 0; i < data.size(); ++i) {
		divisors[i] = findGreatestDivisor(data[i]);
		sum += divisors[i];
	}
}

DWORD gdThreadEntryPoint(LPVOID param) {
	ThreadContext* ctx = (ThreadContext*)param;
	calculateGD(ctx);

	int* resultDividers = (int*)TlsGetValue(tlsDividers);
	int resultSum = (int)TlsGetValue(tlsSum);
	std::cout << std::format("[{}] Done.\nOriginal array: {}.\nModified array: {}.\nSum: {}.\n",
		ctx->id,
		joinArray(&ctx->data[0], ctx->data.size()),
		joinArray(resultDividers, ctx->data.size()),
		resultSum
	);
	
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

	tlsDividers = TlsAlloc();
	if (tlsDividers == TLS_OUT_OF_INDEXES) {
		std::cerr << "Error: Divider TLS allocation error.\n";
		return 2;
	}
	tlsSum = TlsAlloc();
	if (tlsDividers == TLS_OUT_OF_INDEXES) {
		std::cerr << "Error: Sum TLS allocation error.\n";
		return 3;
	}

	std::vector<std::vector<int>> data(threadCount);
	for (int i = 0; i < threadCount; ++i) {
		data[i] = std::vector<int>(randomInteger(5, 10));
		for (int j = 0; j < data[i].size(); ++j) {
			data[i][j] = randomInteger(10, 100);
		}

		threads[i].id = i;
		threads[i].data = data[i];

		threadHandles[i] = CreateThread(NULL, 0, gdThreadEntryPoint, &threads[i], 0, NULL);
		if (threadHandles[i] == INVALID_HANDLE_VALUE) {
			std::cerr << std::format("Failed to create thread {}.\n", i);
		}
	}

	WaitForMultipleObjects(threadCount, threadHandles, TRUE, INFINITE);


	TlsFree(tlsDividers);
	TlsFree(tlsSum);
	delete[] threadHandles;
}