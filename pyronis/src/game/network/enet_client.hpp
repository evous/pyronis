#pragma once
#include <cstdint>
#include <string>

#pragma pack(push, 1)
class enet_client_t {
public:
	void* vftable; //0x0000
	std::uint8_t pad1[152]; //0x0008
	void* host; //0x00a0
	void* peer; //0x00a8
	std::uint32_t unk0; //0x00b0
	std::uint32_t unk1; //0x00b4
	std::uint32_t unk2; //0x00b8
	std::uint32_t unk3; //0x00bc
	char server_ip[16]; //0x00c0
	std::uint32_t unk4; //0x00d8
	std::uint32_t unk5; //0x00dc
	std::uint32_t unk6; //0x00e0
	std::uint32_t unk7; //0x00e4
	std::uint32_t server_port; //0x00e8
	std::uint32_t unk8; //0x00ec
	std::int32_t token; //0x00f0
	std::int32_t user; //0x00f4
	std::uint8_t pad2[32]; //0x00f8
	std::int32_t lmode; //0x0118
	std::uint32_t unk9; //0x011c
	char* uuid_token; //0x0120
};
#pragma pack(pop)
