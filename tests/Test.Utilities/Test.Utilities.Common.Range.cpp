/**
	 @file
	 @brief ͨ��ģ��� Range ����ĵ�Ԫ����

	 @author ˾���
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
/// ���Ի�������
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
/// ����ת��Ϊ��������Ĺ���
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
/// ����ת��Ϊ��������Ĺ���
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