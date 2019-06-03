#pragma once

#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <cstring>
#include <ctime>

#include "Observer.h"

using namespace std;

class Log : public IObserver {
private:
	FILE * fp;
	string filepath; 

public:
	Log(const string & filepath = "./data/log.txt")
		:filepath(filepath) {
		
	}

	virtual void update(const string & msg) {
		time_t tt = time(NULL);
		tm * t = localtime(&tt);
		fp = fopen(filepath.c_str(), "a+");
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

};