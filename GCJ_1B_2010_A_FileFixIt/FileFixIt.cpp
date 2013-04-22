// Google Code Jam 1B 2010 (Practice)
// A - File Fix-it
// http://code.google.com/codejam/contest/635101/dashboard#s=p0
// Adrian Dale 22/04/2013
//
// Code simply inserts all paths into the file system, represented
// as an n-ary tree. It counts how many directories it needs to create
// for each insert, but only stores the count once the initial file system
// has been created.
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

int T, N, M;

class Node
{
public:
	string mName;
	vector<Node> mChildren;
};

Node gRoot;

// Insert path p into our file system.
// Returns the number of new directories we needed to
// create
int insertPath(string &p)
{
	// Split up our string by slashes and put results in dirs.
	vector<string> dirs;
	size_t start = 0, end = 0;
	while((end = p.find('/', start)) != string::npos)
	{
		dirs.push_back( p.substr(start, end - start) );
		start = end + 1;
	}
	dirs.push_back( p.substr(start) );

	int insertCount = 0;
	Node *curN = &gRoot;
	// NB skip first string as it is the blank before the starting slash
	for_each( dirs.begin() + 1, dirs.end(), [&](string &d) {
		auto dirPos = find_if(curN->mChildren.begin(), curN->mChildren.end(), [&](Node &c) {
								return c.mName == d;
		});

		if (dirPos == curN->mChildren.end())
		{
			// Directory not found at this level, so need to insert it
			Node newN;
			newN.mName = d;
			curN->mChildren.push_back(newN);
			++insertCount;
			// current node needs to be a pointer to the node we just added.
			// I think this would normally be dangerous as vectors can re-allocate their
			// entries, however we only need this pointer for a single loop iteration.
			curN = &curN->mChildren[curN->mChildren.size() - 1];
		}
		else
		{
			// Directory found, so traverse to next level by setting current node
			// to the dir we just found
			curN = &(*dirPos);
		}
	});

	return insertCount;
}

void ReadTestCase(int tc)
{
	string line;

	getline(cin, line);
	istringstream parser(line);
	parser >> N >> M;

	gRoot.mChildren.clear();
	int newInserts = 0;
	for(int i=0; i<N+M; ++i)
	{
		getline(cin, line);
		int inserts = insertPath(line);
		if (i >= N)
			newInserts += inserts;
	}

	cout << "Case #" << tc << ": " << newInserts << endl;
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

