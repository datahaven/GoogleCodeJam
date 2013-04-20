// Google Code 2013 Qualification Problems
// C - Fair And Square
//
// Adrian Dale 13/04/2013
/*
https://code.google.com/codejam/contest/2270488/dashboard#s=p2
*/

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <vector>
using namespace std;

typedef long long ull;
vector<ull> AllFAS;

// There's probably a much faster way to do this!
bool isPalindrome(ull N)
{
	ostringstream toStr;
	toStr << N;
	string rev = toStr.str();
	reverse(rev.begin(), rev.end());
	return rev == toStr.str();
}

// Again, there's probably a nicer way to do this.
bool isSquare(ull N, ull &Root)
{
	Root = static_cast<ull>( floor(sqrt(static_cast<double>(N))) );
	return Root * Root == N;
}

bool isFAS(ull N)
{
	ull root;

	if (isSquare(N, root)==false)
		return false;

	if (isPalindrome(root)==false)
		return false;
	return isPalindrome(N);
}

ull solveTestCase(ull A, ull B)
{
	ull fasCount = 0;
	for(ull n=A; n<=B; ++n)
		if (isFAS(n))
		{
			cout << n << endl;
			++fasCount;
		}
		
	return fasCount;
}

ull solveTestCaseMedium(ull A, ull B)
{
	// Find first no in AllFAS >= A
	vector<ull>::iterator lb = lower_bound(AllFAS.begin(), AllFAS.end(), A);
	
	// and find last no in AllFAS <=B
	vector<ull>::iterator ub = upper_bound(lb, AllFAS.end(), B);

	return ub - lb;
}

void ReadTestCase()
{
	static ull testNo = 1;
	string inStr;
	getline(cin, inStr);
	
	istringstream parser(inStr);
	ull A, B;
	parser >> A >> B;
	
	cout << "Case #" << testNo++ << ": " << solveTestCaseMedium(A, B) << endl;
	
}

void ReadInput()
{
	ull T=0;
	string line;
	getline(cin, line);
	istringstream parser(line);
	parser >> T;
	while( T-- > 0 )
		ReadTestCase();
}

void test01()
{
	for(ull i=0; i<1000; ++i)
	{
		ull root;
		if (isSquare(i, root))
			cout << i << " is square " << root << endl;
	}
	exit(0);
}

// Find all square palindromes less than 10^7
void allSqPalindromes()
{
	AllFAS.clear();
	for(ull n=1; n<=10000000; ++n)
	{
		if (isPalindrome(n) && isPalindrome(n*n))
		{
			AllFAS.push_back(n*n);
			//cout << n*n << endl;
		}
	}

	//for_each(AllFAS.begin(), AllFAS.end(), [&](ull n) {
	//	cout << n << endl;
	//});
}

int main()
{
	//test01();
	allSqPalindromes();
	//return 0;
	ReadInput();
	return 0;
}