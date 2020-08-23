#pragma once


typedef struct AVLNode
{
	int value;
	struct AVLNode* left;
	struct AVLNode* right;
	int height;
} AVLNode;

AVLNode* searchAVL(AVLNode* avlTree, int num);
void initAVLTree(AVLNode** avlTree, int nums[], int count);
int insertAVLNode(AVLNode** avlTree, int num);
int deleteAVLNode(AVLNode** avlTree, int num);