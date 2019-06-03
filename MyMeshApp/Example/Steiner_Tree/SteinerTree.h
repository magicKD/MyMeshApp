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

	// 通过 MeshOperation 继承
	// 类似元胞自动机的原理，生成或者消亡
	virtual void updateMeshNode(MeshNode * oldNode, MeshNode * newNode) override {
		((MyNodeAttribute*)(newNode)->attr)->isTerminal = ((MyNodeAttribute*)(oldNode->attr))->isTerminal;
		if (((MyNodeAttribute*)(oldNode->attr))->isTerminal == true) {
			//如果是terminal node 那么我们不可以修改其节点的信息,就直接做一个拷贝就好了
			((MyNodeAttribute*)(newNode)->attr)->isSelected = ((MyNodeAttribute*)(oldNode->attr))->isSelected;
			return;
		}
		//否则，如果周围活的太多，就可以因为资源不足导致一些死去，也可能是因为人群多，又出生了一些
		int selectNum = 0;
		vector<MeshNode*> nodeList = this->myMesh->getNeighborNode(oldNode);
		int neighborNum = nodeList.size();
		for (MeshNode * pnode : nodeList) {
			if (((MyNodeAttribute*)(pnode->attr))->isSelected) selectNum++;
		}
		//如果只有一个邻居，而自己本身不是terminal node那么就一定不选它， 否则本身是terminal node那么就一定选他
		//选择时的一个优化
		if (neighborNum == 1) {
			if (((MyNodeAttribute*)(oldNode->attr))->isTerminal) {
				//本身是terminal 那么旁边那个就一定要选
				((MyNodeAttribute*)(newNode)->attr)->isSelected = true;
			}
			else { //否则就一定不选
				((MyNodeAttribute*)(newNode)->attr)->isSelected = false;
			}
			return;
		}

		double selectedRatio = 1.0 * selectNum / neighborNum;
		//生死反转
		//if (selectedRatio < 1.0 * rand() / RAND_MAX) {
		//	((MyNodeAttribute*)(newNode)->attr)->isSelected = !((MyNodeAttribute*)(oldNode)->attr)->isSelected;
		//}
		bool isSelected = ((MyNodeAttribute*)(oldNode)->attr)->isSelected;
		//小概率事件,荒芜之地诞生
		if (!isSelected && selectedRatio == 0 && 0.10 > 1.0 * rand() / RAND_MAX) {
			((MyNodeAttribute*)(newNode)->attr)->isSelected = true;
		}//大概率事件，落单，死亡
		else if (isSelected && selectedRatio == 0 && 0.80 > 1.0 * rand() / RAND_MAX) {
			((MyNodeAttribute*)(newNode)->attr)->isSelected = false;
		}
		//周围太密集了，也就可能消失了
		else if (isSelected && selectedRatio > 0.8 && 0.80 > 1.0 * rand() / RAND_MAX) {
			((MyNodeAttribute*)(newNode)->attr)->isSelected = false;
		}
		//周围都存在，也就可能诞生
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