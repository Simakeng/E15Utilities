#include "Core.Component.h"
#pragma comment(lib,"Utilities.GUID.lib")

namespace Core
{
	IComponent::IComponent(const char* TypeName, const GUID& TypeGUID) :
		compID(GUID::New()),
		compType(TypeName, TypeGUID)
	{
	}

	const GUID& IComponent::GetCompID()
	{
		return this->compID;
	}

	const ComponentType& IComponent::GetType()
	{
		return this->compType;
	}

	ComponentType::ComponentType(const char* _compName, const GUID& _typeID) :
		compName(_compName),
		typeID(_typeID)
	{
	}
}