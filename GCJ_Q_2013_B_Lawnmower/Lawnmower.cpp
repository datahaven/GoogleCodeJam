// Google Code 2013 Qualification Problems
// B - Lawnmower
//
// Adrian Dale 13/04/2013
/*
https://code.google.com/codejam/contest/2270488/dashboard#s=p1
*/

#include <iostream>
#include <string>
#include <sstream>
using namespace std;

const int MAX_LAWN = 100;

int Lawn[MAX_LAWN][MAX_LAWN];
int N, M;

string solveTestCase()
{
#ifdef _DEBUG
	for(int i=0; i<N; ++i)
	{
		for(int j=0; j<M; ++j)
		{
			cout << Lawn[i][j] << " ";
		}
		cout << endl;
	}
#endif

	// Get max heights for each row/col in desired lawn plan
	int rowMax[MAX_LAWN];
	for(int i=0; i<N; ++i)
	{
		int maxG = 0;
		for(int j=0; j<M; ++j)
			maxG = max(maxG, Lawn[i][j]);
		rowMax[i] = maxG;
	}

	int colMax[MAX_LAWN];
	for(int i=0; i<M; ++i)
	{
		int maxG = 0;
		for(int j=0; j<N; ++j)
			maxG = max(maxG, Lawn[j][i]);
		colMax[i] = maxG;
	}

#ifdef _DEBUG
	cout << "rowMax: ";
	for(int i=0; i<N; ++i)
	{
		cout << rowMax[i] << " ";
	}
	cout << endl;

	cout << "colMax: ";
	for(int i=0; i<M; ++i)
	{
		cout << colMax[i] << " ";
	}
	cout << endl;
#endif

	// Mow a test lawn with max row/col heights
	int testLawn[MAX_LAWN][MAX_LAWN];
	for(int i=0;i<N; ++i)
	{
		for(int j=0; j<M; ++j)
		{
			testLawn[i][j] = 100;
		}
	}
	// Doesn't matter what order we mow in
	for(int i=0; i<N; ++i)
	{
		for(int j=0; j<M; ++j)
		{
			if (testLawn[i][j] > rowMax[i])
				testLawn[i][j] = rowMax[i];
		}
	}
	for(int j=0; j<M; ++j)
	{
		for(int i=0; i<N; ++i)
		{
			if (testLawn[i][j] > colMax[j])
				testLawn[i][j] = colMax[j];
		}
	}

#ifdef _DEBUG
	cout << "testLawn:" << endl;
	for(int i=0; i<N; ++i)
	{
		for(int j=0; j<M; ++j)
		{
			cout << testLawn[i][j] << " ";
		}
		cout << endl;
	}
#endif

	// See if test lawn matches desired lawn
	for(int i=0; i<N; ++i)
	{
		for(int j=0; j<M; ++j)
		{
			if (testLawn[i][j] != Lawn[i][j])
				return "NO";
		}
	}
	return "YES";
}

void ReadTestCase()
{
	static int testNo = 1;
	string inStr;
	getline(cin, inStr);
	istringstream parser(inStr);
	
	parser >> N >> M;
	for(int i=0; i<N; ++i)
	{
		getline(cin, inStr);
		parser.clear();
		parser.str(inStr);
		for(int x=0; x<M; ++x)
		{
			int h;
			parser >> h;
			Lawn[i][x] = h;
		}
	}
	
	cout << "Case #" << testNo++ << ": " << solveTestCase() << endl;
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