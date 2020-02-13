/**
	 @file
	 @brief ���Ŀ� IComponent �����Ľӿڶ���

	 ����ļ���������Ŀ� IComponent �Ľӿڶ���

	 @author ˾���
	 @date 2020/2/13
*/
#pragma once
#include <Utilities.GUID.h>

namespace Core
{
	using GUID = Utilities::GUID;

	/// <summary>
	/// ����������� RTTI �Ķ���
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
		/// ��ȡ�����͵�����
		/// </summary>
		inline const char* GetTypeName() { return compName; };
		/// <summary>
		/// ��ȡ�����͵� GUID
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
	/// ����ӿ�
	/// <para>
	/// �û���̳и���ʵ�־�����������
	/// 
	/// �����һ���ܼ򵥵����ӣ�
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