#pragma once

#include <iostream>
#include <cstring>

/*
���Եĳ����࣬�û������Ҫ�Զ������ԵĻ�������Ҫ�ȼ̳��Ը���
*/
class Attribute {
public:
	virtual void update(){}
};

/*
Ĭ�����Ծ���һ��double value
*/
class DefaultAttribute : public Attribute{
public:
	double value;
};