#pragma once

#include <iostream>
#include <type_traits>

enum LogType {Log, Info, Warning, Error};

class Say
{
private:
    static void Log()
    {
        std::cout << std::endl;
    };

public:
    template <typename... Types>
    static void Log(bool var, Types... vars)
    {
        Log(var ? "true" : "false", vars...);
    };

    template <typename T, typename... Types>
    static void Log(T var, Types... vars)
    {
        std::cout << var;

        if (sizeof...(vars) > 0)
        {
            std::cout << " ";
        }

        Log(vars...);
    };

    template <typename... Types>
    static void Log(LogType logType, Types... vars)
    {
        switch (logType)
        {
        case Info:
            std::cout << "INFO: ";
            break;
        case Warning:
            std::cout << "WARNING: ";
            break;
        case Error:
            std::cout << "ERROR: ";
            break;
        default:
            break;
        }

        Log(vars...);
    };

    template <typename T, typename... Types>
    static void LogError(T var, Types... vars)
    {
        Log(Error, var, vars...);
    };

    template <typename T, typename... Types>
    static void LogWarning(T var, Types... vars)
    {
        Log(Warning, var, vars...);
    };

    template <typename T, typename... Types>
    static void LogInfo(T var, Types... vars)
    {
        Log(Info, var, vars...);
    };

    // conditional logging
    template <typename... Types>
    static void LogIf(bool condition, Types... vars)
    {
        if (condition)
        {
            Log(vars...);
        }
    };

    template <typename... Types>
    static void LogErrorIf(bool condition, Types... vars)
    {
        if (condition)
        {
            LogError(vars...);
        }
    };

    template <typename... Types>
    static void LogWarningIf(bool condition, Types... vars)
    {
        if (condition)
        {
            LogWarning(vars...);
        }
    };

    template <typename... Types>
    static void LogInfoIf(bool condition, Types... vars)
    {
        if (condition)
        {
            LogInfo(vars...);
        }
    };
};

