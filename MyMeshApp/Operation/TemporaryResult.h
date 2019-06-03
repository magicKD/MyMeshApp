#pragma once

#include "Observer.h"
#include "../Mesh/Mesh.h"

#include <cstdio>
#include <ctime>

using namespace std;

//存储临时文件的类
class TemporaryResult : public IObserver{
private:
	FILE * fp;
	string filepath;

public:
	TemporaryResult(const string & str = "./data/temporary_result.txt") {
		this->filepath = str;
	}

	virtual void update(const string & msg) {
		time_t tt = time(NULL);
		tm * t = localtime(&tt);
		//临时结果只需要记录一次即可
		fp = fopen(filepath.c_str(), "w");
		fprintf(fp, "\n%d-%02d-%02d %02d:%02d:%02d:\n",
			t->tm_year + 1900,
			t->tm_mon + 1,
			t->tm_mday,
			t->tm_hour,
			t->tm_min,
			t->tm_sec);
		fprintf(fp, "%s\n", msg.c_str());
		fclose(fp);
	}

	//用来记录临时结果
	virtual void update(Mesh * myMesh) {
		fp = fopen(filepath.c_str(), "a+"); //append在上面的时间之后
		fprintf(fp, "%s", myMesh->toString().c_str());
		fclose(fp);
	}

	virtual ~TemporaryResult() {}
};