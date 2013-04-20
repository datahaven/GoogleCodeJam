// Google Code 2010 1A Problems (Practice)
// A - Rotate
//
// Adrian Dale 13/04/2013
/*
http://code.google.com/codejam/contest/544101/dashboard#s=p0
*/

#include <iostream>
#include <string>
#include <sstream>
using namespace std;

const int NMAX = 50;
int N;
int K;
char Board[NMAX+1][NMAX];

void doGravity()
{
	// Simulate the rotation of the board, then gravity.
	// As a shortcut, just move all pieces to the right,
	// instead of wasting time simulating the rotation, too
	// NB Sentinel takes care of column edge
	// Below code is a bit slow but ought to do the job for now
	for(int y=0; y<N; ++y)
		for(int k=0; k<N-1; ++k) // Max dist a piece can fall.
			for(int x=N; x>=1; --x)
				if (Board[x][y]=='.')
				{
					Board[x][y]=Board[x-1][y];
					Board[x-1][y]='.';
				}
}

void pieceCounts(char &prevPiece, char thisPiece, int &blueCount, int &redCount, bool &blueWin, bool &redWin)
{
	if (thisPiece=='.' || thisPiece != prevPiece)
	{
		blueCount = redCount = 0;
		prevPiece = thisPiece;
	}

	if (thisPiece=='R') ++redCount;
	else if (thisPiece=='B') ++blueCount;
	
	if (redCount >= K)
		redWin = true;
	if (blueCount >= K)
		blueWin = true;
}

// Solve the case currently held in global Board, N, K
string solveTestCase()
{
	doGravity();
#if 0
	cout << "N=" << N << " K=" << K << endl;
	for(int y=0; y<N; ++y)
	{
		for(int x=0; x<N; ++x)
		{
			cout << Board[x+1][y];
		}
		cout << endl;
	}
#endif

	bool redWin = false;
	bool blueWin = false;

	// Horrible code duplication here!

	// Check rows for win
	for(int y=0; y<N; ++y)
	{
		int bCount = 0;
		int rCount = 0;
		char prevPiece='.';
		for(int x=1; x<=N; ++x)
			pieceCounts(prevPiece, Board[x][y], bCount, rCount, blueWin, redWin);
	}

	// Check cols for win
	for(int x=1; x<=N; ++x)
	{
		int bCount = 0;
		int rCount = 0;
		char prevPiece='.';
		for(int y=0; y<N; ++y)
			pieceCounts(prevPiece, Board[x][y], bCount, rCount, blueWin, redWin);
	}

	// Check diagonals for win - starting at each y=0
	for(int x=1; x<=N; ++x)
	{
		int bCount = 0;
		int rCount = 0;
		char prevPiece='.';
		for(int i=0; i<N; ++i)
			pieceCounts(prevPiece, Board[x+i][i], bCount, rCount, blueWin, redWin);
	}
	// Check diagonals for win - starting at each y=0 (backwards)
	for(int x=1; x<=N; ++x)
	{
		int bCount = 0;
		int rCount = 0;
		char prevPiece='.';
		for(int i=0; i<N; ++i)
			pieceCounts(prevPiece, Board[x+i][N-1-i], bCount, rCount, blueWin, redWin);
	}
	// Check diagonals for win - starting at each x=0
	for(int y=0; y<N; ++y)
	{
		int bCount = 0;
		int rCount = 0;
		char prevPiece='.';
		for(int i=0; i<N; ++i)
			pieceCounts(prevPiece, Board[i+1][y+i], bCount, rCount, blueWin, redWin);
	}
	// Check diagonals for win - starting at each x=0 (Backwards)
	for(int y=0; y<N; ++y)
	{
		int bCount = 0;
		int rCount = 0;
		char prevPiece='.';
		for(int i=0; i<N; ++i)
			pieceCounts(prevPiece, Board[N-i][y+i], bCount, rCount, blueWin, redWin);
	}

	if (redWin && blueWin)
		return "Both";
	if (!redWin && !blueWin)
		return "Neither";
	if (redWin)
		return "Red";
	return "Blue";
}

void ReadTestCase()
{
	static int testNo = 1;
	string inStr;

	getline(cin, inStr);
	istringstream parser(inStr);
	parser >> N >> K;

	// Read in Board
	for(int y=0; y<N; ++y)
	{
		// Add a sentinel so we always overwrite pieces falling away from the
		// edge with a space
		Board[0][y] = '.';
		getline(cin, inStr);
		for(int x=0; x<N; ++x)
			Board[x+1][y] = inStr[x];
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