/**
	 @file
	 @brief 通用模板库 Range 对象的单元测试

	 @author 司马坑
	 @date 2020/2/11
*/
#include <string>
#include <vector>
#include <deque>
#include <random>
#include <gtest/gtest.h>
#include <Utilities.Common.h>

#pragma comment(lib,"Utilities.Common.lib")

using namespace std;
using namespace Utilities::Common;

/// <summary>
/// 测试基本功能
/// </summary>
TEST(Utilities_Common_Range, basic)
{
	mt19937 random(time(nullptr));
	int max = random() % 4095 + 1;
	int i = 0;
	for (auto j : Range(0, max))
	{
		EXPECT_EQ(j, i);
		i++;
	}

};

/// <summary>
/// 测试转换为容器对象的功能
/// </summary>
TEST(Utilities_Common_Range, AssigineToContainer_vector)
{
	mt19937 random(time(nullptr));
	int i = random() % 4095 + 1;
	vector<int> v = Range(0, i);
	EXPECT_EQ(v.size(), i);
	for (auto j : Range(0, i))
		EXPECT_EQ(j, v[j]);
}

/// <summary>
/// 测试转换为容器对象的功能
/// </summary>
TEST(Utilities_Common_Range, AssigineToContainer_deque)
{
	mt19937 random(time(nullptr));
	int i = random() % 4095 + 1;
	deque<int> v = Range(0, i);
	EXPECT_EQ(v.size(), i);
	for (auto j : Range(0, i))
		EXPECT_EQ(j, v[j]);
}