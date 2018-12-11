#pragma once
#include "pch.h"
#include "Player.h"

bool restoreBarrel = false;
float dificulty;

void Swap(int &x1, int &x2, int &y1, int &y2) {
	int temp = x1;
	x1 = x2;
	x2 = temp;
	temp = y1;
	y1 = y2;
	y2 = temp;
}

Player::Player(int startPositionX, int startPositionY)
{
	GetSystemTime(&last_st);
	positionX = startPositionX;
	positionY = startPositionY;
}

Player::Player()
{
	GetSystemTime(&last_st);
	shoot_st = last_st;
	explosion_st = last_st;
	positionX = 100;
	positionY = 25;
}

Enemy::Enemy() {
	Player();
}


Player::~Player()
{
}

void Player::move(short mPositionX, short mPositionY) {
	symbol temp;
	temp = Input::getConsoleData(mPositionX, mPositionY);
	if ((collectItems && (temp.type != 'w' && temp.type != 'r' &&  temp.type != 'x'&&  temp.type != 'e' &&  temp.type != '?')) 
		|| (!collectItems && temp.type == 'o')) {
			checkCollectableItems(temp);
			Input::consoleOut(" ", positionX, positionY, true, 'o', 0, 0, 0);
			positionX = mPositionX;
			positionY = mPositionY;
			Input::consoleOut(avatar, positionX, positionY, true, type, id, colour, 0);
			moveFailed = false;
	}
	else if(temp.type != 'x') moveFailed = true;
}

void Player::checkCollectableItems(symbol temp) {
	if (temp.type == 'p') { primary = true; weapons[1] = 'p'; }
	if (temp.type == 's') { secondary = true; weapons[2] = 's'; }
	if (temp.type == 'u') primary_ammo += 10;
	if (temp.type == 'i') secondary_ammo += 15;
	if (temp.type == 'c') coins += 50;
	if (temp.type == 'g') { grenades += 1; weapons[3] = 'g'; }
	if (temp.type == 'h') setHealth(getHealth() + 40);
	if (temp.type == '2') won = true;
}

void Player::swapWeapons(bool toRight) {
	do {
		if (toRight) selectedWeapon++;
		else selectedWeapon--;
		if (selectedWeapon > 3) selectedWeapon = 0;
		else if (selectedWeapon < 0) selectedWeapon = 3;
	} while (weapons[selectedWeapon] == 'N');
	Input::forceKeyToFalse("e");
	Input::forceKeyToFalse("q");
}

void Player::shoot(short targetX, short targetY) {
	symbol target = Input::getConsoleData(targetX, targetY);
	int systemTime = st.wMilliseconds;
	int last_systemTime = shoot_st.wMilliseconds;
	int timeDif = abs(last_systemTime - systemTime);
	//attack with knife
	if (weapons[selectedWeapon] == 'k' ) {
		if (timeDif >= 400) {
			if ((abs(targetX - positionX) <= 2) && (abs(targetY - positionY <= 2))) {
				checkTarget(target, 25, targetX, targetY);
			}
			shoot_st = st;
			Input::forceKeyToFalse("ml");
		}
	}
	//attack with pistol (primary)
	else if (weapons[selectedWeapon] == 'p') {
		COORD coords;
		if (timeDif >= 300 && primary_ammo > 0) {
			coords = RaycastHit(targetX, targetY);
			if (coords.X == -1) checkTarget(target, 20, targetX, targetY);
			else if (Input::getConsoleData(coords.X, coords.Y).type == 'e') {
				target.type = 'e';
				checkTarget(target, 20, coords.X, coords.Y);
			}

			shoot_st = st;
			primary_ammo--;
			Input::forceKeyToFalse("ml");
		}
		//todo: make clamp of 7 bullets
	}
	else if (weapons[selectedWeapon] == 's') {	//enemy
		COORD coords;
		if (timeDif >= 100 && secondary_ammo > 0) {
			coords = RaycastHit(targetX, targetY);
			if (coords.X == -1) checkTarget(target, 20, targetX, targetY);
			else if (Input::getConsoleData(coords.X, coords.Y).type == 'e') {
				target.type = 'e';
				checkTarget(target, 10, coords.X, coords.Y);
			}

			shoot_st = st;
			secondary_ammo--;
		}
		//todo: make clamp of 30 bullets
	}
	else if (weapons[selectedWeapon] == 'g') {	//enemy
		COORD coords;
		if (timeDif >= 500 && grenades > 0) {
			coords = RaycastHit(targetX, targetY);
			if (coords.X == -1) Game::destroyBarrel(targetX, targetY, *this);
			else Game::destroyBarrel(coords.X - 1, coords.Y - 1, *this);
			restoreBarrel = true;
			GetSystemTime(&explosion_st);
			shoot_st = st;
			grenades--;
		}
	}
}

//checks if theres wall or explosive barrel in bullet path,
//and if there is - returns its coordinates
COORD Player::RaycastHit(int x, int y) {
	symbol temp;
	COORD coords;
	bool swap = false;
	int coord_x, coord_y;
	int length_x, length_y;
	//find length and width of rectangle
	length_x = abs(x - positionX);
	length_y = abs(y - positionY);
	//swap player and target coordinates if needed
	if ((positionY > y && positionX > x) || ((positionY > y && positionX < x) && length_x > length_y) || ((positionY < y && positionX > x) && length_x < length_y)) {
		Swap(x, positionX, y, positionY);
		swap = true;
	}
	//starting coordinates of diagonal is equal to position of player
	coord_x = positionX;
	coord_y = positionY;
	//if starting coordinates is lesser then targets (or greater - we swaped them if greater) calculate major diagonal
	if (positionY < y && positionX < x) {
		//major_diagonal_y = (x - start_x) * ((float) rectangle_y_dim) / rectangle_x_dim - formula from stack overflow
		if (length_x > length_y) { // if rectangles length_x is greater then length_y - calculate diagonal y coordinates
			while (coord_x < x) {
				coord_y = (coord_x - positionX) * ((float)length_y) / length_x + positionY;
				if (checkShootCoordinate(temp, coord_x, coord_y, coords, swap, x, y, positionX, positionY)) return coords;
				coord_x++;
			}
		} else { // if rectangles length_x is lesser then length_y - calculate diagonal x coordinates
			while (coord_y < y) {
				coord_x = (coord_y - positionY) * ((float)length_x) / length_y + positionX;
				if (checkShootCoordinate(temp, coord_x, coord_y, coords, swap, x, y, positionX, positionY)) return coords;
				coord_y++;
			}
		}
	}
	//if one of coordinates is greater and other lesser then targets (we swaped them to fit this logic) calculate minor diagonal
	else {
		//minor_diagonal_y = rectangle_y_dim - major_diagonal_y - formula from stack overflow
		if (length_x > length_y) { // if rectangles length_x is greater then length_y - calculate diagonal y coordinates
			while (coord_x > x) {
				coord_y = length_y - (coord_x - positionX + length_x) * ((float)length_y) / length_x + positionY;
				if (checkShootCoordinate(temp, coord_x, coord_y, coords, swap, x, y, positionX, positionY)) return coords;
				coord_x--;
			}
		} else {
			while (coord_y > y) { // if rectangles length_x is lesser then length_y - calculate diagonal x coordinates
				coord_x = length_x - (coord_y - positionY + length_y) * ((float)length_x) / length_y + positionX;
				if (checkShootCoordinate(temp, coord_x, coord_y, coords, swap, x, y, positionX, positionY)) return coords;
				coord_y--;
			}
		}
	}

	if (swap) Swap(x, positionX, y, positionY);
	coords.X = -1;
	coords.Y = -1;
	return coords;
}

bool checkShootCoordinate(symbol &temp, int &coord_x, int &coord_y, COORD &coords, bool &swap, int &x, int &y, int &positionX, int &positionY) {
	temp = Input::getConsoleData(coord_x, coord_y);
	if (temp.type == 'w' || temp.type == 'e') {
		coords.X = coord_x;
		coords.Y = coord_y;
		if (swap) Swap(x, positionX, y, positionY);
		return true;
	}
	return false;
	//Input::consoleOut("H", coord_x, coord_y, true, '0', 0, 14, 14);
}

void Player::checkTarget(symbol target, int demage, int x, int y) {
	Enemy targetEnemy;
	if (target.type == 'x') {
		Game::findEnemyById(target.id, targetEnemy);
		targetEnemy.setHealth(targetEnemy.getHealth() - demage);
		if (targetEnemy.getHealth() <= 0) {
			targetEnemy.setId(-1);
			Input::consoleOut(" ", targetEnemy.getPositionX(), targetEnemy.getPositionY(), true, 'o', 0, 7, 0);
		}
		Game::setEnemyById(target.id, targetEnemy);
	}
	if (target.type == 'e') {
		Game::destroyBarrel(x, y, *this);
		restoreBarrel = true;
		GetSystemTime(&explosion_st);
	}
}

void Player::setHealth(short mHealth) {
	health = mHealth;
	if (health > 100) health = 100;
	if (health < 0) health = 0;
}
short Player::getPositionX() {
	return positionX;
}
short Player::getPositionY() {
	return positionY;
}
short Player::getHealth() {
	return health;
}
bool Player::havePrimary() {
	return primary;
}
bool Player::haveSecondary() {
	return secondary;
}
int Player::getPrimaryAmmo() {
	return primary_ammo;
}
int Player::getSecondaryAmmo() {
	return secondary_ammo;
}
int Player::getCoins() {
	return coins;
}
int Player::getGrenades() {
	return grenades;
}
std::string Player::getCurrentWeapon() {
	if (selectedWeapon == 0) return "KNIFE";
	if (selectedWeapon == 1) return "PRIMARY";
	if (selectedWeapon == 2) return "SECONDARY";
	if (selectedWeapon == 3) return "GRENADES";
}

void Enemy::setId(int id) {
	mId = id;
}

void Player::setPosition(int PositionX, int PositionY) {
	positionX = PositionX;
	positionY = PositionY;
}

int Enemy::getId() {
	return mId;
}

short Enemy::getHealth() {
	return health;
}

void Enemy::setHealth(short mHealth) {
	Player::setHealth(mHealth);
}

void Enemy::Controler(Player &player) {

	COORD coords;
	bool attackMode = false;
	coords = RaycastHit(player.getPositionX(), player.getPositionY());
	if (coords.X == -1) attackMode = true;
	else attackMode = false;

	GetSystemTime(&st);
	int systemTime = st.wMilliseconds;
	int last_systemTime = last_st.wMilliseconds;
	int timeDif = abs(last_systemTime - systemTime);
	if (timeDif >= 300) {
		if (!attackMode) {
			Idle();
		}
		else Attack(player);

		last_st = st;
	}
}

void Enemy::Idle() {
	moveFailed = true;
	avatar = char(157);
	colour = 2;
	collectItems = false;
	type = 'x';
	id = mId;
	int i = 0;
	while(moveFailed){
		short dir = rand() % 8;
		moveTowards(dir);
		if (i > 20) break; //break the cycle if move failed 20 times
		i++;
	}
	
}

void Enemy::moveTowards(short dir) {
	switch (dir)
	{
	case 0:
		move(positionX - 1, positionY);
		break;
	case 1:
		move(positionX + 1, positionY);
		break;
	case 2:
		move(positionX, positionY - 1);
		break;
	case 3:
		move(positionX, positionY + 1);
		break;
	case 4:
		move(positionX - 1, positionY - 1);
		break;
	case 5:
		move(positionX + 1, positionY + 1);
		break;
	case 6:
		move(positionX - 1, positionY + 1);
		break;
	case 7:
		move(positionX + 1, positionY - 1);
		break;
	default:
		break;
	}
}

void Enemy::Attack(Player &player) {
	avatar = char(157);
	colour = 12;
	collectItems = false;
	type = 'x';
	id = mId;
	int dir = 0;
	int targetX = player.getPositionX(), targetY = player.getPositionY();
	if (targetX > positionX) moveTowards(1);
	if (targetY > positionY) moveTowards(3);
	if (targetX < positionX) moveTowards(0);
	if (targetY < positionY) moveTowards(2);
	DDLB_RandomShoot(player);
}

void Enemy::DDLB_RandomShoot(Player &player) {
	float hitProbability = (rand() % 100) / 100.0;
	float distance = sqrt((player.getPositionX() * player.getPositionX()) + (player.getPositionY() * player.getPositionY()));
	float hitLimit = dificulty / distance;
	if (hitProbability < hitLimit) player.setHealth(player.getHealth() - 5);
}

void Enemy::setDificulty(int dificultyLvl) {
	dificulty = dificultyLvl;
}

void Enemy::Prepare() {
	srand(time(NULL));
	setDificulty(5);
}
/*
* Runs player using input controls (keyboard, mouse)
*/
void UseInputControls(Player &player){
	keys key = Input::getPressedKeys();
	bool canMove = false;
	GetSystemTime(&player.st);
	int systemTime = player.st.wMilliseconds;	
	int last_systemTime = player.last_st.wMilliseconds;
	int timeDif = abs(last_systemTime - systemTime);
	//---------------------------------
	if (timeDif >= 200) canMove = true;
	if (key.a && canMove) { 
		player.move(player.getPositionX() - 1, player.getPositionY());
		player.last_st.wMilliseconds = player.st.wMilliseconds;
	}
	if (key.d && canMove) {
		player.move(player.getPositionX() + 1, player.getPositionY());
		player.last_st.wMilliseconds = player.st.wMilliseconds;
	}
	if (key.w && canMove) {
		player.move(player.getPositionX(), player.getPositionY() - 1);
		player.last_st.wMilliseconds = player.st.wMilliseconds;
	}
	if (key.s && canMove) {
		player.move(player.getPositionX(), player.getPositionY() + 1);
		player.last_st.wMilliseconds = player.st.wMilliseconds;
	}
	if (key.q) player.swapWeapons(false);
	if (key.e) player.swapWeapons(true);
	if (key.ml) {
		player.shoot(key.x, key.y);
	}
	int last_explosionTime = player.explosion_st.wMilliseconds;
	if (restoreBarrel && abs(last_explosionTime - systemTime) >= 300) {
		restoreBarrel = false;
		Game::restoreBarrelField();
	}
	//canMove = false;
}
