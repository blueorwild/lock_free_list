#include "lock_free_lisk.h"

void Node::mark() {
	next_ = (Node*)((unsigned long)next_.load() | (unsigned long)0x1);  // 地址是4字节倍数
}

bool Node::is_mark() {
	return (unsigned long)next_.load() & (unsigned long)0x1;
}

Node* Node::get_next() {
	return (Node*)((unsigned long)next_.load() & ~(unsigned long)0x1);
}

LockFreeList::LockFreeList() {
	head_ = new Node(-1, nullptr);
}

LockFreeList::~LockFreeList() {
	Node* cur = head_;
	while (cur->get_next() != nullptr) {
		Node* next = cur->get_next();
		delete cur;
		cur = next;
	}
}

bool LockFreeList::add(int v) {
    // insert node between pre and cur->val
    Node* pre = head_;
    Node* cur = head_->next_;

    while (cur != NULL) {
        if (cur->is_mark()) {
            Node* next = cur->get_next();
            // 原子性的比较pre_->next 和 cur 指向的内容是否相同
            // 若相同，用next替换pre_->next的内容， 即真实删除被逻辑删除的节点，返回success
            // 否则，用cur替换pre_->next的内容，即更新pre_->next，返回fail
            if (!atomic_compare_exchange_strong(&pre->next_, &cur, next)) {
                // 如果当前节点和前继节点都被逻辑删除，此次添加已经不安全了，直接返回（失败）。
                if (pre->is_mark()) {
                    return false;
                }
            }
            // 更新cur， 继续
            cur = pre->get_next();
            continue;
        }
        if (cur->value_ >= v) {  // 找到要添加的位置了
            break;
        }
        pre = cur;
        cur = pre->get_next();
    }
    /* 可以添加相同的值
    if (cur != nullptr && val == cur->val) {
        return false;
    }
    */
    Node* node = new Node(v, cur);
    if (!std::atomic_compare_exchange_strong(&pre->next_, &cur, node)) {
        delete node;
        return false;
    }
    return true;
}

bool LockFreeList::del(int v) {
    // delete node with val == cur->val
    Node* pre = head_;
    Node* cur = head_->next_;

    while (cur != NULL) {
        if (cur->is_mark()) {
            Node* next = cur->get_next();
            // 类似 add 的过程
            if (!std::atomic_compare_exchange_strong(&pre->next_, &cur, next)) {
                if (pre->is_mark()) {
                    return false;
                }
            }
            cur = pre->get_next();
            continue;
        }
        if (cur->value_ > v) {
            return false;
        }
        if (cur->value_ == v) {
            break;
        }
        pre = cur;
        cur = pre->get_next();
    }

    if (cur == nullptr) {
        return false;
    }

    // 先标记为逻辑删除
    cur->mark();
    Node* next = cur->get_next();
    // 尝试物理删除， 若成功即成功， 否则是被其它线程删除，也无问题
    atomic_compare_exchange_strong(&pre->next_, &cur, next);
    return true;
}

bool LockFreeList::contains(int v) {
    Node* cur = head_->next_;

    while (cur != NULL) {
        if (cur->is_mark()) {
            cur = cur->get_next();
            continue;
        }
        if (cur->value_ > v) {
            return false;
        }
        if (cur->value_ == v) {
            break;
        }
        cur = cur->get_next();
    }

    if (cur == nullptr) {
        return false;
    }

    return true;
}
