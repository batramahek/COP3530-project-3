//
// Created by Mahek Batra on 2024-07-15.
//
#include <unordered_map>
#include <set>
#include <queue>
#include <string>

#ifndef COP3530_PROJECT_3_GRAPH_H
#define COP3530_PROJECT_3_GRAPH_H

using namespace std; 

class AdjancencyList
{
private:
	unordered_multimap<string, vector<pair<string, double>>> graph;
	unordered_multimap<string, pair<double, double>> nodeLocations;

public:
	void insertNode(int nodeID, double latitude, double longitude)
	{
		nodeLocations[nodeID] = { latitude, longitude };
	}
	void insertEdge(int fromID, int toID, double weight)
	{
		graph[fromID].push_back(make_pair <toID, weight>);
	}
	//code referenced from Lecture Slides - Mod 8a - Graphs Terminology and Implementation -- slide 64
	vector<int> bfs(int startID, int endID)
	{
		set<int> visited;
		queue<int> queue;

		visited.insert(startID);
		queue.push(startID);

		while (!queue.empty())
		{
			int u = q.front();
			q.pop();

			if (u == endID)
			{
				//ctor<int> pathTrace;
				//for (int i = end; i != start
			}
		}
	}
	vector<int> dijkstras(int startID, int endID);
	unordered_multimap<int, pair<int, double>>& getAdjancencyList()
	{
		return graph;
	}
	unordered_multimap<int, pair<double, double>> getNodeLoc()
	{
		return nodeLocations;
	}

};




#endif //COP3530_PROJECT_3_GRAPH_H
