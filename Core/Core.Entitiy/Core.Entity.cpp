/**
	 @file
	 @brief ���Ŀ� Entity �����Ľӿ�ʵ��

	 ����ļ���������Ŀ� Entity �Ľӿ�ʵ��

	 @author ˾���
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

	Entity::Entity(Entity&& rhs)
	{
		rhs.comps.swap(this->comps);
	}
}