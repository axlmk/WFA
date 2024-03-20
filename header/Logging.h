#include <spdlog/spdlog.h>

//@to-do change to singleton
class Logs {
    public:
        typedef enum logLevel {
            TRACE,
            DEBUG,
            INFO,
            WARNING,
            ERR
        } LogLevel;

        Logs();
        Logs(LogLevel ll);
        void log(std::string msg, logLevel ll);
};