#include "stdafx.h"

#define BUF_SIZE 4098

int main(int argc, char* argv[])
{
    CStringW  mode = argv[1];
    CStringW  path = argv[2];

    if (argc != 3 &&
        (mode == L"-a" || mode == L"-u")) {
        std::wcout << L"Wrong arguments.\n";
        return 1;
    }

    std::wcout << L"Opening file... ";
    HANDLE handleFile = CreateFile(path,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    

    if (handleFile == INVALID_HANDLE_VALUE) {
        std::wcout << L"File opening error.\n";
        return GetLastError();
    }
    else {
        std::wcout << L"OK\n";
        int position = path.Find(L'.');
        if (position != -1) {
            path.Insert(position, mode == L"-a" ? L"ToUnicode" : L"ToANSI");
        }

        std::wcout << L"Creating file... ";
        HANDLE handleFileOut = CreateFile(path,
            GENERIC_WRITE,
            0,
            NULL,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );

        if (handleFileOut == INVALID_HANDLE_VALUE) {
            std::wcout << L"File creating error.\n";
            CloseHandle(handleFile);
            return GetLastError();
        }
        else {
            std::wcout << L"OK\n";

            DWORD dwRead, dwWrite;
            char buf[BUF_SIZE];
            wchar_t bufw[BUF_SIZE];
            if (mode == L"-a") 
            {
                std::wcout << L"Encoding from ANSI to UNICODE... ";
                while (ReadFile(handleFile, buf, BUF_SIZE - 2, &dwRead, NULL)) {
                    int ccnum = MultiByteToWideChar(CP_UTF8, 0, buf, dwRead, bufw, BUF_SIZE);
                    if (ccnum > 0) {
                        WriteFile(handleFileOut, bufw, ccnum * sizeof(wchar_t), &dwWrite, NULL);
                    } 
                    if (dwRead != (BUF_SIZE - 2)) {
                        break;
                    } 
                }
                std::wcout << L"OK\n";
            }
            else {
                std::wcout << L"Encoding from UNICODE to ANSI... ";
                while (ReadFile(handleFile, buf, (BUF_SIZE - 2) * sizeof(wchar_t), &dwRead, NULL)) {
                    for (int i = 0; i < dwRead; ++i) {
                        bufw[i] = (wchar_t)buf[i];
                    }

                    int ccnum = WideCharToMultiByte(CP_UTF8, 0, &bufw[0], dwRead / sizeof(wchar_t), buf, BUF_SIZE, NULL, NULL);
                    if (ccnum > 0) {
                        WriteFile(handleFileOut, buf, ccnum, &dwWrite, NULL);
                    }

                    if (dwRead != (BUF_SIZE - 2) * sizeof(wchar_t)) {
                        break;
                    }
                }
                std::wcout << L"OK\n";
            }


            FlushFileBuffers(handleFileOut);
            CloseHandle(handleFileOut);
            CloseHandle(handleFile);
            std::wcout << L"Done!\n";
        }
    }

    return 0;
}

