#pragma once

template <class T, int CAPACITY> 
class Vector {
	public:
		Vector();
		
		bool push_back(const T& item);
		bool push_front(const T& item);
		bool insert(const T& item, int pos);
		bool remove(int pos);
		bool empty();
		int size() const;
		T at(int index);
		T& operator[](int pos);
		void operator=(const Vector<T, CAPACITY>& rhs);
		void print();

		friend std::ostream& operator<<(std::ostream& os, const Vector<T, CAPACITY>& obj) {
			for (int i = 0; i < obj.size_; ++i) {
				os << obj.arr[obj.real_index(i)] << " ";
			}
			return os;
		}

		const int capacity = CAPACITY;
		
	private:
		int real_index(int pos) const;
		int real_dist(int begin, int end);
		int fix_index(int index);
		int decrement_index(int& index);
		int increment_index(int& index);

		int size_;
		int front, back;
		T arr[CAPACITY];
};

template <class T, int C>
Vector<T, C>::Vector() {
	size_ = 0;
	front = 0;
	back = 0;
}

template <class T, int C>
bool Vector<T, C>::push_back(const T& item) {
	if (size_ == C) return false;

	arr[empty() ? back % capacity : increment_index(back)] = item;
	size_++;

	return true;
}

template <class T, int C>
bool Vector<T, C>::push_front(const T& item) {
	if (size_ == C) return false;

	arr[empty() ? front % capacity : decrement_index(front)] = item;
	size_++;
	
	return true;
}

template <class T, int C>
bool Vector<T, C>::insert(const T& item, int pos) {
	if (pos > size_ || pos < 0) return false;
	int realpos = real_index(pos);
	int fp_dist = real_dist(front, realpos);
	int pb_dist = real_dist(realpos, back);

	if (pos == 0) return push_front(item);
	else if (pos == size_) return push_back(item);

	if (fp_dist > pb_dist) {
		for (int i = realpos + pb_dist; i >= realpos; --i) {
			int ii = i % capacity;
			arr[increment_index(ii)] = arr[fix_index(i)];
		}
		increment_index(back);
		arr[realpos] = item;
	} else {
		for (int i = realpos - fp_dist; i < realpos; ++i) {
			int ii = i % capacity;
			arr[decrement_index(ii)] = arr[fix_index(i)];
		}
		decrement_index(front);
		arr[decrement_index(realpos)] = item;
	}

	size_++;
	return true;
}

template <class T, int C>
bool Vector<T, C>::remove(int pos) {
	if (pos >= size_ || pos < 0) return false;
	int realpos = real_index(pos);
	int fp_dist = real_dist(front, realpos);
	int pb_dist = real_dist(realpos, back);

	if (fp_dist > pb_dist) {
		for (int i = realpos; i < realpos + pb_dist; ++i) {
			int ii = i % capacity;
			arr[fix_index(ii)] = arr[fix_index(ii+1)];
		}
		decrement_index(back);
	} else {
		for (int i = realpos; i > realpos - fp_dist; --i) {
			int ii = i % capacity;
			arr[fix_index(ii)] = arr[fix_index(ii-1)];
		}
		increment_index(front);
	}

	size_--;
	return true;
}

template <class T, int C>
bool Vector<T, C>::empty() {
	return size_ == 0;
}

template <class T, int C>
int Vector<T, C>::size() const {
	return size_;
}

template <class T, int C>
void Vector<T, C>::operator=(const Vector<T, C>& rhs) {
	for (int i = 0; i < C; ++i) arr[i] = rhs.arr[i];
	size_ = rhs.size_;
	front = rhs.front;
	back = rhs.back;
}

template <class T, int C>
T& Vector<T, C>::operator[](int pos) {
	return arr[real_index(pos)];
}

template <class T, int C>
T Vector<T, C>::at(int pos) {
	if (pos >= size_) throw -1;
	return arr[real_index(pos)];
}

template <class T, int C>
int Vector<T, C>::real_index(int pos) const {
	return (front + pos) % capacity;
}

template <class T, int C>
int Vector<T, C>::real_dist(int begin, int end) {
	if (end >= begin) return end - begin;
	return (capacity - begin) + end;
}

template <class T, int C>
int Vector<T, C>::decrement_index(int& index) {
	if (--index < 0) index = capacity + index;
	return index;
}

template <class T, int C>
int Vector<T, C>::increment_index(int& index) {
	if (++index <= capacity) index %= capacity;
	return index;
}

template <class T, int C>
int Vector<T, C>::fix_index(int index) {
	if (index >= capacity) index %= capacity;
	else if (index < 0) index = capacity + index;
	return index;
}

template <class T, int C>
void Vector<T, C>::print() {
	for (int i = 0; i < size_; ++i) {
		std::cout << arr[real_index(i)] << " ";
	}
}