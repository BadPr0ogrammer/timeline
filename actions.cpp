#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cctype>
#include <sstream>
#include <unordered_map>
#include <functional>

#include "actions.h"

void Actions::init()
{
    clear();

    m_actionmap.push_back({ action_help, KMOD_NONE, SDLK_F1, "Help dialog" });
    m_actionmap.push_back({ action_quit, KMOD_CTRL, SDLK_q, "Quit GpuVis" });
    m_actionmap.push_back({ action_save_screenshot, KMOD_NONE, SDLK_F12, "Capture screenshot" });

    m_actionmap.push_back({ action_menu_file, KMOD_ALT, SDLK_f, "File Menu" });
    m_actionmap.push_back({ action_menu_options, KMOD_ALT, SDLK_o, "Options Menu" });

    m_actionmap.push_back({ action_open, KMOD_CTRL | KMOD_SHIFT, SDLK_o, "Open Trace File dialog" });
    m_actionmap.push_back({ action_trace_info, KMOD_CTRL | KMOD_SHIFT, SDLK_i, "Display trace information" });

    m_actionmap.push_back({ action_focus_graph, KMOD_CTRL | KMOD_SHIFT, SDLK_g, "Set focus to events graph" });
    m_actionmap.push_back({ action_focus_eventlist, KMOD_CTRL | KMOD_SHIFT, SDLK_e, "Set focus to event list" });

    m_actionmap.push_back({ action_toggle_show_eventlist, KMOD_NONE, SDLK_F11, "Toggle showing event list" });

    m_actionmap.push_back({ action_graph_show_hovered_pid, KMOD_CTRL | KMOD_SHIFT, SDLK_s, "Toggle showing only first hovered pid events" });
    m_actionmap.push_back({ action_graph_show_hovered_tgid, KMOD_CTRL | KMOD_SHIFT, SDLK_t, "Toggle showing only first hovered tgid events" });

    m_actionmap.push_back({ action_cpugraph_hide_systemevents, KMOD_CTRL | KMOD_SHIFT, SDLK_h, "CPU Graph: Toggle hiding sched_switch 'system' events" });

    m_actionmap.push_back({ action_graph_zoom_row, KMOD_CTRL | KMOD_SHIFT, SDLK_z, "Graph: Toggle hovered row timeline fullscreen" });
    m_actionmap.push_back({ action_graph_zoom_mouse, KMOD_NONE, SDLK_z, "Graph: Toggle hovered location zoom to 3ms / restore pre-zoom" });
    m_actionmap.push_back({ action_graph_hide_row, 0, SDLK_h, "Graph: Hide hovered row" });

    m_actionmap.push_back({ action_toggle_vblank0, KMOD_CTRL | KMOD_SHIFT, SDLK_m, "Graph: Toggle showing vblank 0 markers" });
    m_actionmap.push_back({ action_toggle_vblank1, KMOD_CTRL | KMOD_SHIFT, SDLK_n, "Graph: Toggle showing vblank 1 markers" });
    m_actionmap.push_back({ action_toggle_vblank_hardware_timestamps, KMOD_CTRL | KMOD_SHIFT, SDLK_k, "Graph: Toggle showing hardware vblank timestamps" });
    m_actionmap.push_back({ action_toggle_framemarkers, KMOD_CTRL | KMOD_SHIFT, SDLK_f, "Graph: Toggle showing Frame Markers" });
    m_actionmap.push_back({ action_toggle_frame_filters, KMOD_CTRL | KMOD_SHIFT, SDLK_r, "Graph: Toggle Frame Filters" });

    m_actionmap.push_back({ action_frame_marker_prev_fit, KMOD_CTRL | KMOD_REPEAT, SDLK_LEFT, "Graph: Show and fit previous frame marker frame" });
    m_actionmap.push_back({ action_frame_marker_next_fit, KMOD_CTRL | KMOD_REPEAT, SDLK_RIGHT, "Graph: Show and fit next frame marker frame" });
    m_actionmap.push_back({ action_frame_marker_prev, KMOD_CTRL | KMOD_SHIFT | KMOD_REPEAT, SDLK_LEFT, "Graph: Show previous frame marker frame" });
    m_actionmap.push_back({ action_frame_marker_next, KMOD_CTRL | KMOD_SHIFT | KMOD_REPEAT, SDLK_RIGHT, "Graph: Show next frame marker frame" });

    m_actionmap.push_back({ action_graph_set_markerA, KMOD_CTRL | KMOD_SHIFT, SDLK_a, "Graph: Set marker A" });
    m_actionmap.push_back({ action_graph_set_markerB, KMOD_CTRL | KMOD_SHIFT, SDLK_b, "Graph: Set marker B" });
    m_actionmap.push_back({ action_graph_goto_markerA, KMOD_CTRL, SDLK_a, "Graph: Goto marker A" });
    m_actionmap.push_back({ action_graph_goto_markerB, KMOD_CTRL, SDLK_b, "Graph: Goto marker B" });

    m_actionmap.push_back({ action_graph_save_location1, KMOD_CTRL | KMOD_SHIFT, SDLK_1, "Graph: Save location 1" });
    m_actionmap.push_back({ action_graph_save_location2, KMOD_CTRL | KMOD_SHIFT, SDLK_2, "Graph: Save location 2" });
    m_actionmap.push_back({ action_graph_save_location3, KMOD_CTRL | KMOD_SHIFT, SDLK_3, "Graph: Save location 3" });
    m_actionmap.push_back({ action_graph_save_location4, KMOD_CTRL | KMOD_SHIFT, SDLK_4, "Graph: Save location 4" });
    m_actionmap.push_back({ action_graph_save_location5, KMOD_CTRL | KMOD_SHIFT, SDLK_5, "Graph: Save location 5" });

    m_actionmap.push_back({ action_graph_restore_location1, KMOD_CTRL, SDLK_1, "Graph: Restore location 1" });
    m_actionmap.push_back({ action_graph_restore_location2, KMOD_CTRL, SDLK_2, "Graph: Restore location 2" });
    m_actionmap.push_back({ action_graph_restore_location3, KMOD_CTRL, SDLK_3, "Graph: Restore location 3" });
    m_actionmap.push_back({ action_graph_restore_location4, KMOD_CTRL, SDLK_4, "Graph: Restore location 4" });
    m_actionmap.push_back({ action_graph_restore_location5, KMOD_CTRL, SDLK_5, "Graph: Restore location 5" });

    m_actionmap.push_back({ action_graph_pin_tooltip, KMOD_CTRL, SDLK_p, "Graph: Pin current graph tooltip" });

    m_actionmap.push_back({ action_scroll_up, KMOD_REPEAT, SDLK_UP, "Graph: Scroll / event list up" });
    m_actionmap.push_back({ action_scroll_down, KMOD_REPEAT, SDLK_DOWN, "Graph: Scroll / event list down" });

    m_actionmap.push_back({ action_scroll_left, KMOD_REPEAT, SDLK_LEFT, "Graph: Scroll / event list left" });
    m_actionmap.push_back({ action_scroll_right, KMOD_REPEAT, SDLK_RIGHT, "Graph: Scroll  event list right" });

    m_actionmap.push_back({ action_scroll_pageup, KMOD_REPEAT, SDLK_PAGEUP, "Graph: Page / event list up" });
    m_actionmap.push_back({ action_scroll_pagedown, KMOD_REPEAT, SDLK_PAGEDOWN, "Graph: Page / event list down" });

    m_actionmap.push_back({ action_scroll_home, KMOD_NONE, SDLK_HOME, "Graph: Scroll / event list to start" });
    m_actionmap.push_back({ action_scroll_end, KMOD_NONE, SDLK_END, "Graph: Scroll / event list to end" });

    m_actionmap.push_back({ action_escape, KMOD_NONE, SDLK_ESCAPE, NULL });
    m_actionmap.push_back({ action_return, KMOD_NONE, SDLK_RETURN, NULL });
}

void Actions::clear()
{
    m_action_count = 0;
    memset(m_actions, 0, sizeof(m_actions));
}

void Actions::keydown(SDL_Keycode keycode, uint32_t modstate, bool repeat)
{
    modstate &= (KMOD_CTRL | KMOD_ALT | KMOD_SHIFT);

    if (modstate & KMOD_CTRL)
        modstate |= KMOD_CTRL;
    if (modstate & KMOD_ALT)
        modstate |= KMOD_ALT;
    if (modstate & KMOD_SHIFT)
        modstate |= KMOD_SHIFT;

    for (const actionmap_t& map : m_actionmap)
    {
        // If this is a repeat key and the action handler doesn't have bit set, bail
        if (repeat && !(map.modstate & KMOD_REPEAT))
            continue;

        if ((map.key == keycode) &&
            ((map.modstate & ~KMOD_REPEAT) == modstate))
        {
            set(map.action);
        }
    }
}

bool Actions::get(action_t action)
{
    if (m_actions[action])
    {
        m_action_count--;
        m_actions[action] = false;
        return true;
    }

    return false;
}

bool Actions::peek(action_t action)
{
    return m_actions[action];
}

void Actions::set(action_t action)
{
    if (!m_actions[action])
    {
        m_action_count++;
        m_actions[action] = true;
    }
}

const std::string Actions::hotkey_str(action_t action)
{
    if (action != action_nil)
    {
        for (const actionmap_t& map : m_actionmap)
        {
            if (map.action == action)
            {
                std::string str;

                if (map.modstate & KMOD_CTRL)
                    str += "Ctrl+";
                if (map.modstate & KMOD_ALT)
                    str += "Alt+";
                if (map.modstate & KMOD_SHIFT)
                    str += "Shift+";

                str += SDL_GetKeyName(map.key);

                return str;
            }
        }
    }

    return "";
}

void Keybd::clear()
{
    memset(m_keystate, 0, sizeof(m_keystate));
}

