#pragma once

#include "framework.h"

#define UNDIRECTED 0
#define DIRECTED   1

class cGraph_Process
{
public:
	cGraph_Process() {
		bAddingVertex = bAddingEdge = bChangingVertexLabel = bChangingEdgeW = false;
		bMovingVertex = bMovingEdge = bDeleteVertex = bDeleteEdge = false;
		bVertexSelected = bEdgeSelected = false;
		has_w = network = false;
		idVertexSelected = idEdgeSelected = -1;
		types_of_graph = UNDIRECTED;
	}

	bool bAddingVertex, bAddingEdge;
	bool bChangingVertexLabel, bChangingEdgeW;
	bool bMovingVertex, bMovingEdge, bDeleteVertex, bDeleteEdge;
	bool bVertexSelected, bEdgeSelected;
	bool has_w, network;
	int idVertexSelected, idEdgeSelected;
	int types_of_graph;

	v_Vertex*	GetVertexList();
	v_Edge*		GetEdgeList();
	bool		CheckMousePosInCanvas(ImVec2 V_Pos, ImVec2 canvas_p0, ImVec2 canvas_p1);
	bool		CheckVertexExists(string label);
	int			CheckEdgeExists(Edge edge);
	bool		CheckMousePosIsVertexPos(ImVec2 origin_pos, ImVec2 mouse_pos);
	bool		CheckGraphHasNegativeW();
	void		ProcessGraph();

	void		GetIDVertexSelecting(ImVec2 origin_pos, ImVec2 mouse_pos, bool mouse_r_clicked = false);
	void		GetIDEdgeSelecting(ImVec2 mouse_pos, bool mouse_r_clicked = false);
	void		GetID_W_EdgeSelecting(ImVec2 mouse_pos);
	int			GetIDVertexHover(ImVec2 origin_pos, ImVec2 mouse_pos);
	int			GetIDVertexFromLabel(v_Vertex* VertexList, string label);
	int			GetIDEdgeForConnect(int u, int v);
	
	void		AddVertex(ImVec2 V_Pos, ImVec2 canvas_p0, ImVec2 canvas_p1);
	void		AddVertexLabel(ImVec2 origin_pos);
	void		ChangeVertexLabel(ImVec2 origin_pos);
	void		MoveVertex(ImVec2 pos_new);
	void		DeleteVertex();
	void		AddEdge(ImDrawList* draw_list, ImVec2 origin_pos, ImVec2 centerOxy_pos, ImVec2 mouse_pos);
	void		ChangeEdgeW(ImVec2 origin_pos);
	void		ChangeEdgeF(ImVec2 origin_pos);
	void		ReloadVertexLabelInEdge(string old_vertex_label, string new_vertex_label);
	void		MoveEdge(ImVec2 mouse_pos, ImVec2 origin_pos, ImVec2 centerOxy_pos);
	void		DeleteEdge();
private:
	sGraphUI GraphUI;
};

extern cGraph_Process* Graph_Process;