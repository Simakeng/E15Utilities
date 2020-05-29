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
    using int8 = int8_t;        //!< 8 位整数类型
    using int16 = int16_t;      //!< 16 位整数类型
    using int32 = int32_t;      //!< 32 位整数类型
    using int64 = int64_t;      //!< 64 位整数类型

    using uint8 = uint8_t;      //!< 8 位无符号整数类型
    using uint16 = uint16_t;    //!< 16 位无符号整数类型
    using uint32 = uint32_t;    //!< 32 位无符号整数类型
    using uint64 = uint64_t;    //!< 64 位无符号整数类型

    using utf8 = int8_t;        
    using ucs2 = char16_t;
    using ucs4 = char32_t;

    using u8string = std::basic_string<utf8>;   //!< UTF-8 字符串类型
    using u16string = std::basic_string<ucs2>;  //!< UCS-2 字符串类型 (BMP 字符串)
    using u32string = std::basic_string<ucs4>;  //!< UCS-4 字符串类型

    using string = u32string;           //!< UCS-4 字符串类型

	using Exception = std::exception;   //!< 异常类型
    using Handle = void*;               //!< 句柄类型
}