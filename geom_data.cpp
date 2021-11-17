#define _USE_MATH_DEFINES 
#include <cmath>

#include "geom_data.h"

void geom_data_t::input_geom()
{
	const int tc_num = 100;
	const int tc_step = 10;
	for (int i = 0; i < tc_num; i++) {
		float phi = 2 * M_PI / tc_num * i;
		float c[3];
		c[0] = 1 + (1 - cos(phi)) * sin(phi);
		c[1] = 1 + (1 - cos(phi)) * cos(phi);
		c[2] = (float)i / tc_num;
		for (int j = 0; j < 3; j++)
			geom_interp[j].push_back(std::pair<int, float>(i, c[j]));
		if (i % tc_step == 0) {
			for (int j = 0; j < 3; j++)
				geom_frames[j].push_back(geom_interp[j].at(i));
		}
		for (int j = 0; j < 3; j++) {
			if (geom_min > c[j])
				geom_min = c[j];
			if (geom_max < c[j])
				geom_max = c[j];
		}
	}
}
