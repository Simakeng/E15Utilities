/**
 @file
 @brief 代码库通用GUID实现

 这个文件里面是对通用GUID对象的函数的具体实现

 @see Utilities.GUID.h

 @author 司马坑
 @date 2020/2/10
*/


#include "Utilities.GUID.h"
#include <random>
#include <chrono>
#include <sstream>
#include <iomanip>
/// <summary>
/// 大小端转换函数
/// <para>
/// GUID对象的存储模式涉及到大小端转换，在这里设计一个函数进行处理
/// </para>
/// </summary>
template<typename T>
T BELEConvert(const T& value)
{
	T result = 0;
	const uint8_t* ptr = (const uint8_t*)&value;
	uint8_t * rptr = (uint8_t*)&result;
	for (int i = 0; i < sizeof(T); i++)
	{
		rptr[sizeof(T) - 1 - i] = ptr[i];
	}
	return result;
}

namespace Utilities
{
	struct
	{
		uint64_t o = 0;
		uint64_t l = 0;
	}
	NilGUID;
	const GUID GUID::Nil = NilGUID;
	std::mt19937 ran(clock());
	GUID Utilities::GUID::New()
	{
		auto temp = Nil;
		auto ptr = reinterpret_cast<uint8_t*>(&temp.datas);
		for (int i = 0; i < 16; i++)
		{
			ptr[i] = ran() % 256;
		}
		ptr[7] &= 0x0F;//版本号
		ptr[7] |= 0x40;

		temp.datas.data4 = BELEConvert(temp.datas.data4);
		return temp;
	}

	constexpr uint8_t Convert(uint8_t p) 
	{
		if (p >= 'a')
			return (p - 'a' + 10);
		else
			return(p - '0');
	}

	GUID::GUID(const std::string & rhs)
	{
		auto ptr = (uint8_t*)&datas;
		int i = 0;

#define 偷懒_(c1) case c1: ptr[i/2] |= i%2==0?Convert(c1)<<4:Convert(c1);i++;break
#define 偷懒(c1,c2) case c2: 偷懒_(c1);

		for (auto c : rhs)
		{
			if (i == 32) break;
			switch (c)
			{
				偷懒('a', 'A');
				偷懒('b', 'B');
				偷懒('c', 'C');
				偷懒('d', 'D');
				偷懒('e', 'E');
				偷懒('f', 'F');
				偷懒_('0');
				偷懒_('1');
				偷懒_('2');
				偷懒_('3');
				偷懒_('4');
				偷懒_('5');
				偷懒_('6');
				偷懒_('7');
				偷懒_('8');
				偷懒_('9');
			case '{':
			case '}':
			case '-':
			case ' ':
				continue;
			default:
				throw std::exception("inv input");
			}
		}
		datas.data1 = BELEConvert(datas.data1);
		datas.data2 = BELEConvert(datas.data2);
		datas.data3 = BELEConvert(datas.data3);
		return;
	}
	GUID::GUID(const char * rhs) : GUID(std::string(rhs))
	{
	}
	GUID::GUID(const GUID & rhs)
		:datas(rhs.datas) { }
	GUID::GUID(GUID && rhs)
	{
		this->datas = rhs.datas;
		rhs = Nil;
	}

	GUID & GUID::operator=(const std::string & rhs)
	{
		auto ptr = (uint8_t*)&datas;
		int i = 0;

		for (auto c : rhs)
		{
			if (i == 32) break;
			switch (c)
			{
				偷懒('a', 'A');
				偷懒('b', 'B');
				偷懒('c', 'C');
				偷懒('d', 'D');
				偷懒('e', 'E');
				偷懒('f', 'F');
				偷懒_('0');
				偷懒_('1');
				偷懒_('2');
				偷懒_('3');
				偷懒_('4');
				偷懒_('5');
				偷懒_('6');
				偷懒_('7');
				偷懒_('8');
				偷懒_('9');
			case '{':
			case '}':
			case '-':
			case ' ':
				continue;
			default:
				throw std::exception("inv input");
			}
		}
		datas.data1 = BELEConvert(datas.data1);
		datas.data2 = BELEConvert(datas.data2);
		datas.data3 = BELEConvert(datas.data3);
		return *this;
	}

	GUID & GUID::operator=(const GUID & rhs)
	{
		this->datas = rhs.datas;
		return *this;
	}
	GUID & GUID::operator=(GUID && rhs)
	{
		this->datas = rhs.datas;
		rhs = Nil;
		return *this;
	}

	bool GUID::operator==(const GUID & rhs) const
	{
		return 
			datas.data1 == rhs.datas.data1 &&
			datas.data2 == rhs.datas.data2 &&
			datas.data3 == rhs.datas.data3 &&
			datas.data4 == rhs.datas.data4 ;
	}

	bool GUID::operator!=(const GUID & rhs) const
	{
		return !operator==(rhs);
	}

	GUID::operator std::string() const
	{
		std::ostringstream result;
		result << std::hex << std::setfill('0') << std::setw(8) << datas.data1;
		result << '-';
		result << std::hex << std::setfill('0') << std::setw(4) << datas.data2;
		result << '-';
		result << std::hex << std::setfill('0') << std::setw(4) << datas.data3;
		result << '-';
		auto ptr = (uint8_t*)&datas;
		for (int i = 8; i < 10; i++)
			result << std::hex << std::setfill('0') << std::setw(2) << (int)ptr[i];//不强转int会被当成字符输出
		result << '-';
		for (int i = 10; i < 16; i++)
			result << std::hex << std::setfill('0') << std::setw(2) << (int)ptr[i];
		return result.str();
	}
	std::string GUID::ToString() const
	{
		return std::string(*this);
	}
}