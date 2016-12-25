#include "functions.h"
#include <iostream>
#include <cstdlib>
#include <climits>

using namespace std;

int main(int argc, char** argv){
	int type = atoi(argv[1]), depth = atoi(argv[2]), b_factor = atoi(argv[3]), value;
	node *root = NULL;
	srand(time(NULL));
	cout << "Input search type, search tree depth, and branching factor respectively\n";
	//cin >> type >> depth >> b_factor;
	root = generateSearchTree(depth, b_factor, root);
	if(type == 0) value = MinMax(root, INT_MIN+1, INT_MAX);//MinMax(root, true);
	else if(type == 1) value = AlphaBeta(root, INT_MIN, INT_MAX, true);
	cout << "Search value: " << value << endl;
	return 0;
}
