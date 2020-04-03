//
// Created by jaenster on 04/04/2020.
//

#include "StringManipulation.h"

wchar_t* AnsiToUnicode(const char* str, UINT codepage) {
    wchar_t* buf = NULL;
    int len = MultiByteToWideChar(codepage, 0, str, -1, buf, 0);
    buf = new wchar_t[len];
    MultiByteToWideChar(codepage, 0, str, -1, buf, len);
    return buf;
}

char* UnicodeToAnsi(const wchar_t* str, UINT codepage) {
    char* buf = NULL;
    int len = WideCharToMultiByte(codepage, 0, str, -1, buf, 0, (codepage ? NULL : "?"), NULL);
    buf = new char[len];
    WideCharToMultiByte(codepage, 0, str, -1, buf, len, (codepage ? NULL : "?"), NULL);
    return buf;
}
