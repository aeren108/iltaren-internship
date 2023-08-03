#pragma once

template <class T> 
struct Node {
	Node();
	T item;
	Node* next;
};

template <class T>
Node<T>::Node() {
	next = nullptr;
}

template <class T>
class LinkedList {
	public :
		LinkedList();
		LinkedList(const LinkedList& list);

		void push_back(const T& item);
		void push_front(const T& item);
		void insert(const T& item, int pos);
		void remove(const T& item);
		T* find(const T& item);
		T* find(int index);
		void print();

		void operator=(const LinkedList& rhs);
		friend std::ostream& operator<<(std::ostream& os, const LinkedList<T>& rhs);

		~LinkedList();

	private :
		int size;
		Node<T> *head;
		
};

template <class T>
LinkedList<T>::LinkedList() {
	head = nullptr;
	size = 0;
}

template <class T>
void LinkedList<T>::push_back(const T& item) {
	if (head == nullptr) {
		head = new Node<T>();
		head->item = item;
		head->next = nullptr;
		size++;
		
		return;
	}

	Node<T>* cur = head;
	while (cur->next != nullptr) {
		cur = cur->next;
	}

	Node<T> *newnode = new Node<T>();
	newnode->item = item;
	newnode->next = nullptr;

	cur->next = newnode;
	size++;
}

template <class T>
void LinkedList<T>::push_front(const T& item) {
	if (head == nullptr) {
		head = new Node<T>();
		head->item = item;
		head->next = nullptr;
		size++;

		return;
	}

	Node<T> *newnode = new Node<T>();
	newnode->item = item;
	newnode->next = head;

	head = newnode;
	size++;
}

template <class T>
void LinkedList<T>::insert(const T& item, int pos) {
	if (pos >= size || pos < 0) throw - 1;
	if (pos == 0) return push_front(item);
	if (pos == size) return push_back(item);
	Node<T> *cur = head;
	Node<T> *prev = nullptr;

	for (int i = 0; i < pos; prev = cur, cur = cur->next, ++i);
	
	Node<T>* newnode = new Node<T>();
	newnode->item = item;
	newnode->next = cur;
	prev->next = newnode;
	size++;
}

template <class T>
void LinkedList<T>::remove(const T& item) {
	Node<T> *cur = head;
	Node<T> *prev = nullptr;

	if (head != nullptr && head->item == item) {
		Node<T> *temp = head;
		head = head->next;
		delete temp;
		return;
	}

	for (; cur != nullptr && cur->item != item; prev = cur, cur = cur->next);
	if (cur->item != item) return;

	prev->next = cur->next;
	delete cur;
	size--;
}

template <class T> 
T* LinkedList<T>::find(const T& item) {
	Node<T>* cur = head;
	while (cur != nullptr) {
		if (cur->item == item) return &item;
		cur = cur->next;
	}

	return nullptr;
}

template <class T>
T* LinkedList<T>::find(int index) {
	if (head == nullptr || index >= size) return nullptr;
	Node<T>* cur = head;

	for (int i = 0; i < index; cur = cur->next, ++i);
	return cur;
}

template <class T>
void LinkedList<T>::print() {
	Node<T>* cur = head;
	while (cur->next != nullptr) {
		std::cout << cur->item << " -> ";
		cur = cur->next;
	}
	std::cout << cur->item;
	std::cout << std::endl;
}

template <class T>
LinkedList<T>::~LinkedList() {
	Node<T>* cur = head;
	Node<T>* temp;
	while (cur != nullptr) {
		temp = cur->next;
		delete cur;
		cur = temp;
	}
	head = nullptr;
	size = 0;
}