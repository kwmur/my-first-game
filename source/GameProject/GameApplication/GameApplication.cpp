/** @file
 * @brief GameApplicationの実装
 *
 */



#include "GameApplication.h"
#include "GameConfiguration.h"



// static メンバ変数
CSoundManager GameApplication::s_soundManager;
bool GameApplication::s_bPlaySounds = true; // whether to play sounds
double GameApplication::s_lastAnimTime = 0.0; // Time for the animations
int GameApplication::s_cursorPositionX = 0;
int GameApplication::s_cursorPositionY = 0;
int GameApplication::s_score = 0;
// scene
AbstractScene* GameApplication::s_pCurrentScene = 0;
std::vector<AbstractScene*> GameApplication::s_scenes;
TitleScene GameApplication::s_titleScene;
MainScene GameApplication::s_mainScene;
ResultScene GameApplication::s_resultScene;


/**
 * @brief コールバック関数をDXUTへ登録する
 *
 */
void GameApplication::setCallbackFunctions()
{
	// コールバック関数をセット

	// Set the callback functions. These functions allow DXUT to notify
	// the application about device changes, user input, and windows messages.  The
	// callbacks are optional so you need only set callbacks for events you're interested
	// in. However, if you don't handle the device reset/lost callbacks then the sample
	// framework won't be able to reset your device since the application must first
	// release all device resources before resetting.  Likewise, if you don't handle the
	// device created/destroyed callbacks then DXUT won't be able to
	// recreate your device resources.
	DXUTSetCallbackD3D9DeviceAcceptable(GameApplication::isDeviceAcceptable);
	DXUTSetCallbackDeviceChanging(GameApplication::modifyDeviceSettings);
	DXUTSetCallbackD3D9DeviceCreated(GameApplication::onDeviceCreated);
	DXUTSetCallbackD3D9DeviceDestroyed(GameApplication::onDeviceDestroyed);
	DXUTSetCallbackD3D9DeviceReset(GameApplication::onDeviceReset);
	DXUTSetCallbackD3D9DeviceLost(GameApplication::onDeviceLost);
	DXUTSetCallbackFrameMove(GameApplication::onFrameMove);
	DXUTSetCallbackD3D9FrameRender(GameApplication::onFrameRender);
	DXUTSetCallbackMsgProc(GameApplication::msgProc);
	DXUTSetCallbackKeyboard(GameApplication::keyboardProc);
}


/**
 * @brief ゲームの初期化処理
 *
 */
void GameApplication::initialize()
{
	// scene pointer のセット
	s_scenes.push_back(&s_titleScene);
	s_scenes.push_back(&s_mainScene);
	s_scenes.push_back(&s_resultScene);
	s_pCurrentScene = &s_titleScene;

	// TODO: debug用
	//s_pCurrentScene = &s_mainScene;
	//s_pCurrentScene = &s_resultScene;

	// 初期化
	std::for_each(s_scenes.begin(), s_scenes.end(), std::mem_fun(&AbstractScene::initialize));
	GameApplication::initializeSound();
	s_pCurrentScene->initializeGameScene();
}


/**
 * @brief 
 * サウンドマネージャ初期化と
 * サウンドファイル読み込みなどの初期化処理
 *
 */
void GameApplication::initializeSound()
{
	V_THROW(s_soundManager.Initialize(DXUTGetHWND(), DSSCL_PRIORITY));
	std::for_each(
		s_scenes.begin(),
		s_scenes.end(),
		std::bind2nd(std::mem_fun(&AbstractScene::initializeSound), &s_soundManager)
	);
}


/**
 * @brief マウス位置の初期化
 *
 */
void GameApplication::initializeMousePosition()
{
	// マウスポインタの位置を初期化
	// window の位置を取得
	RECT windowRect;
	if (!GetWindowRect(DXUTGetHWND(), &windowRect)) {
		throw acut::Exception(__FILEW__, __LINE__, __FUNCTIONW__);
	}

	// マウスポインタの位置を初期化
	GameApplication::s_cursorPositionX = windowRect.left + GameConfiguration::s_BACK_BUFFER_WIDTH / 2;
	GameApplication::s_cursorPositionY = windowRect.top + GameConfiguration::s_BACK_BUFFER_HEIGHT / 2;
	if (!SetCursorPos(GameApplication::s_cursorPositionX, GameApplication::s_cursorPositionY)) {
		throw acut::Exception(__FILEW__, __LINE__, __FUNCTIONW__);
	}

	// 非表示にする
	//DXUTGetD3D9Device()->ShowCursor(FALSE);
	ShowCursor(FALSE); // TODO: 戻り値の意味を調べる

	// クリッピング
	if (!ClipCursor(&windowRect)) {
		throw acut::Exception(__FILEW__, __LINE__, __FUNCTIONW__);
	}
}


/**
 * @brief 終了処理
 *
 */
void GameApplication::finalize()
{
	std::for_each(s_scenes.begin(), s_scenes.end(), std::mem_fun(&AbstractScene::finalize));
}


/**
 * @brief
 * Called during device initialization, this code checks the device for some
 * minimum set of capabilities, and rejects those that don't pass by returning false.
 *
 */
bool GameApplication::isDeviceAcceptable(
	D3DCAPS9* pCaps,
	D3DFORMAT AdapterFormat,
	D3DFORMAT BackBufferFormat,
	bool bWindowed,
	void* pUserContext
)
{
	// Skip backbuffer formats that don't support alpha blending
	IDirect3D9* pD3D = DXUTGetD3D9Object();

	if (FAILED(pD3D->CheckDeviceFormat(
		pCaps->AdapterOrdinal,
		pCaps->DeviceType,
		AdapterFormat, 
		D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,
		D3DRTYPE_TEXTURE,
		BackBufferFormat
	))) {
		return false;
	}

	// Pixel shader 2.0 以上が必要
	//if (pCaps->PixelShaderVersion < D3DPS_VERSION(2, 0)) {
	//	return false;
	//}

	// A8R8G8B8 textures のサポートが必要
	if (FAILED(pD3D->CheckDeviceFormat(
		pCaps->AdapterOrdinal, 
		pCaps->DeviceType,
		AdapterFormat,
		0,
		D3DRTYPE_TEXTURE,
		D3DFMT_A8R8G8B8 
	))) {
		return false;
	}

	return true;
}


/**
 * @brief
 * This callback function is called immediately(直ちに，早速) before a device is created to allow the
 * application to modify(修正する，(部分的に)変更する) the device settings. The supplied pDeviceSettings parameter
 * contains the settings that the framework has selected for the new device, and the
 * application can make any desired changes directly to this structure.  Note however that
 * DXUT will not correct invalid device settings so care must be taken
 * to return valid device settings, otherwise IDirect3D9::CreateDevice() will fail.
 * 
 */
bool GameApplication::modifyDeviceSettings(
	DXUTDeviceSettings* pDeviceSettings,
	void* pUserContext
)
{
	assert(DXUT_D3D9_DEVICE == pDeviceSettings->ver); 
	HRESULT hr;
	IDirect3D9* pD3D = DXUTGetD3D9Object();
	D3DCAPS9 caps;

	V(pD3D->GetDeviceCaps(
		pDeviceSettings->d3d9.AdapterOrdinal,
		pDeviceSettings->d3d9.DeviceType,
		&caps
	));

	// If device doesn't support HW T&L or doesn't support 1.1 vertex shaders in HW
	// then switch to SWVP.
	if ((caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0
		|| caps.VertexShaderVersion < D3DVS_VERSION(1, 1)
	) {
		pDeviceSettings->d3d9.BehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	// If the hardware cannot do vertex blending, use software vertex processing.
	if (caps.MaxVertexBlendMatrices < 2) {
		pDeviceSettings->d3d9.BehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	// If using hardware vertex processing, change to mixed vertex processing
	// so there is a fallback.
	if (pDeviceSettings->d3d9.BehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING) {
		pDeviceSettings->d3d9.BehaviorFlags = D3DCREATE_MIXED_VERTEXPROCESSING;
	}

	// Debugging vertex shaders requires either REF or software vertex processing
	// and debugging pixel shaders requires REF.
#ifdef DEBUG_VS
	if (pDeviceSettings->d3d9.DeviceType != D3DDEVTYPE_REF) {
		pDeviceSettings->d3d9.BehaviorFlags &= ~D3DCREATE_HARDWARE_VERTEXPROCESSING;
		pDeviceSettings->d3d9.BehaviorFlags &= ~D3DCREATE_PUREDEVICE;
		pDeviceSettings->d3d9.BehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
#endif
#ifdef DEBUG_PS
	pDeviceSettings->d3d9.DeviceType = D3DDEVTYPE_REF;
#endif

	// For the first device created if its a REF device, optionally display a warning dialog box
	static bool s_bFirstTime = true;
	if (s_bFirstTime) {
		s_bFirstTime = false;
		if (pDeviceSettings->d3d9.DeviceType == D3DDEVTYPE_REF) {
			DXUTDisplaySwitchingToREFWarning(pDeviceSettings->ver);
		}
	}

	return true;
}


/**
 * @brief
 *
 * This callback function will be called immediately after the Direct3D device has been
 * created, which will happen during application initialization and windowed/full screen
 * toggles. This is the best location to create D3DPOOL_MANAGED resources since these
 * resources need to be reloaded whenever the device is destroyed. Resources created
 * here should be released in the OnDestroyDevice callback.
 *
 * @param
 * @param
 * @param
 * @return
 *
 */
HRESULT GameApplication::onDeviceCreated(
	IDirect3DDevice9* pd3dDevice,
	const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
	void* pUserContext
)
{
	GameApplication::initializeMousePosition();

	HRESULT hr = E_FAIL;
	for (SceneIterator it = s_scenes.begin(), itEnd = s_scenes.end();
		it != itEnd;
		++it
	) {
		V_RETURN((*it)->onDeviceCreated(pd3dDevice, pBackBufferSurfaceDesc, pUserContext));
	}

	s_pCurrentScene->playBgm();

	return hr;
}


/**
 * @brief
 * This callback function will be called immediately after the Direct3D device has been
 * reset, which will happen after a lost device scenario. This is the best location to
 * create D3DPOOL_DEFAULT resources since these resources need to be reloaded whenever
 * the device is lost. 
 * Resources created here should be released in the OnLostDevice callback.
 * (ここで作ったResourcesはOnLostDeviceでリリースしないといけない。）
 * この関数は一番最初に呼ばれるっぽい。
 *
 * @param
 * @param
 * @param
 * @return
 *
 */
HRESULT GameApplication::onDeviceReset(
	IDirect3DDevice9* pd3dDevice,
	const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
	void* pUserContext
)
{
	HRESULT hr = E_FAIL;
	for (SceneIterator it = s_scenes.begin(), itEnd = s_scenes.end();
		it != itEnd;
		++it
	) {
		V_RETURN((*it)->onDeviceReset(pd3dDevice, pBackBufferSurfaceDesc, pUserContext));
	}

	return hr;
}


/**
 * @brief
 * This callback function will be called once at the beginning of every frame. This is the
 * best location for your application to handle updates to the scene, but is not
 * intended to contain actual rendering calls, which should instead be placed in the
 * OnFrameRender callback.
 * 
 * この関数は毎フレーム呼ばれる。
 * 
 * @param double fTime
 * @param fload fElapsedTime 経過時間
 * @param void* pUserConatext
 *
 */
void GameApplication::onFrameMove(double fTime, float fElapsedTime, void* pUserContext)
{
	s_pCurrentScene->onFrameMove(fTime, fElapsedTime, pUserContext);

	s_lastAnimTime = fTime;
}


/**
 * @brief
 *
 * This callback function will be called at the end of every frame to perform all the
 * rendering calls for the scene, and it will also be called if the window needs to be
 * repainted. After this function has returned, DXUT will call
 * IDirect3DDevice9::Present to display the contents of the next buffer in the swap chain
 *
 * @param
 * @param
 * @param
 * @param
 * @return 
 *
 */
void GameApplication::onFrameRender(
	IDirect3DDevice9* pd3dDevice,
	double fTime, 
	float fElapsedTime,
	void* pUserContext
)
{
	s_pCurrentScene->onFrameRender(pd3dDevice, fTime, fElapsedTime, pUserContext);

//	s_lastAnimTime = fTime; // TODO: 実行したほうが良い？
}


/**
 * @brief
 *
 * This callback function will be called immediately after the Direct3D device has
 * entered a lost state and before IDirect3DDevice9::Reset is called. Resources created
 * in the OnResetDevice callback should be released here, which generally includes all
 * D3DPOOL_DEFAULT resources. See the "Lost Devices" section of the documentation for
 * information about lost devices.
 *
 * @param
 *
 */
void GameApplication::onDeviceLost(void* pUserContext)
{
	std::for_each(
		s_scenes.begin(),
		s_scenes.end(),
		std::bind2nd(std::mem_fun(&AbstractScene::onDeviceLost), pUserContext)
	);
}


/**
 * @brief
 *
 * This callback function will be called immediately after the Direct3D device has
 * been destroyed, which generally happens as a result of application termination or
 * windowed/full screen toggles. Resources created in the OnCreateDevice callback
 * should be released here, which generally includes all D3DPOOL_MANAGED resources.
 *
 */
void GameApplication::onDeviceDestroyed(void* pUserContext)
{
	std::for_each(
		s_scenes.begin(),
		s_scenes.end(),
		std::bind2nd(std::mem_fun(&AbstractScene::onDeviceDestroyed), pUserContext)
	);
}


/**
 * @brief
 *
 * Before handling window messages, DXUT passes incoming windows
 * messages to the application through this callback function. If the application sets
 * *pbNoFurtherProcessing to TRUE, then DXUT will not process this message.
 *
 * @param
 * @param
 * @param
 * @param
 * @param
 * @param
 * @return
 *
 */
LRESULT GameApplication::msgProc(
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam,
	bool* pbNoFurtherProcessing,
	void* pUserContext
)
{
	return s_pCurrentScene->msgProc(hWnd, uMsg, wParam, lParam, pbNoFurtherProcessing, pUserContext);
}


/**
 * @brief
 *
 * As a convenience, DXUT inspects the incoming windows messages for
 * keystroke messages and decodes the message parameters to pass relevant keyboard
 * messages to the application.  The framework does not remove the underlying keystroke
 * messages, which are still passed to the application's MsgProc callback.
 *
 * @param
 * @param
 * @param
 * @param
 * @return
 *
 */
void GameApplication::keyboardProc(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext)
{
	s_pCurrentScene->keyboardProc(nChar, bKeyDown, bAltDown, pUserContext);
}


/**
 * @brief GUIイベントのハンドル関数
 *
 * @param
 * @param
 * @param
 * @param
 *
 */
void GameApplication::onGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext)
{
	s_pCurrentScene->onGUIEvent(nEvent, nControlID, pControl, pUserContext);
}


/**
 * @brief スコアの加算
 *
 */
void GameApplication::addScore(const int score)
{
	s_score += score;
	DEBUG_OUTPUTF("Total Score : %d\n", s_score);
}


/**
 * @brief シーンの変更
 *
 */
void GameApplication::changeScene(AbstractScene* pNextScene)
{
	s_pCurrentScene->stopBgm();
	s_pCurrentScene = pNextScene;
	s_pCurrentScene->playBgm();
	s_pCurrentScene->initializeGameScene();
}

