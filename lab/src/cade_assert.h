/**
* NOTES
* `do-while (0)` encloses multi-line macro in braces to avoid contextual bugs`
* `#` in `#expr` means convert arg `expr` to string
*/

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
