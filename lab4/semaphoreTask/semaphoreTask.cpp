#include "stdafx.h"

const int MAX_FILE_POINTERS = 3;
const int NUM_THREADS = 5;      

HANDLE fileSemaphore;
HANDLE hFile;

std::string getTime() {
    FILETIME currentTime;
    GetSystemTimeAsFileTime(&currentTime);
    SYSTEMTIME creationTime;
    FileTimeToSystemTime(&currentTime, &creationTime);
    char timeStr[64];
    sprintf_s(timeStr, sizeof(timeStr), "%02d:%02d:%02d.%03d",
        creationTime.wHour,
        creationTime.wMinute,
        creationTime.wSecond,
        creationTime.wMilliseconds
    );
    return std::string(timeStr);
}

int randomInteger(int lowerBound, int upperBound) {
    return lowerBound + std::rand() % (upperBound - lowerBound);
}

DWORD WINAPI writeToFile(LPVOID lpParam) {
    int threadID = (int)lpParam;

    Sleep(randomInteger(1, 3) * 1000);

    WaitForSingleObject(fileSemaphore, INFINITE);

    std::string data = "Thread " + std::to_string(threadID) + " writing data at " + getTime() + "\n";
    WriteFile(hFile, data.c_str(), static_cast<DWORD>(data.size()), NULL, NULL);

    Sleep(randomInteger(1, 3) * 1000);

    ReleaseSemaphore(fileSemaphore, 1, NULL);

    return 0;
}

int main() {
    fileSemaphore = CreateSemaphore(NULL, MAX_FILE_POINTERS, MAX_FILE_POINTERS, NULL);

    if (fileSemaphore == NULL) {
        std::cerr << "Semaphore creation failed." << std::endl;
        return 1;
    }

    hFile = CreateFileA(
        "output.txt",
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open file." << std::endl;
        CloseHandle(fileSemaphore);
        return 1;
    }

    HANDLE threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads[i] = CreateThread(NULL, 0, writeToFile, (LPVOID)i, 0, NULL);
    }

    WaitForMultipleObjects(NUM_THREADS, threads, TRUE, INFINITE);

    for (int i = 0; i < NUM_THREADS; ++i) {
        CloseHandle(threads[i]);
    }

    CloseHandle(hFile);
    CloseHandle(fileSemaphore);

    return 0;
}
