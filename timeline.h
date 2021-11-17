#pragma once

class TimeLineWin;
struct SDL_Window;

#include "geom_data.h";

class MainApp
{
public:
	TimeLineWin* timeline_win = nullptr;
	geom_data_t data;
	SDL_Window* create_window(const char* title);

	void init(int argc, char** argv);
	void shutdown(SDL_Window* window);
	void render_menu(const char* str_id);
	void handle_hotkeys();
	void render();
};

class MainApp& s_app();
