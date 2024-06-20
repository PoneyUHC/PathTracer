#pragma once

#include "../utils.hpp"

#include <string>

int export_ppm(std::string filepath, int width, int height, RGBColor *buffer);