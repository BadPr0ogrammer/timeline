#pragma once

#include "scaled_range.h"
#include "geom_data.h"

class TimeLineWin
{
public:
	TimeLineWin(const geom_data_t& pd) : data(pd) {}
	// Whether our window is open or not
	bool m_open = true;
	void render();

protected:
	const geom_data_t& data;

	enum mouse_captured_t
	{
		MOUSE_NOT_CAPTURED = 0,
		MOUSE_CAPTURED_SELECT_AREA,
		MOUSE_CAPTURED_ZOOM,
		MOUSE_CAPTURED_PAN,
		MOUSE_CAPTURED_RESIZE_GRAPH
	};

	scaled_range_t x_range;
	float y_pos = 0;
	float y_size = 0;

	bool mouse_over = false;
	ImVec2 mouse_pos;

	// Mouse timestamp location in graph
	int32_t ts_marker_mouse = -1;

	ImVec2 mouse_capture_pos;
	ImVec2 mouse_capture_last;
	mouse_captured_t mouse_captured = MOUSE_NOT_CAPTURED;
	// scroll cached params
	float scroll_pos = -1.0f;
	float scroll_x = -1.0f;

	void init();
	// zoom to all	
	void reset_time_scale();

	void graph_render();
	// Render graph decorations
	void graph_render_time_ticks(float h0, float h1);

	void graph_handle_mouse_over();
	// Zoom in / out graph
	void graph_render_mouse_pos();
	void graph_render_mouse_selection();
	void graph_handle_mouse();
	void graph_handle_mouse_captured();
	void graph_render_resizer();
	void graph_render_hscrollbar();

	void graph_render_plot();
};
