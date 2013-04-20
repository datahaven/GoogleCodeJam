// Google Code 2012 Qualification Problems
// A - Speaking in Tongues
//
// Adrian Dale 14/04/2012
/*
http://code.google.com/codejam/contest/1460488/dashboard#s=p0
*/

#include <iostream>
#include <string>
#include <sstream>
using namespace std;

// English         abcdefghijklmnopqrstuvwxyz
char MapTable[] = "yhesocvxduiglbkrztnwjpfmaq";

string solveTestCase(string &inStr)
{
	string outStr = "";
	for( string::iterator it = inStr.begin(); it != inStr.end(); ++it)
	{
		char c = *it;
		char outChar = ' ';
		if (c!=' ')
		{
			int charpos = c - 'a';
			outChar = MapTable[charpos];
		}
		outStr = outStr + outChar;
	}
	
	return outStr;
}

void ReadTestCase()
{
	static int testNo = 1;
	
	string inStr;
	getline(cin, inStr);
	
	cout << "Case #" << testNo++ << ": " << solveTestCase(inStr) << endl;
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