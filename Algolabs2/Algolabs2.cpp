// Algolabs2.cpp : Defines the entry point for the application.
//

#include "Algolabs2.h"
#include "Array.h"
#include "string.h"

#include <gtest/gtest.h>

TEST(Algolabs2Tests, AddElement)
{
	Array<int> arr;
	arr.insert(69);
	EXPECT_EQ(arr.size(), 1);
	EXPECT_EQ(arr[0], 69);
}

TEST(Algolabs2Tests, RemoveElement)
{
	Array<int> arr;
	arr.insert(69);
	ASSERT_EQ(arr.size(), 1);
	
	arr.remove(0);
	EXPECT_EQ(arr.size(), 0);
}

class FilledArrayTest : public ::testing::Test 
{
protected:
	void SetUp() override {
		for (int i = 0; i < 10; ++i)
		{
			arr.insert(i);
		}
	}

	Array<int> arr;
};

TEST_F(FilledArrayTest, InsertAtIndexTest)
{
	const int size = arr.size();

	arr.insert(3, 13);

	ASSERT_EQ(arr.size(), size + 1);
	ASSERT_EQ(arr[3], 13);

	for (int i = 0; i < size + 1; ++i)
	{
		if (i < 3)
		{
			EXPECT_EQ(arr[i], i);
		}
		else if (i > 3)
		{
			EXPECT_EQ(arr[i], i - 1);
		}
	}
}

TEST_F(FilledArrayTest, RemoveAtIndexTest)
{
	const int size = arr.size();

	arr.remove(5);

	ASSERT_EQ(arr.size(), size - 1);
	ASSERT_EQ(arr[5], 6);

	for (int i = 0; i < size - 1; ++i)
	{
		if (i < 5)
		{
			EXPECT_EQ(arr[i], i);
		}
		else if (i > 5)
		{
			EXPECT_EQ(arr[i], i + 1);
		}
	}
}

TEST_F(FilledArrayTest, ForwardIteratorTest)
{
	const int size = arr.size();

	int i = 0;
	for (auto it = arr.iterator(); it.hasNext(); it.next(), ++i)
	{
		ASSERT_EQ(it.get(), i);
	}
}

TEST_F(FilledArrayTest, ReverseIteratorTest)
{
	const int size = arr.size();

	int i = 9;
	for (auto it = arr.reverseIterator(); it.hasNext(); it.next(), --i)
	{
		ASSERT_EQ(it.get(), i);
	}
}

TEST_F(FilledArrayTest, SetWithIteratorTest)
{
	const int size = arr.size();

	int i = 0;
	for (auto it = arr.iterator(); it.hasNext(); it.next(), ++i)
	{
		it.set(i + 69);
	}

	i = 0;
	for (auto it = arr.iterator(); it.hasNext(); it.next(), ++i)
	{
		ASSERT_EQ(it.get(), i + 69);
	}
}

TEST_F(FilledArrayTest, CopyConstructorTest)
{
	Array<int> copy = arr;

	EXPECT_EQ(arr.size(), copy.size());

	for (int i = 0; i < arr.size(); ++i)
	{
		EXPECT_NE(&arr[i], &copy[i]);
	}
}

TEST(Algolabs2Tests, MoveConstructorTest) 
{
	Array<int> arr;

	arr.insert(0);
	arr.insert(1);

	int* const addr = &arr[0];
	const int size = arr.size();

	Array<int> marr = std::move(arr);

	EXPECT_EQ(&marr[0], addr);
	EXPECT_EQ(marr.size(), size);
}

TEST(Algolabs2Tests, ResizeTest) 
{
	// just add elements over the default capacity and make sure there's no memory exceptions
	Array<int> arr;

	for (int i = 0; i <= Array<int>::kDefaultCapacity; ++i)
	{
		arr.insert(i);
	}

	for (int i = 0; i <= Array<int>::kDefaultCapacity; ++i)
	{
		EXPECT_EQ(arr[i], i);
	}
}

TEST(Algolabs2Tests, StringTest)
{
	// just add elements over the default capacity and make sure there's no memory exceptions
	Array<std::string> arr;

	arr.insert("hello");

	for (int i = 0; i <= Array<int>::kDefaultCapacity; ++i)
	{
		arr.insert(std::to_string(i));
	}

}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
