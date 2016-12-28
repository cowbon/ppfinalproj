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
	clock_t t1, t2, t3, t4;
	int type = atoi(argv[1]), depth = atoi(argv[1]), b_factor = atoi(argv[2]), value;
	node *root = NULL;
	srand(time(NULL));
	cout << "Input search type, search tree depth, and branching factor respectively\n";
	//cin >> type >> depth >> b_factor;
	//gettimeofday(&tv1, NULL);
	t1 = clock();
	//param* var = new param(depth, b_factor, root, true);
	//paragenerateSearchTree(var);
	//root = var->root;
	root = generateSearchTree(depth, b_factor, root);
	//gettimeofday(&tv2, NULL);
	t1 = clock()-t1;
	cout << "Search value: " << value << "Generate search tree:" <<(double) t1<<endl;
	t1 = clock();
	value = MinMax(root, INT_MIN+1, INT_MAX);
	/*switch(type){
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
	}*/
	//gettimeofday(&tv3, NULL);
	t1 = clock()-t1;
	cout<<"Run time:"<<(double)t1<<' '<<value<<endl;
	t1 = clock();
	value = AlphaBeta(root, INT_MIN, INT_MAX, true);
	t1 = clock()-t1;
	cout<<"AlphaBeta"<<(double)t1<<' '<<value<<endl;
	t1 = clock();
	value = paraAlphaBeta(root, b_factor);
	t1 = clock()-t1;
	cout<<"paraAlphaBeta"<<(double)t1<<' '<<value<<endl;

	node* idx = root->children;
	if (root->children == 0)
		return root->min_or_max ? root->value : root->value * (-1);

	int num_of_thread = 2;//b_factor <= 5 ? b_factor-1 : 4;
	int* result = new int[num_of_thread];
	pthread_t thr[num_of_thread];
	alphabeta* t[num_of_thread];

	for (int i = 0; i < num_of_thread; i++)
		t[i] = new alphabeta(idx, result, b_factor, num_of_thread, i);

	t1 = clock();
	value = newparaAlphaBeta(root, b_factor, num_of_thread, result, thr, t);
	t1 = clock()-t1;
	cout<<"new"<<(double)t1<<' '<<value<<endl;
	return 0;
}
