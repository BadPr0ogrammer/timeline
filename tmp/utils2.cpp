#include <fcntl.h>
#include <sys/stat.h>

#ifndef WIN32
#include <unistd.h>
#else
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#endif

#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cctype>
#include <sstream>
#include <unordered_map>
#include <functional>

#include <SDL.h>

#include "GL/gl3w.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"   // BeginColumns(), EndColumns() WIP
#include "imgui/imgui_freetype.h"

#include "utils2.h"


std::string string_formatv(const char* fmt, va_list ap)
{
	std::string str;
	int size = 512;

	for (;; )
	{
		str.resize(size);
		int n = vsnprintf((char*)str.c_str(), size, fmt, ap);

		if ((n > -1) && (n < size))
		{
			str.resize(n);
			return str;
		}

		size = (n > -1) ? (n + 1) : (size * 2);
	}
}

std::string string_format(const char* fmt, ...)
{
	va_list ap;
	std::string str;

	va_start(ap, fmt);
	str = string_formatv(fmt, ap);
	va_end(ap);

	return str;
}
/*
#define NSECS_PER_MSEC 1000000LL
#define NSECS_PER_SEC  1000000000LL

std::string ts_to_timestr(int64_t event_ts, int precision, const char* suffix)
{
	double val = event_ts * (1.0 / NSECS_PER_MSEC);

	return string_format("%.*lf%s", precision, val, suffix ? suffix : " ms");
}
*/