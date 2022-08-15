#pragma once


namespace language_np {
	namespace menubar {
		static const char* file[] = { u8"Tệp", u8"File", u8"Mở", u8"Open", u8"Lưu", u8"Save", u8"Xóa đồ thị", u8"Delete graph"};
		static const char* setting[] = { u8"Cài đặt", u8"Setting" };
		static const char* theme[] = { u8"Giao diện", u8"Theme" };
		static const char* theme_dark[] = { u8"Tối", u8"Dark" };
		static const char* theme_light[] = { u8"Sáng", u8"Light" };
		static const char* language_set[] = { u8"Ngôn ngữ", u8"Language", u8"Tiếng Việt", u8"Vietnamese",u8"Tiếng Anh", u8"English" };
		static const char* tool[] = { u8"Công cụ", u8"Tool", u8"Hướng dẫn", u8"Tutorial", u8"Đặt lại", u8"Reset" };
	}

	namespace graphtype {
		static const char* graph_types[] = { u8"Dạng đồ thị:", u8"Types of Graph:",  u8"Vô hướng", u8"Undirected", u8"Có hướng", u8"Directed" };
		static const char* weight[] = { u8"Trọng số", u8"Weight" };
		static const char* network[] = { u8"Mạng", u8"Network" };
	}

	namespace graph_functions {
		static const char* func[] = { u8"Chức năng:", u8"Functions:" };
		static const char* calcdeg[] = { u8"Tính bậc", u8"Degree" };
		static const char* deg_tooltip[] = { u8"Tính bậc của đỉnh", u8"Calc degree of vertex" };
		static const char* deg_title[] = { u8"Bậc", u8"Degree" };
		static const char* bfs[] = { u8"BFS (Hàng đợi)", u8"BFS (Queue)" , u8"Duyệt theo chiều rộng\nSử dụng hàng đợi", u8"Breadth First Search\nUse queue" };
		static const char* dfs[] = { u8"DFS (Ngăn xếp)", u8"DFS (Stack)", u8"DFS (Đệ quy)", u8"DFS (Recursive)", u8"Duyệt theo chiều sâu\nSử dụng ngăn xếp", u8"Depth First Search\nUse Stack" , u8"Duyệt theo chiều sâu\nSử dụng đệ quy", u8"Depth First Search\nUse recursive" };
		static const char* connect[] = { u8"Liên thông", u8"Connect", u8"Kiểm tra đồ thị liên thông", u8"Check graph is connected or not" };
		static const char* cycle[] = { u8"Chu trình", u8"Cycle", u8"Kiểm tra đồ thị chứa chu trình", u8"Detect Cycle in a Graph" };
		static const char* topo[] = { u8"Thứ tự Topo" , u8"Topo Sort", u8"Tìm tất cả các thứ tự topo trong đồ thị có hướng", u8"Find all topological sorts of a Directed Graph"};
		static const wchar_t* topo_log[] = { L"Đồ thị vô hướng -> không tìm được thứ tự topo\nĐiều kiện để có thứ tự topo:\n - Đồ thị có hướng\n - Đồ thị không chứa chu trình\n" ,L"Undirected Graph -> Can't find topological sort\nConditions for topological sorts:\n - Directed Graph\n - Graph don't contain cycle", L"Đồ thị chứa chu trình -> không tìm được thứ tự topo\nĐiều kiện để có thứ tự topo:\n - Đồ thị có hướng\n - Đồ thị không chứa chu trình\n" ,L"Graph contain cycle -> Can't find topological sort\nConditions for topological sorts:\n - Directed Graph\n - Graph don't contain cycle" };
		static const char* rank[] = { u8"Xếp hạng", u8"Rank", u8"Xếp hạng đồ thị", u8"Ranking of nodes in graphs" };
		static const char* shortestpath[] = { u8"Tìm đường đi ngắn nhất giữa các đỉnh trong đồ thị", u8"Find shortest path in graph" };
		static const wchar_t* dijkstra_log[] = { L"Đồ thị có trọng số âm. Vui lòng sử dụng thuật toán Bellman – Ford" , L"Graph have negative weight. Please use Bellman – Ford algorithm" };
		static const char* MST_UDG[] = { u8"Tìm cây khung nhỏ nhất trong đồ thị vô hướng", u8"Find minimum spanning tree in undirected graph" };
		static const char* MST_DG[] = { u8"Tìm cây khung nhỏ nhất trong đồ thị có hướng", u8"Find minimum spanning tree in directed graph" };
		static const char* flow_cut[] = { u8"Luồng & Lát cắt", u8"Flow & Cut" };
		static const char* start_vertex[] = { u8"Vui lòng chọn đỉnh bắt đầu", u8"Please select start vertex" };
		static const char* tree_search[] = { u8"Cây duyệt đồ thị:", u8"Search Graph Tree" };
		static const char* shortest_path_show[] = { u8"Chọn đỉnh bắt đầu:", u8"Select start vertex:", u8"Cây đường đi ngắn nhất", u8"Tree Shortest Path"};
		static const char* spaning_tree[] = { u8"Tổng trọng lượng: %.2f", u8"Total weight: %.2f", u8"Cây khung tối tiểu", u8"Minimum spanning tree"};
		static const char* flow[] = { u8"Luồng & Lát cắt", u8"Flow & Cut", u8"Luồng cực đại: %.2f", u8"Max flow: %.2f" };

	}

	namespace graph_represen {
		static const char* graph_repre[] = { u8"Biểu diễn đồ thị:", u8"Graph Representations:" };
		static const char* edge_lists[] = { u8"Danh sách cung", u8"Edge Lists" };
		static const char* adjacency_lists[] = { u8"Danh sách kề", u8"Adjacency Lists" };
		static const char* adjacency_matrix[] = { u8"Ma trận kề", u8"Adjacency Matrix" };
	}

	namespace tutorial {
		static const char* tutorial_show1[] = { u8"+ Tạo đỉnh: Double click chuột trái và đặt tên cho đỉnh", u8"+ Create Vertex: Double-clicking the left mouse and set the label for the vertex" };
		static const char* tutorial_show_notice[] = { u8"	Chú ý: TÊN ĐỈNH KHÔNG ĐƯỢC PHÉP TRÙNG NHAU", u8"	Notice: The label of Vertex must not be duplicated" };
		static const char* tutorial_show2[] = { u8"+ Tạo cung: Giữ Shift trái và kéo chuột từ đỉnh nguồn đến đỉnh đích", u8"+ Create Edge: Keep the left shift button and drag the left mouse button from vertex source to vertex destination" };
		static const char* tutorial_show3[] = { u8"+ Xóa đỉnh/cung: Double click chuột phải vào đỉnh/cung cần xóa", u8"+ Delete Vertex/Edge: Double-clicking the right mouse on the vertex/edge to be deleted" };
		static const char* tutorial_show4[] = { u8"+ Di chuyển đỉnh/cung: Giữ chuột trái vào đỉnh/cung và kéo đến vị trí mong muốn", u8"+ Move Vertex/Edge: Keep the left mouse button on the vertex/edge and drag it to the desired position" };
		static const char* tutorial_show5[] = { u8"+ Đổi trọng số chung: Double chuột trái vào trọng số của cung", u8"+ Change weight of edge: Double-clicking the left mouse on the weight of the edge" };
		static const char* tutorial_show6[] = { u8"+ Di chuyển bảng: Giữ chuột phải và kéo", u8"+ Move canvas: Keep the right mouse button and drag" };
	}

	namespace calc_deg_log_string {
		static const wchar_t* string1[] = { L"- Loại đồ thị: Vô hướng\n" ,L"- Type of Graph: Undirected\n", L"- Loại đồ thị: Có hướng\n" ,L"- Type of Graph: Directed\n" };
		static const wchar_t* string2[] = { L"	+ Bậc của đỉnh " , L"	+ Degree of Vertex " };
		static const wchar_t* string3[] = { L"	+ Đỉnh: " , L"	+ Vertex: " };
		static const wchar_t* string4[] = { L"		1.Bậc vào: ", L"		1.Degree in: " , L"\n		2.Bậc ra: ", L"\n		2.Degree out: ", L"\n		3.Tổng bậc: ", L"\n		3.Total degree: " };
	}

	namespace graph_algo {
		static const wchar_t* search_string[] = { L"BFS (bắt đầu từ đỉnh ", L"BFS (start from vertex ",  L"DFS (Ngăn xếp, bắt đầu từ đỉnh ",  L"DFS (Stack, start from vertex ",  L"DFS (Đệ quy, bắt đầu từ đỉnh ",  L"DFS (Recursive, start from vertex " };
		static const wchar_t* connect_string[] = { L"Đồ thị liên thông", L"Connected Graph", L"Đồ thị không liên thông và có " , L"Disconnected graph and has ",L" bộ phận liên thông", L" connected components" };
		static const wchar_t* strong_connect_string[] = { L"Đồ thị liên thông mạnh", L"Strong Connected Graph", L"Đồ thị không liên thông và có " , L"Disconnected graph and has ",L" bộ phận liên thông mạnh", L" strong connected components" };
		static const wchar_t* cycle_string[] = { L"Đồ thị chứa chu trình", L"Graph has cycle", L"Đồ thị không chứa chu trình", L"Graph hasn't cycle"};
		static const wchar_t* topo_string[] = { L"Các thứ tự topo:\n", L"Topological sorts:\n" };
		static const wchar_t* rank_string[] = { L"Hạng của các đỉnh:\n", L"Rank of vertexs:\n" };
	}

	namespace graph_process {
		static const wchar_t* add_vertex_label[] = { L"Tên đỉnh không được để trống và không\ntrùng với bất kì tên đỉnh nào đã được tạo", L"The vertex name can't be empty and\ncan't match any already created vertex name" };
	}
	namespace graph_network {
		static const wchar_t* cut[] = { L"Lát cắt hẹp nhất (S,T)\n - S = {" , L"Minimum cut (S,T)\n - S = {" };
	}
}