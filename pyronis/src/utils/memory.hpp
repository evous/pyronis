#pragma once
#include <cstdint>
#include <string_view>
#include <type_traits>

namespace memory {
	template <typename T, typename = void>
	struct get_underlying {
		using type = T;
	};

	template <typename T>
	struct get_underlying<T, std::enable_if_t<std::is_enum_v<T>>> {
		using type = std::underlying_type_t<T>;
	};

	template <typename value_type, typename flag_type>
	inline void remove_flag(value_type& value, flag_type flag) noexcept {
		using type = get_underlying<flag_type>::type;
		value = static_cast<value_type>(static_cast<type>(value) & ~(static_cast<type>(flag)));
	}

	template <typename value_type, typename flag_type>
	inline void add_flag(value_type& value, flag_type flag) noexcept {
		using type = get_underlying<flag_type>::type;
		value = static_cast<value_type>(static_cast<type>(value) | (static_cast<type>(flag)));
	}

	template <typename value_type, typename flag_type>
	inline void toggle_flag(value_type& value, flag_type flag) noexcept {
		using type = get_underlying<flag_type>::type;
		value = static_cast<value_type>(static_cast<type>(value) ^ (static_cast<type>(flag)));
	}

	template <typename value_type, typename flag_type>
	inline bool has_flag(value_type value, flag_type flag) noexcept {
		using type = get_underlying<flag_type>::type;
		return (static_cast<type>(value) & static_cast<type>(flag)) != 0;
	}

	template <typename value_type>
	inline void append_read(value_type& value, std::uint8_t*& data) noexcept {
		value = *reinterpret_cast<value_type*>(data);
		data += sizeof(value_type);
	}

	template <typename size_type = std::uint32_t>
	inline void append_read_string(std::string& str, std::uint8_t*& data) noexcept {
		size_type size;
		append_read(size, data);
		str = std::string{ reinterpret_cast<char*>(data), size };
		data += size;
	}

	template <typename value_type>
	inline void append_write(value_type value, std::uint8_t*& data) noexcept {
		*reinterpret_cast<value_type*>(data) = value;
		data += sizeof(value_type);
	}

	template <typename size_type = std::uint32_t>
	inline void append_write_string(const std::string& str, std::uint8_t*& data) noexcept {
		append_write(static_cast<size_type>(str.size()), data);
		std::memcpy(data, str.data(), str.size());
		data += str.size();
	}

	bool patch_bytes(std::uintptr_t address, std::string_view bytes) noexcept;
	std::uintptr_t find_pattern(std::string_view pattern, std::intptr_t offset = 0) noexcept;
	std::uintptr_t get_address_from_call(std::uintptr_t address) noexcept;
	std::uintptr_t get_address_from_load(std::uintptr_t address) noexcept;
	std::uintptr_t find_function_start(std::uintptr_t address) noexcept;
}