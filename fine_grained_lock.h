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

// 基本互斥锁
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
// 基本自旋锁
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
			_sleep(0);  // 让一下cpu时间，仍然在就绪队列
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
	// mutex_type 1 自旋锁， 0 互斥锁
	Node(int v, Node* next, int mutex_type);
	~Node();

	void lock();
	void unlock();
};

class FineLockList {
private:
	Node* head_;  // head 是空节点，head->next有效

public:
	FineLockList();
	~FineLockList();

	// 添加一个元素
	void add(int v);
	// 删除一个元素
	void del(int v);
	// 是否包含某个元素
	bool contains(int v);
};



#endif // !
#pragma once
