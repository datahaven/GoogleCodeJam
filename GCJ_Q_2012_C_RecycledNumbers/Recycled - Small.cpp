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

int solveTestCase(int A, int B)
{
	int pairCount = 0;
	for(int n=A; n<B; ++n)
	{
		for(int m=n+1; m<=B; ++m)
		{
			//cout << "pair:" << n << ", " << m << endl;
			if (isRecycledPair(n,m))
				++pairCount;
		}
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