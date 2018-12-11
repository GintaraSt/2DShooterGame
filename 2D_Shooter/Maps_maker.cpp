#include "pch.h"
#include "Maps_maker.h"
#pragma once

//int enemiesIdsList[1000];

mapCreator::mapCreator()
{
	welcomeActivity();
}

void mapCreator::displayMap(short x, short y) {
	int enemiesCounter = 0;
	displayMap( x, y, enemiesCounter);
}

void mapCreator::displayMap(short x, short y, int &enemiesCounter) {
	enemiesCounter = 0;
	std::string temp;
	for (int i = 0; i < 145; i++)//row
	{
		for (int j = 0; j < 45; j++)//col
		{
			temp = Map[i][j];
			for (int k = 0; k < 14; k++) {
				if (Map[i][j] == symbols[k]) {
					if (k == 9) {
						Input::consoleOut(temp, x + i, y + j, true, types[k], 7001 + enemiesCounter, colors[k], 0);
						enemiesCounter++;
					} 
					else Input::consoleOut(temp, x + i, y + j, true, types[k], ids[k], colors[k], 0);
				}
			}
		}
		std::cout << std::endl;
	}
}

void mapCreator::Interface() {
	Interface::addBox(0, 0, 40, 8, "MAPS MAKER", "center", true, 2000, 't', 15, 0, 4);
	Interface::addBox(0, 9, 19, 5, "Add Wall", "center", true, 2001, 'b', 15, 0, 6);
	Interface::addBox(20, 9, 19, 5, "Add River", "center", true, 2002, 'b', 15, 0, 6);
	Interface::addBox(0, 15, 19, 5, "   Add Explosive Barrel", "center", true, 2003, 'b', 15, 0, 6);
	Interface::addBox(20, 15, 19, 5, "Add Primary Weapon", "center", true, 2004, 'b', 15, 0, 6);
	Interface::addBox(0, 21, 19, 5, "   Add Secondary Weapon", "center", true, 2005, 'b', 15, 0, 6);
	Interface::addBox(20, 21, 19, 5, "   Add Primary   Bullets", "center", true, 2006, 'b', 15, 0, 6);
	Interface::addBox(0, 27, 19, 5, "  Add Secondary  Bullets", "center", true, 2007, 'b', 15, 0, 6);
	Interface::addBox(20, 27, 19, 5, "Add Coins", "center", true, 2008, 'b', 15, 0, 6);
	Interface::addBox(0, 33, 19, 5, "Add Grenade", "center", true, 2009, 'b', 15, 0, 6);
	Interface::addBox(20, 33, 19, 5, "Add Enemy", "center", true, 2010, 'b', 15, 0, 5);
	Interface::addBox(0, 39, 19, 5, "Health Kit", "center", true, 2070, 'b', 15, 0, 5);
	Interface::addBox(0, 45, 19, 5, "Entrance", "center", true, 2071, 'b', 15, 0, 5);
	Interface::addBox(20, 45, 19, 5, "Exit", "center", true, 2072, 'b', 15, 0, 5);
	Interface::addBox(20, 39, 19, 5, "Eraser", "center", true, 2011, 'b', 15, 0, 5);
	Interface::addBox(120, 45, 14, 7, "Save Map", "center", true, 2012, 'b', 15, 0, 4);
	Interface::addBox(135, 45, 14, 7, "Load Map", "center", true, 2013, 'b', 15, 0, 4);
	Interface::addBox(150, 45, 14, 7, "Reset Map", "center", true, 2014, 'b', 15, 0, 4);
	//Interface::addBox(0, 46, 14, 4, "RUN TEST", "center", true, 2015, 'b', 10, 0, 10);
	Interface::addBox(165, 45, 20, 7, "EXIT MAPS MAKER", "center", true, 2016, 'b', 15, 0, 4);
	Interface::addBox(41, 45, 78, 7, "CONSOLE", "center", true, 2016, 'b', 2, 0, 4);
}

void mapCreator::emptyMap() {
	//fill first and last line with double walls
	for (int j = 0; j < 45; j++)
	{
		Map[0][j] = symbols[0];
		Map[1][j] = symbols[0];
		Map[143][j] = symbols[0];
		Map[144][j] = symbols[0];
	}
	for (int i = 2; i < 143; i++)//row
	{
		//fill first, and last 2 columns of row  with double walls
		Map[i][0] = symbols[0];
		Map[i][44] = symbols[0];
		//fill all map space with empty spaces
		for (int j = 1; j < 44; j++)//col
		{
			Map[i][j] = symbols[10];
		}
	}
}

std::string mapCreator::mapCreatorConsole(std::string text, bool  getInput) {
	short text_size = text.length();
	std::string input_text = "";
	if (last_message != text) {
		text.resize(76, ' ');
		if (coLine <= 5) {
			Input::consoleOut(text, 42, 46 + coLine, true, 'm', 0, 2, 0);
			if (getInput) {
				Input::setCursorPosition(43 + text_size, 46 + coLine);
				std::cin >> input_text;
				if (text_size + input_text.length() > 76) Interface();
			}
			text.resize(text_size);
			consoleBuffer[coLine] = text + input_text;
			consoleBuffer[coLine].resize(76, ' ');
			coLine++;
		}
		else {
			for (int i = 0; i < 5; i++) {
				consoleBuffer[i] = consoleBuffer[i + 1];
				Input::consoleOut(consoleBuffer[i], 42, 46 + i, true, 'm', 0, 2, 0);
			}
			Input::consoleOut(text, 42, 51, true, 'm', 0, 2, 0);
			if (getInput) {
				Input::setCursorPosition(43 + text_size, 51);
				std::cin >> input_text;
				if (text_size + input_text.length() > 76) Interface();
			}
			text.resize(text_size);
			consoleBuffer[5] = text + input_text;
			consoleBuffer[5].resize(76, ' ');
		}
		last_message = text;
	}
	return input_text;
}

void mapCreator::welcomeActivity() {
	Input::clearConsole();
	keys key;
	Interface::addBox(30, 0, 126, 8, "\"MAPS MAKER\"", "center", true, 2018, 't', 15, 0, 4);
	Interface::addBox(31, 5, 124, 3, "select action:", "center", false, 2019, 't', 15, 0, 4);
	Interface::addBox(30, 9, 62, 8, "NEW MAP", "center", true, 2020, 'b', 15, 0, 1);
	Interface::addBox(93, 9, 63, 8, "LOAD MAP", "center", true, 2021, 'b', 15, 0, 1);
	Interface::addBox(157, 40, 10, 6, "EXIT", "center", true, 2024, 'b', 3, 0, 3);
	Interface::addBox(30, 18, 126, 8, "PLAY GAME", "center", true, 2030, 't', 15, 0, 4);
	while (true) {
		key = Input::getPressedKeys();
		if (key.GUI == 2020 && key.ml) {
			Interface::addBox(30, 9, 62, 8, "NEW MAP", "center", true, 2020, 'b', 8, 0, 1);
			newMapActivity();
		}
		else if (key.GUI == 2021 && key.ml) {
			Interface::addBox(93, 9, 63, 8, "LOAD MAP", "center", true, 2021, 'b', 8, 0, 1);
			loadMapActivity(false, true);
		}
		else if (key.GUI == 2024 && key.ml) {
			exit(0);
		}
		else if (key.GUI == 2030 && key.ml) {
			loadMapActivity(true, true);
		}
	}

	Interface::addBox(60, 20, 66, 40, "!!!ERROR!!!", "center", true, 2022, 'm', 15, 0, 4);
}

void mapCreator::newMapActivity(bool useEmptyMap) {
	Input::clearConsole();
	if(useEmptyMap) emptyMap();
	Interface();
	displayMap(41, 0);
	Input::showAvailableColors(0, 53);
	keys key;
	symbol selectedSymbol;
	selectedSymbol.symbol = ' ';
	while (true) {
		key = Input::getPressedKeys();
		for (int i = 0; i < 14; i++) {
			if ((key.GUI - 2001 == ids[i] - 2500) && key.ml) { 
				selectedSymbol.symbol = symbols[i];
				selectedSymbol.type = types[i];
				selectedSymbol.id = ids[i];
				selectedSymbol.color = colors[i];
				std::string temp;
				temp += selectedSymbol.symbol;
				mapCreatorConsole("Clicked button id = " + std::to_string(key.GUI) + "   selected symbol = " + temp, false);
			}
		}
		if (key.GUI == 2012 && key.ml) {
			saveMap();
		}
		if (key.GUI == 2013 && key.ml) {
			loadMapActivity(false);
		}
		if (key.GUI == 2014 && key.ml) {
			emptyMap();
			displayMap(41, 0);
		}
		/*if (key.GUI == 2015) {
			//TODO: run test
		}*/
		if (key.GUI == 2016 && key.ml) {
			welcomeActivity();
		}
		if (key.x > 42 && key.y > 0 && key.x < 184 && key.y < 44 && key.ml) {
			Map[key.x - 41][key.y] = selectedSymbol.symbol;
			std::string temp;
			temp += selectedSymbol.symbol;
			Input::consoleOut(temp, key.x, key.y, true, selectedSymbol.type, selectedSymbol.id, selectedSymbol.color, 0);
			mapCreatorConsole("Added asset - " + temp + " - on coordinates: " + std::to_string(key.x - 43) + " - " + std::to_string(key.y - 1) + " - asset type: "  + selectedSymbol.type, false);
		}
		else if (key.x > 42 && key.y > 0 && key.x < 184 && key.y < 44) {
			mapCreatorConsole("Entered create map mode, your coordinates in the map is: " + std::to_string(key.x - 43) + " - " + std::to_string(key.y - 1), false);
		}
	}
}

void mapCreator::saveMap() {
	std::string mapName;
	mapName = mapCreatorConsole("Enter name of this map: ", true);
	mapName += ".mapf";
	std::ofstream file(mapName);
	if(!file.is_open()) mapCreatorConsole("failed to open file for saving, please try again", false);
	else {
		std::ofstream reg("Maps.registry", std::ios_base::app);
		reg << mapName << std::endl;
		reg.close();
		for (int i = 0; i < 45; i++) {
			for (int j = 0; j < 145; j++) {
				if (Map[j][i] == symbols[10]) file << ".";
				else file << Map[j][i];
			}
			file << std::endl;
		}
		file.close();
		mapCreatorConsole("file was saved: " + mapName, false);
	}
}

void mapCreator::loadMapActivity(bool startGame, bool callFromWelcome) {
	std::ifstream reg("Maps.registry");
	std::string temp[1000];
	short number_of_maps = 0;
	short coord = 39;
	Input::clearConsole();
	Interface::addBox(30, 0, 126, 6, "LOAD MAP", "center", true, 2018, 't', 15, 0, 4);
	Interface::addBox(31, 4, 124, 2, "select map to load:", "center", false, 2019, 't', 15, 0, 4);

	Interface::addBox(30, 46, 126, 4, "ESCAPE MAP SELECTION", "center", true, 2030, 't', 15, 0, 4);
	//---------------------------------------------
	while (!reg.eof()) {
		reg >> temp[number_of_maps];
		number_of_maps++;
	}
	int j = 0, l = 12;
	while (true) {
		for (int i = j; i < l; i += 3) {
			Interface::addBox(coord, 7, 26, 12, temp[i], "center", true, 2200 + i, 'b', 15, 0, 1);
			Interface::addBox(coord, 20, 26, 12, temp[i + 1], "center", true, 2201 + i, 'b', 15, 0, 1);
			Interface::addBox(coord, 33, 26, 12, temp[i + 2], "center", true, 2202 + i, 'b', 15, 0, 1);
			coord += 27;
		}
		Interface::addBox(30, 7, 6, 38, "<-", "center", true, 3500, 'b', 15, 0, 1);
		Interface::addBox(149, 7, 7, 38, "->", "center", true, 3501, 'b', 15, 0, 1);
		char tempC;
		if(startGame) tempC = loadMap(number_of_maps, temp, false);
		else tempC = loadMap(number_of_maps, temp, true);
		if (tempC == 'L') {
			if (j - 12 >= 0) {
				j -= 12;
				l -= 12;
			}
			else {
				j = 0;
				l = 12;
			}
		}
		if (tempC == 'R' && (l < number_of_maps)) {
			if (l + 12 <= number_of_maps) {
				l += 12;
				j += 12;
			}
			else {
				if (j - 12 >= 0) j -= 12;
				else j = 0;
				l = number_of_maps;
			}
		}
		if (tempC == 'E') {
			if (callFromWelcome) welcomeActivity();
			else newMapActivity(false);
		}
		if (l - j > 12) j = l - 12;
		if (l - j < 0) j = l;
		coord = 39;

	}
	//-----------
	loadMap(number_of_maps, temp);
}

void mapCreator::readMap(std::string mapName) {
	std::ifstream file(mapName, std::ios::binary);
	if (!file.is_open()) {Interface::addBox(93, 20, 20, 10, "Registry Error failed to find file", "center", true, 10157, 'M', 12, 1, 12); system("pause");}
	char temp;
	Input::setCursorPosition(0, 0);
	for (int i = 0; i < 45; i++) {
		for (int j = 0; j < 145; j++) {
			file >> temp;
			if (temp == '.') Map[j][i] = symbols[10];
			else Map[j][i] = temp;
			if (temp == symbols[9]) number_of_enemies++;
		}
	}
}

char mapCreator::loadMap(short number_of_maps, std::string* temp, bool editor) {
	while (true) {
		keys key;
		key = Input::getPressedKeys();
		for (int i = 0; i < number_of_maps; i++) {
			if ((key.GUI - 2200 == i) && key.ml) {
				readMap(temp[i]);
				if(editor) newMapActivity(false);
				else Game::gameActivity(*this);
				return ' ';
			}
		}
		if (key.GUI == 3500 && key.ml) return 'L';
		if (key.GUI == 3501 && key.ml) return 'R';
		if ((key.GUI == 2030) && key.ml) return 'E';
	}
}

int mapCreator::getNumberOfEnemies() {
	return number_of_enemies;
}