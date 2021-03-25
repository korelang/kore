#ifndef KORE_LOGGING_HPP
#define KORE_LOGGING_HPP

#include "location.hpp"

void debug(const char* const format, ...);

void debug_group(const std::string& group, const char* const format, ...);

void info(const char* const format, ...);

void info_group(const std::string& group, const char* const format, ...);

void warn(const char* const format, ...);

void error(const char* const format, ...);

void error_group(const std::string& group, const char* const format, ...);

void parser_error(const Location& location, const char* const format, ...);

#endif // KORE_LOGGING_HPP
