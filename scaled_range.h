#pragma once

struct scaled_range_t
{
	const int max_range = 1000000;
	const int subticks = 4;

	int ts0 = 0;			// visible start time of graph
	int ts_len = 0;			// visible time length of graph
	int vpos = 0;            // view pos
	int vsize = 0;           // view size

	double ts_to_screenx(int ts);
	int screenx_to_ts(double x_in);
	int dx_to_ts(double dxin);
	void zoom(int center_ts, bool zoomin);
	bool check_times(int& ts0, int& ts_len);
	bool point_in_rect(float x, float y, float ypos, float ysize);
};

