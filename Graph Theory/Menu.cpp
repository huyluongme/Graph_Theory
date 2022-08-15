#include "Menu.h"

cMenu* Menu = new cMenu();

void cMenu::DrawGUI(LPDIRECT3DDEVICE9 pDevice) {
	InitializeImGuiStartFrame(pDevice);
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::Begin("Graph Theory", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar);

	//MenuBar
	{
		ImGui::BeginMenuBar();

		// MenuBar - Files
		if (ImGui::BeginMenu(menubar::file[i_lang_selected])) {
			if (ImGui::MenuItem(menubar::file[Menu->i_lang_selected+2], "", false))
				OpenGraph();

			if (ImGui::MenuItem(menubar::file[Menu->i_lang_selected + 4], "", false))
				SaveGraph();

			if (ImGui::MenuItem(menubar::file[Menu->i_lang_selected + 6], "", false))
				DeleteGraph();

			ImGui::EndMenu();
		}

		// MenuBar - Setting
		if (ImGui::BeginMenu(menubar::setting[i_lang_selected])) {
			// Choose Theme
			if (ImGui::BeginMenu(menubar::theme[i_lang_selected])) {
				if (ImGui::MenuItem(menubar::theme_dark[i_lang_selected], "", &dark)) {
					dark = true;
					light = false;
					SetTheme(Dark);
				}
				if (ImGui::MenuItem(menubar::theme_light[i_lang_selected], "", &light)) {
					dark = false;
					light = true;
					SetTheme(Light);
				}
				ImGui::EndMenu();
			}

			// Choose Language
			if (ImGui::BeginMenu(menubar::language_set[i_lang_selected])) {
				if (ImGui::MenuItem(menubar::language_set[i_lang_selected + 2], "", &bLanguage_VN)) {
					bLanguage_VN = true;
					bLanguage_EN = false;
					i_lang_selected = 0;
				}
				if (ImGui::MenuItem(menubar::language_set[i_lang_selected + 4], "", &bLanguage_EN)) {
					bLanguage_VN = false;
					bLanguage_EN = true;
					i_lang_selected = 1;
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}

		// Menubar - Tutorial
		if (ImGui::BeginMenu(menubar::tool[i_lang_selected])) {
			// Show tutorial
			if (ImGui::MenuItem(menubar::tool[i_lang_selected + 2], "", bShowTutorial)) bShowTutorial = true;
			// Reset Tool
			if (ImGui::MenuItem(menubar::tool[i_lang_selected + 4], "", false))
				ResetTool();
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	//Main GUI
	{
		ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();
		ImVec2 canvas_sz = ImGui::GetContentRegionAvail();
		if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
		if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
		ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x - 400.0f, canvas_p0.y + canvas_sz.y);

		// Draw border and background color
		ImGuiIO& io = ImGui::GetIO();
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		draw_list->AddRectFilled(canvas_p0, canvas_p1, canvas_rectfill);
		draw_list->AddRect(canvas_p0, canvas_p1, canvas_rect);

		// This will catch our interactions
		ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
		const bool is_hovered = ImGui::IsItemHovered(); // Hovered
		const bool is_active = ImGui::IsItemActive();   // Held
		const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y); // Lock scrolled origin
		const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);
		ImVec2 centerOxy(canvas_p1.x / 2 + origin.x, canvas_p1.y / 2 + origin.y);

		// Show Tutorial
		if (bShowTutorial)
			ShowTutorial();

		// Show Calc Deg Result
		if (Graph_Algorithms->show_deg_log)
			Menu->ShowDegResult(canvas_p0, canvas_p1);

		// Show BFS Result
		if (Graph_Algorithms->show_bfs_log)
			ShowBFS(canvas_p0, canvas_p1);

		// Show DFS (Stack) Result
		if (Graph_Algorithms->show_dfs_stack_log)
			ShowDFSStack(canvas_p0, canvas_p1);

		// Show DFS (Recursive) Result
		if (Graph_Algorithms->show_dfs_recur_log)
			ShowDFSRecursive(canvas_p0, canvas_p1);

		// Show Graph Connect or Not Disconnet
		if (Graph_Algorithms->show_connect_log)
			ShowConnect(canvas_p0, canvas_p1);
		else if (!Graph_Algorithms->show_connect_log && Graph_Algorithms->reset_edge_visited) {
			for (int e = 0; e < Graph_Process->GetEdgeList()->size(); e++)
				Graph_Process->GetEdgeList()->at(e).visited = false;
			for (int u = 0; u < Graph_Process->GetVertexList()->size(); u++)
				Graph_Process->GetVertexList()->at(u).visited = false;
			Graph_Algorithms->reset_edge_visited = false;
		}

		if (Graph_Algorithms->show_cycle_log)
			ShowCycle(canvas_p0, canvas_p1);

		if (Graph_Algorithms->show_toposort_log)
			ShowTopoSort(canvas_p0, canvas_p1);

		if (Graph_Algorithms->show_rank_log)
			ShowRank(canvas_p0, canvas_p1);

		// Show Shortest Path Result (Dijkstra)
		if (Graph_Algorithms->show_dijkstra_log)
			Menu->ShowShortestPath_Dijkstra(canvas_p0, canvas_p1);

		// Show Shortest Path Result (Bellman Ford)
		if (Graph_Algorithms->show_bellmanford_log)
			Menu->ShowShortestPath_BellmanFord(canvas_p0, canvas_p1);

		// Show Kruskal Log
		if (Graph_Algorithms->show_kruskal_log)
			Menu->ShowKruskal(canvas_p0, canvas_p1);
		else if (!Graph_Algorithms->show_kruskal_log && Graph_Algorithms->reset_edge_visited_2) {
			for (int e = 0; e < Graph_Process->GetEdgeList()->size(); e++)
				Graph_Process->GetEdgeList()->at(e).visited = false;
			for (int u = 0; u < Graph_Process->GetVertexList()->size(); u++)
				Graph_Process->GetVertexList()->at(u).visited = false;
			Graph_Algorithms->reset_edge_visited_2 = false;
		}

		// Show Prim
		if (Graph_Algorithms->show_prim_log)
			Menu->ShowPrim(canvas_p0, canvas_p1);

		// Show ChuLiu
		if (ChuLiu->show_chuliu_log)
			Menu->ShowChuLiu(canvas_p0, canvas_p1);

		if (Network->show_network_log)
			ShowNetwork(canvas_p0, canvas_p1);

		// Mouse Event
		if (is_hovered && is_active && Graph_Process->CheckMousePosInCanvas(ImGui::GetMousePos(), canvas_p0, canvas_p1)) {
			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && !ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
				Graph_Process->GetID_W_EdgeSelecting(io.MousePos);
				if (!Graph_Process->bAddingVertex && !Graph_Process->bEdgeSelected && !Graph_Process->bChangingVertexLabel) {
					if (!Graph_Process->CheckMousePosIsVertexPos(origin, io.MousePos))
						Graph_Process->AddVertex(mouse_pos_in_canvas, canvas_p0, canvas_p1);
					else
						Graph_Process->bChangingVertexLabel = true;
				}
			}
			else {
				if (!Graph_Process->bVertexSelected && !Graph_Process->bEdgeSelected) {
					if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Right)) {
						Graph_Process->GetIDVertexSelecting(origin, io.MousePos, true);
						Graph_Process->GetIDEdgeSelecting(io.MousePos, true);
					}
					else {
						Graph_Process->GetIDVertexSelecting(origin, io.MousePos);
						Graph_Process->GetIDEdgeSelecting(io.MousePos);
					}
				}
			}
		}

		// Add Vertex
		if (Graph_Process->bAddingVertex)
			Graph_Process->AddVertexLabel(origin);

		// Change Vertex Label
		if (Graph_Process->bChangingVertexLabel)
			Graph_Process->ChangeVertexLabel(origin);

		// Move Vertex
		if (Graph_Process->bMovingVertex)
			Graph_Process->MoveVertex(mouse_pos_in_canvas);

		// Move Edge
		if (Graph_Process->bMovingEdge)
			Graph_Process->MoveEdge(io.MousePos, origin, centerOxy);

		// Delete Vertex
		if (Graph_Process->bDeleteVertex)
			Graph_Process->DeleteVertex();

		// Delete Edge
		if (Graph_Process->bDeleteEdge)
			Graph_Process->DeleteEdge();

		// Chang W of Edge
		if (Graph_Process->bChangingEdgeW && !Graph_Process->bAddingEdge)
			if (Graph_Process->has_w)
				Graph_Process->ChangeEdgeW(io.MousePos);
			else if (Graph_Process->network)
				Graph_Process->ChangeEdgeF(io.MousePos);

		// Pan (we use a zero mouse threshold when there's no context menu)
		// You may decide to make that threshold dynamic based on whether the mouse is hovering something etc.
		if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, -1.0f))
		{
			scrolling.x += io.MouseDelta.x;
			scrolling.y += io.MouseDelta.y;
		}

		// Draw grid + all lines in the canvas
		draw_list->PushClipRect(canvas_p0, canvas_p1, true);
		//draw grid
		if (opt_enable_grid)
		{
			const float GRID_STEP = 64.0f;
			for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
				draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), grid);
			for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
				draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), grid);
		}

		if (Graph_Process->bAddingEdge)
			Graph_Process->AddEdge(draw_list, origin, centerOxy, io.MousePos);

		//draw graph
		Graph_Draw->DrawEdge(draw_list, Graph_Process->GetVertexList(), Graph_Process->GetEdgeList(), origin, centerOxy);
		Graph_Draw->DrawVertex(draw_list, Graph_Process->GetVertexList(), origin);
		draw_list->PopClipRect();

		// Graph Types
		{
			ImGui::SetCursorPos(ImVec2(canvas_p1.x + 10, canvas_p0.y));
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
			ImGui::BeginChild("Types of Graphs", ImVec2(viewport->WorkSize.x - canvas_p1.x - 18, 68), true);

			ImGui::TextColored(Graph_Draw->header_text_col, graphtype::graph_types[i_lang_selected]);
			ImGui::PushFont(io.Fonts->Fonts[2]);
			if (ImGui::RadioButton(graphtype::graph_types[i_lang_selected + 2], &Graph_Process->types_of_graph, 0)) Graph_Algorithms->InitGraph();
			ImGui::SameLine();
			if (ImGui::RadioButton(graphtype::graph_types[i_lang_selected + 4], &Graph_Process->types_of_graph, 1)) Graph_Algorithms->InitGraph();

			ImGui::SameLine();
			ImGui::Checkbox(graphtype::weight[i_lang_selected], &Graph_Process->has_w);
			ImGui::SameLine();
			ImGui::Checkbox(graphtype::network[i_lang_selected], &Graph_Process->network);
			if (ImGui::IsItemClicked()) {
				if (!Graph_Process->network) {
					Graph_Process->has_w = false;
					Graph_Process->types_of_graph = DIRECTED;
				}
			}
			ImGui::PopFont();
			ImGui::EndChild();

			ImGui::PopStyleVar();
		}

		// Graph Functions
		{
			ImGui::SetCursorPos(ImVec2(canvas_p1.x + 10, canvas_p0.y + 70));
			ImGui::BeginChild("Functions", ImVec2(viewport->WorkSize.x - canvas_p1.x - 18, 208), true);
			ImVec2 rect_min = ImGui::GetItemRectMin();
			ImVec2 rect_max = ImGui::GetItemRectMax();
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);

			ImGui::TextColored(Graph_Draw->header_text_col, graph_functions::func[i_lang_selected]);

			// Calc Deg of Graph
			ImGui::Button(graph_functions::calcdeg[i_lang_selected], ImVec2(-FLT_MIN, 0.0f));
			if (ImGui::IsItemClicked())
				Graph_Algorithms->CalcDeg();
			else if (ImGui::IsItemHovered())
				ToolTip(graph_functions::deg_tooltip[i_lang_selected]);

			// BFS Button
			ImGui::Button(graph_functions::bfs[i_lang_selected], ImVec2((rect_max.x - rect_min.x - 32.0f) / 3, 0.0f));
			if (ImGui::IsItemClicked()) {
				Graph_Algorithms->bfs_start = true;
				Graph_Algorithms->dfs_stack_start = false;
				Graph_Algorithms->dfs_recur_start = false;
			}
			else if (!Graph_Algorithms->bfs_start && ImGui::IsItemHovered())
				ToolTip(graph_functions::bfs[i_lang_selected + 2]);

			ImGui::SameLine();

			// DFS(Stack) Button
			ImGui::Button(graph_functions::dfs[i_lang_selected], ImVec2((rect_max.x - rect_min.x - 32.0f) / 3, 0.0f));
			if (ImGui::IsItemClicked()) {
				Graph_Algorithms->bfs_start = false;
				Graph_Algorithms->dfs_stack_start = true;
				Graph_Algorithms->dfs_recur_start = false;
			}
			else if (!Graph_Algorithms->dfs_stack_start && ImGui::IsItemHovered())
				ToolTip(graph_functions::dfs[i_lang_selected + 4]);

			ImGui::SameLine();

			// DFS(Recursive) Button
			ImGui::Button(graph_functions::dfs[i_lang_selected + 2], ImVec2((rect_max.x - rect_min.x - 32.0f) / 3, 0.0f));
			if (ImGui::IsItemClicked()) {
				Graph_Algorithms->bfs_start = false;
				Graph_Algorithms->dfs_stack_start = false;
				Graph_Algorithms->dfs_recur_start = true;
			}
			else if (!Graph_Algorithms->dfs_recur_start && ImGui::IsItemHovered())
				ToolTip(graph_functions::dfs[i_lang_selected + 6]);


			ImGui::Button(graph_functions::connect[i_lang_selected], ImVec2((rect_max.x - rect_min.x - 32.0f) / 3, 0.0f));
			if (ImGui::IsItemClicked()) {
				if (Graph_Process->types_of_graph == UNDIRECTED)
					Graph_Algorithms->connect();
				else
					Graph_Algorithms->strongconnect();
			}
			else if (ImGui::IsItemHovered())
				ToolTip(graph_functions::connect[i_lang_selected + 2]);

			ImGui::SameLine();
			ImGui::Button(graph_functions::cycle[i_lang_selected], ImVec2((rect_max.x - rect_min.x - 32.0f) / 3, 0.0f));
			if (ImGui::IsItemClicked()) {
				Graph_Algorithms->Check_Has_Cycle();
			}
			else if (ImGui::IsItemHovered()) {
				ToolTip(graph_functions::cycle[i_lang_selected + 2]);
			}

			ImGui::SameLine();
			ImGui::Button(graph_functions::topo[i_lang_selected], ImVec2((rect_max.x - rect_min.x - 32.0f) / 3, 0.0f));
			if (ImGui::IsItemClicked()) {
				if (Graph_Process->types_of_graph == UNDIRECTED) {
					Graph_Algorithms->toposort_log.clear();
					Graph_Algorithms->toposort_log = graph_functions::topo_log[i_lang_selected];
					Graph_Algorithms->show_toposort_log = true;
				}
				else {
					Graph_Algorithms->Check_Has_Cycle();
					Graph_Algorithms->show_cycle_log = false;
					if (Graph_Algorithms->has_circle) {
						Graph_Algorithms->toposort_log.clear();
						Graph_Algorithms->toposort_log = graph_functions::topo_log[i_lang_selected + 2];
						Graph_Algorithms->has_circle = false;
					}
					else Graph_Algorithms->alltopologicalSort();

				}
			}
			else if (ImGui::IsItemHovered())
				ToolTip(graph_functions::topo[i_lang_selected + 2]);

			ImGui::Button(graph_functions::rank[i_lang_selected], ImVec2((rect_max.x - rect_min.x - 32.0f) / 3, 0.0f));
			if (ImGui::IsItemClicked()) {
				Graph_Algorithms->Rank();
			}
			else if (ImGui::IsItemHovered()) {
				ToolTip(graph_functions::rank[i_lang_selected + 2]);
			}

			ImGui::SameLine();
			ImGui::Button("Dijkstra", ImVec2((rect_max.x - rect_min.x - 32.0f) / 3, 0.0f));
			if (ImGui::IsItemClicked()) {
				if (Graph_Process->CheckGraphHasNegativeW()) {
					Graph_Algorithms->dijkstra_log.clear();
					Graph_Algorithms->dijkstra_log = graph_functions::dijkstra_log[i_lang_selected];
					Graph_Algorithms->show_dijkstra_log = true;
				}
				else
					Graph_Algorithms->show_dijkstra_log = true;
			}
			else if (ImGui::IsItemHovered()) {
				ToolTip(graph_functions::shortestpath[i_lang_selected]);
			}

			ImGui::SameLine();
			ImGui::Button(u8"Bellman-Ford", ImVec2((rect_max.x - rect_min.x - 32.0f) / 3, 0.0f));
			if (ImGui::IsItemClicked()) {
				Graph_Algorithms->show_bellmanford_log = true;
			}
			else if (ImGui::IsItemHovered()) {
				ToolTip(graph_functions::shortestpath[i_lang_selected]);
			}

			ImGui::Button("Kruskal", ImVec2((rect_max.x - rect_min.x - 32.0f) / 3, 0.0f));
			if (ImGui::IsItemClicked()) {
				Graph_Algorithms->Kruskal();
			}
			else if (ImGui::IsItemHovered()) {
				ToolTip(graph_functions::MST_UDG[i_lang_selected]);
			}

			ImGui::SameLine();

			ImGui::Button("Prim", ImVec2((rect_max.x - rect_min.x - 32.0f) / 3, 0.0f));
			if (ImGui::IsItemClicked()) {
				Graph_Algorithms->show_prim_log = true;
			}
			else if (ImGui::IsItemHovered()) {
				ToolTip(graph_functions::MST_UDG[i_lang_selected]);
			}

			ImGui::SameLine();

			ImGui::Button("Chu-Liu", ImVec2((rect_max.x - rect_min.x - 32.0f) / 3, 0.0f));
			if (ImGui::IsItemClicked())
				ChuLiu->ChuLiu_Algorithms();
			else if (ImGui::IsItemHovered()) {
				ToolTip(graph_functions::MST_DG[i_lang_selected]);
			}

			ImGui::Button(u8"Luồng & Lát cắt", ImVec2(-FLT_MIN, 0.0f));
			if (ImGui::IsItemClicked())
				Network->Network_Process();
			else if (ImGui::IsItemHovered()) {
				ToolTip(graph_functions::flow_cut[i_lang_selected]);
			}

			if (Graph_Algorithms->bfs_start && Graph_Process->idVertexSelected == -1)
				ToolTip(graph_functions::start_vertex[i_lang_selected]);
			else if (Graph_Algorithms->bfs_start && Graph_Process->idVertexSelected != -1)
				Graph_Algorithms->BFS();

			if (Graph_Algorithms->dfs_stack_start && Graph_Process->idVertexSelected == -1)
				ToolTip(graph_functions::start_vertex[i_lang_selected]);
			else if (Graph_Algorithms->dfs_stack_start && Graph_Process->idVertexSelected != -1)
				Graph_Algorithms->DFS_Stack();

			if (Graph_Algorithms->dfs_recur_start && Graph_Process->idVertexSelected == -1)
				ToolTip(graph_functions::start_vertex[i_lang_selected]);
			else if (Graph_Algorithms->dfs_recur_start && Graph_Process->idVertexSelected != -1)
				Graph_Algorithms->DFS_Recursive_Call();


			ImGui::PopStyleVar();
			ImGui::EndChild();
		}

		// Graph Representations
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
			ImGui::SetCursorPos(ImVec2(canvas_p1.x + 10, canvas_p0.y + 280));
			ImGui::BeginChild("Graph Representations", ImVec2(viewport->WorkSize.x - canvas_p1.x - 18, viewport->WorkSize.y - 320), true);
			ImGui::TextColored(Graph_Draw->header_text_col, graph_represen::graph_repre[i_lang_selected]);
			if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
			{
				if (ImGui::BeginTabItem(graph_represen::edge_lists[i_lang_selected]))
				{
					if (ImGui::BeginTable(graph_represen::edge_lists[i_lang_selected], Graph_Process->has_w == true ? 3 : 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
					{
						ImGui::PushStyleColor(ImGuiCol_Button, ImGuiCol_FrameBg);
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGuiCol_FrameBg);
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGuiCol_FrameBg);
						ImGui::TableNextRow();
						ImGui::TableSetBgColor(1, ImGui::GetColorU32(ImGuiCol_TabActive));
						ImGui::TableNextColumn();
						ImGui::Button("u", ImVec2(-FLT_MIN, 0.0f));
						ImGui::TableNextColumn();
						ImGui::Button("v", ImVec2(-FLT_MIN, 0.0f));
						if (Graph_Process->has_w) {
							ImGui::TableNextColumn();
							ImGui::Button("w", ImVec2(-FLT_MIN, 0.0f));
						}
						ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
						for (int u = 0; u < Graph_Algorithms->Graph.Vertex.size(); u++)
						{
							for (int v = (Graph_Process->types_of_graph == UNDIRECTED ? u : 0); v < Graph_Algorithms->Graph.Vertex.size(); v++)
							{
								for (int e = 0; e < Graph_Algorithms->Graph.A.at(u).at(v); e++) {
									ImGui::TableNextColumn();
									ImGui::Button(Graph_Process->GetVertexList()->at(u).label.c_str(), ImVec2(-FLT_MIN, 0.0f));
									ImGui::TableNextColumn();
									ImGui::Button(Graph_Process->GetVertexList()->at(v).label.c_str(), ImVec2(-FLT_MIN, 0.0f));
									if (Graph_Process->has_w) {
										ImGui::TableNextColumn();
										stringstream stream_w;
										if (Graph_Algorithms->Graph.A[u][v] == 0) stream_w << "No";
										else {
											if (Graph_Algorithms->Graph.W.at(u).at(v) == 0) stream_w << "0";
											else
												stream_w << fixed << setprecision(2) << Graph_Algorithms->Graph.W[u][v];
										}
										ImGui::Button(stream_w.str().c_str(), ImVec2(-FLT_MIN, 0.0f));
									}
								}
							}
						}
						ImGui::PopStyleVar();
						ImGui::PopStyleColor();
						ImGui::PopStyleColor();
						ImGui::PopStyleColor();
						ImGui::EndTable();
					}
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem(graph_represen::adjacency_lists[i_lang_selected]))
				{
					if (ImGui::BeginTable(graph_represen::adjacency_lists[i_lang_selected], 1, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
					{
						ImGui::PushStyleColor(ImGuiCol_Button, ImGuiCol_FrameBg);
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGuiCol_FrameBg);
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGuiCol_FrameBg);
						for (int u = 0; u < Graph_Algorithms->Graph.Vertex.size(); u++) {
							string adj_vertex = "adj[" + Graph_Process->GetVertexList()->at(u).label + "] = [";
							for (int v = 0; v < Graph_Algorithms->Graph.Vertex.size(); v++) {
								for (int e = 0; e < Graph_Algorithms->Graph.A.at(u).at(v); e++) {
									adj_vertex += Graph_Process->GetVertexList()->at(v).label + ", ";
								}
							}
							if (adj_vertex.back() == ' ') {
								adj_vertex.pop_back();
								adj_vertex.pop_back();
							}
							adj_vertex.push_back(']');
							ImGui::TableNextColumn();
							ImGui::Button(adj_vertex.c_str(), ImVec2(-FLT_MIN, 0.0f));
						}
						ImGui::PopStyleColor();
						ImGui::PopStyleColor();
						ImGui::PopStyleColor();
						ImGui::EndTable();
					}
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem(graph_represen::adjacency_matrix[i_lang_selected]))
				{
					if (Graph_Algorithms->Graph.Vertex.size() > 0) {
						if (ImGui::BeginTable(graph_represen::adjacency_matrix[i_lang_selected], Graph_Algorithms->Graph.Vertex.size() + 1, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
						{
							ImGui::PushStyleColor(ImGuiCol_Button, ImGuiCol_FrameBg);
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGuiCol_FrameBg);
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGuiCol_FrameBg);
							ImGui::TableNextRow();
							ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, ImGui::GetColorU32(ImGuiCol_TabActive));
							ImGui::TableNextColumn();
							ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
							ImGui::Button("", ImVec2(-FLT_MIN, 0.0f));
							for (int u = 0; u < Graph_Algorithms->Graph.Vertex.size(); u++) {
								ImGui::TableNextColumn();
								ImGui::Button(Graph_Process->GetVertexList()->at(u).label.c_str(), ImVec2(-FLT_MIN, 0.0f));
							}

							for (int u = 0; u < Graph_Algorithms->Graph.Vertex.size(); u++) {
								ImGui::TableNextColumn();
								ImGui::Button(Graph_Process->GetVertexList()->at(u).label.c_str(), ImVec2(-FLT_MIN, 0.0f));
								ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(ImGuiCol_TabActive));
								ImGui::PushItemFlag(ImGuiTableFlags_RowBg, false);
								for (int v = 0; v < Graph_Algorithms->Graph.Vertex.size(); v++) {
									ImGui::TableNextColumn();
									if (!Graph_Process->has_w)
										ImGui::Button(to_string(Graph_Algorithms->Graph.A[u][v]).c_str(), ImVec2(-FLT_MIN, 0.0f));
									else {
										stringstream stream_w;
										if (Graph_Algorithms->Graph.A[u][v] == 0) stream_w << "No";
										else {
											if (Graph_Algorithms->Graph.W.at(u).at(v) == 0) stream_w << "0";
											else
												stream_w << fixed << setprecision(2) << Graph_Algorithms->Graph.W[u][v];
										}
										ImGui::Button(stream_w.str().c_str(), ImVec2(-FLT_MIN, 0.0f));
									}
								}
								ImGui::PopItemFlag();
							}
							ImGui::PopStyleVar();
							ImGui::PopStyleColor();
							ImGui::PopStyleColor();
							ImGui::PopStyleColor();
							ImGui::EndTable();
						}
					}
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
			ImGui::EndChild();
			ImGui::PopStyleVar();
		}
	}
	ImGui::End();
	InitializeImGuiEndFrame(pDevice);
}

void cMenu::ShowTutorial() {
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.50f, 0.50f));
	ImGui::Begin(menubar::tool[i_lang_selected], &bShowTutorial, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::Text(tutorial::tutorial_show1[i_lang_selected]);
	ImGui::TextColored(Graph_Draw->header_text_col, tutorial::tutorial_show_notice[i_lang_selected]);
	ImGui::Text(tutorial::tutorial_show2[i_lang_selected]);
	ImGui::Text(tutorial::tutorial_show3[i_lang_selected]);
	ImGui::Text(tutorial::tutorial_show4[i_lang_selected]);
	ImGui::Text(tutorial::tutorial_show5[i_lang_selected]);
	ImGui::Text(tutorial::tutorial_show6[i_lang_selected]);
	ImGui::End();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
}

void cMenu::ShowDegResult(ImVec2 p1, ImVec2 p2) {
	ImVec2 center((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.50f, 0.50f));
	ImGui::Begin(graph_functions::deg_title[i_lang_selected], &Graph_Algorithms->show_deg_log, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::Text(utf8_encode(Graph_Algorithms->calc_deg_log).c_str());
	ImGui::End();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
}

void cMenu::ShowBFS(ImVec2 p1, ImVec2 p2) {
	ImVec2 center((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(600, 500), ImGuiCond_Appearing);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.50f, 0.50f));
	ImGui::Begin("BFS", &Graph_Algorithms->show_bfs_log, ImGuiWindowFlags_NoCollapse);
	ImGui::Text(utf8_encode(Graph_Algorithms->bfs_log).c_str());
	ImGui::TextColored(Graph_Draw->header_text_col, graph_functions::tree_search[i_lang_selected]);
	ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();
	ImVec2 canvas_sz = ImGui::GetContentRegionAvail();
	if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
	if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
	ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);
	static ImVec2 scrolling_2(0.0f, 0.0f);
	const ImVec2 origin(canvas_p0.x + scrolling_2.x, canvas_p0.y + scrolling_2.y); // Lock scrolled origin
	ImVec2 centerOxy(canvas_p1.x / 2 + origin.x, canvas_p1.y / 2 + origin.y);
	// Draw border and background color
	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(canvas_p0, canvas_p1, canvas_rectfill);
	draw_list->AddRect(canvas_p0, canvas_p1, canvas_rect);

	// This will catch our interactions
	ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
	const bool is_hovered = ImGui::IsItemHovered(); // Hovered
	const bool is_active = ImGui::IsItemActive();   // Held

	if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, -1.0f))
	{
		scrolling_2.x += io.MouseDelta.x;
		scrolling_2.y += io.MouseDelta.y;
	}

	// Draw grid + all lines in the canvas
	draw_list->PushClipRect(canvas_p0, canvas_p1, true);
	//draw grid
	if (opt_enable_grid)
	{
		const float GRID_STEP = 64.0f;
		for (float x = fmodf(scrolling_2.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
			draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), grid);
		for (float y = fmodf(scrolling_2.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
			draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), grid);
	}

	Graph_Draw->DrawVertex(draw_list, &Graph_Algorithms->BFS_Tree.Vertexs, origin);
	Graph_Draw->DrawEdge(draw_list, &Graph_Algorithms->BFS_Tree.Vertexs, &Graph_Algorithms->BFS_Tree.Edges, origin, origin);
	draw_list->PopClipRect();

	ImGui::End();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
}

void cMenu::ShowDFSStack(ImVec2 p1, ImVec2 p2) {
	ImVec2 center((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(600, 500), ImGuiCond_Appearing);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.50f, 0.50f));
	ImGui::Begin(graph_functions::dfs[i_lang_selected], &Graph_Algorithms->show_dfs_stack_log, ImGuiWindowFlags_NoCollapse);
	ImGui::Text(utf8_encode(Graph_Algorithms->dfs_stack_log).c_str());
	ImGui::TextColored(Graph_Draw->header_text_col, graph_functions::tree_search[i_lang_selected]);
	ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();
	ImVec2 canvas_sz = ImGui::GetContentRegionAvail();
	if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
	if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
	ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);
	static ImVec2 scrolling_2(0.0f, 0.0f);
	const ImVec2 origin(canvas_p0.x + scrolling_2.x, canvas_p0.y + scrolling_2.y); // Lock scrolled origin
	ImVec2 centerOxy(canvas_p1.x / 2 + origin.x, canvas_p1.y / 2 + origin.y);
	// Draw border and background color
	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(canvas_p0, canvas_p1, canvas_rectfill);
	draw_list->AddRect(canvas_p0, canvas_p1, canvas_rect);

	// This will catch our interactions
	ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
	const bool is_hovered = ImGui::IsItemHovered(); // Hovered
	const bool is_active = ImGui::IsItemActive();   // Held

	if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, -1.0f))
	{
		scrolling_2.x += io.MouseDelta.x;
		scrolling_2.y += io.MouseDelta.y;
	}

	// Draw grid + all lines in the canvas
	draw_list->PushClipRect(canvas_p0, canvas_p1, true);
	//draw grid
	if (opt_enable_grid)
	{
		const float GRID_STEP = 64.0f;
		for (float x = fmodf(scrolling_2.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
			draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), grid);
		for (float y = fmodf(scrolling_2.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
			draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), grid);
	}

	Graph_Draw->DrawVertex(draw_list, &Graph_Algorithms->DFS_Stack_Tree.Vertexs, origin);
	Graph_Draw->DrawEdge(draw_list, &Graph_Algorithms->DFS_Stack_Tree.Vertexs, &Graph_Algorithms->DFS_Stack_Tree.Edges, origin, origin);
	draw_list->PopClipRect();

	ImGui::End();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
}

void cMenu::ShowDFSRecursive(ImVec2 p1, ImVec2 p2) {
	ImVec2 center((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(600, 500), ImGuiCond_Appearing);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.50f, 0.50f));
	ImGui::Begin(graph_functions::dfs[i_lang_selected + 2], &Graph_Algorithms->show_dfs_recur_log, ImGuiWindowFlags_NoCollapse);
	ImGui::Text(utf8_encode(Graph_Algorithms->dfs_recur_log).c_str());
	ImGui::TextColored(Graph_Draw->header_text_col, graph_functions::tree_search[i_lang_selected]);
	ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();
	ImVec2 canvas_sz = ImGui::GetContentRegionAvail();
	if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
	if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
	ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);
	static ImVec2 scrolling_2(0.0f, 0.0f);
	const ImVec2 origin(canvas_p0.x + scrolling_2.x, canvas_p0.y + scrolling_2.y); // Lock scrolled origin
	ImVec2 centerOxy(canvas_p1.x / 2 + origin.x, canvas_p1.y / 2 + origin.y);
	// Draw border and background color
	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(canvas_p0, canvas_p1, canvas_rectfill);
	draw_list->AddRect(canvas_p0, canvas_p1, canvas_rect);

	// This will catch our interactions
	ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
	const bool is_hovered = ImGui::IsItemHovered(); // Hovered
	const bool is_active = ImGui::IsItemActive();   // Held

	if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, -1.0f))
	{
		scrolling_2.x += io.MouseDelta.x;
		scrolling_2.y += io.MouseDelta.y;
	}

	// Draw grid + all lines in the canvas
	draw_list->PushClipRect(canvas_p0, canvas_p1, true);
	//draw grid
	if (opt_enable_grid)
	{
		const float GRID_STEP = 64.0f;
		for (float x = fmodf(scrolling_2.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
			draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), grid);
		for (float y = fmodf(scrolling_2.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
			draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), grid);
	}

	Graph_Draw->DrawVertex(draw_list, &Graph_Algorithms->DFS_Recur_Tree.Vertexs, origin);
	Graph_Draw->DrawEdge(draw_list, &Graph_Algorithms->DFS_Recur_Tree.Vertexs, &Graph_Algorithms->DFS_Recur_Tree.Edges, origin, origin);
	draw_list->PopClipRect();

	ImGui::End();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
}

void cMenu::ShowConnect(ImVec2 p1, ImVec2 p2) {
	ImVec2 center((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.50f, 0.50f));
	ImGui::Begin(graph_functions::connect[i_lang_selected], &Graph_Algorithms->show_connect_log, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::Text(utf8_encode(Graph_Algorithms->connect_log).c_str());
	ImGui::End();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
}

void cMenu::ShowCycle(ImVec2 p1, ImVec2 p2) {
	ImVec2 center((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.50f, 0.50f));
	ImGui::Begin(graph_functions::cycle[i_lang_selected], &Graph_Algorithms->show_cycle_log, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::Text(utf8_encode(Graph_Algorithms->cycle_log).c_str());
	ImGui::End();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
}

void cMenu::ShowTopoSort(ImVec2 p1, ImVec2 p2) {
	ImVec2 center((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.50f, 0.50f));
	ImGui::Begin(graph_functions::topo[i_lang_selected], &Graph_Algorithms->show_toposort_log, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::Text(utf8_encode(Graph_Algorithms->toposort_log).c_str());
	ImGui::End();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
}

void cMenu::ShowRank(ImVec2 p1, ImVec2 p2) {
	ImVec2 center((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.50f, 0.50f));
	ImGui::Begin(graph_functions::rank[i_lang_selected], &Graph_Algorithms->show_rank_log, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::Text(utf8_encode(Graph_Algorithms->rank_log).c_str());
	ImGui::End();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
}

void cMenu::ShowShortestPath_Dijkstra(ImVec2 p1, ImVec2 p2) {
	ImVec2 center((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(600, 500), ImGuiCond_Appearing);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.50f, 0.50f));
	ImGui::Begin(u8"Dijkstra", &Graph_Algorithms->show_dijkstra_log, ImGuiWindowFlags_NoCollapse);
	//ImGui::Text(utf8_encode(Graph_Algorithms->DFS_stack_search_result).c_str());
	ImGui::Text(graph_functions::shortest_path_show[i_lang_selected]); ImGui::SameLine();
	static int selected_fish = 0;
	if (ImGui::Button(Graph_Process->GetVertexList()->at(selected_fish).label.c_str()))
		ImGui::OpenPopup("my_select_popup");
	if (ImGui::BeginPopup("my_select_popup"))
	{
		ImGui::Text("Select");
		ImGui::Separator();
		for (int i = 0; i < Graph_Process->GetVertexList()->size(); i++)
			if (ImGui::Selectable(Graph_Process->GetVertexList()->at(i).label.c_str()))
				selected_fish = i;
		ImGui::EndPopup();
	}
	if (ImGui::Button(u8"Bắt đầu"))
		Graph_Algorithms->Dijkstra(selected_fish);
	ImGui::TextColored(Graph_Draw->header_text_col, graph_functions::shortest_path_show[i_lang_selected + 2]);
	ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();
	ImVec2 canvas_sz = ImGui::GetContentRegionAvail();
	if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
	if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
	ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);
	static ImVec2 scrolling_2(0.0f, 0.0f);
	const ImVec2 origin(canvas_p0.x + scrolling_2.x, canvas_p0.y + scrolling_2.y); // Lock scrolled origin
	ImVec2 centerOxy(canvas_p1.x / 2 + origin.x, canvas_p1.y / 2 + origin.y);
	// Draw border and background color
	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(canvas_p0, canvas_p1, canvas_rectfill);
	draw_list->AddRect(canvas_p0, canvas_p1, canvas_rect);

	// This will catch our interactions
	ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
	const bool is_hovered = ImGui::IsItemHovered(); // Hovered
	const bool is_active = ImGui::IsItemActive();   // Held

	if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, -1.0f))
	{
		scrolling_2.x += io.MouseDelta.x;
		scrolling_2.y += io.MouseDelta.y;
	}

	// Draw grid + all lines in the canvas
	draw_list->PushClipRect(canvas_p0, canvas_p1, true);
	//draw grid
	if (opt_enable_grid)
	{
		const float GRID_STEP = 64.0f;
		for (float x = fmodf(scrolling_2.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
			draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), grid);
		for (float y = fmodf(scrolling_2.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
			draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), grid);
	}

	Graph_Draw->DrawVertex(draw_list, &Graph_Algorithms->Dijkstra_Tree.Vertexs, origin);
	Graph_Draw->DrawEdge2(draw_list, &Graph_Algorithms->Dijkstra_Tree.Vertexs, &Graph_Algorithms->Dijkstra_Tree.Edges, origin, origin, true);
	draw_list->PopClipRect();

	ImGui::End();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
}

void cMenu::ShowShortestPath_BellmanFord(ImVec2 p1, ImVec2 p2) {
	ImVec2 center((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(600, 500), ImGuiCond_Appearing);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.50f, 0.50f));
	ImGui::Begin(u8"Bellman - Ford", &Graph_Algorithms->show_bellmanford_log, ImGuiWindowFlags_NoCollapse);
	//ImGui::Text(utf8_encode(Graph_Algorithms->DFS_stack_search_result).c_str());
	ImGui::Text(graph_functions::shortest_path_show[i_lang_selected ]); ImGui::SameLine();
	static int selected_fish = 0;
	if (ImGui::Button(Graph_Process->GetVertexList()->at(selected_fish).label.c_str()))
		ImGui::OpenPopup("my_select_popup");
	if (ImGui::BeginPopup("my_select_popup"))
	{
		ImGui::Text("Select");
		ImGui::Separator();
		for (int i = 0; i < Graph_Process->GetVertexList()->size(); i++)
			if (ImGui::Selectable(Graph_Process->GetVertexList()->at(i).label.c_str()))
				selected_fish = i;
		ImGui::EndPopup();
	}
	if (ImGui::Button(u8"Bắt đầu"))
		Graph_Algorithms->BellmanFord(selected_fish);
	ImGui::TextColored(Graph_Draw->header_text_col, graph_functions::shortest_path_show[i_lang_selected + 2]);
	ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();
	ImVec2 canvas_sz = ImGui::GetContentRegionAvail();
	if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
	if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
	ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);
	static ImVec2 scrolling_2(0.0f, 0.0f);
	const ImVec2 origin(canvas_p0.x + scrolling_2.x, canvas_p0.y + scrolling_2.y); // Lock scrolled origin
	ImVec2 centerOxy(canvas_p1.x / 2 + origin.x, canvas_p1.y / 2 + origin.y);
	// Draw border and background color
	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(canvas_p0, canvas_p1, canvas_rectfill);
	draw_list->AddRect(canvas_p0, canvas_p1, canvas_rect);

	// This will catch our interactions
	ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
	const bool is_hovered = ImGui::IsItemHovered(); // Hovered
	const bool is_active = ImGui::IsItemActive();   // Held

	if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, -1.0f))
	{
		scrolling_2.x += io.MouseDelta.x;
		scrolling_2.y += io.MouseDelta.y;
	}

	// Draw grid + all lines in the canvas
	draw_list->PushClipRect(canvas_p0, canvas_p1, true);
	//draw grid
	if (opt_enable_grid)
	{
		const float GRID_STEP = 64.0f;
		for (float x = fmodf(scrolling_2.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
			draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), grid);
		for (float y = fmodf(scrolling_2.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
			draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), grid);
	}

	Graph_Draw->DrawVertex(draw_list, &Graph_Algorithms->BellmanFord_Tree.Vertexs, origin);
	Graph_Draw->DrawEdge2(draw_list, &Graph_Algorithms->BellmanFord_Tree.Vertexs, &Graph_Algorithms->BellmanFord_Tree.Edges, origin, origin, true);
	draw_list->PopClipRect();

	ImGui::End();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
}

void cMenu::ShowKruskal(ImVec2 p1, ImVec2 p2) {
	ImVec2 center((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(600, 500), ImGuiCond_Appearing);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.50f, 0.50f));
	ImGui::Begin(u8"Kruskal", &Graph_Algorithms->show_kruskal_log, ImGuiWindowFlags_NoCollapse);

	ImGui::TextColored(Graph_Draw->header_text_col, graph_functions::spaning_tree[i_lang_selected], Graph_Algorithms->total_w_kruskal);
	ImGui::TextColored(Graph_Draw->header_text_col, graph_functions::spaning_tree[i_lang_selected + 2]);
	ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();
	ImVec2 canvas_sz = ImGui::GetContentRegionAvail();
	if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
	if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
	ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);
	static ImVec2 scrolling_2(0.0f, 0.0f);
	const ImVec2 origin(canvas_p0.x + scrolling_2.x, canvas_p0.y + scrolling_2.y); // Lock scrolled origin
	ImVec2 centerOxy(canvas_p1.x / 2 + origin.x, canvas_p1.y / 2 + origin.y);
	// Draw border and background color
	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(canvas_p0, canvas_p1, canvas_rectfill);
	draw_list->AddRect(canvas_p0, canvas_p1, canvas_rect);

	// This will catch our interactions
	ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
	const bool is_hovered = ImGui::IsItemHovered(); // Hovered
	const bool is_active = ImGui::IsItemActive();   // Held

	if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, -1.0f))
	{
		scrolling_2.x += io.MouseDelta.x;
		scrolling_2.y += io.MouseDelta.y;
	}

	// Draw grid + all lines in the canvas
	draw_list->PushClipRect(canvas_p0, canvas_p1, true);
	//draw grid
	if (opt_enable_grid)
	{
		const float GRID_STEP = 64.0f;
		for (float x = fmodf(scrolling_2.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
			draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), grid);
		for (float y = fmodf(scrolling_2.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
			draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), grid);
	}

	Graph_Draw->DrawVertex(draw_list, &Graph_Algorithms->Kruskal_Tree.Vertexs, origin);
	Graph_Draw->DrawEdge2(draw_list, &Graph_Algorithms->Kruskal_Tree.Vertexs, &Graph_Algorithms->Kruskal_Tree.Edges, origin, centerOxy, false);
	draw_list->PopClipRect();


	ImGui::End();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
}

void cMenu::ShowPrim(ImVec2 p1, ImVec2 p2) {
	ImVec2 center((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(600, 500), ImGuiCond_Appearing);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.50f, 0.50f));
	ImGui::Begin(u8"Prim", &Graph_Algorithms->show_prim_log, ImGuiWindowFlags_NoCollapse);
	//ImGui::Text(utf8_encode(Graph_Algorithms->DFS_stack_search_result).c_str());
	ImGui::Text(graph_functions::shortest_path_show[i_lang_selected]); ImGui::SameLine();
	static int selected_fish = 0;
	if (ImGui::Button(Graph_Process->GetVertexList()->at(selected_fish).label.c_str()))
		ImGui::OpenPopup("my_select_popup");
	if (ImGui::BeginPopup("my_select_popup"))
	{
		ImGui::Text("Select");
		ImGui::Separator();
		for (int i = 0; i < Graph_Process->GetVertexList()->size(); i++)
			if (ImGui::Selectable(Graph_Process->GetVertexList()->at(i).label.c_str()))
				selected_fish = i;
		ImGui::EndPopup();
	}
	if (ImGui::Button(u8"Bắt đầu"))
		Graph_Algorithms->Prim(selected_fish);
	ImGui::TextColored(Graph_Draw->header_text_col, graph_functions::spaning_tree[i_lang_selected], Graph_Algorithms->total_w_prim);
	ImGui::TextColored(Graph_Draw->header_text_col, graph_functions::spaning_tree[i_lang_selected + 2]);
	ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();
	ImVec2 canvas_sz = ImGui::GetContentRegionAvail();
	if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
	if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
	ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);
	static ImVec2 scrolling_2(0.0f, 0.0f);
	const ImVec2 origin(canvas_p0.x + scrolling_2.x, canvas_p0.y + scrolling_2.y); // Lock scrolled origin
	ImVec2 centerOxy(canvas_p1.x / 2 + origin.x, canvas_p1.y / 2 + origin.y);
	// Draw border and background color
	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(canvas_p0, canvas_p1, canvas_rectfill);
	draw_list->AddRect(canvas_p0, canvas_p1, canvas_rect);

	// This will catch our interactions
	ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
	const bool is_hovered = ImGui::IsItemHovered(); // Hovered
	const bool is_active = ImGui::IsItemActive();   // Held

	if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, -1.0f))
	{
		scrolling_2.x += io.MouseDelta.x;
		scrolling_2.y += io.MouseDelta.y;
	}

	// Draw grid + all lines in the canvas
	draw_list->PushClipRect(canvas_p0, canvas_p1, true);
	//draw grid
	if (opt_enable_grid)
	{
		const float GRID_STEP = 64.0f;
		for (float x = fmodf(scrolling_2.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
			draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), grid);
		for (float y = fmodf(scrolling_2.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
			draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), grid);
	}

	Graph_Draw->DrawVertex(draw_list, &Graph_Algorithms->Prim_Tree.Vertexs, origin);
	Graph_Draw->DrawEdge2(draw_list, &Graph_Algorithms->Prim_Tree.Vertexs, &Graph_Algorithms->Prim_Tree.Edges, origin, origin, false);
	draw_list->PopClipRect();

	ImGui::End();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
}

void cMenu::ShowChuLiu(ImVec2 p1, ImVec2 p2) {
	ImVec2 center((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(600, 500), ImGuiCond_Appearing);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.50f, 0.50f));
	ImGui::Begin(u8"ChuLiu", &ChuLiu->show_chuliu_log, ImGuiWindowFlags_NoCollapse);

	ImGui::TextColored(Graph_Draw->header_text_col, graph_functions::spaning_tree[i_lang_selected], ChuLiu->total_w_chuliu);
	ImGui::TextColored(Graph_Draw->header_text_col, graph_functions::spaning_tree[i_lang_selected + 2]);
	ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();
	ImVec2 canvas_sz = ImGui::GetContentRegionAvail();
	if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
	if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
	ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);
	static ImVec2 scrolling_2(0.0f, 0.0f);
	const ImVec2 origin(canvas_p0.x + scrolling_2.x, canvas_p0.y + scrolling_2.y); // Lock scrolled origin
	ImVec2 centerOxy(canvas_p1.x / 2 + origin.x, canvas_p1.y / 2 + origin.y);
	// Draw border and background color
	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(canvas_p0, canvas_p1, canvas_rectfill);
	draw_list->AddRect(canvas_p0, canvas_p1, canvas_rect);

	// This will catch our interactions
	ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
	const bool is_hovered = ImGui::IsItemHovered(); // Hovered
	const bool is_active = ImGui::IsItemActive();   // Held

	if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, -1.0f))
	{
		scrolling_2.x += io.MouseDelta.x;
		scrolling_2.y += io.MouseDelta.y;
	}

	// Draw grid + all lines in the canvas
	draw_list->PushClipRect(canvas_p0, canvas_p1, true);
	//draw grid
	if (opt_enable_grid)
	{
		const float GRID_STEP = 64.0f;
		for (float x = fmodf(scrolling_2.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
			draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), grid);
		for (float y = fmodf(scrolling_2.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
			draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), grid);
	}

	Graph_Draw->DrawVertex(draw_list, &ChuLiu->ChuLiu_Result.Vertexs, origin);
	Graph_Draw->DrawEdge(draw_list, &ChuLiu->ChuLiu_Result.Vertexs, &ChuLiu->ChuLiu_Result.Edges, origin, centerOxy);
	draw_list->PopClipRect();


	ImGui::End();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
}

void cMenu::ShowNetwork(ImVec2 p1, ImVec2 p2) {
	ImVec2 center((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.50f, 0.50f));
	ImGui::Begin(graph_functions::flow[i_lang_selected], &Network->show_network_log, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::TextColored(Graph_Draw->header_text_col, graph_functions::flow[i_lang_selected + 2], Network->max_flow);
	ImGui::Text(utf8_encode(Network->network_log).c_str());
	ImGui::End();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
}

string cMenu::utf8_encode(const std::wstring& wstr)
{
	if (wstr.empty()) return std::string();
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}

wstring cMenu::utf8_decode(const std::string& str)
{
	if (str.empty()) return std::wstring();
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

wstring cMenu::openfile() {
	OPENFILENAME ofn;
	wchar_t fileName[MAX_PATH] = L"";
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = L"Graph Files (*.graph*)\0*.graph*\0";
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = L"";


	std::wstring fileNameStr;

	if (GetOpenFileName(&ofn))
		fileNameStr = fileName;

	return fileNameStr;
}

wstring cMenu::savefile() {
	OPENFILENAME ofn;
	wchar_t fileName[MAX_PATH] = L"";
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = L"Graph Files (*.graph*)\0*.graph*\0";
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = L"";


	std::wstring fileNameStr;

	if (GetSaveFileName(&ofn))
		fileNameStr = fileName;

	return fileNameStr + L".graph";
}

void cMenu::OpenGraph() {
	fstream f;
	f.open(openfile(), ios::in);
	if (!f.fail()) {
		Graph_Process->GetVertexList()->clear();
		Graph_Process->GetEdgeList()->clear();
		f >> Graph_Process->types_of_graph >> Graph_Process->has_w >> Graph_Process->network;
		int vertex_size, edge_size;
		f >> vertex_size;
		for (int i = 0; i < vertex_size; i++) {
			Vertex tmp;
			f >> tmp.label >> tmp.Pos.x >> tmp.Pos.y >> tmp.color >> tmp.visited;
			Graph_Process->GetVertexList()->push_back(tmp);
		}
		f >> edge_size;
		for (int i = 0; i < edge_size; i++) {
			Edge tmp;
			f >> tmp.vertex_from >> tmp.vertex_to;
			f >> tmp.count >> tmp.length;
			f >> tmp.p1.x >> tmp.p1.y >> tmp.p2.x >> tmp.p2.y >> tmp.p3.x >> tmp.p3.y;
			f >> tmp.w_pos.x >> tmp.w_pos.y;
			f >> tmp.w_pos_rect_p1.x >> tmp.w_pos_rect_p1.y;
			f >> tmp.w_pos_rect_p2.x >> tmp.w_pos_rect_p2.y;
			f >> tmp.angle_from_to >> tmp.w;
			f >> tmp.f >> tmp.c >> tmp.visited;
			Graph_Process->GetEdgeList()->push_back(tmp);
		}
		Graph_Process->ProcessGraph();
	}
	f.close();
}

void cMenu::SaveGraph() {
	ofstream f;
	f.open(savefile(), ios::out);
	if (!f.fail()) {
		int vertex_size = Graph_Process->GetVertexList()->size();
		int edge_size = Graph_Process->GetEdgeList()->size();
		f << Graph_Process->types_of_graph << " " << Graph_Process->has_w << " " << Graph_Process->network << endl;
		f << vertex_size << endl;
		for (int i = 0; i < vertex_size; i++) {
			f << Graph_Process->GetVertexList()->at(i).label << " ";
			f << Graph_Process->GetVertexList()->at(i).Pos.x << " " << Graph_Process->GetVertexList()->at(i).Pos.y << " ";
			f << Graph_Process->GetVertexList()->at(i).color << " ";
			f << Graph_Process->GetVertexList()->at(i).visited;
			f << endl;
		}
		f << edge_size << endl;
		for (int i = 0; i < edge_size; i++) {
			f << Graph_Process->GetEdgeList()->at(i).vertex_from << " " << Graph_Process->GetEdgeList()->at(i).vertex_to << " ";
			f << Graph_Process->GetEdgeList()->at(i).count << " ";
			f << Graph_Process->GetEdgeList()->at(i).length << " ";
			f << Graph_Process->GetEdgeList()->at(i).p1.x << " " << Graph_Process->GetEdgeList()->at(i).p1.y << " ";
			f << Graph_Process->GetEdgeList()->at(i).p2.x << " " << Graph_Process->GetEdgeList()->at(i).p2.y << " ";
			f << Graph_Process->GetEdgeList()->at(i).p3.x << " " << Graph_Process->GetEdgeList()->at(i).p3.y << " ";
			f << Graph_Process->GetEdgeList()->at(i).w_pos.x << " " << Graph_Process->GetEdgeList()->at(i).w_pos.y << " ";
			f << Graph_Process->GetEdgeList()->at(i).w_pos_rect_p1.x << " " << Graph_Process->GetEdgeList()->at(i).w_pos_rect_p1.y << " ";
			f << Graph_Process->GetEdgeList()->at(i).w_pos_rect_p2.x << " " << Graph_Process->GetEdgeList()->at(i).w_pos_rect_p2.y << " ";
			f << Graph_Process->GetEdgeList()->at(i).angle_from_to << " ";
			f << Graph_Process->GetEdgeList()->at(i).w << " ";
			f << Graph_Process->GetEdgeList()->at(i).f << " ";
			f << Graph_Process->GetEdgeList()->at(i).c << " ";
			f << Graph_Process->GetEdgeList()->at(i).visited << endl;
		}
	}
	f.close();
}

void cMenu::DeleteGraph() {
	Graph_Process->GetVertexList()->clear();
	Graph_Process->GetEdgeList()->clear();
	Graph_Process->ProcessGraph();
}

void cMenu::SetTheme(int theme, ImGuiStyle* dst)
{
	ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
	ImVec4* colors = style->Colors;

	if (theme == Dark) {
		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.29f, 0.48f, 0.54f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.29f, 0.48f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		colors[ImGuiCol_Tab] = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.80f);
		colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
		colors[ImGuiCol_TabActive] = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
		colors[ImGuiCol_TabUnfocused] = ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
		colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);   // Prefer using Alpha=1.0 here
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);   // Prefer using Alpha=1.0 here
		colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
		canvas_rect = Graph_Draw->graph_col = IM_COL32(255, 255, 255, 255);
		canvas_rectfill = IM_COL32(36, 36, 36, 255);
		grid = IM_COL32(200, 200, 200, 40);
		Graph_Draw->header_text_col = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
	}
	else if (theme == Light) {
		colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.98f);
		colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.30f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 0.80f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.49f, 0.49f, 0.49f, 0.80f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.46f, 0.54f, 0.80f, 0.60f);
		colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.39f, 0.39f, 0.39f, 0.62f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.14f, 0.44f, 0.80f, 0.78f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.14f, 0.44f, 0.80f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.35f, 0.35f, 0.35f, 0.17f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		colors[ImGuiCol_Tab] = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.90f);
		colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
		colors[ImGuiCol_TabActive] = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
		colors[ImGuiCol_TabUnfocused] = ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
		colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.45f, 0.00f, 1.00f);
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.78f, 0.87f, 0.98f, 1.00f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.57f, 0.57f, 0.64f, 1.00f);   // Prefer using Alpha=1.0 here
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.68f, 0.68f, 0.74f, 1.00f);   // Prefer using Alpha=1.0 here
		colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.30f, 0.30f, 0.30f, 0.09f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		colors[ImGuiCol_NavHighlight] = colors[ImGuiCol_HeaderHovered];
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
		canvas_rectfill = IM_COL32(219, 219, 219, 255);
		canvas_rect = Graph_Draw->graph_col = IM_COL32(15, 15, 15, 255);
		grid = IM_COL32(128, 128, 128, 50);
		Graph_Draw->header_text_col = ImVec4(0.685f, 0.000f, 0.000f, 1.000f);
	}
}

void cMenu::ResetTool() {
	SetTheme(Dark);
	Graph_Process->GetVertexList()->clear();
	Graph_Process->GetEdgeList()->clear();
	Graph_Process->ProcessGraph();
	Graph_Process->types_of_graph = UNDIRECTED;
	Graph_Process->has_w = false;
	Graph_Process->network = false;
	Menu->i_lang_selected = 0;
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

	ImGuiStyle& style = ImGui::GetStyle();
	style.PopupRounding = 6.0f;

	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\tahoma.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesVietnamese());
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\tahoma.ttf", 24.0f, NULL, io.Fonts->GetGlyphRangesVietnamese());
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\tahoma.ttf", 17.0f, NULL, io.Fonts->GetGlyphRangesVietnamese());
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

void cMenu::MessageBoxGUI(string mess) {
	ImGui::OpenPopup(u8"Thông báo");
	if (ImGui::BeginPopupModal(u8"Thông báo", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text(u8"Tên đỉnh không được để trống hoặc không trùng với bất kì tên đỉnh nào đã được tạo!");
		if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}
}

void cMenu::HelpMarker(const char* desc)
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 8.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
	ImGui::TextDisabled("(?) ");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
}

void cMenu::ToolTip(const char* desc) {
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 8.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
	ImGui::BeginTooltip();
	ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
	ImGui::TextUnformatted(desc);
	ImGui::PopTextWrapPos();
	ImGui::EndTooltip();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
}