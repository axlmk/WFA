#include "../header/utils.h"

LPWSTR std2lw(std::string str) {
	USES_CONVERSION_EX;
	return A2W_EX(str.c_str(), str.length());
}

std::string lw2str(LPWSTR str) {
	return CW2A(str);
}