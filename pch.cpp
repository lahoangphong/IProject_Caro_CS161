// pch.cpp: source file corresponding to pre-compiled header; necessary for compilation to succeed
#pragma warning(disable : 4996)

#include "pch.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <queue>
#include <string>
#include <cwchar>
#include <ctime>;

#define FOR(i, x, y)	for (int i = x ; i <= y ; i ++)
using namespace std; 

void Nocursortype()
{
	CONSOLE_CURSOR_INFO Info;
	Info.bVisible = FALSE;
	Info.dwSize = 20;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
}

void Cursortype() {
	CONSOLE_CURSOR_INFO Info;
	Info.bVisible = TRUE;
	Info.dwSize = 20;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
}
void ResizeConsole(int width, int height)
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, width, height, TRUE);
}

void TextColor(int x)
{
	HANDLE mau;
	mau = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(mau, x);
}

void gotoxy(int x, int y)
{
	HANDLE hConsoleOutput;
	COORD Cursor_an_Pos = { x - 1,y - 1 };
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsoleOutput, Cursor_an_Pos);
}

void ClearScreen()
{
	HANDLE hOut;
	COORD Position;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	Position.X = 0;
	Position.Y = 0;
	SetConsoleCursorPosition(hOut, Position);
}

void ChangeFontSize(int h) {
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;                   // Width of each character in the font
	cfi.dwFontSize.Y = h;                  // Height
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas"); // Choose your font
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

void Board(int size_w, int size_h){
	TextColor(7);
	gotoxy(1, 1);
	FOR(i, 1, size_h * 2) {
		FOR(j, 1, size_w * 4 + 1)
			if (j % 4 == 1)
				cout << "|";
			else
				if (i % 2 != 1)
					cout << "_";
				else
					cout << " ";
		cout << "\n";
	}
}

void MaMau() {
	for (int i = 1; i <= 255; i++) {
		TextColor(i);
		cout << "Mau " << i << "\n";
	}
}

void ClearConsoleInputBuffer()
{
	// If you happen to have any trouble clearing already cleared buffer, uncomment the section below.
	/* keybd_event('S', 0, 0, 0);
	keybd_event('S', 0,KEYEVENTF_KEYUP, 0);
	keybd_event(VK_BACK, 0, 0, 0);
	keybd_event(VK_BACK, 0,KEYEVENTF_KEYUP, 0); */
	PINPUT_RECORD ClearingVar1 = new INPUT_RECORD[256];
	DWORD ClearingVar2;
	ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), ClearingVar1, 256, &ClearingVar2);
	delete[] ClearingVar1;
}

