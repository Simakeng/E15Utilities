#include <cstdint>
#include "Utilities.Common.Range.h"
namespace Utilities::Common
{
	// Ԥʵ����һ����ģ�����ٱ���
	template<>
	class Range<int8_t>;
	template<>
	class Range<int16_t>;
	template<>
	class Range<int32_t>;
	template<>
	class Range<int64_t>;
	template<>
	class Range<uint8_t>;
	template<>
	class Range<uint16_t>;
	template<>
	class Range<uint32_t>;
	template<>
	class Range<uint64_t>;
}