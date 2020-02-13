#pragma once

#include <memory>
#include <unordered_map>

#include <Core.h>
#include <Core.Component.h>

namespace Core
{
	class Entity
	{
	public:
		Entity();
		~Entity();
	public:
		Entity(const Entity&) = delete;
		Entity(Entity&& rhs);
	public:
		/**
			举例:
			@code

				e : Entity
				auto v = e.CreateComponent<Components::HelloWorld>();
				if(auto	comp = v.lock())
					printf("%s",comp->GetStr());
				
			@endcode
		*/
		/// <summary>
		/// 创建指定类型的组件
		/// </summary>
		/// <typeparam name="IComponentType">组件类型</typeparam>
		/// <returns>成功创建的组件对象的 View ( View<IComponent> ) </returns>
		template<typename IComponentType, typename...Args>
		View<IComponentType> CreateComponent(Args&& ... args)
		{
			// 转发调用构造函数
			auto ptr = std::make_shared<IComponentType>(args...);
			auto id = ptr->GetCompID();
			// 加入组件列表
			comps[id] = std::move(std::shared_ptr<IComponentType>(ptr));

			return View<IComponentType>(ptr);
		}
	private:
		std::unordered_map<GUID, std::shared_ptr<IComponent>> comps;
	public:
		View<IComponent> GetCompRef(const GUID& compId)
		{
			// 找不到则返回一个空的Ref
			if (comps.find(compId) == comps.end())
				return View<IComponent>();
			else
				return View<IComponent>(comps[compId]);
		}
	};
}
