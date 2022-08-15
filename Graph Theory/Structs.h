#pragma once


// typedef struct for draw graph
typedef struct {
	string label;
	ImVec2 Pos;
	string color;
	bool visited;
}Vertex;

typedef struct {
	string vertex_from, vertex_to;
	int count;
	float length;
	ImVec2 p1, p2, p3, w_pos, w_pos_rect_p1, w_pos_rect_p2;
	float angle_from_to;
	float w;
	float f, c;
	bool visited;
}Edge;

typedef vector<Vertex> v_Vertex;
typedef vector<Edge> v_Edge;

typedef struct {
	v_Vertex Vertexs;
	v_Edge Edges;
}sGraphUI;

typedef struct {
	vector<int> Vertex;
	vector<vector<int>> A;
	vector<vector<float>> W;
	vector<vector<float>> C;
	vector<vector<float>> F;
}sGraphFunction;

typedef struct {
	string vertex_from, vertex_to;
	float W;
}EdgeTree;

typedef vector<EdgeTree> v_EdgeTree;

typedef struct {
	v_Vertex Vertexs;
	v_EdgeTree Edges;
}Tree;

typedef struct {
	int u, v;
	float w;
	int link;
}Edge_GEL;

typedef vector<Edge_GEL> v_Edge_GEL;

typedef struct {
	int n;
	v_Edge_GEL Edges;
}Graph_EL;

//
//typedef struct {
//	string vertex_from, vertex_to;
//}Edge_for_Tree;
//
//typedef struct {
//	string vertex_from, vertex_to;
//	float W;
//	int link;
//}Edge_for_Tree_STP;
//
//typedef vector<Edge_for_Tree> v_EFT;
//typedef vector<Edge_for_Tree_STP> v_EFT_STP;
//
//typedef struct {
//	v_V Vertexs;
//	v_EFT Edges;
//}Tree;
//
//typedef struct {
//	int n;
//	vector<int> p;
//	vector<float> w;
//	vector<int> link;
//}Tree_2;
//
//typedef struct {
//	v_V Vertexs;
//	v_EFT_STP Edges;
//}Tree_STP;
//
//typedef struct {
//	vector<int> vertex;
//	vector<vector<int>> A;
//	vector<vector<float>> W;
//}sGraph_Function;
//
//typedef struct {
//	int u, v;
//	float w;
//	int link;
//}Edge_L;
//
//typedef struct {
//	int n;
//	vector<Edge_L> edges;
//}Graph_L;