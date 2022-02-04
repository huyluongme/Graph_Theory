#include "Graph.h"

void cGraph::Graph_Init() {
	Graph.clear();
	node_sorted.clear();
	for (int i = 0; i < node.size(); i++) {
		node_sorted.push_back(node.at(i).label);
	}
	sort(node_sorted.begin(), node_sorted.end());
	for (int i = 0; i < node_sorted.size(); i++) {
		SGraph tmp;
		tmp.visited = false;
		tmp.label = node_sorted.at(i);
		tmp.adj.clear();
		for (int j = 0; j < edge.size(); j++) {
			if (tmp.label == edge[j].label_from) {
				tmp.adj.push_back(edge[j].label_to);
			}
			if (Graph_Draw->un_directed == 0) {
				if (tmp.label == edge[j].label_to) {
					tmp.adj.push_back(edge[j].label_from);
				}
			}
		}
		sort(tmp.adj.begin(), tmp.adj.end());
		Graph.push_back(tmp);
	}
}

void cGraph::BFS(string Node) {
	browsing_result.clear();
	int id = -1;
	for (int i = 0; i < Graph.size(); i++) {
		if (Graph.at(i).label == Node) {
			id = i;
			Graph.at(i).visited = true;
		}
	}

	list<string> queue;

	queue.push_back(Node);

	while (!queue.empty())
	{
		Node = queue.front();
		browsing_result.push_back(Node);
		queue.pop_front();

		for (int i = 0; i < Graph.size(); i++) {
			if (Graph.at(i).label == Node) {
				id = i;
				Graph.at(i).visited = true;
			}
		}

		for (int i = 0; i < Graph.at(id).adj.size(); i++)
		{
			int id_2 = -1;
			for (int j = 0; j < Graph.size(); j++)
				if (Graph.at(j).label == Graph.at(id).adj.at(i)) {
					id_2 = j;
					break;
				}

			if (!Graph.at(id_2).visited)
			{
				for (int k = 0; k < edge.size(); k++) {
					if (Graph_Draw->un_directed == 0) {
						if ((Graph.at(id).label == edge.at(k).label_from && Graph.at(id_2).label == edge.at(k).label_to) 
							|| (Graph.at(id).label == edge.at(k).label_to && Graph.at(id_2).label == edge.at(k).label_from))
							edge.at(k).visited = true;
					}
					else {
						if (Graph.at(id).label == edge.at(k).label_from && Graph.at(id_2).label == edge.at(k).label_to)
							edge.at(k).visited = true;
					}
				}
				Graph.at(id_2).visited = true;
				queue.push_back(Graph.at(id).adj.at(i));
			}
		}
	}
}