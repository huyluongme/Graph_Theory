#pragma once

typedef struct {
	string label;
	ImVec2 Pos;
}Node;

extern vector<Node> node;

extern vector<string> node_sorted;

typedef struct {
	bool visited;
	string label_from, label_to;
	ImVec2 pos_mid_ori;
	ImVec2 pos_mid_draw;
	float length;
	int stt;
	float angle_from_to, angle_to_from, angle_from_mouse, angle_to_mouse;
	int count;
}Edge;

typedef struct {
	int id_from, id_to;
}Edge_id;

extern vector<Edge> edge;

typedef struct {
	bool visited;
	string label;
	vector<string> adj;
}SGraph;
extern vector<SGraph> Graph;

extern vector<string> browsing_result;