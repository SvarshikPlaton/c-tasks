#include "stdafx.h"

#define _TEXT(x)    __T(x)

void ThrowError() {
    DWORD error = GetLastError();
    LPVOID errorMessage;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, error, 0, (LPWSTR)&errorMessage, 0, NULL);
    LPTSTR errorMessageStr = reinterpret_cast<LPTSTR>(errorMessage);
    std::wcout << "Error code: " << error << ", error message: " << errorMessageStr << std::endl;
    LocalFree(errorMessage);

    system("pause");
}

void ShowDirectory(TCHAR* currentDirectory) {
    TCHAR* _currentDirectory = new TCHAR[MAX_PATH];
    _tcscpy(_currentDirectory, currentDirectory);

    WIN32_FIND_DATA fd;
    TCHAR src_file[MAX_PATH + 2];
    HANDLE dh = FindFirstFile(lstrcat(_currentDirectory, _T("\\*")), &fd);
    
    _tcscpy(_currentDirectory, currentDirectory);
    lstrcat(_currentDirectory, _T("\\\0"));
  

    if (dh != INVALID_HANDLE_VALUE) {
        do
        {
            BY_HANDLE_FILE_INFORMATION fi;
            SYSTEMTIME st;

            memset(src_file, 0, sizeof(src_file));
            lstrcpy(src_file, _currentDirectory);
            lstrcat(src_file, fd.cFileName);

            DWORD dwAttr = GetFileAttributes(src_file);

            if (dwAttr & FILE_ATTRIBUTE_DIRECTORY) {
                printf("%25S    <DIR>\r\n", fd.cFileName);
            }
            else {
                HANDLE hf = CreateFile(
                    src_file,
                    GENERIC_READ,
                    FILE_SHARE_READ | FILE_SHARE_WRITE,
                    NULL,
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL,
                    NULL
                );

                if (hf != INVALID_HANDLE_VALUE) {
                    GetFileInformationByHandle(hf, &fi);
                    FileTimeToSystemTime(&(fi.ftCreationTime), &st);

                    printf(
                        "%25S    %11dB   %02d-%02d-%04d  %02d:%02d\r\n",
                        fd.cFileName,
                        fi.nFileSizeLow,
                        st.wDay,
                        st.wMonth,
                        st.wYear,
                        st.wHour,
                        st.wMinute
                    );

                    CloseHandle(hf);
                }
            }
        } while (FindNextFile(dh, &fd));

        FindClose(dh);
    }
    else {
        ThrowError();
    }
}

void ChangeDirectory(TCHAR* currentDirectory) {
    TCHAR* _currentDirectory = new TCHAR[MAX_PATH];
    _tcscpy(_currentDirectory, currentDirectory);

    std::wcout << "Enter new directory: ";

    TCHAR newDirectory[MAX_PATH];
    std::wcin >> newDirectory;

    if (SetCurrentDirectory(newDirectory)) {
        GetCurrentDirectory(MAX_PATH, currentDirectory);
    }
    else {
        ThrowError();
    }
}

void CopyFile() {
    TCHAR sourcePath[MAX_PATH];
    std::wcout << "Source: ";
    std::wcin >> sourcePath;

    TCHAR destinationPath[MAX_PATH];
    std::wcout << "Destination: ";
    std::wcin >> destinationPath;

    if (CopyFile(sourcePath, destinationPath, FALSE)) {
        std::wcout << "File copied successfully." << std::endl;
    }
    else {
        ThrowError();
    }
}

void CreateDirectory(TCHAR* currentDirectory) {
    TCHAR* _currentDirectory = new TCHAR[MAX_PATH];
    _tcscpy(_currentDirectory, currentDirectory);

    std::wcout << "Enter directory name to create: ";
    TCHAR newDirName[MAX_PATH];
    std::wcin >> newDirName;

    lstrcat(_currentDirectory, _T("\\"));
    lstrcat(_currentDirectory, newDirName);

    if (CreateDirectory(_currentDirectory, nullptr)) {
        std::wcout << "Directory created successfully." << std::endl;
    }
    else {
        ThrowError();
    }
}

void DeleteFileOrDirectory(TCHAR* currentDirectory) {
    TCHAR* _currentDirectory = new TCHAR[MAX_PATH];
    _tcscpy(_currentDirectory, currentDirectory);

    std::wcout << "Enter file or directory name to delete: ";
    TCHAR fileOrDirName[MAX_PATH];
    std::wcin >> fileOrDirName;

    lstrcat(_currentDirectory, _T("\\"));
    lstrcat(_currentDirectory, fileOrDirName);

    if (RemoveDirectory(_currentDirectory) || DeleteFile(_currentDirectory)) {
        std::wcout << "File or directory deleted successfully." << std::endl;
    }
    else {
        ThrowError();
    }
}

void GetFileInfo(TCHAR* currentDirectory) {
    TCHAR* _currentDirectory = new TCHAR[MAX_PATH];
    _tcscpy(_currentDirectory, currentDirectory);

    std::wcout << "Enter file's name: ";
    TCHAR filename[MAX_PATH];
    std::wcin >> filename;

    lstrcat(_currentDirectory, _T("\\"));
    lstrcat(_currentDirectory, filename);

    WIN32_FILE_ATTRIBUTE_DATA fileInfo;

    if (GetFileAttributesEx(_currentDirectory, GetFileExInfoStandard, &fileInfo)) {
        SYSTEMTIME creationTime, lastAccess, lastWrite;
        FileTimeToSystemTime(&fileInfo.ftCreationTime, &creationTime);
        FileTimeToSystemTime(&fileInfo.ftLastAccessTime, &lastAccess);
        FileTimeToSystemTime(&fileInfo.ftLastWriteTime, &lastWrite);

        std::wcout << "File Name: " << _currentDirectory << std::endl;
        std::wcout << "File Size: " << fileInfo.nFileSizeLow << " bytes" << std::endl;
        std::wcout << "Is Directory: " << (fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ? L"Yes" : L"No") << std::endl;
        std::wcout << "Creation Date: " <<
            creationTime.wYear << "-" <<
            creationTime.wMonth << "-" <<
            creationTime.wDay << " " <<
            creationTime.wHour << ":" <<
            creationTime.wMinute << ":" <<
            creationTime.wSecond << std::endl;
        std::wcout << "Last Access Time: " <<
            lastAccess.wYear << "-" <<
            lastAccess.wMonth << "-" <<
            lastAccess.wDay << " " <<
            lastAccess.wHour << ":" <<
            lastAccess.wMinute << ":" <<
            lastAccess.wSecond << std::endl;
        std::wcout << "Last Write Time: " <<
            lastWrite.wYear << "-" <<
            lastWrite.wMonth << "-" <<
            lastWrite.wDay << " " <<
            lastWrite.wHour << ":" <<
            lastWrite.wMinute << ":" <<
            lastWrite.wSecond << std::endl;
    }
    else {
        ThrowError();
    }
}

int main()
{
    std::wcout << "Welcome to File Manager!" << std::endl << std::endl;
    char choice[5];
    TCHAR currentDirectory[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentDirectory);

    while (true) {
        std::wcout << "Current Directory: " << currentDirectory << std::endl;
        std::wcout << "Menu:" << std::endl;
        std::wcout << "cd - Change directory" << std::endl;
        std::wcout << "ls - Print directory" << std::endl;
        std::wcout << "cp - Copy file" << std::endl;
        std::wcout << "mkdir - Create directory" << std::endl;
        std::wcout << "rm - Delete file or directory" << std::endl;
        std::wcout << "info - Get details about the file" << std::endl;
        std::wcout << "Command: " << std::endl << std::endl;
        std::cin >> choice;

        #define CHOISE_EQUALS(str) (strcmp(str, choice) == 0)

        if (CHOISE_EQUALS("cd")) {
            ChangeDirectory(currentDirectory);
        }
        else if (CHOISE_EQUALS("ls"))
        {
            ShowDirectory(currentDirectory);
        }
        else if (CHOISE_EQUALS("cp"))
        {
            CopyFile();
        }
        else if (CHOISE_EQUALS("mkdir"))
        {
            CreateDirectory(currentDirectory);
        }
        else if (CHOISE_EQUALS("rm"))
        {
            DeleteFileOrDirectory(currentDirectory);
        }
        else if (CHOISE_EQUALS("info"))
        {
            GetFileInfo(currentDirectory);
        } else {
            std::wcout << "Wrong choice!" << std::endl;
        }
        std::wcout << std::endl << std::endl;
    }
}