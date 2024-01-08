#include <cstdint>
#include <d3d9.h>

#pragma pack(push, 1)
struct texture_d3d9_t {
	union {
		IDirect3DBaseTexture9* ptr;
		IDirect3DTexture9* texture2d;
		IDirect3DVolumeTexture9* texture3d;
		IDirect3DCubeTexture9* texture_cube;
	};

	IDirect3DSurface9* surface;

	union {
		IDirect3DBaseTexture9* staging;
		IDirect3DTexture9* staging2d;
		IDirect3DVolumeTexture9* staging3d;
		IDirect3DCubeTexture9* staging_cube;
	};

	std::uint64_t flags;
	std::uint32_t width;
	std::uint32_t height;
	std::uint32_t depth;
	std::uint8_t num_mips;
	std::uint8_t type;
	std::uint8_t requested_format;
	std::uint8_t texture_format;
};

class renderer_context_d3d9_t {
public:
	std::uint8_t pad1[312];
	IDirect3D9Ex* d3d9_ex;
	IDirect3DDevice9Ex* device_ex;
	IDirect3D9* d3d9;
	IDirect3DDevice9* device;
	std::uint8_t pad2[423592];
	texture_d3d9_t textures[4 << 10]; // BGFX_CONFIG_MAX_TEXTURES
};
#pragma pack(pop)