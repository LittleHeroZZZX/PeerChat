#include "logger.h"

// 静态成员初始化
std::shared_ptr<spdlog::logger> Logger::instance = nullptr;
std::once_flag Logger::init_instance_flag;
