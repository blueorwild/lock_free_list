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
	Node* head_; // head 是空节点，head->next有效
	mutex mutex_;
public:
	CoarseLockList();
	~CoarseLockList();

	// 添加一个元素
	void add(int v);
	// 删除一个元素
	void del(int v);
	// 是否包含某个元素
	bool contains(int v);
};



#endif // !_COARSE_LOCK_LIST_
#pragma once
