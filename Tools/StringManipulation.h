//
// Created by jaenster on 04/04/2020.
//

#ifndef D2INJECT_STRINGMANIPULATION_H
#define D2INJECT_STRINGMANIPULATION_H

#include <windows.h>

wchar_t* AnsiToUnicode(const char* str, UINT codepage = CP_UTF8);
char* UnicodeToAnsi(const wchar_t* str, UINT codepage = CP_UTF8);

#endif //D2INJECT_STRINGMANIPULATION_H
