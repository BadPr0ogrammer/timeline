#include <string>
#include <vector>
#include <sys/stat.h>
#define SDL_MAIN_HANDLED

#include <SDL.h>

#include "GL/gl3w.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_sdl_gl3.h"

#include "img_utils.h"
#include "clrs.h"


void imgui_render(SDL_Window* window)
{
	const ImVec4 color = s_clrs().getv4(col_ClearColor);
	const ImVec2& size = ImGui::GetIO().DisplaySize;

	glViewport(0, 0, (int)size.x, (int)size.y);
	glClearColor(color.x, color.y, color.z, color.w);
	glClear(GL_COLOR_BUFFER_BIT);

	ImGui::Render();
	ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData(), 1);

	SDL_GL_SwapWindow(window);
}

void imgui_drawrect_filled(float x, float y, float w, float h, ImU32 color)
{
	ImDrawList* DrawList = ImGui::GetWindowDrawList();
	if (w < 0.0f) {
		x += w;
		w = -w;
	}
	if (!imgui_is_rect_clipped(x, y, w, h)) 
	{
		if (w <= 1.0f)
			DrawList->AddLine(ImVec2(x, y - 0.5f), ImVec2(x, y + h - 0.5f), color);
		else
			DrawList->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color);
	}
}

void imgui_set_custom_style(float alpha)
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;

	for (int i = 0; i < ImGuiCol_COUNT; i++)
	{
		colors_t icol = col_index_from_imguicol(i);

		if (icol != col_Max)
		{
			// Get imgui color reference
			ImVec4& col = style.Colors[i];

			// Set to new color
			col = s_clrs().getv4(icol);

			if (col.w < 1.00f)
			{
				// Apply alpha
				col.x *= alpha;
				col.y *= alpha;
				col.z *= alpha;
				col.w *= alpha;
			}
		}
	}
}

static colors_t col_index_from_imguicol(ImGuiCol col)
{
	switch (col)
	{
	case ImGuiCol_Text: return col_ImGui_Text;
	case ImGuiCol_TextDisabled: return col_ImGui_TextDisabled;
	case ImGuiCol_WindowBg: return col_ImGui_WindowBg;
	case ImGuiCol_ChildBg: return col_ImGui_ChildBg;
	case ImGuiCol_PopupBg: return col_ImGui_PopupBg;
	case ImGuiCol_Border: return col_ImGui_Border;
	case ImGuiCol_BorderShadow: return col_ImGui_BorderShadow;
	case ImGuiCol_FrameBg: return col_ImGui_FrameBg;
	case ImGuiCol_FrameBgHovered: return col_ImGui_FrameBgHovered;
	case ImGuiCol_FrameBgActive: return col_ImGui_FrameBgActive;
	case ImGuiCol_TitleBg: return col_ImGui_TitleBg;
	case ImGuiCol_TitleBgActive: return col_ImGui_TitleBgActive;
	case ImGuiCol_TitleBgCollapsed: return col_ImGui_TitleBgCollapsed;
	case ImGuiCol_MenuBarBg: return col_ImGui_MenuBarBg;
	case ImGuiCol_ScrollbarBg: return col_ImGui_ScrollbarBg;
	case ImGuiCol_ScrollbarGrab: return col_ImGui_ScrollbarGrab;
	case ImGuiCol_ScrollbarGrabHovered: return col_ImGui_ScrollbarGrabHovered;
	case ImGuiCol_ScrollbarGrabActive: return col_ImGui_ScrollbarGrabActive;
	case ImGuiCol_CheckMark: return col_ImGui_CheckMark;
	case ImGuiCol_SliderGrab: return col_ImGui_SliderGrab;
	case ImGuiCol_SliderGrabActive: return col_ImGui_SliderGrabActive;
	case ImGuiCol_Button: return col_ImGui_Button;
	case ImGuiCol_ButtonHovered: return col_ImGui_ButtonHovered;
	case ImGuiCol_ButtonActive: return col_ImGui_ButtonActive;
	case ImGuiCol_Header: return col_ImGui_Header;
	case ImGuiCol_HeaderHovered: return col_ImGui_HeaderHovered;
	case ImGuiCol_HeaderActive: return col_ImGui_HeaderActive;
	case ImGuiCol_Separator: return col_ImGui_Separator;
	case ImGuiCol_SeparatorHovered: return col_ImGui_SeparatorHovered;
	case ImGuiCol_SeparatorActive: return col_ImGui_SeparatorActive;
	case ImGuiCol_ResizeGrip: return col_ImGui_ResizeGrip;
	case ImGuiCol_ResizeGripHovered: return col_ImGui_ResizeGripHovered;
	case ImGuiCol_ResizeGripActive: return col_ImGui_ResizeGripActive;
	case ImGuiCol_CloseButton: return col_ImGui_CloseButton;
	case ImGuiCol_CloseButtonHovered: return col_ImGui_CloseButtonHovered;
	case ImGuiCol_CloseButtonActive: return col_ImGui_CloseButtonActive;
	case ImGuiCol_PlotLines: return col_Max;
	case ImGuiCol_PlotLinesHovered: return col_Max;
	case ImGuiCol_PlotHistogram: return col_Max;
	case ImGuiCol_PlotHistogramHovered: return col_Max;
	case ImGuiCol_TextSelectedBg: return col_ImGui_TextSelectedBg;
	case ImGuiCol_ModalWindowDarkening: return col_ImGui_ModalWindowDarkening;
	case ImGuiCol_DragDropTarget: return col_Max;
	case ImGuiCol_NavHighlight: return col_Max;
	case ImGuiCol_NavWindowingHighlight: return col_Max;
	}

	assert(0);
	return col_Max;
}

bool imgui_is_rect_clipped(float x, float y, float w, float h)
{
	ImDrawList* DrawList = ImGui::GetWindowDrawList();
	const ImVec4& cr = DrawList->_ClipRectStack.back();
	if ((x > cr.z) || (x + w < cr.x))
		return true;
	if ((y > cr.w) || (y + h < cr.y))
		return true;
	return false;
}

/*
void imgui_draw_text(float x, float y, ImU32 color, const char* text, bool background)
{
	ImU32 color_b = s_clrs().get(col_Graph_RowLabelTextBk);
	ImDrawList* DrawList = ImGui::GetWindowDrawList();

	if (background)
	{
		ImVec2 textsize = ImGui::CalcTextSize(text);

		imgui_drawrect_filled(x - 1, y - 1,
			textsize.x + 2, textsize.y + 2,
			color_b);
	}

	DrawList->AddText(ImVec2(x, y), color, text);
}
*/
