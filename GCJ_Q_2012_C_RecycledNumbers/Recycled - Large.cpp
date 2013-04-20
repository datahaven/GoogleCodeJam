// Google Code 2012 Qualification Problems
// C - Recycled Numbers
//
// Adrian Dale 14/04/2012
/*
http://code.google.com/codejam/contest/1460488/dashboard#s=p2
*/

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <set>
using namespace std;

bool isRecycledPair(int n, int m)
{
	ostringstream nss;
	nss << n;
	ostringstream mss;
	mss << m;

	// TODO - Worth checking that both numbers contain the same digits?

	// Check each possible way to pivot n
	for(unsigned int pivot=0; pivot < nss.str().length(); ++pivot)
	{
		string revstr = nss.str();
		reverse(revstr.begin(), revstr.end());
		reverse(revstr.begin(), revstr.begin()+pivot);
		reverse(revstr.begin()+pivot, revstr.end());
		

		// Remember to discount leading 0's
		if (revstr == mss.str() && *revstr.begin() != '0')
		{
			//cout << n << ", " << m << ": " << pivot << " " << revstr << endl;
			return true;
		}
	}
	return false;
}

// This goes through every possible pair and checks to see if it is valid.
// A smarter solution would go through each number between A and B and
// find all the pivots and see if they are in range.
// Has to handle uniqueness?
int solveTestCaseOLD(int A, int B)
{
	int pairCount = 0;
	for(int n=A; n<B; ++n)
	{
		for(int m=n+1; m<=B; ++m)
		{
			if (isRecycledPair(n,m))
			{
				cout << "pair:" << n << ", " << m << endl;
				++pairCount;
			}
		}
	}
	
	return pairCount;
}

// This is (hopefully) that smart function referred to above.
// Much faster but still a bit too slow on the big cases
// (approx 5 per minute - close but not good enough) - now fixed!
int solveTestCase(int A, int B)
{
	ostringstream Bss;
	Bss << B;
	string BStr = Bss.str();

	int pairCount=0;
	for(int n=A; n<B; ++n)
	{
		set<int> pairsFound;

		ostringstream nss;
		nss << n;
		
		// Check each possible way to pivot n (not including n=0)
		for(unsigned int pivot=1; pivot < nss.str().length(); ++pivot)
		{
			string revstr = nss.str();
			reverse(revstr.begin(), revstr.end());
			reverse(revstr.begin(), revstr.begin()+pivot);
			reverse(revstr.begin()+pivot, revstr.end());
		
			if (*revstr.begin() == '0')
				continue;
#if 0
			// Convert to a digit and see if it is less than B
			istringstream parser(revstr);
			int m;
			parser >> m;
			
			if (m <= B && m > n)
			{
				//cout << "pair:" << n << "," << m << endl;
				
				//break; // Can't exit here as there can be more than one
				// pair creatable from n that is still within range.
				// However, we need to check we don't have duplicates
				// eg 1212 can make 2121 in several ways, so we keep the m's in a set
				pairsFound.insert(m);
			}
#endif
			// Alternative version to see if lexicographical compare
			// is faster than converting to a digit.
			// Turns out that yes, it is by quite a long way.
			// Can now solve 19 a minute, which is fast enough
			if (revstr <= BStr && revstr > nss.str() )
			{
				istringstream parser(revstr);
				int m;
				parser >> m;
				pairsFound.insert(m);
			}
		}

		pairCount += pairsFound.size();
	}
	
	return pairCount;
}

void ReadTestCase()
{
	static int testNo = 1;
	
	string inStr;
	getline(cin, inStr);
	istringstream parser(inStr);
	int A, B;
	parser >> A;
	parser >> B;
	
	cout << "Case #" << testNo++ << ": " << solveTestCase(A, B) << endl;
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