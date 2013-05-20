#ifndef ACUT_MAIN_SCENE_H__
#define ACUT_MAIN_SCENE_H__



#include "AbstractScene.h"
#include "../actor/Bullet.h"
#include "../actor/Floor.h"
#include "../actor/Enemy.h"
#include "../actor/Player.h"



/**
 * @brief ゲームのメインシーン
 *
 */
class MainScene : public AbstractScene {
public:
	MainScene();
	virtual ~MainScene();

	virtual void initialize();
	virtual void initializeSound(CSoundManager* pSoundManager);
	virtual void initializeGameScene();
	virtual void finalize();

	// CALLBACK関数
	virtual HRESULT onDeviceCreated(
		IDirect3DDevice9* pd3dDevice, 
		const D3DSURFACE_DESC* pBackBufferSurfaceDesc, 
		void* pUserContext
	);

	virtual HRESULT onDeviceReset(
		IDirect3DDevice9* pd3dDevice, 
		const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
		void* pUserContext
	);

	virtual void onFrameMove(
		double fTime, 
		float fElapsedTime, 
		void* pUserContext
	);

	virtual void onFrameRender(
		IDirect3DDevice9* pd3dDevice,
		double fTime, 
		float fElapsedTime, 
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
	// コピー未対応
	MainScene(const MainScene&);
	MainScene& operator=(const MainScene&);

	void renderText();

	// ここからメンバ変数
	Player m_player;
	std::vector<Enemy> m_enemy;
	//std::vector<int> m_buildings;
	std::vector<Bullet> m_bullets;
	Floor m_floor;


	ID3DXEffect* m_pEffect; // D3DX effect interface

	CFirstPersonCamera m_Camera; // A model viewing camera
	ID3DXFont* m_pFont; // Font for drawing text
	ID3DXSprite* m_pTextSprite; // Sprite for batching draw text calls
	CDXUTDialogResourceManager m_DialogResourceManager; // manager for shared resources of dialogs
	CD3DSettingsDlg m_SettingsDlg; // Device settings dialog
	CDXUTDialog m_HUD; // dialog for standard controls
	CDXUTDialog m_SampleUI; // dialog for sample specific controls
};



#endif // #ifndef ACUT_MAIN_SCENE_H__
