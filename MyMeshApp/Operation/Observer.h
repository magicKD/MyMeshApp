#pragma once

#include <string>
#include <list>

using namespace std;

class IObserver;

//观察者模式中，被观察的接口
class ISubject {
public:

	virtual void registerObserver(IObserver * observer) = 0;

	virtual void removeObserver(IObserver * observer) = 0;

	virtual void notifyObserver(const string & msg) = 0;

	virtual ~ISubject(){}
};

//观察者接口
class IObserver {
public:

	virtual void update(const string & msg) = 0;

	//用来记录临时结果
	virtual void update(Mesh * myMesh){}

	virtual ~IObserver(){}
};

//观察者的类
class Subject : public ISubject {
protected:
	list<IObserver*> m_observers;

public:
	virtual void registerObserver(IObserver * observer) {
		m_observers.push_back(observer);
	}

	virtual void removeObserver(IObserver * observer) {
		m_observers.remove(observer);
	}

	virtual void notifyObserver(const string & msg) {
		for (auto it = m_observers.begin(); it != m_observers.end(); ++it) {
			IObserver * p = *it;
			p->update(msg);
		}
	}
	
	virtual void notifyObserver(Mesh * myMesh) {
		for (auto it = m_observers.begin(); it != m_observers.end(); ++it) {
			IObserver * p = *it;
			p->update(myMesh);
		}
	}

	virtual ~Subject(){}
};