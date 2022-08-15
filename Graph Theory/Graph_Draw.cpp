#include "Graph_Draw.h"

cGraph_Draw* Graph_Draw = new cGraph_Draw();

void cGraph_Draw::ImRotateStart() {
	rotation_start_index = ImGui::GetWindowDrawList()->VtxBuffer.Size;
}

ImVec2 cGraph_Draw::ImRotationCenter()
{
	ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX); // bounds

	const auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
	for (int i = rotation_start_index; i < buf.Size; i++)
		l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos);

	return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2); // or use _ClipRectStack?
}

ImVec2 operator-(const ImVec2& l, const ImVec2& r) { return{ l.x - r.x, l.y - r.y }; }

void cGraph_Draw::ImRotateEnd(float rad, ImVec2 center)
{
	float s = sin(rad), c = cos(rad);
	center = ImRotate(center, s, c) - center;

	auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
	for (int i = rotation_start_index; i < buf.Size; i++)
		buf[i].pos = ImRotate(buf[i].pos, s, c) - center;
}

void cGraph_Draw::DrawVertex(ImDrawList* draw_list, v_Vertex* VertexList, ImVec2 origin_pos) {
	for (int u = 0; u < VertexList->size(); u++) {
		ImU32 col = graph_col;

		if (VertexList->at(u).visited)
			col = ImGui::GetColorU32(header_text_col);
		else if (VertexList->at(u).color == "s")
			col = ImGui::GetColorU32(ImVec4(1.000f, 0.000f, 0.000f, 1.000f));
		else if(VertexList->at(u).color == "t")
			col = ImGui::GetColorU32(ImVec4(0.309f, 1.000f, 0.000f, 1.000f));

		draw_list->AddCircle(ImVec2(VertexList->at(u).Pos.x + origin_pos.x, VertexList->at(u).Pos.y + origin_pos.y), circle_radius, col, 0, 1.0f);
		draw_list->AddText(ImVec2(VertexList->at(u).Pos.x + origin_pos.x - ImGui::CalcTextSize(VertexList->at(u).label.c_str()).x / 2, VertexList->at(u).Pos.y + origin_pos.y - ImGui::CalcTextSize(VertexList->at(u).label.c_str()).y / 2),
			graph_col, VertexList->at(u).label.c_str());
	}
}

void cGraph_Draw::DrawEdge(ImDrawList* draw_list, v_Vertex* VertexList, v_Edge* EdgeList, ImVec2 origin_pos, ImVec2 centerOxy_pos) {
	for (int e = 0; e < EdgeList->size(); e++) {
		int id_from = Graph_Process->GetIDVertexFromLabel(VertexList, EdgeList->at(e).vertex_from);
		int id_to = Graph_Process->GetIDVertexFromLabel(VertexList, EdgeList->at(e).vertex_to);
		if (id_from != -1 && id_to != -1) {
			ImVec2 pos_from(VertexList->at(id_from).Pos.x + origin_pos.x - centerOxy_pos.x, -(VertexList->at(id_from).Pos.y + origin_pos.y - centerOxy_pos.y));
			ImVec2 pos_to(VertexList->at(id_to).Pos.x + origin_pos.x - centerOxy_pos.x, -(VertexList->at(id_to).Pos.y + origin_pos.y - centerOxy_pos.y));
			ImVec2 pos_mid((pos_from.x + pos_to.x) / 2, (pos_from.y + pos_to.y) / 2);
			ImVec2 arrow_p1, arrow_p2;
			stringstream stream_w;

			float angle_from_to = Math->GetAngleTwoPoints(pos_from, pos_to);
			if (angle_from_to >= -M_PI_2 && angle_from_to <= M_PI_2)
				angle_from_to = angle_from_to + M_PI_2;
			else
				angle_from_to = angle_from_to - M_PI_2;

			float angle_mid_from = Math->GetAngleTwoPoints(pos_mid, pos_from);

			ImVec2 pos_mid_origin((pos_mid.x + cos(angle_mid_from)) + centerOxy_pos.x, -(pos_mid.y + sin(angle_mid_from) - centerOxy_pos.y));

			pos_mid = ImVec2((pos_mid.x + EdgeList->at(e).length * 1.95f * cos(angle_mid_from + 1.5708)) + centerOxy_pos.x,
				-(pos_mid.y + EdgeList->at(e).length * 1.95f * sin(angle_mid_from + 1.5708) - centerOxy_pos.y));

			ImVec2 pos_mid_w((pos_mid.x + pos_mid_origin.x) / 2, (pos_mid.y + pos_mid_origin.y) / 2);

			float angle_from_mid = Math->GetAngleTwoPoints(pos_from, ImVec2(pos_mid.x - centerOxy_pos.x, -pos_mid.y + centerOxy_pos.y));
			float angle_to_mid = Math->GetAngleTwoPoints(pos_to, ImVec2(pos_mid.x - centerOxy_pos.x, -pos_mid.y + centerOxy_pos.y));

			pos_from = ImVec2((pos_from.x + circle_radius * cos(angle_from_mid)) + centerOxy_pos.x,
				-(pos_from.y + circle_radius * sin(angle_from_mid) - centerOxy_pos.y));

			pos_to = ImVec2((pos_to.x + circle_radius * cos(angle_to_mid)) + centerOxy_pos.x,
				-(pos_to.y + circle_radius * sin(angle_to_mid) - centerOxy_pos.y));

			EdgeList->at(e).p1 = pos_from;
			EdgeList->at(e).p2 = pos_mid;
			EdgeList->at(e).p3 = pos_to;
			EdgeList->at(e).w_pos = pos_mid_w;

			ImU32 col = graph_col;

			if (EdgeList->at(e).visited)
				col = ImGui::GetColorU32(header_text_col);

			draw_list->AddBezierQuadratic(pos_from, pos_mid, pos_to, col, 1.0f);

			
			if (Graph_Process->has_w) {

				if (EdgeList->at(e).w == 0.0f) stream_w << fixed << setprecision(0) << EdgeList->at(e).w;
				else stream_w << fixed << setprecision(2) << EdgeList->at(e).w;
				ImVec2 p1(pos_mid_w.x - (ImGui::CalcTextSize(stream_w.str().c_str()).x / 2) - 5.0f,
					pos_mid_w.y - (ImGui::CalcTextSize(stream_w.str().c_str()).y / 2));
				ImVec2 p2(pos_mid_w.x + (ImGui::CalcTextSize(stream_w.str().c_str()).x / 2) + 5.0f,
					pos_mid_w.y + (ImGui::CalcTextSize(stream_w.str().c_str()).y / 2));
				ImVec2 p_center((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);

				EdgeList->at(e).w_pos_rect_p1 = p1;
				EdgeList->at(e).w_pos_rect_p2 = p2;
				EdgeList->at(e).w_pos = pos_mid_w;

				ImRotateStart();
				draw_list->AddRectFilled(p1, p2, Menu->canvas_rectfill, 3.0f);
				draw_list->AddRect(p1, p2, Menu->canvas_rect, 3.0f);
				draw_list->AddText(ImVec2(pos_mid_w.x - ImGui::CalcTextSize(stream_w.str().c_str()).x / 2 + 1.0f, pos_mid_w.y - ImGui::CalcTextSize(stream_w.str().c_str()).y / 2),
					graph_col, stream_w.str().c_str());
				ImRotateEnd(angle_from_to, p_center);

			}
			else if (Graph_Process->network) {
				stream_w << fixed << setprecision(2) << EdgeList->at(e).f << " | " << fixed << setprecision(2) << EdgeList->at(e).c;
				ImVec2 p1(pos_mid_w.x - (ImGui::CalcTextSize(stream_w.str().c_str()).x / 2) - 5.0f,
					pos_mid_w.y - (ImGui::CalcTextSize(stream_w.str().c_str()).y / 2));
				ImVec2 p2(pos_mid_w.x + (ImGui::CalcTextSize(stream_w.str().c_str()).x / 2) + 5.0f,
					pos_mid_w.y + (ImGui::CalcTextSize(stream_w.str().c_str()).y / 2));
				ImVec2 p_center((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);

				EdgeList->at(e).w_pos_rect_p1 = p1;
				EdgeList->at(e).w_pos_rect_p2 = p2;
				EdgeList->at(e).w_pos = pos_mid_w;

				ImRotateStart();
				draw_list->AddRectFilled(p1, p2, Menu->canvas_rectfill, 3.0f);
				draw_list->AddRect(p1, p2, Menu->canvas_rect, 3.0f);
				draw_list->AddText(ImVec2(pos_mid_w.x - ImGui::CalcTextSize(stream_w.str().c_str()).x / 2 + 1.0f, pos_mid_w.y - ImGui::CalcTextSize(stream_w.str().c_str()).y / 2),
					graph_col, stream_w.str().c_str());
				ImRotateEnd(angle_from_to, p_center);
			}

			if (Graph_Process->types_of_graph == DIRECTED) {
				arrow_p1.x = (pos_to.x - centerOxy_pos.x + (circle_radius - 15) * cos(angle_to_mid + 0.4)) + centerOxy_pos.x;
				arrow_p1.y = -(-pos_to.y + centerOxy_pos.y + (circle_radius - 15) * sin(angle_to_mid + 0.4) - centerOxy_pos.y);
				arrow_p2.x = (pos_to.x - centerOxy_pos.x + (circle_radius - 15) * cos(angle_to_mid - 0.4)) + centerOxy_pos.x;
				arrow_p2.y = -(-pos_to.y + centerOxy_pos.y + (circle_radius - 15) * sin(angle_to_mid - 0.4) - centerOxy_pos.y);
				draw_list->AddTriangleFilled(pos_to, arrow_p1, arrow_p2, col);
			}
		}
	}
}

void cGraph_Draw::DrawEdge(ImDrawList* draw_list, v_Vertex* VertexList, v_EdgeTree* EdgeList, ImVec2 origin_pos, ImVec2 centerOxy_pos) {
	for (int e = 0; e < EdgeList->size(); e++) {
		int id_from = Graph_Process->GetIDVertexFromLabel(VertexList, EdgeList->at(e).vertex_from);
		int id_to = Graph_Process->GetIDVertexFromLabel(VertexList, EdgeList->at(e).vertex_to);
		if (id_from != -1 && id_to != -1) {
			ImVec2 pos_from(VertexList->at(id_from).Pos.x + origin_pos.x - centerOxy_pos.x, -(VertexList->at(id_from).Pos.y + origin_pos.y - centerOxy_pos.y));
			ImVec2 pos_to(VertexList->at(id_to).Pos.x + origin_pos.x - centerOxy_pos.x, -(VertexList->at(id_to).Pos.y + origin_pos.y - centerOxy_pos.y));
			float angle_from_to = Math->GetAngleTwoPoints(pos_from, pos_to);
			float angle_to_from = Math->GetAngleTwoPoints(pos_to, pos_from);
			pos_from = ImVec2((pos_from.x + circle_radius * cos(angle_from_to)) + centerOxy_pos.x,
				-(pos_from.y + circle_radius * sin(angle_from_to) - centerOxy_pos.y));

			pos_to = ImVec2((pos_to.x + circle_radius * cos(angle_to_from)) + centerOxy_pos.x,
				-(pos_to.y + circle_radius * sin(angle_to_from) - centerOxy_pos.y));

			ImVec2 arrow_p1((pos_to.x - centerOxy_pos.x + (circle_radius - 15) * cos(angle_to_from + 0.4)) + centerOxy_pos.x, -(-pos_to.y + centerOxy_pos.y + (circle_radius - 15) * sin(angle_to_from + 0.4) - centerOxy_pos.y));
			ImVec2 arrow_p2((pos_to.x - centerOxy_pos.x + (circle_radius - 15) * cos(angle_to_from - 0.4)) + centerOxy_pos.x, -(-pos_to.y + centerOxy_pos.y + (circle_radius - 15) * sin(angle_to_from - 0.4) - centerOxy_pos.y));
			draw_list->AddLine(pos_from, pos_to, graph_col);
			draw_list->AddTriangleFilled(pos_to, arrow_p1, arrow_p2, graph_col);
		}
	}
}

void cGraph_Draw::DrawEdge2(ImDrawList* draw_list, v_Vertex* VertexList, v_EdgeTree* EdgeList, ImVec2 origin_pos, ImVec2 centerOxy_pos, bool arrow) {
	for (int e = 0; e < EdgeList->size(); e++) {
		int id_from = Graph_Process->GetIDVertexFromLabel(VertexList, EdgeList->at(e).vertex_from);
		int id_to = Graph_Process->GetIDVertexFromLabel(VertexList, EdgeList->at(e).vertex_to);
		if (id_from != -1 && id_to != -1) {
			ImVec2 pos_from(VertexList->at(id_from).Pos.x + origin_pos.x - centerOxy_pos.x, -(VertexList->at(id_from).Pos.y + origin_pos.y - centerOxy_pos.y));
			ImVec2 pos_to(VertexList->at(id_to).Pos.x + origin_pos.x - centerOxy_pos.x, -(VertexList->at(id_to).Pos.y + origin_pos.y - centerOxy_pos.y));
			float angle_from_to = Math->GetAngleTwoPoints(pos_from, pos_to);
			float angle_to_from = Math->GetAngleTwoPoints(pos_to, pos_from);
			pos_from = ImVec2((pos_from.x + circle_radius * cos(angle_from_to)) + centerOxy_pos.x,
				-(pos_from.y + circle_radius * sin(angle_from_to) - centerOxy_pos.y));

			pos_to = ImVec2((pos_to.x + circle_radius * cos(angle_to_from)) + centerOxy_pos.x,
				-(pos_to.y + circle_radius * sin(angle_to_from) - centerOxy_pos.y));

			ImVec2 pos_mid((pos_from.x + pos_to.x) / 2, (pos_from.y + pos_to.y) / 2);

			stringstream stream_w;

			if (EdgeList->at(e).W == 0.0f) stream_w << fixed << setprecision(0) << EdgeList->at(e).W;
			else stream_w << fixed << setprecision(2) << EdgeList->at(e).W;

			ImVec2 p1(pos_mid.x - (ImGui::CalcTextSize(stream_w.str().c_str()).x / 2) - 5.0f,
				pos_mid.y - (ImGui::CalcTextSize(stream_w.str().c_str()).y / 2));
			ImVec2 p2(pos_mid.x + (ImGui::CalcTextSize(stream_w.str().c_str()).x / 2) + 5.0f,
				pos_mid.y + (ImGui::CalcTextSize(stream_w.str().c_str()).y / 2));
			ImVec2 p_center((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);

			ImVec2 arrow_p1((pos_to.x - centerOxy_pos.x + (circle_radius - 15) * cos(angle_to_from + 0.4)) + centerOxy_pos.x, -(-pos_to.y + centerOxy_pos.y + (circle_radius - 15) * sin(angle_to_from + 0.4) - centerOxy_pos.y));
			ImVec2 arrow_p2((pos_to.x - centerOxy_pos.x + (circle_radius - 15) * cos(angle_to_from - 0.4)) + centerOxy_pos.x, -(-pos_to.y + centerOxy_pos.y + (circle_radius - 15) * sin(angle_to_from - 0.4) - centerOxy_pos.y));
			draw_list->AddLine(pos_from, pos_to, graph_col);
			if (arrow)
				draw_list->AddTriangleFilled(pos_to, arrow_p1, arrow_p2, graph_col);

			if (Graph_Process->has_w) {

				if (angle_from_to >= -M_PI_2 && angle_from_to <= M_PI_2)
					angle_from_to = angle_from_to + M_PI_2;
				else
					angle_from_to = angle_from_to - M_PI_2;

				ImRotateStart();
				draw_list->AddRectFilled(p1, p2, Menu->canvas_rectfill, 3.0f);
				draw_list->AddRect(p1, p2, Menu->canvas_rect, 3.0f);
				draw_list->AddText(ImVec2(pos_mid.x - ImGui::CalcTextSize(stream_w.str().c_str()).x / 2 + 1.0f, pos_mid.y - ImGui::CalcTextSize(stream_w.str().c_str()).y / 2),
					graph_col, stream_w.str().c_str());
				ImRotateEnd(angle_from_to, p_center);

			}
		}
	}
}