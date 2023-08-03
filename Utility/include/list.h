#pragma once

template <class T>
struct ListNode {
	ListNode();
	T item;
	int next;
};

template <class T>
ListNode<T>::ListNode() {
	next = -1;
}

template <class T, int CAPACITY>
class List {
	public:
		List();

		bool push_back(const T& item);
		bool push_front(const T& item);
		bool insert(const T& item, int pos);
		bool remove(const T& item);
		bool remove_at(int pos);
		bool empty();
		int size();
		T& operator[](int pos);
		T& find(const T& item);

		void operator=(List<T, CAPACITY>& rhs);

		friend std::ostream& operator<<(std::ostream& os, const List<T, CAPACITY>& rhs) {
			int cur = rhs.head;
			if (cur == -1) return os;
			for (; rhs.arr[cur].next != -1; cur = rhs.arr[cur].next) {
				os << rhs.arr[cur].item << " -> ";
			}
			os << rhs.arr[cur].item;
			return os;
		}

		const int capacity = CAPACITY;

	private:
		int allocate();
		bool free(int pos);

		int size_;
		int head, tail;
		int free_head;
		ListNode<T> arr[CAPACITY];
};

template <class T, int C>
List<T, C>::List() {
	size_ = 0;
	head = tail = -1;
	free_head = 0;

	for (int i = 0; i < C; ++i) arr[i].next = i + 1;
	arr[C - 1].next = -1;
}

template <class T, int C>
int List<T, C>::allocate() {
	if (size_ == capacity) return -1;
	int free = free_head;
	free_head = arr[free_head].next;
	return free;
}

template <class T, int C>
bool List<T, C>::free(int pos) {
	if (pos < 0 || pos >= capacity) return false;
	arr[pos].next = free_head;
	free_head = pos;
	return true;
}

template <class T, int C>
bool List<T, C>::push_back(const T& item) {
	if (tail == -1) {
		int free = allocate();
		head = tail = free;
		
		arr[free].next = -1;
		arr[free].item = item;
		size_++;

		return true;
	}

	if (free_head == -1) return false;

	int free = allocate();
	arr[tail].next = free;
	arr[free].next = -1;
	arr[free].item = item;
	tail = free;
	size_++;

	return true;
}

template <class T, int C>
bool List<T, C>::push_front(const T& item) {
	if (head == -1) {
		int free = allocate();
		head = tail = free;

		arr[free].next = -1;
		arr[free].item = item;
		size_++;

		return true;
	}

	if (free_head == -1) return false;

	int free = allocate();
	arr[free].next = head;
	arr[free].item = item;
	head = free;
	size_++;

	return true;
}

template <class T, int C>
bool List<T, C>::insert(const T& item, int pos) {
	if (pos < 0 || pos > capacity) return false;
	if (pos == 0) return push_front(item);
	else if (pos == size_) return push_back(item);

	int cur = head, i = 1;
	for (; i < pos; cur = arr[cur].next, ++i);
	
	int free = allocate();
	arr[free].item = item;
	arr[free].next = arr[cur].next;
	arr[cur].next = free;
	size_++;

	return true;
}

template <class T, int C>
bool List<T, C>::remove(const T& item) {
	if (size_ == 0) return false;
	if (arr[head].item == item) {
		int temp = head;
		head = arr[head].next;  if (head == -1) tail = -1;
		free(temp);
		size_--;
		return true;
	}

	int cur = head, prev = -1;
	for (; cur != -1 && arr[cur].item != item; prev = cur, cur = arr[cur].next);
	if (arr[cur].item != item) return false;
	if (tail == cur) tail = prev; if (tail == -1) head = -1;
	
	arr[prev].next = arr[cur].next;
	free(cur);
	size_--;

	return true;
}

template <class T, int C>
bool List<T, C>::remove_at(int pos) {
	if (pos < 0 || pos >= size_) return false;

	int cur = head, prev = -1;
	for (int i = 0; i < pos; prev = cur, cur = arr[cur].next, ++i);
	if (cur == head) head = arr[cur].next;
	if (tail == cur) tail = prev; if (tail == -1 || head == -1) head = tail = -1;

	arr[prev].next = arr[cur].next;
	free(cur);
	size_--;

	return true;
}

template <class T, int C>
bool List<T, C>::empty() {
	return size_ == 0;
}

template <class T, int C>
int List<T, C>::size() {
	return size_;
}

template <class T, int C>
T& List<T, C>::operator[](int pos) {
	int cur = head;
	for (int i = 0; i < pos; cur = arr[cur].next, ++i);
	return arr[cur].item;
}

template <class T, int C>
void List<T,C>::operator=(List<T, C>& rhs) {
	size_ = rhs.size_;
	head = rhs.head;
	tail = rhs.tail;
	free_head = rhs.free_head;
	
	for (int i = 0; i < C; ++i) {
		arr[i] = rhs.arr[i];
	}
}