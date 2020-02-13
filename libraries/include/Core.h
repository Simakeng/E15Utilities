#pragma once
namespace Core 
{
#ifdef _MEMORY_
	template<typename T>
	using View = std::weak_ptr<T>;
#endif // _MEMORY_

}
