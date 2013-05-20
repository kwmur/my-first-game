#ifndef ACUT_GAME_APPLICATION_H__
#define ACUT_GAME_APPLICATION_H__



#include "scene/TitleScene.h"
#include "scene/MainScene.h"
#include "scene/ResultScene.h"



class GameApplication {
public:
	// ������ 
	static void initialize();
	static void initializeSound();
	static void finalize();

	// call back functions 11
	static void GameApplication::setCallbackFunctions();

	static bool CALLBACK isDeviceAcceptable(
		D3DCAPS9* pCaps,
		D3DFORMAT AdapterFormat, 
		D3DFORMAT BackBufferFormat,
		bool bWindowed,
		void* pUserContext
	);

	static bool CALLBACK modifyDeviceSettings(
		DXUTDeviceSettings* pDeviceSettings,
		void* pUserContext
	);

	static HRESULT CALLBACK onDeviceCreated(
		IDirect3DDevice9* pd3dDevice, 
		const D3DSURFACE_DESC* pBackBufferSurfaceDesc, 
		void* pUserContext
	);

	static HRESULT CALLBACK onDeviceReset(
		IDirect3DDevice9* pd3dDevice, 
		const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
		void* pUserContext
	);

	static void CALLBACK onFrameMove(
		double fTime, 
		float fElapsedTime, 
		void* pUserContext
	);

	static void CALLBACK onFrameRender(
		IDirect3DDevice9* pd3dDevice,
		double fTime, 
		float fElapsedTime, 
		void* pUserContext
	);

	static void CALLBACK onDeviceLost(void* pUserContext);

	static void CALLBACK onDeviceDestroyed(void* pUserContext);

	static LRESULT CALLBACK msgProc(
		HWND hWnd,
		UINT uMsg, 
		WPARAM wParam,
		LPARAM lParam, 
		bool* pbNoFurtherProcessing, 
		void* pUserContext
	);

	static void CALLBACK keyboardProc(
		UINT nChar, 
		bool bKeyDown, 
		bool bAltDown, 
		void* pUserContext
	);

	static void CALLBACK onGUIEvent(
		UINT nEvent, 
		int nControlID, 
		CDXUTControl* pControl, 
		void* pUserContext
	);
	// CALLBACK�����܂�


	static void initializeMousePosition();

	static void addScore(const int score);

	static void changeScene(AbstractScene* pScene);


	// static�����o�ϐ�
	static CSoundManager s_soundManager; //!< sound
	static bool s_bPlaySounds; // whether to play sounds
	static double s_lastAnimTime; // Time for the animations
	static int s_score; //!< �_��
	// scene
	static TitleScene s_titleScene; //!< �^�C�g�����
	static MainScene s_mainScene; //!< ���C���Q�[��
	static ResultScene s_resultScene; //!< �X�R�A���ʉ��
	// mouse
	static int s_cursorPositionX; //!< �}�E�X�|�C���^��X���W
	static int s_cursorPositionY; //!< �}�E�X�|�C���^��Y���W

private:	
	// �C���X�^���X�����Ή�
	GameApplication();
    GameApplication(const GameApplication&);
    GameApplication& operator=(const GameApplication&);
	~GameApplication();


	// static�����o�ϐ�
	// scene
	static AbstractScene* s_pCurrentScene; //!< ���݂̃V�[���ւ̃|�C���^
	static std::vector<AbstractScene*> s_scenes; //!< �S�ẴV�[��
	typedef std::vector<AbstractScene*>::iterator SceneIterator; //!< �V�[���̃C�e���[�^
};



#endif // #ifndef ACUT_GAME_APPLICATION_H__
