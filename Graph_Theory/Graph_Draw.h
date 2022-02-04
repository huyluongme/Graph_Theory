#pragma once

#include "framework.h"

class cGraph_Draw
{
public:

	cGraph_Draw() {
		circle_radius = 25.0f;
		un_directed = 0;
	}

	float circle_radius;
	int un_directed;

	int check_edge_created(Edge edge_check);
	int check_node_created(string label);
	Edge_id get_id_node_from_label(string label_from, string label_to);
	void AddNode(ImVec2 pos, bool* add_node);
	void AddLabel(ImVec2 pos, ImFont* font, bool* add_node);
	void Get_ID_Node_Selected(ImVec2 origin_pos, ImVec2 mouse_pos, bool* node_selected, int* id_node_selected, bool* move_node, int* rename_del_node, bool* adding_line);
	void Get_ID_Edge_Selected(bool* edge_selected, int* id_edge_selected, bool* move_edge);
	void MoveNode(ImVec2 mouse_pos_in_canvas, int* id_node_selected, bool* move_node, bool* node_selected, int* id_edge_selected, bool* move_edge, bool* edge_selected);
	void RenameNode(int* id_node_selected, ImVec2 pos, ImFont* font, int* rename_del_node, bool* node_selected);
	void MoveEdge(int* id_edge_selected, bool* move_edge, bool* edge_selected, int* id_node_selected, bool* move_node, bool* node_selected);
	void AddLine(ImDrawList* draw_list, ImVec2 origin_pos, ImVec2 centerOxy_pos, ImVec2 mouse_pos, int *id_node_selected, bool* adding_edge, bool* node_selected);
	void DrawNode(ImDrawList* draw_list, ImVec2 origin_pos);
	void DrawLine(ImDrawList* draw_list, ImVec2 origin_pos, ImVec2 centerOxy_pos, ImFont* font);
	void DrawPopupRenameOrDel(int* id_node_selected, ImVec2 origin_pos, int* rename_del_node, bool* node_selected);
	void DeleteNode(int id);
	void DeleteEdge(int id);
	void ReloadEdge(string str_old, string str_new);
};
extern class cGraph_Draw* Graph_Draw;