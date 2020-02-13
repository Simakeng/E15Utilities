#include <gtest\gtest.h>
#include <Core.h>
#include <Core.Entitiy.h>
#include <Core.Component.h>

#pragma comment(lib,"Core.Entitiy.lib")
#pragma comment(lib,"Core.Component.lib")

using namespace Core;

TEST(Core_Entity,Creation)
{
	Entity e = Entity();
	auto v = e.CreateComponent<Components::HelloWorld>();
	
	EXPECT_EQ(false, v.expired());

	if (auto comp = v.lock())
	{
		EXPECT_STREQ(comp->GetStr(), u8"Hello World!\n");

		EXPECT_NE(comp->GetCompID().ToString(), GUID::Nil.ToString());
	}


	
}