
#pragma once

#include "math/vec.hpp"

#include <string>
#include <cstdint>


std::string get_env_var(const std::string& key);

void change_endianess(uint32_t n, uint8_t *dest);

void rgb_normalized_to_8bits(const RGBColor& color, uint8_t *dest);

uint8_t rgb_normalized_to_8bits(float value);

void log_progress_bar(int progress, int total, int bar_width, bool is_last);

extern float MAGENTA[3];