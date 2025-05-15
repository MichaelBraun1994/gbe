#include "logger.hpp"

#include <plog/Initializers/RollingFileInitializer.h>

Logger::Logger(const std::string& logfilePath)
{
  plog::init(plog::info, logfilePath.c_str(), 1'000'000, 1);
  plog::get()->addAppender(&consoleAppender);
}