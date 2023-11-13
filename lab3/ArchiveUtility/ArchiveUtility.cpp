#include <iostream>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <tchar.h>

bool fileExists(const TCHAR* path) {
    DWORD attribs = GetFileAttributes(path);
    return (attribs != INVALID_FILE_ATTRIBUTES &&
        !(attribs & FILE_ATTRIBUTE_DIRECTORY));
}

bool directoryExists(const TCHAR* path) {
    DWORD attribs = GetFileAttributes(path);
    return (attribs != INVALID_FILE_ATTRIBUTES &&
        (attribs & FILE_ATTRIBUTE_DIRECTORY));
}

void printUsage() {
    std::cout << "Usage: ArchiveUtility [operation] [source] [destination]\n\n";
    std::cout << "Available operations:\n";
    std::cout << "\tp - pack\n";
    std::cout << "\tu - unpack\n\n";
    std::cout << "Example:\n";
    std::cout << "ArchiveUtility u Packed.zip C:\\dst - Unpack Packed.zip to C:\\dst\n";
    std::cout << "ArchiveUtility p Message.txt Message.zip - Pack Message.txt to Message.zip\n";
}



DWORD run7Zip(const TCHAR* args) {
    TCHAR* command = new TCHAR[MAX_PATH];
    _sntprintf_s(command, MAX_PATH, MAX_PATH, _T("7za.exe %s"), args);

    SECURITY_ATTRIBUTES saAttr;
    HANDLE stdReadHandle = NULL;
    HANDLE stdWriteHandle = NULL;

    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&stdReadHandle, &stdWriteHandle, &saAttr, 0)) {
        std::cerr << "CreatePipe failed" << std::endl;
        return 1;
    }

    if (!SetHandleInformation(stdReadHandle, HANDLE_FLAG_INHERIT, 0)) {
        std::cerr << "SetHandleInformation failed" << std::endl;
        return 1;
    }

    STARTUPINFO startInfo;
    PROCESS_INFORMATION procInfo;

    ZeroMemory(&startInfo, sizeof(STARTUPINFO));
    startInfo.cb = sizeof(STARTUPINFO);
    startInfo.hStdError = stdWriteHandle;
    startInfo.hStdOutput = stdWriteHandle;
    startInfo.dwFlags |= STARTF_USESTDHANDLES;

    if (!CreateProcess(NULL, command, NULL, NULL, TRUE, 0, NULL, NULL, &startInfo, &procInfo)) {
        std::cerr << "CreateProcess failed" << std::endl;
        return 1;
    }

    WaitForSingleObject(procInfo.hProcess, INFINITE);
    DWORD exitCode;
    GetExitCodeProcess(procInfo.hProcess, &exitCode);

    if (exitCode != 0) {
        char buffer[4096];
        DWORD bytesRead;
        ReadFile(stdReadHandle, buffer, sizeof(buffer), &bytesRead, NULL);
        std::cerr.write(buffer, bytesRead);
    }

    CloseHandle(stdWriteHandle);
    CloseHandle(stdReadHandle);
    CloseHandle(procInfo.hProcess);
    CloseHandle(procInfo.hThread);
    delete[] command;
    return exitCode;
}

void createArchive(const TCHAR* srcPath, const TCHAR* dstPath) {
    if (!fileExists(srcPath)) {
        std::cerr << "Error: Source file doesn't exist.\n";
        return;
    }

    TCHAR* args = new TCHAR[MAX_PATH];
    _sntprintf_s(args, MAX_PATH, MAX_PATH, _T("a %s %s"), dstPath, srcPath);
    DWORD exitCode = run7Zip(args);

    if (exitCode == 0) {
        std::cout << "Done.\n";
    }

    delete[] args;
}

void extractArchive(const TCHAR* srcPath, const TCHAR* dstPath) {
    if (!fileExists(srcPath)) {
        std::cerr << "Error: Source archive doesn't exist.\n";
        return;
    }

    TCHAR* args = new TCHAR[MAX_PATH];;
    _sntprintf_s(args, MAX_PATH, MAX_PATH, _T("x %s -o%s"), srcPath, dstPath);
    DWORD exitCode = run7Zip(args);

    if (exitCode == 0) {
        std::cout << "Done.\n";
    }

    delete[] args;
}

int _tmain(int argc, TCHAR* argv[])
{
    if (!fileExists(_T("7za.exe"))) {
        std::cerr << "Error: 7za.exe not found.\n";
        return 1;
    }

    if (argc < 4) {
        std::cerr << "Error: Invalid argument count.\n\n";
        printUsage();
        return 1;
    }

    if (*argv[1] == _T('p')) {
        createArchive(argv[2], argv[3]);
    } else if (*argv[1] == _T('u')) {
        extractArchive(argv[2], argv[3]);
    } else {
        std::cerr << "Error: Invalid operation.\n\n";
        printUsage();
        return 1;
    }

    return 0;
}