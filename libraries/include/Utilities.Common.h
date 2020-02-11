/**
	 @file
	 @brief 通用模板库 Range 对象的实现

	 这个文件里面是通用模板库 Range 对象的实现
	 其与Python之类语言中的 Range 对象行为相似，通过迭代器来进行范围遍历

	@code
		for(auto& i in range(100))
		{
			// ...  do things
		}
	@endcode

	 @author 司马坑
	 @date 2020/2/10
*/

/**
	@defgroup Utilities_Common Utilities::Commom 通用模板库
	该模块包含了通用工具模板
	- 工具列表
		- Utilities::Common::Range
	@{
*/
#pragma once
#include <iterator>

namespace Utilities::Common
{
	/**
		例子：
		@code
			for(auto& i in range(100))
			{
				// ...  do things
			}
		@endcode
	*/
	/// <summary>
	/// 迭代遍历器 Range
	/// <para>
	/// 其与Python之类语言中的 Range 对象行为相似，通过迭代器来进行范围遍历
	/// </para>
	/// </summary>
	/// <typeparam name="T">数据类型</typeparam>
	template<typename T = int>
	class [[nodiscard]] Range
	{
	private:
		T _begin, _end, _distance;
	public:
		/// <summary>
		/// 生成一个在 区间[ begin , end ) 之间遍历的迭代遍历器
		/// <param name="_begin">遍历的起始点</param>
		/// <param name="_end">遍历的终止点</param>
		/// <param name="_distance">遍历的间隔</param>
		/// </summary>
		Range(T _begin, T _end, T _distance = 1) :_begin(_begin), _end(_end), _distance(_distance) {};
		Range() = delete;
		~Range() { };
		/// <summary>
		/// 复制构造函数
		/// </summary>
		Range(const Range& rhs) :Range(rhs._begin, rhs._end, rhs._distance) {};
		/// <summary>
		/// 移动构造函数
		/// </summary>
		Range(Range&& rhs) :Range(rhs._begin, rhs._end, rhs._distance) { rhs._begin = 0; rhs._end = 0; rhs._distance = 0; };
		/// <summary>
		/// 复制赋值函数
		/// </summary>
		Range& operator=(const Range& rhs) { this->_begin = rhs._begin; this->_end = rhs._end; this->_distance = rhs._distance; };
		/// <summary>
		/// 移动赋值函数
		/// </summary>
		Range& operator=(Range&& rhs) { this->_begin = rhs._begin; this->_end = rhs._end; this->_distance = rhs._distance; rhs._begin = 0; rhs._end = 0; rhs._distance = 0; };

	public:
		/**
			使用示例：
			@code
				for(auto it = range.begin();it!= range.end();it++)
				{
					// ...  do things
				}
				for(auto& i : range)
				{
					// ...  do things
				}
			@endcode
		*/
		/// <summary>
		/// 迭代器类 此类不可被用户实例化，其仅可以通过 Range::begin() 得到
		/// </summary>
		class iterator
		{
		private:
			friend class ::Utilities::Common::Range<T>;
			T current = 0;
			T distance = 0;
			iterator(const T& current, const T& distance) :current(current), distance(distance) {}
		public:
			iterator(const iterator& rhs) { this->current = rhs.current; this->distance = rhs.distance; }
			iterator& operator=(const iterator& rhs) { this->current = rhs.current; this->distance = rhs.distance; }
			~iterator() {}

			using value_type = T;
			using difference_type = T;
			using reference = T&;
			using pointer = T*;
			using iterator_category = std::output_iterator_tag;
			/// <summary>
			/// 使迭代器自增
			/// </summary>
			iterator& operator++() { current += distance; return *this; };
			/// <summary>
			/// 对迭代器取值
			/// </summary>
			/// <returns>当前迭代器的值</returns>
			T operator*() { return current; }
			/// <summary>
			/// 判断两迭代器是否相等
			/// </summary>
			bool operator!=(const iterator& rhs)
			{
				if constexpr (std::is_floating_point_v<T>)
					return std::fabs(current - rhs.current)
								<
					std::numeric_limits<T>::epsilon() * std::fabs(current + rhs.current) * 2
								||
					std::fabs(current - rhs.current)
								<
					std::numeric_limits<T>::min();
				else
					return current != rhs.current;
			}
		};
	public:
		/**
			使用示例：
			@code
				for(auto it = range.begin();it!= range.end();it++)
				{
					// ...  do things
				}
			@endcode
		*/
		/// <summary>
		/// 生成一个迭代器对象，该迭代器将指向区间[ begin , end )中的begin
		/// </summary>
		::Utilities::Common::Range<T>::iterator begin() { return iterator(_begin, _distance); };
		/**
			使用示例：
			@code
				for(auto it = range.begin();it!= range.end();it++)
				{
					// ...  do things
				}
			@endcode
		*/
		/// <summary>
		/// 生成一个迭代器对象，该迭代器将指向区间[ begin , end )中的end
		/// </summary>
		::Utilities::Common::Range<T>::iterator end() { return iterator(_end, _distance); };

		/**
			使用示例:
			@code
				std::deque<int> deque = Range(10);
				std::vector<int> vector = Range(10);
			@endcode
		*/
		/// <summary>
		/// 容器类型转换函数
		/// <para>
		///		该函数将一个 Range 对象转换为另一个具有 std::random_access_iterator_tag 的容器
		/// </para>
		/// </summary>
		/// <typeparam name="Container">容器类型</typeparam>
		template<typename Container>
		operator Container()
		{
			static_assert(std::is_same_v<Container::iterator::iterator_category, std::random_access_iterator_tag>,
				"iterator of container must be random access iterator!");
			Container c;
			c.resize(size_t(this->_end - this->_begin) / this->_distance);
			T i = this->_begin;
			for (auto& it : c)
			{
				it = i;
				i += _distance;
			}
			return c;
		}
	};
};

/**
@}
*/
