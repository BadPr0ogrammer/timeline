#pragma once

typedef uint32_t colors_t;

void imgui_render(SDL_Window* window);
void imgui_set_custom_style(float alpha);
colors_t col_index_from_imguicol(ImGuiCol col);
void imgui_drawrect_filled(float x, float y, float w, float h, ImU32 color);
bool imgui_is_rect_clipped(float x, float y, float w, float h);
//void imgui_draw_text(float x, float y, ImU32 color, const char* text, bool background = false);
