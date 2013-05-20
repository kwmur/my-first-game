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
	// ���ɏ����͖���
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
 * @brief ������
 *
 */
void TitleScene::initialize()
{
	// ���ɏ����͖���
}


/**
 * @brief sound�̏�����
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
		MessageBox(NULL, _T("BGM�t�@�C���̓ǂݍ��݂ŃG���[���������܂����B"), _T("�G���[����"), MB_ICONEXCLAMATION);
		throw;
	}
}


/**
 * @brief �I��
 *
 */
void TitleScene::finalize()
{
	AbstractScene::finalize();
}


/**
 * @brief �f�o�C�X�쐬��ɌĂяo�����
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
 * @brief �f�o�C�X�̃��Z�b�g���ɌĂ΂��
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
 * @brief �^�C�g����ʂ̍X�V
 *
 */
void TitleScene::onFrameMove(double time, float elapsedTime, void* pUserContext)
{
	GameApplication::initializeMousePosition();
}


/**
 * @brief �^�C�g����ʂ̕`��
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
		// sprite �̕`��
		V_THROW(m_pSprite->Begin(NULL));
		V_THROW(m_pSprite->Draw(m_pTexture , NULL, NULL, NULL, 0xFFFFFFFF));
		V_THROW(m_pSprite->End());

		V_THROW(pDevice->EndScene());
	}
}


/**
 * @brief Device ���X�g���ɌĂ΂��
 * 
 * onDeviceReset() �Ŏ擾�������\�[�X�������ŉ������
 *
 */
void TitleScene::onDeviceLost(void* pUserContext)
{
	SAFE_RELEASE(m_pSprite);
}


/**
 * @brief Direct3D device �̉�̌�ɌĂ΂��
 *
 * onDeviceCreated() �Ŏ擾�������\�[�X�������ŉ������
 *
 */
void TitleScene::onDeviceDestroyed(void* pUserContext)
{
	SAFE_RELEASE(m_pTexture);
}


/**
 * @brief window ���b�Z�[�W�������Ă����Ƃ��ɌĂяo�����
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
	// ���N���b�N���ꂽ��A���C���V�[���Ɉڍs����
	if (WM_LBUTTONDOWN == uMsg) {
		GameApplication::changeScene(&GameApplication::s_mainScene);
	}

	return 0;
}


/**
 * @brief �L�[�{�[�h�������ɌĂ΂��
 *
 */
void TitleScene::keyboardProc(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext)
{
	// ���ɏ����͖���
}


/**
 * @brief DXUT��GUI�p�[�c���N���b�N�������ɌĂ΂��
 *
 */
void TitleScene::onGUIEvent(
	UINT nEvent,
	int nControlID,
	CDXUTControl* pControl,
	void* pUserContext
)
{
	// ���ɏ����͖���
}
