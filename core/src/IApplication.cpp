#include "../include/IApplication.h"

IApplication::IApplication() :
	m_iWidth(0),
	m_iHeight(0),
	m_Window(nullptr)
{
}

IApplication::~IApplication()
{
}

bool IApplication::Create(int32_t resX, int32_t resY, const std::string& title)
{
	m_Window = MakeWindow(resX, resY, title);

	if (!m_Window) {
		return false;
	}

	m_iWidth = resX;
	m_iHeight = resY;

	return true;
}

void IApplication::Run()
{
	MSG msg;
	BOOL gotMsg = false;
	::PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE);

	while (msg.message != WM_QUIT) 
	{
		gotMsg = ::GetMessage(&msg, nullptr, 0, 0);

		if (gotMsg) 
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
}

void IApplication::Debug(const wchar_t* msg)
{
	::OutputDebugStringW(msg);
}

void IApplication::Debug(const char* msg)
{
	::OutputDebugStringA(msg);
}

void IApplication::Debug(const std::string& msg)
{
	::OutputDebugStringA(msg.c_str());
}

HWND IApplication::MakeWindow(int32_t width, int32_t height, const std::string& title)
{
	HINSTANCE inst = ::GetModuleHandle(nullptr);
	DWORD windowStyle = 0;

	windowStyle = WS_OVERLAPPED |
		WS_CAPTION |
		WS_SYSMENU |
		WS_THICKFRAME |
		WS_MINIMIZEBOX |
		WS_MAXIMIZEBOX;

	WNDCLASS wc;
	memset(&wc, 0, sizeof(WNDCLASS));
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.hInstance = inst;
	wc.hIcon = ::LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) ::GetStockObject(BLACK_BRUSH);
	wc.lpszClassName = L"GRAPHICSENGINE_WNDCLASS";
	
	if (!::RegisterClass(&wc)) {
		Debug("Failed to register window class, exiting app...");
		return nullptr;
	}

	// Create the actual window :P
	auto widestr = std::wstring(title.begin(), title.end());
	HWND window = ::CreateWindow(
		wc.lpszClassName,
		widestr.c_str(),
		windowStyle,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width,
		height,
		nullptr,
		nullptr,
		inst,
		nullptr);

	if (!window) {
		Debug("Failed to create window, exiting app...");
		return nullptr;
	}

	::SetWindowLong(window, GWL_STYLE, windowStyle);
	RECT clientArea = { 0,0, width, height };
	::AdjustWindowRectEx(&clientArea, windowStyle, FALSE, 0);
	::SetWindowPos(
		window,
		nullptr,
		0,
		0,
		clientArea.right - clientArea.left,
		clientArea.bottom - clientArea.top,
		SWP_NOZORDER | SWP_NOMOVE | SWP_SHOWWINDOW);
	::UpdateWindow(window);
	::ShowWindow(window, SW_SHOWNORMAL);

	return window;
}

long __stdcall IApplication::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
		break;
	case WM_CREATE:
		::SetForegroundWindow(hwnd);
		break;
	case WM_CLOSE:
		::DestroyWindow(hwnd);
		return 0;
		break;
	default:
		break;
	}

	return (long)::DefWindowProc(hwnd, message, wParam, lParam);
}
