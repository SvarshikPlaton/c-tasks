#include "stdafx.h"

std::string GetProcessorArchitectureName(WORD architectureCode)
{
    std::string architectureName;

    switch (architectureCode)
    {
    case PROCESSOR_ARCHITECTURE_INTEL:
        architectureName = "x86 (32-bit)";
        break;
    case PROCESSOR_ARCHITECTURE_AMD64:
        architectureName = "x64 (64-bit)";
        break;
    case PROCESSOR_ARCHITECTURE_ARM:
        architectureName = "ARM";
        break;
    case PROCESSOR_ARCHITECTURE_IA64:
        architectureName = "Itanium-based";
        break;
    default:
        architectureName = "Unknown";
        break;
    }

    return architectureName;
}

int main(int argc, char* argv[])
{
    CStringW  mode = argv[1];

    if (argc != 2 &&
        (mode == "-e" || mode == "-s")) {
        std::cout << "Wrong arguments." << std::endl;
        return 1;
    }

    if (mode == "-e") {
        std::cout << "Throwing error:" << std::endl;

        // More than 2 GB for the heap, throwing an error  
        LPVOID pMemory = LocalAlloc(LMEM_FIXED, 0xFFFFFFFFF);

        if (pMemory == NULL)
        {
            DWORD error = GetLastError();
            LPVOID errorMessage;
            FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, error, 0, (LPWSTR)&errorMessage, 0, NULL);
            std::cout << "Error code: " << error << ", error message: " << errorMessage << std::endl;
            LocalFree(errorMessage);
        }
        else
        {
            LocalFree(pMemory);
        }
    }
    else
    {
        std::cout << "System info:" << std::endl;

        MEMORYSTATUSEX memStatus;
        memStatus.dwLength = sizeof(memStatus);
        GlobalMemoryStatusEx(&memStatus);

        std::cout << "Physical Memory (RAM) Total: " << memStatus.ullTotalPhys << " bytes" << std::endl;
        std::cout << "Physical Memory (RAM) Available: " << memStatus.ullAvailPhys << " bytes" << std::endl;
        std::cout << "Virtual  Memory (Page File) Total: " << memStatus.ullTotalPageFile << " bytes" << std::endl;
        std::cout << "Virtual  Memory (Page File) Available: " << memStatus.ullAvailPageFile << " bytes" << std::endl;

        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);

        std::cout << "Processor Architecture: " << GetProcessorArchitectureName(sysInfo.wProcessorArchitecture) << std::endl;
        std::cout << "Page Size: " << sysInfo.dwPageSize << " bytes" << std::endl;
        std::cout << "Number of Processors: " << sysInfo.dwNumberOfProcessors << std::endl;
    }
   
    return 0;
}


    


