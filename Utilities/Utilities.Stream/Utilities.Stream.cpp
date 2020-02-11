/**
 @file
 @brief 通用IO库 缺省函数实现

 @author 司马坑
 @date 2020/2/11
*/

#include "Utilities.Stream.h"

Utilities::Stream::Stream(Stream&& rhs) noexcept : streamType(rhs.streamType)
{
	rhs.streamType = Type::Unkonwn;
}

Utilities::Stream::Stream(const Type& _streamType) noexcept : streamType(_streamType) {}

