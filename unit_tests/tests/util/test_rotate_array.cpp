#include "pch.h"
#include "arrays_util.h"

TEST(Util, RotateArray) {
	{
		// Test case: Rotate by 0
		int arr[] = {1, 2, 3, 4, 5};
		rotateArray(arr, 5, 0);
		EXPECT_EQ(arr[0], 1);
		EXPECT_EQ(arr[1], 2);
		EXPECT_EQ(arr[2], 3);
		EXPECT_EQ(arr[3], 4);
		EXPECT_EQ(arr[4], 5);
	}

	{
		// Test case: Rotate by 1
		// Elements move forward: at i moves to (i+1)%5
		// {1, 2, 3, 4, 5} -> {5, 1, 2, 3, 4}
		int arr[] = {1, 2, 3, 4, 5};
		rotateArray(arr, 5, 1);
		EXPECT_EQ(arr[0], 5);
		EXPECT_EQ(arr[1], 1);
		EXPECT_EQ(arr[2], 2);
		EXPECT_EQ(arr[3], 3);
		EXPECT_EQ(arr[4], 4);
	}

	{
		// Test case: Rotate by size (5)
		int arr[] = {1, 2, 3, 4, 5};
		rotateArray(arr, 5, 5);
		EXPECT_EQ(arr[0], 1);
		EXPECT_EQ(arr[4], 5);
	}

	{
		// Test case: Rotate by -1
		// Elements move backward: at i moves to (i-1)%5
		// {1, 2, 3, 4, 5} -> {2, 3, 4, 5, 1}
		int arr[] = {1, 2, 3, 4, 5};
		rotateArray(arr, 5, -1);
		EXPECT_EQ(arr[0], 2);
		EXPECT_EQ(arr[1], 3);
		EXPECT_EQ(arr[2], 4);
		EXPECT_EQ(arr[3], 5);
		EXPECT_EQ(arr[4], 1);
	}

	{
		// Test case: Rotate by 7 (equivalent to 2)
		// {1, 2, 3, 4, 5} -> {4, 5, 1, 2, 3}
		int arr[] = {1, 2, 3, 4, 5};
		rotateArray(arr, 5, 7);
		EXPECT_EQ(arr[0], 4);
		EXPECT_EQ(arr[1], 5);
		EXPECT_EQ(arr[2], 1);
		EXPECT_EQ(arr[3], 2);
		EXPECT_EQ(arr[4], 3);
	}

	{
		// Test case: float array
		float arr[] = {1.1f, 2.2f, 3.3f};
		rotateArray(arr, 3, 1);
		EXPECT_NEAR(arr[0], 3.3f, 1e-5);
		EXPECT_NEAR(arr[1], 1.1f, 1e-5);
		EXPECT_NEAR(arr[2], 2.2f, 1e-5);
	}

	{
		// Test case: empty array
		int* arr = nullptr;
		rotateArray(arr, 0, 1); // Should not crash
	}

	{
		// Test case: size 1 array
		int arr[] = {42};
		rotateArray(arr, 1, 10);
		EXPECT_EQ(arr[0], 42);
	}
}
