//**Include Headers**//
#include "Node.h"
#include <chrono>
#include <stack>
#include <list>
#include <iostream>
#include <string>
#include <windows.h>
#include <iterator>
using namespace std;
//*******************//

//**Variables**//
Node firstNode;
list<Node> frontier;
list<Node> expanded;
//*************//

//**Function Prototypes**//
void printPuzzle(Node node);
void printPuzzleInfo(Node node);
void printPathTaken(Node node);
void copyPuzzle(Node copiedFromNode, Node* copiedToNode);
bool cmpPuzzles(Node* node1, Node* node2);
int initGameState(Node* node, int curPos);
void init(Node* firstNode, list<Node>* frontier);
bool isGoalMet(Node node);
int addAlteredChildNode(Node* parentNode, list<Node>* frontier, char direction);
void expand(list<Node>* frontier, list<Node>* expanded);
bool findGoalPath(list<Node>* frontier, list<Node>* expanded);

//***********************//

//**Test functions for functionallity**//
int main()
{
	//Function to test

	//**Test**//
	init(&firstNode, &frontier);

	frontier.begin()->gameState[0] = 1;
	frontier.begin()->gameState[1] = 4;
	frontier.begin()->gameState[2] = 2;
	frontier.begin()->gameState[3] = 3;
	frontier.begin()->gameState[4] = 0;
	frontier.begin()->gameState[5] = 5;
	frontier.begin()->gameState[6] = 6;
	frontier.begin()->gameState[7] = 7;
	frontier.begin()->gameState[8] = 8;
	frontier.begin()->zeroPos = 4;

	if (findGoalPath(&frontier, &expanded) == true)
	{
		cout << "success" << endl;
	}

	//********//

	return 0;
}
//*************************************//


//**Functions**//

//Print gameState
void printPuzzle(Node node)
{
	for (int i = 0; i < 9; i++)
	{
		cout << node.gameState[i] << " ";
		if ((i + 1) % 3 == 0)
		{
			cout << endl;
		}
	}
}

void printPuzzleInfo(Node node)
{
	cout << "X is located at: " << node.zeroPos << endl;

	cout << "This is the parent:" << endl << endl;
	if(node.parent != NULL)
	{
		printPuzzle(*(node.parent));
	}

	cout << "This is the node:" << endl << endl;
	printPuzzle(node);
}

//Print goalPath
void printPathTaken(Node node)
{
	list<Node> goalPath;
	cout << "First unit just penetrated the first layer of the enemy stronghold. Over" << endl;
	if (node.parent != NULL)
	{
		while (node.parent != NULL);
		{
			goalPath.push_back(node);
			node = *node.parent;
		}
	}

	cout << "Second unit reached the base of operations with 30% losses. Over" << endl;
	while (goalPath.empty() == false)
	{
		printPuzzle(goalPath.front());
		Sleep(1000);
		system("cls");
		goalPath.pop_front();
	}

	cout << "Survivors of third unit surrounded enemy commander\nVictory is ours. Over" << endl;
}

bool cmpPuzzles(Node* node1, Node* node2)
{
	for (int i = 0; i < 9; i++)
	{
		if (node1->gameState[i] != node2->gameState[i])
		{
			return false;
		}
	}
	return true;
}

//Copy Nodes gamestate to other Node
void copyPuzzle(Node copiedFromNode, Node* copiedToNode)
{
	for (int i = 0; i < 9; i++)
	{
		copiedToNode->gameState[i] = copiedFromNode.gameState[i];
	}
}

//Recursion till it's double no more
int initGameState(Node* node, int curPos)
{
	int tmp = rand() % 9;
	for (int j = 0; j < curPos; j++)
	{
		if (tmp == node->gameState[j])
		{
			tmp = initGameState(node, curPos);
		}
	}
	return tmp;
}

//Init Game
void init(Node* firstNode, list<Node>* frontier)
{
	//Randomise gameState
	unsigned __int64 now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	srand(now);

	for (int i = 0, tmp = 0; i < 9; i++)
	{
		firstNode->gameState[i] = initGameState(firstNode, i);
	}
	firstNode->parent = NULL;

	for (int i = 0; i < 9; i++)
	{
		if (firstNode->gameState[i] == 0)
		{
			firstNode->zeroPos = i;
		}
	}

	//Set-up Frontier
	frontier->push_front(*firstNode);
}

//Check if Game is solved
bool isGoalMet(Node node)
{
	if (node.gameState[0] == 0 && node.gameState[1] == 1 &&
		node.gameState[2] == 2 && node.gameState[3] == 3 &&
		node.gameState[4] == 4 && node.gameState[5] == 5 &&
		node.gameState[6] == 6 && node.gameState[7] == 7 &&
		node.gameState[8] == 8)
	{
		return true;
	}
	return false;
}

int addAlteredChildNode(Node* parentNode, list<Node>* frontier, char direction)
{
	Node childNode;
	copyPuzzle(*parentNode, &childNode);

	switch (direction)
	{
		case 'l':
			if (parentNode->zeroPos % 3 == 0)
			{
				return 0;
			}
			childNode.gameState[parentNode->zeroPos] = childNode.gameState[parentNode->zeroPos - 1];
			childNode.gameState[parentNode->zeroPos - 1] = 0;
			childNode.zeroPos = parentNode->zeroPos - 1;
			break;

		case 'd':
			if (parentNode->zeroPos > 5)
			{
				return 0;
			}
			childNode.gameState[parentNode->zeroPos] = childNode.gameState[parentNode->zeroPos + 3];
			childNode.gameState[parentNode->zeroPos + 3] = 0;
			childNode.zeroPos = parentNode->zeroPos + 3;
			break;

		case 'r':
			if (parentNode->zeroPos % 3 == 2)
			{
				return 0;
			}
			childNode.gameState[parentNode->zeroPos] = childNode.gameState[parentNode->zeroPos + 1];
			childNode.gameState[parentNode->zeroPos + 1] = 0;
			childNode.zeroPos = parentNode->zeroPos + 1;
			break;

		case 'u':
			if (parentNode->zeroPos < 3)
			{
				return 0;
			}
			childNode.gameState[parentNode->zeroPos] = childNode.gameState[parentNode->zeroPos - 3];
			childNode.gameState[parentNode->zeroPos - 3] = 0;
			childNode.zeroPos = parentNode->zeroPos - 3;
			break;
	}
	childNode.parent = parentNode;
	for (list<Node>::iterator i = expanded.begin(); i != expanded.end(); i++)
	{
		if (cmpPuzzles(&(*i), &childNode) == true)
		{
			return 0;
		}
	}
	frontier->push_front(childNode);
	return 1;
}

//Create and add next Nodes to Frontier
void expand(list<Node>* frontier, list<Node>* expanded)
{
	int numExplored = 0;

	numExplored += addAlteredChildNode(&(*(frontier->begin())), frontier, 'l');
	numExplored += addAlteredChildNode(&(*(frontier->begin())), frontier, 'd');
	numExplored += addAlteredChildNode(&(*(frontier->begin())), frontier, 'r');
	numExplored += addAlteredChildNode(&(*(frontier->begin())), frontier, 'u');

	expanded->push_back(*(frontier->begin()));
	list<Node>::iterator i = frontier->begin();
	for (; numExplored > 0; numExplored--, i++) 
	{
		(*i).parent = *(&(expanded->begin()));
	}

	frontier->pop_back();
}

//Get goalState
bool findGoalPath(list<Node>* frontier, list<Node>* expanded)
{
	while (frontier->empty() == false)
	{
		if (isGoalMet(frontier->front()) == true)
		{
			return true;
		}
		expand(frontier, expanded);
	}
	return false;
}

//*************//