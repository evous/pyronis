#include <utils/memory.hpp>
#include <game/gt.hpp>

#include <sstream>
#include <vector>

std::vector<std::uint8_t> string_to_bytes(std::string_view str) noexcept {
	std::vector<std::uint8_t> bytes;

	char* start = const_cast<char*>(str.data());
	char* end = start + str.size();

	for (char* c = start; c < end; ++c)
		bytes.push_back(static_cast<std::uint8_t>(std::strtoul(c, &c, 16)));

	return bytes;
}

std::vector<std::int32_t> pattern_to_bytes(std::string_view pattern) noexcept {
	std::vector<std::int32_t> bytes;

	char* start = const_cast<char*>(pattern.data());
	char* end = start + pattern.size();

	for (char* c = start; c < end; ++c) {
		if (*c == '?') {
			++c;
			bytes.push_back(-1);
		}
		else {
			bytes.push_back(static_cast<std::int32_t>(std::strtoul(c, &c, 16)));
		}
	}

	return bytes;
}

bool memory::patch_bytes(std::uintptr_t address, std::string_view string_bytes) noexcept {
	std::vector<std::uint8_t> bytes = string_to_bytes(string_bytes);
	void* dest = reinterpret_cast<void*>(address);
	DWORD old_protection = 0;

	if (!VirtualProtect(dest, bytes.size(), PAGE_EXECUTE_READWRITE, &old_protection))
		return false;

	std::memcpy(dest, bytes.data(), bytes.size());

	return VirtualProtect(dest, bytes.size(), old_protection, &old_protection);
}

std::uintptr_t memory::find_pattern(std::string_view pattern, std::intptr_t offset) noexcept {
	if (gt::base_address == 0 || gt::end_address == 0)
		return 0;

	std::vector<std::int32_t> bytes = pattern_to_bytes(pattern);

	for (std::size_t i = gt::base_address; i < gt::end_address; ++i) {
		for (std::size_t j = 0; j < bytes.size(); ++j) {
			if (*reinterpret_cast<std::uint8_t*>(i + j) != bytes[j] && bytes[j] != -1)
				break;

			if (j == bytes.size() - 1)
				return i + offset;
		}
	}

	return 0;
}


std::uintptr_t memory::get_address_from_call(std::uintptr_t address) noexcept {
	if (address == 0)
		return 0;

	if (*reinterpret_cast<std::uint8_t*>(address) != 0xE8)
		return 0;

	std::int32_t relative_address = *reinterpret_cast<std::int32_t*>(address + 1);

	return address + relative_address + sizeof(std::int32_t) + 1;
}

std::uintptr_t memory::get_address_from_load(std::uintptr_t address) noexcept {
	if (address == 0)
		return 0;

	std::int32_t relative_address = *reinterpret_cast<std::int32_t*>(address);

	return address + relative_address + sizeof(std::int32_t);
}

std::uintptr_t memory::find_function_start(std::uintptr_t address) noexcept {
	if (address == 0)
		return 0;

	while (*reinterpret_cast<std::uint16_t*>(address - 2) != 0xCCCC)
		address--;

	return address;
}