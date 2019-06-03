#pragma once

#include "Attribute.h"
#include "MeshEdge.h"
#include "MeshNode.h"

#include <vector>
#include <iostream>
#include <map>
#include <unordered_map>
#include <string>

using namespace std;

class Mesh {
private:

	vector<MeshNode*> nodeSet; //��Žڵ�ļ���

	vector<MeshEdge*> edgeSet; //��űߵļ���
	
	//buffer
	vector<MeshNode*> buffer_nodeSet;
	vector<MeshEdge*> buffer_edgeSet;

	//unordered_map<int, int> node2edge; //node id map to edge id
	vector<vector<int> > node2edge; //node id to vector of edge id, �����ڽӱ�Ĵ洢

	unordered_map<int, pair<int, int> > edge2node; //edge id map to pair of node id

public:

	int nodeNum;
	int edgeNum;

	vector<MeshNode*> * p_nodeSet;

	vector<MeshEdge*> * p_edgeSet;

	//use as buffer 
	vector<MeshNode*> * p_buffer_nodeSet;

	//use as buffer
	vector<MeshEdge*> * p_buffer_edgeSet;

	Mesh(int nodeNum, int edgeNum)
	: nodeNum(nodeNum), edgeNum(edgeNum){
		this->p_nodeSet = &this->nodeSet;
		this->p_edgeSet = &this->edgeSet;
		//��ʼ���������Ĵ�С
		for (int i = 0; i < nodeNum; i++) buffer_nodeSet.push_back(new MeshNode());
		for (int i = 0; i < edgeNum; i++) buffer_edgeSet.push_back(new MeshEdge());
		//buffer_nodeSet.resize(nodeNum);
		//buffer_edgeSet.resize(edgeNum);
		//���û�����ָ��
		this->p_buffer_nodeSet = &this->buffer_nodeSet;
		this->p_buffer_edgeSet = &this->buffer_edgeSet;
		//��ʼ��node2edge
		vector<int> v;
		for (int i = 0; i < nodeNum; i++) {
			this->node2edge.push_back(v);
		}
	}

	void addNode(MeshNode * node) {
		nodeSet.push_back(node);
		node->id = nodeSet.size() - 1;
	}

	//add edge and its end point
	void addEdge(MeshEdge * edge, int u, int v) {
		edgeSet.push_back(edge);
		edge->id = edgeSet.size() - 1;
		int edgeId = edgeSet.size() - 1;
		//node2edge[u] = node2edge[v] = edgeId;
		node2edge[u].push_back(edgeId);
		node2edge[v].push_back(edgeId);
		edge2node[edgeId] = make_pair(u, v);
	}

	MeshNode* getMeshNode(int id) {
		return (*p_nodeSet)[id];
	}

	MeshEdge* getMeshEdge(int id) {
		return (*p_edgeSet)[id];
	}

	vector<MeshNode*> getNeighborNode(MeshNode * node) {
		vector<MeshNode*> ret;
		if (node == NULL) return ret;
		int u = node->id;
		for (int i = 0; i < node2edge[u].size(); i++) {
			int edgeId = node2edge[u][i];
			pair<int, int> nodepair = edge2node[edgeId];
			int v = 0;
			if (nodepair.first != u) v = nodepair.first;
			else v = nodepair.second;
			ret.push_back((*p_nodeSet)[v]);
		}
		return ret;
	}

	string toString() {
		string str = to_string(nodeNum) + " " + to_string(edgeNum) + "\n";
		//�ߵ����˽ṹ
		for (auto p : edge2node) {
			str += to_string(p.second.first) + " " + to_string(p.second.second) + "\n";
		}
		return str;
	}

	void swapWithBuffer() {
		swap(this->p_nodeSet, this->p_buffer_nodeSet);
		swap(this->p_edgeSet, this->p_buffer_edgeSet);
	}
};