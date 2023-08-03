#pragma once
template <class T>
struct Node {
	Node() : next(-1) {}

	T item;
	int next;
	int prev;
};

template <class T, int CAPACITY>
class ObjectPool {
	public:
		ObjectPool();

		T* allocate();
		bool free(const T* ptr);

		const int capacity;
		Node<T> arr[CAPACITY];
	private:
		
		int free_head, full_head;
		int available;
};

template <class T, int C>
ObjectPool<T, C>::ObjectPool() : capacity(C), free_head(0), full_head(-1), available(C) {
	for (int i = 0; i < C; ++i) {
		arr[i].next = i + 1;
		arr[i].prev = i - 1;
	}
	arr[C - 1].next = -1;
	arr[0].prev = -1;
}

template <class T, int C>
T* ObjectPool<T, C>::allocate() {
	if (free_head == -1) return nullptr;

	int temp = arr[free_head].next;
	arr[free_head].next = full_head;
	full_head = free_head;
	free_head = temp;

	available--;
}

template <class T, int C>
bool ObjectPool<T, C>::free(const T* ptr) {
	int pos = (((long) ptr) - ((long) arr)) / sizeof(Node<T>);
	//TODO: free node at pos'th index

	return true;
}