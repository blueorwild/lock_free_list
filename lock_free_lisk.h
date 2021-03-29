#ifndef _LOCK_FREE_LIST_
#define _LOCK_FREE_LIST_
#include <atomic>
#include <iostream>

using namespace std;


class Node {
public:
	int value_;
	atomic<Node*> next_;

	Node() {}
	Node(int v, Node* next) : value_(v), next_(next) {}
	~Node() {}

	// 标记节点逻辑删除
	void mark();
	// 节点是否被逻辑删除
	bool is_mark();
	// 获取next_节点，包括被逻辑删除的
	Node* get_next();
};


/*
* In concurrent mode, multithreads call rm() function,
* some threads will just do delete logically, **so
* after all threads finish, there maybe some logical
* deleted nodes in the list.** For example:
*   Head->A->B->C->D->NULL
*   timeline 1: thread[0] marks B as deleted
*   timeline 2: thread[1] marks A as deleted
*   timeline 3: thread[0] finds A has been deleted,
*               it returns without delete B physically
*   timeline 4: thread[1] delete A physically
*   After thread[0] and thread[1] finished, the list is:
*   Head->B(logical deleted)->C->D->NULL
* So in any time, the list may contains logical deleted
* nodes.
*/

class LockFreeList {
private:
	Node* head_; // head 是空节点，head->next有效
public:
	LockFreeList();
	~LockFreeList();

	// 添加一个元素
	bool add(int v);
	// 删除一个元素
	bool del(int v);
	// 是否包含某个元素
	bool contains(int v);
};


#endif // !_LOCK_FREE_LIST_
