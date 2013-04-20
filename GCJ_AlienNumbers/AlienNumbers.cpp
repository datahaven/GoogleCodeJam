// Google Code Jam Practice Problems
// Alien Numbers
//
// Adrian Dale 20/09/2010
/*
http://code.google.com/codejam/contest/dashboard?c=32003#
*/

// Both small and large responses judged correct.
// Took about 50 minutes to program at a leisurely pace, whilst listening to
// the radio.
#include <iostream>
#include <string>
using namespace std;

string solveTestCase(string &no, string &srcl, string &tgtl)
{
	int srcBase = srcl.length();
	int tgtBase = tgtl.length();
	
	// Convert alien number in src language to decimal
	int decimal = 0;
	int digit_pos = 1;
	for( string::reverse_iterator rit = no.rbegin(); rit != no.rend(); ++rit)
	{
		size_t digpos = srcl.find(*rit);
		decimal += digpos * digit_pos;
		digit_pos *= srcBase;
	}
	
	// Convert decimal number to target language
	string tgtNo;
	do
	{
		size_t lsb = decimal % tgtBase;
		char lsbc = tgtl[lsb];
		tgtNo = lsbc + tgtNo;
		decimal /= tgtBase;
	} while(decimal > 0);

	return tgtNo;
}

void ReadTestCase()
{
	static int testNo = 1;
	
	string no;
	string srcl;
	string tgtl;

	cin >> no;
	cin >> srcl;
	cin >> tgtl;
	
	cout << "Case #" << testNo++ << ": " << solveTestCase(no, srcl, tgtl) << endl;
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