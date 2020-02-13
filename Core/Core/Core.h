#pragma once

/// <summary>
/// 引擎核心组件命名空间
/// </summary>
namespace Core 
{
#ifdef _MEMORY_
	template<typename T>
	using View = std::weak_ptr<T>;
#endif // _MEMORY_

}