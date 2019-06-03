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

	//��ÿһ������ʱ����Ĵ��������û��Զ�����debug����xxx
	virtual void solveTemporaryResult(){} 

	//��bStop����Ϊtrue�ǣ���ֹ����
	virtual bool bStop() { return false; }

	void setMesh(Mesh* mesh) {
		this->myMesh = mesh;
	}

	//ѭ������������Ĭ��Ϊ1
	void loop(int iterNum = 1) {
		time_t last = time(NULL);
		time_t cur;
		//ѭ������
		solveTemporaryResult();
		for (int iter = 0; iter < iterNum; iter++) {
			if (bStop()) break;
			//��ʱ
			cur = time(NULL);
			string msg = "iter " + to_string(iter) + " : cost time : " + to_string((cur - last) * 1000 / CLOCKS_PER_SEC) + " ms.";
			this->notifyObserver(msg); //֪ͨ�۲���

			//���½ڵ�
#ifdef OPENMP_ALLOW
#pragma omp parallel for
#endif // OPENMP_ALLOW
			for (int i = 0; i < myMesh->nodeNum; i++) {
				this->updateMeshNode((myMesh->p_nodeSet->at(i)), (myMesh->p_buffer_nodeSet->at(i)));
			}
			//���±�
#ifdef OPENMP_ALLOW
#pragma omp parallel for
#endif // OPENMP_ALLOW
			for (int i = 0; i < myMesh->edgeNum; i++) {
				this->updateMeshEdge((myMesh->p_edgeSet->at(i)), (myMesh->p_buffer_edgeSet->at(i)));
			}
			//ѭ������
			myMesh->swapWithBuffer();
			last = cur;

			solveTemporaryResult();
		}
	}

	virtual ~MeshOperation(){}
};