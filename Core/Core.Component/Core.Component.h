/**
	 @file
	 @brief 核心库 IComponent 容器的接口定义

	 这个文件是引擎核心库 IComponent 的接口定义

	 @author 司马坑
	 @date 2020/2/13
*/
#pragma once
#include <Utilities.GUID.h>

namespace Core
{
	using GUID = Utilities::GUID;

	/// <summary>
	/// 描述组件类型 RTTI 的对象
	/// </summary>
	class ComponentType
	{
	private:
		ComponentType(const char* _compName, const GUID& _typeID);
	public:
		~ComponentType() = default;
		ComponentType(const ComponentType&) = default;
		ComponentType(ComponentType&&) = default;
	private:
		const char* compName;
		const GUID typeID;
	public:
		/// <summary>
		/// 获取该类型的名称
		/// </summary>
		inline const char* GetTypeName() { return compName; };
		/// <summary>
		/// 获取该类型的 GUID
		/// </summary>
		inline const GUID& GetTypeGUID() { return typeID; };
	public:
		friend class IComponent;
	};
	/**
		@code
			class HelloWorld : public Core::IComponent
			{
			public:
				HelloWorld() : IComponent(u8"HelloWorld", GUID("677e5b80-6835-4942-a1ce-db589ceaacb5")) {}
				~HelloWorld() = default;
			private:
				const char* str = u8"Hello World!\n";
			public:
				const char* GetStr() { return str; }
			};
		@endcode
	*/
	/// <summary>
	/// 组件接口
	/// <para>
	/// 用户需继承该类实现具体的组件功能
	/// 
	/// 这个是一个很简单的例子：
	/// @see Core::Components::HelloWorld
	/// 
	/// </para>
	/// </summary>
	class IComponent
	{
	protected:
		IComponent(const char* TypeName, const GUID& TypeGUID);
	public:
		virtual ~IComponent() = default;
	public:
		const GUID& GetCompID();
		const ComponentType& GetType();
	private:
		const GUID compID;
		const ComponentType compType;
	};
}