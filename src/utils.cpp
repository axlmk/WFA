#include "../header/utils.h"

LPCWSTR str2lp(const std::string& str) {
    int wideLength = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    std::wstring wStr(wideLength, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wStr[0], wideLength);
    return wStr.c_str();
}

std::string lp2str(const LPWSTR& str) {
	int strLength = WideCharToMultiByte(CP_UTF8, 0, str, -1, nullptr, 0, nullptr, nullptr); 
    std::string stri(strLength, 0); 
    WideCharToMultiByte(CP_UTF8, 0, str, -1, &stri[0], strLength, nullptr, nullptr); 
    return stri;
}