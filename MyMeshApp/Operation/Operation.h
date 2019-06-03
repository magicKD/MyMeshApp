#pragma once

#include "../Mesh/Mesh.h"
#include "Observer.h"
#include <ctime>

#ifdef OPENMP_ALLOW
#include <omp.h>
#endif // OPENMP_ALLOW


class MeshOperation : public Subject {
protected:
	Mesh * myMesh;

public:

	MeshOperation(Mesh * myMesh)
		:myMesh(myMesh) {}

	MeshOperation() {}

	virtual void updateMeshNode(MeshNode * oldNode, MeshNode * newNode) = 0;

	virtual void updateMeshEdge(MeshEdge * oldNode, MeshEdge * newEdge) = 0;

	//对每一步的临时结果的处理，允许用户自定义来debug或者xxx
	virtual void solveTemporaryResult(){} 

	//当bStop返回为true是，终止迭代
	virtual bool bStop() { return false; }

	void setMesh(Mesh* mesh) {
		this->myMesh = mesh;
	}

	//循环遍历次数，默认为1
	void loop(int iterNum = 1) {
		time_t last = time(NULL);
		time_t cur;
		//循环次数
		solveTemporaryResult();
		for (int iter = 0; iter < iterNum; iter++) {
			if (bStop()) break;
			//计时
			cur = time(NULL);
			string msg = "iter " + to_string(iter) + " : cost time : " + to_string((cur - last) * 1000 / CLOCKS_PER_SEC) + " ms.";
			this->notifyObserver(msg); //通知观察者

			//更新节点
#ifdef OPENMP_ALLOW
#pragma omp parallel for
#endif // OPENMP_ALLOW
			for (int i = 0; i < myMesh->nodeNum; i++) {
				this->updateMeshNode((myMesh->p_nodeSet->at(i)), (myMesh->p_buffer_nodeSet->at(i)));
			}
			//更新边
#ifdef OPENMP_ALLOW
#pragma omp parallel for
#endif // OPENMP_ALLOW
			for (int i = 0; i < myMesh->edgeNum; i++) {
				this->updateMeshEdge((myMesh->p_edgeSet->at(i)), (myMesh->p_buffer_edgeSet->at(i)));
			}
			//循环结束
			myMesh->swapWithBuffer();
			last = cur;

			solveTemporaryResult();
		}
	}

	virtual ~MeshOperation(){}
};