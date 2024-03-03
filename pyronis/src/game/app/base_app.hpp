#pragma once
#include <game/utils/math.hpp>
#include <game/utils/boost.hpp>
#include <game/manager/game_timer.hpp>
#include <game/manager/console.hpp>
#include <game/gui/rtfont.hpp>
#include <game/manager/resource_manager.hpp>

#include <cstdint>
#include <deque>

enum class os_message_type : std::uint32_t {
    none,
    open_text_box,
    close_text_box,
    check_connection,
    set_fps_limit,
    set_accelerometer_update_hz,
    finish_app,
    set_video_mode,
    tapjoy_get_featured_app,
    tapjoy_get_ad,
    tapjoy_get_movie,
    tapjoy_show_featured_app,
    tapjoy_show_ad,
    tapjoy_show_movie_ad,
    iap_purchase,
    iap_get_purchased_list,
    tapjoy_get_tap_points,
    tapjoy_spend_tap_points,
    tapjoy_award_tap_points,
    tapjoy_show_offers,
    hooked_show_rate_dialog,
    allow_screen_dimming,
    request_ad_size,
    chartboost_cache_interstitial,
    chartboost_show_interstitial,
    chartboost_cache_more_apps,
    chartboost_show_more_apps,
    chartboost_setup,
    chartboost_notify_install,
    chartboost_reserved1,
    chartboost_reserved2,
    flurry_setup,
    flurry_on_page_view,
    flurry_log_event,
    suspend_to_home_screen,
    tapjoy_init_main,
    tapjoy_init_paid_app_with_actionid,
    tapjoy_set_userid,
    iap_consume_item,
    iap_item_details,
    appsflyer_log_purchase,
    flurry_start_timed_event,
    flurry_stop_timed_event,
    user = 1000,
};

#pragma pack(push, 1)
struct os_message_t {
	os_message_type type;
	std::int32_t parm1;
	vec2f_t pos;
	vec2f_t size;
	float font_size;
	std::string string;
	std::uint32_t parm2;
	bool fullscreen;
	std::string string2;
	std::string string3;
};

class base_app_t {
public:
    virtual ~base_app_t();
	boost_signal_t sig_input;
	boost_signal_t sig_input_move;
	boost_signal_t sig_os;
	boost_signal_t sig_update;
	boost_signal_t sig_render;
	boost_signal_t sig_pre_enterbackground;
	boost_signal_t sig_enterbackground;
	boost_signal_t sig_enterforeground;
	boost_signal_t sig_accel;
	boost_signal_t sig_trackball;
	boost_signal_t sig_arcade_input;
	boost_signal_t sig_raw_keyboard;
	boost_signal_t sig_hardware;
	boost_signal_t sig_on_screen_size_changed;
	boost_signal_t sig_unload_surfaces;
	boost_signal_t sig_load_surfaces;
	boost_signal_t sig_joypad_events;
	boost_signal_t sig_native_input_state_changed;
	bool console_visible;
	bool fps_visible;
	bool initted;
	std::uint8_t pad1[5];
	game_timer_t game_timer;
	console_t console;
	rt_font_t small_font;
	rt_font_t large_font;
	rt_font_t fixed_font;
	rt_font_t growmoji_font;
	std::deque<os_message_t> os_messages;
	std::uint8_t pad2[8];
	resource_manager_t resource_manager;
	std::uint8_t pad3[240];
};
#pragma pack(pop)
