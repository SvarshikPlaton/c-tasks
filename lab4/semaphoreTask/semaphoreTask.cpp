#include "stdafx.h"

const int MAX_FILE_POINTERS = 3;
const int NUM_THREADS = 5;      

HANDLE fileSemaphore;          

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

void writeToFile(int threadID) {
    Sleep(randomInteger(1, 3) * 1000);

    WaitForSingleObject(fileSemaphore, INFINITE);
    std::string filename = "output_" + std::to_string(threadID) + ".txt";
    HANDLE hFile = CreateFileA(
        filename.c_str(),
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile != INVALID_HANDLE_VALUE) {
        {
            std::cout << std::format("Thread {} writing to {}.\n", threadID, filename);
        }
        std::string data = "Thread " + std::to_string(threadID) + " writing data at " + getTime() + "\n";
        WriteFile(hFile, data.c_str(), static_cast<DWORD>(data.size()), NULL, NULL);
        Sleep(randomInteger(1, 3) * 1000);

        CloseHandle(hFile);
        ReleaseSemaphore(fileSemaphore, 1, NULL);
        std::cout << std::format("Thread {} finished writing to {}.\n", threadID, filename);
    }
    else {
        std::cerr << std::format("Thread {} failed to open file {}.\n", threadID, filename);
        ReleaseSemaphore(fileSemaphore, 1, NULL);
    }
}

int main() {
    fileSemaphore = CreateSemaphore(NULL, MAX_FILE_POINTERS, MAX_FILE_POINTERS, NULL);

    if (fileSemaphore == NULL) {
        std::cerr << "Semaphore creation failed." << std::endl;
        return 1;
    }

    std::thread threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads[i] = std::thread(writeToFile, i);
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        threads[i].join();
    }

    CloseHandle(fileSemaphore);

    return 0;
}
