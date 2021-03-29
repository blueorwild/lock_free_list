#ifndef _COARSE_LOCK_LIST_
#define _COARSE_LOCK_LIST_

#include <mutex>

using namespace std;

struct Node {
	int value_;
	Node* next_;
	Node() {
		this->value_ = 0;
		this->next_ = nullptr;
	}
	Node(int v, Node* next) {
		this->value_ = v;
		this->next_ = next;
	}
};

class CoarseLockList {
private:
	Node* head_; // head �ǿսڵ㣬head->next��Ч
	mutex mutex_;
public:
	CoarseLockList();
	~CoarseLockList();

	// ���һ��Ԫ��
	void add(int v);
	// ɾ��һ��Ԫ��
	void del(int v);
	// �Ƿ����ĳ��Ԫ��
	bool contains(int v);
};



#endif // !_COARSE_LOCK_LIST_
#pragma once
