#pragma once

#include "trace_win.h"
#include "rect.h"

/*
struct row_info_t
{
	uint32_t id = 0;
	std::string row_name;
	float row_y = 0;
	float row_h = 0;
};
class GraphRows;
*/
class TraceWin;

class graph_info_t
{
public:
	TraceWin& win;
	graph_info_t(TraceWin& winin) : win(winin) {}

	int64_t ts0;         // visible start time of graph
	int64_t ts1;         // visible end time of graph
	int64_t tsdx;        // ts1 - ts0
	double tsdxrcp;      // 1 / tsdx

	bool mouse_over = false;
	ImVec2 mouse_pos;
	
	ImVec2 size;        // window dimensions
	/*
	rect_t rc;           // current drawing rect

	float text_h = 0;
	float start_y = 0;
	float total_graph_height = 0;
	float visible_graph_height = 0;

	std::vector<row_info_t> row_info;
	row_info_t* prinfo_cur = nullptr;
	row_info_t* prinfo_zoom = nullptr;
	*/
	void set_ts(int64_t start_ts, int64_t length_ts);
	/*
	void init();
	void init_rows(const std::vector<GraphRows::graph_rows_info_t>& graph_rows);
	void calc_process_graph_height();
	//void set_pos_y(float y_in, float h_in, row_info_t* ri);
	float ts_to_x(int64_t ts);
	int64_t screenx_to_ts(float x_in);
	int64_t dx_to_ts(float x_in);
	*/
};
