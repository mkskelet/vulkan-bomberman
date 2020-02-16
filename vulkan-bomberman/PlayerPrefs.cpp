#include "PlayerPrefs.h"

#include "PlayerPrefs.h"
#include <sstream>

std::string PlayerPrefs::path = "../examples/player.prefs";

void PlayerPrefs::SetInt(std::string key, int value)
{
	std::ostringstream ss;
	ss << value;
	SetString(key, ss.str());
}

void PlayerPrefs::SetFloat(std::string key, float value)
{
	std::ostringstream ss;
	ss << value;
	SetString(key, ss.str());
}

void PlayerPrefs::SetString(std::string key, std::string value)
{
	std::fstream file;

	//cout << "TRYING TO SAVE KEY (" << key << ") with value " << "(" << value << ")\n";	// TEMP

	if (GetString(key, "NULL").compare("NULL") == 0)
	{
		file.open(path.c_str(), std::ios::out | std::ios::app);

		file << key << "=" << value << "\n";

		file.close();
	}
	else
	{
		std::string playerPrefs = "";
		file.open(path.c_str(), std::ios::in);

		std::string line;
		while (getline(file, line, '='))
		{
			//cout << line << "\n";
			playerPrefs += line;
			playerPrefs += "=";
			std::string l;
			getline(file, l);
			if (line.compare(key) == 0)
			{
				playerPrefs += value;
				playerPrefs += "\n";
			}
			else
			{
				playerPrefs += l;
				playerPrefs += "\n";
			}
		}

		file.close();

		file.open(path.c_str(), std::ios::out | std::ios::trunc);

		file << playerPrefs;

		file.close();
	}
}

int PlayerPrefs::GetInt(std::string key, int defaultValue)
{
	std::string s = GetString(key, "NULL");
	//cout << "GOT INT: " << s << "\n";		// TEMP
	if (s.compare("NULL") == 0) return defaultValue;
	else return atoi(s.c_str());
}

float PlayerPrefs::GetFloat(std::string key, float defaultValue)
{
	std::string s = GetString(key, "NULL");

	if (s.compare("NULL") == 0) return defaultValue;
	else return atof(s.c_str());
}

std::string PlayerPrefs::GetString(std::string key, std::string defaultValue)
{
	std::ifstream file;

	file.open(path.c_str(), std::ios::in);
	if (file.is_open())
	{
		std::string line;
		//cout << "COMPARING KEY(" << key << ") to lines:\n";	// TEMP
		while (getline(file, line, '='))
		{
			//cout << "-\t" << line << "\t- ";				// TEMP
			std::string l;			// value
			getline(file, l);
			if (line.compare(key) == 0)
			{
				//cout << "comparison successfull\n";		//TEMP

				//cout << "EXTRACTED LINE: " << l << "\n";	// TEMP
				file.close();
				return l;
			}
			//cout << "comparison failed\n";				// TEMP	
		}
		file.close();
	}

	return defaultValue;
}