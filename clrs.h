#pragma once

#define IM_COL32_R( _x ) ( ( ( _x ) >> IM_COL32_R_SHIFT ) & 0xFF )
#define IM_COL32_G( _x ) ( ( ( _x ) >> IM_COL32_G_SHIFT ) & 0xFF )
#define IM_COL32_B( _x ) ( ( ( _x ) >> IM_COL32_B_SHIFT ) & 0xFF )
#define IM_COL32_A( _x ) ( ( ( _x ) >> IM_COL32_A_SHIFT ) & 0xFF )

typedef uint32_t colors_t;
enum : uint32_t
{
#define _XTAG( _name, _color, _desc ) _name,
#include "gpuvis_colors.inl"
#undef _XTAG
    col_Max
};

class Clrs
{
public:
    Clrs() {}
    ~Clrs() {}

    //void init();
    //void shutdown();

    ImU32 get(colors_t col, ImU32 alpha = (uint32_t)-1);
    ImVec4 getv4(colors_t col, float alpha = -1.0f);
    float getalpha(colors_t col);
    ImU32 getdef(colors_t col);

    void set(colors_t col, ImU32 color);
    void reset(colors_t col);

    const char* name(colors_t col);
    const char* desc(colors_t col);

    bool is_default(colors_t col);

    // True if this is an alpha or saturation only color
    bool is_alpha_color(colors_t col);
    bool is_imgui_color(colors_t col);

private:
    struct colordata_t
    {
        const char* name;
        ImU32 color;
        const ImU32 defcolor;
        bool modified;
        const char* desc;
    };
    static colordata_t s_colordata[col_Max];
};


// Print color marked up text.
// We've added a quick hack in ImFont::RenderText() which checks for:
//   ESC + RGBA bytes
// This class helps embed these 5 byte color esc sequences.
enum text_colors_t
{
    TClr_Def,
    TClr_Bright,
    TClr_BrightComp,
    TClr_Max
};

class TextClrs& s_textclrs();
class TextClrs
{
public:
    TextClrs() {}
    ~TextClrs() {}

    const char* str(text_colors_t clr)
    {
        return m_buf[clr];
    }

    const std::string mstr(const std::string& str_in, ImU32 color);
    const std::string bright_str(const std::string& str_in)
    {
        return m_buf[TClr_Bright] + str_in + m_buf[TClr_Def];
    }
    const std::string brightcomp_str(const std::string& str_in)
    {
        return m_buf[TClr_BrightComp] + str_in + m_buf[TClr_Def];
    }

    void update_colors();

    static char* set(char(&dest)[6], ImU32 color);

public:
    char m_buf[TClr_Max][6];
};

class TextClr
{
public:
    TextClr(ImU32 color) { TextClrs::set(m_buf, color); }
    ~TextClr() {}

    const char* str() { return m_buf; }

public:
    char m_buf[6];
};

// Color singletons
class Clrs& s_clrs();
class TextClrs& s_textclrs();
