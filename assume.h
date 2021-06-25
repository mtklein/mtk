#pragma once

#include <assert.h>

#define assume(cond) assert(cond); if (!(cond)) __builtin_unreachable()
