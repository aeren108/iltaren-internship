// cmake-deneme.cpp : Defines the entry point for the application.
//

#include "cmake-deneme.h"
#include "list.h"
#include "vector.h"
#include "object_pool.h"

int main() {
	Vector<int, 50> arr;
	arr.push_back(10);
	arr.push_back(25);
	arr.push_back(-56);
	arr.push_back(21);
	arr.push_back(-10);
	arr.insert(41, 4);
	arr.push_front(-5);
	arr.push_front(-1);
	arr.insert(5, 2);
	std::cout << arr << std::endl;
	arr.remove(7);
	
	List<int, 15> l;
	l.push_back(10);
	l.push_front(2);
	l.push_back(56);
	l.insert(-8, 0);
	l.push_front(5);
	l.remove_at(4);
	std::cout << l << std::endl;
	l.remove_at(3);
	std::cout << l << std::endl;
	l.remove_at(1);
	std::cout << l << std::endl;
	l.remove(0);
	std::cout << l << std::endl;

	ObjectPool<int, 12> pool;
	pool.free(&pool.arr[5].item);

	return 0;
}
