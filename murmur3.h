#pragma once

#include <stddef.h>
#include <stdint.h>

uint32_t     mix(uint32_t seed);
uint32_t murmur3(uint32_t seed, const void*, size_t);
