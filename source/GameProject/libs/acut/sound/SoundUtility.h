#ifndef ACUT_SOUND_UTILITY_H__
#define ACUT_SOUND_UTILITY_H__



namespace acut {


	class SoundUtility {
	public:
		static CSound* loadSound(CSoundManager* pSoundManager, const acut::TString& soundFilePath);

	private:
		// �C���X�^���X�����Ή�
		SoundUtility();
		~SoundUtility();
		SoundUtility(const SoundUtility&);
		SoundUtility& operator=(const SoundUtility&);
	};


};



#endif // #ifndef ACUT_SOUND_UTILITY_H__
