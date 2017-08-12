#include <iostream>
#include <math.h>
#include <algorithm> 
#include <string>
#include <sstream>

#include "a_star.hpp"

using namespace std;

// Calculates and returns the fcost
int AStar::getFCost(Node* currentNode) {
	currentNode->fCost = currentNode->gCost + currentNode->hCost;
	// TODO Step1. Return fCost as a sum of gCost and hCost	
	return currentNode->fCost;
}

// Computes the Euclidean distance between two nodes and scales it by 10 to avoid floats
int AStar::getHDistance(Node* A, Node* B) {
	// TODO Step2. Return the Euclidian distance scaled by 10
	return sqrt(pow((abs(B->posX-A->posX)),2) +pow((abs(B->posY-A->posY)),2))*10;
}

// Iterates through the node's parents and creates a list of the shortest path to take
list<Node*> AStar::retracePath (Node* startNode, Node* endNode) {
	list<Node*> path;	
	Node* currentNode = endNode;

	while (currentNode != startNode) {
		path.push_front(currentNode);
		currentNode = currentNode->parent;
	}
	return path;
}

// For the current node, cnode, discovers all walkable neighbours, and adds them to the neighbours list
list<Node*> AStar::getNeighbours(Node* cnode, Node* grid[Y_LENGTH][X_LENGTH]) {
	list<Node*> neighbours;	
	// TODO Step3. Add walkable neighbours to the neighbours list
	// Step3.1 Iterate from (cnode.y - 1) to (cnode.y + 1)
	// Step3.2 Iterate from (cnode.x - 1) to (cnode.x + 1)	
	// Step3.3 Add neighbours that are not outside the world bounds and that are walkable
	for(int i = cnode->posY -1; i <= cnode->posY + 1; i ++){
		for(int j = cnode->posX -1; j <= cnode->posX + 1; j ++){
			if(i<Y_LENGTH&&i>=0&&j<X_LENGTH&&j>=0){
				if(grid[i][j]->walkable)
					if(grid[i][j]!=cnode)
						neighbours.push_front(grid[i][j]);
			}
		}
	}

	return neighbours;
}

// Outputs the discovered path as a string
string AStar::getPathString(list<Node*> path) {
	stringstream outputStream;
	for (list<Node*>::iterator it = path.begin(); it != path.end(); ++it) {
		outputStream << "(" << (*it)->posX << "," << (*it)->posY << ")" << endl;
	}
	return outputStream.str();
}

// Outputs path distance
int AStar::getPathDistance(list<Node*> path) {
	return path.size() > 0 ? path.back() -> fCost : 0;
}

// Finds shortest path between startNode and endNode using A* algorithm
list<Node*> AStar::findPath(Node* world[Y_LENGTH][X_LENGTH], Node* startNode, Node* endNode) {
	list<Node*> openSet; // list of nodes not yet evaluated
	list<Node*> closedSet; // list of nodes already evaluated
	Node* currentNode;

	startNode->gCost = 0;
	startNode->hCost = getHDistance(startNode, endNode);
	startNode->fCost = startNode->hCost;
	
	openSet.push_back(startNode); // insert the starting node at the beginning of the open set
	while(!openSet.empty()) {	
//		break; // REMOVE this line once the following code has been implemented	
		
		// TODO Step4. Find a node in the openSet that has the smallest fCost
		// If there is a conflict, select the node with the smaller hCost
		// Use <list> iterator to iterate through the list; see sample iterator code below
		int minFcost = 2147483647;
		int minHcost = 2147483647;
			 
		for (list<Node*>::iterator i = openSet.begin(); i != openSet.end(); i ++){
			if((*i)->fCost < minFcost){
				currentNode = *i;
				minFcost = (*i)->fCost;
				minHcost = (*i)->hCost;
			}
			else if((*i)->fCost == minFcost && (*i)->hCost < minHcost){
				currentNode = *i;
				minFcost = (*i)->fCost;
				minHcost = (*i)->hCost;				
			}
		}
		
//		 TODO Step5. Remove the found node from the open list and insert it into closed list
		closedSet.push_back(currentNode);
		openSet.remove(currentNode);
		
		// TODO Step6. Get a list of walkable neighbours for the current node		
		list<Node*> neighbours = getNeighbours(currentNode, world);
		int minH2Cost = 2147483647;
		// TODO Step7. Iterate through the neighbours list and add matching neighbours to the open list	
		for(list<Node*>::iterator it = neighbours.begin(); it != neighbours.end(); it++) { 
			bool exists = false;
			// Step7.1. Check if the current neighbour is already in the closed list; if it is, skip it
			for (list<Node*>::iterator i = closedSet.begin(); i != closedSet.end(); i ++){
				if((*it)->posX==(*i)->posX && (*it)->posY==(*i)->posY)
					exists = true;
			}
			// Step7.2. Compute gCost from the start node for the curent neighbour
			// If that cost is less than previously computed gCost, update the neighbour's parent to the current node, and 
			// update gCost, hCost, and fCost values for the neighbour to match the current node
			// Use getHDistance to get the cost from the current node to the current neighour
			if(!exists){
				(*it)->gCost = currentNode->gCost + getHDistance(currentNode, *it);
				(*it)->hCost = getHDistance(*it, endNode);
				if((*it)->hCost < minH2Cost){
					minH2Cost = (*it)->hCost;
					(*it)->parent = currentNode;
					(*it)->fCost = getFCost(*it);
					openSet.push_back(*it);
				}
				
			}

 		}
 		
 		// TODO Step8. Check if the current node is the end node; if it is, return the retraced path from start to end
 		if(currentNode==endNode)
 			return retracePath(startNode,currentNode);

	}
	
	list<Node*> empty;
	return empty;
}
