#pragma once

#include "MeshElement.h"

class MeshNode : public MeshElement {
public:
	
	MeshNode(Attribute * attr = NULL) : MeshElement(attr) {}

	virtual ~MeshNode() {}
};