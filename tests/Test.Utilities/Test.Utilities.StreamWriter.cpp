/**
 @file
 @brief 对 Utilities::StreamWriter 进行单元测试

 这个文件里面是通过几组函数对 Utilities::StreamWriter 进行功能上的单元测试

 @author 司马坑
 @date 2020/2/12
*/
#define _CRT_SECURE_NO_WARNINGS
#include <gtest/gtest.h>
#include <Utilities.Stream.h>
using namespace Utilities;

TEST(Utilties_StreamWriter, Default)
{
	wchar_t fileName[L_tmpnam];
	_wtmpnam(fileName);

	const auto size = 1024;
	char buf[size] = { 0 };
	FileStream fs = FileStream(fileName, Stream::Type::WriteOnly, false);
	StreamWriter sw = StreamWriter(fs);
	for (auto i = 0; i < size; i++)
	{
		buf[i] ^= rand() & 255;
		sw.Write(buf[i]);
	}
	fs.Close();

	FILE* fp = _wfopen(fileName, L"rb");
	for (auto i = 0; i < size; i++) 
	{
		char t;
		fread(&t, 1, 1, fp);
		EXPECT_EQ(t, buf[i]);
	}
	fclose(fp);

}

TEST(Utilties_StreamWriter, STDIO)
{
	wchar_t fileName[L_tmpnam];
	_wtmpnam(fileName);

	const auto size = 1024;
	char buf[size] = { 0 };
	FILE* fs = _wfopen(fileName, L"wb");
	StreamWriter sw = StreamWriter(fs);
	for (auto i = 0; i < size; i++)
	{
		buf[i] ^= rand() & 255;
		sw.Write(buf[i]);
	}
	fclose(fs);

	FILE* fp = _wfopen(fileName, L"rb");
	for (auto i = 0; i < size; i++)
	{
		char t;
		fread(&t, 1, 1, fp);
		EXPECT_EQ(t, buf[i]);
	}
	fclose(fp);

}