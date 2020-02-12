/**
 @file
 @brief 对 Utilities::StreamReader 进行单元测试

 这个文件里面是通过几组函数对 Utilities::StreamReader 进行功能上的单元测试

 @author 司马坑
 @date 2020/2/12
*/
#define _CRT_SECURE_NO_WARNINGS
#include <gtest/gtest.h>
#include <Utilities.Stream.h>
using namespace Utilities;

TEST(Utilties_StreamReader, Default)
{
	wchar_t fileName[L_tmpnam];
	_wtmpnam(fileName);
	FILE* fp = _wfopen(fileName, L"wb");

	const auto size = 1024;
	char buf[size] = { 0 };
	for (auto i = 0; i < size; i++)
	{
		buf[i] ^= rand() & 255;
		fwrite(&buf[i], 1, 1, fp);
	}
	fclose(fp);

	FileStream fs = FileStream(fileName, Stream::Type::ReadOnly,false);
	StreamReader sr = StreamReader(fs);
	for (auto i = 0; i < size; i++) 
		EXPECT_EQ(sr.Read<char>(), buf[i]);
	
}

TEST(Utilties_StreamReader, STDIO)
{
	wchar_t fileName[L_tmpnam];
	_wtmpnam(fileName);
	FILE* fp = _wfopen(fileName, L"wb");

	const auto size = 1024;
	char buf[size] = { 0 };
	for (auto i = 0; i < size; i++)
	{
		buf[i] ^= rand() & 255;
		fwrite(&buf[i], 1, 1, fp);
	}
	fclose(fp);
	fp = _wfopen(fileName, L"rb");

	StreamReader sr = StreamReader(fp);
	for (auto i = 0; i < size; i++)
		EXPECT_EQ(sr.Read<char>(), buf[i]);

}