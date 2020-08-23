#include <stdlib.h>
#include <string.h>
#include "AVL.h"

// 一个二叉排序树 任意节点不管是左旋还是右旋 它的中序遍历的结果不变
/*
	平衡因子(bf) = 左子树的高度 - 右子树的高度
	bf的绝对值大于等于2需要做旋转操作
	if(bf>=2){
		if(node.left.bf<0){
			LR
		}else{
			R
		}
	}

	if(bf<=-2){
		if(node.right.bf>0){
			RL
		}else{
			L
		}
	}

		5                             6 
		    6         L(5)->      5       8            
			    8                         


			    8                     6
		    6         R(5)->      5       8
		5


	        6                         5
        3       8	  LR(6)->     3       6
	 2     5                   2     4       8
	         4


			5                         5
		3       6	  RL(6)->     3       7
	 2     4       8           2     4 6     8
                 7   	            
*/
			

static AVLNode* newBinNode(int num) {
	AVLNode* newNode = (AVLNode*)malloc(sizeof(AVLNode));
	memset(newNode, 0, sizeof(AVLNode));
	newNode->value = num;
	newNode->left = NULL;
	newNode->right = NULL;
	newNode->height = 1;
	return newNode;
}


AVLNode* searchAVL(AVLNode* avlTree, int num) {
	if (avlTree) {
		if (num < avlTree->value) {
			return searchAVL(avlTree->left,num);
		}
		else if(num > avlTree->value)
		{
			return searchAVL(avlTree->right,num);
		}
		else {
			return avlTree;
		}
	}
	return NULL;
}


void initAVLTree(AVLNode** avlTree, int nums[], int count) {
	for (int i = 0; i < count; i++)
	{
		insertAVLNode(avlTree, nums[i], NULL);
	}
}


static int get_height(AVLNode* avlTree)
{
	return avlTree ? avlTree->height : 0;
}

static void update_height(AVLNode* avlTree)
{
	avlTree->height = 1 + max(get_height(avlTree->left), get_height(avlTree->right));
}

static int get_bf(AVLNode* avlTree) {
	return get_height(avlTree->left) - get_height(avlTree->right);
}

static void rotate_left(AVLNode** avlTree) {
	AVLNode* right = (*avlTree)->right;
	(*avlTree)->right = right->left;
	right->left = *avlTree;
	*avlTree = right;

	update_height((*avlTree)->left);
	update_height((*avlTree));
}

static void rotate_right(AVLNode** avlTree) {
	AVLNode* left = (*avlTree)->left;
	(*avlTree)->left = left->right;
	left->right = *avlTree;
	*avlTree = left;

	update_height((*avlTree)->right);
	update_height((*avlTree));
}

static void try_balance(AVLNode** avlTree)
{
	int bf = get_bf((*avlTree));
	if (bf>=2) {
		int leftBF = get_bf((*avlTree)->left);
		if (leftBF < 0) {
			// LR
			rotate_left(&(*avlTree)->left);
			rotate_right(avlTree);
		}
		else {
			// R
			rotate_right(avlTree);
		}
	}
	else if(bf<=-2){
		int rightBF = get_bf((*avlTree)->right);
		if (rightBF > 0) {
			// RL
			rotate_right(&(*avlTree)->right);
			rotate_left(avlTree);
		}
		else {
			// L
			rotate_left(avlTree);
		}
	}
}

int insertAVLNode(AVLNode** avlTree, int num) { // 是否插入成功
	if (!(*avlTree)) {
		AVLNode* newNode = newBinNode(num);
		*avlTree = newNode;
		return 1;
	}
	else {
		if (num < (*avlTree)->value) {
			int succ = insertAVLNode(&(*avlTree)->left, num);
			if (succ) {
				try_balance(avlTree);
				update_height(*avlTree);
			}
			return succ;
		}
		else if (num >(*avlTree)->value) {
			int succ = insertAVLNode(&(*avlTree)->right, num);
			if (succ) {
				try_balance(avlTree);
				update_height(*avlTree);
			}
			return succ;
		}
		else {
			return 0;
		}
	}
}

static void deleteAVLNode_helper(AVLNode** avlTree) { // 是否删除成功
	if (*avlTree) {
		deleteAVLNode_helper(&(*avlTree)->right);
		try_balance(avlTree);
		update_height(*avlTree);
	}
}

int deleteAVLNode(AVLNode** avlTree, int num) { // 是否删除成功
	if (!(*avlTree)) {
		return 0;
	}
	int succ = 0;
	if (num < (*avlTree)->value) {
		succ = deleteAVLNode(&(*avlTree)->left,num);
	}
	else if (num > (*avlTree)->value) {
		succ = deleteAVLNode(&(*avlTree)->right, num);
	}
	else { // ==
		if (!(*avlTree)->left) {
			AVLNode* forDel = (*avlTree);
			*avlTree = (*avlTree)->right;
			free(forDel);
		}else if(!(*avlTree)->right){
			AVLNode* forDel = (*avlTree);
			*avlTree = (*avlTree)->left;
			free(forDel);
		}
		else { // 左右子树都存在，左子树的最后一个补上来
			AVLNode** frontNode = &(*avlTree)->left;
			while ((*frontNode)->right)
			{
				frontNode = &(*frontNode)->right;
			}
			(*avlTree)->value = (*frontNode)->value;
			AVLNode* forDel = (*frontNode);
			(*frontNode) = (*frontNode)->left;
			free(forDel);

			deleteAVLNode_helper(&(*avlTree)->left);
		}
		succ = 1;
	}

	if (succ == 1) {
		if (*avlTree) {
			try_balance(avlTree);
			update_height(*avlTree);
		}
	}
	return succ;
}
