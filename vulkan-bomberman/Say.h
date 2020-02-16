#pragma once

#include <iostream>

class Say
{
public:
    static void Log()
    {
        std::cout << std::endl;
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

    template <typename T, typename... Types>
    static void LogError(T var, Types... vars)
    {
        std::cout << "ERROR: ";
        Log(var, vars...);
    };

    template <typename T, typename... Types>
    static void LogWarning(T var, Types... vars)
    {
        std::cout << "WARNING: ";
        Log(var, vars...);
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
};

