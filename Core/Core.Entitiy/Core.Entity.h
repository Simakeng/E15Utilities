/**
	 @file
	 @brief 核心库 Entity 容器的接口定义

	 这个文件是引擎核心库 Entity 的接口定义

	 @author 司马坑
	 @date 2020/2/13
*/

#pragma once

#include <memory>
#include <unordered_map>

#include <Core.h>
#include <Core.Component.h>

namespace Core
{
	/// <summary>
	/// 实体容器
	/// </summary>
	class Entity
	{
	public:
		Entity();
		~Entity();
	public:
		Entity(const Entity&) = delete;
		/// <summary>
		/// 原位构造一个 Entity 对象
		/// </summary>
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
		/// <summary>
		/// 根据组件的 GUID 从实体中获取组件的引用 
		/// </summary>
		/// <param name="compId">组件 GUID </param>
		/// <returns>
		/// 如果该实体中不存在 GUID 为 compId 的组件，返回值为空引用
		/// 如果该实体中存在，则返回改组件的引用
		/// @warning 对空引用的<b>任何</b>访问操作都是未定义行为 
		/// </returns>
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