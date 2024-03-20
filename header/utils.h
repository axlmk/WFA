#pragma once
#define NTDDI_VERSION NTDDI_WIN8 // required to use the IDesktopWallpaper
#include <curl\curl.h>
#include <Windows.h>
#include <initguid.h>
#include <iostream>
#include <filesystem>
#include <exception>

#include "../header/Logging.h"

namespace fs = std::filesystem;

LPWSTR str2lp(std::string str);
std::string lp2str(LPWSTR str);