#include <string>
#include <vector>
#include <sys/stat.h>
#define SDL_MAIN_HANDLED

#include <SDL.h>

#include "GL/gl3w.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_sdl_gl3.h"

#include "timeline.h"
#include "img_utils.h"
#include "actions.h"
#include "clrs.h"
#include "timeline_win.h"

MainApp& s_app()
{
	static MainApp s_app;
	return s_app;
}

Clrs& s_clrs()
{
	static Clrs s_clrs;
	return s_clrs;
}

TextClrs& s_textclrs()
{
	static TextClrs s_textclrs;
	return s_textclrs;
}

Keybd& s_keybd()
{
	static Keybd s_keybd;
	return s_keybd;
}

Actions& s_actions()
{
	static Actions s_actions;
	return s_actions;
}

SDL_Window* MainApp::create_window(const char* title)
{
	SDL_Window* window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 600,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	return window;
}

void MainApp::init(int argc, char** argv)
{
	imgui_set_custom_style(s_clrs().getalpha(col_ThemeAlpha));
}

int main(int argc, char** argv)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER)) {
		fprintf(stderr, "Error. SDL_Init failed: %s\n", SDL_GetError());
		return -1;
	}
	ImGui::CreateContext();

	//s_clrs().init();
	// Init actions singleton
	s_actions().init();
	// Init app
	MainApp& app = s_app();
	app.init(argc, argv);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	SDL_Window* window = app.create_window("Time Line");
	SDL_GLContext glcontext = SDL_GL_CreateContext(window);

	gl3wInit();
	// Setup ImGui binding
	ImGui_ImplSdlGL3_Init(window);
	// 1 for updates synchronized with the vertical retrace
	bool vsync = true;
	SDL_GL_SetSwapInterval(vsync);

	app.data.input_geom();
	app.timeline_win = new TimeLineWin(app.data);

	for (;;) {
		SDL_Event event;
		bool done = false;
		s_actions().clear();
		while (SDL_PollEvent(&event)) {
			ImGui_ImplSdlGL3_ProcessEvent(&event);
			if (event.type == SDL_QUIT)
				done = true;
		}
		bool use_freetype = false;
		ImGui_ImplSdlGL3_NewFrame(window, &use_freetype);

		vsync = !vsync;
		SDL_GL_SetSwapInterval(vsync);

		app.handle_hotkeys();

		app.render();

		imgui_render(window);

		if (done)
			break;
	}
	// Shut down app
	app.shutdown(window);
	// Save color entries
	//s_clrs().shutdown();
	// Close ini file
	//s_ini().Close();

	ImGui_ImplSdlGL3_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void MainApp::shutdown(SDL_Window* window)
{
	delete timeline_win;
	timeline_win = NULL;
}

void MainApp::handle_hotkeys()
{
	if (s_actions().get(action_quit)) {
		SDL_Event event;
		event.type = SDL_QUIT;
		SDL_PushEvent(&event);
	}
}

void MainApp::render()
{
	if (timeline_win->m_open) {
		float w = ImGui::GetIO().DisplaySize.x;
		float h = ImGui::GetIO().DisplaySize.y;

		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::SetNextWindowSizeConstraints(ImVec2(w, h), ImVec2(w, h));

		timeline_win->render();
	}
}

void MainApp::render_menu(const char* str_id)
{
	if (!ImGui::BeginMenuBar())
		return;
	if (ImGui::BeginMenu("File")) {
		if (s_actions().get(action_escape))
			ImGui::CloseCurrentPopup();
		if (ImGui::MenuItem("Quit", s_actions().hotkey_str(action_quit).c_str())) {
			SDL_Event event;
			event.type = SDL_QUIT;
			SDL_PushEvent(&event);
		}
		ImGui::EndMenu();
	}
	ImGui::EndMenuBar();
}
