// MyMeshApp.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <omp.h>

#include "Mesh\Mesh.h"
#include "Example\Cooling_Simulation\CoolingSimulation.h"
#include "Example\Steiner_Tree\SteinerTree.h"

using namespace std;

int main()
{

	CoolingSimulation app;
	app.run();
    return 0;
}

