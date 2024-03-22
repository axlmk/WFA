#include <spdlog/spdlog.h>

namespace Log{
    typedef enum logLevel {
        TRACE,
        DEBUG,
        INFO,
        WARNING,
        ERR
    } LogLevel;

    inline void InitiazeLogLevel(LogLevel ll) {
        spdlog::set_level((spdlog::level::level_enum)ll);
    }

    inline void log(std::string msg, logLevel ll = DEBUG) {
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
}