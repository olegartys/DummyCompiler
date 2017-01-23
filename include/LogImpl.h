//
// Created by olegartys on 08.01.17.
//

#ifndef SHOUSESERVERCOMPONENT_LOGIMPL_H
#define SHOUSESERVERCOMPONENT_LOGIMPL_H

#include <iostream>

// TODO: use branch prediction optimization.
// WARN: platform-specific. Need to check whether this optimization is supported.
#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)

template <typename T>
void Log::info(const std::string& TAG, const T& val) {
    auto logger = spdlog::get(TAG);
    if (logger) {
        logger->info(val);
    } else {
        logger = spdlog::stdout_color_mt(TAG);
        logger->info(val);
    }
}

template <typename ...Args>
void Log::info(const std::string& TAG, const char* fmt, const Args&... args) {
    auto logger = spdlog::get(TAG);
    if (logger) {
        logger->info(fmt, args...);
    } else {
        logger = spdlog::stdout_color_mt(TAG);
        logger->info(fmt, args...);
    }
}

// FIXME: for some reason debug from spdlog doesn't work on my machine
template <typename T>
void Log::debug(const std::string& TAG, const T& val) {
    auto logger = spdlog::get(TAG);

    if (logger != nullptr) {
        logger->info(val);
    } else {
        logger = spdlog::stdout_color_mt(TAG);
        logger->info(val);
    }
}

template <typename ...Args>
void Log::debug(const std::string& TAG, const char* fmt, const Args&... args) {
    auto logger = spdlog::get(TAG);
    if (logger) {
        logger->info(fmt, args...);
    } else {
        logger = spdlog::stdout_color_mt(TAG);
        logger->info(fmt, args...);
    }
}

template <typename T>
void Log::warn(const std::string& TAG, const T& val) {
    auto logger = spdlog::get(TAG);
    if (logger) {
        logger->warn(val);
    } else {
        logger = spdlog::stdout_color_mt(TAG);
        logger->warn(val);
    }
}

template <typename ...Args>
void Log::warn(const std::string& TAG, const char* fmt, const Args&... args) {
    auto logger = spdlog::get(TAG);
    if (logger) {
        logger->warn(fmt, args...);
    } else {
        logger = spdlog::stdout_color_mt(TAG);
        logger->warn(fmt, args...);
    }
}

template <typename T>
void Log::error(const std::string& TAG, const T& val) {
    auto logger = spdlog::get(TAG);
    if (logger) {
        logger->error(val);
    } else {
        logger = spdlog::stdout_color_mt(TAG);
        logger->error(val);
    }
}

template <typename ...Args>
void Log::error(const std::string& TAG, const char* fmt, const Args&... args) {
    auto logger = spdlog::get(TAG);
    if (logger) {
        logger->error(fmt, args...);
    } else {
        logger = spdlog::stdout_color_mt(TAG);
        logger->error(fmt, args...);
    }
}

template <typename T>
void Log::critical(const std::string& TAG, const T& val) {
    auto logger = spdlog::get(TAG);
    if (logger) {
        logger->critical(val);
    } else {
        logger = spdlog::stdout_color_mt(TAG);
        logger->critical(val);
    }
}

template <typename ...Args>
void Log::critical(const std::string& TAG, const char* fmt, const Args&... args) {
    auto logger = spdlog::get(TAG);
    if (logger) {
        logger->critical(fmt, args...);
    } else {
        logger = spdlog::stdout_color_mt(TAG);
        logger->critical(fmt, args...);
    }
}

#endif //SHOUSESERVERCOMPONENT_LOGIMPL_H
