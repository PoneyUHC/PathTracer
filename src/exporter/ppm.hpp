#pragma once

#include "../color.hpp"

#include <string>
#include <memory>

int export_ppm(std::string filepath, int width, int height, std::unique_ptr<RGBColor[]> buffer);