#include "stdafx.h"

int main()
{
    printf("Boolean:\n");
    printf("bool\tsize = %i, min = %i, max = %i\n\n", sizeof(bool), false, true);

    printf("Integers:\n");
    printf("short\tsize = %i, min = %i, max = %i\n", sizeof(short), 0x8000, 0x7FFF);
    printf("int\tsize = %i, min = %i, max = %i\n", sizeof(int), 0x80000000, 0x7FFFFFFF);
    printf("long\tsize = %i, min = %i, max = %i\n", sizeof(long), 0x80000000, 0x7FFFFFFF);
    printf("long long\tsize = %i, min = %lli, max = %lli\n\n", sizeof(long long), 0x8000000000000000, 0x7FFFFFFFFFFFFFFF);
    
    printf("Unsigned integers:\n");
    printf("byte\tsize = %i, min = %u, max = %u\n", sizeof(byte), 0, (byte)(0xFF));
    printf("unsigned int\tsize = %i, min = %u, max = %u\n", sizeof(unsigned int), 0, (unsigned int)(0xFFFFFFFF));
    printf("unsigned long\tsize = %i, min = %u, max = %u\n", sizeof(unsigned long), 0, (unsigned int)(0xFFFFFFFF));
    printf("word long\tsize = %i, min = %u, max = %u\n", sizeof(WORD), 0, (WORD)(0xFFFF));
    printf("unsigned short\tsize = %i, min = %u, max = %u\n", sizeof(unsigned short), 0, 0xFFFF);
    printf("dword long\tsize = %i, min = %u, max = %u\n", sizeof(DWORD), 0, (DWORD)(0xFFFFFFFF));
    printf("unsigned long long\tsize = %i, min = %llu, max = %llu\n\n", sizeof(unsigned long long), 0, 0xFFFFFFFFFFFFFFFF);

    printf("Chars:\n");
    printf("char\tsize = %i, min = %i, max = %i\n", sizeof(char), (char)0x80, 0x7F);
    printf("unsigned char\tsize = %u, min = %u, max = %u\n", sizeof(unsigned char), 0, (unsigned char)(0xFF));
    printf("wchar\tsize = %i, min = %i, max = %i\n", sizeof(wchar_t), 0, 0xFFFF);
    printf("tchar\tsize = %i, min = %i, max = %i\n\n", sizeof(TCHAR), 0, 0xFFFF);

    printf("String pointers:\n");
    printf("lpstr\tsize = %i, min = %u, max = %llu\n", sizeof(LPSTR), 0, 0xFFFFFFFFFFFFFFFF);
    printf("lpctstr\tsize = %i, min = %u, max = %llu\n", sizeof(LPCTSTR), 0, 0xFFFFFFFFFFFFFFFF);
    printf("lpwstr\tsize = %i, min = %u, max = %llu\n", sizeof(LPWSTR), 0, 0xFFFFFFFFFFFFFFFF);
    printf("lpcwstr\tsize = %i, min = %u, max = %llu\n", sizeof(LPCWSTR), 0, 0xFFFFFFFFFFFFFFFF);


    return 0;
}


