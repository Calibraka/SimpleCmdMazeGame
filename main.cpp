#include<iostream>
#include<set>
#include<time.h>
#include<Windows.h>
#include<stdlib.h>
#include<utility>
#include<fstream>
#include<vector>
#include<string>
#include<conio.h>
#include<sstream>

using namespace::std;

#define ROAD 0
#define WALL 1
#define MONSTER 2
#define GOLD 3
#define PLAYER 4
#define WIN 5

pair<int, int> getDimensions() {
	pair<int, int> dimensions = { 0, 0 };
	while (1) {
		if (dimensions.first < 10 || dimensions.first > 100) {
			cout << "Please enter the length (Must be within 10-100):";
			cin >> dimensions.first;
		}
		if (dimensions.first <= 100 && dimensions.first >= 10)
			break;
	}
	while (1) {
		if (dimensions.second < 10 || dimensions.second > 100) {
			cout << "Please enter the width (Must be within 10-100):";
			cin >> dimensions.second;
		}
		if (dimensions.second <= 100 && dimensions.second >= 10)
			break;
	}
	return dimensions;
}

void fillMaze(int** maze, int length, int width) {
	for (int i = 0; i < length; i++)
	{
		for (int j = 0; j < width; j++)
		{
			maze[i][j] = WALL;
		}
	}
}

bool isVisited(int y, int x, set<pair<int, int>> visited) {
	if (visited.count({ y, x }))
		return true;
	else {
		return false;
	}
}

bool isInBounds(int length, int width, int y, int x) {
	if (y < 0 || y >= length ) return false;
	if (x < 0 || x >= width ) return false;
	return true;
}

pair<int, int> getNeighbor(int length, int width, int y, int x, set<pair<int, int>> visited) {
	set<pair<int, int>> available;
	if (isInBounds(length, width, y + 1, x) == true) {
		if (isVisited(y + 1, x, visited) == false)
			available.insert({ y + 1, x });
	}
	if (isInBounds(length, width, y + -1, x) == true) {
		if (isVisited(y - 1, x, visited) == false)
			available.insert({ y + -1, x });
	}
	if (isInBounds(length, width, y, x + 1) == true) {
		if (isVisited(y, x + 1, visited) == false)
			available.insert({ y, x + 1 });
	}
	if (isInBounds(length, width, y, x - 1) == true) {
		if (isVisited(y, x - 1, visited) == false)
			available.insert({ y, x - 1 });
	}
	//for (auto const& x : available) {
	//	cout << x.first << "," << x.second << endl;
	//}
	int size = available.size();

	if (size > 0) {
		int random = rand() % size;
		pair<int, int> returnedPair;
		set < pair < int, int > > ::iterator it = available.begin();
		advance(it, random);
		returnedPair.first = it->first;
		returnedPair.second = it->second;
		available.clear();
		return returnedPair;
	}
	else {
		pair<int, int> returnedPair = { y + 1, x };
		return returnedPair;
	}
}

void addRoad(int** maze, int y, int x, int length) {
	if (y == length - 1) {
		maze[y][x] = WIN;
	}
	else {
		maze[y][x] = ROAD;
	}
}

void populateRoad(int** maze, int length, int width, int Y, int X) {
	int curY = Y, curX = X;
	set<pair<int, int>> visited;
	addRoad(maze, curY, curX, length);
	visited.insert({ curY, curX });
	while (1)
	{
		pair<int, int> cur = getNeighbor(length, width, curY, curX, visited);
		curY = cur.first, curX = cur.second;
		addRoad(maze, curY, curX, length);
		visited.insert({ curY, curX });
		if (curY == length - 1) {
			break;
		}
	}
}

void addGold(int** maze, int length, int width) {
	int maxGoldCount = 0;
	while (1) {
		int randy = rand() % length;
		int randx = rand() % width;
		if (maze[randy][randx] == 0) {
			maze[randy][randx] = GOLD;
			maxGoldCount++;
		}
		if (maxGoldCount == 4)
			break;
	}
}

bool acceptableTile(int** maze, int length, int width, int y, int x) {
	int wallcount = 0;
	if (isInBounds(length, width, y - 1, x) == true) {
		if (maze[y - 1][x] == 1 || maze[y - 1][x] == 2)
			wallcount++;
	}
	else
		wallcount++;
	if (isInBounds(length, width, y - 1, x - 1) == true) {
		if (maze[y - 1][x - 1] == 1 || maze[y - 1][x - 1] == 2)
			wallcount++;
	}
	else
		wallcount++;
	if (isInBounds(length, width, y - 1, x + 1) == true) {
		if (maze[y - 1][x + 1] == 1 || maze[y - 1][x + 1] == 2)
			wallcount++;
	}
	else
		wallcount++;
	if (isInBounds(length, width, y, x + 1) == true) {
		if (maze[y][x + 1] == 1 || maze[y][x + 1] == 2)
			wallcount++;
	}
	else
		wallcount++;
	if (isInBounds(length, width, y, x - 1) == true) {
		if (maze[y][x - 1] == 1 || maze[y][x - 1] == 2)
			wallcount++;
	}
	else
		wallcount++;
	if (isInBounds(length, width, y + 1 , x) == true) {
		if (maze[y + 1][x] == 1 || maze[y + 1][x] == 2)
			wallcount++;
	}
	else
		wallcount++;
	if (isInBounds(length, width, y + 1, x - 1) == true) {
		if (maze[y + 1][x - 1] == 1 || maze[y + 1][x - 1] == 2)
			wallcount++;
	}
	else
		wallcount++;
	if (isInBounds(length, width, y + 1, x + 1) == true) {
		if (maze[y + 1][x + 1] == 1 || maze[y + 1][x + 1] == 2)
			wallcount++;
	}
	else
		wallcount++;
	if (wallcount < 2) 
		return true;
	else return false;
}

void addMonster(int** maze, int length, int width) {
	int monsterCount = 0, counter = 0;
	while (1) {
		int randy = rand() % length;
		int randx = rand() % width;
		if (maze[randy][randx] == 0) {
			if (acceptableTile(maze, length, width, randy, randx) == true) {
				maze[randy][randx] = MONSTER;
				monsterCount++;
			}
		}
		if (monsterCount == 4 || counter == 200)
			break;
		counter++;
	}
}

void consoleClear() {
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	SetConsoleCursorPosition(console, topLeft);
}

void displayMaze(int** maze, int length, int width, int gc) {
	consoleClear();
	for (int i = 0; i < length; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (maze[i][j] == 0)
				cout << "[" << "-" << "]";
			if (maze[i][j] == 1)
				cout << (char)177 << (char)177 << (char)177;
			if (maze[i][j] == 2)
				cout << "[" << "M" << "]";
			if (maze[i][j] == 3)
				cout << "[" << "$" << "]";
			if (maze[i][j] == 4)
				cout << "[" << "P" << "]";
			if (maze[i][j] == 5)
				cout << (char)205 << (char)203 << (char)205;
		}
		cout << endl;

	}
	cout << "Total gold coins:" << gc << endl;
}

void printFile(vector<vector<int>> pmov) {
	ofstream output("output.txt");
	output << "x,y" << endl;
	for (vector<int> vect1D : pmov)
	{
		for (int x : vect1D)
		{
			output << x << " ";
		}
		output << endl;
	}
}

void playerMovement(int** maze, int length, int width) {
	maze[0][0] = PLAYER;
	vector<vector<int>> pmov;
	int posY = 0, posX = 0, gc = 0;
	pmov.push_back({ posX, posY });
	displayMaze(maze, length, width, gc);
	string smov;
	int movement;
	while (1) {
		cout << "Please enter which way you want to go(2:down, 4:left, 6:right, 8:up):";
		smov = _getch();
		stringstream ssmov(smov);
		ssmov >> movement;
		if (movement == 8) {
			if (isInBounds(length, width, posY - 1, posX) == true) {
				maze[posY][posX] = 0;
				if (maze[posY - 1][posX] == 1) {
					continue;
				}
				else if (maze[posY - 1][posX] == 2) {
					maze[posY - 1][posX] = 0;
					maze[0][0] = PLAYER;
					posY = 0, posX = 0;
				}
				else if (maze[posY - 1][posX] == 3) {
					maze[posY - 1][posX] = PLAYER;
					posY--;
					pmov.push_back({ posX, posY });
					gc++;
				}
				else if (maze[posY - 1][posX] == 5) {
					printFile(pmov);
					consoleClear();
					cout << "You win!" << endl;
					break;
				}
				else if (maze[posY - 1][posX] == 0) {
					maze[posY - 1][posX] = PLAYER;
					posY--;
					pmov.push_back({ posX, posY });
				}
				displayMaze(maze, length, width, gc);


			}
			else continue;
		}
		if (movement == 4) {
			if (isInBounds(length, width, posY, posX - 1) == true) {
				maze[posY][posX] = 0;
				if (maze[posY][posX - 1] == 1) {
					continue;
				}
				else if (maze[posY][posX - 1] == 2) {
					maze[posY][posX - 1] = 0;
					maze[0][0] = PLAYER;
					posY = 0, posX = 0;
				}
				else if (maze[posY][posX - 1] == 3) {
					maze[posY][posX - 1] = PLAYER;
					posX--;
					pmov.push_back({ posX, posY });
					gc++;
				}
				else if (maze[posY][posX - 1] == 5) {
					printFile(pmov);
					consoleClear();
					cout << "You win!" << endl;
					break;
				}
				else if (maze[posY][posX - 1] == 0) {
					maze[posY][posX - 1] = PLAYER;
					posX--;
					pmov.push_back({ posX, posY });
				}
				displayMaze(maze, length, width, gc);

			}
			else continue;
		}
		if (movement == 6) {
			if (isInBounds(length, width, posY, posX + 1) == true) {
				maze[posY][posX] = 0;
				if (maze[posY][posX + 1] == 1) {
					continue;
				}
				else if (maze[posY][posX + 1] == 2) {
					maze[posY][posX + 1] = 0;
					maze[0][0] = PLAYER;
					posY = 0, posX = 0;
				}
				else if (maze[posY][posX + 1] == 3) {
					maze[posY][posX + 1] = PLAYER;
					posX++;
					pmov.push_back({ posX, posY });
					gc++;
				}
				else if (maze[posY][posX + 1] == 5) {
					printFile(pmov);
					consoleClear();
					cout << "You win!" << endl;
					break;
				}
				else if (maze[posY][posX + 1] == 0) {
					maze[posY][posX + 1] = PLAYER;
					posX++;
					pmov.push_back({ posX, posY });
				}
				displayMaze(maze, length, width, gc);
			}
			else continue;
		}
		if (movement == 2) {
			if (isInBounds(length, width, posY + 1, posX) == true) {
				maze[posY][posX] = 0;
				if (maze[posY + 1][posX] == 1) {
					continue;
				}
				else if (maze[posY + 1][posX] == 2) {
					maze[posY + 1][posX] = 0;
					maze[0][0] = PLAYER;
					posY = 0, posX = 0;
				}
				else if (maze[posY + 1][posX] == 3) {
					maze[posY + 1][posX] = PLAYER;
					posY++;
					pmov.push_back({ posX, posY });
					gc++;
				}
				else if (maze[posY + 1][posX] == 0) {
					maze[posY + 1][posX] = PLAYER;
					posY++;
					pmov.push_back({ posX, posY });
				}
				else if(maze[posY + 1][posX] == 5) {
					printFile(pmov);
					consoleClear();
					cout << "You win!" << endl;
					break;
				}		
				displayMaze(maze, length, width, gc);
			}
			else continue;
		}
	}
}

int main() {
	srand(time(0));
	pair<int, int> dim = getDimensions();
	int length = dim.first, width = dim.second;
	int** maze = new int* [length];
	for (int i = 0; i < length; i++)
		maze[i] = new int[width];

	fillMaze(maze, length, width);
	populateRoad(maze, length, width, 0, 0);
	addGold(maze, length, width);
	addMonster(maze, length, width);
	playerMovement(maze, length, width);

}
