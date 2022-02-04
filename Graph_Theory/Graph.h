#pragma once

#include "framework.h"

class cGraph
{
public:
    cGraph() {
        start = -1;
    }
    int start;
    void Graph_Init();
    void BFS(string Node);
};

extern cGraph* CGraph;