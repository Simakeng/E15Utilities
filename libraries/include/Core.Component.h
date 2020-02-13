#pragma once
#pragma once
#include <Utilities.GUID.h>

namespace Core
{
	using GUID = Utilities::GUID;

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
		inline const char* GetTypeName() { return compName; };
		inline const GUID& GetTypeGUID() { return typeID; };
	public:
		friend class IComponent;
	};

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
namespace Core::Components
{
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
}
