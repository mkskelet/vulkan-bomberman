#pragma once

//  Map constants
#define BLOCK_SPACE 100
#define BLOCK_SOLID 101
#define BLOCK_EXPLODABLE 102
#define BLOCK_PLAYER 103

#include <string>
#include <vector>

class Map
{
private:
	std::string name;
	std::vector<int> mapBlocks;
	int xSize;
	int ySize;
	bool singlePlayer;		///< Is map single player or not.
	int bombPowerups;		///< Amount of bomb powerups spawning on current map.
	int speedPowerups;		///< Amount of speed powerups spawning on current map.
	int flamePowerups;		///< Amount of flame powerups spawning on current map.
	int remotePowerups;	///< Amount of RP's spawning on current map.
	int timeLimit;		///< time limit for completing map, default 180 set in constructor initializer list
public:
	Map() : name("NULL"), xSize(0), ySize(0), singlePlayer(true), bombPowerups(0), speedPowerups(0), flamePowerups(0), remotePowerups(0), timeLimit(180) {};
	// SETTERS
	void SetName(std::string n) { name = n; }
	void SetMapBlocks(std::vector<int> blocks) { mapBlocks = blocks; }
	void SetX(int x) { xSize = x; }
	void SetY(int y) { ySize = y; }
	void SetSinglePlayer(bool single) { singlePlayer = single; }
	void SetBombPowerups(int bp) { bombPowerups = bp; }
	void SetSpeedPowerups(int sp) { speedPowerups = sp; }
	void SetFlamePowerups(int fp) { flamePowerups = fp; }
	void SetRemotePowerups(int rp) { remotePowerups = rp; }
	void SetTimeLimit(int t) { timeLimit = t; }					///

	//GETTERS
	std::string GetName() const { return name; }
	const int operator[](int index) const;
	int GetX() const { return xSize; }
	int GetY() const { return ySize; }
	int GetSize() const { return xSize * ySize; }
	bool GetSinglePlayer() const { return singlePlayer; }
	int GetBombPowerups() const { return bombPowerups; }
	int GetSpeedPowerups() const { return speedPowerups; }
	int GetFlamePowerups() const { return flamePowerups; }
	int GetRemotePowerups() const { return remotePowerups; }
	int GetTimeLimit() const { return timeLimit; }
};