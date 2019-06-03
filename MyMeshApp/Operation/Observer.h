#pragma once

#include <string>
#include <list>

using namespace std;

class IObserver;

//�۲���ģʽ�У����۲�Ľӿ�
class ISubject {
public:

	virtual void registerObserver(IObserver * observer) = 0;

	virtual void removeObserver(IObserver * observer) = 0;

	virtual void notifyObserver(const string & msg) = 0;

	virtual ~ISubject(){}
};

//�۲��߽ӿ�
class IObserver {
public:

	virtual void update(const string & msg) = 0;

	//������¼��ʱ���
	virtual void update(Mesh * myMesh){}

	virtual ~IObserver(){}
};

//�۲��ߵ���
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