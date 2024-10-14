#pragma once

/**
 * NOTES
 * `do-while (0)` encloses multi-line macro in braces to avoid contextual bugs`
 * `#` in `#expr` means convert arg `expr` to string
 */

#include "defines.h"
#include <stdio.h>

#define ASSERTIONS_ENABLED
#define DEBUG

#ifdef ASSERTIONS_ENABLED
#define CADE_ASSERT(expr)                                                      \
  do {                                                                         \
    if (!(expr)) {                                                             \
      cade_report_assertion_failure(#expr, __FILE__, __LINE__);                \
      cade_trigger_breakpoint();                                               \
    }                                                                          \
  } while (0)

#define CADE_ASSERT_MSG(expr, message)                                         \
  do {                                                                         \
    cade_report_assertion_failure_msg(#expr, message, __FILE__, __LINE__);     \
    cade_trigger_breakpoint();                                                 \
  } while (0)

#ifdef DEBUG
#define CADE_ASSERT_DEBUG(expr) CADE_ASSERT(expr)
#else
#define CADE_ASSERT_DEBUG(expr) ((void)0)
#endif
#else
#define CADE_ASSERT(expr) ((void)0)
#define CADE_ASSERT_MSG(expr, message) ((void)0)
#define CADE_ASSERT_DEBUG(expr) ((void)0)
#endif

static void cade_report_assertion_failure(const char *expression,
                                          const char *file, u32 line) {
  fprintf(stderr, "[CADE] Assertion failed: (%s), file %s, line %u.\n",
          expression, file, line);
}

static void cade_report_assertion_failure_msg(const char *expression,
                                              const char *message,
                                              const char *file, int line) {
  fprintf(stderr,
          "[CADE] Assertion failed: (%s), message: %s, file %s, line %d.\n",
          expression, message, file, line);
}

static void cade_trigger_breakpoint() {
#if defined(_MSC_VER)
  __debugbreak(); // Visual Studio
#elif defined(__GNUC__) || defined(__clang__)
  __builtin_trap(); // GCC or Clang
#else
  abort(); // Fallback
#endif
}
