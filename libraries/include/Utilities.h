/**
 @file
 @brief 通用编程库命名空间 类型定义

 @author 司马坑
 @date 2020/2/10
*/
#pragma once

#include <cstdint>
#include <string>
#include <exception>

/// <summary>
/// 通用编程库命名空间
/// </summary>
namespace Utilities 
{
    using string = std::u32string;      //!< UCS-4字符串类型
    using u8string = std::string;       //!< UTF-8字符串类型
	using Exception = std::exception;   //!< 异常类型
    using Handle = void*;               //!< 句柄类型
}
#pragma once

namespace Utilities 
{
	/// <summary>
	/// 获取当前代码库的版本信息
	/// </summary>
	const char* GetVersion();
}
