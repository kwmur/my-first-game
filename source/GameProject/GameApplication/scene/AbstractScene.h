#ifndef ACUT_ABSTRACT_SCENE_H__
#define ACUT_ABSTRACT_SCENE_H__



/**
 * @brief SceneÇÃíäè€äÓíÍÉNÉâÉX
 *
 */
class AbstractScene {
public:
	AbstractScene();
	virtual ~AbstractScene();

	virtual void initialize() = 0;
	virtual void initializeSound(CSoundManager* pSoundManager) = 0;
	virtual void initializeGameScene();
	virtual void finalize();

	// sound
	void playBgm();
	void stopBgm();

	// CALLBACKä÷êî
	virtual HRESULT onDeviceCreated(
		IDirect3DDevice9* pDevice, 
		const D3DSURFACE_DESC* pBackBufferSurfaceDesc, 
		void* pUserContext
	) = 0;

	virtual HRESULT onDeviceReset(
		IDirect3DDevice9* pDevice, 
		const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
		void* pUserContext
	) = 0;

	virtual void onFrameMove(double time, float elapsedTime, void* pUserContext) = 0;

	virtual void onFrameRender(
		IDirect3DDevice9* pd3dDevice,
		double fTime, 
		float fElapsedTime, 
		void* pUserContext
	) = 0;

	virtual void onDeviceLost(void* pUserContext) = 0;

	virtual void onDeviceDestroyed(void* pUserContext) = 0;

	virtual LRESULT msgProc(
		HWND hWnd,
		UINT uMsg, 
		WPARAM wParam,
		LPARAM lParam, 
		bool* pbNoFurtherProcessing, 
		void* pUserContext
	) = 0;

	virtual void keyboardProc(
		UINT nChar, 
		bool bKeyDown, 
		bool bAltDown, 
		void* pUserContext
	) = 0;

	virtual void onGUIEvent(
		UINT nEvent,
		int nControlID,
		CDXUTControl* pControl,
		void* pUserContext
	) = 0;
	// CALLBACKä÷êîÇ±Ç±Ç‹Ç≈

protected:
	void loadBgm(CSoundManager* pSoundManager, const acut::TString& bgmFilePath);

private:
	CSound* m_pSoundBgm; //!< BGM
};



#endif // #ifndef ACUT_ABSTRACT_SCENE_H__
