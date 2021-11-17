#include <string>
#include <vector>
#include <sys/stat.h>
#define SDL_MAIN_HANDLED

#include <SDL.h>

#include "GL/gl3w.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_sdl_gl3.h"

#include "utils2.h"
#include "graph_info.h"
#include "timeline.h"
#include "img_utils.h"

void graph_info_t::set_ts(int64_t start_ts, int64_t length_ts)
{
	ts0 = start_ts;
	ts1 = ts0 + length_ts;

	tsdx = ts1 - ts0 + 1;
	tsdxrcp = 1.0 / tsdx;
}

/*
void graph_info_t::init()
{
	const ImVec2 windowpos = ImGui::GetCursorScreenPos();
	const ImVec2 windowsize = ImGui::GetContentRegionAvail();
	rcwin = { windowpos.x, windowpos.y, windowsize.x, windowsize.y };
	rc.x = windowpos.x;
	rc.w = windowsize.x;

	// Get mouse position
	mouse_pos = ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows) ?
		ImGui::GetMousePos() : ImVec2(-FLT_MAX, -FLT_MAX);

	start_y = win.m_graph.start_y;

	// Range check mouse pan values
	start_y = Clamp< float >(start_y, visible_graph_height - total_graph_height, 0.0f);
	win.m_graph.start_y = start_y;
}

void graph_info_t::init_rows(const std::vector<GraphRows::graph_rows_info_t>& graph_rows)
{
	uint32_t id = 0;
	//imgui_push_smallfont();
	float graph_row_padding = ImGui::GetStyle().FramePadding.y;
	text_h = ImGui::GetTextLineHeightWithSpacing();
	total_graph_height = graph_row_padding;
	//imgui_pop_font();

	for (const GraphRows::graph_rows_info_t& grow : graph_rows) {
		row_info_t rinfo;
		const std::string& row_name = grow.row_name;

		rinfo.row_y = total_graph_height;
		rinfo.row_h = text_h * 2;
		rinfo.row_name = row_name;

		rinfo.id = id++;
		row_info.push_back(rinfo);

		total_graph_height += rinfo.row_h + graph_row_padding;
	}

	total_graph_height += imgui_scale(2.0f);
	total_graph_height = std::max< float >(total_graph_height, 8.0f * text_h);

	// Figure out visible_graph_height
	calc_process_graph_height();
}

void graph_info_t::calc_process_graph_height()
{
	// Zoom mode if we have a gfx row and zoom option is set
	float max_graph_size;
	const float valf_min = 8.0f * text_h;

	max_graph_size = total_graph_height;

	// First time initialization - start with 1/2 the screen
	float valf = ImGui::GetContentRegionAvail().y / 2.0f;

	valf = Clamp< float >(valf, valf_min, max_graph_size);
	visible_graph_height = valf;
}

void graph_info_t::set_pos_y(float y_in, float h_in, row_info_t* ri)
{
	rc.y = y_in;
	rc.h = h_in;
	
	prinfo_cur = ri;
	mouse_over = rc.point_in_rect(mouse_pos);
}

float graph_info_t::ts_to_x(int64_t ts)
{
	return rc.w * (ts - ts0) * tsdxrcp;
}

int64_t graph_info_t::screenx_to_ts(float x_in)
{
	double val = (x_in - rc.x) / rc.w;

	return ts0 + val * tsdx;
}

int64_t graph_info_t::dx_to_ts(float x_in)
{
	return (x_in / rc.w) * tsdx;
}

*/

