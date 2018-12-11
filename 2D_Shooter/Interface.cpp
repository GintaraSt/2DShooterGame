#include "pch.h"
#include "Interface.h"
#pragma once

Interface::Interface()
{
}


Interface::~Interface()
{
}

void Interface::addBox(short x_position, short y_position, short x_size, short y_size, std::string text, std::string textAlignment, bool useFrame, int id, char type, int textColor, int backgroundColor, int frameColor) {
	std::string temp;
	if (useFrame) {
		drawFrame(x_position, y_position, x_size, y_size, id, type, backgroundColor, frameColor);
	}
	colorBox(x_position, y_position, x_size, y_size, id, type, backgroundColor);
	if (text.length() >= x_size) {
		writeMultilineText(x_position, y_position, x_size, y_size, text, textAlignment, id, type, textColor, backgroundColor);
	}
	else {
		textAlign(x_position, y_position, x_size, y_size, text, textAlignment);
		writeText(x_position, y_position, x_size, text, id, type, textColor, backgroundColor);
	}
	
	
}

void Interface::writeText(short x_position, short y_position, short x_size, std::string text, int id, char type, int textColor, int backgroundColor) {
	int k = 0;
	std::string temp;
	for (int i = x_position; i < x_position + text.length(); i++) {
		if (k >= x_size) break;
		temp = text[k];
		k++;
		Input::consoleOut(temp, i, y_position, true, type, id, textColor, backgroundColor);
	}
}

void Interface::writeMultilineText(short x_position, short y_position, short x_size, short y_size, std::string text, std::string textAlignment, int id, char type, int textColor, int backgroundColor) {
	int index = 0;
	double temp0 = text.length() * 1.0 / (x_size - 1);
	int lines_needed = temp0;
	if (lines_needed * 1.0 < temp0) lines_needed++;

	std::string text_continue;
	while (x_size < text.length() - index)
	{
		for (int i = 0; i < x_size - 1; i++) {
			text_continue += text[index];
			index++;
			if (index >= text.length()) break;
		}
		while (y_position > (y_position + ((y_size - lines_needed) / 2))) { lines_needed--; }
		lines_needed--;
		if ((y_size - lines_needed) / 2 >= y_size) break;
		writeText(x_position, y_position + ((y_size - lines_needed) / 2), x_size, text_continue, id, type, textColor, backgroundColor);
		lines_needed--;
		text_continue.clear();
	}
	if (!((y_size - lines_needed) / 2 >= y_size - 1)) {
		for (int i = 0; i < x_size; i++) {
			text_continue += text[index];
			index++;
			if (index >= text.length()) break;
		}
		lines_needed += 2;
		textAlign(x_position, y_position, x_size, y_size, text_continue, textAlignment);
		writeText(x_position, y_position + ((y_size - lines_needed) / 2), x_size, text_continue, id, type, textColor, backgroundColor);
	}
}

void Interface::textAlign(short &x_position, short &y_position, short x_size, short y_size, std::string text, std::string textAlignment) {
	if (text.length() == 0) return;
	if (textAlignment == "center") {
		if (x_size > text.length()) {
			x_position = x_position + x_size / 2 - text.length() / 2;
		}
		y_position = y_position + y_size / 2;
	}
	else if (textAlignment == "left_center") {
		y_position = y_position + y_size / 2;
	}
	else if (textAlignment == "right_center") {
		if (x_size > text.length()) {
			x_position = x_position + x_size - text.length();
		}
		y_position = y_position + y_size / 2;
	}
	else if (textAlignment == "center_horizontal_top") {
		if (x_size > text.length()) {
			x_position = x_position + x_size / 2 - text.length() / 2;
		}
	}
	else if (textAlignment == "center_horizontal_bottom") {
		if (x_size > text.length()) {
			x_position = x_position + x_size / 2 - text.length() / 2;
		}
		y_position += y_size - 1;
	}
	else if (textAlignment == "left_top") {
		//nothing to do
	}
	else if (textAlignment == "right_top") {
		if (x_size > text.length()) {
			x_position = x_position + x_size - text.length();
		}
	}
	else if (textAlignment == "right_bottom") {
		if (x_size > text.length()) {
			x_position = x_position + x_size - text.length();
		}
		y_position += y_size - 1;
	}
	else if (textAlignment == "left_bottom") {
		y_position += y_size - 1;
	}
}

void Interface::colorBox(short x_position, short y_position, short x_size, short y_size, int id, char type, int backgroundColor) {
	for (int j = y_position; j < y_position + y_size; j++) {
		for (int i = x_position; i < x_position + x_size; i++) {
			Input::consoleOut(" ", i, j, true, type, id, 0, backgroundColor);
		}
	}
}

void Interface::drawFrame(short &x_position, short &y_position, short &x_size, short &y_size, int id, char type, int backgroundColor, int frameColor) {
	std::string temp;
	temp = char(201);
	Input::consoleOut(temp, x_position, y_position, true, type, id, frameColor, backgroundColor);
	temp = char(187);
	Input::consoleOut(temp, x_position + x_size, y_position, true, type, id, frameColor, backgroundColor);
	temp = char(200);
	Input::consoleOut(temp, x_position, y_position + y_size, true, type, id, frameColor, backgroundColor);
	temp = char(188);
	Input::consoleOut(temp, x_position + x_size, y_position + y_size, true, type, id, frameColor, backgroundColor);
	temp = char(205);
	for (int i = x_position + 1; i < x_position + x_size; i++) {
		Input::consoleOut(temp, i, y_position, true, type, id, frameColor, backgroundColor);
		Input::consoleOut(temp, i, y_position + y_size, true, type, id, frameColor, backgroundColor);
	}
	temp = char(186);
	for (int i = y_position + 1; i < y_position + y_size; i++) {
		Input::consoleOut(temp, x_position, i, true, type, id, frameColor, backgroundColor);
		Input::consoleOut(temp, x_position + x_size, i, true, type, id, frameColor, backgroundColor);
	}
	x_position++;
	y_position++;
	x_size--;
	y_size--;
}