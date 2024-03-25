#pragma once
#define NTDDI_VERSION NTDDI_WIN8 // required to use the IDesktopWallpaper
#include <iostream>
#include <filesystem>
#include <exception>

#include "../header/Logging.h"

namespace fs = std::filesystem;

// Convert a string to a wide string. Required by some win32 functions
LPCWSTR str2lp(const std::string& str);

// Narrow a wide string to a normal string
std::string lp2str(const LPWSTR& str);