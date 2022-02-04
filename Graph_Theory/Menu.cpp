#include "Menu.h"

void cMenu::DrawMenu(LPDIRECT3DDEVICE9 pDevice) {
	InitializeImGuiStartFrame(pDevice);
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);

	ImGui::Begin(u8"Lý thuyết đồ thị", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	// Using InvisibleButton() as a convenience 1) it will advance the layout cursor and 2) allows us to use IsItemHovered()/IsItemActive()
	ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
	ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
	if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
	if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
	ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x - 400.0f, canvas_p0.y + canvas_sz.y - 200.0f);


	// Draw border and background color
	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
	draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

	// This will catch our interactions
	ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
	const bool is_hovered = ImGui::IsItemHovered(); // Hovered
	const bool is_active = ImGui::IsItemActive();   // Held
	const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y); // Lock scrolled origin
	const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

	ImVec2 centerOxy((canvas_p0.x + canvas_sz.x - 400.0f) / 2 + origin.x, (canvas_p0.y + canvas_sz.y - 200.0f) / 2 + origin.y);

	if (is_hovered && !adding_node && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		Graph_Draw->AddNode(mouse_pos_in_canvas, &adding_node);

	if (adding_node)
		Graph_Draw->AddLabel(ImVec2(node.back().Pos.x + origin.x - 5, node.back().Pos.y + origin.y - 15), io.Fonts->Fonts[1], &adding_node);

	if (is_hovered && is_active && !node_selected && !edge_selected) {
		Graph_Draw->Get_ID_Node_Selected(origin, io.MousePos, &node_selected, &id_node_selected, &move_node, &rename_del_node, &adding_edge);
		Graph_Draw->Get_ID_Edge_Selected(&edge_selected, &id_edge_selected, &move_edge);
	}

	if (move_node) {
		Graph_Draw->MoveNode(mouse_pos_in_canvas, &id_node_selected, &move_node, &node_selected, &id_edge_selected, &move_edge, &edge_selected);
	}

	if (move_edge)
		Graph_Draw->MoveEdge(&id_edge_selected, &move_edge, &edge_selected, &id_node_selected, &move_node, &node_selected);

	if (rename_del_node == 0) {

		Graph_Draw->DrawPopupRenameOrDel(&id_node_selected, origin, &rename_del_node, &node_selected);
	}

	if (rename_del_node == 1)
		Graph_Draw->RenameNode(&id_node_selected, ImVec2(node.at(id_node_selected).Pos.x + origin.x - 5, node.at(id_node_selected).Pos.y + origin.y - 15), io.Fonts->Fonts[1], &rename_del_node, &node_selected);

	// Pan (we use a zero mouse threshold when there's no context menu)
	// You may decide to make that threshold dynamic based on whether the mouse is hovering something etc.
	const float mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;
	if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan))
	{
		scrolling.x += io.MouseDelta.x;
		scrolling.y += io.MouseDelta.y;
	}

	// Draw grid + all lines in the canvas
	draw_list->PushClipRect(canvas_p0, canvas_p1, true);

	if (opt_enable_grid)
	{
		const float GRID_STEP = 64.0f;
		for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
			draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 40));
		for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
			draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 40));
	}

	if (adding_edge)
		Graph_Draw->AddLine(draw_list, origin, centerOxy, io.MousePos, &id_node_selected, &adding_edge, &node_selected);


	Graph_Draw->DrawLine(draw_list, origin, centerOxy, io.Fonts->Fonts[2]);
	Graph_Draw->DrawNode(draw_list, origin);


	draw_list->PopClipRect();

	ImGui::SetCursorPos(ImVec2(canvas_p1.x + 10, canvas_p0.y));
	ImGui::BeginChild("Un/Direted", ImVec2(viewport->WorkSize.x - canvas_p1.x - 18, 68), true);

	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), u8"Dạng đồ thị:");
	ImGui::RadioButton(u8"Vô hướng", &Graph_Draw->un_directed, 0);
	ImGui::SameLine();
	ImGui::RadioButton(u8"Có hướng", &Graph_Draw->un_directed, 1);
	ImGui::EndChild();

	ImGui::SetCursorPos(ImVec2(canvas_p1.x + 10, canvas_p0.y + 70));
	ImGui::BeginChild(u8"Duyệt đồ thị", ImVec2(viewport->WorkSize.x - canvas_p1.x - 18, 72), true);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), u8"Duyệt đồ thị:");
	if (ImGui::Button("BFS", ImVec2(120, 0))) {
		if (node.size() > 0)
			CGraph->start = 0;
		browsing_result.clear();
	}
	ImGui::SameLine();
	ImGui::Button(u8"DFS (Ngăm xếp)"); ImGui::SameLine();
	ImGui::Button(u8"DFS (Đệ quy)");
	ImGui::PopStyleVar();
	ImGui::EndChild();

	ImGui::SetCursorPos(ImVec2(canvas_p1.x + 10, canvas_p0.y + 144));
	ImGui::BeginChild(u8"Biểu diễn đồ thị", ImVec2(viewport->WorkSize.x - canvas_p1.x - 18, viewport->WorkSize.y - 86), true);
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), u8"Biểu diễn đồ thị:");
	if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem(u8"Danh sách cung"))
		{
			if (ImGui::BeginTable(u8"Danh sách cung", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
			{
				for (int i = 0; i < edge.size(); i++)
				{
					ImGui::TableNextColumn();
					ImGui::Button(edge.at(i).label_from.c_str(), ImVec2(-FLT_MIN, 0.0f));
					ImGui::TableNextColumn();
					ImGui::Button(edge.at(i).label_to.c_str(), ImVec2(-FLT_MIN, 0.0f));

				}
				ImGui::EndTable();
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(u8"Danh sách kề"))
		{
			if (ImGui::BeginTable(u8"Danh sách kề", 1, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
			{
				for (int i = 0; i < Graph.size(); i++)
				{
					string tmp = "adj[" + Graph[i].label + "] = [";
					for (int j = 0; j < Graph[i].adj.size(); j++) {
						tmp += Graph[i].adj[j];
						if (j < Graph[i].adj.size() - 1)
							tmp += ", ";
					}
					tmp += "]";
					ImGui::TableNextColumn();
					ImGui::Button(tmp.c_str(), ImVec2(-FLT_MIN, 0.0f));

				}
				ImGui::EndTable();
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(u8"Ma trận kề"))
		{
			if (Graph.size() > 0) {
				if (ImGui::BeginTable(u8"Ma trận kề", Graph.size() + 1, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
				{
					ImGui::TableNextColumn();
					ImGui::Button("", ImVec2(-FLT_MIN, 0.0f));
					for (int i = 0; i < Graph.size(); i++) {
						ImGui::TableNextColumn();
						ImGui::Button(Graph.at(i).label.c_str(), ImVec2(-FLT_MIN, 0.0f));
					}

					for (int i = 0; i < Graph.size(); i++) {
						ImGui::TableNextColumn();
						ImGui::Button(Graph.at(i).label.c_str(), ImVec2(-FLT_MIN, 0.0f));
						for (int j = 0; j < Graph.size(); j++) {
							int count = 0;

							if (Graph_Draw->un_directed == 0) {
								Edge e1;
								e1.label_from = Graph.at(i).label;
								e1.label_to = Graph.at(j).label;
								int check1 = Graph_Draw->check_edge_created(e1);
								e1.label_from = Graph.at(j).label;
								e1.label_to = Graph.at(i).label;
								int check2 = Graph_Draw->check_edge_created(e1);
								if (check1 != -1 && check2 == -1)
									count = edge.at(check1).count;
								else if (check1 == -1 && check2 != -1)
									count = edge.at(check2).count;
								else if (check1 != -1 && check2 != -1)
									count = edge.at(check1).count + edge.at(check2).count;
							}
							else {
								Edge e1;
								e1.label_from = Graph.at(i).label;
								e1.label_to = Graph.at(j).label;
								int check1 = Graph_Draw->check_edge_created(e1);
								if (check1 != -1)
									count = edge.at(check1).count;
							}

							ImGui::TableNextColumn();
							ImGui::Button(to_string(count).c_str(), ImVec2(-FLT_MIN, 0.0f));
						}
					}
					ImGui::EndTable();
				}
			}
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::EndChild();

	ImGui::SetCursorPosY(canvas_p1.y + 10);
	ImGui::PushFont(io.Fonts->Fonts[3]);
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), u8"Log/Help:");
	ImGui::PopFont();
	ImGui::SetCursorPosX(canvas_p0.x);
	ImGui::BeginChild("Log/Help", ImVec2(canvas_p1.x - canvas_p0.x, viewport->WorkSize.y - canvas_p1.y - 40), true, ImGuiWindowFlags_HorizontalScrollbar);
	if (CGraph->start == -1) {
		ImGui::PushFont(io.Fonts->Fonts[3]);
		ImGui::Text(u8"+ Tạo đỉnh: Double click chuột -> Đặt tên đỉnh (Bắt buộc)");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), u8"(Chú ý: TÊN ĐỈNH KHÔNG ĐƯỢC PHÉP TRÙNG NHAU)");
		ImGui::Text(u8"+ Di chuyển đỉnh: Giữ chuột trái vào đỉnh và kéo đến vị trí mong muốn");
		ImGui::Text(u8"+ Xóa đỉnh - Đổi tên đỉnh: Chuột phải vào đỉnh cần xóa - đổi tên");
		ImGui::Text(u8"+ Tạo cung: Giữ Shift trái + kéo chuột (Đỉnh đầu -> Đỉnh cuối)");
		ImGui::Text(u8"+ Di chuyển cung: Giữ chuột trái tại vị trí số được đánh dấu trên cung và di chuyển đến vị trí mong muốn");
		ImGui::Text(u8"+ Xóa cung: chuột phải vào vị trí số được đánh dấu trên cung cần xóa");
		ImGui::Text(u8"+ Di chuyển bảng: Giữ chuột phải và kéo");
		ImGui::PopFont();
	}
	else if (CGraph->start == 0) {
		static bool browsed = false;
		ImGui::PushFont(io.Fonts->Fonts[1]);
		if (id_node_selected == -1)
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), u8"Vui lòng chọn đỉnh bắt đầu!");
		else {
			string res = "";
			CGraph->BFS(node.at(id_node_selected).label);

			for (int i = 0; i < browsing_result.size() - 1; i++) {
				res += browsing_result.at(i) + " --> ";
			}
			res += browsing_result.at(browsing_result.size() - 1);

			ImGui::Text(u8"Đỉnh bắt đầu: %s", browsing_result.at(0).c_str());
			ImGui::Text(u8"BFS: %s", res.c_str());
			ImGui::PushFont(io.Fonts->Fonts[0]);
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), u8"Click chuột vào vị trí bất kì trên bảng để tiếp tục!");
			ImGui::PopFont();

			if (is_hovered && browsed == true && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
				for (int i = 0; i < edge.size(); i++) {
					edge.at(i).visited = false;
				}
				CGraph->start = -1;
				node_selected = false;
				id_node_selected = -1;
				res.clear();
				browsed = false;
			}
			else
				browsed = true;
		}
		ImGui::PopFont();
	}
	ImGui::EndChild();
	CGraph->Graph_Init();
	ImGui::End();
	InitializeImGuiEndFrame(pDevice);
}

void cMenu::InitializeImGui(LPDIRECT3DDEVICE9 pDevice, HWND hwnd)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX9_Init(pDevice);

	char fontdir[MAX_PATH];
	GetWindowsDirectoryA(fontdir, sizeof(fontdir));
	strcat(fontdir, "\\Fonts\\tahoma.ttf");

	io.Fonts->AddFontFromFileTTF(fontdir, 20.0f, NULL, io.Fonts->GetGlyphRangesVietnamese());
	io.Fonts->AddFontFromFileTTF(fontdir, 26.0f, NULL, io.Fonts->GetGlyphRangesVietnamese());
	io.Fonts->AddFontFromFileTTF(fontdir, 14.0f, NULL, io.Fonts->GetGlyphRangesVietnamese());
	io.Fonts->AddFontFromFileTTF(fontdir, 18.0f, NULL, io.Fonts->GetGlyphRangesVietnamese());

	io.IniFilename = NULL;
}

void cMenu::InitializeImGuiStartFrame(LPDIRECT3DDEVICE9 pDevice)
{
	// Start the Dear ImGui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void cMenu::InitializeImGuiEndFrame(LPDIRECT3DDEVICE9 pDevice)
{
	// Rendering
	ImGui::EndFrame();
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, NULL, 1.0f, 0);
	if (pDevice->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		pDevice->EndScene();
	}
}