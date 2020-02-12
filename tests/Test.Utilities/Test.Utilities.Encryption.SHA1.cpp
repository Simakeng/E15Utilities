/**
	@file
	@brief 对 Utilities::Encryption::SHA1 进行单元测试
	
	这个文件里面是通过几组函数对 Utilities::Encryption::SHA1 进行功能上的单元测试
	
	@author iriszero
	@date 2020/2/12
*/

#include <string>
#include <gtest/gtest.h>

#include <Utilities.Encryption.h>
#pragma comment(lib,"Utilities.Encryption.lib")

using namespace Utilities::Encryption;
using namespace std;

//测试构造函数和字符串转换函数
TEST(Utilities_Encryption_SHA1, ConstractorAndConvertToString)
{
	string s = "123";
	EXPECT_STREQ("da39a3ee5e6b4b0d3255bfef95601890afd80709", SHA1().Get().ToString().data());
	EXPECT_STREQ("40bd001563085fc35165329ea1ff5c5ecbdbbeef", SHA1(s.c_str(), strlen(s.c_str())).Get().ToString().data());
	EXPECT_STREQ("40bd001563085fc35165329ea1ff5c5ecbdbbeef", SHA1(s).Get().ToString().data());
	EXPECT_STREQ("40bd001563085fc35165329ea1ff5c5ecbdbbeef", SHA1(s.begin(), s.end()).Get().ToString().data());
}

//测试逻辑函数
TEST(Utilities_Encryption_SHA1, OperatorEQ)
{
	SHA1 nil = SHA1();
	SHA1 sha1 = SHA1(string("123"));
	EXPECT_EQ(sha1.Get() == sha1.Get(), 1);
	EXPECT_EQ(nil.Get() != sha1.Get(), 1);
	EXPECT_EQ(sha1.Get() == "40BD001563085FC35165329EA1FF5C5ECBDBBEEF", 1);
	EXPECT_EQ(sha1.Get() == "40bd001563085fc35165329ea1ff5c5ecbdbbeef", 1);
	EXPECT_EQ(sha1.Get() != nil.Get().ToString(), 1);
}

//测试移动构造函数
TEST(Utilities_Encryption_SHA1, MoveConstructor)
{
	SHA1 sha1(string("123"));
	auto sha2(move(sha1));
	EXPECT_STREQ(static_cast<string>(sha2.Get()).data(), "40bd001563085fc35165329ea1ff5c5ecbdbbeef");
	EXPECT_STRNE(static_cast<string>(sha1.Get()).data(), "40bd001563085fc35165329ea1ff5c5ecbdbbeef");
}

//测试移动赋值函数
TEST(Utilities_Encryption_SHA1, CopyMovement)
{
	auto sha1 = SHA1(string("123"));
	auto sha2 = move(sha1);
	EXPECT_STREQ(static_cast<string>(sha2.Get()).data(), "40bd001563085fc35165329ea1ff5c5ecbdbbeef");
	EXPECT_STRNE(static_cast<string>(sha1.Get()).data(), "40bd001563085fc35165329ea1ff5c5ecbdbbeef");
}

//测试添加数据函数
TEST(Utilities_Encryption_SHA1, AppendData)
{
	auto sha1 = SHA1();
	vector<string> s{
		"How razorback-jumping frogs can level six piqued gymnasts!",
		"Cozy lummox gives smart squid who asks for job pen.",
		"The quick brown fox jumps over the lazy dog." };
	sha1.core.AppendData(s[0].c_str(), strlen(s[0].c_str()));
	EXPECT_STREQ(sha1.Get().ToString().data(), "072b4495f81b711396545bb51f9cf1b76f10100c");
	sha1.core.AppendData(s[1]);
	EXPECT_STREQ(sha1.Get().ToString().data(), "9476e7ab2244bca0e536616fea617ba5f3992f02");
	sha1.core.AppendData(s[2].begin(), s[2].end());
	EXPECT_STREQ(sha1.Get().ToString().data(), "43b8056c5fd35ed657b157502b501d271815c76b");
}