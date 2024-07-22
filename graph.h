#pragma once
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

class AdjacencyList
{
private:
	unordered_multimap<int, vector<pair<int, double>>> graph;
	unordered_multimap<int, pair<double, double>> nodeLocations;

public:
	void insertNode(int nodeID, double latitude, double longitude)
	{
		nodeLocations.insert({ nodeID, {latitude, longitude} });
	}
	void insertEdge(int fromID, int toID, double weight)
	{
		graph.insert({ fromID, {{toID, weight}} });
	}
	//code referenced from Lecture Slides - Mod 8a - Graphs Terminology and Implementation -- slide 64
	vector<int> bfs(int startID, int endID)
	{
		//create containers for bfs
		set<int> visited;
		queue<int> queue;
		unordered_map<int, int> previous;

		//insert and push startID into visited and queue
		visited.insert(startID);
		queue.push(startID);

		//set node previous to start id as -1
		previous[startID] = -1;

		while (!queue.empty())
		{
			int u = queue.front();

			//checks if u is the same as the endID, add it to the path and keep adding the previous nodes until it reaches the previous[start] which is -1
			if (u == endID)
			{
				vector<int> pathTrace;
				for (int i = endID; i != -1; i = previous[i])
				{
					pathTrace.push_back(i);
				}

				//reverse order of path since path contains endID to fromID instead of fromID to endID
				reverse(pathTrace.begin(), pathTrace.end());
				return pathTrace;
			}

			queue.pop();

			//check if element is in the visited set, if its not, add it to vector and queue
			vector<int> neighbours = graph[u];
			for (const auto& v : neighbours)
			{
				if (visited.count(v) == 0)
				{
					visited.insert(v);
					queue.push(v);
				}
			}

			//no path found
			return {};

		}
	}
	vector<int> dijkstras(int startID, int endID);
	unordered_multimap<int, vector<pair<int, double>>>& getAdjacencyList()
	{
		return graph;
	}
	unordered_multimap<int, pair<double, double>> getNodeLoc()
	{
		return nodeLocations;
	}

};




#endif //COP3530_PROJECT_3_GRAPH_H
