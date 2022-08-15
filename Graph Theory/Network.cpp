#include "Network.h"

cNetwork* Network = new cNetwork();

void cNetwork::Graph_Network_Init(Graph_Network* G) {
	G->n = Graph_Algorithms->Graph.Vertex.size();
	for (int u = 0; u < Graph_Algorithms->Graph.Vertex.size(); u++) {
		for (int v = 0; v < Graph_Algorithms->Graph.Vertex.size(); v++) {
			if (Graph_Algorithms->Graph.A.at(u).at(v) > 0) {
				G->C[u][v] = Graph_Algorithms->Graph.C.at(u).at(v);
			}
		}
	}
}

void cNetwork::init_flow(Graph_Network* pG) {
	for (int u = 0; u < pG->n; u++) {
		for (int v = 0; v < pG->n; v++) {
			pG->F[u][v] = 0.0f;
		}
	}
}

Label label[MAX_N];

float cNetwork::FordFulkerson(Graph_Network* pG, int s, int t) {
	init_flow(pG);
	float max_flow = 0;
	list<int> queue;

	do {
		for (int i = 0; i < pG->n; i++)
			label[i].dir = 0;
		label[s].dir = +1;
		label[s].p = s;
		label[s].sigma = INF;
		queue.clear();
		queue.push_back(s);
		int found = 0;
		while (!queue.empty()) {
			int u = queue.front();
			queue.pop_front();
			for (int j = 0; j < pG->n; j++) {
				if (pG->C[u][j] != 0 && !label[j].dir && pG->F[u][j] < pG->C[u][j]) {
					label[j].dir = +1;
					label[j].p = u;
					label[j].sigma = min(label[u].sigma, pG->C[u][j] - pG->F[u][j]);
					queue.push_back(j);
				}
			}
			for (int j = 0; j < pG->n; j++) {
				if (pG->C[j][u] != 0 && !label[j].dir && pG->F[j][u] > 0) {
					label[j].dir = -1;
					label[j].p = u;
					label[j].sigma = min(label[u].sigma, pG->F[j][u]);
					queue.push_back(j);
				}
			}
			if (label[t].dir != 0) {
				found = 1;
				break;
			}
		}
		if (found) {
			int sigma = label[t].sigma;
			int u = t;
			while (u != s) {
				int p = label[u].p;
				if (label[u].dir > 0)
					pG->F[p][u] += sigma;
				else
					pG->F[u][p] -= sigma;
				u = p;
			}
			max_flow += sigma;
		}
		else break;
	} while (1);
	return max_flow;
}

void cNetwork::Network_Process() {
	network_log.clear();
	Graph_Network G;
	Graph_Network_Init(&G);

	max_flow = FordFulkerson(&G, 0, G.n - 1);
	network_log = graph_network::cut[Menu->i_lang_selected];
	for (int i = 0; i < G.n; i++) {
		if (label[i].dir) {
			network_log += Menu->utf8_decode(Graph_Process->GetVertexList()->at(i).label) + L", ";
			Graph_Process->GetVertexList()->at(i).color = "s";
		}
	}
	network_log.pop_back();
	network_log.pop_back();
	network_log += L"}\n - T = {";
	for (int i = 0; i < G.n; i++) {
		if (!label[i].dir) {
			network_log += Menu->utf8_decode(Graph_Process->GetVertexList()->at(i).label) + L", ";
			Graph_Process->GetVertexList()->at(i).color = "t";
		}
	}
	network_log.pop_back();
	network_log.pop_back();
	network_log += L"}";

	for (int u = 0; u < G.n; u++) {
		for (int v = 0; v < G.n; v++) {
			if (Graph_Algorithms->Graph.A.at(u).at(v) > 0) {
				for (v_Edge::iterator it = Graph_Process->GetEdgeList()->begin(); it != Graph_Process->GetEdgeList()->end(); ++it) {
					if (it->vertex_from == Graph_Process->GetVertexList()->at(u).label && it->vertex_to == Graph_Process->GetVertexList()->at(v).label) {
						it->f = G.F[u][v];
					}
				}
			}
		}
	}

	show_network_log = true;
}