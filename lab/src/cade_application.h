#pragma once

#include "defines.h"

typedef struct cade_window_t {
  u16 width;
  u16 height;
} cade_window_t;

typedef struct cade_context_t {
  cade_window_t window;
} cade_context_t;
