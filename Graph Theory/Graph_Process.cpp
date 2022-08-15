#include "Graph_Process.h"

#pragma warning(disable:4996)

cGraph_Process* Graph_Process = new cGraph_Process();

v_Vertex* cGraph_Process::GetVertexList() {
	return &GraphUI.Vertexs;
}

v_Edge* cGraph_Process::GetEdgeList() {
	return &GraphUI.Edges;
}

bool cGraph_Process::CheckMousePosInCanvas(ImVec2 V_Pos, ImVec2 canvas_p0, ImVec2 canvas_p1) {
	return V_Pos.x >= canvas_p0.x && V_Pos.x <= canvas_p1.x && V_Pos.y >= canvas_p0.y && V_Pos.y <= canvas_p1.y;
}

bool cGraph_Process::CheckVertexExists(string label) {
	for (v_Vertex::iterator it = GetVertexList()->begin(); it != GetVertexList()->end(); ++it)
		if (it->label == label) return true;
	return false;
}

int cGraph_Process::CheckEdgeExists(Edge edge) {
	for (int e = GetEdgeList()->size() - 1; e >= 0; e--)
		if (edge.vertex_from == GetEdgeList()->at(e).vertex_from && edge.vertex_to == GetEdgeList()->at(e).vertex_to)
			return e;
	return -1;
}

bool cGraph_Process::CheckMousePosIsVertexPos(ImVec2 origin_pos, ImVec2 mouse_pos) {
	for (int u = 0; u < GetVertexList()->size(); u++) {
		float distance = sqrt(pow(GetVertexList()->at(u).Pos.x + origin_pos.x - mouse_pos.x, 2) + pow(GetVertexList()->at(u).Pos.y + origin_pos.y - mouse_pos.y, 2));
		if (distance <= Graph_Draw->circle_radius) {
			idVertexSelected = u;
			return true;
		}
	}
	idVertexSelected = -1;
	return false;
}

bool cGraph_Process::CheckGraphHasNegativeW() {
	for (v_Edge::iterator it = GetEdgeList()->begin(); it != GetEdgeList()->end(); ++it)
		if (it->w < 0.0f) return true;
	return false;
}

bool compareByString(const Vertex& a, const Vertex& b) {
	return a.label < b.label;
}

void cGraph_Process::ProcessGraph() {
	sort(GetVertexList()->begin(), GetVertexList()->end(), compareByString);
	Graph_Algorithms->InitGraph();
}

void cGraph_Process::GetIDVertexSelecting(ImVec2 origin_pos, ImVec2 mouse_pos, bool mouse_r_clicked) {
	if (mouse_r_clicked) {
		for (int u = 0; u < GetVertexList()->size(); u++) {
			float distance = sqrt(pow(GetVertexList()->at(u).Pos.x + origin_pos.x - mouse_pos.x, 2) + pow(GetVertexList()->at(u).Pos.y + origin_pos.y - mouse_pos.y, 2));
			if (distance <= Graph_Draw->circle_radius) {
				idVertexSelected = u;
				bDeleteVertex = true;
				bVertexSelected = true;
				break;
			}
		}
	}
	else {
		if (!Graph_Algorithms->bfs_start && !Graph_Algorithms->dfs_stack_start && !Graph_Algorithms->dfs_recur_start && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
			for (int u = 0; u < GetVertexList()->size(); u++) {
				float distance = sqrt(pow(GetVertexList()->at(u).Pos.x + origin_pos.x - mouse_pos.x, 2) + pow(GetVertexList()->at(u).Pos.y + origin_pos.y - mouse_pos.y, 2));
				if (distance <= Graph_Draw->circle_radius) {
					idVertexSelected = u;
					bVertexSelected = true;

					if (!ImGui::IsKeyDown(ImGuiKey_LeftShift))
						bMovingVertex = true;
					else
						bAddingEdge = true;

					break;
				}
			}
		}
		else if ((Graph_Algorithms->bfs_start || Graph_Algorithms->dfs_stack_start || Graph_Algorithms->dfs_recur_start) && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
			for (int u = 0; u < GetVertexList()->size(); u++) {
				float distance = sqrt(pow(GetVertexList()->at(u).Pos.x + origin_pos.x - mouse_pos.x, 2) + pow(GetVertexList()->at(u).Pos.y + origin_pos.y - mouse_pos.y, 2));
				if (distance <= Graph_Draw->circle_radius) {
					idVertexSelected = u;
					bVertexSelected = true;
					break;
				}
			}
		}
	}
}

void cGraph_Process::GetIDEdgeSelecting(ImVec2 mouse_pos, bool mouse_r_clicked) {
	if (mouse_r_clicked) {
		for (int e = 0; e < GetEdgeList()->size(); e++) {
			bool is_segment_hovered = false;
			ImVec2 mouse_pos_projected_on_segment = ImBezierQuadraticClosestPoint(GetEdgeList()->at(e).p1, GetEdgeList()->at(e).p2, GetEdgeList()->at(e).p3, mouse_pos, 100);
			ImVec2 mouse_pos_delta_to_segment = ImVec2(mouse_pos_projected_on_segment.x - mouse_pos.x, mouse_pos_projected_on_segment.y - mouse_pos.y);
			if (mouse_pos_delta_to_segment.y < 0) {
				is_segment_hovered = (ImLengthSqr(mouse_pos_delta_to_segment) <= 10.0f);
			}
			else
				is_segment_hovered = (ImLengthSqr(mouse_pos_delta_to_segment) <= 30.0f);
			if (is_segment_hovered) {
				idEdgeSelected = e;
				bDeleteEdge = true;
				bEdgeSelected = true;
				break;

			}
			else idEdgeSelected = -1;
		}
	}
	else {
		if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
			for (int e = 0; e < GetEdgeList()->size(); e++) {
				bool is_segment_hovered = false;
				ImVec2 mouse_pos_projected_on_segment = ImBezierQuadraticClosestPoint(GetEdgeList()->at(e).p1, GetEdgeList()->at(e).p2, GetEdgeList()->at(e).p3, mouse_pos, 100);
				ImVec2 mouse_pos_delta_to_segment = ImVec2(mouse_pos_projected_on_segment.x - mouse_pos.x, mouse_pos_projected_on_segment.y - mouse_pos.y);
				if (mouse_pos_delta_to_segment.y < 0) {
					is_segment_hovered = (ImLengthSqr(mouse_pos_delta_to_segment) <= 10.0f);
				}
				else
					is_segment_hovered = (ImLengthSqr(mouse_pos_delta_to_segment) <= 30.0f);
				if (is_segment_hovered) {
					idEdgeSelected = e;
					if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
						bMovingEdge = true;
						bEdgeSelected = true;
						break;
					}
				}
				else idEdgeSelected = -1;
			}
		}
	}
}

void cGraph_Process::GetID_W_EdgeSelecting(ImVec2 mouse_pos) {
	for (int e = 0; e < GetEdgeList()->size(); ++e) {
		if (mouse_pos.x >= GetEdgeList()->at(e).w_pos_rect_p1.x && mouse_pos.x <= GetEdgeList()->at(e).w_pos_rect_p2.x
			&& mouse_pos.y >= GetEdgeList()->at(e).w_pos_rect_p1.y && mouse_pos.y <= GetEdgeList()->at(e).w_pos_rect_p2.y) {
			idEdgeSelected = e;
			bChangingEdgeW = true;
			bEdgeSelected = true;
			break;
		}
	}
}

int cGraph_Process::GetIDVertexHover(ImVec2 origin_pos, ImVec2 mouse_pos) {
	for (int u = 0; u < GetVertexList()->size(); u++) {
		if (u != idVertexSelected) {
			float distance = sqrt(pow(GetVertexList()->at(u).Pos.x + origin_pos.x - mouse_pos.x, 2) + pow(GetVertexList()->at(u).Pos.y + origin_pos.y - mouse_pos.y, 2));
			if (distance <= Graph_Draw->circle_radius)  return u;
		}
	}
	return -1;
}

int cGraph_Process::GetIDVertexFromLabel(v_Vertex* VertexList, string label) {
	for (int u = 0; u < VertexList->size(); u++) {
		if (VertexList->at(u).label == label) return u;
	}
	return -1;
}

int cGraph_Process::GetIDEdgeForConnect(int u, int v) {
	if (types_of_graph == UNDIRECTED) {
		for (int e = 0; e < GetEdgeList()->size(); e++) {
			if (GetEdgeList()->at(e).vertex_from == GetVertexList()->at(u).label && GetEdgeList()->at(e).vertex_to == GetVertexList()->at(v).label
				|| GetEdgeList()->at(e).vertex_to == GetVertexList()->at(u).label && GetEdgeList()->at(e).vertex_from == GetVertexList()->at(v).label)
				return e;
		}
		return -1;
	}
	else {
		for (int e = 0; e < GetEdgeList()->size(); e++) {
			if (GetEdgeList()->at(e).vertex_from == GetVertexList()->at(u).label && GetEdgeList()->at(e).vertex_to == GetVertexList()->at(v).label)
				return e;
		}
		return -1;
	}
}

void cGraph_Process::AddVertex(ImVec2 V_Pos, ImVec2 canvas_p0, ImVec2 canvas_p1) {
	GetVertexList()->push_back({ "", V_Pos, "n",false });
	bAddingVertex = true;
}

void cGraph_Process::AddVertexLabel(ImVec2 origin_pos) {
	char label_tmp[100] = "";
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
	ImGui::OpenPopup(u8"Add Vertex Label");
	ImGui::SetNextWindowPos(ImVec2(GetVertexList()->back().Pos.x + origin_pos.x - 4, GetVertexList()->back().Pos.y + origin_pos.y - 15));
	if (ImGui::BeginPopup(u8"Add Vertex Label")) {
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
		ImGui::PushItemWidth(100);
		ImGui::SetKeyboardFocusHere(0);
		if (ImGui::InputText("##edit", label_tmp, IM_ARRAYSIZE(label_tmp), ImGuiInputTextFlags_EnterReturnsTrue))
			if (strlen(label_tmp) > 0 && !CheckVertexExists(label_tmp)) {
				GetVertexList()->back().label = string(label_tmp);
				ProcessGraph();
				bAddingVertex = false;
			}
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsItemHovered())
			if (strlen(label_tmp) > 0 && !CheckVertexExists(label_tmp)) {
				GetVertexList()->back().label = string(label_tmp);
				ProcessGraph();
				bAddingVertex = false;
			}
		ImGui::PopFont();
		ImGui::SameLine();
		Menu->HelpMarker(Menu->utf8_encode(graph_process::add_vertex_label[Menu->i_lang_selected]).c_str());
		ImGui::EndPopup();
	}
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
}

void cGraph_Process::ChangeVertexLabel(ImVec2 origin_pos) {
	char label_tmp[100] = "", label_tmp_bkup[100] = "";
	strncpy(label_tmp, GetVertexList()->at(idVertexSelected).label.c_str(), sizeof(label_tmp));
	strncpy(label_tmp_bkup, GetVertexList()->at(idVertexSelected).label.c_str(), sizeof(label_tmp_bkup));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
	ImGui::OpenPopup(u8"Change Vertex Label");
	ImGui::SetNextWindowPos(ImVec2(GetVertexList()->at(idVertexSelected).Pos.x + origin_pos.x - 4, GetVertexList()->at(idVertexSelected).Pos.y + origin_pos.y - 15));
	if (ImGui::BeginPopup(u8"Change Vertex Label")) {
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
		ImGui::PushItemWidth(100);
		ImGui::SetKeyboardFocusHere(0);
		if (ImGui::InputText("##changevlabel", label_tmp, IM_ARRAYSIZE(label_tmp), ImGuiInputTextFlags_EnterReturnsTrue)) {
			if (strlen(label_tmp) > 0) {
				if (label_tmp == GetVertexList()->at(idVertexSelected).label) {
					idVertexSelected = -1;
					bChangingVertexLabel = false;
				}
				else {
					if (!CheckVertexExists(label_tmp)) {
						GetVertexList()->at(idVertexSelected).label = string(label_tmp);
						ReloadVertexLabelInEdge(label_tmp_bkup, label_tmp);
						ProcessGraph();
						idVertexSelected = -1;
						bChangingVertexLabel = false;
					}
				}

			}
		}
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsItemHovered()) {
			if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) == 1) {
				if (strlen(label_tmp) > 0) {
					if (label_tmp == GetVertexList()->at(idVertexSelected).label) {
						idVertexSelected = -1;
						bChangingVertexLabel = false;
					}
					else {
						if (!CheckVertexExists(label_tmp)) {
							GetVertexList()->at(idVertexSelected).label = string(label_tmp);
							ReloadVertexLabelInEdge(label_tmp_bkup, label_tmp);
							ProcessGraph();
							idVertexSelected = -1;
							bChangingVertexLabel = false;
						}
					}
				}
			}
		}
		ImGui::PopFont();
		ImGui::SameLine();
		Menu->HelpMarker(Menu->utf8_encode(graph_process::add_vertex_label[Menu->i_lang_selected]).c_str());
		ImGui::EndPopup();
	}
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
}

void cGraph_Process::MoveVertex(ImVec2 pos_new) {
	if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
		GetVertexList()->at(idVertexSelected).Pos = pos_new;
	else { bMovingVertex = false; idVertexSelected = -1; bVertexSelected = false; }
}

void cGraph_Process::DeleteVertex() {
	for (int e = 0; e < GetEdgeList()->size(); e++) {
		if (GetEdgeList()->at(e).vertex_from == GetVertexList()->at(idVertexSelected).label || GetEdgeList()->at(e).vertex_to == GetVertexList()->at(idVertexSelected).label) {
			idEdgeSelected = e;
			DeleteEdge();
		}
	}
	GetVertexList()->erase(GetVertexList()->begin() + idVertexSelected, GetVertexList()->begin() + idVertexSelected + 1);
	ProcessGraph();
	idVertexSelected = -1;
	bDeleteVertex = false;
	bVertexSelected = false;
}

void cGraph_Process::AddEdge(ImDrawList* draw_list, ImVec2 origin_pos, ImVec2 centerOxy_pos, ImVec2 mouse_pos) {
	static bool dragged = false;
	static Edge edge_tmp;
	if (ImGui::IsKeyDown(ImGuiKey_LeftShift) && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
		dragged = true;
		ImVec2 pos_from, pos_to, arrow_p1, arrow_p2;
		int idVertexHover = GetIDVertexHover(origin_pos, mouse_pos);
		if (idVertexHover == -1) {
			pos_from.x = GetVertexList()->at(idVertexSelected).Pos.x + origin_pos.x - centerOxy_pos.x;
			pos_from.y = -(GetVertexList()->at(idVertexSelected).Pos.y + origin_pos.y - centerOxy_pos.y);
			pos_to.x = mouse_pos.x - centerOxy_pos.x;
			pos_to.y = -(mouse_pos.y - centerOxy_pos.y);
			float angle_from_to = Math->GetAngleTwoPoints(pos_from, pos_to);
			float angle_to_from = Math->GetAngleTwoPoints(pos_to, pos_from);
			pos_from.x = (pos_from.x + Graph_Draw->circle_radius * cos(angle_from_to)) + centerOxy_pos.x;
			pos_from.y = -(pos_from.y + Graph_Draw->circle_radius * sin(angle_from_to) - centerOxy_pos.y);
			edge_tmp.vertex_from = "";
			edge_tmp.vertex_from = "";
			if (types_of_graph == DIRECTED) {
				arrow_p1.x = (pos_to.x + (Graph_Draw->circle_radius - 15) * cos(angle_to_from + 0.4)) + centerOxy_pos.x;
				arrow_p1.y = -(pos_to.y + (Graph_Draw->circle_radius - 15) * sin(angle_to_from + 0.4) - centerOxy_pos.y);
				arrow_p2.x = (pos_to.x + (Graph_Draw->circle_radius - 15) * cos(angle_to_from - 0.4)) + centerOxy_pos.x;
				arrow_p2.y = -(pos_to.y + (Graph_Draw->circle_radius - 15) * sin(angle_to_from - 0.4) - centerOxy_pos.y);
			}
			pos_to = mouse_pos;
		}
		else {
			pos_from.x = GetVertexList()->at(idVertexSelected).Pos.x + origin_pos.x - centerOxy_pos.x;
			pos_from.y = -(GetVertexList()->at(idVertexSelected).Pos.y + origin_pos.y - centerOxy_pos.y);
			pos_to.x = GetVertexList()->at(idVertexHover).Pos.x + origin_pos.x - centerOxy_pos.x;
			pos_to.y = -(GetVertexList()->at(idVertexHover).Pos.y + origin_pos.y - centerOxy_pos.y);
			float angle_from_to = Math->GetAngleTwoPoints(pos_from, pos_to);
			float angle_to_from = Math->GetAngleTwoPoints(pos_to, pos_from);
			pos_from.x = (pos_from.x + Graph_Draw->circle_radius * cos(angle_from_to)) + centerOxy_pos.x;
			pos_from.y = -(pos_from.y + Graph_Draw->circle_radius * sin(angle_from_to) - centerOxy_pos.y);
			pos_to.x = (pos_to.x + Graph_Draw->circle_radius * cos(angle_to_from)) + centerOxy_pos.x;
			pos_to.y = -(pos_to.y + Graph_Draw->circle_radius * sin(angle_to_from) - centerOxy_pos.y);
			edge_tmp.vertex_from = GetVertexList()->at(idVertexSelected).label;
			edge_tmp.vertex_to = GetVertexList()->at(idVertexHover).label;
			if (types_of_graph == DIRECTED) {
				arrow_p1.x = (pos_to.x - centerOxy_pos.x + (Graph_Draw->circle_radius - 15) * cos(angle_to_from + 0.4)) + centerOxy_pos.x;
				arrow_p1.y = -(-pos_to.y + centerOxy_pos.y + (Graph_Draw->circle_radius - 15) * sin(angle_to_from + 0.4) - centerOxy_pos.y);
				arrow_p2.x = (pos_to.x - centerOxy_pos.x + (Graph_Draw->circle_radius - 15) * cos(angle_to_from - 0.4)) + centerOxy_pos.x;
				arrow_p2.y = -(-pos_to.y + centerOxy_pos.y + (Graph_Draw->circle_radius - 15) * sin(angle_to_from - 0.4) - centerOxy_pos.y);
			}
		}
		draw_list->AddLine(pos_from, pos_to, Graph_Draw->graph_col);
		if (types_of_graph == DIRECTED)
			draw_list->AddTriangleFilled(pos_to, arrow_p1, arrow_p2, Graph_Draw->graph_col);
	}
	else {
		if (dragged) {
			dragged = false;
			if (!edge_tmp.vertex_from.empty() && !edge_tmp.vertex_to.empty()) {
				int id = CheckEdgeExists(edge_tmp);
				int id_reverse = CheckEdgeExists({ edge_tmp.vertex_to, edge_tmp.vertex_from });
				if (id == -1 && id_reverse == -1) {
					edge_tmp.count = 0;
					edge_tmp.length = 0;
				}
				else if (id == -1 && id_reverse != -1) {
					edge_tmp.count = 1;
					edge_tmp.length = 30.0f;
				}
				else if (id != -1) {
					edge_tmp.length = 30.0f + GetEdgeList()->at(id).count * 30.0f;
					edge_tmp.count = GetEdgeList()->at(id).count + 1;
				}
				edge_tmp.p1 = ImVec2(0, 0);
				edge_tmp.p2 = ImVec2(0, 0);
				edge_tmp.p3 = ImVec2(0, 0);
				edge_tmp.w = 0.0f;
				edge_tmp.f = 0.0f;
				edge_tmp.c = 0.0f;
				edge_tmp.visited = false;
				GetEdgeList()->push_back(edge_tmp);
				ProcessGraph();
			}
		}
		idVertexSelected = -1;
		bAddingEdge = false;
		bVertexSelected = false;
	}
}

void cGraph_Process::ChangeEdgeW(ImVec2 origin_pos) {
	char w_tmp[100] = "";
	snprintf(w_tmp, sizeof(w_tmp), "%f", GetEdgeList()->at(idEdgeSelected).w);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
	ImGui::OpenPopup(u8"Change Edge W");
	ImGui::SetNextWindowPos(ImVec2(GetEdgeList()->at(idEdgeSelected).w_pos.x, GetEdgeList()->at(idEdgeSelected).w_pos.y));
	if (ImGui::BeginPopup(u8"Change Edge W")) {
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
		ImGui::PushItemWidth(100);
		ImGui::SetKeyboardFocusHere(0);
		if (ImGui::InputText("##changevlabel", w_tmp, IM_ARRAYSIZE(w_tmp), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal)) {
			if (strlen(w_tmp) > 0) {
				GetEdgeList()->at(idEdgeSelected).w = (float)strtod(w_tmp, NULL);
				idEdgeSelected = -1;
				bChangingEdgeW = false;
				bEdgeSelected = false;
				ProcessGraph();
			}
			else {
				GetEdgeList()->at(idEdgeSelected).w = 0;
				idEdgeSelected = -1;
				bChangingEdgeW = false;
				bEdgeSelected = false;
			}
		}
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsItemHovered()) {
			if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) == 1) {
				if (strlen(w_tmp) > 0) {
					GetEdgeList()->at(idEdgeSelected).w = (float)strtod(w_tmp, NULL);
					idEdgeSelected = -1;
					bChangingEdgeW = false;
					bEdgeSelected = false;
					ProcessGraph();
				}
				else {
					GetEdgeList()->at(idEdgeSelected).w = 0;
					idEdgeSelected = -1;
					bChangingEdgeW = false;
					bEdgeSelected = false;
				}
			}
		}
		ImGui::PopFont();
		ImGui::EndPopup();
	}
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
}

void cGraph_Process::ChangeEdgeF(ImVec2 origin_pos) {
	float tmp[2] = { GetEdgeList()->at(idEdgeSelected).f,GetEdgeList()->at(idEdgeSelected).c };
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
	ImGui::OpenPopup(u8"Change Edge F");
	ImGui::SetNextWindowPos(ImVec2(GetEdgeList()->at(idEdgeSelected).w_pos.x, GetEdgeList()->at(idEdgeSelected).w_pos.y));
	if (ImGui::BeginPopup(u8"Change Edge F")) {
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
		ImGui::PushItemWidth(200);
		if (ImGui::InputFloat2("##inputfloat", tmp, "%f", ImGuiInputTextFlags_EnterReturnsTrue)) {
			if (tmp[0] >= 0 && tmp[1] >= 0) {
				GetEdgeList()->at(idEdgeSelected).f = tmp[0];
				GetEdgeList()->at(idEdgeSelected).c = tmp[1];
				idEdgeSelected = -1;
				bChangingEdgeW = false;
				bEdgeSelected = false;
				ProcessGraph();
			}
			else {
				GetEdgeList()->at(idEdgeSelected).f = 0;
				GetEdgeList()->at(idEdgeSelected).c = 0;
				idEdgeSelected = -1;
				bChangingEdgeW = false;
				bEdgeSelected = false;
			}
		}
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsItemHovered()) {
			if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) == 1) {
				if (tmp[0] >= 0 && tmp[1] >= 0) {
					GetEdgeList()->at(idEdgeSelected).f = tmp[0];
					GetEdgeList()->at(idEdgeSelected).c = tmp[1];
					idEdgeSelected = -1;
					bChangingEdgeW = false;
					bEdgeSelected = false;
					ProcessGraph();
				}
				else {
					GetEdgeList()->at(idEdgeSelected).f = 0;
					GetEdgeList()->at(idEdgeSelected).c = 0;
					idEdgeSelected = -1;
					bChangingEdgeW = false;
					bEdgeSelected = false;
				}
			}
		}
		ImGui::PopFont();
		ImGui::EndPopup();
	}
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
}

void cGraph_Process::ReloadVertexLabelInEdge(string old_vertex_label, string new_vertex_label) {
	for (int e = 0; e < GetEdgeList()->size(); e++) {
		if (GetEdgeList()->at(e).vertex_from == old_vertex_label)
			GetEdgeList()->at(e).vertex_from = new_vertex_label;
		if (GetEdgeList()->at(e).vertex_to == old_vertex_label)
			GetEdgeList()->at(e).vertex_to = new_vertex_label;
	}
}

void cGraph_Process::MoveEdge(ImVec2 mouse_pos, ImVec2 origin_pos, ImVec2 centerOxy_pos) {
	if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
		int id_from = GetIDVertexFromLabel(GetVertexList(), GetEdgeList()->at(idEdgeSelected).vertex_from);
		int id_to = GetIDVertexFromLabel(GetVertexList(), GetEdgeList()->at(idEdgeSelected).vertex_to);

		ImVec2 pos_from(GetVertexList()->at(id_from).Pos.x + origin_pos.x - centerOxy_pos.x, -(GetVertexList()->at(id_from).Pos.y + origin_pos.y - centerOxy_pos.y));
		ImVec2 pos_to(GetVertexList()->at(id_to).Pos.x + origin_pos.x - centerOxy_pos.x, -(GetVertexList()->at(id_to).Pos.y + origin_pos.y - centerOxy_pos.y));
		ImVec2 pos_mouse(mouse_pos.x - centerOxy_pos.x, -(mouse_pos.y - centerOxy_pos.y));
		float distance = Math->DistanceOfPointToLine(pos_from, pos_to, pos_mouse);
		float len = NULL;
		if (!ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
			if (len == NULL)  len = GetEdgeList()->at(idEdgeSelected).length;
			if (len > distance)  distance = (-distance) - (len + distance);
			else  distance = (-distance);
		}
		else
			GetEdgeList()->at(idEdgeSelected).length = -distance;
	}
	else {
		idEdgeSelected = -1;
		bMovingEdge = false;
		bEdgeSelected = false;
	}
}

void cGraph_Process::DeleteEdge() {
	GetEdgeList()->erase(GetEdgeList()->begin() + idEdgeSelected, GetEdgeList()->begin() + idEdgeSelected + 1);
	ProcessGraph();
	idEdgeSelected = -1;
	bDeleteEdge = false;
	bEdgeSelected = false;
}