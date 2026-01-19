#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>
#include <mutex>
#include <sstream>
#include <iomanip>
#include <ctime>
#include "colors.h"

class Logger {
public:
    static void log(const std::string& msg) {
        std::lock_guard<std::mutex> lock(mutex_);
        std::cout << "[" << getCurrentTime() << "] " << msg << std::endl;
    }

    static void error(const std::string& msg) {
        std::lock_guard<std::mutex> lock(mutex_);
        std::cerr << Color::RED << "[" << getCurrentTime() << "] [ERROR] " << msg << Color::RESET << std::endl;
    }

private:
    static std::string getCurrentTime() {
        auto now = std::time(nullptr);
        auto tm = *std::localtime(&now);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%H:%M:%S");
        return oss.str();
    }

    static std::mutex mutex_;
};

inline std::mutex Logger::mutex_;

#endif // LOGGER_H
