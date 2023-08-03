#include <gtest/gtest.h>
#include "list.h"

class ListTest : public ::testing::Test {
	protected:
		void SetUp() override {
			l0.push_back(5);
			l0.push_back(12);
			l0.push_back(-2);

			l1.push_front(5);

			l3.push_back("merhaba");
			l3.push_back("dünya");
			l3.push_back("2023");
			l3.push_front("pazartesi");
		}

		List<int, 5> l0;
		List<int, 1> l1;
		List<int, 20> l2;
		List<std::string, 50> l3;
};

TEST_F(ListTest, RemoveEmpty) {
	l2.remove(0);
	EXPECT_EQ(0, l2.size());
	l2.remove_at(0);
	EXPECT_EQ(0, l2.size());
	l2.remove_at(5);
	EXPECT_EQ(0, l2.size());
}

TEST_F(ListTest, PushFull) {
	l1.push_front(45);
	EXPECT_EQ(1, l1.size());
	l1.push_back(47);
	EXPECT_EQ(1, l1.size());
}

TEST_F(ListTest, Insert) {
	l3.insert("tron", 2);
	l3.insert("legacy", 3);

	EXPECT_EQ("tron", l3[2]);
	EXPECT_EQ("legacy", l3[3]);

	l3.insert("deneme", l3.size());
	EXPECT_EQ("deneme", l3[l3.size() - 1]);

	l3.insert("salý", 0);
	EXPECT_EQ("salý", l3[0]);
	
	int oldsize = l3.size();
	l3.insert("22", 89);
	EXPECT_EQ(oldsize, l3.size());
}

TEST_F(ListTest, RemoveAndPush) {
	//5 12 -2
	l0.remove(12);
	//5 -2
	l0.remove(4);
	l0.remove_at(1);
	//5
	l0.remove_at(56);
	l0.push_back(17);
	//5 17

	EXPECT_EQ(2, l0.size());
	EXPECT_EQ(5, l0[0]);
	EXPECT_EQ(17, l0[1]);
}