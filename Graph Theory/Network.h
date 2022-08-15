#pragma once

#include "framework.h"

#define MAX_N 100
#define INF 999999

typedef struct {
	float C[MAX_N][MAX_N];
	float F[MAX_N][MAX_N];
	int n;
}Graph_Network;

typedef struct {
	int dir;
	int p;
	int sigma;
}Label;

class cNetwork
{
public:
	cNetwork() {
		max_flow = 0.0f;
		network_log = L"";
		show_network_log = false;
	}
	float max_flow;
	wstring network_log;
	bool show_network_log;

	void	Graph_Network_Init(Graph_Network *G);
	void	init_flow(Graph_Network* pG);
	float	FordFulkerson(Graph_Network* pG, int s, int t);
	void	Network_Process();
};

extern cNetwork* Network;

