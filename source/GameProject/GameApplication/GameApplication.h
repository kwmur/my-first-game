#ifndef ACUT_GAME_APPLICATION_H__
#define ACUT_GAME_APPLICATION_H__



#include "scene/TitleScene.h"
#include "scene/MainScene.h"
#include "scene/ResultScene.h"



class GameApplication {
public:
	// 初期化 
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
	// CALLBACKここまで


	static void initializeMousePosition();

	static void addScore(const int score);

	static void changeScene(AbstractScene* pScene);


	// staticメンバ変数
	static CSoundManager s_soundManager; //!< sound
	static bool s_bPlaySounds; // whether to play sounds
	static double s_lastAnimTime; // Time for the animations
	static int s_score; //!< 点数
	// scene
	static TitleScene s_titleScene; //!< タイトル画面
	static MainScene s_mainScene; //!< メインゲーム
	static ResultScene s_resultScene; //!< スコア結果画面
	// mouse
	static int s_cursorPositionX; //!< マウスポインタのX座標
	static int s_cursorPositionY; //!< マウスポインタのY座標

private:	
	// インスタンス化未対応
	GameApplication();
    GameApplication(const GameApplication&);
    GameApplication& operator=(const GameApplication&);
	~GameApplication();


	// staticメンバ変数
	// scene
	static AbstractScene* s_pCurrentScene; //!< 現在のシーンへのポインタ
	static std::vector<AbstractScene*> s_scenes; //!< 全てのシーン
	typedef std::vector<AbstractScene*>::iterator SceneIterator; //!< シーンのイテレータ
};



#endif // #ifndef ACUT_GAME_APPLICATION_H__
