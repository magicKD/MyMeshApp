#pragma once

#include "Attribute.h"

/*
MeshElementΪ����MeshԪ�صĻ���
*/
class MeshElement {
public:
	Attribute * attr;

	int id;

	MeshElement(Attribute * attr = NULL, int id = 0) 
		: attr(attr), id(id){
		if (attr == NULL) this->attr = new Attribute();
	}

	//����������
	virtual ~MeshElement() {

	}
};