#include "../acut.h"



namespace acut {


	/**
	 * @brief sound�t�@�C���̓ǂݍ��� .wav�t�@�C���̂ݑΉ�
	 *
	 * @param soundManager
	 * @param soundFilePath
	 * @return CSound�ւ̃|�C���^
	 *
	 */
	CSound* SoundUtility::loadSound(CSoundManager* pSoundManager, const acut::TString& argSoundFilePath)
	{
		// CSoundManager::Create()�̈����̌^�����킹��ׁA�R�s�[����
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
