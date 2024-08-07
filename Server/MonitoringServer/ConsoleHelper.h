#pragma once

enum class ConsoleColor
{
	BLACK = 0,
	RED = FOREGROUND_RED,
	GREEN = FOREGROUND_GREEN,
	BLUE = FOREGROUND_BLUE,
	YELLOW = RED | GREEN,
	WHITE = RED | GREEN | BLUE,
};

class ConsoleHelper
{
public:
	static void SetCursorPosition(int32 x, int32 y);
	static void SetCursorColor(ConsoleColor color);
	static void ShowConsoleCursor(bool flag);
	static void Clear();
};
