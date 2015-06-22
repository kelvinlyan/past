#include <iostream>
#include "lMap.h"
using namespace std;


int main()
{
	int f;
	lMap lmap;
	lmap.Init(4, 4);
	lmap.PrintMap();
	lmap.Insert(2, 1, 1);
	lmap.Insert(2, 2, 4);
	lmap.Insert(2, 3, 3);
	lmap.Insert(2, 4, 1);
	lmap.Insert(3, 1, 2);
	lmap.Insert(3, 2, 3);
	lmap.Insert(3, 3, 4);
	lmap.Insert(3, 4, 2);
	lmap.Insert(4, 2, 5);
	lmap.Insert(4, 4, 5);
	//lmap.PrintLineMap();
	lmap.PrintMap();
	int a, b, c, d;
	while(cin >> a >> b >> c >> d){
		if(lmap.IsLinked(a, b, c, d)){
			cout << "Linked" << endl;
			lmap.PrintPath();
			lmap.Erase(a, b);
			lmap.Erase(c, d);
			cout << "Erase" << endl;
		}
		else
			cout << "not Linked" << endl;
		//lmap.PrintLineMap();
		lmap.PrintMap();
	}
}
