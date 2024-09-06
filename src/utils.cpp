
#include "utils.hpp"

#include "math/interval.hpp"
#include "logger.hpp"

#include <algorithm>
#include <sstream>
#include <format>


using namespace std;


float MAGENTA[3] = { 1.0f, 0.0f, 1.0f };


string get_env_var(const string& key)
{
    char* val = getenv(key.c_str());
    if(val == NULL){
        Logger::LogError("Could not retrieve environment variable with key '" + key + "'"); 
    }
    return val == NULL ? string("") : string(val);
}


void change_endianess(uint32_t n, uint8_t *dest)
{
    uint8_t *data_ptr = (uint8_t*) &n;

    for(ulong i=0; i<sizeof(uint32_t); ++i){
        dest[sizeof(uint32_t) - i - 1] = data_ptr[i];
    }
}


void rgb_normalized_to_8bits(const RGBColor& color, uint8_t *dest){
    static const Interval intensity(0.000, 0.999);
    dest[0] = uint8_t(256 * intensity.Clamp(color[0]));
    dest[1] = uint8_t(256 * intensity.Clamp(color[1]));
    dest[2] = uint8_t(256 * intensity.Clamp(color[2]));
}


uint8_t rgb_normalized_to_8bits(float value){
    return static_cast<uint8_t>(255.999 * std::clamp(value, 0.f, 1.f));
}


void log_progress_bar(int progress, int total, int bar_width, bool is_last)
{
    Logger::SetOverwrite(true);

    stringstream string_builder;

    float progress_ratio = float(progress)/total;
    int pos = bar_width * progress_ratio;

    string_builder << format("[{}/{}] [", progress, total);
    for (int i = 0; i < bar_width; ++i) {
        if (i < pos) string_builder << "=";
        else if (i == pos) string_builder << "-";
        else string_builder << "_";
    }
    string_builder << format("] {:.2f} %", progress_ratio*100);

    if(is_last) {
        Logger::SetOverwrite(false);
    }

    Logger::LogInfo(string_builder.str());
}
