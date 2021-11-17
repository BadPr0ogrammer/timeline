#pragma once

struct rect_t
{
    float x = FLT_MAX;
    float y = FLT_MAX;
    float w = FLT_MAX;
    float h = FLT_MAX;

    rect_t() {}
    rect_t(float _x, float _y, float _w, float _h) :
        x(_x), y(_y), w(_w), h(_h) {}

    bool point_in_rect(const ImVec2& pt) const
    {
        return (pt.x >= x && pt.x < x + w &&
            pt.y >= y && pt.y < y + h);
    }
};

