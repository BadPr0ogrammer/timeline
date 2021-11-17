#include "scaled_range.h"

double scaled_range_t::ts_to_screenx(int ts)
{
	double r = 1.0 / ts_len;
	return vpos + (ts - ts0) * r * vsize;
}

int scaled_range_t::screenx_to_ts(double x_in)
{
	return (int)(ts0 + (double)(x_in - vpos) / vsize * ts_len);
}

int scaled_range_t::dx_to_ts(double dx)
{
	return (int)((double)ts_len / vsize * dx);
}

void scaled_range_t::zoom(int center_ts, bool zoomin)
{
	int origlen = ts_len;
	int amt = zoomin ? -(origlen / 2) : (origlen / 2);
	ts_len = origlen + amt;
	double scale = (double)ts_len / origlen;
	ts0 = center_ts - (int)((center_ts - ts0) * scale);
}

bool scaled_range_t::check_times(int& ts0, int& ts_len)
{
	bool f = false;
	if (ts_len > max_range) {
		ts_len = max_range;
		f = true;
	}
	if (ts0 < 0) {
		ts0 = 0;
		f = true;
	}
	if (ts0 + ts_len > max_range) {
		ts_len = max_range - ts0;
		f = true;
	}
	return f;
}

bool scaled_range_t::point_in_rect(float x, float y, float ypos, float ysize)
{
	return (x >= vpos && x < vpos + vsize && y >= ypos && y < ypos + ysize);
}
