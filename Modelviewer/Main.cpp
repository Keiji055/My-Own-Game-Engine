#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#include "resource.h"
#include "Core/Modelviewer.h"
#include <CommonUtilities/InputHandler.h>
#include <Imgui/imgui_impl_win32.h>
#include <Imgui/imgui_impl_dx11.h>
#include <shellapi.h>
#include <filesystem>
#include <system_error>

namespace fs = std::filesystem;
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
std::shared_ptr<CommonUtilities::InputHandler> inputHandler = std::make_shared<CommonUtilities::InputHandler>();

LRESULT CALLBACK WinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	constexpr SIZE windowSize = { 1920, 1080 };
	constexpr LPCWSTR windowTitle = L"Modelviewer";

	ModelViewer& MV = ModelViewer::Get();

	FILE* consoleOut;
	AllocConsole();
	freopen_s(&consoleOut, "CONOUT$", "w", stdout);
	setvbuf(consoleOut, nullptr, _IONBF, 1024);

	HWND consoleWindow = GetConsoleWindow();
	RECT consoleSize;
	GetWindowRect(consoleWindow, &consoleSize);
	MoveWindow(consoleWindow, consoleSize.left, consoleSize.top, 1280, 720, true);

	MV.Initialize(hInstance, windowSize, WinProc, windowTitle, inputHandler);


	return MV.Run();
}

LRESULT CALLBACK WinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	if (uMsg == WM_DESTROY || uMsg == WM_CLOSE)
	{
		PostQuitMessage(0);
	}
	if (uMsg == WM_CREATE)
	{
		DragAcceptFiles(hWnd, TRUE);
		// SetWindowSubclass(hWnd, StaticWndProc, uIdSubclass, dwRefData);
		 //CreateWindow(TEXT("edit"), NULL,
		 //    WS_CHILD | WS_VISIBLE |
		 //    ES_AUTOHSCROLL | ES_AUTOVSCROLL,
		 //    5, 5, 300, 18, hWnd, (HMENU)0,
		 //    ((LPCREATESTRUCT)lParam)->hInstance, //instance handle
		 //    NULL);

		 //return 0;
	}
	if (uMsg == WM_DROPFILES)
	{
		HDROP hDrop = reinterpret_cast<HDROP>(wParam);

		// extract files here
		std::vector<std::string> files;
		std::string filename;

		UINT count = DragQueryFileA(hDrop, -1, NULL, 0);
		for (UINT i = 0; i < count; ++i)
		{
			UINT size = DragQueryFileA(hDrop, i, NULL, 0);
			if (size > 0)
			{
				filename.resize(size);
				DragQueryFileA(hDrop, i, &filename[0], size + 1);
				files.push_back(filename);
				if (fs::path(filename).extension() == ".dds")
				{
					fs::path file = fs::path(filename).filename();
					fs::path p = fs::current_path();
					fs::path rp = p.parent_path();
					fs::path to = rp.parent_path();
					to += "/Assets/Textures/";
					if (fs::exists(to))
					{
						if (fs::exists(fs::path(to += file)))
						{
							int msgBoxID = MessageBox(NULL, L"File already exists in directory!", L"File Handler", MB_ICONWARNING | MB_YESNO);
							switch (msgBoxID)
							{
							case IDYES:
								fs::copy(fs::path(filename), to, fs::copy_options::overwrite_existing);
								break;

							case IDNO:

								break;
							default:
								break;
							}
						}
						else
						{
							fs::copy(fs::path(filename), to);
						}
					}
				}
			}
		}

		DragFinish(hDrop);
		return 0;

	}
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	if (inputHandler->UpdateEvents(uMsg, wParam, lParam))
	{
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}