/**
 @file
 @brief 通用IO库 文件流接口定义

 @author 司马坑
 @date 2020/2/11
*/
#pragma once
#include "Utilities.Stream.h"

namespace Utilities
{
	/// <summary>
	/// 文件流对象
	/// </summary>
	class FileStream : public Stream
	{
	public:
		/// <summary>
		/// 实例化一个文件流对象以访问文件
		/// </summary>
		/// <param name="fileName">文件名</param>
		/// <param name="ioType">操作类型</param>
		/// <param name="IstextMode">是否以文本模式打开</param>
		FileStream(const wchar_t* fileName, const Type& ioType, bool IstextMode = true);
		/// <summary>
		/// 析构函数
		/// </summary>
		virtual ~FileStream();
	public:
		/// <summary>
		/// 流对象读取接口
		/// </summary>
		/// <param name="len">要读取的数据长度</param>
		/// <param name="data">数据写入的目标缓冲区</param>
		virtual void Read(size_t len, void* data) override;
		/// <summary>
		/// 流对象写入接口
		/// </summary>
		/// <param name="len">要写入的数据长度</param>
		/// <param name="data">数据</param>
		virtual void Write(size_t len, const void* data) override;
		/// <summary>
		/// 关闭流对象
		/// </summary>
		virtual void Close() override;
		/// <summary>
		/// 检测流对象是否可用
		/// </summary>
		virtual bool IsVaild() override;
	public:
		/// <summary>
		/// 获取文件流的长度
		/// </summary>
		/// <returns></returns>
		uint64_t GetLength();
		/// <summary>
		/// 获取当前读取指针的位置
		/// </summary>
		/// <returns></returns>
		uint64_t GetPosition();
		/// <summary>
		/// 设置当前读取指针的位置
		/// </summary>
		/// <param name="pos"></param>
		void SetPosition(uint64_t pos);
		/// <summary>
		/// 移动当前读取指针的位置
		/// </summary>
		/// <param name="offset">偏移量</param>
		void Seek(int64_t offset);
	public:
		/// <summary>
		/// 获取由系统维护的文件句柄
		/// <para>
		/// 该返回值在 Winodows 操作系统下即为 File *
		/// </para>
		/// </summary>
		/// <returns></returns>
		Handle GetHandle();
	private:
		Handle handle = nullptr;
	};
}