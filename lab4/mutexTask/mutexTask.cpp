#include "stdafx.h"

int main() {
    HANDLE mutex = HANDLE WINAPI CreateMutex(NULL, true, L"mutex");
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        std::cerr << "Error: Process is already running." << std::endl;
        return 1;
    }

    std::cout << "Is Running.." << std::endl;
    std::cin.get();

    ReleaseMutex(mutex);
    CloseHandle(mutex);

    return 0;
}