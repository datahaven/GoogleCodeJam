// Google Code Jam 1A 2010 (Practice)
// Make It Smooth
// http://code.google.com/codejam/contest/544101/dashboard#s=p1
// Adrian Dale 21/04/2013

// Notes
// This took me about an hour and three quarters to write the code and get it
// working for both large and small solutions. I had to spend a bit of time
// thinking about the problem beforehand, too.
//
// The hardest part was to get the insight
// of how to split it into non-overlapping sub-problems so that I could get a DP
// solution. I had to convince myself that it was possible to solve the problem
// from left to right along the row of pixels and only worry about the value of the
// left hand pixel.
//
// Also had some difficulty with recursively inserting pixels, having to take
// care not to get stuck in an infinite recursion of inserting the same pixel(s)
// over and over. Either that, or not testing all possible insertions.
// Answer was to only insert next pixel in order between the left value and the
// current first pixel.
//
// Solution is a memoized recursive function that tries all possible moves
// for a given set of pixels (specified as an index into our row of given pixels),
// given a value for the pixel to the left of that set of pixels.
// It uses the index so that it doesn't need to actually insert or delete pixels.
// Runs in a couple of seconds on the large input.
// I wrote this code without reading the Contest Analysis :-)
//
// Could it be optimised?
// I actually reckon the code is pretty much optimal as-is. There's no copying
// of data; vectors are nearly as fast as arrays for the initial data reads;
// algorithm is as good as it gets; memo is pretty small so initialisation is fast.
// It might be possible to change the for loop of "change pixel" so that it only
// loops round cases it actually needs to try.
// The only real optimisation I can think of would be to run test cases in
// parallel.
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

int T;
int D;
int I;
int M;
int N;
vector<int> Pixels;

const int NO_LEFT = 256;
int memo[257][101];

// Return the cheapest solution to solve the puzzle, starting
// at solPos in the list of Pixels, assuming a left neighbour
// of value leftValue.
// This is a Dynamic Programming solution that stores results of
// previous calculations in a memo
int cheapestSolution(int leftValue, int solPos)
{
	// If we've seen solution before then return stored answer
	if (memo[leftValue][solPos] != -1)
		return memo[leftValue][solPos];

	//cout << "cheapestSolution(" << leftValue << ", " << solPos << ")" << endl;

	int bestCostSoFar = 2147483647;
	
	// See if we've solved problem
	// - cost of further solving is zero
	if (solPos >= static_cast<int>(Pixels.size()))
	{
		memo[leftValue][solPos] = 0;
		return 0;
	}

	// Try each possible move:

	// - delete current pixel - This is always allowed
	bestCostSoFar = min( bestCostSoFar, D + cheapestSolution(leftValue, solPos+1) );

	// - change value of current pixel (including no change)
	for(int i=0; i<256; ++i)
	{
		if (abs(leftValue - i) <= M || leftValue == NO_LEFT )
			bestCostSoFar = min( bestCostSoFar, abs(Pixels[solPos]-i) + cheapestSolution(i, solPos+1) );
	}

	// - insert a new pixel but only if necessary.
	// It'll never be cheaper to insert unnecessary pixels
	if (leftValue != NO_LEFT && abs(Pixels[solPos]-leftValue) > M )
	{
		// Try all possible inserted pixels between leftValue and current pixel
		if (leftValue < Pixels[solPos])
		{
			for(int i=leftValue+1; i<Pixels[solPos]; ++i)
			{
				if (abs(leftValue - i) <= M )
					bestCostSoFar = min( bestCostSoFar, I + cheapestSolution(i, solPos) );
			}
		}
		else
		{
			for(int i=leftValue-1; i>Pixels[solPos]; --i)
			{
				if (abs(leftValue - i) <= M )
					bestCostSoFar = min( bestCostSoFar, I + cheapestSolution(i, solPos) );
			}
		}
	}

	memo[leftValue][solPos] = bestCostSoFar;
	return bestCostSoFar;
}

int solveTestCase()
{
	// I'm going to solve the line of pixels from
	// left to right, each time trying all possible
	// actions, assuming a given left neighbour.
	// First pos is a special case
	memset(memo, -1, sizeof(memo));
	return cheapestSolution(NO_LEFT, 0);
}

void ReadTestCase(int tc)
{
	string line;

	getline(cin, line);
	istringstream parser(line);
	parser >> D >> I >> M >> N;

	parser.clear();
	getline(cin, line);
	parser.str(line);
	Pixels.clear();
	for(int i=0; i<N; ++i)
	{
		int pixel;
		parser >> pixel;
		Pixels.push_back(pixel);
	}

	cout << "Case #" << tc << ": " << solveTestCase() << endl;
}

void ReadTestCases()
{
	string line;
	getline(cin, line);
	istringstream parser(line);
	parser >> T;
	for(int tc=1; tc<=T; ++tc)
		ReadTestCase(tc);
}

int main()
{
	ReadTestCases();
	return 0;
}

