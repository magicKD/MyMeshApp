#pragma once

#include "MeshElement.h"

class MeshEdge : public MeshElement {
public:

	MeshEdge(Attribute * attr = NULL) : MeshElement(attr) {}

	virtual ~MeshEdge(){}

};