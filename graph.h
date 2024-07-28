#pragma once
//
// Created by Mahek Batra on 2024-07-15.
//
#include <unordered_map>
#include <vector>
#pragma once
//
// Created by Mahek Batra on 2024-07-15.
//
#include <unordered_map>
#include <set>
#include <queue>
#include <string>
#include <functional> // for priority queue



#ifndef COP3530_PROJECT_3_GRAPH_H
#define COP3530_PROJECT_3_GRAPH_H

using namespace std;

class AdjacencyList
{
private:
	//graph structure
	unordered_multimap<int, vector<pair<int, double>>> graph;
	//store lat and long of nodes as pair
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

	//function to return weights between 2 connected nodes
	double getWeight(int fromID, int toID)
	{
		auto it = graph.equal_range(fromID);
		for (auto i = it.first; i != it.second; ++i)
		{
			for (const auto& neighbour : i->second)
			{
				if (neighbour.first == toID)
				{
					return neighbour.second;
				}
			}
		}
	}

	//code referenced from Lecture Slides - Mod 8a - Graphs Terminology and Implementation -- slide 63
	pair<vector<int>, double> bfs(int startID, int endID)
	{
		//create containers for bfs
		set<int> visited;
		queue<int> queue;
		unordered_map<int, int> previous;

		//unordered_map to store weightDist from fromID
		unordered_map<int, double> weightDist;

		//insert and push startID into visited and queue
		visited.insert(startID);
		queue.push(startID);

		weightDist[startID] = 0.0;

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

				//reverse order of path 
				reverse(pathTrace.begin(), pathTrace.end());
				pair<vector<int>, double> path = make_pair(pathTrace, weightDist[endID]);

				return path;
			}

			queue.pop();

			//check if element is in the visited set, if its not, add it to vector and queue
			vector<int> neighbours;
			auto it = graph.equal_range(u);
			for (auto& i = it.first; i != it.second; ++i)
			{

				for (const auto& neighbour : i->second)
				{
					int v = neighbour.first;
					if (visited.count(v) == 0)
					{
						visited.insert(v);
						queue.push(v);

						// set u to be previous of v
						previous[v] = u;

						//sum of dist
						weightDist[v] = weightDist[u] + getWeight(u, v);
					}
				}

			}
		}

		//no path found return empty
		return {};
	}

	pair<vector<int>, double> dijkstras(int startID, int endID) {    //Justin Sui 7/25-7/26
		//pq to store (distance, node)
		priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> pq;
		
		//unordered map to store distance from source node to each other node
		unordered_map<int, double> distances;
		
		//map to store the path 
        unordered_map<int, int> previous;
		
		//set to keep track of visited nodes
       	set<int> visited;

		//distance of source node to source = 0 
  		distances[startID] = 0.0;
        pq.push({0.0, startID});
	 
		//parse the queue
	 	while (!pq.empty()) 
		{
   			double dist = pq.top().first;
            int x = pq.top().second;
            pq.pop();

			//skip node if already in visited
            if (visited.find(x) != visited.end()) 
			{
                continue;
			}
            
			visited.insert(x);

			//construst path if reached the destination node
        	if (x == endID) 
			{ //construct shortest path
				vector<int> pt;
				for (int i = endID; i != -1; i = previous[i])
				{
					pt.push_back(i);
				}
                reverse(pt.begin(), pt.end());
				pair<vector<int>, double> path = make_pair(pt, distances[endID]);

                return path;
            }

			// Iterate over neighbors
			auto it = graph.equal_range(x);
			for (auto i = it.first; i != it.second; ++i) {
				for (auto& neighbor : i->second) {
					int y = neighbor.first;
					double weight = neighbor.second;
					if (visited.find(y) == visited.end() && (distances.find(y) == distances.end() || dist + weight < distances[y])) 
					{
						distances[y] = distances[x] + getWeight(x, y);
						pq.push({ distances[y], y });
						previous[y] = x;
					}
				}
			}
		}
	 	return {}; //no path found
	}


	unordered_multimap<int, vector<pair<int, double>>>& getAdjacencyList()
	{
		return graph;
	}
	unordered_multimap<int, pair<double, double>>& getNodeLoc()
	{
		return nodeLocations;
	}

};




#endif //COP3530_PROJECT_3_GRAPH_H
