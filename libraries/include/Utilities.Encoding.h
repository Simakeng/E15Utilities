#pragma once
#include <string>
#include <Utilities.h>
namespace Utilities::Encoding
{
	class Encoding 
	{
	public:
		virtual bytes Encode(const string& s) = 0;
		virtual string Decode(const bytes& s) = 0;
	};

	class UTF8 : public Encoding
	{
	public:
		virtual bytes Encode(const string& s) override;
		virtual string Decode(const bytes& s) override;
	};

	class GB2312 : public Encoding
	{
	public:
		virtual bytes Encode(const string& s) override;
		virtual string Decode(const bytes& s) override;
	};



}
