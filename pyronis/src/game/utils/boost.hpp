#pragma once
#include <cstdint>

#pragma pack(push, 1)
class boost_signal_t {
public:
	std::uint8_t pad[32];
};

class boost_trackable_signal_t {
public:
	std::uint8_t pad[16];
};
#pragma pack(pop)