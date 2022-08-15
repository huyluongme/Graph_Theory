#pragma once

#include "framework.h"

#define Dark 0
#define Light 1

class cMenu
{
public:
	cMenu() {
		dark = true;
		light = false;
		opt_enable_grid = true;
		bShowTutorial = false;
		scrolling = ImVec2(0.0f, 0.0f);
		canvas_rect = IM_COL32(255, 255, 255, 255);
		canvas_rectfill = IM_COL32(36, 36, 36, 255);
		grid = IM_COL32(200, 200, 200, 40);
		bLanguage_EN = false;
		bLanguage_VN = true;
		i_lang_selected = 0;
	}

	ImU32 canvas_rect, canvas_rectfill, grid;
	int i_lang_selected;

	void	DrawGUI(LPDIRECT3DDEVICE9 pDevice);
	void	InitializeImGui(LPDIRECT3DDEVICE9 pDevice, HWND hwnd);
	void	MessageBoxGUI(string mess);
	void	ToolTip(const char* desc);
	void	HelpMarker(const char* desc);
	string	utf8_encode(const std::wstring& wstr);
	wstring	utf8_decode(const std::string& str);

private:
	bool dark, light;
	bool opt_enable_grid;
	ImVec2 scrolling;
	bool bShowTutorial;
	bool bLanguage_EN, bLanguage_VN;

	void	InitializeImGuiStartFrame(LPDIRECT3DDEVICE9 pDevice);
	void	InitializeImGuiEndFrame(LPDIRECT3DDEVICE9 pDevice);
	void	SetTheme(int theme, ImGuiStyle* dst = NULL);
	void	ResetTool();

	wstring openfile();
	wstring savefile();
	void	OpenGraph();
	void	SaveGraph();
	void	DeleteGraph();

	void	ShowTutorial();

	void	ShowDegResult(ImVec2 p1, ImVec2 p2);
	
	void	ShowBFS(ImVec2 p1, ImVec2 p2);
	void	ShowDFSStack(ImVec2 p1, ImVec2 p2);
	void	ShowDFSRecursive(ImVec2 p1, ImVec2 p2);
	
	void	ShowConnect(ImVec2 p1, ImVec2 p2);
	void	ShowCycle(ImVec2 p1, ImVec2 p2);
	void	ShowTopoSort(ImVec2 p1, ImVec2 p2);
	void	ShowRank(ImVec2 p1, ImVec2 p2);
	
	void	ShowShortestPath_Dijkstra(ImVec2 p1, ImVec2 p2);
	void	ShowShortestPath_BellmanFord(ImVec2 p1, ImVec2 p2);
	
	void	ShowKruskal(ImVec2 p1, ImVec2 p2);
	void	ShowPrim(ImVec2 p1, ImVec2 p2);
	void	ShowChuLiu(ImVec2 p1, ImVec2 p2);

	void	ShowNetwork(ImVec2 p1, ImVec2 p2);
};

extern cMenu* Menu;