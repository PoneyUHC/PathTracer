
#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>


enum class LogLevel { 
    DEBUG = 1, 
    INFO = 2, 
    WARNING = 3, 
    ERROR = 4, 
    FATAL = 5
};


class Logger {
    
private:

    static inline std::vector<std::ostream*> s_outputs = { &std::cout };
    static inline LogLevel s_level = LogLevel::DEBUG;
    static inline bool s_overwrite = false;
    
    static inline const std::unordered_map<LogLevel, std::string> s_colored_name_lookup
    {
        { LogLevel::DEBUG,   "[\x1b[94mDEBUG\x1b[0m]  " },
        { LogLevel::INFO,    "[INFO]   "         },
        { LogLevel::WARNING, "[\x1b[93mWARNING\x1b[0m]" },
        { LogLevel::ERROR,   "[\x1b[91mERROR\x1b[0m]  " },
        { LogLevel::FATAL,   "[\x1b[31mFATAL\x1b[0m]  " }
    };

    static void Log(const std::string& log, const LogLevel& level);
    static std::string GetCurrentTimeFormatted();


public:

    static void SetLogLevel(const LogLevel& level) { s_level = level; }
    static void AddOutput(std::ostream& stream) { s_outputs.push_back(&stream); }
    static void SetOverwrite(bool value) { s_overwrite = value; }
    
    static void LogDebug(const std::string& log)   { Log(log, LogLevel::DEBUG); }
    static void LogInfo(const std::string& log)    { Log(log, LogLevel::INFO); }
    static void LogWarning(const std::string& log) { Log(log, LogLevel::WARNING); }
    static void LogError(const std::string& log)   { Log(log, LogLevel::ERROR); }
    static void LogFatal(const std::string& log)   { Log(log, LogLevel::FATAL); }

};