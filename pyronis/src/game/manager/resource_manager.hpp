#pragma once
#include <map>
#include <string>

enum resource_type {
	unknown,
	surface,
	sprite_animation_set
};

class surface_t;

#pragma pack(push, 1)
class resource_t {
public:
	surface_t* surface;
	resource_type type;
};

class resource_manager_t {
public:
	virtual ~resource_manager_t();

	std::map<std::string, resource_t*> data;
};
#pragma pack(pop)