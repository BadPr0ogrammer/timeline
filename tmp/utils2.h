#pragma once

#include <string>

template < typename T > T Clamp(const T& val, const T& lower, const T& upper) { return std::max< T >(lower, std::min< T >(val, upper)); }
std::string string_formatv(const char* fmt, va_list ap);
std::string string_format(const char* fmt, ...);
