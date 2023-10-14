#pragma once

#include <filesystem>
#include <exception>
#include <atlstr.h>
#include "spdlog/spdlog.h"

namespace fs = std::filesystem;

LPWSTR std2lw(std::string str);
std::string lw2str(LPWSTR str);