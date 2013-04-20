// Google Code 2013 Qualification Problems
// A - Tic Tac Toe Tomek
//
// Adrian Dale 13/04/2013
/*
https://code.google.com/codejam/contest/2270488/dashboard#s=p0
*/

#include <iostream>
#include <string>
#include <sstream>
using namespace std;

char Board[4][4];

// Return true if winner found
bool checkLine(char *line, string &result)
{
	int XCount = 0;
	int OCount = 0;
	for(int x=0; x<4; ++x)
	{
		char cell = line[x];
		if (cell == '.')
		{
			// Early finish as clearly nobody won this row
			result = "Game has not completed";
			return false;
		}
		else if (cell == 'X')
			++XCount;
		else if (cell == 'O')
			++OCount;
		else if (cell == 'T')
		{
			++XCount;
			++OCount;
		}
	}

	if (XCount == 4)
	{
		result = "X won";
		return true;
	}
	else if (OCount == 4)
	{
		result = "O won";
		return true;
	}

	// Possible draw
	result = "Draw";
	return false;
}

// Solve the case currently held in Board
string solveTestCase()
{
	bool spacesFound = false;
	string result;
	bool checkVal;

	// Check rows
	for(int y=0; y<4; ++y)
	{
		char row[4];
		for(int x=0; x<4; ++x)
			row[x] = Board[x][y];
		checkVal = checkLine(row, result);
		if (checkVal)
			return result;
		else
		{
			if (result == "Game has not completed")
				spacesFound = true;
		}
	}

	// Check cols
	for(int x=0; x<4; ++x)
	{
		char col[4];
		for(int y=0; y<4; ++y)
			col[y] = Board[x][y];
		checkVal = checkLine(col, result);
		if (checkVal)
			return result;
		else
		{
			if (result == "Game has not completed")
				spacesFound = true;
		}
	}

	// Check diags
	char diagLR[4];
	
	for(int x=0; x<4; ++x)
		diagLR[x] = Board[x][x];
	checkVal = checkLine(diagLR, result);
	if (checkVal)
		return result;
	else
	{
		if (result == "Game has not completed")
			spacesFound = true;
	}

	char diagRL[4];
	for(int x=0; x<4; ++x)
		diagRL[x] = Board[3-x][x];
	checkVal = checkLine(diagRL, result);
	if (checkVal)
		return result;
	else
	{
		if (result == "Game has not completed")
			spacesFound = true;
	}

	if (spacesFound)
		return "Game has not completed";
	return "Draw";
}

void ReadTestCase()
{
	static int testNo = 1;
	string inStr;
	// Read in Board
	for(int y=0; y<4; ++y)
	{
		getline(cin, inStr);
		for(int x=0; x<4; ++x)
			Board[x][y] = inStr[x];
	}
	
	cout << "Case #" << testNo++ << ": " << solveTestCase() << endl;
	
	// Skip blank line after each test case
	getline(cin, inStr);
}

void ReadInput()
{
	int T=0;
	string line;
	getline(cin, line);
	istringstream parser(line);
	parser >> T;
	while( T-- > 0 )
		ReadTestCase();
}

int main()
{
	ReadInput();
	return 0;
}