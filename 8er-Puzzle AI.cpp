//**Include Headers**//
#include "Node.h"
#include <chrono>
#include <list>
#include <iostream>
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
void printPathTaken(Node* node);
void copyPuzzle(Node copiedFromNode, Node* copiedToNode);
bool cmpPuzzles(Node* node1, Node* node2);
void makeMove(Node* node);
void init(Node* firstNode, list<Node>* frontier, int numOfMovesToRandomize);
bool isGoalMet(Node* node);
int  addAlteredChildNode(Node* parentNode, list<Node>* frontier, list<Node>* expanded, char direction);
void expand(list<Node>* frontier, list<Node>* expanded);
bool findGoalPath(list<Node>* frontier, list<Node>* expanded);

//***********************//

//**Test functions for functionallity**//
int main()
{
	char showPathTaken = 0;
	char redoRandomization = 'y';
	int numOfMovesToRandomize = 0;

	cout << "How many moves from solved should the puzzle start?" << endl;
	cin >> numOfMovesToRandomize;
	system("cls");
	init(&firstNode, &frontier, numOfMovesToRandomize); //Initialize variables

	cout << "Are you fine with the randomization?[Y/N]" << endl;
	printPuzzle(firstNode);
	cin >> redoRandomization;
	system("cls");

	if (redoRandomization != 'y' && redoRandomization != 'Y')
	{
		while (redoRandomization != 'y' && redoRandomization != 'Y') //Randomize again
		{

			init(&firstNode, &frontier, numOfMovesToRandomize);

			cout << "Are you fine with the randomization?[Y/N]" << endl;
			printPuzzle(firstNode);
			cin >> redoRandomization;
			system("cls");
		}
	}

	//Set-up Frontier
	frontier.push_front(firstNode);
	
	if (findGoalPath(&frontier, &expanded) == true) //DFS
	{
		cout << "Success" << endl;
		Sleep(3000);
		system("cls");
		cout << "Would you like to see the solution?[Y/N]" << endl;
		cin >> showPathTaken;
		if (showPathTaken == 'y' || showPathTaken == 'Y')
		{
			system("cls");
			printPathTaken(&(frontier.back())); //Print Path Taken
		}
	}

	else
	{
		cout << "Failure" << endl;
	}

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

//Print goalPath
void printPathTaken(Node* node)
{
	list<Node*> goalPath;

	while (node != NULL)
	{
		goalPath.push_front(node);
		node = node->parent;
	}


	while (goalPath.empty() == false)
	{
		printPuzzle(*(goalPath.front()));
		Sleep(3000);
		system("cls");
		goalPath.pop_front();
	}
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
void makeMove(Node* node)
{
	int move = rand() % 4;
	switch (move)
	{
	case 0:
		if (node->zeroPos % 3 == 0)
		{
			makeMove(node);
			return;
		}
		node->gameState[node->zeroPos] = node->gameState[node->zeroPos - 1];
		node->gameState[node->zeroPos - 1] = 0;
		node->zeroPos = node->zeroPos - 1;
		break;

	case 1:
		if (node->zeroPos > 5)
		{
			makeMove(node);
			return;
		}
		node->gameState[node->zeroPos] = node->gameState[node->zeroPos + 3];
		node->gameState[node->zeroPos + 3] = 0;
		node->zeroPos = node->zeroPos + 3;
		break;

	case 2:
		if (node->zeroPos % 3 == 2)
		{
			makeMove(node);
			return;
		}
		node->gameState[node->zeroPos] = node->gameState[node->zeroPos + 1];
		node->gameState[node->zeroPos + 1] = 0;
		node->zeroPos = node->zeroPos + 1;
		break;

	case 3:
		if (node->zeroPos < 3)
		{
			makeMove(node);
			return;
		}
		node->gameState[node->zeroPos] = node->gameState[node->zeroPos - 3];
		node->gameState[node->zeroPos - 3] = 0;
		node->zeroPos = node->zeroPos - 3;
		break;
	}
}

//Init Game
void init(Node* firstNode, list<Node>* frontier, int numOfMovesToRandomize)
{
	//Randomise gameState
	unsigned __int64 now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	srand(now);

	for (int i = 0, tmp = 0; i < 9; i++)
	{
		firstNode->gameState[i] = i;
	}
	firstNode->zeroPos = 0;
	firstNode->parent = NULL;

	for (int i = 0; i < (numOfMovesToRandomize + 1); i++)
	{
		makeMove(firstNode);
	}
}

//Check if Game is solved
bool isGoalMet(Node* node)
{
	if (node->gameState[0] == 0 && node->gameState[1] == 1 &&
		node->gameState[2] == 2 && node->gameState[3] == 3 &&
		node->gameState[4] == 4 && node->gameState[5] == 5 &&
		node->gameState[6] == 6 && node->gameState[7] == 7 &&
		node->gameState[8] == 8)
	{
		return true;
	}
	return false;
}

int addAlteredChildNode(Node* parentNode, list<Node>* frontier, list<Node>* expanded, char direction)
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
	//check if it isn't already in expanded
	for (list<Node>::iterator i = expanded->begin(); i != expanded->end(); i++)
	{
		if (cmpPuzzles(&(*i), &childNode) == true)
		{
			return 0;
		}
	}

	//check if it isn't already in frontier
	for (list<Node>::iterator i = frontier->begin(); i != frontier->end(); i++)
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
	unsigned int numExplored = 0;

	numExplored += addAlteredChildNode(&(frontier->back()), frontier, expanded, 'l');
	numExplored += addAlteredChildNode(&(frontier->back()), frontier, expanded, 'd');
	numExplored += addAlteredChildNode(&(frontier->back()), frontier, expanded, 'r');
	numExplored += addAlteredChildNode(&(frontier->back()), frontier, expanded, 'u');

	//Add explored node to expanded & correct parent pointer of children
	expanded->push_back(frontier->back());
	list<Node>::iterator it = frontier->begin();
	for (; numExplored > 1; numExplored--, ++it) 
	{
		it->parent = &(expanded->back());
	}
	it->parent = &(expanded->back());

	frontier->pop_back();
}

//Get goalState
bool findGoalPath(list<Node>* frontier, list<Node>* expanded)
{
	while (frontier->empty() == false)
	{
		if (isGoalMet(&(frontier->back())) == true)
		{
			return true;
		}
		expand(frontier, expanded);
	}
	return false;
}

//*************//