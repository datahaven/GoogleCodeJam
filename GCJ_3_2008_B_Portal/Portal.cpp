/*
 * Portal.cpp
 *
 *  Created on: 29 Nov 2011
 *      Author: Adrian Dale
 *
 *  Updated on: 27/04/2013
 *
 * Google Code Jam 2008 Round 3 - Portal
 *
 * Solves for both Small and Large test cases.
 */
#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <limits>

using namespace std;

int N; // No of test cases
int R; // Rows
int C; // Columns

int CakeR;
int CakeC;
int StartR;
int StartC;

const int MAX_R = 15;
const int MAX_C = 15;

// Room, plus border
char Grid[MAX_R+2][MAX_C+2];

struct PuzzleState
{
	int PosR;
	int PosC;
	int PortalR;
	int PortalC;
	int Cost;
	bool isWin() { return PosR==CakeR && PosC==CakeC; }
	bool operator<(const PuzzleState &rhs) const;
};

int Visited[MAX_R+2][MAX_C+2][MAX_R+2][MAX_C+2];

// Define an ordering for our puzzle states so that search
// prioritises ones with the cheapest costs.
bool PuzzleState::operator<(const PuzzleState &rhs) const
{
	if (this->Cost==rhs.Cost)
	{
		if (this->PosR==rhs.PosR)
		{
			if (this->PosC == rhs.PosC)
			{
				if (this->PortalR == rhs.PortalR)
				{
					return this->PortalC < rhs.PortalC;
				}
				return this->PortalR < rhs.PortalR;
			}
			return this->PosC < rhs.PosC;
		}
		return this->PosR < rhs.PosR;
	}
	return this->Cost < rhs.Cost;
}

const int DR[] = {0,1,0,-1}; // r, d, l, u
const int DC[] = {1,0,-1,0};
const int DF[] = {2,3,0,1}; // Direction flip

// Simple version that should solve a normal maze without portals
void PossibleMoves(const PuzzleState &ps, vector<PuzzleState> &out)
{
	for(unsigned int d=0; d<sizeof(DR)/sizeof(DR[0]); ++d)
	{
		char adjacentSq = Grid[ps.PosR+DR[d]][ps.PosC+DC[d]];
		if (adjacentSq != '#')
		{
			// Not next to a wall, so we can walk that way
			PuzzleState ns = ps;
			ns.PosR += DR[d];
			ns.PosC += DC[d];
			ns.Cost++;
			out.push_back(ns);
		}
	}
}

// Given a current PuzzleState, what other puzzle
// states can we reach in one move.
// We can move in any of the four directions
// (including through existing portal, or through a new portal)
// and can either leave the current portal where it is,
// or move it to one of the four directions we can see in,
// if it isn't there already.
// NB This routine does not clear the output vector.
//    It can also leave duplicate moves in the output.
void PossibleMovesII(const PuzzleState &ps, vector<PuzzleState> &out)
{
	// See if we're stood by a wall, and hence have the option to
	// walk through it if we've got a portal placed.
	bool nextToWall = false;
	for(unsigned int d=0; d<sizeof(DR)/sizeof(DR[0]); ++d)
	{
		char adjacentSq = Grid[ps.PosR+DR[d]][ps.PosC+DC[d]];
		if (adjacentSq == '#')
		{
			nextToWall = true;
			break;
		}
	}

	// Try walking through the portal
	if (nextToWall && ps.PortalR != 0 && ps.PortalC != 0)
	{
		// Portal is position of square the portal exits onto
		// Don't move if it leads us back to our current square
		if ( (ps.PortalC == ps.PosC && ps.PortalR == ps.PosR) == false)
		{
			PuzzleState newN = ps;
			newN.Cost = ps.Cost+1;
			newN.PosC = ps.PortalC;
			newN.PosR = ps.PortalR;
			// Portal stays where it is.
			out.push_back(newN);
		}
	}

	// Try all possible portal placements (we stand still)
	for(unsigned int pd=0; pd<sizeof(DR)/sizeof(DR[0]); ++pd)
	{
		// Scan in pd direction until we find first wall.
		// NB Works because we guarantee grid is enclosed by a wall
		int pwR = ps.PosR;
		int pwC = ps.PosC;
		int pwO = DF[pd];
		while(Grid[pwR][pwC] != '#')
		{
			pwR += DR[pd];
			pwC += DC[pd];
		}
		// Step back one space to find the portal exit square
		pwR -= DR[pd];
		pwC -= DC[pd];
		// No point placing portal to exit onto the square
		// we are already standing on.
		if ( (pwR == ps.PosR && pwC == ps.PosC) == false)
		{
			PuzzleState newN = ps;
			// 0 cost for portal placement, and pos stays same
			newN.PortalC = pwC;
			newN.PortalR = pwR;
			out.push_back(newN);
		}
	}

	// Find all the squares we can walk to
	for(unsigned int d=0; d<sizeof(DR)/sizeof(DR[0]); ++d)
	{
		char adjacentSq = Grid[ps.PosR+DR[d]][ps.PosC+DC[d]];
		if ( adjacentSq == '.')
		{
			PuzzleState newN = ps;
			newN.Cost = ps.Cost+1;
			newN.PosC = ps.PosC+DC[d];
			newN.PosR = ps.PosR+DR[d];
			// Portal stays where it is.
			out.push_back(newN);
		}
	}
}

void DumpGrid()
{
	for(int i=0; i<=R+1; ++i)
	{
		for(int j=0; j<=C+1; ++j)
		{
			if (i==CakeR && j==CakeC)
				cout << "X";
			else if (i==StartR && j==StartC)
				cout << "O";
			else
				cout << Grid[i][j];
		}
		cout << endl;
	}
}

void SolveTestCase()
{
	PuzzleState start;
	start.PosR = StartR;
	start.PosC = StartC;
	start.PortalC = 0;
	start.PortalR = 0;
	start.Cost = 1;

	memset(Visited, 0, sizeof(Visited));

	int bestCost = numeric_limits<int>::max();

	// Originally I implemented this as BFS, expecting it to be 
	// enough to solve the small input but too slow for large.
	// This is what happened.
	// It was then quick to change the queue to be a priority_queue,
	// which I thought effectively turned the problem into Dijkstra's
	// Shortest Paths algorithm.
	// Idea is here:
	// http://community.topcoder.com/tc?module=Static&d1=tutorials&d2=graphsDataStrucs3
	// Weirdly, though, C++'s pq returns the highest
	// cost state, whereas algorithm needs lowest. If I switch the compare
	// around then it takes longer to run and/or crashes, leaving me in the rather unsatisfying
	// state of having code that works, but without me knowing why!
	//
	// I suspect there is some bogus info in that article somewhere, or possibly I
	// have misunderstood something. It doesn't seem to implement Dijkstra the way
	// I've seen it described elsewhere.
	// It may just be luck that this code works!
	priority_queue<PuzzleState> Nodes;
	Nodes.push( start );
	while( Nodes.empty() == false )
	{
		PuzzleState N = Nodes.top();
		Nodes.pop();

		if (N.isWin())
		{
			// Store this solution
			bestCost = min(bestCost, N.Cost);
			continue;
		}

		// Prune
		if (N.Cost >= bestCost)
			continue;

		// Get next possible moves
		vector<PuzzleState> nextNodes;
		PossibleMovesII(N, nextNodes);
		for_each(nextNodes.begin(), nextNodes.end(), [&](PuzzleState &ps) {
			if (Visited[ps.PosR][ps.PosC][ps.PortalR][ps.PortalC] == 0 ||
				Visited[ps.PosR][ps.PosC][ps.PortalR][ps.PortalC] > ps.Cost)
				Nodes.push(ps);
		});

		Visited[N.PosR][N.PosC][N.PortalR][N.PortalC] = N.Cost;
	}

	if (bestCost == numeric_limits<int>::max() )
		cout << "THE CAKE IS A LIE";
	else
		cout << bestCost-1;
}

void ReadTestCases()
{
	string line;

	getline(cin, line);
	istringstream parser(line);
	parser >> N;

	int TestNo = 1;
	while(TestNo <= N)
	{
		getline(cin, line);
		parser.str(line);
		parser.clear();
		parser >> R >> C;

		// Draw grid border
		for(int i=0; i<=R+1; ++i)
		{
			Grid[i][0] = '#';
			Grid[i][C+1] = '#';
		}
		for(int j=0; j<=C+1; ++j)
		{
			Grid[0][j] = '#';
			Grid[R+1][j] = '#';
		}
		// Get grid data from cin
		for(int rc=1; rc <= R; ++rc)
		{
			getline(cin, line);
			for(int cc=1; cc<=C; ++cc)
			{
				char cell = line[cc-1];
				if (cell == 'O')
				{
					StartR = rc;
					StartC = cc;
					cell = '.';
				}
				else if (cell == 'X')
				{
					CakeR = rc;
					CakeC = cc;
					cell = '.';
				}
				Grid[rc][cc] = cell;
			}
		}

		cout << "Case #" << TestNo << ": ";
		SolveTestCase();
		cout << endl;

		++TestNo;
	}
}

int main()
{
	ReadTestCases();
	return 0;
}
