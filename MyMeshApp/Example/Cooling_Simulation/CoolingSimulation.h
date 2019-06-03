#pragma once

#include "../../MeshApp.h"

#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>

//#define OPENMP_ALLOW

using namespace std;

//用户自定的属性类型
class NodeAttribute : public Attribute {
public:
	double temperature;

	NodeAttribute(double temperature = 0)
		:temperature(temperature){}

	//继承的虚函数
	virtual void update() {
		this->temperature = temperature * 0.8 - 5;
		if (this->temperature <= 0) temperature = 0;
	}
};

class MyOperation : public MeshOperation {
public:

	// 通过 MeshOperation 继承
	virtual void updateMeshNode(MeshNode * oldNode, MeshNode * newNode) override {
		//每次为周围温度取平均，然后以0.8x-5的方式降温，最低降为0
		vector<MeshNode*> nodeList = this->myMesh->getNeighborNode(oldNode);
		double avg = ((NodeAttribute*)(oldNode->attr))->temperature;
		for (MeshNode* pnode : nodeList) {
			avg += ((NodeAttribute*)(pnode->attr))->temperature;
		}
		avg = avg / (nodeList.size() + 1);
		double res = avg * 0.8 - 5;
		if (res <= 0) res = 0;
		((NodeAttribute *)(newNode->attr))->temperature = res;
	}

	virtual void updateMeshEdge(MeshEdge * oldNode, MeshEdge * newEdge) override {
		// we should do nothing here
	}

	//让每次输出当前节点的温度
	virtual void solveTemporaryResult() {
		for (int i = 0; i < myMesh->nodeNum; i++) {
			double t = ((NodeAttribute*)(myMesh->p_nodeSet->at(i)->attr))->temperature;
			cout << t << "\t";
		}
		cout << endl;
	}

	virtual bool bStop() { 
		for (int i = 0; i < myMesh->nodeNum; i++) {
			double t = ((NodeAttribute*)(myMesh->p_nodeSet->at(i)->attr))->temperature;
			//只要有一个大于0，说明还没降温完，就继续
			if (t > 0) return false;
		}
		return true;
	}
};

class CoolingSimulation {
private:
	Mesh * myMesh;
	MeshOperation * myOperation;

	//数据集的信息
	int nodeNum, edgeNum;

public:
	//根据输入文件构造mesh
	void run() {
		freopen("./Example/Cooling_Simulation/example1.txt", "r", stdin);
		cin >> nodeNum >> edgeNum;
		myMesh = new Mesh(nodeNum, edgeNum);

		double temperature;
		for (int i = 0; i < nodeNum; i++) {
			cin >> temperature;
			myMesh->addNode(new MeshNode(new NodeAttribute(temperature)));
		}

		int u, v;
		for (int i = 0; i < edgeNum; i++) {
			cin >> u >> v;
			myMesh->addEdge(new MeshEdge(new Attribute()), u, v);
		}

		myOperation = new MyOperation();
		myOperation->setMesh(myMesh);

		MeshApp meshapp(myMesh, myOperation);
		meshapp.run(10);
	}

};