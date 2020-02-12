/**
 @file
 @brief 通用IO流 StreamWriter 特化部分的实现

 @author 司马坑
 @date 2020/2/12
*/ 
#include "Utilities.StreamWriter.h"
namespace Utilities 
{

	StreamWriter<FILE*>::StreamWriter(FILE* stream) : rs(stream) { }

	StreamWriter<FILE*>::~StreamWriter() { }



	void StreamWriter<FILE*>::WriteString(const char* string)
	{
		auto len = strlen(string);
		fwrite(string, 1, len, rs);
	}


	void StreamWriter<FILE*>::Write(const char* string)
	{
		WriteString(string);
	}

	template<>
	void StreamWriter<FILE*>::Write(const u8string& obj)
	{
		WriteString(obj);
	}
}