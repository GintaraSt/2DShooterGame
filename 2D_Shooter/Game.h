#pragma once
#ifndef GAME_H_
#define GAME_H_
#include "pch.h"
#include "Interface.h"
#include "Maps_maker.h"
#include "Input.h"
#include "Player.h"
#include <vector>

/*calss mapCreator is used to solve circular dependency
it appears because mapCreator class uses Game class objects
and Game class uses mapsCreator objects*/
class mapCreator;
class Player;
class Enemy;

#pragma message ("Entered Game")
class Game
{
private:
	static void Interface();
	static void assignIdsToEnemies(int number_of_enemies);
	static void showCrosshair();
	static void UpdateHealthBar(int health);
	static void controlEnemies(Player &player);
public:
	Game();
	~Game();
	static mapCreator getMap();
	static void GameOver(mapCreator &map, Player &player, bool lost = true);
	static void destroyBarrel(int x, int y, Player &player);
	static void restoreBarrelField();
	static bool checkGUI(mapCreator &map, Player &player);
	static void gameActivity(mapCreator &map);
	static void runGame(mapCreator &map);
	static void prepareForGame();
	static void findEnemyById(int id, Enemy &enemy);
	static void setEnemyById(int id, Enemy enemy);
};
#pragma message ("Exited Game")
#endif