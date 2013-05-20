#ifndef ACUT_TITLE_SCENE_H__
#define ACUT_TITLE_SCENE_H__



#include "AbstractScene.h"



/**
 * @brief タイトル画面表示用クラス
 *
 */
class TitleScene : public AbstractScene {
public:
	TitleScene();
	virtual ~TitleScene();

	virtual void initialize();
	virtual void initializeSound(CSoundManager* pSoundManager);
	virtual void finalize();

	// CALLBACK関数
	virtual HRESULT onDeviceCreated(
		IDirect3DDevice9* pDevice, 
		const D3DSURFACE_DESC* pBackBufferSurfaceDesc, 
		void* pUserContext
	);

	virtual HRESULT onDeviceReset(
		IDirect3DDevice9* pd3dDevice, 
		const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
		void* pUserContext
	);

	virtual void onFrameMove(double time, float elapsedTime, void* pUserContext);

	virtual void onFrameRender(
		IDirect3DDevice9* pDevice,
		double time, 
		float elapsedTime, 
		void* pUserContext
	);

	virtual void onDeviceLost(void* pUserContext);

	virtual void onDeviceDestroyed(void* pUserContext);

	virtual LRESULT msgProc(
		HWND hWnd,
		UINT uMsg, 
		WPARAM wParam,
		LPARAM lParam, 
		bool* pbNoFurtherProcessing, 
		void* pUserContext
	);

	virtual void keyboardProc(
		UINT nChar, 
		bool bKeyDown, 
		bool bAltDown, 
		void* pUserContext
	);

	virtual void onGUIEvent(
		UINT nEvent,
		int nControlID,
		CDXUTControl* pControl,
		void* pUserContext
	);
	// CALLBACK関数ここまで

private:
	LPDIRECT3DTEXTURE9 m_pTexture; //!< テクスチャ
	LPD3DXSPRITE m_pSprite; //!< スプライト

};



#endif // #ifndef ACUT_TITLE_SCENE_H__
