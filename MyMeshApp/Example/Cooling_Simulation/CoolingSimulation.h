#pragma once

#include "../../MeshApp.h"

#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>

//#define OPENMP_ALLOW

using namespace std;

//�û��Զ�����������
class NodeAttribute : public Attribute {
public:
	double temperature;

	NodeAttribute(double temperature = 0)
		:temperature(temperature){}

	//�̳е��麯��
	virtual void update() {
		this->temperature = temperature * 0.8 - 5;
		if (this->temperature <= 0) temperature = 0;
	}
};

class MyOperation : public MeshOperation {
public:

	// ͨ�� MeshOperation �̳�
	virtual void updateMeshNode(MeshNode * oldNode, MeshNode * newNode) override {
		//ÿ��Ϊ��Χ�¶�ȡƽ����Ȼ����0.8x-5�ķ�ʽ���£���ͽ�Ϊ0
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

	//��ÿ�������ǰ�ڵ���¶�
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
			//ֻҪ��һ������0��˵����û�����꣬�ͼ���
			if (t > 0) return false;
		}
		return true;
	}
};

class CoolingSimulation {
private:
	Mesh * myMesh;
	MeshOperation * myOperation;

	//���ݼ�����Ϣ
	int nodeNum, edgeNum;

public:
	//���������ļ�����mesh
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