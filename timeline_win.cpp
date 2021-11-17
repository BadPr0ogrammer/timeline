#include <string>
#include <vector>
#include <sys/stat.h>
#define SDL_MAIN_HANDLED

#include <SDL.h>

#include "GL/gl3w.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_sdl_gl3.h"

#include "actions.h"
#include "clrs.h"
#include "img_utils.h"
#include "timeline_win.h"
#include "timeline.h"

void TimeLineWin::render()
{
	ImGui::Begin("TimeLineWin", &m_open,
		ImGuiWindowFlags_MenuBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoBringToFrontOnFocus);

	s_app().render_menu("menu_TimeLineWin");

	static bool once = false;
	if (!once) {
		reset_time_scale();
		once = true;
	}
	graph_render();

	ImGui::End();
}

void TimeLineWin::reset_time_scale()
{
	x_range.ts0 = 0;
	x_range.ts_len = x_range.max_range;
}

void TimeLineWin::graph_render()
{
	init();
	// Make sure ts start and length values are sane
	x_range.check_times(x_range.ts0, x_range.ts_len);

	ImGui::BeginChild("eventgraph", ImVec2(0, y_size), false,
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);
	{
		imgui_drawrect_filled(x_range.vpos, y_pos, x_range.vsize, 20, 0xFFFFFFFF);
		graph_render_time_ticks(16, 8);
		
		graph_render_plot();

		graph_render_mouse_pos();		
		graph_render_mouse_selection();
		// Render mouse tooltips, mouse selections, etc
		mouse_over = x_range.point_in_rect(mouse_pos.x, mouse_pos.y, y_pos, y_size);
		graph_handle_mouse();
	}
	ImGui::EndChild();
	// Horizontal scrollbar
	graph_render_hscrollbar();
	graph_render_resizer();
}

void TimeLineWin::init()
{
	ImVec2 pos = ImGui::GetCursorScreenPos();
	ImVec2 size = ImGui::GetContentRegionAvail();
	x_range.vpos = pos.x;
	x_range.vsize = size.x;
	y_pos = pos.y;
	y_size = size.y - 20;//scroll h
	// Get mouse position
	mouse_pos = ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows) ? ImGui::GetMousePos() : ImVec2(-FLT_MAX, -FLT_MAX);
}

// Render graph decorations
void TimeLineWin::graph_render_time_ticks(float h0, float h1)
{
	double r = 1.0 / x_range.ts_len;
	float dx = x_range.vsize * r * x_range.max_range / 100;
	float x0 = x_range.ts_to_screenx(x_range.ts0);
	for (; x0 <= x_range.vsize; x0 += dx) {
		imgui_drawrect_filled(x0, y_pos, 2, h0, x_range.ts0 < 10 && x0 < dx ? 0xFF0000FF : 0xFF000000);

		for (int i = 1; i < x_range.subticks; i++)
			imgui_drawrect_filled(x0 + i * dx / x_range.subticks, y_pos, 1, h1, 0xFF000000);
	}
}

void TimeLineWin::graph_render_mouse_pos()
{
	// Don't render mouse position if we're resizing the graph (it flashes).
	if (mouse_captured == MOUSE_CAPTURED_RESIZE_GRAPH)
		return;
	// Draw location line for mouse if is over graph
	if (mouse_over) 
		imgui_drawrect_filled(mouse_pos.x, y_pos, 2, y_size, s_clrs().get(col_Graph_MousePos));
}

void TimeLineWin::graph_render_mouse_selection()
{
	// Draw mouse selection location
	if ((mouse_captured == MOUSE_CAPTURED_ZOOM) ||
		(mouse_captured == MOUSE_CAPTURED_SELECT_AREA)) {
		float mousex0 = mouse_capture_pos.x;
		float mousex1 = mouse_pos.x;

		imgui_drawrect_filled(mousex0, y_pos, mousex1 - mousex0, y_size, s_clrs().get(col_Graph_ZoomSel));
	}
}

void TimeLineWin::graph_handle_mouse()
{
	if (mouse_captured)
		graph_handle_mouse_captured();
	else if (mouse_over)
		graph_handle_mouse_over();
}

// Mouse is over our active graph window
void TimeLineWin::graph_handle_mouse_over()
{
	ImGuiIO& io = ImGui::GetIO();
	// Check for clicking, wheeling, etc.
	if (ImGui::IsMouseDoubleClicked(0))
		reset_time_scale(); // zoom to all
	else if (ImGui::IsMouseClicked(0)) {
		// click: pan
		// shift + click: zoom
		// ctrl + click: select area
		uint32_t mask = (io.KeyCtrl << 0) | (io.KeyAlt << 1) | (io.KeyShift << 2);
		if (!mask)
			mouse_captured = MOUSE_CAPTURED_PAN;
		else if (mask == (1 << 2))
			mouse_captured = MOUSE_CAPTURED_ZOOM;
		else if (mask == (1 << 0))
			mouse_captured = MOUSE_CAPTURED_SELECT_AREA;

		if (mouse_captured != MOUSE_NOT_CAPTURED)
			mouse_capture_pos = mouse_pos;
	}
	else if (io.MouseWheel) {
		bool zoomin = (io.MouseWheel > 0.0f);
		int mouse_ts = x_range.screenx_to_ts(mouse_pos.x);
		x_range.zoom(mouse_ts, zoomin);
	}
}

void TimeLineWin::graph_handle_mouse_captured()
{
	// Uncapture mouse if user hits escape
	if (mouse_captured && s_actions().get(action_escape)) {
		mouse_captured = MOUSE_NOT_CAPTURED;
		return;
	}
	bool is_mouse_down = ImGui::IsMouseDown(0);
	bool mouse_pos_valid = ImGui::IsMousePosValid(&mouse_pos);

	// When mouse goes up and mouse is out of window area, mouse_pos goes
	//  to invalid (-FLT_MAX) values. So store away the last known valid
	//  mouse pos here so we can zoom there on mouse up.
	if (mouse_pos_valid)
		mouse_capture_last = mouse_pos;

	if ((mouse_captured == MOUSE_CAPTURED_ZOOM) ||
		(mouse_captured == MOUSE_CAPTURED_SELECT_AREA)) {
		// shift + click: zoom area
		int event_ts0 = x_range.screenx_to_ts(mouse_capture_pos.x);
		int event_ts1 = x_range.screenx_to_ts(mouse_capture_last.x);

		if (event_ts0 > event_ts1)
			std::swap(event_ts0, event_ts1);
		if (mouse_captured == MOUSE_CAPTURED_ZOOM) {
			x_range.ts0 = event_ts0;
			x_range.ts_len = event_ts1 - event_ts0;
		}
	}
	else if (mouse_captured == MOUSE_CAPTURED_PAN) {
		// click: pan
		if (is_mouse_down && mouse_pos_valid) {
			float dx = mouse_pos.x - mouse_capture_pos.x;
			int tsdiff = x_range.dx_to_ts(dx);

			x_range.ts0 -= tsdiff;

			//start_y += mouse_pos.y - mouse_capture_pos.y;

			mouse_capture_pos = mouse_pos;
		}
	}
	if (!is_mouse_down) {
		// Mouse is no longer down, uncapture mouse...
		mouse_captured = MOUSE_NOT_CAPTURED;
	}
}

void TimeLineWin::graph_render_hscrollbar()
{
	float scrollbar_size = ImGui::GetStyle().ScrollbarSize;
	double r = 1.0 / x_range.ts_len;
	float pos = x_range.vsize * r * x_range.ts0;
	float width = x_range.vsize * r * x_range.max_range;

	float w2 = ImGui::GetContentRegionAvailWidth();
	{
		ImGui::SetNextWindowContentSize(ImVec2(width, 0));
		ImGui::BeginChild("#graph_scrollbar", ImVec2(w2, scrollbar_size),
			false, ImGuiWindowFlags_AlwaysHorizontalScrollbar);

		if (pos != scroll_pos) {
			scroll_pos = pos;
			// Graph pos changed: scroll_x should be 0..maxX
			scroll_x = ImGui::GetScrollMaxX() * pos / width;
			ImGui::SetScrollX(scroll_x);
		}
		else if (scroll_x != ImGui::GetScrollX()) {
			// Scrollbar changed: pct should be 0..1
			float pct = ImGui::GetScrollX() / ImGui::GetScrollMaxX();
			int ts0 = pct * x_range.max_range;
			int len = x_range.ts_len;
			if (!x_range.check_times(ts0, len)) 
				x_range.ts0 = ts0;
		}
		ImGui::EndChild();
	}
}

void TimeLineWin::graph_render_resizer()
{
	bool mcaptured = mouse_captured == MOUSE_CAPTURED_RESIZE_GRAPH;
	if (mcaptured || ImGui::IsItemHovered())
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);

	if (mcaptured) {
		if (!ImGui::IsMouseDown(0))
			mouse_captured = MOUSE_NOT_CAPTURED;
	}
	else if (ImGui::IsItemClicked()) 
		mouse_captured = MOUSE_CAPTURED_RESIZE_GRAPH;
}

void TimeLineWin::graph_render_plot()
{
	float scale_y = 150;
	float shift_y = 200;
	float r = (double)x_range.vsize / x_range.ts_len;
	float scale_x = r * 10000; //12
	float shift_x = r * x_range.ts0;
	float val0[3]{
		scale_y * data.geom_interp[0][0].second,
		scale_y * data.geom_interp[1][0].second,
		scale_y * data.geom_interp[2][0].second };
	ImColor color[3]{
		ImColor(0, 0, 255, 255),
		ImColor(0, 255, 0, 255),
		ImColor(255, 0, 0, 255) };
	for (int i = 1; i < data.geom_interp[0].size(); i++) {
		float val1[3]{
			scale_y * data.geom_interp[0][i].second,
			scale_y * data.geom_interp[1][i].second,
			scale_y * data.geom_interp[2][i].second };
		for (int j = 0; j < 3; j++) {
			ImGui::GetWindowDrawList()->AddLine(
				ImVec2(-shift_x + scale_x * (i - 1), shift_y + val0[j]),
				ImVec2(-shift_x + scale_x * i, shift_y + val1[j]),
				color[j], 2);
		}
		val0[0] = val1[0];
		val0[1] = val1[1];
		val0[2] = val1[2];
	}

	float tc_step = data.geom_interp[0].size() / data.geom_frames[0].size();
	for (int i = 0; i < data.geom_frames[0].size(); i++) {
		float val1[3]{
			scale_y * data.geom_frames[0][i].second,
			scale_y * data.geom_frames[1][i].second,
			scale_y * data.geom_frames[2][i].second };
		for (int j = 0; j < 3; j++) {
			ImGui::GetWindowDrawList()->AddCircleFilled(
				ImVec2(-shift_x + tc_step * scale_x * i, shift_y + val1[j]),
				7, color[j], 12);
		}
	}
}
