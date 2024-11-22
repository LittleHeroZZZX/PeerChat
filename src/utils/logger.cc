#include "logger.h"

// 静态成员初始化
std::shared_ptr<spdlog::logger> Logger::instance = nullptr;
std::once_flag Logger::init_instance_flag;

void log_error(const std::string &message) {
    Logger::get_instance()->error(message);
}

void log_info(const std::string &message) {
    Logger::get_instance()->info(message);
}

void log_debug(const std::string &message) {
    Logger::get_instance()->debug(message);
}

void log_warning(const std::string &message) {
    Logger::get_instance()->warn(message);
}
