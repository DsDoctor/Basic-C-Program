// Made By Sheng Du
// Z5171466
// COMP 9024 Assignment 3

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

// data type for heap nodes
typedef struct HeapNode {
	// each node stores the priority (key), name, execution time,
	//  release time and deadline of one task
	int key; //key of this task
	int TaskName;  //task name 
	int Etime; //execution time of this task
	int Rtime; // release time of this task
	int Dline; // deadline of this task
} HeapNode;

//data type for a priority queue (heap) 
typedef struct BinomialHeap { //this is heap header
	int size;      // count of items in the heap
	// add additional fields here
	struct HeapNode** NodeHeap;		// point to a struct array
} BinomialHeap;

//data type for Cores situations 
typedef	struct Core {
	int CoreNum;		// Core number
	int CoreState;		// Core state, if CoreState <= time means Core is available 
} Core;

// create a new heap node to store an item (task) 
HeapNode* newHeapNode(int k, int n, int c, int r, int d)
{ // k:key, n:task name, c: execution time, r:release time, d:deadline
  // you need to define other parameters yourself) 	 
	HeapNode* new;
	new = malloc(sizeof(HeapNode));
	assert(new != NULL);
	new->key = k;
	new->TaskName = n;
	new->Etime = c;
	new->Rtime = r;
	new->Dline = d;
	// initialise other fields 
	return new;
}

// create a new empty heap-based priority queue
BinomialHeap* newHeap()
{ // this function creates an empty binomial heap-based priority queue
	BinomialHeap* T;
	T = (BinomialHeap*)malloc(sizeof(BinomialHeap));
	assert(T != NULL);
	T->size = 0;
	HeapNode** Nodes = (HeapNode * *)malloc(sizeof(HeapNode*) * 101);
	for (int i = 0; i < 101; i++)
	{
		Nodes[i] = (HeapNode*)malloc(sizeof(HeapNode));
		assert(Nodes[i] != NULL);
		Nodes[i]->Dline = 0;
		Nodes[i]->Etime = 0;
		Nodes[i]->key = 0;
		Nodes[i]->Rtime = 0;
		Nodes[i]->TaskName = -1;
	}
	T->NodeHeap = Nodes;
	T->NodeHeap[0] = NULL;
	T->NodeHeap[1] = NULL;
	return T;
}

// create a new heap of m Cores
Core** newCores(int m)
{
	Core** Cores = (Core**)malloc((m+1) * sizeof(Core*));
	for (int i = 0; i <= m; i++)
	{
		Cores[i] = (Core*)malloc(sizeof(Core));
		assert(Cores[i] != NULL);
		Cores[i]->CoreNum = i;
		Cores[i]->CoreState = 0;
	}
	return Cores;
}

// check the NodesHeap is a Heap
void CheckNodesUp(HeapNode** Nodes, int size)
{
	if (size == 1) return;
	if (Nodes[size]->key < Nodes[size / 2]->key)
	{
		HeapNode* Node = Nodes[size];
		Nodes[size] = Nodes[size / 2];
		Nodes[size / 2] = Node;
		CheckNodesUp(Nodes, size / 2);
	}
	return;
}

void CheckNodesDown(HeapNode * *Nodes, int size)
{
	int i = 1;
	while (i <= size)
	{
		if (2 * i + 1 <= size) // Node has right child and left child
		{
			if (Nodes[2 * i + 1]->key < Nodes[2 * i]->key)		// right child smaller
			{
				HeapNode* Node = Nodes[i];
				Nodes[i] = Nodes[2 * i + 1];
				Nodes[2 * i + 1] = Node;
				i = 2 * i + 1;
				continue;
			}
		}
		if (2 * i <= size)				// Node only has left child
		{
			if (Nodes[2 * i]->key < Nodes[i]->key)
			{
				HeapNode* Node = Nodes[i];
				Nodes[i] = Nodes[2 * i];
				Nodes[2 * i] = Node;
				i = 2 * i;
				continue;
			}
		}
		return;
	}
	return;
}

void CheckCoreDown(Core** Cores, int m)
{
	int i = 1;
	while (i <= m)
	{
		if (2 * i + 1 <= m) // Node has right child and left child
		{
			if (Cores[2 * i + 1]->CoreState < Cores[2 * i]->CoreState)		// right child smaller
			{
				Core* Core = Cores[i];
				Cores[i] = Cores[2 * i + 1];
				Cores[2 * i + 1] = Core;
				i = 2 * i + 1;
				continue;
			}
		}
		if (2 * i <= m)				// Node only has left child
		{
			if (Cores[2 * i]->CoreState < Cores[i]->CoreState)
			{
				Core* Core = Cores[i];
				Cores[i] = Cores[2 * i];
				Cores[2 * i] = Core;
				i = 2 * i;
				continue;
			}
		}
		return;
	}
	return;
}

// put the time complexity analysis for Insert() here   
// for n tasks Insert runs in O(nlogn)
void Insert(BinomialHeap* T, int k, int n, int c, int r, int d)
{ // k: key, n: task name, c: execution time, r: release time, d:deadline 
  // You don't need to check if this task already exists in T 	 
  //put your code here
	T->size += 1;
	T->NodeHeap[T->size] = newHeapNode(k, n, c, r, d);
	CheckNodesUp(T->NodeHeap, T->size);	// runs in O(logn)
}

// put your time complexity for RemoveMin() here
// for n tasks RemoveMin runs in O(logn)
HeapNode* RemoveMin(BinomialHeap* T)
{
	HeapNode* Node = T->NodeHeap[1];		// set a pointer
	if (T->size == 1)
	{
		T->NodeHeap[1] = NULL;
		T->size -= 1;
		return Node;
	}
	T->NodeHeap[1] = T->NodeHeap[T->size];
	T->NodeHeap[T->size] = Node;
	T->size -= 1;
	T->NodeHeap[T->size + 1] = NULL;
	CheckNodesDown(T->NodeHeap, T->size);	// runs in O(logn)
	return Node;
}

// return the minimum key of a heap
int Min(BinomialHeap* T)
{
	return T->NodeHeap[1]->key;
}

// put your time complexity analysis for MyTaskScheduler here
// for n tasks TaskScheduler runs in O(nlogn)
int TaskScheduler(char* f1, char* f2, int m)
{
	//---------------------------------------------------------------------------------------------------------
	// runs in O(nlogn)
	FILE* fp1;										// 	read file1
	fp1 = fopen(f1, "r");
	// if file1 does not exist, print information and exit
	if (fp1 == NULL) { printf("file1 does not exist\n"); exit(0); }
	int v, c, r, d;									// record value of nodes
	BinomialHeap* HeapRt = newHeap();				// save Nodes to a Heap whoes key is Release time
	HeapNode* Node;
	// create Release Time heap, for n Nodes runs in O(n*logn)
	while (fscanf(fp1, " %d %d %d %d ", &v, &c, &r, &d) != EOF)
		Insert(HeapRt, r, v, c, r, d);				// O(logn) create Heap, key = release time
	fclose(fp1);									// file1 read done, close file1
//---------------------------------------------------------------------------------------------------------
// runs in O(mlogm)
	Core * *Cores = newCores(m);						// create a Heap of Cores, for m Cores runs in O(mlogm)
//---------------------------------------------------------------------------------------------------------
// runs in O(nlogn)
	FILE * fp2;
	fp2 = fopen(f2, "w");
	BinomialHeap * HeapDt = newHeap();				// save Nodes to a Heap whoes key is Deadline
	int time = 0;									// initialize time = 0   
MainFunction:
	// the worst situation runs in O(nlogn), n is the number of tasks
	// transmit released tasks to HeapDt
	while (HeapRt->NodeHeap[1] != NULL)				// while there is Node in Heap Rt, worst takes O(n)
	{
		if (Min(HeapRt) > time) break;	// if there is no released task, break
		Node = RemoveMin(HeapRt);					// O(logn)
		Insert(HeapDt, Node->Dline, Node->TaskName, Node->Etime, Node->Rtime, Node->Dline);	//O(logn)
	}
	// in HeapDt ,run EDF 
	// worst situation runs in O(mlogm), m is the number of Cores
	while (HeapDt->NodeHeap[1] != NULL)
	{
		// if there is no Core can be used, time + 1 go back
		if (Cores[1]->CoreState > time) { time += 1; goto MainFunction; }
		// else write in file2
		fprintf(fp2, "%d Core%d %d\n", HeapDt->NodeHeap[1]->TaskName, Cores[1]->CoreNum, time);
		// update Core State
		Cores[1]->CoreState = time + HeapDt->NodeHeap[1]->Etime;
		CheckCoreDown(Cores, m);
		Node = RemoveMin(HeapDt);					// remove Node from HeapDt
		if (Node->key < time + Node->Etime)						// check the task can be completed on time
		{
			fprintf(fp2, "where task %d misses its deadline\n", Node->TaskName);	// if not, write in file2 and return 0
			fclose(fp2);
			return 0;
		}
	}
	if (HeapRt->NodeHeap[1] != NULL) { time += 1; goto MainFunction; }
	fclose(fp2);									// close file2
	return 1;
}

int main() //sample main for testing 
{
	int i;
	i = TaskScheduler("samplefile1.txt", "feasibleschedule1.txt", 4);
	if (i == 0) printf("No feasible schedule!\n");
	/* There is a feasible schedule on 4 cores */
	i = TaskScheduler("samplefile1.txt", "feasibleschedule2.txt", 3);
	if (i == 0) printf("No feasible schedule!\n");
	/* There is no feasible schedule on 3 cores */
	i = TaskScheduler("samplefile2.txt", "feasibleschedule3.txt", 5);
	if (i == 0) printf("No feasible schedule!\n");
	/* There is a feasible schedule on 5 cores */
	i = TaskScheduler("samplefile2.txt", "feasibleschedule4.txt", 4);
	if (i == 0) printf("No feasible schedule!\n");
	/* There is no feasible schedule on 4 cores */
	i = TaskScheduler("samplefile3.txt", "feasibleschedule5.txt", 2);
	if (i == 0) printf("No feasible schedule!\n");
	/* There is no feasible schedule on 2 cores */
	i = TaskScheduler("samplefile4.txt", "feasibleschedule6.txt", 2);
	if (i == 0) printf("No feasible schedule!\n");
	/* There is a feasible schedule on 2 cores */
	return 0;
}