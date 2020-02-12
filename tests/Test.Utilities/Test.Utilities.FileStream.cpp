/**
 @file
 @brief �� Utilities::FileStream ���е�Ԫ����

 ����ļ�������ͨ�����麯���� Utilities::FileStream ���й����ϵĵ�Ԫ����

 @author ˾���
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

TEST(Utilities_FileStream, Creat)
{
	wchar_t fileName[L_tmpnam];
	_wtmpnam(fileName);
	FileStream fs = FileStream(fileName, Stream::Type::WriteOnly);
	fs.Close();
	// �ж��ļ��Ƿ����
	// https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/access-waccess?view=vs-2019
	EXPECT_EQ(_waccess(fileName, 0), 0);
}

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