#include <gtest/gtest.h>
#include "vector.h"

class VectorTest : public ::testing::Test {
	protected:
		void SetUp() override {
			v0.push_back(12);
			v0.push_front(10);

			v2.push_front(10);
			v2.push_front(16);
			v2.push_front(21);
			v2.push_front(78);
		}

		Vector<int, 5> v0;
		Vector<int, 1> v1;
		Vector<int, 24> v2;
};

TEST_F(VectorTest, PushFull) {
	v1.push_back(15);
	EXPECT_EQ(1, v1.size());
	v1.push_front(12);
	EXPECT_EQ(1, v1.size());
}

TEST_F(VectorTest, RemoveEmpty) {
	EXPECT_EQ(0, v1.size());
	v1.remove(0);
	EXPECT_EQ(0, v1.size());
}

TEST_F(VectorTest, Insert) {
	v0.insert(45, 0);
	EXPECT_EQ(45, v0[0]);
	v0.insert(56, v0.size());
	EXPECT_EQ(56, v0[v0.size()-1]);
	v0.insert(96, 2);

	ASSERT_EQ(5, v0.size()) << "Size does not match" << std::endl;
	EXPECT_EQ(45, v0[0]) << "0th element does not match" << std::endl;
	EXPECT_EQ(10, v0[1]) << "1th element does not match" << std::endl;
	EXPECT_EQ(96, v0[2]) << "2th element does not match" << std::endl;
	EXPECT_EQ(12, v0[3]) << "3th element does not match" << std::endl;
	EXPECT_EQ(56, v0[4]) << "4th element does not match" << std::endl;
}

TEST_F(VectorTest, RemoveAndPush) {
	v2.remove(0);
	//21 16 10
	v2.remove(1);
	//21 10
	v2.remove(1);
	//21
	v2.push_front(45);
	//45 21
	v2.push_back(89);
	//45 21 89

	EXPECT_EQ(45, v2[0]) << "0th element does not match" << std::endl;
	EXPECT_EQ(21, v2[1]) << "1th element does not match" << std::endl;
	EXPECT_EQ(89, v2[2]) << "2th element does not match" << std::endl;


}
