#include "GameConfiguration.h"
#include "GameApplication.h"



/**
 * @brief
 * Entry point to the program. Initializes everything and goes into a message processing
 * loop. Idle time is used to render the scene.
 *
 */
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// Show the cursor and clip it when in full screen
	DXUTSetCursorSettings(true, true);

	// Initialize DXUT and create the desired Win32 window and Direct3D
	// device for the application. Calling each of these functions is optional, but they
	// allow you to set several options which control the behavior of the framework.
	DXUTInit(true, true); // Parse the command line and show msgboxes
	DXUTSetHotkeyHandling(true, true, true);  // handle the defaul hotkeys
	DXUTCreateWindow(GameConfiguration::s_GAME_TITLE.c_str());

	try {
		// We need to set up DirectSound after we have a window.
		GameApplication::initialize();
		GameApplication::setCallbackFunctions();

		DXUTCreateDevice(true, GameConfiguration::s_BACK_BUFFER_WIDTH, GameConfiguration::s_BACK_BUFFER_HEIGHT);

		// Pass control to DXUT for handling the message pump and
		// dispatching render calls. DXUT will call your FrameMove
		// and FrameRender callback when there is idle time between handling window messages.
		DXUTMainLoop();
	} catch (acut::Exception& ex) {
		// TODO: 詳細なメッセージを表示する
		// TODO: log
		acut::ExceptionHandler::handleException(ex);
		MessageBox(NULL, _T("エラーが発生しました。\nゲームを終了します。"), _T("エラー発生"), MB_ICONEXCLAMATION);
		GameApplication::onDeviceLost(NULL);
	}

	try {
		// Perform any application-level cleanup here. Direct3D device resources are released within the
		// appropriate callback functions and therefore don't require any cleanup code here.
		GameApplication::finalize();
	} catch (acut::Exception& ex) {
		acut::ExceptionHandler::handleException(ex);
		MessageBox(NULL, _T("ゲーム終了処理でエラーが発生しました。"), _T("エラー発生"), MB_ICONEXCLAMATION);
	}

	return DXUTGetExitCode();
}
