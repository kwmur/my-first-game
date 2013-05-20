#include "TitleScene.h"
#include "../GameApplication.h"
#include "../GameConfiguration.h"



/**
 * @brief constructor
 *
 */
TitleScene::TitleScene() :
	m_pTexture(0),
	m_pSprite(0)
{
	// 特に処理は無し
}


/**
 * @brief destructor
 *
 */
TitleScene::~TitleScene()
{
	this->finalize();
}


/**
 * @brief 初期化
 *
 */
void TitleScene::initialize()
{
	// 特に処理は無し
}


/**
 * @brief soundの初期化
 *
 */
void TitleScene::initializeSound(CSoundManager* pSoundManager)
{
	try {
		this->loadBgm(
			pSoundManager,
			GameConfiguration::s_TITLE_SCENE_BGM_FILE_PATH
		);
	} catch (acut::Exception&) {
		MessageBox(NULL, _T("BGMファイルの読み込みでエラーが発生しました。"), _T("エラー発生"), MB_ICONEXCLAMATION);
		throw;
	}
}


/**
 * @brief 終了
 *
 */
void TitleScene::finalize()
{
	AbstractScene::finalize();
}


/**
 * @brief デバイス作成後に呼び出される
 *
 */
HRESULT TitleScene::onDeviceCreated(
	IDirect3DDevice9* pDevice, 
	const D3DSURFACE_DESC* pBackBufferSurfaceDesc, 
	void* pUserContext
)
{
	HRESULT hr = D3D_OK;

	//V_RETURN(D3DXCreateTextureFromFile(pDevice, _T("media\\images\\title.bmp"), &m_pTexture));
	V_RETURN(D3DXCreateTextureFromFileEx(
		pDevice,
		GameConfiguration::s_TITLE_SCENE_IMAGE_FILE_PATH.c_str(),
		GameConfiguration::s_BACK_BUFFER_WIDTH,
		GameConfiguration::s_BACK_BUFFER_HEIGHT,
		D3DX_DEFAULT,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_FILTER_POINT, //D3DX_DEFAULT,
		D3DX_DEFAULT,
		0,
		NULL,
		NULL,
		&m_pTexture
	));

	return hr;
}


/**
 * @brief デバイスのリセット時に呼ばれる
 *
 */
HRESULT TitleScene::onDeviceReset(
	IDirect3DDevice9* pd3dDevice, 
	const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
	void* pUserContext
)
{
	HRESULT hr = D3D_OK;

	V_RETURN(D3DXCreateSprite(DXUTGetD3D9Device(), &m_pSprite));

	return hr;
}


/**
 * @brief タイトル画面の更新
 *
 */
void TitleScene::onFrameMove(double time, float elapsedTime, void* pUserContext)
{
	GameApplication::initializeMousePosition();
}


/**
 * @brief タイトル画面の描画
 *
 */
void TitleScene::onFrameRender(
	IDirect3DDevice9* pDevice,
	double time, 
	float elapsedTime, 
	void* pUserContext
)
{
	V_THROW(pDevice->Clear(
		0L,
		NULL, 
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_ARGB(0, 0x3F, 0xAF, 0xFF),
		1.0f,
		0L
	));

	if (SUCCEEDED(pDevice->BeginScene())) {
		// sprite の描画
		V_THROW(m_pSprite->Begin(NULL));
		V_THROW(m_pSprite->Draw(m_pTexture , NULL, NULL, NULL, 0xFFFFFFFF));
		V_THROW(m_pSprite->End());

		V_THROW(pDevice->EndScene());
	}
}


/**
 * @brief Device ロスト時に呼ばれる
 * 
 * onDeviceReset() で取得したリソースをここで解放する
 *
 */
void TitleScene::onDeviceLost(void* pUserContext)
{
	SAFE_RELEASE(m_pSprite);
}


/**
 * @brief Direct3D device の解体後に呼ばれる
 *
 * onDeviceCreated() で取得したリソースをここで解放する
 *
 */
void TitleScene::onDeviceDestroyed(void* pUserContext)
{
	SAFE_RELEASE(m_pTexture);
}


/**
 * @brief window メッセージが送られてきたときに呼び出される
 *
 */
LRESULT TitleScene::msgProc(
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam,
	bool* pbNoFurtherProcessing,
	void* pUserContext
)
{
	// 左クリックされたら、メインシーンに移行する
	if (WM_LBUTTONDOWN == uMsg) {
		GameApplication::changeScene(&GameApplication::s_mainScene);
	}

	return 0;
}


/**
 * @brief キーボード押下時に呼ばれる
 *
 */
void TitleScene::keyboardProc(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext)
{
	// 特に処理は無し
}


/**
 * @brief DXUTのGUIパーツをクリックした時に呼ばれる
 *
 */
void TitleScene::onGUIEvent(
	UINT nEvent,
	int nControlID,
	CDXUTControl* pControl,
	void* pUserContext
)
{
	// 特に処理は無し
}
