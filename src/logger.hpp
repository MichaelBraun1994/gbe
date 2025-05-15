#pragma once

#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Log.h>

class Logger
{
  plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;

public:
  Logger(const std::string& logfilePath);
};