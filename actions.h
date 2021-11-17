#pragma once

#include <SDL.h>

// Useful SDL functions:
//   const char *SDL_GetKeyName( SDL_Keycode key );
//   const char *SDL_GetScancodeName( SDL_Scancode scancode );
class Keybd
{
public:
    Keybd() { clear(); }
    ~Keybd() {}

    void clear();
    void update(const SDL_KeyboardEvent& key);

public:
    uint32_t m_keystate[SDL_NUM_SCANCODES];
};

enum action_t
{
    action_nil,

    action_help,
    action_open,
    action_quit,
    action_trace_info,

    action_menu_file,
    action_menu_options,

    action_scroll_up,
    action_scroll_down,
    action_scroll_left,
    action_scroll_right,
    action_scroll_pageup,
    action_scroll_pagedown,
    action_scroll_home,
    action_scroll_end,

    action_toggle_vblank0,
    action_toggle_vblank1,
    action_toggle_vblank_hardware_timestamps,
    action_toggle_framemarkers,
    action_toggle_frame_filters,

    action_toggle_show_eventlist,

    action_graph_zoom_row,
    action_graph_zoom_mouse,
    action_graph_hide_row,

    action_graph_show_hovered_pid,
    action_graph_show_hovered_tgid,

    action_cpugraph_hide_systemevents,

    action_graph_set_markerA,
    action_graph_set_markerB,
    action_graph_goto_markerA,
    action_graph_goto_markerB,

    action_graph_save_location1,
    action_graph_save_location2,
    action_graph_save_location3,
    action_graph_save_location4,
    action_graph_save_location5,

    action_graph_restore_location1,
    action_graph_restore_location2,
    action_graph_restore_location3,
    action_graph_restore_location4,
    action_graph_restore_location5,

    action_graph_pin_tooltip,

    action_focus_graph,
    action_focus_eventlist,

    action_frame_marker_prev_fit,
    action_frame_marker_next_fit,
    action_frame_marker_prev,
    action_frame_marker_next,

    action_save_screenshot,

    action_escape,
    action_return,

    action_max
};

class Actions
{
public:
    Actions() {}
    ~Actions() {}

    void init();
    void clear();

    bool get(action_t action);
    bool peek(action_t action);
    size_t count() { return m_action_count; }

    void set(action_t action);

    const std::string hotkey_str(action_t action);

    void keydown(SDL_Keycode keycode, uint32_t modstate, bool repeat);

public:
    // modstate actionmap bit for allowing repeating keys
#define KMOD_REPEAT 0x80000000

    struct actionmap_t
    {
        action_t action;
        uint32_t modstate;
        SDL_Keycode key;
        const char* desc;
    };
    std::vector< actionmap_t > m_actionmap;

    uint32_t m_action_count = 0;
    bool m_actions[action_max];
};

// Keyboard singletons
class Keybd& s_keybd();
class Actions& s_actions();

