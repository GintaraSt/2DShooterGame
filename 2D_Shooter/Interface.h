#pragma once
#ifndef INTERFACE_H_
#define INTERFACE_H_
#include "pch.h"
#include "Input.h"

#pragma message ("Entered Interface")
class Interface
{
private:
	void static textAlign(short &x_position, short &y_position, short x_size, short y_size, std::string text = "text", std::string textAlignment = "center");
	void static writeText(short x_position, short y_position, short x_size, std::string text, int id, char type, int textColor, int backgroundColor);
	void static writeMultilineText(short x_position, short y_position, short x_size, short y_size, std::string text, std::string textAlignment, int id, char type, int textColor, int backgroundColor);
public:
	Interface();
	~Interface();
	static void addBox(short x_position, short y_position, short x_size, short y_size, std::string text = "text", std::string textAlignment = "center",  bool useFrame = false, int id = 0, char type = 'N', int textColor = 15, int backgroundColor = 0, int frameColor = 15);
	static void colorBox(short x_position, short y_position, short x_size, short y_size, int id = 0, char type = 'N', int backgroundColor = 0);
	static void drawFrame(short &x_position, short &y_position, short &x_size, short &y_size, int id = 0, char type = 'N', int backgroundColor = 0, int frameColor = 15);
};

#pragma message ("Exited Interface")
#endif