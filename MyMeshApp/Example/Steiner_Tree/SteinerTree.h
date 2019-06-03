#pragma once

#include "../../Mesh/Attribute.h"
#include "../../Mesh/Mesh.h"
#include "../../MeshApp.h"

#include <ctime>
#include <cstdlib>

using namespace std;


class MyNodeAttribute : public Attribute {
public:
	bool isTerminal;
	bool isSelected;

	MyNodeAttribute(bool isTerminal = false, bool isSelected = false)
		: isTerminal(isTerminal), isSelected(false){
	}
};

class MyEdgeAttribute : public Attribute {
public:
	int weight;

	MyEdgeAttribute(int weight = 0) : weight(weight){}
};

const double high_ratio = 0.8;
const double p_high_disapper = 0.6;
const double small_ratio = 0.05;
const double low_ratio = 0.2;

class MySteinerTreeOperation : public MeshOperation {
public:

	// ͨ�� MeshOperation �̳�
	// ����Ԫ���Զ�����ԭ�����ɻ�������
	virtual void updateMeshNode(MeshNode * oldNode, MeshNode * newNode) override {
		((MyNodeAttribute*)(newNode)->attr)->isTerminal = ((MyNodeAttribute*)(oldNode->attr))->isTerminal;
		if (((MyNodeAttribute*)(oldNode->attr))->isTerminal == true) {
			//�����terminal node ��ô���ǲ������޸���ڵ����Ϣ,��ֱ����һ�������ͺ���
			((MyNodeAttribute*)(newNode)->attr)->isSelected = ((MyNodeAttribute*)(oldNode->attr))->isSelected;
			return;
		}
		//���������Χ���̫�࣬�Ϳ�����Ϊ��Դ���㵼��һЩ��ȥ��Ҳ��������Ϊ��Ⱥ�࣬�ֳ�����һЩ
		int selectNum = 0;
		vector<MeshNode*> nodeList = this->myMesh->getNeighborNode(oldNode);
		int neighborNum = nodeList.size();
		for (MeshNode * pnode : nodeList) {
			if (((MyNodeAttribute*)(pnode->attr))->isSelected) selectNum++;
		}
		//���ֻ��һ���ھӣ����Լ�������terminal node��ô��һ����ѡ���� ��������terminal node��ô��һ��ѡ��
		//ѡ��ʱ��һ���Ż�
		if (neighborNum == 1) {
			if (((MyNodeAttribute*)(oldNode->attr))->isTerminal) {
				//������terminal ��ô�Ա��Ǹ���һ��Ҫѡ
				((MyNodeAttribute*)(newNode)->attr)->isSelected = true;
			}
			else { //�����һ����ѡ
				((MyNodeAttribute*)(newNode)->attr)->isSelected = false;
			}
			return;
		}

		double selectedRatio = 1.0 * selectNum / neighborNum;
		//������ת
		//if (selectedRatio < 1.0 * rand() / RAND_MAX) {
		//	((MyNodeAttribute*)(newNode)->attr)->isSelected = !((MyNodeAttribute*)(oldNode)->attr)->isSelected;
		//}
		bool isSelected = ((MyNodeAttribute*)(oldNode)->attr)->isSelected;
		//С�����¼�,����֮�ص���
		if (!isSelected && selectedRatio == 0 && 0.10 > 1.0 * rand() / RAND_MAX) {
			((MyNodeAttribute*)(newNode)->attr)->isSelected = true;
		}//������¼����䵥������
		else if (isSelected && selectedRatio == 0 && 0.80 > 1.0 * rand() / RAND_MAX) {
			((MyNodeAttribute*)(newNode)->attr)->isSelected = false;
		}
		//��Χ̫�ܼ��ˣ�Ҳ�Ϳ�����ʧ��
		else if (isSelected && selectedRatio > 0.8 && 0.80 > 1.0 * rand() / RAND_MAX) {
			((MyNodeAttribute*)(newNode)->attr)->isSelected = false;
		}
		//��Χ�����ڣ�Ҳ�Ϳ��ܵ���
		else if (!isSelected && selectedRatio > 1.0 * rand() / RAND_MAX) {
			((MyNodeAttribute*)(newNode)->attr)->isSelected = true;
		}
	}

	virtual void updateMeshEdge(MeshEdge * oldNode, MeshEdge * newEdge) override {
		// we should do nothing here
	}
};

class SteinerTreeNeighborhood {
private:
	Mesh* myMesh;
	MeshOperation * myOperation;
	MeshApp * myMeshApp;

	int nodeNum, terminalNum, edgeNum;
	
	void init() {
		scanf("%d%d%d", &nodeNum, &terminalNum, &edgeNum);
		myMesh = new Mesh(nodeNum, edgeNum);
		for (int i = 0; i < terminalNum; i++) {
			myMesh->addNode(new MeshNode(new MyNodeAttribute(true, true)));
		}
		for (int i = terminalNum; i < nodeNum; i++) {
			myMesh->addNode(new MeshNode(new MyNodeAttribute(false, false)));
		}
		for (int i = 0; i < edgeNum; i++) {
			int u, v, w;
			scanf("%d%d%d", &u, &v, &w);
			myMesh->addEdge(new MeshEdge(new MyEdgeAttribute(w)), u, v);
		}
		myOperation = new MySteinerTreeOperation();
		myOperation->setMesh(myMesh);

		myMeshApp = new MeshApp(myMesh, myOperation);
	}

public:
	SteinerTreeNeighborhood() {
		srand(time(NULL));
		init();
	}

	string nextState() {
		myMeshApp->run();
		return this->getState();
	}

	void setState(const string& state) {
		for (int i = terminalNum; i < nodeNum; i++) {
			if (state[i - terminalNum] == '1') {
				((MyNodeAttribute*)(myMesh->p_nodeSet->at(i)->attr))->isSelected = true;
				//((NodeAttribute*)myMesh.nodeSet[i]->attr)->isSelected = true;
			}
			else {
				((MyNodeAttribute*)(myMesh->p_nodeSet->at(i)->attr))->isSelected = false;
				//((NodeAttribute*)myMesh.nodeSet[i]->attr)->isSelected = false;
			}
		}
	}

	string getState() {
		string state = "";
		for (int i = terminalNum; i < nodeNum; i++) {
			bool isSelected = ((MyNodeAttribute*)(myMesh->p_nodeSet->at(i)->attr))->isSelected;
			if (isSelected) state.push_back('1');
			else state.push_back('0');
		}
		return state;
	}
};