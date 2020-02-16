#pragma once

#include <fstream>
#include <string>
#include <cstdlib>

class PlayerPrefs
{
public:
	static std::string path;
	static void SetInt(std::string key, int value);
	static void SetFloat(std::string key, float value);
	static void SetString(std::string key, std::string value);
	static void SetBool(std::string key, bool value) { SetInt(key, value ? 1 : 0); }
	static int GetInt(std::string key, int defaultValue);
	static float GetFloat(std::string key, float defaultValue);
	static std::string GetString(std::string key, std::string defaultValue);
	static bool GetBool(std::string key, bool defaultValue) { return (GetInt(key, defaultValue ? 1 : 0) == 1 ? true : false); }
};
