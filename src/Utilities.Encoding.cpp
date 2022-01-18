#include "Utilities.Encoding.h"

#define MAKE_UTF8_2_1

namespace Utilities::Encoding
{
	bytes UTF8::Encode(const string& s)
	{
		size_t strlen = 0;

		for (auto& ch : s)
		{
			if (ch < 0x7F)
				strlen += 1;
			else if (ch < 0x7FF)
				strlen += 2;
			else if (ch < 0xFFFF)
				strlen += 3;
			else if (ch < 0x1FFFFF)
				strlen += 4;
			else if (ch < 0x3fFFFFF)
				strlen += 5;
			else
				strlen += 6;
		}

		auto res = bytes(strlen, 0x00);
		auto it = res.begin();

		for (auto& ch : s)
		{
			if (ch < 0x7F)
				*(it++) = ch;
			else if (ch < 0x7FF)
			{
				*(it++) = ((ch & 0x07C0) >> 6) | 0xC0;
				*(it++) = ((ch & 0x003F) >> 0) | 0x80;
			}
			else if (ch < 0xFFFF)
			{
				*(it++) = ((ch & 0xF000) >> 12) | 0xE0;
				*(it++) = ((ch & 0x0FC0) >> 6) | 0x80;
				*(it++) = ((ch & 0x003F) >> 0) | 0x80;
			}
			else if (ch < 0x1FFFFF)
			{
				*(it++) = ((ch & 0x1C0000) >> 18) | 0xF0;
				*(it++) = ((ch & 0x03F000) >> 12) | 0x80;
				*(it++) = ((ch & 0x000FC0) >> 6) | 0x80;
				*(it++) = ((ch & 0x00003F) >> 0) | 0x80;
			}
			else if (ch < 0x3fFFFFF)
			{
				*(it++) = ((ch & 0x3000000) >> 24) | 0xF0;
				*(it++) = ((ch & 0x0FC0000) >> 18) | 0x80;
				*(it++) = ((ch & 0x003F000) >> 12) | 0x80;
				*(it++) = ((ch & 0x0000FC0) >> 6) | 0x80;
				*(it++) = ((ch & 0x000003F) >> 0) | 0x80;
			}
			else
			{
				*(it++) = ((ch & 0x40000000) >> 30) | 0xFC;
				*(it++) = ((ch & 0x3F000000) >> 24) | 0x80;
				*(it++) = ((ch & 0x00FC0000) >> 18) | 0x80;
				*(it++) = ((ch & 0x0003F000) >> 12) | 0x80;
				*(it++) = ((ch & 0x00000FC0) >> 6) | 0x80;
				*(it++) = ((ch & 0x0000003F) >> 0) | 0x80;
			}
		}
		return res;
	}

	string UTF8::Decode(const bytes& s)
	{
		return string();
	}

}