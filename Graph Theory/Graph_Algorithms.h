#pragma once

#include "framework.h"

#define WHITE 0
#define GRAY 1
#define BLACK 2

#define oo 999999.0f
#define INF 9999999
#define No_EDGE 0


class cGraph_Algorithms
{
public:
	cGraph_Algorithms() {
		calc_deg_log = L"";
		show_deg_log = false;

		bfs_start = dfs_stack_start = dfs_recur_start = false;
		bfs_log = dfs_stack_log = dfs_recur_log = L"";
		show_bfs_log = show_dfs_stack_log = show_dfs_recur_log = false;

		connect_log = L"";
		reset_edge_visited = false;
		show_connect_log = false;

		cycle_log = L"";
		has_circle = false;
		show_cycle_log = false;

		toposort_log = L"";
		show_toposort_log = false;

		rank_log = L"";
		show_rank_log = false;

		dijkstra_log = L"";
		show_dijkstra_log = show_bellmanford_log = false;

		reset_edge_visited_2 = show_kruskal_log = show_prim_log = false;
		total_w_kruskal = total_w_prim = 0.0f;
	}
	sGraphFunction Graph;

	// Var for Calc Deg
	wstring calc_deg_log;
	bool show_deg_log;

	// Var for BFS, DFS
	bool bfs_start, dfs_stack_start, dfs_recur_start;
	wstring bfs_log, dfs_stack_log, dfs_recur_log;
	bool show_bfs_log, show_dfs_stack_log, show_dfs_recur_log;
	Tree BFS_Tree, DFS_Stack_Tree, DFS_Recur_Tree;

	// Var for Connect
	wstring connect_log;
	bool reset_edge_visited;
	bool show_connect_log;

	// Var for Cycle
	wstring cycle_log;
	bool has_circle;
	bool show_cycle_log;

	// Var for Topo Sort
	wstring toposort_log;
	bool show_toposort_log;

	// Var for Rank
	wstring rank_log;
	bool show_rank_log;

	// Var for shortestpath
	wstring dijkstra_log;
	Tree Dijkstra_Tree, BellmanFord_Tree;
	bool show_dijkstra_log, show_bellmanford_log;

	// Var for MST
	Tree Kruskal_Tree, Prim_Tree;
	bool reset_edge_visited_2;
	bool show_kruskal_log, show_prim_log;
	float total_w_kruskal, total_w_prim;

	void InitGraph();
	void CalcDeg();
	void BFS();
	void DFS_Stack();
	void DFS_Recursive(int u, int p, vector<bool>* visited, vector<int>* parent);
	void DFS_Recursive(int *end, int u, vector<bool> *visited);
	void DFS_Recursive_Call();
	void connect();
	void SCC(int u, vector<int>* num, vector<int>* min_num, vector<bool>* on_stack, int* k, list<int>* stack, int* cnt);
	void strongconnect();
	void Cycle(int u, int p, vector<int> *color);
	void Check_Has_Cycle();
	void alltopologicalSortUtil(vector<int>res, vector<bool>* visited, vector<int>* indegree);
	void alltopologicalSort();
	void Rank();
	void Dijkstra(int s);
	void BellmanFord(int s);
	void Kruskal();
	void Prim(int s);
	void ChuLiu(int s);
};

extern cGraph_Algorithms* Graph_Algorithms;

