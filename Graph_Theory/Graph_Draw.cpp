#include "Graph_Draw.h"

int cGraph_Draw::check_edge_created(Edge edge_check) {
	for (int i = edge.size() - 1; i >= 0; i--) {
		if (edge_check.label_from == edge.at(i).label_from && edge_check.label_to == edge.at(i).label_to) {
			return i;
		}
	}
	return -1;
}

int cGraph_Draw::check_node_created(string label) {
	for (int i = 0; i < node.size(); i++) {
		if (label == node.at(i).label)
			return i;
	}
	return -1;
}

Edge_id cGraph_Draw::get_id_node_from_label(string label_from, string label_to) {
	Edge_id res = { -1,-1 };
	for (int i = 0; i < node.size(); i++) {
		if (res.id_from != -1 && res.id_to != -1)
			break;
		if (node.at(i).label == label_from)
			res.id_from = i;
		if (node.at(i).label == label_to)
			res.id_to = i;
	}
	return res;
}

void cGraph_Draw::AddNode(ImVec2 pos, bool* add_node) {
	Node tmp; tmp.Pos = pos;
	node.push_back(tmp);
	*add_node = true;
}

void cGraph_Draw::AddLabel(ImVec2 pos, ImFont* font, bool* add_node) {
	char label_tmp[100] = "";
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::OpenPopup(u8"Add Label");
	ImGui::SetNextWindowPos(pos);
	if (ImGui::BeginPopup(u8"Add Label")) {
		ImGui::PushFont(font);
		ImGui::PushItemWidth(50);
		if (ImGui::InputText("##edit", label_tmp, IM_ARRAYSIZE(label_tmp), ImGuiInputTextFlags_EnterReturnsTrue)) {
			if (strlen(label_tmp) > 0 && check_node_created(label_tmp) == -1) {
				node.back().label = string(label_tmp);
				*add_node = false;
			}
				
		}
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsItemHovered())
		{
			if (strlen(label_tmp) > 0 && check_node_created(label_tmp) == -1) {
				node.back().label = string(label_tmp);
				*add_node = false;
			}
		}
		ImGui::PopFont();
		ImGui::EndPopup();
	}
	ImGui::PopStyleVar();
}

void cGraph_Draw::Get_ID_Node_Selected(ImVec2 origin_pos, ImVec2 mouse_pos, bool* node_selected, int* id_node_selected, bool* move_node, int* rename_del_node, bool* adding_line) {
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
		for (int i = 0; i < node.size(); i++) {
			float distance = sqrt(pow(node.at(i).Pos.x + origin_pos.x - mouse_pos.x, 2) + pow(node.at(i).Pos.y + origin_pos.y - mouse_pos.y, 2));
			if (distance <= 25) {
				*id_node_selected = i;
				*rename_del_node = 0;
				*node_selected = true;
				break;
			}
		}
	}
	else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && CGraph->start == 0) {
		for (int i = 0; i < node.size(); i++) {
			float distance = sqrt(pow(node.at(i).Pos.x + origin_pos.x - mouse_pos.x, 2) + pow(node.at(i).Pos.y + origin_pos.y - mouse_pos.y, 2));
			if (distance <= 25) {
				*id_node_selected = i;
				*node_selected = true;
				break;
			}
		}
	}
	else if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
		for (int i = 0; i < node.size(); i++) {
			float distance = sqrt(pow(node.at(i).Pos.x + origin_pos.x - mouse_pos.x, 2) + pow(node.at(i).Pos.y + origin_pos.y - mouse_pos.y, 2));
			if (distance <= 25) {
				if (!ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
					cout << i << endl;
					*id_node_selected = i;
					*move_node = true;
					*node_selected = true;
					break;
				}
				else {
					*id_node_selected = i;
					*adding_line = true;
					*node_selected = true;
				}
			}
		}
	}
}

void cGraph_Draw::Get_ID_Edge_Selected(bool* move_edge, int* id_edge_selected, bool* edge_selected) {
	if (!ImGui::IsKeyDown(ImGuiKey_LeftShift) && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
		for (int i = 0; i < edge.size(); i++) {
			float distance = sqrt(pow(edge.at(i).pos_mid_draw.x - ImGui::GetMousePos().x, 2) + pow(edge.at(i).pos_mid_draw.y - ImGui::GetMousePos().y, 2));
			if (distance <= 15) {
				*id_edge_selected = i;
				*move_edge = true;
				*edge_selected = true;
				break;
			}
		}
	}
}


void cGraph_Draw::MoveNode(ImVec2 mouse_pos_in_canvas, int* id_node_selected, bool* move_node, bool* node_selected, int* id_edge_selected, bool* move_edge, bool* edge_selected) {
	if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
		*id_edge_selected = -1;
		*move_edge = false;
		*edge_selected = true;
		node.at(*id_node_selected).Pos = mouse_pos_in_canvas;
	}
	else {
		*id_node_selected = -1;
		*move_node = false;
		*edge_selected = false;
		*node_selected = false;
	}
}

void cGraph_Draw::RenameNode(int* id_node_selected, ImVec2 pos, ImFont* font, int* rename_del_node, bool* node_selected) {

	char label_tmp[100] = "";
	strncpy(label_tmp, node.at(*id_node_selected).label.c_str(), sizeof(label_tmp));
	label_tmp[sizeof(label_tmp) - 1] = 0;
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::OpenPopup(u8"Rename Node");
	ImGui::SetNextWindowPos(pos);
	if (ImGui::BeginPopup(u8"Rename Node")) {
		ImGui::PushFont(font);
		ImGui::PushItemWidth(50);
		if (ImGui::InputText("##edit", label_tmp, IM_ARRAYSIZE(label_tmp), ImGuiInputTextFlags_EnterReturnsTrue)) {
			if (strlen(label_tmp) > 0 && (check_node_created(label_tmp) == -1 || check_node_created(label_tmp) == *id_node_selected)) {
				ReloadEdge(node.at(*id_node_selected).label, string(label_tmp));
				node.at(*id_node_selected).label = string(label_tmp);
				*id_node_selected = -1;
				*rename_del_node = -1;
				*node_selected = false;
			}
		}

		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsItemHovered())
		{
			if (strlen(label_tmp) > 0 && (check_node_created(label_tmp) == -1 || check_node_created(label_tmp) == *id_node_selected)) {
				ReloadEdge(node.at(*id_node_selected).label, string(label_tmp));
				node.at(*id_node_selected).label = string(label_tmp);
				*id_node_selected = -1;
				*rename_del_node = -1;
				*node_selected = false;
			}
		}

		ImGui::PopFont();
		ImGui::EndPopup();
	}
	ImGui::PopStyleVar();
}

void cGraph_Draw::MoveEdge(int* id_edge_selected, bool* move_edge, bool* edge_selected, int* id_node_selected, bool* move_node, bool* node_selected) {
	if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
		*id_node_selected = -1;
		*move_node = false;
		*node_selected = true;
		float distance = sqrt(pow(edge.at(*id_edge_selected).pos_mid_ori.x - ImGui::GetMousePos().x, 2) + pow(edge.at(*id_edge_selected).pos_mid_ori.y - ImGui::GetMousePos().y, 2));
		if (edge.at(*id_edge_selected).angle_from_to >= -1.5708 && edge.at(*id_edge_selected).angle_from_to <= 1.5708)
			if (edge.at(*id_edge_selected).angle_from_to > edge.at(*id_edge_selected).angle_from_mouse)
				edge.at(*id_edge_selected).length = distance;
			else
				edge.at(*id_edge_selected).length = -distance;
		if ((edge.at(*id_edge_selected).angle_from_to > 1.5708 && edge.at(*id_edge_selected).angle_from_to <= 3.14159) || (edge.at(*id_edge_selected).angle_from_to < -1.5708 && edge.at(*id_edge_selected).angle_from_to >= -3.14159))
			if (edge.at(*id_edge_selected).angle_to_from > edge.at(*id_edge_selected).angle_to_mouse)
				edge.at(*id_edge_selected).length = -distance;
			else
				edge.at(*id_edge_selected).length = distance;
	}
	else {
		*id_edge_selected = -1;
		*move_edge = false;
		*node_selected = false;
		*edge_selected = false;
	}
}

void cGraph_Draw::AddLine(ImDrawList* draw_list, ImVec2 origin_pos, ImVec2 centerOxy_pos, ImVec2 mouse_pos, int *id_node_selected, bool* adding_edge, bool* node_selected) {
	static bool dragged = false;
	static Edge edge_tmp;
	if (ImGui::IsKeyDown(ImGuiKey_LeftShift) && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
		dragged = true;
		ImVec2 pos_from, pos_to, arrow_p1, arrow_p2; int id_node_to = -1;
		for (int i = 0; i < node.size(); i++) {
			if (i != *id_node_selected) {
				float distance = sqrt(pow(node.at(i).Pos.x + origin_pos.x - mouse_pos.x, 2) + pow(node.at(i).Pos.y + origin_pos.y - mouse_pos.y, 2));
				if (distance <= 25) {
					id_node_to = i;
					break;
				}
				else
					id_node_to = -1;
			}
		}
		if (id_node_to == -1) {
			pos_from.x = node.at(*id_node_selected).Pos.x + origin_pos.x - centerOxy_pos.x;
			pos_from.y = -(node.at(*id_node_selected).Pos.y + origin_pos.y - centerOxy_pos.y);
			pos_to.x = mouse_pos.x - centerOxy_pos.x;
			pos_to.y = -(mouse_pos.y - centerOxy_pos.y);
			float angle_from_to = Math->get_angle_2points(pos_from.x, pos_from.y, pos_to.x, pos_to.y);
			float angle_to_from = Math->get_angle_2points(pos_to.x, pos_to.y, pos_from.x, pos_from.y);
			pos_from.x = (pos_from.x + circle_radius * cos(angle_from_to)) + centerOxy_pos.x;
			pos_from.y = -(pos_from.y + circle_radius * sin(angle_from_to) - centerOxy_pos.y);
			if (un_directed) {
				arrow_p1.x = (pos_to.x + (circle_radius - 15) * cos(angle_to_from + 0.4)) + centerOxy_pos.x;
				arrow_p1.y = -(pos_to.y + (circle_radius - 15) * sin(angle_to_from + 0.4) - centerOxy_pos.y);
				arrow_p2.x = (pos_to.x + (circle_radius - 15) * cos(angle_to_from - 0.4)) + centerOxy_pos.x;
				arrow_p2.y = -(pos_to.y + (circle_radius - 15) * sin(angle_to_from - 0.4) - centerOxy_pos.y);
			}
			pos_to = mouse_pos;
			edge_tmp.label_from = "";
			edge_tmp.label_to = "";
		}
		else {
			pos_from.x = node.at(*id_node_selected).Pos.x + origin_pos.x - centerOxy_pos.x;
			pos_from.y = -(node.at(*id_node_selected).Pos.y + origin_pos.y - centerOxy_pos.y);
			pos_to.x = node.at(id_node_to).Pos.x + origin_pos.x - centerOxy_pos.x;
			pos_to.y = -(node.at(id_node_to).Pos.y + origin_pos.y - centerOxy_pos.y);
			float angle_from_to = Math->get_angle_2points(pos_from.x, pos_from.y, pos_to.x, pos_to.y);
			float angle_to_from = Math->get_angle_2points(pos_to.x, pos_to.y, pos_from.x, pos_from.y);
			pos_from.x = (pos_from.x + circle_radius * cos(angle_from_to)) + centerOxy_pos.x;
			pos_from.y = -(pos_from.y + circle_radius * sin(angle_from_to) - centerOxy_pos.y);
			pos_to.x = (pos_to.x + circle_radius * cos(angle_to_from)) + centerOxy_pos.x;
			pos_to.y = -(pos_to.y + circle_radius * sin(angle_to_from) - centerOxy_pos.y);
			if (un_directed) {
				arrow_p1.x = (pos_to.x - centerOxy_pos.x + (circle_radius - 15) * cos(angle_to_from + 0.4)) + centerOxy_pos.x;
				arrow_p1.y = -(-pos_to.y + centerOxy_pos.y + (circle_radius - 15) * sin(angle_to_from + 0.4) - centerOxy_pos.y);
				arrow_p2.x = (pos_to.x - centerOxy_pos.x + (circle_radius - 15) * cos(angle_to_from - 0.4)) + centerOxy_pos.x;
				arrow_p2.y = -(-pos_to.y + centerOxy_pos.y + (circle_radius - 15) * sin(angle_to_from - 0.4) - centerOxy_pos.y);
			}
			edge_tmp.label_from = node.at(*id_node_selected).label;
			edge_tmp.label_to = node.at(id_node_to).label;
		}
		draw_list->AddLine(pos_from, pos_to, IM_COL32(255, 255, 255, 255));
		if (un_directed)
			draw_list->AddTriangleFilled(pos_to, arrow_p1, arrow_p2, IM_COL32(255, 255, 255, 255));
	}
	else {
		if (dragged) {
			dragged = false;
			if (edge_tmp.label_from != "" && edge_tmp.label_to != "") {
				int id_check = check_edge_created(edge_tmp);
				Edge edge_tmp_reverse;
				edge_tmp_reverse.label_from = edge_tmp.label_to;
				edge_tmp_reverse.label_to = edge_tmp.label_from;
				int id_check_reverse = check_edge_created(edge_tmp_reverse);
				if (id_check == -1 && id_check_reverse == -1) {
					edge_tmp.visited = false;
					edge_tmp.count = 1;
					edge_tmp.length = 0;
					edge_tmp.stt = 0;
					edge.push_back(edge_tmp);
				}
				if (id_check == -1 && id_check_reverse != -1) {
					edge_tmp.visited = false;
					edge_tmp.count = 1;
					edge_tmp.stt = 1;
					edge_tmp.length = 25;
					edge.push_back(edge_tmp);
				}
				if (id_check != -1) {
					edge_tmp.visited = false;
					edge_tmp.count = edge.at(id_check).count + 1;
					edge_tmp.length = 25 + edge.at(id_check).stt * 25;
					edge_tmp.stt = edge.at(id_check).stt + 1;

					edge.push_back(edge_tmp);
				}
			}
		}
		*id_node_selected = -1;
		*adding_edge = false;
		*node_selected = false;
	}
}


void cGraph_Draw::DrawNode(ImDrawList* draw_list, ImVec2 origin_pos) {
	for (int n = 0; n < node.size(); n++) {
		ImU32 Col = IM_COL32(255, 255, 255, 255);
		if (CGraph->start != -1) {
			for (int j = 0; j < browsing_result.size(); j++) {
				if (node.at(n).label == browsing_result.at(j)) {
					Col = IM_COL32(255, 153, 0, 255);
					break;
				}
			}
		}
		draw_list->AddCircle(ImVec2(node.at(n).Pos.x + origin_pos.x, node.at(n).Pos.y + origin_pos.y), circle_radius, Col, 0);
		draw_list->AddText(ImVec2(node.at(n).Pos.x + origin_pos.x - ImGui::CalcTextSize(node.at(n).label.c_str()).x / 2, node.at(n).Pos.y + origin_pos.y - ImGui::CalcTextSize(node.at(n).label.c_str()).y / 2), Col, node.at(n).label.c_str());

	}
}

void cGraph_Draw::DrawLine(ImDrawList* draw_list, ImVec2 origin_pos, ImVec2 centerOxy_pos, ImFont* font) {
	for (int n = 0; n < edge.size(); n++) {
		Edge_id edge_id_tmp = get_id_node_from_label(edge.at(n).label_from, edge.at(n).label_to);
		if (edge_id_tmp.id_from != -1 && edge_id_tmp.id_to != -1) {
			ImVec2 pos_from, pos_to, pos_mid, pos_mid_2, pos_mid_draw, pos_mouse, arrow_p1, arrow_p2;

			float x1 = node.at(edge_id_tmp.id_from).Pos.x + origin_pos.x - centerOxy_pos.x;
			float y1 = -(node.at(edge_id_tmp.id_from).Pos.y + origin_pos.y - centerOxy_pos.y);

			float x_mou = ImGui::GetMousePos().x - centerOxy_pos.x;
			float y_mou = -(ImGui::GetMousePos().y - centerOxy_pos.y);

			float x2 = node.at(edge_id_tmp.id_to).Pos.x + origin_pos.x - centerOxy_pos.x;
			float y2 = -(node.at(edge_id_tmp.id_to).Pos.y + origin_pos.y - centerOxy_pos.y);

			float x_mid = (x1 + x2) / 2;
			float y_mid = (y1 + y2) / 2;

			float angle_from_to = Math->get_angle_2points(x1, y1, x2, y2);
			edge.at(n).angle_from_to = angle_from_to;

			float angle_to_from = Math->get_angle_2points(x2, y2, x1, y1);
			edge.at(n).angle_to_from = angle_to_from;

			float angle_mid_from = Math->get_angle_2points(x_mid, y_mid, x1, y1);

			float angle_from_mouse = Math->get_angle_2points(x1, y1, x_mou, y_mou);
			edge.at(n).angle_from_mouse = angle_from_mouse;

			float angle_to_mouse = Math->get_angle_2points(x2, y2, x_mou, y_mou);
			edge.at(n).angle_to_mouse = angle_to_mouse;

			pos_mid.x = (x_mid + cos(angle_mid_from)) + centerOxy_pos.x;
			pos_mid.y = -(y_mid + sin(angle_mid_from) - centerOxy_pos.y);
			edge.at(n).pos_mid_ori = pos_mid;

			pos_mid_2.x = (x_mid + edge.at(n).length * 2 * cos(angle_mid_from + 1.5708)) + centerOxy_pos.x;
			pos_mid_2.y = -(y_mid + edge.at(n).length * 2 * sin(angle_mid_from + 1.5708) - centerOxy_pos.y);

			pos_mid_draw.x = (pos_mid.x + pos_mid_2.x) / 2;
			pos_mid_draw.y = (pos_mid.y + pos_mid_2.y) / 2;
			edge.at(n).pos_mid_draw = pos_mid_draw;

			float angle_from_mid = Math->get_angle_2points(x1, y1, pos_mid_2.x - centerOxy_pos.x, -pos_mid_2.y + centerOxy_pos.y);
			float angle_to_mid = Math->get_angle_2points(x2, y2, pos_mid_2.x - centerOxy_pos.x, -pos_mid_2.y + centerOxy_pos.y);

			pos_from.x = (x1 + circle_radius * cos(angle_from_mid)) + centerOxy_pos.x;
			pos_from.y = -(y1 + circle_radius * sin(angle_from_mid) - centerOxy_pos.y);

			pos_to.x = (x2 + circle_radius * cos(angle_to_mid)) + centerOxy_pos.x;
			pos_to.y = -(y2 + circle_radius * sin(angle_to_mid) - centerOxy_pos.y);

			ImU32 Col = IM_COL32(255, 255, 255, 255);
			if (CGraph->start != -1 && browsing_result.size() > 0) {
				if(edge.at(n).visited)
					Col = IM_COL32(255, 153, 0, 255);
				
			}


			draw_list->AddBezierQuadratic(pos_from, pos_mid_2, pos_to, Col, 1.0f, 100);
			draw_list->AddCircleFilled(pos_mid_draw, 8, IM_COL32(50, 50, 50, 255));
			ImGui::PushFont(font);
			draw_list->AddText(ImVec2(pos_mid_draw.x - ImGui::CalcTextSize(to_string(n).c_str()).x / 2, pos_mid_draw.y - ImGui::CalcTextSize(to_string(n).c_str()).y / 2), Col, to_string(n).c_str());
			ImGui::PopFont();
			if (un_directed) {
				arrow_p1.x = (pos_to.x - centerOxy_pos.x + (circle_radius - 15) * cos(angle_to_mid + 0.4)) + centerOxy_pos.x;
				arrow_p1.y = -(-pos_to.y + centerOxy_pos.y + (circle_radius - 15) * sin(angle_to_mid + 0.4) - centerOxy_pos.y);
				arrow_p2.x = (pos_to.x - centerOxy_pos.x + (circle_radius - 15) * cos(angle_to_mid - 0.4)) + centerOxy_pos.x;
				arrow_p2.y = -(-pos_to.y + centerOxy_pos.y + (circle_radius - 15) * sin(angle_to_mid - 0.4) - centerOxy_pos.y);
				draw_list->AddTriangleFilled(pos_to, arrow_p1, arrow_p2, Col);
			}
			if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
				float distance = sqrt(pow(pos_mid_draw.x - ImGui::GetMousePos().x, 2) + pow(pos_mid_draw.y - ImGui::GetMousePos().y, 2));
				if (distance < 10) {
					DeleteEdge(n);
					n--;
				}
			}
		}
		else {
			edge.erase(edge.begin() + n, edge.begin() + n + 1);
			n--;
		}
	}
}

void cGraph_Draw::DeleteNode(int id) {
	node.erase(node.begin() + id, node.begin() + id + 1);
}

void cGraph_Draw::DeleteEdge(int id) {
	edge.erase(edge.begin() + id, edge.begin() + id + 1);
}

void cGraph_Draw::DrawPopupRenameOrDel(int* id_node_selected, ImVec2 origin_pos, int* rename_del_node, bool* node_selected) {
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
		float distance = sqrt(pow(node.at(*id_node_selected).Pos.x + origin_pos.x - ImGui::GetMousePos().x, 2) + pow(node.at(*id_node_selected).Pos.y + origin_pos.y - ImGui::GetMousePos().y, 2));
		if (distance > 25) {
			*id_node_selected = -1;
			*rename_del_node = -1;
			*node_selected = false;
		}
	}
	ImGui::OpenPopupOnItemClick("Rename/Delete Node");
	if (ImGui::BeginPopup("Rename/Delete Node"))
	{

		if (ImGui::MenuItem(u8"Xóa đỉnh", NULL, false)) {
			DeleteNode(*id_node_selected);
			*id_node_selected = -1;
			*rename_del_node = -1;
			*node_selected = false;
		}
		if (ImGui::MenuItem(u8"Đổi tên", NULL, false))
			*rename_del_node = 1;
		ImGui::EndPopup();
	}
}

void cGraph_Draw::ReloadEdge(string str_old, string str_new) {
	for (int i = 0; i < edge.size(); i++) {
		if (edge.at(i).label_from == str_old)
			edge.at(i).label_from = str_new;
		if (edge.at(i).label_to == str_old)
			edge.at(i).label_to = str_new;
	}
}
