#pragma once

#include "Attribute.h"

/*
MeshElement为所有Mesh元素的基类
*/
class MeshElement {
public:
	Attribute * attr;

	int id;

	MeshElement(Attribute * attr = NULL, int id = 0) 
		: attr(attr), id(id){
		if (attr == NULL) this->attr = new Attribute();
	}

	//虚析构函数
	virtual ~MeshElement() {

	}
};