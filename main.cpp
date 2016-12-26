#include "functions.h"
#include "parallel_alpha_beta.h"
#include <iostream>
#include <cstdlib>
#include <climits>
#include <sys/time.h>
#include <ctime>
using namespace std;

int main(int argc, char** argv){
	//struct timeval tv1, tv2, tv3;
	clock_t t1, t2;
	int type = atoi(argv[1]), depth = atoi(argv[2]), b_factor = atoi(argv[3]), value;
	node *root = NULL;
	srand(time(NULL));
	cout << "Input search type, search tree depth, and branching factor respectively\n";
	//cin >> type >> depth >> b_factor;
	//gettimeofday(&tv1, NULL);
	t1 = clock();
	root = generateSearchTree(depth, b_factor, root);
	//gettimeofday(&tv2, NULL);
	t1 = clock()-t1;

	switch(type){
		case 0:
			value = MinMax(root, INT_MIN+1, INT_MAX);
			break;
		case 1:
			value = AlphaBeta(root, INT_MIN, INT_MAX, true);
			break;
		case 2:
			value = paraAlphaBeta(root, b_factor);
			break;
		default:
			break;
	}
	//gettimeofday(&tv3, NULL);
	t2 = clock()-t1;
	cout << "Search value: " << value << "Generate search tree:" <<(double) t1<<"Run time:"<<(double)t2<<endl;
	return 0;
}
