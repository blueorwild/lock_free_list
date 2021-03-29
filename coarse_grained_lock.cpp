#include "coarse_grained_lock.h"


CoarseLockList::CoarseLockList() {
	this->head_ = new Node();
}

CoarseLockList::~CoarseLockList() {
	Node* cur = head_;
	while (cur) {
		Node* next = cur->next_;
		delete cur;
		cur = next;
	}
}

void CoarseLockList::add(int v) {
	// 有序链表中添加节点的常规思想是维护pred 和cur 两个指针
	// 这里通过取指针的地址，维护一个cur 即可
	mutex_.lock();
	Node* pre = head_, * cur = head_->next_;
	while (cur != nullptr) {
		if (v <= cur->value_) {  // 找到位置
			Node* newNode = new Node(v, cur);
			pre->next_ = newNode;
			mutex_.unlock();
			return;
		}
	}
	Node* newNode = new Node(v, cur);
	pre->next_ = newNode;
	mutex_.unlock();
}

void CoarseLockList::del(int v) {
	mutex_.lock();
	Node* pre = head_, * cur = head_->next_;
	while (cur != nullptr) {
		if (v < cur->value_) {  // 提前结束
			mutex_.unlock();
			return;
		}
		if (v == cur->value_) {
			pre->next_ = cur->next_;
			mutex_.unlock();
			return;
		}
	}
	mutex_.unlock();
}

bool CoarseLockList::contains(int v) {
	mutex_.lock();
	Node* cur = head_->next_;
	while (cur != nullptr && cur->value_ < v) {
		cur = cur->next_;
	}
	bool res = false;
	if (cur != nullptr && cur->value_ == v) {
		res = true;
	}
	mutex_.unlock();
	return res;
}