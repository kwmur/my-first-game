#include "../acut.h"



namespace acut {


	/**
	 * @brief soundファイルの読み込み .wavファイルのみ対応
	 *
	 * @param soundManager
	 * @param soundFilePath
	 * @return CSoundへのポインタ
	 *
	 */
	CSound* SoundUtility::loadSound(CSoundManager* pSoundManager, const acut::TString& argSoundFilePath)
	{
		// CSoundManager::Create()の引数の型を合わせる為、コピーする
		TCHAR soundFilePath[MAX_PATH];
		_tcsncpy_s(soundFilePath, argSoundFilePath.c_str(), MAX_PATH);

		CSound* pSound = 0;
		HRESULT hr;
		V(pSoundManager->Create(
			&pSound,
			soundFilePath,
			DSBCAPS_CTRLVOLUME
		));
		if (FAILED(hr)) {
			throw acut::Exception();
		}

		return pSound;
	}


};
