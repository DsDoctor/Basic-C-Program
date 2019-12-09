//Made by Sheng Du
//Z5171466

#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
char *gets(char *str);
size_t strlen(char const *str);

// data type for nodes
typedef struct DLListNode {
	int  value;  // value (int) of this list item 
	struct DLListNode *prev;
	// pointer previous node in list
	struct DLListNode *next;
	// pointer to next node in list
} DLListNode;

//data type for doubly linked lists
typedef struct DLList {
	int  size;      // count of items in list
	DLListNode *first; // first node in list
	DLListNode *last;  // last node in list
} DLList;

// create a new DLListNode
DLListNode *newDLListNode(int it)
{
	DLListNode *new;
	new = malloc(sizeof(DLListNode));
	assert(new != NULL);
	new->value = it;
	new->prev = new->next = NULL;
	return new;
}

// create a new empty DLList
DLList *newDLList()
{
	DLList *L;

	L = malloc(sizeof(struct DLList));
	assert(L != NULL);
	L->size = 0;
	L->first = NULL;
	L->last = NULL;
	return L;
}

// create a DLList from a text file
// put your time complexity analysis for CreateDLListFromFileDlist() here
// for n-input elements file
// CreatDLListFromFileDlist runs O(n) time
// for n-input keyboard elements
// CreatDLList runs O(n) time
DLList *CreateDLListFromFileDlist(const char *filename)
{
	DLList *L = newDLList();			// create new list L
	int num;							// read input/file number
	if (strcmp(filename, "stdin") == 0)	// if filename == "stdin", then read numbers from keyboard
	{
		char ch[15];						// reserve a space to save character from keyboard
		while (gets(ch))			// for every times gets input from key board
		{
			if (strlen(ch) < 1)		// if get empty line and do not get any elements from keyboard
			{
				break;					// break loop and return empty list L
			}
			else
			{
				sscanf(&ch[0], "%d", &num);	// get input value
				goto GetValue;
			}
		}
		return L;				// return empty list L
	GetValue:
		{
			DLListNode *Node = newDLListNode(num);	// create new Node from keyboard
			L->first = Node;			// set first Node
			L->size = 1;			// set size = 1
			while (gets(ch))			// for every input from keyboard
			{
				if (strlen(ch) < 1)	// if input is an empty line
				{
					break;			// break loop, and set last Node
				}
				else
				{
					sscanf(&ch[0], "%d", &num);			// get input value
					Node->next = newDLListNode(num);	// create new Node
					Node->next->prev = Node;			// and link them
					Node = Node->next;			// move to next Node
					L->size += 1;				// size + 1
				}
			}
			L->last = Node;				// set last Node
			return L;				// return list L
		}
	}
	else				// else read numbers from filename
	{
		FILE *fp;		// pointer point to the beginning of file
		fp = fopen(filename, "r");	// open the file
		if (fp == NULL) { return L; }	// if it is fail to read the file, return empty list L
		if (fscanf(fp, "%d", &num) == 1)// if it is an empty file, skip this, and return empty list L
		{
			DLListNode *Node = newDLListNode(num);	// create a new Node
			L->first = Node;						// link L->first to Node
			L->size = 1;							// set L->size = 1
			while (fscanf(fp, "%d", &num) == 1)		// keep reading the file, until all numbers has been read
			{
				Node->next = newDLListNode(num);	// create new Node and link the new Node to Node
				Node->next->prev = Node;			// link new Node->prev to Node
				Node = Node->next;					// move to next Node
				L->size += 1;						// L->size +1
			}
			L->last = Node;							// link L->last to Node
		}
		fclose(fp);		// close the file
		return L;		// return DLList L
	}
}

// clone a DLList
// put your time complexity analysis for cloneList() here
// for n Nodes in u
// cloneList runs in O(n) time
DLList *cloneList(DLList *u)
{
	DLList *L = newDLList();		// create a new DLList L
	if (u->size >= 1)				// if u has elements, then copy u to L, else return empty L
	{
		L->size = u->size;			// copy the size of DLList u to L
		DLListNode *Node = newDLListNode(u->first->value); //create new Node of first Node of u
		L->first = Node;			// set L->first 
		if (u->size > 1)			// if u only has 1 Node then skip this
		{
			DLListNode *cur_Node = u->first->next;	// set current Node of u
			for (int i = 0; i < u->size - 1; i++)
			{
				Node->next = newDLListNode(cur_Node->value);	//creat new Node
				Node->next->prev = Node;			// create links of two Nodes
				Node = Node->next;					// move to next Node
				cur_Node = cur_Node->next;			// move current Node to next Node
			}
		}
		L->last = Node;				// set L->last
	}
	return L;
}

// compute the union of two DLLists u and v
// for n-Nodes list u and m-Nodes list v
// setUnion runs in O(n*m) time
DLList *setUnion(DLList *u, DLList *v)
{
	DLList *L = cloneList(u);			// copy DLList u to L
	DLListNode *Node_l = L->last;		// initial Node_L point to L->last
	DLListNode *Node_v = v->first;		// current Node for list V Node_v, point to v->first
	char flag = 1;
	// set a flag to 1; if flag = 1, then add the Node_v to L,else flag = 0, do NOT add Node to L
	if (L->size == 0)					// if list u has 0 Node, then copy list v to L and then return L
	{
		L = cloneList(v);
		return L;
	}
	for (int b = 0; b < (v->size); b++)	// for every Nodes in list v
	{
		DLListNode *Node_u = u->first;	// initial Node_u to first Node
		for (int a = 0; a < (u->size); a++)	// for every Nodes in list u
		{
			flag = 1;						// default list v do not include Node of this value
			if (Node_u->value == Node_v->value)	// when found the Node whose value already in list v
			{
				flag = 0;						// set flag to 0 
				break;							// break and skip next step
			}
			Node_u = Node_u->next;				// if the value are not equal, then compare next Node_u
		}
		if (flag == 1)					// flag = 1, add the Node into list L
		{
			Node_l->next = newDLListNode(Node_v->value);	// create new Node
			Node_l->next->prev = Node_l;					// link two Node
			Node_l = Node_l->next;							// move to next Node
			L->size += 1;									// L->size + 1
		}
		Node_v = Node_v->next;			// move to next node_v
	}
	L->last = Node_l;					// set L->last Node
	return L;
}

// compute the insection of two DLLists u and v
// put your time complexity analysis for intersection() here
// for n-Nodes list u and m-Nodes list v
// seIntersection runs in O(n*m) time
DLList *setIntersection(DLList *u, DLList *v)
{
	DLList *L = newDLList();			// create a new DLList L
	DLListNode *Node_u = u->first;		// initial Node_u
	DLListNode *Node_v = v->first;		// initial Node_v
	if (u->size == 0 || v->size == 0)	// if one of the list is empty list, then return empty list L
	{
		return L;
	}
	for (int a = 0; a < u->size; a++)	// for every Node in list u
	{
		Node_v = v->first;				// initial first Node in list v
		for (int b = 0; b < v->size; b++)	// for every Node in list v
		{
			if (Node_u->value == Node_v->value)	// compare the value
			{
				goto GetIntersection;			// if value are equal, then goto GetIntersection
			}
			Node_v = Node_v->next;	// next Node_v
		}
		Node_u = Node_u->next;	// next Node_u
	}
	return L;					// if not get any Intersection, then return empty list L
GetIntersection:				// get intersection
	{
		DLListNode *Node = newDLListNode(Node_u->value);	// create new Node of intersection
		L->first = Node;							// set first Node in list L
		L->size = 1;						// set L->size = 1
		Node_u = Node_u->next;				// move to next Node_u
		while (Node_u)			// for every rest of Nodes in list u
		{
			Node_v = v->first;		// initail Node for list v
			for (int b = 0; b < v->size; b++)			// for every Node in list v
			{
				if (Node_u->value == Node_v->value)	// if get intersections
				{
					Node->next = newDLListNode(Node_u->value);	// create new Node and link them
					Node->next->prev = Node;	// 
					Node = Node->next;		// move to next Node
					L->size += 1;			// L->size + 1
					break;	// break loop go next Node_u
				}
				Node_v = Node_v->next;		// move to next Node_v
			}
			Node_u = Node_u->next;		// move to next Node_u
		}
		L->last = Node;			// set last Node in list L
	}
	return L;				// return list L
}

// free up all space associated with list
// put your time complexity analysis for freeDLList() here
// for n nodes in DLList L 
// freeDLList runs in O(n)time
void freeDLList(DLList *L)
{
	if (L->size >= 1)		// if L->size == 0, then just free L
	{						// or free all nodes,then free L

		DLListNode *Node = L->first;
		for (int i = 0; i < (L->size - 1); i++)	// if L only has 1 node, skip this
		{
			Node = Node->next;
			free(Node->prev);	// free 0 - (n-1)th nodes
		}
		free(Node);				// free nth node
	}
	free(L);
	return;
}

// display items of a DLList
// put your time complexity analysis for printDDList() here
// for n nodes in DLList u 
// printDLList runs in O(n)time
void printDLList(DLList *u)
{
	if (u->size >= 1)		// if u->size == 0, then just return
	{						// or print all nodes
		DLListNode *Node = u->first;
		for (int i = 0; i < (u->size - 1); i++)	// if u only has 1 node,then skip this
		{
			printf("%d\n", Node->value);		// print node
			Node = Node->next;					// go next node
		}
		printf("%d\n", Node->value);			// print node
	}
	return;
}

int main()
{
	DLList *list1, *list2, *list3, *list4;

	list1 = CreateDLListFromFileDlist("File1.txt");
	printDLList(list1);

	list2 = CreateDLListFromFileDlist("File2.txt");
	printDLList(list2);

	list3 = setUnion(list1, list2);
	printDLList(list3);

	list4 = setIntersection(list1, list2);
	printDLList(list4);

	freeDLList(list1);
	freeDLList(list2);
	freeDLList(list3);
	freeDLList(list4);

	printf("please type all the integers of list1\n");
	list1 = CreateDLListFromFileDlist("stdin");

	printf("please type all the integers of list2\n");
	list2 = CreateDLListFromFileDlist("stdin");

	list3 = cloneList(list1);
	printDLList(list3);
	list4 = cloneList(list2);
	printDLList(list4);

	freeDLList(list1);
	freeDLList(list2);
	freeDLList(list3);
	freeDLList(list4);

	return 0;
}
