#include <gtest/gtest.h>

#include <Utilities.Encoding.h>
#pragma comment(lib,"Utilities.Encoding.lib")
/// <summary>
/// 测试基本功能
/// </summary>

using namespace Utilities;
using namespace Utilities::Encoding;

TEST(Utilities_Encoding_UTF8, Encoding) 
{
	const auto input = U"苟利国家生死以，岂因祸福避趋之!";
	const auto assump = u8"苟利国家生死以，岂因祸福避趋之!";

	UTF8 codec = UTF8();
	auto res = codec.Encode(input);

	EXPECT_STREQ(reinterpret_cast<const char*>(res.data()), assump);
}