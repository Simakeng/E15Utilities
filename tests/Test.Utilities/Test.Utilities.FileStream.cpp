/**
 @file
 @brief 对 Utilities::FileStream 进行单元测试

 这个文件里面是通过几组函数对 Utilities::FileStream 进行功能上的单元测试

 @author 司马坑
 @date 2020/2/12
*/
#define _CRT_SECURE_NO_WARNINGS

#include <string> 

#include <gtest/gtest.h>

#include <Utilities.Stream.h>
#pragma comment(lib,"Utilities.Stream.lib")

using namespace std;
using namespace Utilities;

static_assert(std::is_abstract<Stream>::value, "Stream MUST be a abstract class");

/// <summary>
/// 测试文件流是否会创建文件并打开文件句柄
/// </summary>
TEST(Utilities_FileStream, Creat)
{ 
	wchar_t fileName[L_tmpnam];
	_wtmpnam(fileName);
	FileStream fs = FileStream(fileName, Stream::Type::WriteOnly);
	EXPECT_NE(nullptr, fs.GetHandle());
	
	fs.Close();
	// 判断文件是否存在
	// https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/access-waccess?view=vs-2019
	EXPECT_EQ(_waccess(fileName, 0), 0);
}
/// <summary>
/// 测试文件流的数据读取功能
/// </summary>
TEST(Utilities_FileStream, Read)
{
	wchar_t fileName[L_tmpnam];
	_wtmpnam(fileName);

	srand(time(nullptr));

	const auto size = 1024;
	char buf[size] = { 0 };
	for (auto i = 0; i < size; i++)
		buf[i] ^= rand() & 255;

	FILE* fp = _wfopen(fileName, L"wb");
	fwrite(buf, 1, size, fp);
	fclose(fp);

	auto fs = FileStream(fileName, Stream::Type::ReadOnly, false);
	char cmp[size];
	fs.Read(size, cmp);

	EXPECT_EQ(memcmp(cmp, buf, size), 0);
}
/// <summary>
/// 测试文件流的数据写入功能
/// </summary>
TEST(Utilities_FileStream, Write)
{
	wchar_t fileName[L_tmpnam];
	_wtmpnam(fileName);

	srand(time(nullptr));

	const auto size = 1024;
	char buf[size] = { 0 };
	for (auto i = 0; i < size; i++)
		buf[i] ^= rand() & 255;

	auto fs = FileStream(fileName, Stream::Type::WriteOnly, false);
	fs.Write(size, buf);
	fs.Close();

	char cmp[size];
	FILE* fp = _wfopen(fileName, L"rb");
	fread(cmp, 1, size, fp);

	EXPECT_EQ(memcmp(cmp, buf, size), 0);
}

/// <summary>
/// 测试文件流的文件长度获取功能
/// </summary>
TEST(Utilities_FileStream, Length)
{
	wchar_t fileName[L_tmpnam];
	_wtmpnam(fileName);

	srand(time(nullptr));

	const auto size = 1024;
	char buf[size] = { 0 };
	for (auto i = 0; i < size; i++)
		buf[i] ^= rand() & 255;

	FILE* fp = _wfopen(fileName, L"wb");
	fwrite(buf, 1, size, fp);
	fclose(fp);

	auto fs = FileStream(fileName, Stream::Type::ReadOnly, false);
	EXPECT_EQ(fs.GetLength(), size);	
}