#include "../header/Logging.h"

Logs::Logs() : Logs(INFO) {}

Logs::Logs(LogLevel ll) {
    spdlog::set_level((spdlog::level::level_enum)ll);   
};

void Logs::log(std::string msg, logLevel ll) {
    switch (ll) {
        case DEBUG:
            spdlog::debug(msg);
            break;
        case INFO:
            spdlog::info(msg);
            break;
        case WARNING:
            spdlog::warn(msg);
            break;
        case ERR:
            spdlog::error(msg);
            break;
        case TRACE:
            spdlog::trace(msg);
            break;
        default:
            break;
    }
};