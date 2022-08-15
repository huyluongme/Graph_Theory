// header.h : include file for standard system include files,
// or project specific include files
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <list>
#include <d3d9.h>
#include <corecrt_math_defines.h>
#include <commdlg.h>
#include <fstream>
using namespace std;

#pragma comment(lib, "d3d9.lib")

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_internal.h"

#include "Graph Theory.h"
#include "Menu.h"
#include "Structs.h"
#include "Graph_Process.h"
#include "Graph_Draw.h"
#include "Math.h"
#include "Graph_Algorithms.h"
#include "language.hpp"
#include "ChuLiu.h"
#include "Network.h"

using namespace language_np;