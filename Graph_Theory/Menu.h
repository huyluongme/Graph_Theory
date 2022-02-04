#pragma once

#include "framework.h"



class cMenu
{
public:
	cMenu() {
		opt_enable_grid = true;
		scrolling = ImVec2(0.0f, 0.0f);
		opt_enable_context_menu = true;
		id_node_selected = -1;
		id_edge_selected = -1;
		node_selected = false;
		edge_selected = false;
		adding_node = false;
		rename_del_node = -1;
		move_node = false;
		move_edge = false;
		adding_edge = false;
	}

	bool opt_enable_grid;
	ImVec2 scrolling;
	bool opt_enable_context_menu;
	int id_node_selected;
	int id_edge_selected;
	bool node_selected;
	bool edge_selected;
	bool adding_node;
	int rename_del_node;
	bool move_node;
	bool move_edge;
	bool adding_edge;

	void DrawMenu(LPDIRECT3DDEVICE9 pDevice);
	void InitializeImGuiStartFrame(LPDIRECT3DDEVICE9 pDevice);
	void InitializeImGuiEndFrame(LPDIRECT3DDEVICE9 pDevice);
	void InitializeImGui(LPDIRECT3DDEVICE9 pDevice, HWND hwnd);
};
extern class cMenu* Menu;