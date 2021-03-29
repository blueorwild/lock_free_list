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
	// ������������ӽڵ�ĳ���˼����ά��pred ��cur ����ָ��
	// ����ͨ��ȡָ��ĵ�ַ��ά��һ��cur ����
	mutex_.lock();
	Node* pre = head_, * cur = head_->next_;
	while (cur != nullptr) {
		if (v <= cur->value_) {  // �ҵ�λ��
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
		if (v < cur->value_) {  // ��ǰ����
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