/**
	 @file
	 @brief 对核心库 Core::Entity 组件进行测试

	 @author 司马坑
	 @date 2020/2/13
*/
#include <gtest\gtest.h>
#include <Core.h> 
#include <Core.Entitiy.h>
#include <Core.Component.h>

#pragma comment(lib,"Core.Entitiy.lib")
#pragma comment(lib,"Core.Component.lib")

using namespace Core;

/// <summary>
///  测试实体对象的创建
/// </summary>
TEST(Core_Entity,Creation)
{
	Entity e = Entity();
}

/// <summary>
///  测试从实体对象创建组件的功能
/// </summary>
TEST(Core_Entity, Componet_Creation) 
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

/// <summary>
///  测试从实体对象中获取组件的功能
/// </summary>
TEST(Core_Entity, Componet_Find)
{
	Entity e = Entity();

	auto v = e.CreateComponent<Components::HelloWorld>();

	EXPECT_EQ(false, v.expired());
	auto comp = v.lock();

	auto compid = comp->GetCompID();
	
	auto view = e.GetCompView(compid);

	EXPECT_EQ((view.lock())->GetCompID(), compid);

	if (auto comp = v.lock())
	{
		EXPECT_STREQ(comp->GetStr(), u8"Hello World!\n");

		EXPECT_NE(comp->GetCompID().ToString(), GUID::Nil.ToString());
	}
}