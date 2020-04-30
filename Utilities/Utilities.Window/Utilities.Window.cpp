#include "Utilities.Window.h"
#include <Windows.h>
#include <future>
std::unordered_map<HWND, Utilities::Window*> WindowList;
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	using Event = ::Utilities::WindowEvent;
	auto callback = _get_callback(WindowList[hwnd]);
	switch (msg)
	{
	case WM_MOVE:
		return (LRESULT)callback(Event::Move, 0);
	case WM_MOVING:
		return (LRESULT)callback(Event::Moving, 0);
	case WM_SIZE:
		return (LRESULT)callback(Event::Resize, 0);
	case WM_SIZING:
		return (LRESULT)callback(Event::Resizing, 0);
	case WM_SETFOCUS:
		return (LRESULT)callback(Event::GetFocus, wParam);
	case WM_KILLFOCUS:
		return (LRESULT)callback(Event::LostFocus, wParam);
	case WM_PAINT:
		ValidateRect(hwnd, NULL);
		return (LRESULT)callback(Event::RePaint, 0);
	case WM_CLOSE:
		if (callback(Event::Closing, 0))
			return 0;
		else return DefWindowProcA(hwnd, msg, wParam, lParam);
	case WM_DESTROY:
		callback(Event::Destroy, 0);
		PostQuitMessage(0);
		return 0;
	case WM_LBUTTONDOWN:
		return (LRESULT)callback(Event::LeftButtonDown, 0);
	case WM_LBUTTONUP:
		return (LRESULT)callback(Event::LeftClick, 0);
	case WM_LBUTTONDBLCLK:
		return (LRESULT)callback(Event::LeftDoubleClick, 0);

	case WM_RBUTTONDOWN:
		return (LRESULT)callback(Event::RightButtonDown, 0);
	case WM_RBUTTONUP:
		return (LRESULT)callback(Event::RightClick, 0);
	case WM_RBUTTONDBLCLK:
		return (LRESULT)callback(Event::RightDoubleClick, 0);

	case WM_MOUSEWHEEL:
	{
		auto zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		if (zDelta >= 0)
			return (LRESULT)callback(Event::WheelUp, zDelta);
		else
			return (LRESULT)callback(Event::WheelDown, -zDelta);
	}

	case WM_MBUTTONDOWN:
		return (LRESULT)callback(Event::MiddleButtonDown, 0);
	case WM_MBUTTONUP:
		return (LRESULT)callback(Event::MiddleClick, 0);
	case WM_MBUTTONDBLCLK:
		return (LRESULT)callback(Event::MiddleDoubleClick, 0);

	case WM_KEYDOWN:
		return (LRESULT)callback(Event::KeyDown, wParam);

	case WM_KEYUP:
		return (LRESULT)callback(Event::KeyUp, wParam);

	default:
		return DefWindowProcA(hwnd, msg, wParam, lParam);
	}
};

void GenerateRandomString(char* pbuffer, int len)
{
	srand(clock());
	for (int i = 0; i < len; i++)
	{
		pbuffer[i] = 'A' + rand() % 26;
	}
};
namespace Utilities
{
	Window::Window(const WindowDesc& _desc)
	{
		WindowDesc Desc(_desc);
		this->callback = _desc.callback;
		std::promise<HWND> promise;
		auto future = promise.get_future();
		this->windowThread = std::move(std::thread([](std::promise<HWND> promise, WindowDesc desc)
			{
				char nameBuffer[41] = { 0 };
				GenerateRandomString(nameBuffer, 40);
				WNDCLASSEXA wndClass;
				memset(&wndClass, 0, sizeof(WNDCLASSEXW));
				wndClass.cbSize = sizeof(wndClass);
				wndClass.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
				wndClass.hInstance = GetModuleHandleW(nullptr);
				wndClass.lpfnWndProc = WndProc;
				wndClass.lpszClassName = nameBuffer;
				wndClass.hbrBackground = CreateSolidBrush(desc.backGroundColor);
				auto Style = WS_CAPTION | WS_SYSMENU;
				auto StyleEX = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE | WS_EX_ACCEPTFILES;
				if (desc.withMaxBox)
					Style |= WS_MAXIMIZEBOX;
				if (desc.withMinBox)
					Style |= WS_MINIMIZEBOX;

				if (!desc.withCaption)
				{
					Style = WS_POPUP;
					StyleEX = WS_EX_APPWINDOW | WS_EX_ACCEPTFILES;
				}
				if (desc.sizeable)
					Style |= WS_SIZEBOX;

				//The GetSystemMetrics function returns values for the primary monitor
					//如果想指定具体的显示器,请自己计算数值
				int screenwidth = GetSystemMetrics(SM_CXFULLSCREEN);
				int screenheight = GetSystemMetrics(SM_CYFULLSCREEN);

				if (desc.align == WindowDesc::Center)
				{
					desc.posY = (screenheight - desc.height) / 2;
					desc.posX = (screenwidth - desc.width) / 2;
				}
				else
				{
					if (desc.align & WindowDesc::Left)
						desc.posX = 0;
					if (desc.align & WindowDesc::Top)
						desc.posY = 0;
					if (desc.align & WindowDesc::Right)
						desc.posX = screenwidth - desc.width;
					if (desc.align & WindowDesc::Buttom)
						desc.posY = screenheight - desc.height;
				}

				HRESULT hr = RegisterClassExA(&wndClass);

				if (FAILED(hr))throw std::exception();

				auto rect = RECT({ desc.posX, desc.posY,desc.width + desc.posX, desc.height + desc.posY });
				AdjustWindowRectEx(&rect, Style, false, StyleEX);

				HWND hwnd = CreateWindowExA(
					StyleEX,
					nameBuffer,
					desc.tittle.data(),
					Style,
					rect.left, rect.top,
					rect.right - rect.left,
					rect.bottom - rect.top,
					0, 0,
					::GetModuleHandleA(0),
					0);
				if (hwnd == 0)
				{
					UnregisterClassA(nameBuffer, ::GetModuleHandleA(0));
					throw std::exception();
				}

				MSG msg = { 0 };
				PeekMessageA(&msg, hwnd, 0, 0, PM_NOREMOVE);// BUG修复日志：2019-5-31
															// =====================
															// 访问一次消息队列让Windows初始化队列，
															// 如果不这么做，多核心异步模型下外部程序
															// 对此窗口的发送的消息有可能被忽略
															// =====================
															// 例如：初始化之后立即析构时发送的WM_QUIT
															// 因为消息队列未初始化无法处理,会导致主线程阻塞。
				promise.set_value(hwnd);

				while (GetMessageA(&msg, NULL, 0, 0) != 0) // MSDN : 如果消息是WM_QUIT，这函数返回0
				{
					TranslateMessage(&msg);
					DispatchMessageA(&msg);
				}

				UnregisterClassA(nameBuffer, ::GetModuleHandleA(0));  //程序准备结束，注销窗口类
			}, std::move(promise), _desc));
		future.wait();
		auto hwnd = future.get();
		WindowList[hwnd] = this;
		this->windowHandle = hwnd;
	}
	WindowDesc CreateByTittle(const std::string& tittle)
	{
		WindowDesc wd;
		wd.tittle = tittle;
		return wd;
	}

	Window::Window(const std::string& tittle)
		: Window(CreateByTittle(tittle)) {}

	Window::~Window()
	{
		PostMessageA((HWND)windowHandle, WM_QUIT, 0, 0);
		if (windowThread.joinable())
			windowThread.join();
	}

	Window::Window(Window&& rhs) :windowHandle(rhs.windowHandle),
		windowThread(std::move(windowThread)), callback(rhs.callback)
	{
		rhs.windowHandle = nullptr;
		rhs.callback = nullptr;
	}

	bool DefaultWindowCallback(const WindowEvent&, uint32_t)
	{
		return 0;
	}

	WindowCallback _get_callback(const Window* w)
	{
		if (w == nullptr)return DefaultWindowCallback;
		return w->callback;
	}

	void Window::Hide()
	{
		ShowWindow((HWND)this->windowHandle, SW_HIDE);
	}

	void Window::Show()
	{
		ShowWindow((HWND)this->windowHandle, SW_SHOW);
	}

	void Window::SetMax()
	{
		ShowWindow((HWND)windowHandle, SW_MAXIMIZE);
	}

	void Window::SetMin()
	{
		ShowWindow((HWND)windowHandle, SW_MINIMIZE);
	}

	void Window::RestoreSize()
	{
		ShowWindow((HWND)windowHandle, SW_RESTORE);
	}

	void Window::WaitUntillDestroy()
	{
		if (windowThread.joinable())
			windowThread.join();
	}

	void Window::SetIcon(std::string IconFilePath) noexcept
	{
		auto hIcon = LoadImageA(::GetModuleHandleA(0), IconFilePath.data(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_LOADTRANSPARENT | LR_DEFAULTSIZE);
		this->SetIcon(hIcon);
	}

	void Window::SetCursor(std::string IconFilePath) noexcept
	{
		auto hCursor = LoadImageA(::GetModuleHandleA(0), IconFilePath.data(), IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE | LR_LOADTRANSPARENT | LR_DEFAULTSIZE);
		this->SetCursor(hCursor);
	}

	Window::Handle Window::GetWindowHandle() noexcept
	{
		return windowHandle;
	}

	void Window::SetIcon(Handle hIcon) noexcept
	{
		SendMessageA((HWND)windowHandle, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
		SendMessageA((HWND)windowHandle, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
		SendMessageA((HWND)windowHandle, WM_SETICON, ICON_SMALL2, (LPARAM)hIcon);
	}

	void Window::SetCursor(Handle hCursor) noexcept
	{
		SendMessageA((HWND)windowHandle, WM_SETCURSOR, (WPARAM)hCursor, 0);
	}

	void Window::SetTittle(const std::string& tittle) noexcept
	{
		SendMessageA((HWND)windowHandle, WM_SETTEXT, 0, (LPARAM)tittle.data());
	}

	WindowDesc::WindowDesc()
	{
		callback = DefaultWindowCallback;
	}
	Window::Point Window::GetPos()
	{
		POINT origin{ 0, 0 };
		ClientToScreen((HWND)this->windowHandle, &origin);
		return Point({ origin.x,origin.y });
	}
	void Window::SetPos(int x, int y)
	{
		auto Style = GetWindowLongA((HWND)this->windowHandle, GWL_STYLE);
		auto StyleEX = GetWindowLongA((HWND)this->windowHandle, GWL_EXSTYLE);
		RECT rc;
		GetClientRect((HWND)this->windowHandle, &rc);
		auto dx = x - rc.left;
		auto dy = y - rc.top;
		AdjustWindowRectEx(&rc, Style, false, StyleEX);
		MoveWindow((HWND)this->windowHandle, rc.left + dx, rc.top + dy, rc.right - rc.left, rc.bottom - rc.top, false);
	}
	Window::Size Window::GetSize()
	{
		RECT rc;
		GetClientRect((HWND)this->windowHandle, &rc);
		return Size({ rc.right - rc.left,rc.bottom - rc.top });
	}
	void Window::SetSize(int width, int height)
	{
		auto Style = GetWindowLongA((HWND)this->windowHandle, GWL_STYLE);
		auto StyleEX = GetWindowLongA((HWND)this->windowHandle, GWL_EXSTYLE);
		auto pos = this->GetPos();

		RECT rc{ 0 };
		rc.left = pos.x;
		rc.top = pos.y;
		rc.right = width + rc.left;
		rc.bottom = height + rc.top;
		AdjustWindowRectEx(&rc, Style, false, StyleEX);
		MoveWindow((HWND)this->windowHandle, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, true);
	}
}