#pragma once

#include "framework.h"

#define MAXN 100
#define MAXM 500
#define INF 9999999
#define MAXIT 10
typedef struct {
	int u, v;
	float w;
	int link;
}Edge_ChuLiu;

typedef struct {
	int n, m;
	Edge_ChuLiu Edges[MAXM];
}Graph_ChuLiu;

typedef struct {
	int n;
	int parent[MAXN];
	int weight[MAXN];
	int link[MAXN]; 
}Tree_ChuLiu;

class cChuLiu
{
public:

	cChuLiu() {
		show_chuliu_log = false;
		total_w_chuliu = 0.0f;
	}

	sGraphUI ChuLiu_Result;
	bool show_chuliu_log;
	float total_w_chuliu;

	void	Graph_Init(Graph_ChuLiu* G0);
	void	Tree_Init(Tree_ChuLiu* T, int n);
	void	buildH(Graph_ChuLiu* G, int root, Tree_ChuLiu* H);
	int		find_cycles(Tree_ChuLiu* H, int root);
	void	contract(Graph_ChuLiu* G, Tree_ChuLiu* H, int no, Graph_ChuLiu* G1);
	void	expand(Tree_ChuLiu* H, Graph_ChuLiu* G1, Tree_ChuLiu* H1);
	void	ChuLiu_Algorithms();
};

extern cChuLiu* ChuLiu;
