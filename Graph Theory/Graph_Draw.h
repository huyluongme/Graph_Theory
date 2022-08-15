#pragma once

#include "framework.h"

class cGraph_Draw
{
public:
	cGraph_Draw() {
		circle_radius = 25.0f;
		graph_col = IM_COL32(255, 255, 255, 255);
		header_text_col = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
		rotation_start_index = -1;
	}
	float circle_radius;
	ImU32 graph_col;
	ImVec4 header_text_col;
	int rotation_start_index;

	void	ImRotateStart();
	ImVec2	ImRotationCenter();
	void	ImRotateEnd(float rad, ImVec2 center);
	
	void	DrawVertex(ImDrawList* draw_list, v_Vertex* VertexList, ImVec2 origin_pos);
	void	DrawEdge(ImDrawList* draw_list, v_Vertex* VertexList, v_Edge* EdgeList, ImVec2 origin_pos, ImVec2 centerOxy_pos);
	void	DrawEdge(ImDrawList* draw_list, v_Vertex* VertexList, v_EdgeTree* EdgeList, ImVec2 origin_pos, ImVec2 centerOxy_pos);
	void	DrawEdge2(ImDrawList* draw_list, v_Vertex* VertexList, v_EdgeTree* EdgeList, ImVec2 origin_pos, ImVec2 centerOxy_pos, bool arrow = false);
private:
};

extern cGraph_Draw* Graph_Draw;