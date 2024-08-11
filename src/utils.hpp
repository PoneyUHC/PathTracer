
#pragma once

#include <string>

#include <cstdint>


std::string get_env_var(const std::string& key);

void change_endianess(uint32_t n, uint8_t *dest);