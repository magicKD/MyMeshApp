#pragma once

#include <iostream>
#include <cstring>

/*
属性的抽象类，用户如果需要自定义属性的话，必须要先继承自该类
*/
class Attribute {
public:
	virtual void update(){}
};

/*
默认属性就是一个double value
*/
class DefaultAttribute : public Attribute{
public:
	double value;
};