// Google Code Jam Practice Problems
// Always Turn Left
//
// Adrian Dale 21/09/2010
/*
http://code.google.com/codejam/contest/dashboard?c=32003#s=p1
*/

/*
Analysis - I got this to work but it took me WAY too long. Three and a half hours, including
some messing around doing other unrelated things to take a break.
Issues
- I got bitten by a bug with not realising how low the precedence of operator & is.
  I spent longer than I should before doing the basic tests that spotted it. Comes down
  to level of detailed knowledge of language, though.
- I got a bit too clever with my bit representation. A simpler scheme with a trivial
  conversion at the end would have done, rather than the scheme I chose which was
  flash but violated K.I.S.S
- A little time messing with Powershell to get redirects to work. Again, knowing the 
  environment.
- Tons of trouble getting things upside down / back to front.
- Rushing into coding without an exact idea of solution. (Although being fair, I 
  knew pretty much what I wanted.)
*/

#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

// Max length of path allowed, which implies max possible dimension
// of the maze in a worst case
const int nMax = 10000;

// NESW
const int dirX[] = {0,1,0,-1};
const int dirY[] = {-1,0,1,0};
const unsigned char dirMask[] = {1 << 0, 1 << 3, 1 << 1, 1 << 2};

// Maze is twice as wide as we don't know where the path starts in
// the x axis, so we'll start in the middle and assume a worst case
// of starting in a corner.
// Maze contains char
// - top four bits will say if there is a wall for UDLR
// - bottom four bits say if we know whether or not there is a wall
//   in each UDLR direction.
// An alternative would have been to have pairs of bits representing
// our three states unknown, wall, gap This doesn't give us our
// output bits in hex but would have simplified the representation
unsigned char gMaze[2*nMax][nMax];

// Macros to specify a rotation R or L when facing direction f
#define ROT_R(f) (((f)+1) % 4)
#define ROT_L(f) (((f)+3) % 4)
// Same for Behind
#define ROT_B(f) (((f)+2) % 4)

// Maze boundary
int minX;
int maxX;
int maxY;

// Set a single wall piece for given cell
void setCell(int x, int y, int f, bool wall)
{
	if ( (gMaze[x][y] & (dirMask[f] << 4)) == 0 )
	{
		// Mark piece as known
		gMaze[x][y] |= dirMask[f] << 4;
		// Set to wall/open
		if (!wall)
			gMaze[x][y] |= dirMask[f];
		else
			gMaze[x][y] &= ~dirMask[f];
	}
}

// Set maze wall facing f at (x,y) to wall
void setMaze(int x, int y, int f, bool wall)
{
	// Set wall for specified cell
	setCell(x,y,f, wall);
	// and also for adjoining cell in facing direction
	setCell(x+dirX[f], y+dirY[f], ROT_B(f), wall );
}

// Follow the given path through the maze,
// starting in the maze at x,y.
// When we return, x and y will contain the coords of the square we
// finished at
void followPath(string &p, int &x, int &y, int &facing)
{
	for( string::iterator it = p.begin(); it != p.end(); ++it )
	{
		int ox = x;
		int oy = y;

		switch(*it)
		{
		case 'W':
			// Move square in the direction we are facing
			x += dirX[facing];
			y += dirY[facing];

			// See where we've ended up and update maze boundaries
			// Note that if we're at the end we're outside the maze,
			// so don't count that square in boundary
			if ( it + 1 != p.end() )
			{
				minX = min(x, minX);
				maxX = max(x, maxX);
				maxY = max(y, maxY);
			}

			// Draw conclusions:
			//  - Can't be a wall in front of the old square
			setMaze(ox,oy,facing,false);
			
			// - There also must have been a wall to the left
			// of the old square, otherwise lh rule would have
			// taken us down that route instead
			setMaze(ox,oy,ROT_L(facing), true);
			break;
		case 'R':
			// Must be a wall ahead of us and to our left
			setMaze(ox,oy,facing,true);
			setMaze(ox,oy,ROT_L(facing),true);
			facing = ROT_R(facing);
			break;
		case 'L':
			facing = ROT_L(facing);
			break;
		}
	}
}

// Goes through maze in both directions, adding walls and gaps
// that can be inferred from the route taken
void solveTestCase(string &fwd_path, string &rev_path)
{
	// Clear out the maze
	memset(gMaze,0, sizeof(gMaze));

	// Start facing South at [nMax][1]
	// The 1 is so we can pretend the top 0 row of the maze is part of the maze.
	// We'll skip it when we output the results. Saves boundary checking when
	// filling in walls.
	int f = 2;
	int x = nMax;
	int y = 1;
	minX = 2*nMax + 10;
	int minY = 2;
	maxX = 0;
	maxY = 0;
	
	followPath(fwd_path, x, y, f);

	// Turn right around and head back into maze along reverse path
	f = ROT_B(f);

	followPath(rev_path, x, y, f);

	// Output the maze, skipping top row as we set that to be outside the maze
	for( int j=minY; j<=maxY; ++j )
	{
		for( int i=minX; i<=maxX; ++i )
		{
			char cell = gMaze[i][j] & 0xf;
			if (cell != 0)
				cout << hex << static_cast<int>(cell);
		}
		cout << endl;
	}
}

void ReadTestCase()
{
	static int testNo = 1;
	
	string path;
	string revpath;

	cin >> path;
	cin >> revpath;
	
	cout << "Case #" << dec << testNo++ << ":" << endl;
	solveTestCase(path, revpath);
}

void ReadInput()
{
	int T=0;
	cin >> T;
	while( T-- > 0 )
		ReadTestCase();
}

int main()
{
	ReadInput();

	return 0;
}