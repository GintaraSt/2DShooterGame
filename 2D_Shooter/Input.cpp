#include "pch.h"
#include "Input.h"
#pragma once
COORD size;
keys key;
CONSOLE_CURSOR_INFO cursorInfo;
bool mFullScreen;
symbol consoleData[215][55]; // real size 213(190 used in fhd) 53
short fontX = 9, fontY = 20;
bool force_false_ml;
bool force_false_e;
bool force_false_q;

Input::Input()
{
	mFullScreen = false;
	size = getScreenResolution(true);
}

Input::~Input()
{
}

/*enable/disable cursor in console*/
void Input::showConsoleCursor(bool showFlag)
{
	GetConsoleCursorInfo(hout, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(hout, &cursorInfo);
}
/*get pressed keys: w, d, a, s, q, e, mouse  right, mouse left,
cursor position x, y*/
keys Input::getPressedKeys() {
	tagPOINT pos;
	SHORT keyState = GetKeyState('W');
	key.w = keyState & 0x8000;
	keyState = GetKeyState('S');
	key.s = keyState & 0x8000;
	keyState = GetKeyState('A');
	key.a = keyState & 0x8000;
	keyState = GetKeyState('D');
	key.d = keyState & 0x8000;
	keyState = GetKeyState('Q');
	key.q = keyState & 0x8000;
	if (!key.q) force_false_q = false;
	keyState = GetKeyState('E');
	key.e = keyState & 0x8000;
	if (!key.e) force_false_e = false;
	keyState = GetKeyState(VK_LBUTTON);
	key.ml = keyState & 0x8000;
	if(!key.ml) force_false_ml = false;	//if key was unpressed, force false ml is deactivated
	keyState = GetKeyState(VK_RBUTTON);
	key.mr = keyState & 0x8000;

	if (force_false_ml) key.ml = false;
	if (force_false_q) key.q = false;
	if (force_false_e) key.e = false;

	GetCursorPos(&pos);
	key.x = pos.x / fontX;
	key.y = pos.y / fontY;
	key.GUI = consoleData[key.x][key.y].id;
	return key;
}

/*if called, button will return false
from getPressedKeys() until user unpresses it.
When button been unpressed, its force false button is reset back to false
Codes: ml, e, q*/
void Input::forceKeyToFalse(std::string keyCode) {
	if (keyCode == "ml") force_false_ml = true;
	if (keyCode == "e") force_false_e = true;
	if (keyCode == "q") force_false_q = true;
}
/*set position of cursor in console*/
void Input::setCursorPosition(int x, int y)
{
	if (x < size.X && x >= 0 && y >= 0 && y < size.Y) {
		std::cout.flush();						//flushes console?
		COORD coord = { (SHORT)x, (SHORT)y };	//creates a coordinates vector? for seting cursors position?
		SetConsoleCursorPosition(hout, coord);	//sets position of cursor in console obviously
	}
	//else {
		//MessageBox(nullptr, TEXT("Cursor coordinates is out of range"), TEXT("ERROR: "), MB_OK);
	//}
}
/*return resolution of screen in symbols*/
COORD Input::getScreenResolution(bool inSymbols) {
	RECT mConsoleWindow;
	GetWindowRect(ConsoleWindow, &mConsoleWindow);
	if (inSymbols) {
		size.X = mConsoleWindow.right / fontX;
		size.Y = mConsoleWindow.bottom / fontY;
	}
	else {
		size.X = mConsoleWindow.right;
		size.Y = mConsoleWindow.bottom;
	}
	
	return size;
}
/*full screen mode*/
void Input::fullScreen(bool fullScreen) {
	if (fullScreen != mFullScreen) {
		::SendMessage(::GetConsoleWindow(), WM_SYSKEYDOWN, VK_RETURN, 0x20000000);
		mFullScreen = fullScreen;
	}
}
/*resize font acording to console screen size in pixels*/
void Input::adjustWindowRes() {
	getScreenResolution(false);
	fontX = (size.X / 213);				//divider = FHD / current screen
	fontY = (size.Y / 53);
	setFontSize(fontX - 1, fontY - 4);
	fontX += 1;
}
/*set font size of console*/
void Input::setFontSize(short x, short y) {
	CONSOLE_FONT_INFOEX cfi;			//structure that stores info about font in console
	cfi.cbSize = sizeof(cfi);			//size of this structure in bytes? why is it important?
	cfi.nFont = 0;						// DWORD - index of font in the systems console font table
	cfi.dwFontSize.X = x;				// COORD - Width of each character in the font
	cfi.dwFontSize.Y = y;				// COORD - Height of each character in the font
	cfi.FontFamily = FF_DONTCARE;		// UINT - font
	cfi.FontWeight = FW_NORMAL;			// UINT - weight 100-1000, 400 - normal, 700 - bold
	wcscpy_s(cfi.FaceName, L"Consolas"); // Choose your font
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}
/*console output*/ 
bool Input::consoleOut(std::string Text, int x, int y, bool horizontal, char type, int id, int textColor, int backgroundColor) {
	if (horizontal) {
		if (x + Text.length() > 213) return false;
		setCursorPosition(x, y);
		for (int i = 0; i < Text.length(); i++) {
			consoleData[x + i][y].symbol = Text[i];
			consoleData[x + i][y].type = type;
			consoleData[x + i][y].id = id;
			consoleData[x + i][y].color = textColor + (backgroundColor * 16);
			SetConsoleTextAttribute(hout, consoleData[x + i][y].color);
			std::cout << Text[i];
		}
	}
	else {
		if (y + Text.length() > 53) return false;
		for (int i = 0; i < Text.length(); i++) {
			setCursorPosition(x, y + i);
			consoleData[x][y + i].symbol = Text[i];
			consoleData[x][y + i].type = type;
			consoleData[x][y + i].id = id;
			consoleData[x][y + i].color = textColor + (backgroundColor * 16);
			SetConsoleTextAttribute(hout, consoleData[x][y + i].color);
			std::cout << Text[i];
		}
	}
	SetConsoleTextAttribute(hout, 15); // reset color to black background and white text
	std::cout.flush();
	return true;
}
/*get color of console character in given position*/
int Input::getColorOfSymbol(short x, short y) {
	return consoleData[x][y].color;
}
/*get id of console character in given position*/
int Input::getIdOfSymbol(short x, short y) {
	return consoleData[x][y].id;
}
/*get char of console character in given position*/
char Input::getCharOfSymbol(short x, short y) {
	return consoleData[x][y].symbol;
}
/*get type of console character in given position*/
char Input::getTypeOfSymbol(short x, short y) {
	return consoleData[x][y].type;
}
/*show available colors*/
void Input::showAvailableColors(short x, short y){
	setCursorPosition(x, y);
	for (int i = 0; i < 16; i++) {
		SetConsoleTextAttribute(hout, i * 16 + 15);
		std::cout << "     " << i << "     ";
	}
	SetConsoleTextAttribute(hout, 15);
}
/*get symbol from console data array*/
symbol Input::getConsoleData(short x, short y) {
	return consoleData[x][y];
}
/*for simple symbol change, id=0, type=0, color=15, horizontal=true*/
bool Input::consoleOut(std::string Text, int x, int y) {
	if (x + Text.length() > 213) return false;
	setCursorPosition(x, y);
	for (int i = 0; i < Text.length(); i++) {
		consoleData[x + i][y].symbol = Text[i];
		consoleData[x + i][y].type = '0';
		consoleData[x + i][y].id = 0;
		consoleData[x + i][y].color = 15;
		SetConsoleTextAttribute(hout, consoleData[x + i][y].color);
		std::cout << Text[i];
	}
	std::cout.flush();
	return true;
}
/*clears console entirely*/
void Input::clearConsole() {
	system("cls");
	for (int i = 0; i < 213; i++) {
		for (int j = 0; j < 53; j++) {
			consoleData[i][j].symbol = ' ';
			consoleData[i][j].type = 'N';
			consoleData[i][j].id = 0;
			consoleData[i][j].color = 0;
		}
	}
}