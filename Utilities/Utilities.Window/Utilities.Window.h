#pragma once
#include <cstdint>
#include <deque>
#include <functional>
#include <thread>

/**
 @file
 @brief 窗口绘图 窗口类及窗口事件的接口定义

 @author 司马坑
 @date 2020/4/30
*/

/**
 @defgroup Utilities_Window Utilities::Window 窗口接口类
 该模块定义了一个通过与系统交互实现窗口绘图的接口

 具体使用方式以及接口定义请参考 Utilities::Window
*/

namespace Utilities
{
#pragma region 窗口事件交互
	/**
		示例代码：
		@code
			WindowDesc desc;
			desc.tittle = "Example Window";
			desc.callback = [](const WindowEvent& event, uint32_t param) 
			{ 
				switch(event)
				{
					case WindowEvent::Moving:
						return BobDoSomeThing();
					case WindowEvent::Closing:
						return HoustonWeHaveAProblem();
					default:
						return true;
				}
			};
			Window window(desc);
		@endcode
	*/
	/// <summary>
	/// 窗口事件枚举体
	/// </summary>
	/// <para>
	/// 该枚举体列举了一系列可能发生在窗口上的事件
	/// </para>
	enum class WindowEvent {
		//!窗口被移动后此事件会被激活
		Move,
		//!窗口正在被移动时此事件会被激活
		Moving,
		//!窗口大小改变后此事件会被激活
		Resize,
		//!窗口大小正在改变时此事件会被激活
		Resizing,
		//!窗口获得焦点时此事件会被激活
		GetFocus,
		//!窗口失去焦点时此事件会被激活
		LostFocus,
		//!窗口重绘前此事件会被激活
		RePaint,
		//!窗口将要关闭时此事件会被激活
		Closing,
		//!窗口被销毁时此事件会被激活
		Destroy,
		//!窗口被左键单击时此事件会被激活
		LeftClick,
		//!窗口被左键双击时此事件会被激活
		LeftDoubleClick,
		//!窗口被右键单击时此事件会被激活
		RightClick,
		//!窗口被右键双击时此事件会被激活
		RightDoubleClick,
		//!鼠标悬停在窗口并将滚轮往上滑时此事件会被激活
		WheelUp,
		//!鼠标悬停在窗口并将滚轮往下滑时此事件会被激活
		WheelDown,
		//!窗口被中键单击时此事件会被激活
		MiddleClick,
		//!窗口被中键双击时此事件会被激活
		MiddleDoubleClick,
		//!窗口被中键按下时此事件会被激活
		MiddleButtonDown,
		//!鼠标左键被按下时此事件会被激活
		LeftButtonDown,
		//!鼠标右键被按下时此事件会被激活
		RightButtonDown,
		//!键盘上按下某建时此事件会被激活
		KeyDown,
		//!键盘上松开某键时此事件会被激活
		KeyUp
	};
#pragma endregion
#pragma region 默认的窗口回调函数
	/// <summary>
	/// 默认的窗口回调函数
	/// </summary>
	bool DefaultWindowCallback(const WindowEvent&, uint32_t);
	/// <summary>
	/// 回调函数
	/// <para>
	/// 该类型为一个函数指针，其接受一个参数类型为 (const WindowEvent&, uint32_t) 、返回值为 bool 的函数.
	/// </para>
	/// </summary>
	using WindowCallback = bool(*)(const WindowEvent&, uint32_t);
#pragma endregion
#pragma region 预定义类型
	using ColorRef = int;
	/// <summary>
	/// 生成一个颜色值
	/// </summary>
	/// <param name="r">红色分量</param>
	/// <param name="g">绿色分量</param>
	/// <param name="b">蓝色分量</param>
	/// <example>
	/// auto color = MakeRGB(128,128,128);
	/// </example>
	/// <returns></returns>
	inline constexpr ColorRef MakeRGB(uint8_t r, uint8_t g, uint8_t b)
	{
		return r | g << 8 | b << 16;
	}
	/// <summary>
	/// 用于描述窗口信息的结构体
	/// </summary>
	struct WindowDesc
	{
		WindowDesc();

		//!窗口位置常量
		enum Pos : uint8_t
		{
			Non = 0,		//!< 默认位置
			Left = 1,		//!< 紧贴左边
			Right = 2,		//!< 紧贴右边
			Center = 0xFF,	//!< 居中
			Top = 4,		//!< 紧贴顶部
			Buttom = 8		//!< 紧贴底部
		};
		//!指明要创建的窗口的对齐方式
		Pos align = Center;
		//!指明要创建的窗口的标题
		std::string tittle = "E15GameStudio://Utilities::Window";
		//!指明要创建的窗口是否有最大化按钮
		bool withMaxBox = true;
		//!指明要创建的窗口是否有最小化按钮
		bool withMinBox = true;
		//!指明要创建的窗口是否有标题栏
		bool withCaption = true;
		//!指明要创建的窗口是否可以拖动边框改变尺寸
		bool sizeable = true;
		//!高度
		int32_t height = 720;
		//!宽度
		int32_t width = 1280;
		//!客户区原点相对于主屏幕左上角的X坐标
		int32_t posX = 0;
		//!客户区原点相对于主屏幕左上角的Y坐标
		int32_t posY = 0;
		//!背景颜色
		ColorRef backGroundColor = MakeRGB(0, 0, 0);
		/**
			示例代码：
			@code
				WindowDesc desc;
				desc.tittle = "Example Window";
				desc.callback = [](const WindowEvent& event, uint32_t param)
				{
					switch(event)
					{
						case WindowEvent::Moving:
							return BobDoSomeThing();
						case WindowEvent::Closing:
							return HoustonWeHaveAProblem();
						default:
							return true;
					}
				};
				Window window(desc);
			@endcode
		*/
		//!窗口消息回调函数
		WindowCallback callback;
	};
#pragma endregion

	/// <summary>
	/// 实例化一个空的纯色窗口对象，并通过此对象控制窗口行为。
	/// </summary>
	class Window
	{
#pragma region 构造与赋值
	public:
		Window() = delete;
		/// <summary>
		/// 使用窗口描述信息创建一个窗口
		/// </summary>
		/// <param name="desc">描述信息</param>
		Window(const WindowDesc& desc);
		/// <summary>
		/// 使用默认的窗口描述信息创建一个窗口
		/// </summary>
		/// <param name="tittle">窗口标题</param>
		Window(const std::string& tittle);
		//析构函数
		~Window()noexcept;
	public:
		//移动构造函数
		Window(Window&& rhs);
		//移动赋值函数
		Window& operator= (Window&&) = delete;
	public:
		//复制构造函数
		Window(const Window&) = delete;
		//复制赋值函数
		Window& operator= (Window&) = delete;

#pragma endregion
#pragma region 窗口类行为交互函数
	public:
		struct Point
		{
			int x, y;
		};
		struct Size
		{
			int width, height;
		};
	public:
		/// <summary>
		/// 获取当前窗口对象客户区域左上角相对于主屏幕左上角的坐标
		/// </summary>
		/// <returns>::Utilities::Window::Point</returns>
		Point GetPos();

		/// <summary>
		/// 设置当前窗口对象左上角相对于主屏幕左上角的坐标
		/// </summary>
		/// <param name="x">相对于主屏幕左上角的X坐标</param>
		/// <param name="y">相对于主屏幕左上角的Y坐标</param>
		void SetPos(int x, int y);

		/// <summary>
		/// 获取当前窗口对象的尺寸
		/// </summary>
		/// <returns>::Utilities::Window::Size</returns>
		Size GetSize();
		/// <summary>
		/// 设置此窗口对象的尺寸
		/// </summary>
		/// <param name="width">宽度</param>
		/// <param name="height">高度</param>
		void SetSize(int width, int height);
	public:
		/// <summary>
		/// 隐藏此窗口对象
		/// </summary>
		void Hide();
		/// <summary>
		/// 显示此窗口对象
		/// </summary>
		void Show();
		/// <summary>
		/// 最大化此窗口对象
		/// </summary>
		void SetMax();
		/// <summary>
		/// 最小化此窗口对象
		/// </summary>
		void SetMin();
		/// <summary>
		/// 取消此窗口的对象最大化或最小化
		/// </summary>
		void RestoreSize();
	public:
		/// <summary>
		/// 等待窗口销毁
		/// </summary>
		void WaitUntillDestroy();
	public:
		/// <summary>
		/// 设置窗口图标
		/// </summary>
		/// <param name="IconFilePath">ico格式图标文件路径</param>
		/// <returns></returns>
		void SetIcon(std::string IconFilePath) noexcept;
		/// <summary>
		/// 设置鼠标悬浮在窗口上时的光标
		/// </summary>
		/// <param name="IconFilePath">cur格式图标文件路径</param>
		/// <returns></returns>
		void SetCursor(std::string IconFilePath) noexcept;
#pragma endregion
#pragma region NativeAPI交互
	public:
		using Handle = void*;
		/// <summary>
		/// 获取当前窗口的在系统中的原生句柄
		/// <para>
		/// 	@warning 注意 : 此函数依赖于系统支持
		/// </para>
		/// </summary>
		/// <returns>Handle</returns>
		Handle GetWindowHandle() noexcept;
		/// <summary>
		/// 设置当前窗口对象的图标
		/// <para>
		/// 	@warning 注意 : 此函数依赖于系统支持
		/// </para>
		/// </summary>
		/// <param name="hIcon">系统原生的图标句柄</param>
		void SetIcon(Handle hIcon) noexcept;
		/// <summary>
		/// 设置当前窗口对象的光标
		/// <para>
		///		@warning 注意 : 此函数依赖于系统支持
		/// </para>
		/// </summary>
		/// <param name="hCursor">系统原生的光标句柄</param>
		void SetCursor(Handle hCursor) noexcept;
		/// <summary>
		/// 设置当前窗口对象的标题
		/// <para>
		///		@warning 注意 : 此函数依赖于系统支持
		/// </para>
		/// </summary>
		/// <param name="tittle">以系统默认编码的窗口标题</param>
		void SetTittle(const std::string& tittle) noexcept;
#pragma endregion
#pragma region 窗口状态量
	private:
		Handle windowHandle = nullptr;
		std::thread windowThread;
		WindowCallback callback = nullptr;
		friend WindowCallback _get_callback(const Window* w);
#pragma endregion
	};
}
