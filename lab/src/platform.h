#if (defined(linux) || defined(__linux) || defined(__linux__))
#define CADE_PLATFORM_LINUX 1
#endif

#if defined(_WIN32)
#define CADE_PLATFORM_WIN32 1
#endif

// https://github.com/KhronosGroup/Vulkan-Loader/blob/main/docs/LoaderDriverInterface.md

// https://github.com/libsdl-org/SDL/blob/main/src/video/x11/SDL_x11vulkan.c#L172
// Create surface using device and window
void platform_create_surface();


void platform_pump_events();
