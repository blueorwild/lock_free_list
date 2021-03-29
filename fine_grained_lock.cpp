#include "fine_grained_lock.h"

int MUTEX_TYPE = 0;
Node::Node(int v, Node* next, int mutex_type) {
	value_ = v;
	next_ = next;
	if (mutex_type == 1) {
		mutex_ = new SpinLock();
	}
	else {
		mutex_ = new Mutex();
	}
}
Node::~Node() {
	delete mutex_;
}

void Node::lock() {
	mutex_->lock();
}
void Node::unlock() {
	mutex_->unlock();
}


FineLockList::FineLockList() {
	head_ = new Node(0, nullptr, MUTEX_TYPE);
}

FineLockList::~FineLockList() {
	Node* cur = head_;
	while (cur != nullptr) {
		Node* next = cur->next_;
		delete cur;
		cur = next;
	}
}

void FineLockList::add(int v) {
	Node* pre = head_;
	pre->lock();
	while (pre->next_ != nullptr) {
		Node* cur = pre->next_;
		cur->lock();
		if (cur->value_ >= v) {
			Node* newNode = new Node(v, cur, MUTEX_TYPE);
			pre->next_ = newNode;
			cur->unlock();
			pre->unlock();
			return;
		}
		pre->unlock();
		pre = cur;
	}
	Node* newNode = new Node(v, nullptr, MUTEX_TYPE);
	pre->next_ = newNode;
	pre->unlock();
}

void FineLockList::del(int v) {
	Node* pre = head_;
	pre->lock();
	while (pre->next_ != nullptr) {
		Node* cur = pre->next_;
		cur->lock();
		if (v < cur->value_) {   // 提前结束
			cur->unlock();
			pre->unlock();
			return;
		}
		if (v == cur->value_) {
			pre->next_ = cur->next_;
			cur->unlock();
			delete cur;
			pre->unlock();
			return;
		}
		pre->unlock();
		pre = cur;
	}
	pre->unlock();
}

bool FineLockList::contains(int v) {
	Node* pre = head_;
	pre->lock();
	while (pre->next_ != nullptr) {
		Node* cur = pre->next_;
		cur->lock();
		if (v < cur->value_) {  // 提前结束
			cur->unlock();
			pre->unlock();
			return false;
		}
		if (v == cur->value_) {
			cur->unlock();
			pre->unlock();
			return true;
		}
		pre->unlock();
		pre = cur;
	}
	pre->unlock();
	return false;
}