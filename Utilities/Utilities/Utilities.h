/**
 @file
 @brief 通用编程库命名空间 基本类型定义

    该文件定义了部分基本的类型，如：
        - 各种整数
        - 字符类型
        - 字符串类型
        - 基本异常类型

 @author 司马坑
 @date 2020/5/29
*/
#pragma once

#include <cstdint>
#include <string>
#include <exception>

namespace Utilities 
{
    using int8 = int8_t;        //!< UTF-8字符串类型
    using int16 = int16_t;      
    using int32 = int32_t;
    using int64 = int64_t;

    using uint8 = uint8_t;
    using uint16 = uint16_t;
    using uint32 = uint32_t;
    using uint64 = uint64_t;

    using utf8 = int8_t;
    using ucs2 = char16_t;
    using ucs4 = char32_t;

    using u8string = std::basic_string<utf8>;   //!< UTF-8字符串类型
    using u16string = std::basic_string<ucs2>;
    using u32string = std::basic_string<ucs4>;

    using string = std::u32string;      //!< UCS-4字符串类型
    using u8string = std::string;       //!< UTF-8字符串类型
	using Exception = std::exception;   //!< 异常类型
    using Handle = void*;               //!< 句柄类型
}