#pragma once
#include <cstdint>
#include <string>

#pragma pack(push, 1)
class enet_client_t {
public:
	std::uint8_t pad1[152];
	void* host;
	void* peer;
	std::uint32_t timed_out_timestamp;
	std::uint32_t unk0;
	std::uint32_t unk1;
	std::uint32_t unk2;
	std::string server_ip;
	std::uint32_t server_port;
	std::uint32_t unk3;
	std::int32_t token;
	std::int32_t user;
	std::string door_id;
	std::int32_t lmode;
	std::uint32_t unk4;
	std::string uuid_token;
};
#pragma pack(pop)
