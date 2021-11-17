#pragma once

#include <vector>

struct geom_data_t
{
	std::vector<std::pair<int, float>> geom_interp[3];
	std::vector<std::pair<int, float>> geom_frames[3];
	float geom_max = -FLT_MAX, geom_min = FLT_MAX;

	void input_geom();
};


