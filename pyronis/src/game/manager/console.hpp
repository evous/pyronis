#pragma once
#include <game/utils/boost.hpp>

#include <cstdint>
#include <deque>
#include <string>

#pragma pack(push, 1)
class console_t {
public:
	void* vftable;
	boost_signal_t sig_on_text_added;
	std::uint32_t max_lines;
	std::deque<std::string> log;
	std::uint8_t pad1[84];
};
#pragma pack(pop)