#include "ChuLiu.h"

cChuLiu* ChuLiu = new cChuLiu();

void cChuLiu::Graph_Init(Graph_ChuLiu* G0) {
	G0->n = Graph_Algorithms->Graph.Vertex.size();
	G0->m = 0;
	for (int u = 0; u < Graph_Algorithms->Graph.Vertex.size(); u++) {
		for (int v = 0; v < Graph_Algorithms->Graph.Vertex.size(); v++) {
			if (Graph_Algorithms->Graph.A.at(u).at(v) > 0) {
				G0->Edges[G0->m].u = u;
				G0->Edges[G0->m].v = v;
				G0->Edges[G0->m].w = Graph_Algorithms->Graph.W.at(u).at(v);
				G0->Edges[G0->m].link = -1;
				G0->m++;
			}
		}
	}
}

void cChuLiu::Tree_Init(Tree_ChuLiu* T, int n) {
	T->n = n;
	for (int i = 0; i < n; i++) {
		T->parent[i] = -1;
		T->weight[i] = INF;
		T->link[i] = -1;
	}
}

void cChuLiu::buildH(Graph_ChuLiu* G, int root, Tree_ChuLiu* H) {
	Tree_Init(H, G->n);

	for (int e = 0; e < G->m; e++) {
		int u = G->Edges[e].u;
		int v = G->Edges[e].v;
		int w = G->Edges[e].w;
		int link = G->Edges[e].link;
		if (w < H->weight[v]) {
			H->parent[v] = u;
			H->weight[v] = w;
			H->link[v] = link;
		}
	}
	H->parent[root] = -1;
	H->weight[root] = 0;
}

int id[MAXN];

int cChuLiu::find_cycles(Tree_ChuLiu* H, int root) {
	int i, u, no = 0;
	int color[MAXN];
	for (i = 0; i < H->n; i++) {
		id[i] = -1;
		color[i] = -1;
	}
	for (i = 0; i < H->n; i++) {
		int u = i;
		while (u != root && id[u] == -1 && color[u] != i) {
			color[u] = i;
			u = H->parent[u];
		}
		if (color[u] == i) {
			no++;
			int v = H->parent[u];
			while (v != u) {
				id[v] = no;
				v = H->parent[v];
			}
			id[u] = no; 
		}
	}
	return no;
}

void cChuLiu::contract(Graph_ChuLiu* G, Tree_ChuLiu* H, int no, Graph_ChuLiu* G1) {
	G1->n = no;
	G1->m = 0;

	for (int e = 0; e < G->m; e++) {
		int u = G->Edges[e].u;
		int v = G->Edges[e].v;
		int w = G->Edges[e].w;
		if (id[u] != id[v]) {
			G1->Edges[G1->m].u = id[u];
			G1->Edges[G1->m].v = id[v];
			G1->Edges[G1->m].w = w - H->weight[v];
			G1->Edges[G1->m].link = e;
			G1->m++;
		}
	}
}

void cChuLiu::expand(Tree_ChuLiu* H, Graph_ChuLiu* G1, Tree_ChuLiu* H1) {
	for (int i = 0; i < H->n; i++) {
		if (H->parent[i] != -1) {
			Edge_ChuLiu pe = G1->Edges[H->link[i]];

			H1->parent[pe.v] = pe.u;
			H1->weight[pe.v] += H->weight[i];
			H1->link[pe.v] = pe.link;
		}
	}
}

void cChuLiu::ChuLiu_Algorithms() {
	Graph_ChuLiu G0;
	Graph_Init(&G0);

	Graph_ChuLiu G[MAXIT];
	Tree_ChuLiu H[MAXIT];

	int t = 0;
	int root = 0;
	G[0] = G0;

	while (1) {

		buildH(&G[t], root, &H[t]);
		int no = find_cycles(&H[t], root);
		if (no == 0) break;
		for (int i = 0; i < H[t].n; i++) {
			if (id[i] == -1)
				id[i] = ++no;
		}
		contract(&G[t], &H[t], no, &G[t + 1]);
		root = id[root];
		t++;
	}

	int k;
	for (k = t; k > 0; k--)
		expand(&H[k], &G[k - 1], &H[k - 1]);
	Tree_ChuLiu T = H[0];

	ChuLiu_Result.Vertexs.clear();
	ChuLiu_Result.Vertexs = *Graph_Process->GetVertexList();
	ChuLiu_Result.Edges.clear();
	total_w_chuliu = 0.0f;
	for (int i = 0; i < T.n; i++) {
		if (T.parent[i] != -1) {
			for (v_Edge::iterator it = Graph_Process->GetEdgeList()->begin(); it != Graph_Process->GetEdgeList()->end(); ++it) {
				if (it->vertex_from == Graph_Process->GetVertexList()->at(T.parent[i]).label && it->vertex_to == Graph_Process->GetVertexList()->at(i).label) {
					ChuLiu_Result.Edges.push_back(*it);
					total_w_chuliu += it->w;
					break;
				}
			}
		}
	}
	show_chuliu_log = true;
}