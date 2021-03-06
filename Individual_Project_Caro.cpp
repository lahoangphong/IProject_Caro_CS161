// Individual _ Project _ Caro
// Name : La Hoang Phong
// ID   : 18125017

#include "pch.h"

#pragma comment(lib, "winmm.lib")
#pragma warning(disable : 4996)

#include <iostream>
#include <fstream>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <queue>
#include <map>
#include <string>
#include <cwchar>
#include <stdio.h>
#include <ctime>
#include <MMsystem.h>

using namespace std;

#define FOR(i,a,b) for (int i = a ;i <= b ; ++i)
#define ROF(i,a,b) for (int i = a ;i >= b ; --i)
#define REP(i,a) for (int i = 0 ; i < a ; i++)
#define ii pair <int, int>
#define fi first
#define se second 
#define pb push_back
#define ll long long

const int dx[8] = { 0, 1, 0, -1, 1, 1, -1, -1 }; // For Caro_Image of splash
const int dy[8] = { 1, 0, -1, 0, 1, -1, 1, -1 };
const int w = 1280; // console size 
const int h = 720; // console size
const int y_max = 42; // number of row
const int x_max = 172; // number of col
const int X_color = 12; // color of x ( RED )
const int O_color = 9; // color of O ( BLUE )
const int F = 219; // For Image Caro 
const int N = 32; // For Image Caro 

bool Enable_Music = 1; // Music on or off;
char playerX = 'X'; // Default Icon of 'X'
char playerO = 'O'; // Default Icon of 'O'
int size_w = 18, size_h = 15; // max 24, 20;

struct save { // a simple save game
	string name, p1, p2, cur_time; // Game name, name of player1, player2, time when saving
	int size_list, type; // size of move_list, type of game
	int size_w, size_h; // size of game board
	vector <ii> xo_list; // move_list 
	int turn_game; // current turn game
};

map <string, int> Rank; // name and number of game won

struct dang_info { // struct for calc dangerous of move
	int fi, se, th, fo;
};

bool equal(dang_info x, dang_info y) {
	return x.fi == y.fi && x.se == y.se && x.th == y.th && x.fo == y.fo;
} // function for check equal 2 dang_info
vector <save> file_save, history;

bool comp(pair<string, int> x, pair<string, int> y) {
	return x.second > y.second;
}

int x_now, y_now ; // current positive ;
int board_now[30][30] = { 0 }; // a board
char type[2] = { 'X', 'O' }; // For Splash

string Time() {
	time_t now = time(0);
	char* dt = ctime(&now);
	string cv(dt);
	return cv;
} // Return Time Now 

ll Rand(ll l, ll r) {
	return l + ((ll)rand() * (RAND_MAX + 1) * (RAND_MAX + 1) * (RAND_MAX + 1) +
		(ll)rand() * (RAND_MAX + 1) * (RAND_MAX + 1) +
		(ll)rand() * (RAND_MAX + 1) +
		(ll)rand()) % (r - l + 1);
} // Random  Function 

void Put(int type, int x, int y) {
	if (type == 0)
		TextColor(4);
	else
		TextColor(1);
	gotoxy((x - 1) * 4 + 3, y * 2);
	if (Enable_Music)
		PlaySound(L"CLICK.WAV", NULL, SND_ASYNC);
	if (type == 0)
		cout << playerX;
	else
		cout << playerO;
} // Put X or O to Board

void Load_File_Save() {
	file_save.clear();
	int Size;
	ifstream inp("Save.txt");
	inp >> Size;
	string clear_line;
	REP(i, Size) {
		getline(inp, clear_line);
		save save_inp;
		getline(inp, save_inp.name);
		getline(inp, save_inp.p1);
		getline(inp, save_inp.p2);
		getline(inp, save_inp.cur_time);
		inp >> save_inp.type;
		inp >> save_inp.size_w;
		inp >> save_inp.size_h;
		inp >> save_inp.size_list;
		REP(i_list, save_inp.size_list) {
			int x, y;
			inp >> x >> y;
			save_inp.xo_list.pb(ii(x, y));
		}
		inp >> save_inp.turn_game;
		file_save.pb(save_inp);
	}
	inp.close();
	// Load a list of file save
	Rank = map<string, int>();
	ifstream inp_1("Rank.txt");
	inp_1 >> Size;
	REP(i, Size) {
		string name;
		int gamewon;
		getline(inp_1, clear_line);
		getline(inp_1, name);
		inp_1 >> gamewon;
		Rank[name] = gamewon;
	}
	inp_1.close();
	// Load a list of Ranking
	history.clear();
	ifstream inp_2("History.txt");
	inp_2 >> Size;
	clear_line;
	REP(i, Size) {
		getline(inp_2, clear_line);
		save save_inp;
		getline(inp_2, save_inp.name); // player who win
		getline(inp_2, save_inp.p1);
		getline(inp_2, save_inp.p2);
		getline(inp_2, save_inp.cur_time);
		inp_2 >> save_inp.type;
		inp_2 >> save_inp.size_w;
		inp_2 >> save_inp.size_h;
		inp_2 >> save_inp.size_list;
		REP(i_list, save_inp.size_list) {
			int x, y;
			inp_2 >> x >> y;
			save_inp.xo_list.pb(ii(x, y));
		}
		inp_2 >> save_inp.turn_game;
		history.pb(save_inp);
	}
	inp_2.close();
	// Load a list of game
} // Load

void Rewrite_File_Save() {
	//cout << "";
	//Sleep(2000);
	ofstream out("Save.txt");
	out << file_save.size() << "\n";
	REP(i_file, file_save.size()) {
		out << file_save[i_file].name << "\n";
		out << file_save[i_file].p1 << "\n";
		out << file_save[i_file].p2 << "\n";
		out << file_save[i_file].cur_time << "\n";
		out << file_save[i_file].type << "\n";
		out << file_save[i_file].size_w << " " << file_save[i_file].size_h << "\n";
		out << file_save[i_file].size_list << "\n";
		REP(i_xo_list, file_save[i_file].xo_list.size()) {
			out << file_save[i_file].xo_list[i_xo_list].first << " " << file_save[i_file].xo_list[i_xo_list].second << "\n";
		}
		out << file_save[i_file].turn_game << "\n";
	}
	out.close(); 
	// add save game

	ofstream out_1("Rank.txt");
	out_1 << Rank.size() << "\n";
	map<string, int>::iterator it = Rank.begin();
	while (it != Rank.end()) {
		out_1 << it->first << "\n" << it->second << "\n";
		++it;
	}
	out_1.close();

	// add new player to ranking

	ofstream out_2("History.txt");
	out_2 << history.size() << "\n";
	REP(i_file, history.size()) {
		out_2 << history[i_file].name << "\n";
		out_2 << history[i_file].p1 << "\n";
		out_2 << history[i_file].p2 << "\n";
		out_2 << history[i_file].cur_time << "\n";
		out_2 << history[i_file].type << "\n";
		out_2 << history[i_file].size_w << " " << history[i_file].size_h << "\n";
		out_2 << history[i_file].size_list << "\n";
		REP(i_xo_list, history[i_file].xo_list.size()) {
			out_2 << history[i_file].xo_list[i_xo_list].first << " " << history[i_file].xo_list[i_xo_list].second << "\n";
		}
		out_2 << history[i_file].turn_game << "\n";
	}
	out_2.close();
	
	// add a game to history list
} 

int Check_Board(int x, int y) {
	int x_next, y_next;
	bool ok = 1;
	int n1 = 0, n2 = 0, d1 = 0, d2 = 0, cc1 = 0, cc2 = 0, cp1 = 0, cp2 = 0;
	FOR(i, 1, 4) { // ngang phai
		gotoxy(100, 20 + i);
		x_next = x + i;
		y_next = y;
		if (x_next > 18 || y_next > 15 || x_next < 1 || y_next < 1 || board_now[x_next][y_next] != board_now[x][y]) {
			break;
		}
		++n1;
	}
	FOR(i, 1, 4) { // ngang trai

		x_next = x - i;
		y_next = y;
		if (x_next > 18 || y_next > 15 || x_next < 1 || y_next < 1 || board_now[x_next][y_next] != board_now[x][y]) {
			break;
		}
		++n2;
	}
	FOR(i, 1, 4) { // doc duoi
		x_next = x;
		y_next = y + i;
		if (x_next > 18 || y_next > 15 || x_next < 1 || y_next < 1 || board_now[x_next][y_next] != board_now[x][y]) {
			break;
		}
		++d1;
	}
	FOR(i, 1, 4) { // doc tren
		x_next = x;
		y_next = y - i;
		if (x_next > 18 || y_next > 15 || x_next < 1 || y_next < 1 || board_now[x_next][y_next] != board_now[x][y]) {
			break;
		}
		++d2;
	}
	FOR(i, 1, 4) { // cheo chinh 1
		x_next = x + i;
		y_next = y + i;
		if (x_next > 18 || y_next > 15 || x_next < 1 || y_next < 1 || board_now[x_next][y_next] != board_now[x][y]) {
			break;
		}
		++cc1;
	}
	FOR(i, 1, 4) { // cheo chinh 2
		x_next = x - i;
		y_next = y - i;
		if (x_next > 18 || y_next > 15 || x_next < 1 || y_next < 1 || board_now[x_next][y_next] != board_now[x][y]) {
			break;
		}
		++cc2;
	}
	FOR(i, 1, 4) { // cheo phu 1
		x_next = x + i;
		y_next = y - i;
		if (x_next > 18 || y_next > 15 || x_next < 1 || y_next < 1 || board_now[x_next][y_next] != board_now[x][y]) {
			break;
		}
		++cp1;
	}
	FOR(i, 1, 4) { // cheophu2
		x_next = x - i;
		y_next = y + i;
		if (x_next > 18 || y_next > 15 || x_next < 1 || y_next < 1 || board_now[x_next][y_next] != board_now[x][y]) {
			break;
		}
		++cp2;
	}
	return max(n1 + n2, max(d1 + d2, max(cc1 + cc2, cp1 + cp2)));
} 

int Check_Board_Hard(int x, int y) {
	int x_next, y_next;
	bool ok = 1;
	int n1 = 0, n2 = 0, d1 = 0, d2 = 0, cc1 = 0, cc2 = 0, cp1 = 0, cp2 = 0;
	FOR(i, 1, 4) { // ngang phai
		gotoxy(100, 20 + i);
		x_next = x + i;
		y_next = y;
		if (x_next > 18 || y_next > 15 || x_next < 1 || y_next < 1 || board_now[x_next][y_next] != board_now[x][y]) {
			break;
		}
		++n1;
	}
	FOR(i, 1, 4) { // ngang trai

		x_next = x - i;
		y_next = y;
		if (x_next > 18 || y_next > 15 || x_next < 1 || y_next < 1 || board_now[x_next][y_next] != board_now[x][y]) {
			break;
		}
		++n2;
	}
	FOR(i, 1, 4) { // doc duoi
		x_next = x;
		y_next = y + i;
		if (x_next > 18 || y_next > 15 || x_next < 1 || y_next < 1 || board_now[x_next][y_next] != board_now[x][y]) {
			break;
		}
		++d1;
	}
	FOR(i, 1, 4) { // doc tren
		x_next = x;
		y_next = y - i;
		if (x_next > 18 || y_next > 15 || x_next < 1 || y_next < 1 || board_now[x_next][y_next] != board_now[x][y]) {
			break;
		}
		++d2;
	}
	FOR(i, 1, 4) { // cheo chinh 1
		x_next = x + i;
		y_next = y + i;
		if (x_next > 18 || y_next > 15 || x_next < 1 || y_next < 1 || board_now[x_next][y_next] != board_now[x][y]) {
			break;
		}
		++cc1;
	}
	FOR(i, 1, 4) { // cheo chinh 2
		x_next = x - i;
		y_next = y - i;
		if (x_next > 18 || y_next > 15 || x_next < 1 || y_next < 1 || board_now[x_next][y_next] != board_now[x][y]) {
			break;
		}
		++cc2;
	}
	FOR(i, 1, 4) { // cheo phu 1
		x_next = x + i;
		y_next = y - i;
		if (x_next > 18 || y_next > 15 || x_next < 1 || y_next < 1 || board_now[x_next][y_next] != board_now[x][y]) {
			break;
		}
		++cp1;
	}
	FOR(i, 1, 4) { // cheophu2
		x_next = x - i;
		y_next = y + i;
		if (x_next > 18 || y_next > 15 || x_next < 1 || y_next < 1 || board_now[x_next][y_next] != board_now[x][y]) {
			break;
		}
		++cp2;
	}
	return n1 + n2 + d1 + d2 + cc1 + cc2 + cp1 + cp2;
}

void SplashScreen() {
	char Center_Text[5][25] ={ N,F,F,F,N,N,N,N,F,F,N,N,N,N,F,F,F,N,N,N,N,F,F,F,N
							  ,F,N,N,N,N,N,N,F,N,N,F,N,N,N,F,N,N,F,N,N,F,N,N,N,F
							  ,F,N,N,N,N,N,N,F,F,F,F,N,N,N,F,F,F,F,N,N,F,N,N,N,F
							  ,F,N,N,N,N,N,F,N,N,N,N,F,N,N,F,N,F,N,N,N,F,N,N,N,F
							  ,N,F,F,F,N,N,F,N,N,N,N,F,N,N,F,N,N,F,N,N,N,F,F,F,N
							 };
	string Menu_simple_[7];
	string Version = "Version 1.0 - Made By LaHoangPhong";

	Menu_simple_[0] = "1. PvP Mode			";
	Menu_simple_[1] = "2. PvC Mode			";
	Menu_simple_[2] = "3. Load Game        ";
	Menu_simple_[3] = "4. Player Statistics";
	Menu_simple_[4] = "5. Option";
	Menu_simple_[5] = "6. Exit";
	Menu_simple_[6] = "Choose Your Mode Here : ";
	bool Filled[5][25] = { 0 };
	queue<ii> Queue;
	REP(rep_for_begin, 1) {
		ROF(i, x_max, 1) {
			gotoxy(i, y_max);
			TextColor(X_color);
			cout << "X X X X";
			gotoxy(x_max - i + 1, 1);
			TextColor(O_color);
			cout << "O O O O";
			Sleep(11);
			system("cls");
		}
		ROF(i, y_max, 1) {
			TextColor(X_color);
			gotoxy(1, i);
			cout << "X";
			gotoxy(1, i+1);
			cout << "X";
			gotoxy(1, i+2);
			cout << "X";
			gotoxy(1, i+3);
			cout << "X";
			TextColor(O_color);
			gotoxy(x_max, y_max - i + 1);
			cout << "O";
			gotoxy(x_max, y_max - i + 2);
			cout << "O";
			gotoxy(x_max, y_max - i + 3);
			cout << "O";
			gotoxy(x_max, y_max - i + 4);
			cout << "O";
			Sleep(41);
			system("cls");
		}

	}
	REP(rep_for_next, 1) {
		FOR(i, 1, y_max/2) {
			TextColor(X_color);
			gotoxy(i - 3 , i - 3);
			cout << "X";
			gotoxy(i - 2, i - 2);
			cout << "X";
			gotoxy(i - 1, i - 1);
			cout << "X";
			gotoxy(i , i );
			cout << "X";
			TextColor(O_color);
			gotoxy(x_max - i + 3, y_max - i + 3);
			cout << "O";
			gotoxy(x_max - i + 2, y_max - i + 2);
			cout << "O";
			gotoxy(x_max - i + 1, y_max - i + 1);
			cout << "O";
			gotoxy(x_max - i , y_max - i);
			cout << "O";
			Sleep(41);
			system("cls");
		}
	}
	
	REP(rep_for_last, 1) {
		FOR(i, y_max / 2, x_max / 2-1) {
			TextColor(X_color);
			gotoxy(i, y_max / 2);
			cout << "X X X X";
			TextColor(O_color);	
			gotoxy(x_max - i - 2, y_max / 2);
			cout << "O O O O";
			Sleep(21);
			system("cls");

		}
	}
	REP(rep_for_text, 1) {
		Queue.push(ii(2, 12));
		Filled[2][12] = 1;
		TextColor(6);
		while (!Queue.empty()) {
			int u = Queue.front().fi;
			int v = Queue.front().se;
			gotoxy(x_max / 2 - 12 + v, y_max / 2 - 2 + u);
			cout << Center_Text[u][v];
			Sleep(25);
			Queue.pop();
			REP(i_next, 4) {
				int u_next = u + dx[i_next];
				int v_next = v + dy[i_next];
				if (u_next >= 0 && u_next < 5 && v_next >= 0 && v_next < 25 && !Filled[u_next][v_next]) {
					Filled[u_next][v_next] = 1;
					Queue.push(ii(u_next, v_next));
				}
			}
		}
		REP(up_line, 10) {
			REP(i, 5) {
				gotoxy(x_max / 2 - 12, y_max / 2 - 2 + i - up_line);
				REP(j, 25) {
					if (Filled[i][j])
						cout << Center_Text[i][j];
					else
						cout << " ";
				}
				cout << "\n";
			}
			Sleep(5);
			if (up_line < 9)
				system("cls");
		}
		//Text end in line y_max/2 - 8;
		TextColor(11);
		REP(i, 7) {
			if (i < 6) {
				gotoxy(x_max / 2 - 6, y_max / 2 - 5 + i * 2);
				cout << Menu_simple_[i];
			}
			else {
				TextColor(1);
				gotoxy(x_max / 2 - 10, y_max / 2 - 5 + i * 2 + 1);
				cout << Menu_simple_[i];
			}
			Sleep(100);
		}
	}
	REP(rep_for_my_info, 1) {
		TextColor(14);
		gotoxy(x_max / 2 - Version.length() / 2, y_max);
		REP(i, Version.length()) {
			cout << Version[i];
			Sleep(10);
		}
	}
}

void PvP_Mode(int id_game) {
 // Initization for id = 0;
	system("cls");
	save new_save;
	int turn;
	new_save.type = 0;
	memset(board_now, 0, sizeof(board_now));
	int count_play = 0;
	TextColor(11);
	gotoxy(77, y_max);
	cout << "Press ESC to Save and Exit";
	TextColor(7);
	gotoxy(100, 20);
	cout << "WRONG MOVE !!!";
	if (id_game == 0) {
		new_save.size_h = size_h;
		new_save.size_w = size_w;
		Board(size_w, size_h); // BOARD 15 x 18
		x_now = size_w / 2, y_now = size_h / 2;
		turn = 0;
		Cursortype();
		gotoxy(100, 3);
		TextColor(4);
		cout << "Player 1 : ";
		TextColor(14);
		getline(cin, new_save.p1);
		TextColor(1);
		gotoxy(100, 5);
		cout << "Player 2 : ";
		TextColor(14);
		getline(cin, new_save.p2);
		Nocursortype();
	}
	else {
		// Load old game;
		//1. Load name
		new_save = file_save[id_game - 1];
		size_w = new_save.size_w;
		size_h = new_save.size_h;
		Board(size_w, size_h); // BOARD 15 x 18
		x_now = size_w / 2, y_now = size_h / 2;
		gotoxy(100, 3);
		TextColor(4);
		cout << "Player 1 : ";
		TextColor(14);
		REP(i_p1, new_save.p1.length()) {
			cout << new_save.p1[i_p1];
			Sleep(50);
		}
		TextColor(1);
		gotoxy(100, 5);
		cout << "Player 2 : ";
		TextColor(14);
		REP(i_p2, new_save.p2.length()) {
			cout << new_save.p2[i_p2];
			Sleep(50);
		}
		//2. Load Board;
		turn = 0;
		REP(i_xo, new_save.xo_list.size()) {
			Put(turn, new_save.xo_list[i_xo].first, new_save.xo_list[i_xo].second);
			board_now[new_save.xo_list[i_xo].first][new_save.xo_list[i_xo].second] = turn + 1;
			turn = 1 - turn;
			Sleep(50);
		}
	}
// Game_loop
	while (1) {
		if (GetAsyncKeyState(VK_UP) & 1)
			--y_now;
		if (GetAsyncKeyState(VK_DOWN) & 1)
			++y_now;
		if (GetAsyncKeyState(VK_LEFT) & 1)
			--x_now;
		if (GetAsyncKeyState(VK_RIGHT) & 1)
			++x_now;
		if (GetAsyncKeyState(VK_RETURN) & 1) {
			if (!count_play) {
				++count_play;
				continue;
			}
			if (board_now[x_now][y_now] != 0) {
				TextColor(4);
				gotoxy(100, 20);
				cout << "WRONG MOVE !!!";
				Sleep(100);
				TextColor(15);
				gotoxy(100, 20);
				cout << "WRONG MOVE !!!";
				Sleep(20);
				TextColor(4);
				gotoxy(100, 20);
				cout << "WRONG MOVE !!!";
				Sleep(100);
				TextColor(15);
				gotoxy(100, 20);
				cout << "WRONG MOVE !!!";
				Sleep(20);
				continue;
			}
			board_now[x_now][y_now] = turn + 1;
			Put(turn, x_now, y_now);
			turn = 1 - turn;
			new_save.xo_list.pb(ii(x_now, y_now));
			new_save.turn_game = turn;
			if (Check_Board(x_now, y_now) >= 4) { // Check_Win_Game
				TextColor(7);
				gotoxy(x_max / 2 - 5, y_max / 2 + 13);
				if (turn == 1){
					new_save.name = new_save.p1;
					++Rank[new_save.p1];
					REP(i_write, new_save.p1.length()) {
						cout << new_save.p1[i_write];
						Sleep(200);
					}
				}
				else{
					new_save.name = new_save.p2;
					++Rank[new_save.p2];
					REP(i_write, new_save.p2.length()) {
						cout << new_save.p2[i_write];
						Sleep(200);
					}
				}
				new_save.size_list = new_save.xo_list.size();
				new_save.cur_time = Time();
				history.push_back(new_save);
				cout << " WIN !!!\n\n";
				Rewrite_File_Save();
				if (Enable_Music)
					PlaySound(L"WIN.WAV", NULL, SND_SYNC);
				ClearConsoleInputBuffer();
				return;
			}
			
		}
		if (GetAsyncKeyState(VK_ESCAPE) & 1) {
			ClearConsoleInputBuffer();
			TextColor(14);
			system("cls");
			new_save.size_list = new_save.xo_list.size();
			cout << "NOTE : If you do not want to save, please write 'NONE' \n\n";
			cout << "Game Name : ";
			getline(cin, new_save.name);
			if (new_save.name == "NONE") {
				return;
			}
			new_save.cur_time = Time();
			file_save.pb(new_save);
			return;
		}
		x_now = max(x_now, 1);
		y_now = max(y_now, 1);
		x_now = min(x_now, size_w);
		y_now = min(y_now, size_h);
		gotoxy((x_now - 1) * 4 + 3, y_now * 2);
		if (!turn)
			TextColor(4);
		else
			TextColor(1);
		cout << type[turn];
		Sleep(100);
		TextColor(7);
		gotoxy((x_now - 1) * 4 + 3, y_now * 2);
		if (board_now[x_now][y_now] == 0)
			cout << "_";
		else {
			if (board_now[x_now][y_now] == 1)
				TextColor(4);
			else
				TextColor(1);
			cout << type[board_now[x_now][y_now] - 1];
		}
		++count_play;
		Sleep(100);
	}
}

ii Bot(int level, int x, int y) {
	int x_next = 0, y_next;
	vector <ii> next_move;
	if (level == 1) {
		FOR(i, 1, 18) {
			bool found = 0;
			REP(j, 8) {
				int x_ = x + dx[j] * i;
				int y_ = y + dy[j] * i;
				if (x_ > 0 && x_ <= 18 && y_ > 0 && y_ <= 15 && !board_now[x_][y_]) {
					board_now[x_][y_] = 1;
					if (Check_Board(x_, y_) >= 3) {
						x_next = x_;
						y_next = y_;
						next_move.clear();
						next_move.push_back(ii(x_, y_));
						found = 1;
						board_now[x_][y_] = 0;
						break;
					}
					board_now[x_][y_] = 0;
					next_move.push_back(ii(x_, y_));
					found = 1;
				}
			}
			if (found)
				break;
		}
	}

	if (level == 2) {
		ii dang = ii(0, 0); // dangerous of this move
		ii dang_move = ii(0, 0);
		bool found = 0;
		FOR(x_, 1, 18) {
			FOR(y_, 1, 15) {
				if (board_now[x_][y_] == 0) {
					board_now[x_][y_] = 2;
					dang_move.first = Check_Board(x_, y_);
					board_now[x_][y_] = 1;
					dang_move.second = Check_Board(x_, y_);
					board_now[x_][y_] = 0;
					if (dang == dang_move)
						next_move.push_back(ii(x_, y_));
					if (max(dang_move.first, dang_move.second) > max(dang.first, dang.second)
						|| (max(dang_move.first, dang_move.second) == max(dang.first, dang.second)
							&& dang_move.first > dang.first)
						|| (max(dang_move.first, dang_move.second) == max(dang.first, dang.second)
							&& (dang_move.first == dang.first) && (dang_move.second > dang.second ))) {
						dang = dang_move;
						next_move.clear();
						next_move.push_back(ii(x_, y_));
						x_next = x_;
						y_next = y_;
					}
				}
			}
		}
	}

	if (level == 3) {
		dang_info dang, dang_move ; // dangerous of this move
		dang.fi = dang.se = dang.th = dang.fo = 0;
		dang_move.fi = dang_move.se = dang_move.th = dang_move.fo = 0;
		bool found = 0;
		FOR(x_, 1, 18) {
			FOR(y_, 1, 15) {
				if (board_now[x_][y_] == 0) {
					board_now[x_][y_] = 2;
					dang_move.fi = Check_Board(x_, y_);
					dang_move.th = Check_Board_Hard(x_, y_);
					board_now[x_][y_] = 1;
					dang_move.se = Check_Board(x_, y_);
					dang_move.fo = Check_Board_Hard(x_, y_);
					board_now[x_][y_] = 0;
					if (equal(dang, dang_move))
						next_move.push_back(ii(x_, y_));
					if (max(dang_move.first, dang_move.second) > max(dang.first, dang.second)
						|| (max(dang_move.first, dang_move.second) == max(dang.first, dang.second)
							&& dang_move.first > dang.first)
						|| (max(dang_move.first, dang_move.second) == max(dang.first, dang.second)
							&& (dang_move.first == dang.first) && (dang_move.second > dang.second))
						|| (dang_move.first == dang.first && dang_move.second == dang.second && dang_move.th < dang.th)
				        || (dang_move.first == dang.first && dang_move.second == dang.second && dang_move.th == dang.th && dang_move.fo < dang.fo)) {
							dang = dang_move;
							next_move.clear();
							next_move.push_back(ii(x_, y_));
							x_next = x_;
							y_next = y_;
					}
				}
			}
		}
	}
	int select = Rand(0, next_move.size() - 1);
	x_next = next_move[select].first;
	y_next = next_move[select].second;
	Put(1, x_next, y_next);
	board_now[x_next][y_next] = 2;
	return ii(x_next, y_next);
}

void PvC_Mode(int id_game) {
	system("cls");
	save new_save; // new_save_game
	int level;    // level of game ( easy, med, hard )
	x_now = size_w / 2, y_now = size_h / 2; // init x and y postition
	memset(board_now, 0, sizeof(board_now));
	int count_play = 0;
	int turn;
	if (id_game == 0) {
		new_save.size_h = size_h;
		new_save.size_w = size_w;
		TextColor(10);
		gotoxy(x_max / 2 - 2, y_max / 2 - 7);
		cout << "1. Easy";
		Sleep(100);
		TextColor(14);
		gotoxy(x_max / 2 - 2, y_max / 2 - 5);
		cout << "2. Medium";
		Sleep(100);
		TextColor(12);
		gotoxy(x_max / 2 - 2, y_max / 2 - 3);
		cout << "3. Hard";
		Sleep(100);
		TextColor(11);
		gotoxy(x_max / 2 - 10, y_max / 2 - 1);
		cout << "Select difficulty level: ";
		Cursortype();
		cin >> level;
		system("cls");
		Board(size_w, size_h); // BOARD 18 x 15
		turn = 0;
		gotoxy(100, 3);
		TextColor(4);
		cout << "Player X : ";
		TextColor(14);
		getline(cin, new_save.p1);
		getline(cin, new_save.p1);
		TextColor(1);
		gotoxy(100, 5);
		cout << "Player O : ";
		TextColor(14);
		new_save.p2 = "Computer level ";
		new_save.p2 += (char)(level + '0');
		new_save.type = level;
		REP(i_name, new_save.p2.length()) {
			cout << new_save.p2[i_name];
			Sleep(20);

		}
		TextColor(11);
		gotoxy(77, y_max);
		cout << "Press ESC to Save and Exit";
		TextColor(7);
		gotoxy(100, 20);
		cout << "WRONG MOVE !!!";
		Nocursortype();
	}
	else {
		TextColor(11);
		gotoxy(77, y_max);
		cout << "Press ESC to Save and Exit";
		TextColor(7);
		gotoxy(100, 20);
		cout << "WRONG MOVE !!!";
		Nocursortype();
		// Load old game;
		//1. Load name
		new_save = file_save[id_game - 1];
		level = file_save[id_game - 1].type;
		gotoxy(100, 3);
		TextColor(4);
		cout << "Player X : ";
		TextColor(14);
		REP(i_p1, new_save.p1.length()) {
			cout << new_save.p1[i_p1];
			Sleep(50);
		}
		TextColor(1);
		gotoxy(100, 5);
		cout << "Player O : ";
		TextColor(14);
		REP(i_p2, new_save.p2.length()) {
			cout << new_save.p2[i_p2];
			Sleep(50);
		}
		//2. Load Board;
		turn = 0;
		size_w = new_save.size_w;
		size_h = new_save.size_h;
		Board(size_w, size_h); // BOARD 18 x 15
		REP(i_xo, new_save.xo_list.size()) {
			Put(turn, new_save.xo_list[i_xo].first, new_save.xo_list[i_xo].second);
			board_now[new_save.xo_list[i_xo].first][new_save.xo_list[i_xo].second] = turn + 1;
			turn = 1 - turn;
			Sleep(50);
		}
	}
	// Game_loop
	while (1) {
		if (turn == 0) {
			if (GetAsyncKeyState(VK_UP) & 1)
				--y_now;
			if (GetAsyncKeyState(VK_DOWN) & 1)
				++y_now;
			if (GetAsyncKeyState(VK_LEFT) & 1)
				--x_now;
			if (GetAsyncKeyState(VK_RIGHT) & 1)
				++x_now;
			if (GetAsyncKeyState(VK_RETURN) & 1) {
				if (!count_play) {
					++count_play;
					continue;
				}
				if (board_now[x_now][y_now] != 0) {
					TextColor(4);
					gotoxy(100, 20);
					cout << "WRONG MOVE !!!";
					Sleep(100);
					TextColor(15);
					gotoxy(100, 20);
					cout << "WRONG MOVE !!!";
					Sleep(20);
					TextColor(4);
					gotoxy(100, 20);
					cout << "WRONG MOVE !!!";
					Sleep(100);
					TextColor(15);
					gotoxy(100, 20);
					cout << "WRONG MOVE !!!";
					Sleep(20);
					continue;
				}
				board_now[x_now][y_now] = turn + 1;
				Put(turn, x_now, y_now);
				turn = 1 - turn;
				new_save.xo_list.pb(ii(x_now, y_now));
				new_save.turn_game = turn;
				if (Check_Board(x_now, y_now) >= 4) { // CheckWin
					TextColor(7);
					gotoxy(x_max / 2 - 5, y_max / 2 + 13);
					if (turn == 1)
						REP(i_write, new_save.p1.length()) {
							cout << new_save.p1[i_write];
							Sleep(200);
						}
					cout << " WIN !!!\n\n";
					if (Enable_Music)
						PlaySound(L"WIN.WAV", NULL, SND_SYNC);
					ClearConsoleInputBuffer();
					return;
				}
			}
			if (GetAsyncKeyState(VK_ESCAPE) & 1) {
				ClearConsoleInputBuffer();
				TextColor(14);
				system("cls");
				new_save.size_list = new_save.xo_list.size();
				cout << "NOTE : If you do not want to save, please write 'NONE' \n\n";
				cout << "Game Name : ";
				getline(cin, new_save.name);
				if (new_save.name == "NONE") {
					return;
				}
				new_save.cur_time = Time();
				file_save.pb(new_save);
				return;
			}
			x_now = max(x_now, 1);
			y_now = max(y_now, 1);
			x_now = min(x_now, size_w);
			y_now = min(y_now, size_h);
			gotoxy((x_now - 1) * 4 + 3, y_now * 2);
			if (!turn)
				TextColor(4);
			else
				TextColor(1);
			cout << type[turn];
			Sleep(100);
			TextColor(7);
			gotoxy((x_now - 1) * 4 + 3, y_now * 2);
			if (board_now[x_now][y_now] == 0)
				cout << "_";
			else {
				if (board_now[x_now][y_now] == 1)
					TextColor(4);
				else
					TextColor(1);
				cout << type[board_now[x_now][y_now] - 1];
			}
			++count_play;
			Sleep(100);
		}
		else {
			ii next = Bot(level, x_now, y_now);
			turn = 1 - turn;
			new_save.xo_list.pb(next);
			new_save.turn_game = turn;
			if (Check_Board(next.first, next.se) >= 4){
				TextColor(7);
				gotoxy(x_max / 2 - 5, y_max / 2 + 13);
				cout << new_save.p2 << " WIN !!!\n\n";
				if (Enable_Music)
					PlaySound(L"LOSE.WAV", NULL, SND_SYNC);
				ClearConsoleInputBuffer();
				return;
			}
		}
	}

}

void Play(int id_game) {
	if (file_save[id_game-1].type == 0)
		PvP_Mode(id_game);
	else
		PvC_Mode(id_game);
}

void Load_Mode() {
	system("cls");
	TextColor(6);
	gotoxy(1, 1);
	cout << "Game ID";
	gotoxy(20, 1);
	cout << "Game Name";
	gotoxy(50, 1);
	cout << "Game Mode";
	gotoxy(90, 1);
	cout << "Date And Time";
	REP(i_list, file_save.size()) {
		TextColor(11 + (i_list % 4));
		gotoxy(1, (i_list + 1) * 2 + 1 );
		cout << i_list + 1;
		gotoxy(20, (i_list + 1) * 2 + 1);
		cout << file_save[i_list].name;
		gotoxy(50, (i_list + 1) * 2 + 1);
		if (file_save[i_list].type == 0)
			cout << "PvP Mode";
		if (file_save[i_list].type == 1)
			cout << "PvC Mode (Easy)";
		if (file_save[i_list].type == 2)
			cout << "PvC Mode (Medium)";
		if (file_save[i_list].type == 3)
			cout << "PvC Mode (Hard)";

		gotoxy(90, (i_list + 1) * 2 + 1 );
		cout << file_save[i_list].cur_time << "\n\n";
		Sleep(50);
	}
	gotoxy(1, (file_save.size() + 1) * 2 + 1);
	TextColor(7);
	cout << "NOTE    : Write 'EXIT' to exit \n\n";
	cout << "DELETE  : Write 'DEL' + your game id if you want to delete this game !!! \n\n";
	cout << "Example : 'DEL 1' or '1'\n\n";
	cout << "Please write your selection here : ";
	string id_choice;
	int id_game = 0;
	Cursortype();
	getline(cin, id_choice);

	Nocursortype();
	if (id_choice[0] == 'D') {
		cerr << id_choice << "\n";
		FOR(i, 4, id_choice.length() - 1) {
			id_game = id_game * 10 + (id_choice[i] - '0');
		}
		cout << id_game << "\n";
		file_save.erase(file_save.begin() + id_game - 1);
		Rewrite_File_Save();
		Load_Mode();
	}
	else {
		if (id_choice[0] == 'E') {
			return;
		}
		REP(i, id_choice.length()) {
			id_game = id_game * 10 + (id_choice[i] - '0');
		}
		Play(id_game);
	}
	string clear_line;
	getline(cin, clear_line);
}

void Statictics() {
	while (1) {
		system("cls");
		int select = 0;
		TextColor(9);
		gotoxy(x_max / 2 - 4, y_max / 2 - 6);
		cout << "1. Ranking";
		TextColor(10);
		gotoxy(x_max / 2 - 4, y_max / 2 - 4);
		cout << "2. History";
		gotoxy(x_max / 2 - 4, y_max / 2 - 2);
		TextColor(11);
		cout << "3. Back to Menu";
		gotoxy(x_max / 2 - 7, y_max / 2);
		TextColor(14);
		cout << "Your Selection : ";
		Cursortype();
		cin >> select;
		if (select == 1) {
			system("cls");
			gotoxy(50, 1);
			TextColor(11);
			cout << "RANK";
			gotoxy(70, 1);
			cout << "PLAYER";
			gotoxy(100, 1);
			cout << "NUMBER OF GAME WON";
			map<string, int>::iterator it = Rank.begin(); // save Ranking list by map < string , int > 
			vector <pair<string, int>> Rank_Sorted; // Sort map 
			while (it != Rank.end()) {
				Rank_Sorted.push_back(make_pair(it->first, it->second));
				++it;
			}
			sort(Rank_Sorted.begin(), Rank_Sorted.end(), comp);
			REP(cnt, min(Rank_Sorted.size(), 20)) { // top 20 player
				if (cnt == 0)
					TextColor(14);
				if (cnt == 1)
					TextColor(8);
				if (cnt == 2)
					TextColor(6);
				gotoxy(50, cnt * 2 + 3);
				cout << cnt + 1;
				gotoxy(70, cnt * 2 + 3);
				cout << Rank_Sorted[cnt].first;
				gotoxy(100, cnt * 2 + 3);
				cout << Rank_Sorted[cnt].second;
				TextColor(7);
			}
			ClearConsoleInputBuffer();
		}
		if (select == 2) {
			system("cls");
			TextColor(10);
			gotoxy(1, 1);
			cout << "GAME ID";
			gotoxy(30, 1);
			cout << "PLAYER 1";
			gotoxy(60, 1);
			cout << "PLAYER 2";
			gotoxy(90, 1);
			cout << "WINNER";
			gotoxy(140, 1);
			cout << "DATE AND TIME";
			REP(i, history.size()) {
				TextColor(11 + (i % 4));
				gotoxy(1, i * 2 + 3);
				cout << i + 1;
				gotoxy(30, i * 2 + 3);
				cout << history[i].p1;
				gotoxy(60, i * 2 + 3);
				cout << history[i].p2;
				gotoxy(90, i * 2 + 3);
				cout << history[i].name;
				gotoxy(140, i * 2 + 3);
				cout << history[i].cur_time;
			}
			int id_game;
			TextColor(7);
			gotoxy(1, history.size() * 2 + 3);
			cout << "Select game you want to show ( Write '0' if you want to go back ) : ";
			cin >> id_game;
			if (id_game) {
				system("cls");
				Board(history[id_game - 1].size_w, history[id_game - 1].size_h);
				gotoxy(100, 3);
				TextColor(4);
				cout << "Player 1 : ";
				TextColor(14);
				REP(i_p1, history[id_game - 1].p1.length()) {
					cout << history[id_game-1].p1[i_p1];
					Sleep(50);
				}
				TextColor(1);
				gotoxy(100, 5);
				cout << "Player 2 : ";
				TextColor(14);
				REP(i_p2, history[id_game-1].p2.length()) {
					cout << history[id_game-1].p2[i_p2];
					Sleep(50);
				}
				//2. Load Board;
				int turn = 0;
				REP(i_xo, history[id_game-1].xo_list.size()) {
					Put(turn, history[id_game-1].xo_list[i_xo].first, history[id_game-1].xo_list[i_xo].second);
					board_now[history[id_game-1].xo_list[i_xo].first][history[id_game-1].xo_list[i_xo].second] = turn + 1;
					turn = 1 - turn;
					Sleep(50);
				}
				ClearConsoleInputBuffer();
			}
			else {
				continue;
			}
		}
		if (select == 3)
			return;
	}
	return;

}

void Option_Mode() {
	while (1) {
		string Menu_Option[6];
		int Mode;
		Menu_Option[0] = "1. Change Player Icon \n\n";
		Menu_Option[1] = "2. Change Gameboard Size\n\n";
		Menu_Option[2] = "3. Turn On/Off Sound anh Background Music\n\n";
		Menu_Option[3] = "4. About me";
		Menu_Option[4] = "5. Exit\n\n";
		Menu_Option[5] = "Your Selection: ";
		system("cls");
		REP(i, 6) {
			TextColor(10 + i);
			gotoxy(x_max / 2 - 14, y_max / 2 + i * 2 - 11);
			if (i == 5)
				gotoxy(x_max / 2 - 5, y_max / 2 + i * 2 - 10);
			cout << Menu_Option[i];
		}
		Cursortype();
		gotoxy(x_max / 2 - 5 + Menu_Option[5].length(), y_max / 2);
		cin >> Mode;
		if (Mode == 1) {
			system("cls");
			TextColor(14);
			cout << "Player X : ";
			cin >> playerX;
			cout << "\nPlayer O : ";
			cin >> playerO;
			type[0] = playerX;
			type[1] = playerO;
			continue;
		}
		if (Mode == 2) {
			system("cls");
			TextColor(14);
			cout << "Col ( 10 ~ 24 ): ";
			cin >> size_w;
			cout << "\nRow ( 10 ~ 20 ): ";
			cin >> size_h;
			continue;
		}
		if (Mode == 3) {
			system("cls");
			cout << "Turn On/Off Music ( 1. On / 0. Off ) : ";
			cin >> Enable_Music;
			continue;
		}
		if (Mode == 4) {
			system("cls");
			TextColor(14);
			cout << "Version 1.0 Of Caro Game \n\n";
			cout << "ID   : 18125017\n\n";
			cout << "Name : La Hoang Phong \n\n";
			continue;
		}
		if (Mode == 5) {
			return;
		}
	}
	return;
}

void Menu() {
	char Center_Text[5][25] = { N,F,F,F,N,N,N,N,F,F,N,N,N,N,F,F,F,N,N,N,N,F,F,F,N
						  ,F,N,N,N,N,N,N,F,N,N,F,N,N,N,F,N,N,F,N,N,F,N,N,N,F
						  ,F,N,N,N,N,N,N,F,F,F,F,N,N,N,F,F,F,F,N,N,F,N,N,N,F
						  ,F,N,N,N,N,N,F,N,N,N,N,F,N,N,F,N,F,N,N,N,F,N,N,N,F
						  ,N,F,F,F,N,N,F,N,N,N,N,F,N,N,F,N,N,F,N,N,N,F,F,F,N
	};
	string Menu_simple_[7];
	string Version = "Version 1.0 - Made By LaHoangPhong";
	Menu_simple_[0] = "1. PvP Mode			";
	Menu_simple_[1] = "2. PvC Mode			";
	Menu_simple_[2] = "3. Load Game        ";
	Menu_simple_[3] = "4. Player Statistics";
	Menu_simple_[4] = "5. Option";
	Menu_simple_[5] = "6. Exit";
	Menu_simple_[6] = "Choose Your Mode Here : ";
	while (1){
		if (Enable_Music)
			PlaySound(L"BACKGROUND.WAV", NULL, SND_ASYNC);
		ClearConsoleInputBuffer();
		Load_File_Save();
		system("cls");
		string mode ;
		TextColor(6);
		REP(i, 5) {
			gotoxy(x_max / 2 - 12, y_max / 2 + i - 11);
			REP(j, 25)
				cout << Center_Text[i][j];
		}
		TextColor(11);
		REP(i, 7) {
			if (i < 6) {
				gotoxy(x_max / 2 - 6, y_max / 2 - 5 + i * 2);
				cout << Menu_simple_[i];
			}
			else {
				TextColor(1);
				gotoxy(x_max / 2 - 10, y_max / 2 - 5 + i * 2 + 1);
				cout << Menu_simple_[i];
			}
		}
		REP(rep_for_my_info, 1) {
			TextColor(14);
			gotoxy(x_max / 2 - Version.length() / 2, y_max);
			cout << Version;
		}
		gotoxy(x_max / 2 - 10 + Menu_simple_[6].length(), y_max / 2 + 8);
		TextColor(12);
		Cursortype();
		getline(cin, mode);
		PlaySound(0, NULL, 0);
		Nocursortype();
		if (mode[0] == '1') {
			PvP_Mode(0);
		}
		if (mode[0] == '2') {
			PvC_Mode(0);
		}
		if (mode[0] == '3') {
			Load_Mode();
		}
		if (mode[0] == '4') {
			Statictics();
		}
		if (mode[0] == '5') {
			Option_Mode();
		}
		Rewrite_File_Save();
		if (mode[0] == '6') {
			TextColor(7);
			system("cls");
			gotoxy(x_max / 2 - 5, y_max / 2);
			cout << "Press any key to Exit !!!";
			exit(0);
		}
	}

}

int main() {
	srand(time(NULL));
	ResizeConsole(w, h);
	Nocursortype();
	SplashScreen();
	Menu();
	cin.get();
	return 0;
}
