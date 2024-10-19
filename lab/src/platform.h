#if (defined(linux) || defined(__linux) || defined(__linux__))
#define CADE_PLATFORM_LINUX 1
#endif

#if defined(_WIN32)
#define CADE_PLATFORM_WIN32 1
#endif
