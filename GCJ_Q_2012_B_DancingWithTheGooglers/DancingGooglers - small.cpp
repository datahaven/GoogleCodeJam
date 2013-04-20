// Google Code 2012 Qualification Problems
// B - Dancing Googlers
//
// Adrian Dale 14/04/2012
/*
http://code.google.com/codejam/contest/1460488/dashboard#s=p1
*/

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

// 0 is normal
// 1 is surprise
// Stores best score for normal/surprise
int TripletsBest[31][2];

int solveTestCase(int N, int S, int P, int *Total)
{
	int result = 0;
	
	// Go through input and see if any of our scores could
	// only have one possible result - ie 0, 1, 29, 30
	// Surprises are shared out among the remaining results
	for(int i=0; i<N; ++i)
	{
		if (Total[i] == 0 && P==0)
			++result;
		else if (Total[i] == 1 && P<=1)
			++result;
		else if (Total[i] >= 29) // always includes a 10
			++result;
	}

	// Store the remaining scores that may be surprises
	int Scores[100];
	int SCCount = 0;
	for(int i=0; i<N; ++i)
	{
		if (Total[i] > 1 && Total[i] < 29)
			Scores[SCCount++] = Total[i];
	}

	// Set out our bit array
	// 0000011 where 1 is surprise, 0 isn't, for no. of surprises
	int PermArray[100];
	for(int i=0; i<SCCount; ++i)
	{
		if (i<SCCount-S)
			PermArray[i] = 0;
		else
			PermArray[i] = 1;
	}

	//for(int i=0; i<SCCount; ++i)
	//	cout << Scores[i] << " ";
	//cout << endl;

	// Go through each permutation of the bit array
	// and see how many bests that set of surprise/no-surprise gives us
	int bestCountSoFar = 0;
	do
	{
		int bestCount = 0;
		//for(int i=0; i<SCCount; ++i)
		//	cout << PermArray[i];
		//cout << endl;

		for(int i=0; i<SCCount; ++i)
		{
			// If score was surprise then best is total/3 rounded up
			// else best is total/3 rounded up + 1
			int best = TripletsBest[ Scores[i] ][ PermArray[i] ];
			if (best >= P)
				++bestCount;
		}
		if (bestCount > bestCountSoFar)
			bestCountSoFar = bestCount;

	} while( next_permutation(PermArray, PermArray+SCCount) == true );

	return bestCountSoFar + result;
}

void ReadTestCase()
{
	static int testNo = 1;
	
	string inStr;
	getline(cin, inStr);
	istringstream parser(inStr);
	int N, S, P;
	static int Total[100];
	parser >> N;
	parser >> S;
	parser >> P;
	for(int i=0; i<N; ++i)
		parser >> Total[i];
	
	int newtc = solveTestCase(N, S, P, Total);
	
	cout << "Case #" << testNo++ << ": " << newtc << endl;
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

void FillTripletsBest()
{
	int tcount = 0;

	for(int total=0; total<=30; ++total)
	{
		for(int a=0;a<=10;++a)
		{
			for(int b=a;b<=10;++b)
			{
				int c = total - a - b;
				if (c<0 || c>10 || c<a || c<b)
					continue;
				// Is abc a valid triple?
				if (abs(a-b) > 2 || abs(a-c) > 2 || abs(b-c) > 2)
					continue;

				bool surprising = abs(a-b) == 2 || abs(a-c) == 2 || abs(b-c) == 2;

				if (surprising)
				{
					TripletsBest[total][1] = c;
				}
				else
				{
					TripletsBest[total][0] = c;
				}

				++tcount;
				//cout << tcount << " Total=" << total << " (" << a << "," << b << "," << c << ") ";
		 
				//if (surprising)
				//	cout << "S " << TripletsBest[total][1];
				//else
				//	cout << TripletsBest[total][0];
				//cout << endl;
			}
		}
	}
}

int main()
{
	
	FillTripletsBest();
	ReadInput();
	return 0;
}