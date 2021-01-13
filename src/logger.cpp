#include "logger.h"


void setupLoggers()
{
    const std::string basic_log_format = "[%T][%n:%l]%15v";
    const std::string errors_log_format = "[%T][%l][%@]%15v";

    std::vector<spdlog::sink_ptr> logger_sinks;
    std::vector<spdlog::sink_ptr> warn_logger_sinks;

    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::debug);
    console_sink->set_pattern(basic_log_format);
    logger_sinks.push_back(console_sink);

    // file sinks:
    // basic-log.txt
    auto basic_file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
        QDir::tempPath().toStdString() + "/logs-server/basic-log.txt", true);
    basic_file_sink->set_level(spdlog::level::trace);
    basic_file_sink->set_pattern(basic_log_format);
    logger_sinks.push_back(basic_file_sink);

    // warning-log.txt
    auto warn_file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
        QDir::tempPath().toStdString() + "/logs-server/warns-log.txt", true);
    warn_file_sink->set_level(spdlog::level::warn);
    warn_file_sink->set_pattern(errors_log_format);
    warn_logger_sinks.push_back(warn_file_sink);

    // errors-log.txt
    auto error_file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
        QDir::tempPath().toStdString() + "/logs-server/errors-log.txt", true);
    error_file_sink->set_level(spdlog::level::err);
    error_file_sink->set_pattern(errors_log_format);
    logger_sinks.push_back(error_file_sink);
    //-----------------------------------------------------------------------------------------------------------------------------------------------------
    auto logger = std::make_shared<spdlog::logger>(
        "server_logger", logger_sinks.begin(), logger_sinks.end());
    logger->set_level(spdlog::level::trace);

    // use warn_logger only for "warning-log.txt"
    auto warn_logger = std::make_shared<spdlog::logger>(
        "server_warn_logger", warn_logger_sinks.begin(), warn_logger_sinks.end());
    warn_logger->set_level(spdlog::level::warn);

    spdlog::register_logger(logger);
    spdlog::register_logger(warn_logger);

    spdlog::set_default_logger(logger);

    spdlog::flush_every(std::chrono::seconds(5));



}
