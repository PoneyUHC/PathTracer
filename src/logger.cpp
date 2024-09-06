
#include "logger.hpp"

#include <sstream>
#include <chrono>
#include <iostream>


using namespace std;
using namespace std::chrono;


string Logger::GetCurrentTimeFormatted()
{
    auto now = std::chrono::system_clock::now();
    return format("[{:%T}]", zoned_time{ current_zone(), floor<milliseconds>(now)});
}


void Logger::Log(const std::string &log, const LogLevel& level)
{
    if(level < s_level){
        return;
    }

    ostringstream string_builder;
    string_builder << GetCurrentTimeFormatted() << " ";
    string_builder << s_colored_name_lookup.at(level) << " ";
    string_builder << log;
    string_builder << (s_overwrite ? '\r' : '\n');

    const string& formatted_log = string_builder.str();
    for(ostream* stream : s_outputs){
        *stream << formatted_log << flush;
    }
}
