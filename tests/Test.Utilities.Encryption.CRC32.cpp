/**
	@file
	@brief 对 Utilities::Encryption::SHA1 进行单元测试

	这个文件里面是通过几组函数对 Utilities::Encryption::SHA1 进行功能上的单元测试

	@author iriszero
	@date 2020/2/12
*/

#include <string>
#include <gtest/gtest.h>

#include <Utilities.Encryption.CRC32.h>
#pragma comment(lib,"E15Utilities.lib")

using namespace Utilities::Encryption;
using namespace std;

//测试构造函数
TEST(Utilities_Encryption_CRC32, Constractor)
{
	string str = "123";
	EXPECT_EQ(0, CRC32().Get().HashData);
	EXPECT_EQ(2286445522, CRC32(str).Get().HashData);
	EXPECT_EQ(2286445522, CRC32(str.c_str(), strlen(str.c_str())).Get().HashData);
	EXPECT_EQ(2286445522, CRC32(str.begin(), str.end()).Get().HashData);
}

//测试字符串转换函数
TEST(Utilities_Encryption_CRC32, ConvertToString)
{
	EXPECT_STREQ(static_cast<string>(CRC32().Get()).data(), string("00000000").data());
	EXPECT_STREQ(static_cast<string>(CRC32(string("123")).Get()).data(), string("884863d2").data());
	EXPECT_STREQ(CRC32(string("123")).Get().ToString().data(), string("884863d2").data());
}

//测试逻辑函数
TEST(Utilities_Encryption_CRC32, OperatorEQ)
{
	auto nil = CRC32();
	auto crc32 = CRC32(string("123"));
	EXPECT_EQ(crc32.Get() == crc32.Get(), 1);
	EXPECT_EQ(nil.Get() != crc32.Get(), 1);
	EXPECT_EQ(crc32.Get() == "884863d2", 1);
	EXPECT_EQ(crc32.Get() == "884863D2", 1);
	EXPECT_EQ(crc32.Get() != nil.Get().ToString(), 1);
}

//测试移动构造函数
TEST(Utilities_Encryption_CRC32, MoveConstructor)
{
	CRC32 crc1(string("123"));
	const auto crc2(move(crc1));
	EXPECT_STREQ(crc2.Get().ToString().data(), "884863d2");
	EXPECT_STRNE(crc1.Get().ToString().data(), "884863d2");
}

//测试移动赋值函数
TEST(Utilities_Encryption_CRC32, CopyMovement)
{
	auto crc1 = CRC32(string("123"));
	const auto crc2 = move(crc1);
	EXPECT_STREQ(crc2.Get().ToString().data(), "884863d2");
	EXPECT_STRNE(crc1.Get().ToString().data(), "884863d2");
}

//测试添加数据函数
TEST(Utilities_Encryption_CRC32, AppendData)
{
	auto crc32 = CRC32();
	vector<string> s{ "The quick brown ", "fox jumps over ", "the lazy dog" };
	crc32.core.AppendData(s[0].c_str(), strlen(s[0].c_str()));
	EXPECT_STREQ(crc32.Get().ToString().data(), "c81b2a7c");
	crc32.core.AppendData(s[1]);
	EXPECT_STREQ(crc32.Get().ToString().data(), "88022e8c");
	crc32.core.AppendData(s[2].begin(), s[2].end());
	EXPECT_STREQ(crc32.Get().ToString().data(), "414fa339");
}
