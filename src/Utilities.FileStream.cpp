/**
 @file
 @brief 通用IO库 文件流实现

 @author 司马坑
 @date 2020/2/11
*/
#define _CRT_SECURE_NO_WARNINGS
#include "Utilities.FileStream.h"

#include <cstdio>

namespace Utilities
{
	FileStream::FileStream(const wchar_t* fileName, const Type& ioType, bool isTextMode) : Stream(ioType)
	{
		//! @todo 使用_waccess函数进行可访问性验证并抛出异常
		// https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/access-waccess?view=vs-2019

		FILE* fp = nullptr;
		const wchar_t* mode = nullptr;
		switch (ioType)
		{
		case Stream::Type::ReadOnly: mode = isTextMode ? L"r" : L"rb"; break;
		case Stream::Type::WriteOnly: mode = isTextMode ? L"w" : L"wb"; break;
		case Stream::Type::ReadWrite:throw Exception(u8"IO Mode 'Type::ReadWrite' on 'FileStream' is not supported.");
		default: throw Exception(u8"Unknow IO Mode 'Type::Unknown'");
		};
		auto err = _wfopen_s(&fp, fileName, mode);
		if (err != 0)
		{
			//! @todo 等CodeConv完事了把这里的异常处理加上文件名
			// std::codecvt_utf8<wchar_t>
			auto errInfo = u8string("Error occured when opening file :") + strerror(err);
			throw Exception(errInfo.data());
		};

		this->handle = fp;
	}
	FileStream::~FileStream()
	{
		auto fp = reinterpret_cast<FILE*>(handle);
		if (fp != nullptr)
			fclose(fp);
	}
	void FileStream::Read(size_t len, void* data)
	{
		if (GetStreamType() == Type::WriteOnly)
			throw Exception(u8"Error occured when reading stream : Cannot_Read_WriteOnly_Stream");

		auto fp = reinterpret_cast<FILE*>(handle);
		auto readLen = fread(data, 1, len, fp);

		if (readLen != len)
		{
			auto err = ferror(fp);
			auto errInfo = u8string("Error occured when opening file :") + strerror(err);
			throw Exception(errInfo.data());
		}
	}
	void FileStream::Write(size_t len, const void* data)
	{
		if (GetStreamType() == Type::ReadOnly)
			throw Exception(u8"Error occured when reading stream : Cannot_Write_ReadOnly_Stream");

		auto fp = reinterpret_cast<FILE*>(handle);
		auto writeLen = fwrite(data, 1, len, fp);

		if (writeLen != len)
		{
			auto err = ferror(fp);
			auto errInfo = u8string("Error occured when opening file :") + strerror(err);
			throw Exception(errInfo.data());
		}
	}
	void FileStream::Close()
	{
		auto fp = reinterpret_cast<FILE*>(handle);
		if (fp != nullptr)
			fclose(fp);
	}
	bool FileStream::IsVaild()
	{
		auto fp = reinterpret_cast<FILE*>(handle);
		return fp != nullptr;
	}
	uint64_t FileStream::GetLength()
	{
		auto fp = reinterpret_cast<FILE*>(handle);
		if (fp != nullptr)
		{
			auto currentPos = _ftelli64(fp);
			_fseeki64(fp, 0, SEEK_END);
			auto endPos = _ftelli64(fp);
			_fseeki64(fp, currentPos, SEEK_SET);
			return endPos;
		}
		else
			throw Exception("Stream Closed");
	}
	uint64_t FileStream::GetPosition()
	{
		auto fp = reinterpret_cast<FILE*>(handle);
		if (fp != nullptr)
		{
			return _ftelli64(fp);
		}
		else
			throw Exception("Stream Closed");
	}
	void FileStream::SetPosition(uint64_t pos)
	{
		auto fp = reinterpret_cast<FILE*>(handle);
		if (fp != nullptr)
		{
			_fseeki64(fp, static_cast<long long>(pos), SEEK_SET);
			return;
		}
		else
			throw Exception("Stream Closed");
	}
	void FileStream::Seek(int64_t offset)
	{
		auto fp = reinterpret_cast<FILE*>(handle);
		if (fp != nullptr)
		{
			_fseeki64(fp, static_cast<long long>(offset), SEEK_CUR);
			return;
		}
		else
			throw Exception("Stream Closed");
	}
	Handle FileStream::GetHandle()
	{
		return handle;
	}
}