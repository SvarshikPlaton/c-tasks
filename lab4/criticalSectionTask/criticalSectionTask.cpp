#include "stdafx.h"

const int ARRAY_SIZE = 50000;
int data [ARRAY_SIZE];

DWORD WINAPI calculateSum(LPVOID criticalSection) {
    int sum = 0;
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        if (criticalSection) {
            EnterCriticalSection((CRITICAL_SECTION*)criticalSection);
        }
        sum += data[i];
        if (criticalSection) {
            LeaveCriticalSection((CRITICAL_SECTION*)criticalSection);
        }
    }
    std::cout << std::format("Sum {}\n", sum);
    return 0;
}

DWORD WINAPI calculateAverage(LPVOID criticalSection) {
    int sum = 0;
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        if (criticalSection) {
            EnterCriticalSection((CRITICAL_SECTION*)criticalSection);
        }
        sum += data[i];
        if (criticalSection) {
            LeaveCriticalSection((CRITICAL_SECTION*)criticalSection);
        }
    }
    double average = static_cast<double>(sum) / ARRAY_SIZE;
    std::cout << std::format("Average: {}\n", average);
    return 0;
}

DWORD WINAPI calculateMinMax(LPVOID criticalSection) {
    int minVal = data[0];
    int maxVal = data[0];
    for (int i = 1; i < ARRAY_SIZE; ++i) {
        if (criticalSection) {
            EnterCriticalSection((CRITICAL_SECTION*)criticalSection);
        }
        minVal = std::min(minVal, data[i]);
        maxVal = std::max(maxVal, data[i]);
        if (criticalSection) {
            LeaveCriticalSection((CRITICAL_SECTION*)criticalSection);
        }
    }
    std::cout << std::format("Min: {}\n", minVal);
    std::cout << std::format("Max: {}\n", maxVal);
    return 0;
}

int randomInteger(int lowerBound, int upperBound) {
    return lowerBound + std::rand() % (upperBound - lowerBound);
}

int main()
{
    for (int i = 0; i < ARRAY_SIZE-1; ++i) {
        data[i] = randomInteger(-1000, 1001);
    }

    CRITICAL_SECTION criticalSection;
    InitializeCriticalSection(&criticalSection);

    DWORD startTime = GetTickCount();

    HANDLE threads[3];
    threads[0] = CreateThread(NULL, 0, calculateSum, &criticalSection, 0, NULL);
    threads[1] = CreateThread(NULL, 0, calculateAverage, &criticalSection, 0, NULL);
    threads[2] = CreateThread(NULL, 0, calculateMinMax, &criticalSection, 0, NULL);

    WaitForMultipleObjects(3, threads, TRUE, INFINITE);

    DWORD endTime = GetTickCount();

    std::cout << "Time with critical section: " << endTime - startTime << " ms\n";

    startTime = GetTickCount();

    threads[0] = CreateThread(NULL, 0, calculateSum, NULL, 0, NULL);
    threads[1] = CreateThread(NULL, 0, calculateAverage, NULL, 0, NULL);
    threads[2] = CreateThread(NULL, 0, calculateMinMax, NULL, 0, NULL);

    WaitForMultipleObjects(3, threads, TRUE, INFINITE);

    endTime = GetTickCount();

    std::cout << "Time without critical section: " << endTime - startTime << " ms\n";
}
