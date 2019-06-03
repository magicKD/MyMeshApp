#pragma once

#include "Mesh\Mesh.h"
#include "Operation\Operation.h"
#include "Operation\Log.h"

class MeshApp {
private:
	Mesh * myMesh;
	//多态，使用的是用户的operation
	MeshOperation * myOpt;

	void systemInit() {
		myOpt->registerObserver(new Log());
	}

public:

	MeshApp(Mesh * myMesh, MeshOperation * myOpt)
		:myMesh(myMesh), myOpt(myOpt){
		systemInit();
	}

	void run(int iteration = 1) {
		if (myOpt) {
			this->myOpt->loop(iteration);
		}
	}

	~MeshApp() {
		//if (myMesh) delete[] myMesh;
		//if (myOpt) delete[] myOpt;
	}
};