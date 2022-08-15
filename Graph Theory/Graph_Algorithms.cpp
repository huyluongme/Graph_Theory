#include "Graph_Algorithms.h"

cGraph_Algorithms* Graph_Algorithms = new cGraph_Algorithms();

typedef struct {
	int u, p;
}elementtype;

void cGraph_Algorithms::InitGraph() {
	v_Vertex* VertexList = Graph_Process->GetVertexList();
	v_Edge* EdgeList = Graph_Process->GetEdgeList();
	Graph.Vertex.clear();
	Graph.A.clear();
	Graph.W.clear();
	Graph.C.clear();
	Graph.F.clear();
	Graph.Vertex.resize(VertexList->size());
	for (int u = 0; u < Graph.Vertex.size(); u++) {
		vector<int> tmp;
		vector<float> tmp_2;
		for (int v = 0; v < Graph.Vertex.size(); v++) {
			tmp.push_back(0);
			tmp_2.push_back(0);
		}
		Graph.A.push_back(tmp);
		Graph.W.push_back(tmp_2);
		Graph.C.push_back(tmp_2);
		Graph.F.push_back(tmp_2);
	}

	for (int e = 0; e < EdgeList->size(); e++) {
		int id_from = Graph_Process->GetIDVertexFromLabel(Graph_Process->GetVertexList(), EdgeList->at(e).vertex_from);
		int id_to = Graph_Process->GetIDVertexFromLabel(Graph_Process->GetVertexList(), EdgeList->at(e).vertex_to);
		if (id_from != -1 && id_to != -1) {
			Graph.A.at(id_from).at(id_to)++;
			Graph.W.at(id_from).at(id_to) = Graph_Process->GetEdgeList()->at(e).w;
			Graph.C.at(id_from).at(id_to) = Graph_Process->GetEdgeList()->at(e).c;
			Graph.F.at(id_from).at(id_to) = Graph_Process->GetEdgeList()->at(e).f;
			if (Graph_Process->types_of_graph == UNDIRECTED) {
				Graph.A.at(id_to).at(id_from)++;
				Graph.W.at(id_to).at(id_from) = Graph_Process->GetEdgeList()->at(e).w;
				Graph.C.at(id_to).at(id_from) = Graph_Process->GetEdgeList()->at(e).c;
				Graph.F.at(id_to).at(id_from) = Graph_Process->GetEdgeList()->at(e).f;
			}
		}
	}
}

void cGraph_Algorithms::CalcDeg() {
	calc_deg_log.clear();
	if (Graph_Process->types_of_graph == UNDIRECTED) {
		calc_deg_log += calc_deg_log_string::string1[Menu->i_lang_selected];
		for (int u = 0; u < Graph.Vertex.size(); u++) {
			int deg_tmp = 0;
			calc_deg_log += calc_deg_log_string::string2[Menu->i_lang_selected] + wstring(Graph_Process->GetVertexList()->at(u).label.begin(), Graph_Process->GetVertexList()->at(u).label.end()) + L": ";
			for (int v = 0; v < Graph.Vertex.size(); v++) {
				deg_tmp += Graph.A.at(u).at(v);
			}
			calc_deg_log += to_wstring(deg_tmp) + L"\n";
		}
	}
	else {
		calc_deg_log += calc_deg_log_string::string1[Menu->i_lang_selected + 2];;
		for (int u = 0; u < Graph.Vertex.size(); u++) {
			int deg_in_tmp = 0, deg_out_tmp = 0;

			for (int v = 0; v < Graph.Vertex.size(); v++)
				deg_in_tmp += Graph.A.at(v).at(u);

			for (int v = 0; v < Graph.Vertex.size(); v++)
				deg_out_tmp += Graph.A.at(u).at(v);

			calc_deg_log += calc_deg_log_string::string3[Menu->i_lang_selected] + wstring(Graph_Process->GetVertexList()->at(u).label.begin(), Graph_Process->GetVertexList()->at(u).label.end()) + L"\n";
			calc_deg_log += calc_deg_log_string::string4[Menu->i_lang_selected] + to_wstring(deg_in_tmp) + calc_deg_log_string::string4[Menu->i_lang_selected + 2] + to_wstring(deg_out_tmp) + calc_deg_log_string::string4[Menu->i_lang_selected + 4] + to_wstring(deg_in_tmp + deg_out_tmp) + L"\n";
		}
	}

	show_deg_log = true;
}

void cGraph_Algorithms::BFS() {
	bfs_start = true;
	if (Graph_Process->idVertexSelected != -1) {
		BFS_Tree.Vertexs.clear();
		BFS_Tree.Edges.clear();
		bfs_log.clear();
		bfs_log += graph_algo::search_string[Menu->i_lang_selected] + wstring(Graph_Process->GetVertexList()->at(Graph_Process->idVertexSelected).label.begin(), Graph_Process->GetVertexList()->at(Graph_Process->idVertexSelected).label.end()) + L"): ";

		vector<bool> visited;
		vector<int> parent;
		visited.resize(Graph.Vertex.size(), false);
		parent.resize(Graph.Vertex.size(), -1);

		list<elementtype> queue;
		elementtype pair = { Graph_Process->idVertexSelected, -1 };

		queue.push_back(pair);

		BFS_Tree.Vertexs.push_back(Graph_Process->GetVertexList()->at(Graph_Process->idVertexSelected));

		while (!queue.empty()) {
			elementtype pair = queue.front();
			int u = pair.u, p = pair.p;

			queue.pop_front();
			if (visited.at(u))
				continue;
			visited.at(u) = true;
			parent.at(u) = p;
			bfs_log += wstring(Graph_Process->GetVertexList()->at(u).label.begin(), Graph_Process->GetVertexList()->at(u).label.end()) + L" -> ";

			for (int v = 0; v < Graph.Vertex.size(); v++) {
				if (Graph.A.at(u).at(v) > 0) {
					elementtype pair = { v,u };
					queue.push_back(pair);
				}
			}
		}

		for (int u = 0; u < parent.size(); u++)
			if (parent.at(u) != -1) {
				BFS_Tree.Vertexs.push_back(Graph_Process->GetVertexList()->at(u));
				BFS_Tree.Edges.push_back({ Graph_Process->GetVertexList()->at(parent.at(u)).label,Graph_Process->GetVertexList()->at(u).label });
			}

		bfs_log.pop_back();
		bfs_log.pop_back();
		bfs_log.pop_back();
		bfs_log.pop_back();

		Graph_Process->idVertexSelected = -1;
		Graph_Process->bVertexSelected = false;
		show_bfs_log = true;
		bfs_start = false;
	}
}

void cGraph_Algorithms::DFS_Stack() {
	dfs_stack_start = true;
	if (Graph_Process->idVertexSelected != -1) {
		DFS_Stack_Tree.Vertexs.clear();
		DFS_Stack_Tree.Edges.clear();
		dfs_stack_log.clear();
		dfs_stack_log += graph_algo::search_string[Menu->i_lang_selected + 2] + wstring(Graph_Process->GetVertexList()->at(Graph_Process->idVertexSelected).label.begin(), Graph_Process->GetVertexList()->at(Graph_Process->idVertexSelected).label.end()) + L"): ";
		vector<bool> visited;
		vector<int> parent;
		visited.resize(Graph.Vertex.size(), false);
		parent.resize(Graph.Vertex.size(), -1);


		list<elementtype> stack;

		elementtype pair = { Graph_Process->idVertexSelected, -1 };

		stack.push_back(pair);

		DFS_Stack_Tree.Vertexs.push_back(Graph_Process->GetVertexList()->at(pair.u));

		while (!stack.empty()) {
			elementtype pair = stack.back();
			stack.pop_back();

			int u = pair.u, p = pair.p;

			if (visited.at(u))
				continue;

			parent.at(u) = p;

			dfs_stack_log += wstring(Graph_Process->GetVertexList()->at(u).label.begin(), Graph_Process->GetVertexList()->at(u).label.end()) + L" -> ";
			visited.at(u) = true;

			for (int v = 0; v < Graph.Vertex.size(); v++) {
				if (Graph.A.at(u).at(v) > 0) {
					elementtype pair = { v,u };
					stack.push_back(pair);
				}
			}
		}

		for (int u = 0; u < parent.size(); u++)
			if (parent.at(u) != -1) {
				DFS_Stack_Tree.Vertexs.push_back(Graph_Process->GetVertexList()->at(u));
				DFS_Stack_Tree.Edges.push_back({ Graph_Process->GetVertexList()->at(parent.at(u)).label,Graph_Process->GetVertexList()->at(u).label });
			}

		dfs_stack_log.pop_back();
		dfs_stack_log.pop_back();
		dfs_stack_log.pop_back();
		dfs_stack_log.pop_back();
		Graph_Process->idVertexSelected = -1;
		Graph_Process->bVertexSelected = false;
		show_dfs_stack_log = true;
		dfs_stack_start = false;
	}
}

void cGraph_Algorithms::DFS_Recursive(int u, int p, vector<bool>* visited, vector<int>* parent) {
	visited->at(u) = true;
	parent->at(u) = p;

	dfs_recur_log += wstring(Graph_Process->GetVertexList()->at(u).label.begin(), Graph_Process->GetVertexList()->at(u).label.end()) + L" -> ";

	for (int v = 0; v < Graph.Vertex.size(); v++)
		if (Graph.A.at(u).at(v) > 0 && !visited->at(v))
			DFS_Recursive(v, u, visited, parent);
}

void cGraph_Algorithms::DFS_Recursive_Call() {
	dfs_recur_start = true;
	if (Graph_Process->idVertexSelected != -1) {
		DFS_Recur_Tree.Vertexs.clear();
		DFS_Recur_Tree.Edges.clear();
		dfs_recur_log.clear();
		dfs_recur_log += graph_algo::search_string[Menu->i_lang_selected + 4] + wstring(Graph_Process->GetVertexList()->at(Graph_Process->idVertexSelected).label.begin(), Graph_Process->GetVertexList()->at(Graph_Process->idVertexSelected).label.end()) + L"): ";
		vector<bool> visited;
		vector<int> parent;
		visited.resize(Graph.Vertex.size(), false);
		parent.resize(Graph.Vertex.size(), -1);

		DFS_Recur_Tree.Vertexs.push_back(Graph_Process->GetVertexList()->at(Graph_Process->idVertexSelected));

		DFS_Recursive(Graph_Process->idVertexSelected, -1, &visited, &parent);

		for (int u = 0; u < parent.size(); u++)
			if (parent.at(u) != -1) {
				DFS_Recur_Tree.Vertexs.push_back(Graph_Process->GetVertexList()->at(u));
				DFS_Recur_Tree.Edges.push_back({ Graph_Process->GetVertexList()->at(parent.at(u)).label,Graph_Process->GetVertexList()->at(u).label });
			}

		dfs_recur_log.pop_back();
		dfs_recur_log.pop_back();
		dfs_recur_log.pop_back();
		dfs_recur_log.pop_back();
		Graph_Process->idVertexSelected = -1;
		Graph_Process->bVertexSelected = false;
		show_dfs_recur_log = true;
		dfs_recur_start = false;
	}
}

void cGraph_Algorithms::DFS_Recursive(int* end, int u, vector<bool>* visited) {
	visited->at(u) = true;
	Graph_Process->GetVertexList()->at(u).visited = true;
	for (int v = 0; v < Graph.Vertex.size(); v++)
		if (Graph.A.at(u).at(v) > 0 && !visited->at(v)) {
			int id = Graph_Process->GetIDEdgeForConnect(u, v);
			if (id != -1)
				Graph_Process->GetEdgeList()->at(id).visited = true;
			*end = v;
			DFS_Recursive(end, v, visited);
		}
}

void cGraph_Algorithms::connect() {
	if (Graph.Vertex.size() == 0) return;
	connect_log.clear();
	vector<bool> visited;
	int cnt = 0;
	int end = -1;
	visited.resize(Graph.Vertex.size(), false);

	for (int u = 0; u < Graph.Vertex.size(); u++) {
		if (!visited.at(u)) {
			cnt++;
			DFS_Recursive(&end, u, &visited);
			int id = Graph_Process->GetIDEdgeForConnect(u, end);
			if (id != -1) {
				Graph_Process->GetEdgeList()->at(id).visited = true;
				Graph_Process->GetVertexList()->at(u).visited = true;
				Graph_Process->GetVertexList()->at(end).visited = true;
			}
		}
	}

	if (cnt == 1) {
		connect_log = graph_algo::connect_string[Menu->i_lang_selected];
		for (int e = 0; e < Graph_Process->GetEdgeList()->size(); e++)
			Graph_Process->GetEdgeList()->at(e).visited = true;
	}
	else connect_log = graph_algo::connect_string[Menu->i_lang_selected + 2] + to_wstring(cnt) + graph_algo::connect_string[Menu->i_lang_selected + 4];
	reset_edge_visited = true;
	show_connect_log = true;
}

void cGraph_Algorithms::SCC(int u, vector<int>* num, vector<int>* min_num, vector<bool>* on_stack, int* k, list<int>* stack, int* cnt) {
	num->at(u) = min_num->at(u) = *k;
	*k += 1;
	stack->push_back(u);
	on_stack->at(u) = true;
	Graph_Process->GetVertexList()->at(u).visited = true;
	for (int v = 0; v < Graph.Vertex.size(); v++) {
		if (Graph.A.at(u).at(v) > 0) {
			if (num->at(v) < 0) {
				SCC(v, num, min_num, on_stack, k, stack, cnt);
				min_num->at(u) = min(min_num->at(u), min_num->at(v));
			}
			else if (on_stack->at(v))
				min_num->at(u) = min(min_num->at(u), num->at(v));
		}
	}

	if (num->at(u) == min_num->at(u)) {
		*cnt += 1;
		int w;
		vector<int> tmp;
		do {
			w = stack->back();

			tmp.push_back(w);
			stack->pop_back();
			on_stack->at(w) = false;
		} while (w != u);
		for (int i = 0; i < tmp.size(); i++) {
			for (int j = 0; j < tmp.size(); j++) {
				int id = Graph_Process->GetIDEdgeForConnect(tmp.at(i), tmp.at(j));
				int id_2 = Graph_Process->GetIDEdgeForConnect(tmp.at(j), tmp.at(i));
				if (id != -1) {
					Graph_Process->GetEdgeList()->at(id).visited = true;
					Graph_Process->GetVertexList()->at(tmp.at(i)).visited = true;
					Graph_Process->GetVertexList()->at(tmp.at(j)).visited = true;
				}
				if (id_2 != -1) {
					Graph_Process->GetEdgeList()->at(id_2).visited = true;
					Graph_Process->GetVertexList()->at(tmp.at(i)).visited = true;
					Graph_Process->GetVertexList()->at(tmp.at(j)).visited = true;
				}
			}
		}
	}
}

void cGraph_Algorithms::strongconnect() {
	int k = 1, cnt = 0;
	vector<int> num, min_num;
	vector<bool> on_stack;
	list<int> stack;
	num.resize(Graph.Vertex.size(), -1);
	min_num.resize(Graph.Vertex.size(), -1);
	on_stack.resize(Graph.Vertex.size(), false);

	for (int u = 0; u < Graph.Vertex.size(); u++)
		if (num.at(u) == -1)
			SCC(u, &num, &min_num, &on_stack, &k, &stack, &cnt);

	if (cnt == 1) connect_log = graph_algo::strong_connect_string[Menu->i_lang_selected];
	else connect_log = graph_algo::strong_connect_string[Menu->i_lang_selected + 2] + to_wstring(cnt) + graph_algo::strong_connect_string[Menu->i_lang_selected + 4];
	reset_edge_visited = true;
	show_connect_log = true;
}

void cGraph_Algorithms::Cycle(int u, int p, vector<int>* color) {
	color->at(u) = GRAY;
	for (int v = 0; v < Graph.Vertex.size(); v++) {
		if (Graph.A[u][v] > 0) {
			if (Graph_Process->types_of_graph == UNDIRECTED)
				if (v == p) continue;
			if (color->at(v) == WHITE)
				Cycle(v, u, color);
			else if (color->at(v) == GRAY) {
				cycle_log = graph_algo::cycle_string[Menu->i_lang_selected];
				has_circle = true;
				return;
			}
		}
	}
	color->at(u) = BLACK;
}

void cGraph_Algorithms::Check_Has_Cycle() {
	vector<int> color;
	color.resize(Graph.Vertex.size(), WHITE);
	cycle_log.clear();
	for (int u = 0; u < Graph.Vertex.size(); u++) {
		if (color.at(u) == WHITE)
			Cycle(u, -1, &color);
	}
	if (cycle_log.empty()) { cycle_log = graph_algo::cycle_string[Menu->i_lang_selected + 2]; has_circle = false; };
	show_cycle_log = true;
}

void cGraph_Algorithms::alltopologicalSortUtil(vector<int>res, vector<bool>* visited, vector<int>* indegree) {
	bool flag = false;
	for (int u = 0; u < Graph.Vertex.size(); u++) {
		if (indegree->at(u) == 0 && !visited->at(u)) {
			for (int v = 0; v < Graph.Vertex.size(); v++)
				if (Graph.A[u][v] > 0)
					indegree->at(v)--;

			res.push_back(u);
			visited->at(u) = true;
			alltopologicalSortUtil(res, visited, indegree);

			visited->at(u) = false;
			res.erase(res.end() - 1);
			for (int v = 0; v < Graph.Vertex.size(); v++)
				if (Graph.A[u][v] > 0)
					indegree->at(v)++;

			flag = true;
		}
	}
	if (!flag) {
		toposort_log += L"  - ";
		for (int i = 0; i < res.size(); i++) {
			toposort_log += Menu->utf8_decode(Graph_Process->GetVertexList()->at(res.at(i)).label);
			if (i < res.size() - 1) toposort_log += L", ";
		}
		toposort_log += L"\n";
	}
}

void cGraph_Algorithms::alltopologicalSort() {
	vector<bool> visited;
	visited.resize(Graph.Vertex.size(), false);
	vector<int> res;
	vector<int> indegree;
	indegree.resize(Graph.Vertex.size(), 0);
	for (int u = 0; u < Graph.Vertex.size(); u++)
		for (int v = 0; v < Graph.Vertex.size(); v++) {
			indegree.at(u) += Graph.A.at(v).at(u);
		}
	toposort_log.clear();
	toposort_log = graph_algo::topo_string[Menu->i_lang_selected];
	alltopologicalSortUtil(res, &visited, &indegree);
	show_toposort_log = true;
}

void copy_list(list<int>* S1, list<int>* S2) {
	S1->clear();
	for (auto it = S2->begin(); it != S2->end(); ++it)
		S1->push_back(*it);
}

void cGraph_Algorithms::Rank() {
	vector<int> rank;
	rank.resize(Graph.Vertex.size());
	vector<int> indegree;
	indegree.resize(Graph.Vertex.size(), 0);

	for (int u = 0; u < Graph.Vertex.size(); u++)
		for (int v = 0; v < Graph.Vertex.size(); v++) {
			indegree.at(u) += Graph.A.at(v).at(u);
		}

	list<int> S1, S2;

	for (int u = 0; u < Graph.Vertex.size(); u++)
		if (indegree.at(u) == 0)
			S1.push_back(u);

	int k = 0;
	while (!S1.empty()) {
		S2.clear();
		for (auto it = S1.begin(); it != S1.end(); ++it) {
			int u = *it;
			rank.at(u) = k;

			for (int v = 0; v < Graph.Vertex.size(); v++) {
				if (Graph.A.at(u)[v] > 0) {
					indegree.at(v)--;
					if (indegree.at(v) == 0)
						S2.push_back(v);
				}
			}
		}
		copy_list(&S1, &S2);
		k++;
	}

	rank_log.clear();
	rank_log = graph_algo::rank_string[Menu->i_lang_selected];
	for (int u = 0; u < Graph.Vertex.size(); u++) {
		rank_log += L"  - rank(" + Menu->utf8_decode(Graph_Process->GetVertexList()->at(u).label) + L"): " + to_wstring(rank.at(u)) + L"\n";
	}
	show_rank_log = true;
}

void cGraph_Algorithms::Dijkstra(int s) {
	vector<bool> visited;
	visited.resize(Graph.Vertex.size(), false);
	vector<int> p;
	vector <float> pi;
	pi.resize(Graph.Vertex.size(), oo);
	p.resize(Graph.Vertex.size(), -1);
	pi.at(s) = 0;
	p.at(s) = -1;

	int u;
	for (int i = 1; i < Graph.Vertex.size(); i++) {
		float min_pi = oo;
		for (int v = 0; v < Graph.Vertex.size(); v++) {
			if (!visited.at(v) && pi.at(v) < min_pi) {
				min_pi = pi.at(v);
				u = v;
			}
		}
		visited.at(u) = true;
		for (int v = 0; v < Graph.Vertex.size(); v++) {
			if (!visited.at(v) && Graph.A.at(u).at(v) > 0) {
				if (pi.at(u) + Graph.W.at(u).at(v) < pi.at(v)) {
					pi.at(v) = pi.at(u) + Graph.W.at(u).at(v);
					p.at(v) = u;
				}
			}
		}
	}

	Dijkstra_Tree.Vertexs.clear();
	Dijkstra_Tree.Edges.clear();
	for (auto it = Graph_Process->GetVertexList()->begin(); it != Graph_Process->GetVertexList()->end(); ++it)
		Dijkstra_Tree.Vertexs.push_back(*it);

	for (int u = 0; u < Graph_Process->GetVertexList()->size(); u++) {
		if (p.at(u) != -1) {
			Dijkstra_Tree.Edges.push_back({ Graph_Process->GetVertexList()->at(p.at(u)).label,Graph_Process->GetVertexList()->at(u).label, Graph.W.at(p.at(u)).at(u) });
		}
	}

}

void cGraph_Algorithms::BellmanFord(int s) {
	int u, v, w, it, k;

	vector<int> p;
	vector <float> pi;
	pi.resize(Graph.Vertex.size(), oo);
	p.resize(Graph.Vertex.size(), -1);
	pi.at(s) = 0;
	p.at(s) = -1;

	for (it = 1; it < Graph.Vertex.size(); it++) {

		for (int u = 0; u < Graph.Vertex.size(); u++) {
			for (int v = 0; v < Graph.Vertex.size(); v++) {
				if (Graph.A.at(u).at(v) > 0) {
					int w = Graph.W.at(u).at(v);
					if (pi.at(u) == oo) continue;

					if (pi.at(u) + w < pi.at(v)) {
						pi.at(v) = pi.at(u) + w;
						p.at(v) = u;
					}
				}
			}
		}

	}

	BellmanFord_Tree.Vertexs.clear();
	BellmanFord_Tree.Edges.clear();
	for (auto it = Graph_Process->GetVertexList()->begin(); it != Graph_Process->GetVertexList()->end(); ++it)
		BellmanFord_Tree.Vertexs.push_back(*it);

	for (int u = 0; u < Graph_Process->GetVertexList()->size(); u++) {
		if (p.at(u) != -1) {
			BellmanFord_Tree.Edges.push_back({ Graph_Process->GetVertexList()->at(p.at(u)).label,Graph_Process->GetVertexList()->at(u).label, Graph.W.at(p.at(u)).at(u) });
		}
	}
}

bool compareByW(const Edge_GEL& a, const Edge_GEL& b)
{
	return a.w < b.w;
}

int findroot(vector<int>p, int u) {
	if (u == p[u]) {
		return u;
	}
	else {
		return findroot(p, p[u]);
	}
}
void cGraph_Algorithms::Kruskal() {
	Graph_EL graph_tmp;
	graph_tmp.n = Graph.Vertex.size();
	for (int u = 0; u < Graph.Vertex.size(); u++) {
		for (int v = u; v < Graph.Vertex.size(); v++) {
			if (Graph.A.at(u).at(v) > 0) {
				float w = Graph.W.at(u).at(v);
				graph_tmp.Edges.push_back({ u,v,w });
			}
		}
	}
	sort(graph_tmp.Edges.begin(), graph_tmp.Edges.end(), compareByW);
	
	vector<int> p;
	for (int u = 0; u < Graph.Vertex.size(); u++)
		p.push_back(u);
	
	Kruskal_Tree.Vertexs.clear();
	Kruskal_Tree.Edges.clear();
	total_w_kruskal = 0.0f;
	for (int e = 0; e < graph_tmp.Edges.size(); e++) {
		int u = graph_tmp.Edges.at(e).u;
		int v = graph_tmp.Edges.at(e).v;
		float w = graph_tmp.Edges.at(e).w;
		int root_u = findroot(p, u);
		int root_v = findroot(p, v);
		if (root_u != root_v) {
			int id = Graph_Process->GetIDEdgeForConnect(u, v);
			if (id != -1) {
				Kruskal_Tree.Edges.push_back({ Graph_Process->GetVertexList()->at(u).label,Graph_Process->GetVertexList()->at(v).label,  w });
				total_w_kruskal += w;
			}
			p[root_v] = root_u;
		}
	}

	for (auto it = Graph_Process->GetVertexList()->begin(); it != Graph_Process->GetVertexList()->end(); ++it)
		Kruskal_Tree.Vertexs.push_back(*it);
	show_kruskal_log = true;
}

void cGraph_Algorithms::Prim(int s) {
	vector<int> p;
	vector <float> pi;
	vector<bool> visited;
	pi.resize(Graph.Vertex.size(), oo);
	visited.resize(Graph.Vertex.size(), false);
	p.resize(Graph.Vertex.size(), -1);
	pi.at(s) = 0;
	p.at(s) = -1;

	int u, v;
	for (int i = 1; i < Graph.Vertex.size(); i++) {
		int min_pi = oo;
		for (int j = 0; j < Graph.Vertex.size(); j++) {
			if (!visited.at(j) && pi[j] < min_pi) {
				min_pi = pi[j];
				u = j;
			}
		}
		visited.at(u) = true;
		for (v = 0; v < Graph.Vertex.size(); v++) {
			if (Graph.A[u][v] > 0 && !visited.at(v))
				if (Graph.W[u][v] < pi[v]) {
					pi[v] = Graph.W[u][v];
					p[v] = u;
				}
		}
	}
	Prim_Tree.Vertexs.clear();
	Prim_Tree.Edges.clear();
	for (auto it = Graph_Process->GetVertexList()->begin(); it != Graph_Process->GetVertexList()->end(); ++it)
		Prim_Tree.Vertexs.push_back(*it);
	
	total_w_prim = 0.0f;

	for (int u = 0; u < Graph_Process->GetVertexList()->size(); u++) {
		if (p.at(u) != -1) {
			Prim_Tree.Edges.push_back({ Graph_Process->GetVertexList()->at(p.at(u)).label,Graph_Process->GetVertexList()->at(u).label, Graph.W.at(p.at(u)).at(u) });
			total_w_prim += Graph.W.at(p.at(u)).at(u);
		}
	}
}
