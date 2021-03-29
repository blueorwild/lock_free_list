#pragma once
#ifndef _FINE_LOCK_LIST_
#define _FINE_LOCK_LIST_

#include <mutex>
#include <atomic>
#include <stdlib.h>

using namespace std;

class MyMutex {
public:
	MyMutex() {}
	virtual ~MyMutex(){}

	virtual void lock() = 0;
	virtual void unlock() = 0;
};

// ����������
class Mutex : public MyMutex {
private:
	mutex mutex_;
public:
	Mutex() {} 
	virtual ~Mutex() {}

	virtual void lock() {
		mutex_.lock();
	}
	virtual void unlock() {
		mutex_.unlock();
	}
};
// ����������
class SpinLock : public MyMutex {
private:
	atomic_flag flag_ ;
public:
	SpinLock() {
		flag_.clear();
	}
	virtual ~SpinLock() {}

	virtual void lock() {
		while (flag_.test_and_set(memory_order_acquire)) {
			_sleep(0);  // ��һ��cpuʱ�䣬��Ȼ�ھ�������
		}
	}
	virtual void unlock() {
		flag_.clear(memory_order_release);
	}
};


class Node {
public:
	int value_;
	Node* next_;
	MyMutex* mutex_;
public:
	Node() {}
	// mutex_type 1 �������� 0 ������
	Node(int v, Node* next, int mutex_type);
	~Node();

	void lock();
	void unlock();
};

class FineLockList {
private:
	Node* head_;  // head �ǿսڵ㣬head->next��Ч

public:
	FineLockList();
	~FineLockList();

	// ���һ��Ԫ��
	void add(int v);
	// ɾ��һ��Ԫ��
	void del(int v);
	// �Ƿ����ĳ��Ԫ��
	bool contains(int v);
};



#endif // !
#pragma once
