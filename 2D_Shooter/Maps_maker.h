#pragma once
#ifndef MAPS_MAKER_H_
#define MAPS_MAKER_H_
#include "pch.h"
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <thread>
#include <chrono>
#include <fstream>
#include "time.h"
#include <fstream>
#include "Player.h"
#include "Game.h"
#include "Interface.h"

#pragma message ("Entered maps maker")
class mapCreator {
private:
	char Map[145][45];
	char symbols[14] = { char(219) ,char(177) ,char(167) ,char(217) ,char(37) ,char(240) ,char(158) ,char(36) ,char(42) ,char(157) ,char(32), char(72), char(62), char(60) };
	char types[14] = { 'w', 'r', 'e', 'p', 's', 'u', 'i', 'c', 'g', 'x', 'o', 'h', '1', '2' };
	int ids[14] = { 2500, 2501, 2502, 2503, 2504, 2505, 2506, 2507, 2508, 2509, 2510, 2569, 2570, 2571 };
	int colors[14] = {8, 9, 4, 6, 3, 6, 3, 14, 5, 12, 0, 14, 7, 13};
	int coLine = 0;
	int number_of_enemies = 0;
	std::string last_message;
	std::string consoleBuffer[6];
	void welcomeActivity();
	void newMapActivity(bool useEmptyMap = true);
	void loadMapActivity(bool startGame, bool callFromWelcome = false);
	std::string mapCreatorConsole(std::string text, bool  getInput);
	void saveMap();
	void readMap(std::string mapName);
	char loadMap(short number_of_maps, std::string* temp, bool editor = true);
public:
	mapCreator();
	void displayMap(short x, short y, int &enemiesCounter);
	void displayMap(short x, short y);
	void Interface();
	void emptyMap();
	int getNumberOfEnemies();
};

#pragma message ("Exited maps maker")
#endif