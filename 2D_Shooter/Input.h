#pragma once
#ifndef INPUT_H_
#define INPUT_H_
#include "pch.h"
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <thread>
#include <chrono>
#include <fstream>
#include "time.h"
#include "Maps_maker.h"
#include <string>

#pragma message ("Entered Input")
const HWND ConsoleWindow = GetConsoleWindow();
const HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
const double NATIVE_X_FONT = 9;//if font size is 16 then its 16px + 4px line space in height and 8px + 1px symbols space in width;
const double NATIVE_Y_FONT = 20;

struct keys {
	bool w = false;
	bool s = false;
	bool a = false;
	bool d = false;
	bool mr = false;
	bool ml = false;
	bool q = false;
	bool e = false;
	short x = 0;
	short y = 0;
	int GUI = 0;
};

struct symbol {
	char type = 'N'; //w - wall, g - gun, t - text, b - button etc...
	char symbol;
	int id = 0;			//if button, with this id it will be posible to find function asigned to it
	int color = 15;
};
struct extendedSymbol {
	symbol main;
	int x;
	int y;
};

class Input
{
private:
	void static setFontSize(short x, short y);
public:
	Input();
	~Input();
	void static showConsoleCursor(bool showFlag);
	keys static getPressedKeys();
	void static setCursorPosition(int x, int y);
	COORD static getScreenResolution(bool inSymbols);
	void static fullScreen(bool fullScreen);
	void static adjustWindowRes();
	int static getColorOfSymbol(short x, short y);
	int static getIdOfSymbol(short x, short y);
	char static getCharOfSymbol(short x, short y);
	char static getTypeOfSymbol(short x, short y);
	bool static consoleOut(std::string Text, int x, int y, bool horizontal, char type, int id, int textColor, int backgroundColor);
	bool static consoleOut(std::string Text, int x, int y);
	void static showAvailableColors(short x, short y);
	symbol static getConsoleData(short x, short y);
	void static clearConsole();
	void static forceKeyToFalse(std::string keyCode);
};

#pragma message ("Exited Input")
#endif