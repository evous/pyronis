#pragma once
#include <iostream>
#include <string_view>
#include <format>

namespace console {
    enum class color {
        black = 30,
        dark_red = 31,
        dark_green = 32,
        dark_yellow = 33,
        dark_blue = 34,
        dark_magenta = 35,
        dark_cyan = 36,
        red = 91,
        green = 92,
        yellow = 93,
        blue = 94,
        magenta = 95,
        cyan = 96,
        white = 97
    };

	template <color c = color::white>
    inline void print(std::string_view text) noexcept { // todo: add formatting
		std::cout << "\x1b[" << static_cast<int>(c) << "m" << text << "\x1b[0m";
	}

    template <color c = color::white>
    inline void println(std::string_view text) noexcept {
        print<c>(std::format("{}\n", text));
    }

    inline void print_good(std::string_view text) noexcept {
        print<color::dark_cyan> ("[+] ");
        println<color::white>(text);
    }
    
    inline void print_error(std::string_view text) noexcept {
        print<color::red>("error: ");
        println<color::white>(text);
    }

    inline void setup() noexcept {
        AllocConsole();
        freopen("CONOUT$", "w", stdout);

        SetConsoleTitle("pyronis console");

        // enable ansi colors
        HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD mode = 0;
        GetConsoleMode(console, &mode);
        SetConsoleMode(console, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
}