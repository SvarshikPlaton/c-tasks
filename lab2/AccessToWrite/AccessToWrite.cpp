#include "stdafx.h"
#define _TEXT(x)    __T(x)
#define _CRT_SECURE_NO_WARNINGS
#define TEXT_SIZE 80

typedef struct {
    int recordCount;
    int fileSize;
} Header;

typedef struct {
    int recordNumber;
    int modificationCount;
    FILETIME creationTime;
    char text[TEXT_SIZE];
} Record;
 
void AddRecord(HANDLE hFile, char* string) {
    Header header;
    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
    ReadFile(hFile, &header, sizeof(Header), NULL, NULL);

    FILETIME currentTime;
    GetSystemTimeAsFileTime(&currentTime);

    Record record;
    record.recordNumber = header.recordCount + 1;
    record.modificationCount = 0;
    record.creationTime = currentTime;
    strncpy_s(record.text, string, TEXT_SIZE);

    SetFilePointer(hFile, sizeof(Header) +  sizeof(Record) * header.recordCount, NULL, FILE_BEGIN);
    WriteFile(hFile, &record, sizeof(Record), NULL, NULL);

    header.recordCount++;
    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
    WriteFile(hFile, &header, sizeof(Header), NULL, NULL);
    std::cout << "Record #" << header.recordCount << " added..";
}

void ShowRecord(HANDLE hFile, int recordNumber) {
    Header header;
    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
    ReadFile(hFile, &header, sizeof(Header), NULL, NULL);

    if (recordNumber <= header.recordCount && 0 < recordNumber) {
        Record record;
        SetFilePointer(hFile, sizeof(header) +  sizeof(Record) * (recordNumber - 1), NULL, FILE_BEGIN);
        ReadFile(hFile, &record, sizeof(Record), NULL, NULL);

        SYSTEMTIME creationTime;
        FileTimeToSystemTime(&record.creationTime, &creationTime);

        std::cout << "\nHeader:" << std::endl <<
            "Records: "<< header.recordCount << std::endl <<
            "Size: "<< header.fileSize << std::endl << std::endl <<
            "Record:" << std::endl <<
            "Number: " << record.recordNumber << std::endl <<
            "Modification count: " << record.modificationCount << std::endl <<
            "Text: " << record.text << std::endl;
        printf(
            "Creation time: %04d-%02d-%02d %02d:%02d:%02d\n",
            creationTime.wYear,
            creationTime.wMonth,
            creationTime.wDay,
            creationTime.wHour,
            creationTime.wMinute,
            creationTime.wSecond
        );
    }
    else {
        std::cout << "Wrong record number. There are " <<
            header.recordCount << " records in total.";
    }
}

void ChangeRecord(HANDLE hFile, int recordNumber, char* string) {
    Header header;
    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
    ReadFile(hFile, &header, sizeof(Header), NULL, NULL);

    if (recordNumber <= header.recordCount && 0 < recordNumber) {
        Record record;
        SetFilePointer(hFile, sizeof(Header) + sizeof(Record) * (recordNumber - 1), NULL, FILE_BEGIN);
        ReadFile(hFile, &record, sizeof(Record), NULL, NULL);

        strcpy_s(record.text, string);
        record.modificationCount++;

        SetFilePointer(hFile, -(int)sizeof(Record), NULL, FILE_CURRENT);
        WriteFile(hFile, &record, sizeof(Record), NULL, NULL);

        std::cout << "Record #" << recordNumber << " changed..";
    }
    else {
        std::cout << "Wrong record number. There are " << 
            header.recordCount << " records in total.";
    }
}

void RemoveRecord(HANDLE hFile, int recordNumber) {
    Header header;
    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
    ReadFile(hFile, &header, sizeof(Header), NULL, NULL);

    if (recordNumber <= header.recordCount && 0 < recordNumber) {
        for (int i = recordNumber; i < header.recordCount; i++) {
            Record record;
            SetFilePointer(hFile, sizeof(Header) + sizeof(Record) * i, NULL, FILE_BEGIN);
            ReadFile(hFile, &record, sizeof(Record), NULL, NULL);

            record.recordNumber--;
            SetFilePointer(hFile, -(int)(sizeof(Record)) * 2, NULL, FILE_CURRENT);
            WriteFile(hFile, &record, sizeof(Record), NULL, NULL);
        }

        header.recordCount--;
        SetEndOfFile(hFile);
        SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
        WriteFile(hFile, &header, sizeof(Header), NULL, NULL);

        std::cout << "Record #" << recordNumber << "removed..";
    }
    else {
        std::cout << "Wrong record number. There are " <<
             header.recordCount << " records in total.";
    }
}

int _tmain(int argc, TCHAR** argv) {

    HANDLE hFile = CreateFile(argv[1], GENERIC_ALL, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        hFile = CreateFile(argv[1], GENERIC_ALL, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

        if (hFile != INVALID_HANDLE_VALUE) {
            Header header = { 0, 0 };
            BYTE* buffer = reinterpret_cast<BYTE*>(&header);
            WriteFile(hFile, buffer, sizeof(header), NULL, NULL);
            std::cout << "The ñreated file is open..\n";
        }
        else {
            std::cerr << "Failed to create file." << std::endl;
            return 1;
        }
    }
    else {
        std::cout << "The existing file is open..\n";
    }
    

    while (true) {
        std::cout << "File records menu:" << std::endl <<
            "1 - Add" << std::endl <<
            "2 - Show" << std::endl <<
            "3 - Change" << std::endl <<
            "4 - Remove" << std::endl << 
            std::endl <<
            "Choice: " ;

        int choice, number; 
        char text[TEXT_SIZE];

        std::cin >> choice;
        switch (choice)
        {
        case 1:
            std::cout << "Add some text for new record (maximum " <<
                TEXT_SIZE << " characters):" << std::endl << std::endl;
            std::cin.ignore();
            std::cin.getline(text, TEXT_SIZE);

            AddRecord(hFile, text);
            break;
        case 2:
            std::cout << "Enter the number of existing record: ";
            std::cin >> number;

            ShowRecord(hFile, number);
            break;
        case 3:
            std::cout << "Enter the record number: ";
            std::cin >> number;

            std::cout << "Add some text to change (maximum " << TEXT_SIZE <<
                " characters):" << std::endl << std::endl;
            std::cin.ignore();
            std::cin.getline(text, TEXT_SIZE);

            ChangeRecord(hFile, number, text);
            break;
        case 4:
            std::cout << "Enter the record number: ";
            std::cin >> number;

            RemoveRecord(hFile, number);
            break;
        default:
            std::cout << "Wrong choice!";
            break;
        }
        std::cout << std::endl << std::endl;
        system("pause");
        std::cout << std::endl << std::endl;
    }
    return 0;
}