#pragma once

class Node
{
	public:
	Node* parent = nullptr;
	int gameState[9] = {};
	int zeroPos = -1;
};