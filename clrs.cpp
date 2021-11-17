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

#include "clrs.h"

Clrs::colordata_t Clrs::s_colordata[col_Max] =
{
#define _XTAG( _name, _color, _desc ) { #_name, _color, _color, false, _desc },
  #include "gpuvis_colors.inl"
#undef _XTAG
};

ImU32 Clrs::get(colors_t col, ImU32 alpha)
{
    if (alpha <= 0xff)
        return (s_colordata[col].color & ~IM_COL32_A_MASK) | (alpha << IM_COL32_A_SHIFT);

    return s_colordata[col].color;
}

ImU32 Clrs::getdef(colors_t col)
{
    return s_colordata[col].defcolor;
}

ImVec4 Clrs::getv4(colors_t col, float alpha)
{
    ImVec4 color = ImColor(s_colordata[col].color);

    if (alpha >= 0.0f)
        color.w = alpha;
    return color;
}

float Clrs::getalpha(colors_t col)
{
    return IM_COL32_A(s_colordata[col].color) * (1.0f / 255.0f);
}

void Clrs::set(colors_t col, ImU32 color)
{
    if (s_colordata[col].color != color)
    {
        s_colordata[col].color = color;
        s_colordata[col].modified = true;
    }
}

void Clrs::reset(colors_t col)
{
    set(col, s_colordata[col].defcolor);
}

const char* Clrs::name(colors_t col)
{
    // Skip "col_" prefix
    return s_colordata[col].name + 4;
}

const char* Clrs::desc(colors_t col)
{
    return s_colordata[col].desc;
}

bool Clrs::is_default(colors_t col)
{
    return (s_colordata[col].color == s_colordata[col].defcolor);
}

bool Clrs::is_alpha_color(colors_t col)
{
    switch (col)
    {
    case col_ThemeAlpha:
    case col_Graph_PrintLabelSat:
    case col_Graph_PrintLabelAlpha:
    case col_Graph_TimelineLabelSat:
    case col_Graph_TimelineLabelAlpha:
        return true;
    }

    return false;
}

bool Clrs::is_imgui_color(colors_t col)
{
    if ((col >= col_ImGui_Text) || (col == col_ThemeAlpha))
        return true;

    return false;
}

