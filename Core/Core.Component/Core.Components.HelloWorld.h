#pragma once
#include "Core.Component.h"
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