/**
 @file
 @brief 对 Utilities::GUID 进行单元测试

 这个文件里面是通过几组函数对 Utilities::GUID 进行功能上的单元测试

 @author 司马坑
 @date 2020/2/10
*/

#include <string>
#include <gtest/gtest.h>

#include <Utilities.GUID.h>
#pragma comment(lib,"Utilities.GUID.lib")

using namespace Utilities;
using namespace std;

//! 测试字符串转换函数
TEST(Utilities_GUID, ConvertToString)
{
	auto s = GUID::Nil.ToString();
	EXPECT_STREQ("00000000-0000-0000-0000-000000000000", s.data());
	s = GUID::Nil;
	EXPECT_STREQ("00000000-0000-0000-0000-000000000000", s.data());
}

//! 测试构造函数
TEST(Utilities_GUID, CopyingConstractor)
{
	string s(GUID::Nil);
	EXPECT_STREQ(s.data(), "00000000-0000-0000-0000-000000000000");
}

//! 测试以字符串数组为参数进行初始化的构造函数
TEST(Utilities_GUID, InitializeByCharArray)
{
	const auto str = "ea58b776-ea19-4b28-a804-0bdb1391796a";
	GUID guid(str);
	string s(guid);
	EXPECT_STREQ(s.data(), str);
}

//! 测试以std::string为参数的构造函数
TEST(Utilities_GUID, InitializeByString)
{
	const string str = "84edf704-1688-4826-987d-871897a1003b";
	GUID guid(str);
	string s(guid);
	EXPECT_STREQ(s.data(), str.data());
}

//! 测试移动构造函数
TEST(Utilities_GUID, MoveConstructor)
{
	const auto str = "76a3fabe-4572-466a-b3d0-d223c3b2dbe8";
	GUID guid = str;
	GUID movedGuid(std::move(guid));
	EXPECT_STREQ(movedGuid.ToString().data(), str);
	EXPECT_STRNE(guid.ToString().data(), str);
}

//! 测试赋值函数
TEST(Utilities_GUID, CopyAssigine)
{
	const auto str = "44364147-f9ba-40be-bb7c-881bc0a6ce2c";
	GUID guid(GUID::Nil);
	GUID rhsGuid(str);
	guid = rhsGuid;
	EXPECT_STREQ(guid.ToString().data(), rhsGuid.ToString().data());
	EXPECT_STREQ(str, guid.ToString().data());
}

//! 测试以std::string 为参数的赋值函数
TEST(Utilities_GUID, CopyAssigineWithString)
{
	const auto str = "6d206919-a7bb-4e45-bdf2-bbe9014ab00f";
	GUID guid(GUID::Nil);
	guid = std::string(str);
	EXPECT_STREQ(guid.ToString().data(), str);
}

//! 测试移动赋值函数
TEST(Utilities_GUID, CopyMovement)
{
	const auto str = "269a64ca-579b-489f-b07f-6bb7df4d6f9a";
	GUID moveGuid = str;
	GUID guid = GUID::Nil;
	guid = std::move(moveGuid);

	EXPECT_STREQ(guid.ToString().data(), str);
	EXPECT_STRNE(moveGuid.ToString().data(), str);
}

//! 测试逻辑函数
TEST(Utilities_GUID, OperatorEQ)
{
	const auto str = "44364147-f9ba-40be-bb7c-881bc0a6ce2c";
	GUID guid(GUID::Nil);
	GUID rhsGuid(str);
	guid = rhsGuid;
	EXPECT_EQ(guid, rhsGuid);
	EXPECT_STREQ(guid.ToString().data(), str);
	EXPECT_STREQ(rhsGuid.ToString().data(), str);
}

