#include "../header/utils.h"

LPWSTR str2lp(std::string str) {
	LPWSTR a = (LPWSTR)str.c_str();
	return a;
}

std::string lp2str(LPWSTR str) {
	int strLength = WideCharToMultiByte(CP_UTF8, 0, str, -1, nullptr, 0, nullptr, nullptr); 
    std::string stri(strLength, 0); 
    WideCharToMultiByte(CP_UTF8, 0, str, -1, &stri[0], strLength, nullptr, nullptr); 
    return stri; 
}