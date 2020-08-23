#include <stdio.h>
#include "AVL.h"

int main(int argc, const char *argv[])
{
	AVLNode* avlTree = NULL;
	int avlNums[] = { 5,3,6,2,4,7,1 };
	initAVLTree(&avlTree, avlNums, 7);
	deleteAVLNode(&avlTree, 5);

	return 0;
}