// Google Code 2013 Qualification Problems
// D - Treasure
//
// Adrian Dale 13/04/2013
//
// Note that this solution only works for the small input
/*
https://code.google.com/codejam/contest/2270488/dashboard#s=p3
*/

#include <iostream>
#include <iterator>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <map>
using namespace std;

class ChestType
{
public:
	vector<int> mKeys;
	int mUnlockKey;
};

vector<ChestType> Chests;
vector<int> StartKeysHeld;

// Stored solutions for Dynamic Programming solution
// Max of 20 chests
int Solutions[1048576]; 

// chests is a bitfield where bit n is set if chest n is open
// Solutions starts out at -1 for every possible combination of chests open/closed
// Solutions is 0 for not possible, 1 for possible
bool isSolutionPossible(int chests)
{
	if (Solutions[chests] != -1)
		return Solutions[chests]==1;

	// See if puzzle is already solved
	size_t openCount = 0;
	for(size_t i=0; i<Chests.size(); ++i)
		if (chests & (1<<i))
			++openCount;
	if (openCount == Chests.size())
	{
		Solutions[chests] = 1;
		return true;
	}

	// Need to calculate the solution
	// - work out the keys we must have
	// Start keys + keys in open chests - keys needed to open opened chests
	vector<int> keysHeld = StartKeysHeld;
	int maskPos = 1;
	for(size_t i=0; i<Chests.size(); ++i)
	{
		if ( (chests & maskPos) != 0 )
		{
			keysHeld.insert(keysHeld.end(), Chests[i].mKeys.begin(), Chests[i].mKeys.end());
		}
		maskPos <<= 1;
	}
	// Note that we get all keys before removing the ones we used up, since we don't
	// know what order we opened the chests in
	maskPos = 1;
	for(size_t i=0; i<Chests.size(); ++i)
	{
		if ( (chests & maskPos) != 0 )
		{
			// look for the key that would open this chest in our list of held keys
			auto keyPos = find( keysHeld.begin(), keysHeld.end(), Chests[i].mUnlockKey );
			// if it isn't there it means this isn't a valid position to be in
			if (keyPos == keysHeld.end())
			{
				Solutions[chests] = 0;
				return false;
			}
			keysHeld.erase( keyPos );
		}
		maskPos <<= 1;
	}

	// See if there are any chests we can no longer open
	
	// - Get a list of all keys in unopened chests and keys currently held
	vector<int> myKeys = keysHeld;
	for(size_t i=0; i<Chests.size(); ++i)
		if ( (chests & (1<<i)) == 0 )
			myKeys.insert(myKeys.end(), Chests[i].mKeys.begin(), Chests[i].mKeys.end());

	// - Count how many of each key we've got.
	map<int, int> keyCounts;
	for_each(myKeys.begin(), myKeys.end(), [&](int k) {
		// This syntax works because [] will insert a default-constructed
		// element (which is an int set to 0) into the map if it doesn't already exist.
		++keyCounts[k];
	});
	// - Count how many of each type of unopened chest we've got
	map<int, int> unopenedChestCounts;
	for(size_t i=0; i<Chests.size(); ++i)
	{
		if ( (chests & (1<<i)) == 0 )
			++unopenedChestCounts[Chests[i].mUnlockKey];
	}

	// - Now see if we've got enough keys for each chest
	bool enoughKeys = true;
	for_each(unopenedChestCounts.begin(), unopenedChestCounts.end(), [&](pair<const int, int> &uc) {
		if (keyCounts[uc.first] < uc.second)
			enoughKeys = false;
	});

	if (enoughKeys == false)
	{
		Solutions[chests] = 0;
		return false;
	}

	// Now start a recursive search.
	// I've skipped the bulk-unlock thing for now, since using Dynamic Programming sped
	// things up enough anyway
	for( size_t i=0; i<Chests.size(); ++i)
	{
		if ( (chests & (1<<i)) == 0 )
		{
			// Do we have the key for this chest?
			vector<int>::iterator keyPos = find(keysHeld.begin(), keysHeld.end(), Chests[i].mUnlockKey);
			if (keyPos != keysHeld.end())
			{
				int nextChests = chests;
				
				// Yes, we do, so open this chest, then continue search
				nextChests |= (1<<i);

				if ( isSolutionPossible(nextChests) )
				{
					Solutions[chests] = 1;
					return true;
				}
			}
		}
	}

	Solutions[chests] = 0;
	return false;
}

void solveTestCase()
{
	// I could make this faster by only initialising Solutions
	// as much as needed, rather than the full amount every time.
	// But it's only a million ints, so shouldn't take long anyway,
	// and only happens once per test case.
	memset(Solutions, 0xff, sizeof(Solutions));
	vector<int> keysHeld = StartKeysHeld;

	size_t chestsOpen = 0;
	bool anySolPossible = false;
	// Plan is to keep looking for the next chest to open
	// until we've either opened all chests,
	// or tried each chest as the first chest and discovered that
	// it can't be opened.
	vector<int> solution;
	int chests = 0;
	size_t i=0;
	while( i < Chests.size() && chestsOpen < Chests.size())
	{
		if ( (chests & (1<<i))==0)
		{
			// Do we have the key for this chest?
			vector<int>::iterator keyPos = find(keysHeld.begin(), keysHeld.end(), Chests[i].mUnlockKey);
			if (keyPos != keysHeld.end())
			{
				// Yes - Let's see if we can still solve the puzzle
				// if we open this chest.
				chests |= (1<<i);
					
				if (isSolutionPossible(chests))
				{
					anySolPossible = true;
					++chestsOpen;
					// open the chest, take keys (and loot) from inside, remove the key we used up
					keysHeld.insert(keysHeld.end(), Chests[i].mKeys.begin(), Chests[i].mKeys.end());
					keysHeld.erase( find(keysHeld.begin(), keysHeld.end(), Chests[i].mUnlockKey) );
					solution.push_back(i);
					// Start search through chests back at the start so we keep the solution in
					// lexicographical order
					i = -1;
				}
				else
				{
					// Can't open all the chests if we open chest i next,
					// so undo move ready for next try
					chests &= ~(1<<i);	
				}
			}
		}
		++i;
	}
		
	if (anySolPossible == false)
	{
		cout << " IMPOSSIBLE";
		return;
	}
	
	// Display solution, taking care to not put spaces after last item
	// NB I could have just printed each digit as I found it - this is just
	// slightly tidier
	for_each(solution.begin(), solution.end(), [&](int cn) {
		cout << " " << cn+1;
	});
}

void ReadTestCase()
{
	static int testNo = 1;
	string inStr;
	getline(cin, inStr);
	
	istringstream parser(inStr);
	int K, N;
	parser >> K >> N;
	
	StartKeysHeld.clear();
	Chests.clear();

	parser.clear();
	getline(cin, inStr);
	parser.str(inStr);
	while(K-- > 0)
	{	
		int Kn;
		parser >> Kn;
		StartKeysHeld.push_back(Kn);
	}

	while(N-- > 0)
	{
		parser.clear();
		getline(cin, inStr);
		parser.str(inStr);

		int Ti, Ki;
		parser >> Ti >> Ki;

		ChestType chest;
		chest.mUnlockKey = Ti;
		chest.mKeys.clear();
		while(Ki-- > 0)
		{
			int Kn;
			parser >> Kn;
			chest.mKeys.push_back(Kn);
		}
		Chests.push_back(chest);
	}

	cout << "Case #" << testNo++ << ":";
	solveTestCase();
	cout << endl;
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