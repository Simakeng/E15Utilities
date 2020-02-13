/**
	 @file
	 @brief 核心库 Entity 容器的接口实现

	 这个文件是引擎核心库 Entity 的接口实现

	 @author 司马坑
	 @date 2020/2/13
*/

#include "Core.Entity.h"

namespace Core
{
	Entity::Entity()
	{
	}

	Entity::~Entity()
	{
	}

	Entity::Entity(Entity&& rhs) noexcept
	{
		rhs.comps.swap(this->comps);
	}
}