// Made by Sheng Du
// Z5171466

#define _CRT_SECURE_NO_WARNINGS
// header file
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>
// define
#define L -1
#define R 1
#define No 0

// all the basic data structures and functions are included in this template
// you can add your own auxiliary functions as you like 
// data type for avl tree nodes
typedef struct AVLTreeNode {
	int key; //key of this item
	int value;  //value (int) of this item 
	int height; //height of the subtree rooted at this node
	struct AVLTreeNode *parent; //pointer to parent
	struct AVLTreeNode *left; //pointer to left child
	struct AVLTreeNode *right; //pointer to right child
} AVLTreeNode;

//data type for AVL trees
typedef struct AVLTree {
	int size;      // count of items in avl tree
	AVLTreeNode *root; // root
} AVLTree;

// create a new AVLTreeNode
AVLTreeNode *newAVLTreeNode(int k, int v)
{
	AVLTreeNode *new;
	new = malloc(sizeof(AVLTreeNode));
	assert(new != NULL);
	new->key = k;
	new->value = v;
	new->height = 0; // height of this new node is set to 0
	new->left = NULL; // this node has no child
	new->right = NULL;
	new->parent = NULL; // no parent
	return new;
}

// create a new empty avl tree
AVLTree *newAVLTree()
{
	AVLTree *T;
	T = malloc(sizeof(AVLTree));
	assert(T != NULL);
	T->size = 0;
	T->root = NULL;
	return T;
}

// create stack function
typedef struct Stack {
	AVLTreeNode *Node[20];
	int top;
}Stack;

Stack *CreateStack()
{
	Stack *stack;
	stack = malloc(sizeof(Stack));
	assert(stack != NULL);
	stack->top = 0;
	for (int i = 0; i < 20; i++) stack->Node[i] = NULL;
	return stack;
}

void push(Stack *stack, AVLTreeNode *Node)
{
	stack->Node[stack->top] = Node;
	stack->top++;
	return;
}

AVLTreeNode *pop(Stack *stack)
{
	if (!stack->top) return;
	stack->top--;
	return stack->Node[stack->top];
}

void release(Stack *stack)
{
	stack->Node[stack->top] = NULL;
	return;
}

// funcitons clarify 
int InsertNode(AVLTree *T, int k, int v);			
AVLTreeNode *FindRoot(AVLTreeNode *Node);			// return the root of tree
int Derection(AVLTreeNode *Node, int k, int v);		// compare two Node
void LBalance(AVLTreeNode *Node);					// left insert need blance
void RBalance(AVLTreeNode *Node);					// right insert need blance
void LRotate(AVLTreeNode *Node);
void RRotate(AVLTreeNode *Node);
void PrintSubtree(AVLTreeNode *Node);				// iterate print 
void FreeSubtree(AVLTreeNode *Node);				// iterate free
void CloneSubtree(AVLTreeNode *Node, AVLTreeNode*NewNode);		// iterate clone 
void UnionSecTree(AVLTree *T, AVLTreeNode *Node);
void CheckDelBalance(AVLTreeNode *Node);
void CreateTree(AVLTreeNode *root, AVLTreeNode *Node[], int H, int T);
void CheckHeight(AVLTree *T);


// put your time complexity analysis of CreateAVLTree() here
// for n-Node input or n-Node file 
// Tree size is n
// time complexity is O(n logn)
AVLTree *CreateAVLTree(const char *filename)
{
	AVLTree *T = newAVLTree();		// create a new tree T
	int k, v;						// key and value to prepare get input or read file
	if (strcmp(filename, "stdin") == 0)			// if filename == stdin, read keyboard
	{
		char ch[500];				// empty char reserve input from keyboard
		char *inp;					// input pointer 
		gets(ch);					// read from keyboard
		inp = ch;					// set pointer point to first char
		if (strlen(ch) < 1) return T;			// if no input at begining, return empty tree T
		else									// else read first input
		{
			if (sscanf(inp, " ( %d , %d )", &k, &v))	// get first Node
			{
				AVLTreeNode *Node = newAVLTreeNode(k, v);
				T->root = Node;							// set the root of tree
				T->size = 1;							// set the size of tree
				// time complexity is O(num) num is decided by input form, such as space number 
				while (*inp != ')') inp++;				// move pointer
				inp++;
			}
			else { printf("Error! Invalid inputs!"); _sleep(5); exit(0); }
			// time complexity is O(n-1) n is the number the input Nodes in one times
			while (*inp != 0)						// read other Node in the first input
			{
				if (sscanf(inp, " ( %d , %d )", &k, &v))		// get Node
				{
					// time complexity is O(log n)
					InsertNode(T, k, v);						// insertNode to Tree
					T->root = FindRoot(T->root);				// update the root of tree
					while (*inp != ')') inp++;			// update pointer
					inp++;
				}
				else { printf("Error! Invalid inputs!"); _sleep(5); exit(0); }
			}
			// time complexity is O(n) n is the number of input Nodes
			while (gets(ch))							// go on reading inputs
			{
				inp = ch;								// reset pointer
				if (strlen(ch) < 1) return T;
				else
				{
					while (*inp != 0)						// read other Node in the first input
					{
						if (sscanf(inp, " ( %d , %d )", &k, &v))		// get Node
						{
							InsertNode(T, k, v);						// insertNode to Tree
							T->root = FindRoot(T->root);				// update the root of tree
							while (*inp != ')') inp++;			// update pointer
							inp++;
						}
						else { printf("Error! Invalid inputs!"); _sleep(5); exit(0); }
					}
				}
			}
		}
	}
	else										// filename != stdin, read file
	{
		FILE *fp;					// pointer point to the beginning of file
		fp = fopen(filename, "r");	// open the file
		if (fp == NULL)
		{
			printf("Error! File Not Found or File is empty!\n");
		}
		fscanf(fp, " ( %d , %d )", &k, &v);		// read file get first Node
		AVLTreeNode *Node = newAVLTreeNode(k, v);
		T->root = Node;							// set the root of tree T
		T->size = 1;							// set the size of tree T
		while (fscanf(fp, " ( %d , %d )", &k, &v) != EOF)	// keep reading file
		{
			InsertNode(T, k, v);				// insertNode to tree T
			T->root = FindRoot(Node);			// update the root of tree
			Node = T->root;						// update Node point to the root of tree 
		}
		fclose(fp);					// close file
	}
	return T;		// return tree T
}

// put your time complexity analysis for CloneAVLTree() here
// for n-Node AVLTree T
// CloneAVLTree runs in O(n) time
AVLTree *CloneAVLTree(AVLTree *T)
{
	AVLTree *Tree = newAVLTree();			// create a new empty Tree
	if (!T->size) return Tree;				// if the Tree T is empty, return an empty tree
	Tree->root = newAVLTreeNode(T->root->key, T->root->value);	// if tree is not empty, copy the root
	Tree->root->height = T->root->height;						// copy the hight of root
	Tree->size = T->size;										// copy the size of tree
	if (T->root->left || T->root->right)			// if tree has children
		CloneSubtree(T->root, Tree->root);			// then copy all children the Tree has
	return Tree;							// return Tree
}

// put your time complexity for ALVTreesUNion() here
// for n-Nodes Tree1 and m-Nodes Tree2
// if n>m Union runs in time O(n + mlogn)
// if m>n Union runs in time O(m + nlogn)
AVLTree *AVLTreesUnion(AVLTree *T1, AVLTree *T2)
{
	AVLTree *T;
	if (T1->size >= T2->size)
	{
		T = CloneAVLTree(T1);		// Clone big Tree	O(n)
		UnionSecTree(T, T2->root);	// insert Nodes from small tree, runs in O(mlogn)
	}
	else
	{
		T = CloneAVLTree(T2);		// Clone big Tree	O(m)
		UnionSecTree(T, T1->root);	// insert Nodes from small tree, runs in O(nlogm)
	}
	return T;
}

// put your time complexity for ALVTreesIntersection() here
// for n-Nodes T1 and m-Nodes T2
// intersection runs in O(m+n)times
AVLTree *AVLTreesIntersection(AVLTree *T1, AVLTree *T2)
{
	AVLTree *T = newAVLTree();
	AVLTreeNode *Node1List[501];
	AVLTreeNode *Node2List[501];
	AVLTreeNode *Node[200];
	int tag = 0;
	int L1 = 0;
	int L2 = 0;
	int size = 0;
	Stack *stack1 = CreateStack();
	Stack *stack2 = CreateStack();
	if (!T1->size && !T2->size) return T;
	AVLTreeNode *Node1, *Node2;
	Node1 = T1->root;
	Node2 = T2->root;
	while (stack1->Node[0] || Node1)		// copy T1 to arrary, runs in O(n)
	{
		if (Node1) { push(stack1, Node1); Node1 = Node1->left; continue; }
		if (!Node1) { Node1 = pop(stack1); release(stack1); }
		Node1List[tag] = Node1;
		tag++;
		Node1 = Node1->right;
		continue;
	}
	tag = 0;
	while (stack2->Node[0] || Node2)		// copy T2 to array, runs in O(m)
	{
		if (Node2) { push(stack2, Node2); Node2 = Node2->left; continue; }
		if (!Node2) { Node2 = pop(stack2); release(stack2); }
		Node2List[tag] = Node2;
		tag++;
		Node2 = Node2->right;
		continue;
	}
	tag = 0;
	while (L1<T1->size && L2<T2->size)		// union in orderd array, runs in O(n+m)
	{
		switch (Derection(Node1List[L1], Node2List[L2]->key, Node2List[L2]->value))
		{
			case R: { L1 += 1; continue; }
			case L: { L2 += 1; continue; }
			case No:
			{
				Node[tag] = Node1List[L1];
				L1 += 1;
				L2 += 1;
				tag += 1;
				size += 1;
				continue;
			}
		}
	}
	T->root = newAVLTreeNode(Node[size / 2]->key, Node[size / 2]->value);
	T->root->height = log(size)+1;
	CreateTree(T->root,Node,0,size);		// runs in O(1)
	T->size = size;
	T->root = FindRoot(T->root);
	CheckHeight(T);
	return T;
}

// put the time complexity analysis for InsertNode() here    
// for n-Node Tree
// time complexity is logn
int InsertNode(AVLTree *T, int k, int v)
{
	if (!T->size) { T->root = newAVLTreeNode(k, v); T->size = 1; return; }
	AVLTreeNode *Node = T->root;			// set a pointer point a Node
	// time complexity is logn
	while (1)								// compare two Node
	{
		switch (Derection(Node, k, v))		// compare which is greater and select the derection
		{
		case L:								// Node smaller, go left
		{
			if (Node->left) { Node = Node->left; continue; }	// if there is already a Node on the left, continue comparing
			else							// this is the location should be inserted
			{
				Node->left = newAVLTreeNode(k, v);	// create the Node
				Node->left->parent = Node;			// link both side
				T->size += 1;						// increase the size of tree
				if (Node->right) { return 1; }		// check wheather the tree grown higher
				else 
				{
					LBalance(Node); 
					T->root = FindRoot(T->root);
					return 1; 
				}	// if the tree grown, check balance of the tree
			}
		}
		case R:								// Node greater, go right
		{
			if (Node->right) { Node = Node->right; continue; }	// if there is already a Node on the right, continue comparing
			else							// this is the location should be inserted
			{
				Node->right = newAVLTreeNode(k, v);	// create the Node
				Node->right->parent = Node;			// link both side
				T->size += 1;						// increase the size of tree
				if (Node->left) { return 1; }		// check wheather the tree grown higher
				else { 
					RBalance(Node);
					T->root = FindRoot(T->root);
					return 1; 
				}	// if the tree grown, check balance of the tree
			}
		}
		case No: return 0;					// there is already same Node in the tree, return 0
		}
	}
}

// put your time complexity for DeleteNode() here
// for n-Nodes Tree T
// deleteNode runs in O(logn) time
int DeleteNode(AVLTree *T, int k, int v)
{
	if (!T->size) return 0;
	AVLTreeNode *Node = T->root;
	while (Node)				// runs in O(logn) time to find Node
	{
		switch (Derection(Node, k, v))
		{
		case L: if (Node->left) { Node = Node->left; continue; }; return 0;
		case R: if (Node->right) { Node = Node->right; continue; }; return 0;
		case No: goto Delete;
		}
	}
Delete:							// runs in O(1)
	if (!Node->left && !Node->right)
	{
		if (!Node->parent) { free(Node); T->root = NULL; T->size = 0; return 1; }
		if (Derection(Node->parent, k, v) == R)
		{
			Node = Node->parent;
			free(Node->right);
			Node->right = NULL;
			T->size -= 1;
			if (Node->left) 
			{
				if (!Node->left->height) return 1;
				CheckDelBalance(Node);
				T->root = FindRoot(Node);
				return 1;
			}
			if (!Node->parent) { T->root = Node; return 1;}
			Node->height -= 1;
			CheckDelBalance(Node->parent);
			T->root = FindRoot(Node);
			return 1;
		}
		else
		{
			Node = Node->parent;
			free(Node->left);
			Node->left = NULL;
			T->size -= 1;
			if (Node->right)
			{
				if (!Node->right->height) return 1;
				CheckDelBalance(Node);
				T->root = FindRoot(Node);
				return 1;
			}
			if (!Node->parent) { T->root = Node; return 1; }
			Node->height -= 1;
			if (Node->parent->left->height == 1) return 1;
			CheckDelBalance(Node->parent);
			T->root = FindRoot(Node);
			return 1;

		}
		return 1;
	}
	if (!Node->left && Node->right)
	{
		AVLTreeNode *R_Node = Node->right;
		if (!Node->parent) 
		{ 
			free(R_Node->parent); 
			R_Node->parent = NULL; 
			T->root = R_Node;
			T->size -= 1;
			return 1;
		}
		Node = Node->parent;
		if (Derection(Node, k, v) == R)
		{
			free(Node->right);
			Node->right = R_Node;
			R_Node->parent = Node;
			T->size -= 1;
			CheckDelBalance(Node);
			T->root = FindRoot(T->root);
			return 1;
		}
		free(Node->left);
		Node->left = R_Node;
		R_Node->parent = Node;
		T->size -= 1;
		CheckDelBalance(Node);
		T->root = FindRoot(T->root);
		return 1;
	}
	if (Node->left && !Node->right)
	{
		AVLTreeNode *L_Node = Node->left;
		if (!Node->parent)
		{
			free(L_Node->parent);
			L_Node->parent = NULL;
			T->root = L_Node;
			T->size -= 1;
			return 1;
		}
		Node = Node->parent;
		if (Derection(Node, k, v) == L)
		{
			free(Node->left);
			Node->right = L_Node;
			L_Node->parent = Node;
			T->size -= 1;
			CheckDelBalance(Node);
			T->root = FindRoot(T->root);
			return 1;
		}
		free(Node->right);
		Node->right = L_Node;
		L_Node->parent = Node;
		T->size -= 1;
		CheckDelBalance(Node);
		T->root = FindRoot(T->root);
		return 1;
	}
	if (Node->left && Node->right)
	{
		if (Node->left->height - Node->right->height >= 0)
		{
			AVLTreeNode *L_Node = Node->left;
			while (L_Node->right) L_Node = L_Node->right;
			Node->right->parent = L_Node;
			Node->left->parent = L_Node;
			L_Node->height = Node->height;
			L_Node->right = Node->right;
			AVLTreeNode *RotateNode = L_Node->parent;
			L_Node->left = Node->left;
			free(Node);
			Node = NULL;
			T->root = L_Node;
			if (!RotateNode->left->height) return 1;
			RRotate(RotateNode);
			return 1;
		}
		else
		{
			AVLTreeNode *R_Node = Node->right;
			while (R_Node->left) R_Node = R_Node->left;
			Node->left->parent = R_Node;
			Node->right->parent = R_Node;
			R_Node->height = Node->height;
			R_Node->left = Node->left;
			AVLTreeNode *RotateNode = R_Node->parent;
			R_Node->left = Node->right;
			free(Node);
			Node = NULL;
			T->root = R_Node;
			if (!RotateNode->right->height) return 1;
			LRotate(RotateNode);
			return 1;
		}
	}

}

// put your time complexity analysis for Search() here
// for n-Node AVLTree 
// Search runs in O(logn) time
AVLTreeNode *Search(AVLTree *T, int k, int v)
{
	if (!T->size) return NULL;					// if T is empty tree, return NULL
	AVLTreeNode *Node = T->root;				// if T has elements, Node point to its root
	while (Node)								// this loop will takes O(logn) time
	{
		switch (Derection(Node, k, v))			// compare Node and input
		{
		case L:						// input is smaller
		{
			if (Node->left) { Node = Node->left; continue; } // if there is already a Node at Node->left, compare 
			return NULL;
		}
		case R:						// input is greater
		{
			if (Node->right) { Node = Node->right; continue; }
			return NULL;
		}
		case No: return Node;		// input is equal to Node
		}
	}
}

// put your time complexity analysis for freeAVLTree() here
// for n-Nodes tree
// FreeAVLTree runs O(n)time
void FreeAVLTree(AVLTree *T)
{
	if (!T->size) { free(T); return; }		// if Tree T is empty, free T and return
	AVLTreeNode *Node = T->root;			// read the root of tree
	if (Node->left)FreeSubtree(Node->left);			// free the left half of the tree
	if (Node->right)FreeSubtree(Node->right);		// free the right half of the tree
	free(Node);										// free the root
	free(T);										// free Tree
	return;
}

// put your time complexity analysis for PrintAVLTree() here
// for n-Nodes tree
// PrintAVLTree runs O(n)time
// PrintSubtree(Node->left) and PrintSubtree(Node->right) take O(n/2) respectively.
void PrintAVLTree(AVLTree *T)
{
	if (!T->size) return;					// if the tree is empty, return
	AVLTreeNode *Node = T->root;			// read the root of tree
	if (Node->left) PrintSubtree(Node->left);		// print left half of the tree
	printf("(%d,%d),%d\n", Node->key, Node->value,Node->height); // print the root
	if (Node->right) PrintSubtree(Node->right);		// print right half of the tree
	return;
}

// additional function
AVLTreeNode *FindRoot(AVLTreeNode *Node)
{
	AVLTreeNode *Root = Node;
	while (Root->parent) Root = Root->parent;
	return Root;
}

int Derection(AVLTreeNode *Node, int k, int v)
{
	if (k - Node->key > 0) return R;
	if (k - Node->key < 0) return L;
	else
	{
		if (v - Node->value > 0) return R;
		if (v - Node->value < 0) return L;
		else return No;
	}
}

void LBalance(AVLTreeNode *Node)
{
	Node->height += 1;							// Node->height + 1
	if (!Node->parent) return;					// if Node has no parent(root), return
	Node = Node->parent;						// else point to its parent
	if (!Node->right) { RRotate(Node); return; }	// check if Node has no right Node,it must be LL, 
													// and need single Right Rotate
	if (!Node->left) 
	{
		RRotate(Node->right);
		Node->right->height += 1;
		LRotate(Node);
		return;
	}
	switch (Node->left->height - Node->right->height)
	{
	case 1: { LBalance(Node); return; }
	case -1: { RBalance(Node); return; }
	case 2: { RRotate(Node); return;  }			// LL, single Right Rotate
	case -2: 
	{
		RRotate(Node->right);
		Node->right->height += 1;
		LRotate(Node);
	}								// RL, RRotate and LRotate
	}
	return;
}

void RBalance(AVLTreeNode *Node)
{
	Node->height += 1;							// Node->height + 1
	if (!Node->parent) return;					// if Node has no parent(root), return
	Node = Node->parent;						// else point to its parent
	if (!Node->left) { LRotate(Node); return; }		// check if Node has no left Node,it must be RR, 
													// and need single Left Rotate
	if (!Node->right)								// RL, RRotate and LRotate
	{
		LRotate(Node->left);					// RRotate Node->left
		Node->left->height += 1;				// ensure height is correct
		RRotate(Node);							// LRotate Node
		return;
	}
	switch (Node->right->height - Node->left->height)
	{
	case 1: { RBalance(Node); return; }
	case -1: { LBalance(Node); return; }		// continue compare
	case 2: { LRotate(Node); return; }			// RR, single Left Rotate
	case -2:									// LR, LRotate and RRotate
	{
		LRotate(Node->left);
		Node->left->height += 1;
		RRotate(Node);
	}								
	}
	return;
}

void LRotate(AVLTreeNode *Node)
{
	AVLTreeNode *R_Node = Node->right;
	Node->right = R_Node->left;
	if (Node->right) Node->right->parent = Node;
	R_Node->left = Node;
	if (Node->parent)
	{
		if (Node == Node->parent->right) Node->parent->right = R_Node;
		if (Node == Node->parent->left) Node->parent->left = R_Node;
		R_Node->parent = Node->parent;
	}
	else R_Node->parent = NULL;
	Node->parent = R_Node;
	Node->height -= 1;
	return;
}

void RRotate(AVLTreeNode *Node)
{
	AVLTreeNode *l_Node = Node->left;
	Node->left = l_Node->right;
	if (Node->left) Node->left->parent = Node;
	l_Node->right = Node;
	if (Node->parent)
	{
		if (Node == Node->parent->left) Node->parent->left = l_Node;
		if (Node == Node->parent->right) Node->parent->right = l_Node;
		l_Node->parent = Node->parent;
	}
	else l_Node->parent = NULL;
	Node->parent = l_Node;
	Node->height -= 1;
	return;
}

void PrintSubtree(AVLTreeNode *Node)
{
	if (Node)				// Node is not NULL
	{
		PrintSubtree(Node->left);		// print left half of subtree,whose root is Node->left
		printf("(%d,%d),%d\n", Node->key, Node->value,Node->height); // print the root of subtree
		PrintSubtree(Node->right);		// print right half of subtree, whose root is Node->right
	}
	return;
}

void FreeSubtree(AVLTreeNode *Node)
{
	if (Node)				// Node is not NULL
	{
		FreeSubtree(Node->left);		// free left half of subtree, whose root is Node->left
		FreeSubtree(Node->right);		// free right half of subtree, whose root is Node->right
		free(Node);						// free the root of subtree
	}
	return;
}

void CloneSubtree(AVLTreeNode *Node, AVLTreeNode *NewNode)
{
	if (Node->left) 
	{
		NewNode->left = newAVLTreeNode(Node->left->key, Node->left->value);	// create a duplicate of Node, key and value
		NewNode->left->height = Node->left->height;							// copy height
		NewNode->left->parent = NewNode;									// link two Node
		CloneSubtree(Node->left, NewNode->left);							// continue copy left child
	}
	if (Node->right)
	{
		NewNode->right = newAVLTreeNode(Node->right->key, Node->right->value);	// create a duplicate of Node, key and value
		NewNode->right->height = Node->right->height;						// copy height
		NewNode->right->parent = NewNode;									// link two Node
		CloneSubtree(Node->right, NewNode->right);							// continue copy right child
	}
	return;
}

void UnionSecTree(AVLTree *T, AVLTreeNode *Node)
{
	if (Node)
	{
		UnionSecTree(T, Node->left);
		InsertNode(T, Node->key, Node->value);
		UnionSecTree(T, Node->right);
	}
	return;
}

void CheckDelBalance(AVLTreeNode *Node)
{
	if (!Node->right)
	{
		if (Node->left->left) 
		{
			RRotate(Node);
			if (Node->left) Node->parent->height += 1;
			else Node->height -= 1;
			return;
		}
		LRotate(Node->left);
		Node->left->height += 1;
		RRotate(Node);
		Node->height -= 1;
		return;
	}
	if (!Node->left)
	{
		if (Node->right->right)
		{
			LRotate(Node);
			if (Node->right) Node->parent->height += 1;
			else Node->height -= 1;
			return;
		}
		RRotate(Node->right);
		Node->right->height += 1;
		LRotate(Node);
		Node->height -= 1;
		return;
	}
	if (Node->left->height - Node->right->height == 2)
	{
		RRotate(Node);
		if(!(Node->left->left || Node->left->right))
			Node->height -= 1;
		else Node->parent->height += 1;
		return;
	}
	if (Node->left->height - Node->right->height == -2)
	{
		LRotate(Node);
		if(!(Node->right->right) || Node->right->left)
			Node->height -= 1;
		else Node->parent->height += 1;
		return;
	}
	if (!(Node->left->height - Node->right->height))
	{
		Node->height -= 1;
		if(Node->parent) CheckDelBalance(Node->parent);
		return;
	}
}

void CreateTree(AVLTreeNode *root, AVLTreeNode *Node[],int H,int T)
{	
	if (T - H <= 1)return;
	if ((H+T)/2>H)
	{
		root->left = newAVLTreeNode(Node[(3*H+T)/4]->key, Node[(3 * H + T) / 4]->value);
		root->left->parent = root;
		root->left->height = root->left->parent->height -1;
		CreateTree(root->left, Node, H, ((T + H) / 2));
	}
	if ((H+T)/2 <=T)
	{
		if (Derection(root, Node[(H + 3 * T) / 4]->key, Node[(H + 3 * T) / 4]->value) == No) return;
		root->right = newAVLTreeNode(Node[(H+3*T)/4]->key, Node[(H + 3 * T) / 4]->value);
		root->right->parent = root;
		root->right->height = root->right->parent->height -1;
		CreateTree(root->right, Node, ((T + H) / 2) + 1,T);
	}
	return;
}

void CheckHeight(AVLTree *T)
{
	AVLTreeNode *Node = T->root;
	while (Node->right) Node = Node->right;
	if (!(Node->left) && !(Node->right))
	{
		if (Node->height > 0)
			Node->height -= 1;
	}
	return;
}

int main() //sample main for testing 
{
	int i, j;
	AVLTree *tree1, *tree2, *tree3, *tree4, *tree5, *tree6, *tree7, *tree8;
	AVLTreeNode *node1;

	tree1 = CreateAVLTree("stdin");
	PrintAVLTree(tree1);
	FreeAVLTree(tree1);
	//you need to create the text file file1.txt
	// to store a set of items without duplicate items
	tree2 = CreateAVLTree("file1.txt");
	PrintAVLTree(tree2);
	tree3 = CloneAVLTree(tree2);
	PrintAVLTree(tree3);
	FreeAVLTree(tree2);
	FreeAVLTree(tree3);
	//Create tree4 
	tree4 = newAVLTree();
	j = InsertNode(tree4, 10, 10);
	for (i = 0; i < 15; i++)
	{
		j = InsertNode(tree4, i, i);
		if (j == 0) printf("(%d, %d) already exists\n", i, i);
	}
	PrintAVLTree(tree4);
	node1 = Search(tree4, 20, 20);
	if (node1 != NULL)
		printf("key= %d value= %d\n", node1->key, node1->value);
	else
		printf("Key 20 does not exist\n");

	for (i = 17; i > 0; i--)
	{
		j = DeleteNode(tree4, i, i);
		if (j == 0)
			printf("Key %d does not exist\n", i);
		PrintAVLTree(tree4);
	}
	FreeAVLTree(tree4);
	//Create tree5
	tree5 = newAVLTree();
	j = InsertNode(tree5, 6, 25);
	j = InsertNode(tree5, 6, 10);
	j = InsertNode(tree5, 6, 12);
	j = InsertNode(tree5, 6, 20);
	j = InsertNode(tree5, 9, 25);
	j = InsertNode(tree5, 10, 25);
	PrintAVLTree(tree5);
	//Create tree6
	tree6 = newAVLTree();
	j = InsertNode(tree6, 6, 25);
	j = InsertNode(tree6, 5, 10);
	j = InsertNode(tree6, 6, 12);
	j = InsertNode(tree6, 6, 20);
	j = InsertNode(tree6, 8, 35);
	j = InsertNode(tree6, 10, 25);
	PrintAVLTree(tree6);
	tree7 = AVLTreesIntersection(tree5, tree6);
	tree8 = AVLTreesUnion(tree5, tree6);
	PrintAVLTree(tree7);
	PrintAVLTree(tree8);
	return 0;
}
