#include "pch.h"
#include "Game.h"

int last_pAmmo, last_sAmmo, last_health, last_grenades, last_coins;
std::string last_weapon = "KNIFE";
bool last_p, last_s;
bool pauseGame = false;
int lastCursorX=0, cursorX;
int lastCursorY=0, cursorY;
const HDC console = GetDC(ConsoleWindow);
std::vector<Enemy> Enemies;

Game::Game()
{
	mapCreator myMap;
	lastCursorY = 100;
	lastCursorX = 100;
	cursorY = 100;
	cursorX = 100;
	Enemies.reserve(20);
	myMap = mapCreator();
}


Game::~Game()
{
}

void Game::runGame(mapCreator &map) {
	//find players start point
	int x, y;
	for (int i = 0; i < 145; i++) {
		for (int j = 0; j < 45; j++) {
			if (Input::getConsoleData(20 + i, j).type == '1') {
				x = 20 + i;
				y = j;
			}
		}
	}
	Player player = Player(x, y);
	Enemy::Prepare();
	pauseGame = false;
	while (true) {
		checkGUI(map, player);
		if(!pauseGame) UseInputControls(player);
		controlEnemies(player);
	}

}

void Game::GameOver(mapCreator &map, Player &player, bool lost) {
	std::ifstream file;
	if (lost) file.open("GameOverActivity.mapf", std::ios::binary);
	else file.open("GameWonActivity.mapf", std::ios::binary);
	if (!file.is_open()) { Interface::addBox(93, 20, 20, 10, "ERROR - failed to launch gameOverActivity", "center", true, 10157, 'M', 12, 1, 12); system("pause"); return; }
	char temp;
	for (int i = 0; i < 45; i++) {
		for (int j = 0; j < 145; j++) {
			file >> temp;
			std::string sTemp;
			sTemp += temp;
			if (temp == char(157)) Input::consoleOut(sTemp, 20 + j, i, true, '0', 0, 12, 4);
			else if(temp == char(42)) Input::consoleOut(sTemp, 20 + j, i, true, '0', 0, 14, 7);
			else Input::consoleOut(sTemp, 20 + j, i, true, '0', 0, 3, 4);
		}
	}
	while (true) {
		checkGUI(map, player);
		//GUI bugs occur
	}
}

void Game::Interface() {
	Interface::addBox(0, 0, 19, 8, "EXIT GAME", "center", true, 4000, 'b', 12, 0, 12);
	Interface::addBox(0, 12, 19, 8, "PAUSE", "center", true, 4001, 'b', 10, 0, 11);
	Interface::addBox(0, 24, 19, 8, "RESTART", "center", true, 4002, 'b', 10, 0, 11);
	//if hit update health bar and if found bullets update bullet count
	Interface::addBox(20, 45, 105, 5, "", "center", true, 1, 'M', 0, 0, 3);

	Interface::addBox(22, 46, 8, 1, "health:", "left", false, 4500, 't', 10);
	Interface::addBox(31, 46, 10, 1, "||||||||||", "left", false, 4501, 't', 10, 2);

	Interface::addBox(44, 46, 15, 1, "Primary ammo: ", "left", false, 4502, 't', 2);
	Interface::addBox(59, 46, 4, 1, "0", "left", false, 4503, 't', 12);

	Interface::addBox(44, 48, 14, 1, "Primary: ", "left", false, 4504, 't', 2);
	Interface::addBox(59, 48, 11, 1, "unequipped", "left", false, 4505, 't', 12);

	Interface::addBox(71, 46, 17, 1, "Secondary ammo: ", "left", false, 4506, 't', 2);
	Interface::addBox(88, 46, 4, 1, "0", "left", false, 4507, 't', 12);

	Interface::addBox(71, 48, 14, 1, "Secondary: ", "left", false, 4504, 't', 2);
	Interface::addBox(88, 48, 11, 1, "unequipped", "left", false, 4505, 't', 12);

	Interface::addBox(102, 46, 11, 1, "Grenades: ", "left", false, 4506, 't', 2);
	Interface::addBox(114, 46, 4, 1, "0", "left", false, 4507, 't', 12);

	Interface::addBox(102, 48, 11, 1, "Coins: ", "left", false, 4504, 't', 2);
	Interface::addBox(114, 48, 10, 1, "0", "left", false, 4505, 't', 12);

	Interface::addBox(126, 45, 12, 5, "KNIFE", "center", true, 1, 't', 7, 0, 5);
}

void Game::gameActivity(mapCreator &map) {
	Input::clearConsole();
	Interface();
	int number_of_enemies = 0;
	map.displayMap(20, 0, number_of_enemies);
	assignIdsToEnemies(number_of_enemies);
	runGame(map);
}

void Game::prepareForGame() {
	Input::fullScreen(true);
	Input::adjustWindowRes();
	Input::fullScreen(false);
	Input::fullScreen(true);
	Input::showConsoleCursor(false);
}

bool Game::checkGUI(mapCreator &map, Player &player) {
	keys key;
	key = Input::getPressedKeys();
	if (key.GUI == 4000 && key.ml) {
		map = mapCreator();
		return false;
	}
	if (key.GUI == 4001 && key.ml) {
		pauseGame = !pauseGame;
		Input::forceKeyToFalse("ml");
		return true;
	}
	if (key.GUI == 4002 && key.ml) {
		gameActivity(map);
		return false;
	}
	if (last_p != player.havePrimary()) {
		last_p = !last_p;
		Interface::addBox(59, 48, 11, 1, "equipped  ", "left", false, 4505, 't', 10);
	}
	if (last_s != player.haveSecondary()) {
		last_s = !last_s;
		Interface::addBox(88, 48, 11, 1, "equipped  ", "left", false, 4505, 't', 10);
	}
	int temp = player.getPrimaryAmmo();
	if (last_pAmmo != temp) {
		last_pAmmo = temp;
		if(temp == 0) Interface::addBox(59, 46, 4, 1, "0", "left", false, 4503, 't', 12);
		else Interface::addBox(59, 46, 4, 1, std::to_string(temp), "left", false, 4503, 't', 10);
	}
	temp = player.getSecondaryAmmo();
	if (last_sAmmo != temp) {
		last_sAmmo = temp;
		if (temp == 0) Interface::addBox(88, 46, 4, 1, "0", "left", false, 4507, 't', 12);
		else Interface::addBox(88, 46, 4, 1, std::to_string(temp), "left", false, 4507, 't', 10);
	}
	temp = player.getGrenades();
	if (last_grenades != temp) {
		last_grenades = temp;
		if (temp == 0) Interface::addBox(114, 46, 4, 1, "0", "left", false, 4507, 't', 12);
		else Interface::addBox(114, 46, 4, 1, std::to_string(temp), "left", false, 4507, 't', 10);
	}
	temp = player.getCoins();
	if (last_coins != temp) {
		last_coins = temp;
		if (temp == 0) Interface::addBox(114, 48, 10, 1, "0", "left", false, 4505, 't', 12);
		else Interface::addBox(114, 48, 10, 1, std::to_string(temp), "left", false, 4505, 't', 10);
	}
	std::string weapon = player.getCurrentWeapon();
	if (last_weapon != weapon) {
		Interface::addBox(126, 45, 12, 5, weapon, "center", true, 1, 't', 7, 0, 5);
		last_weapon = weapon;
	}
	temp = player.getHealth();
	if (last_health != temp) {
		UpdateHealthBar(temp);
		if (temp <= 0) GameOver(map, player);
		last_health = temp;
	}
	if(player.won) GameOver(map, player, false);
}

void Game::UpdateHealthBar(int health) {
	std::string temp;
	int colour = 10;
	int temp_health = 0;
	while (health > temp_health) {
			temp_health += 10;
			temp += '|';
	}
	if (health >= 40 && health <= 70) colour = 14;
	else if (health < 40) colour = 12;
	Interface::addBox(31, 46, 10, 1, "||||||||||", "left", false, 4501, 't', 0, 0);
	Interface::addBox(31, 46, temp.length(), 1, temp, "left", false, 4501, 't', colour, colour);
}

void Game::assignIdsToEnemies(int number_of_enemies) {
	Enemies.resize(number_of_enemies + 1);
	symbol temp;
	int k = 0;
	for (int i = 0; i < 213; i++) {
		for (int j = 0; j < 53; j++) {
			temp = Input::getConsoleData(i, j);
			if (temp.id >= 7001) {
				Enemies[k].setId(temp.id);
				Enemies[k].setPosition(i, j);
				k++;
				if (k >= Enemies.size()) {
					MessageBox(nullptr, TEXT("uknown error occurred:\nGame.cpp --> assignIdsToEnemies()\n\nEnemies.size() is smaller then number\n of enemies in loaded map"), TEXT("ERROR: "), MB_OK);
					exit(-75);
				}
			}
		}
	}
}

void Game::findEnemyById(int id, Enemy &enemy) {
	if (Enemies.size() <= 0) return;
	for (int i = 0; i < Enemies.size() - 1; i++) {
		if (Enemies.at(i).getId() == id) {
			enemy = Enemies.at(i);
			break;
		}
	}
	return;
}

void Game::setEnemyById(int id, Enemy enemy) {
	if (Enemies.size() <= 0) return;
	for (int i = 0; i < Enemies.size() - 1; i++) {
		if (Enemies.at(i).getId() == id) {
			Enemies.at(i) = enemy;
			break;
		}
	}
	return;
}


//buffer for barels explosion and area restoration
std::vector<extendedSymbol> barrelBuffer;
/*destroys barrel and barrels that are in range of its fire
(this function starts chain reaction if theres a chain of barrels in map),
also decreases enemys or players health if in range of fire by 60*/
void Game::destroyBarrel(int x, int y, Player &player) {
	bool first_run = true;
	extendedSymbol temp;
	std::string fire;
	fire += char(176);
	int size = 4;
	int color = 4;
	for (int k = 0; k < 2; k++) {
		for (int i = x - size; i <= x + size; i++) {
			for (int j = y - size/2; j <= y + size/2; j++) {
				if (first_run) {
					if (!(x == i && y == j)) temp.main = Input::getConsoleData(i, j);
					else {
						temp.main.symbol = ' ';
						temp.main.type = '0';
						temp.main.id = 0;
						temp.main.color = 0;
					}
					temp.x = i; temp.y = j;
					if(temp.main.type != 'z') barrelBuffer.push_back(temp);
				}
				Input::consoleOut(fire, i, j, true, 'z', 0, color, 0);
			}
		}
		first_run = false;
		size -= 2;
		color = 12;
	}
	Input::consoleOut(fire, x, y, true, 'z', 0, 14, 0);

	for (int i = 0; i < barrelBuffer.size(); i++) {
		if (barrelBuffer.at(i).main.type == 'e') {
			barrelBuffer.at(i).main.type = '0';
			destroyBarrel(barrelBuffer.at(i).x, barrelBuffer.at(i).y, player);
		}
		if (barrelBuffer.at(i).main.type == 'x') {
			Enemy targetEnemy;
			findEnemyById(barrelBuffer.at(i).main.id, targetEnemy);
			targetEnemy.setHealth(targetEnemy.getHealth() - 60);
			if (targetEnemy.getHealth() <= 0) {
				targetEnemy.setId(-1);
				barrelBuffer.at(i).main.symbol = ' ';
			}
			Game::setEnemyById(barrelBuffer.at(i).main.id, targetEnemy);
		}
		if (barrelBuffer.at(i).main.type == '?') {
			player.setHealth(player.getHealth() - 40);
		}
	}
}

void Game::restoreBarrelField() {
	for (int i = 0; i < barrelBuffer.size(); i++) {
		std::string text;
		text += barrelBuffer.at(i).main.symbol;
		Input::consoleOut(text, barrelBuffer.at(i).x, barrelBuffer.at(i).y, true, barrelBuffer.at(i).main.type, barrelBuffer.at(i).main.id, barrelBuffer.at(i).main.color, 0);
	}
	barrelBuffer.erase(barrelBuffer.begin(), barrelBuffer.end());
}

void Game::controlEnemies(Player &player) {
	if (Enemies.size() <= 0) return;
	for (int i = 0; i < Enemies.size() - 1; i++) {
		if (Enemies.at(i).getId() > 0) Enemies.at(i).Controler(player);
	}
	return;
}

/*move to graphics class???*/
void Game::showCrosshair() {
	//TODO: FIX IT
	tagPOINT pos;
	COLORREF lastPixelsColor[2][11];
	GetCursorPos(&pos);
	cursorX = pos.x;
	cursorY = pos.y;
	if ((lastCursorX != cursorX) && (lastCursorY != cursorY)) {
		int k = 0;
		for (int i = lastCursorX - 5; i <= lastCursorX + 5; i++) {
			SetPixel(console, i, lastCursorY, lastPixelsColor[0][k]);
			k++;
		}
		k = 0;
		for (int i = lastCursorY - 5; i <= lastCursorY + 5; i++) {
			SetPixel(console, lastCursorX, i, lastPixelsColor[1][k]);
			k++;
		}
		k = 0;
		for (int i = cursorX - 5; i <= cursorX + 5; i++) {
			lastPixelsColor[0][k] = GetPixel(console, i, cursorY);
			SetPixel(console, i, cursorY, RGB(255, 0, 0));
			k++;
		}
		k = 0;
		for (int i = cursorY - 5; i <= cursorY + 5; i++) {
			lastPixelsColor[1][k] = GetPixel(console, cursorX, i);
			SetPixel(console, cursorX, i, RGB(255, 0, 0));
			k++;
		}
		lastCursorX = cursorX;
		lastCursorY = cursorY;
	}
	return;
}
