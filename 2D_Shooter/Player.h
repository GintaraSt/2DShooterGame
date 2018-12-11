#pragma once
#ifndef PLAYER_H_
#define PLAYER_H_
#include "pch.h"
#include "Input.h"
#include <string>
#include "Player.h"
#include "Game.h"
#include <time.h>

struct symbol;
class Enemy;
#pragma message ("Player is defined")
class Player
{
protected:
	char weapons[4] = { 'k', 'N', 'N', 'N' };
	bool moveFailed = false;
	bool primary = false;
	bool secondary = false;
	int primary_ammo = 0;
	int secondary_ammo = 0;
	int coins = 0;
	char type = '?';
	int id = 7000;
	int grenades = 0;
	bool collectItems = true;
	short health = 100;
	std::string avatar = "O";
	int colour = 15;
	SYSTEMTIME st, last_st, dif, shoot_st, explosion_st;
	int positionX;
	int positionY;
	short selectedWeapon = 0;
	COORD RaycastHit(int x, int y);
	void checkTarget(symbol target, int demage, int x, int y);
	void checkCollectableItems(symbol temp);
public:
	bool won = false;
	Player(int startPositionX, int startPositionY);
	Player();
	~Player();
	short getHealth();
	void setHealth(short mHealth);
	void move(short mPositionX, short mPositionY);
	short getPositionX();
	short getPositionY();
	bool havePrimary();
	bool haveSecondary();
	int getPrimaryAmmo();
	int getSecondaryAmmo();
	int getCoins();
	int getGrenades();
	std::string getCurrentWeapon();
	void shoot(short targetX, short targetY); //TODO: Shoot Function
	void swapWeapons(bool toRight);
	void setPosition(int positionX, int positionY);
	friend void UseInputControls(Player &player);
};

class Player;
#pragma message ("Enemy is defined")


class Enemy :
	public Player
{
private:
	int mId;
	void Idle();
	void Attack(Player &player);
	void moveTowards(short dir);
	void DDLB_RandomShoot(Player &player);
public:
	Enemy();
	//~Enemy();
	static void Prepare();
	static void setDificulty(int dificultyLvl);
	void Controler(Player &player);
	short getHealth();
	void setHealth(short mHealth);
	void setId(int id);
	int getId();
};
void Swap(int &x1, int &x2, int &y1, int &y2);
bool checkShootCoordinate(symbol &temp, int &coord_x, int &coord_y, COORD &coords, bool &swap, int &x, int &y, int &positionX, int &positionY);
void UseInputControls(Player &player);
/*Enemy findEnemyById(int id);
void checkTarget(symbol target, int demage);
#pragma message ("Exited Player")*/
#endif