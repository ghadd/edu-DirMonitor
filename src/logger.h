#ifndef LOGGER_H
#define LOGGER_H

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include <QDir>

static std::shared_ptr<spdlog::logger> server_logger;
static std::shared_ptr<spdlog::logger> server_warn_logger;

void setupLoggers();


#endif // LOGGER_H
