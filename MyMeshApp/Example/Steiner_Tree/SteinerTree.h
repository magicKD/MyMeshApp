#pragma once

#include "../../Mesh/Attribute.h"
#include "../../Mesh/Mesh.h"


class NodeAttribute : public Attribute {
public:
	bool isTerminal;
	bool isSelected;

	NodeAttribute(bool isTerminal = false, bool isSelected = false)
		: isTerminal(isTerminal), isSelected(false){
	}
};

class EdgeAttribute : public Attribute {
public:
	int weight;

	EdgeAttribute(int weight = 0) : weight(weight){}
};

class SteinerTreeNeighborhood {
private:
	Mesh myMesh;

	int nodeNum, terminalNum, edgeNum;
	
	void init() {
		scanf("%d%d%d", &nodeNum, &terminalNum, &edgeNum);
		myMesh.nodeNum = nodeNum;
		myMesh.edgeNum = edgeNum;
		for (int i = 0; i < terminalNum; i++) {
			myMesh.addNode(new MeshNode(new NodeAttribute(true, true)));
		}
		for (int i = terminalNum; i < nodeNum; i++) {
			myMesh.addNode(new MeshNode(new NodeAttribute(false, false)));
		}
		for (int i = 0; i < edgeNum; i++) {
			int u, v, w;
			scanf("%d%d%d", &u, &v, &w);
			myMesh.addEdge(new MeshEdge(new EdgeAttribute(w)), u, v);
		}
	}

public:
	void setState(const string& state) {
		for (int i = terminalNum; i < nodeNum; i++) {
			if (state[i - terminalNum] == '1') {
				//((NodeAttribute*)myMesh.nodeSet[i]->attr)->isSelected = true;
			}
			else {
				//((NodeAttribute*)myMesh.nodeSet[i]->attr)->isSelected = false;
			}
		}
	}
};