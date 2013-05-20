#include "AbstractScene.h"



/**
 * @brief constructor
 *
 */
AbstractScene::AbstractScene() :
	m_pSoundBgm(0)
{
	// ���ɏ����͖���
}


/**
 * @brief destructor
 *
 */
AbstractScene::~AbstractScene()
{
	this->finalize();
}


/**
 * @brief �V�[���̘_���I�ȏ�����
 *
 */
void AbstractScene::initializeGameScene()
{
}


/**
 * @brief �I�����̏���
 *
 */
void AbstractScene::finalize()
{
	SAFE_DELETE(m_pSoundBgm);
}


/**
 * @brief BGM�̓ǂݍ���
 *
 */
void AbstractScene::loadBgm(CSoundManager* pSoundManager, const acut::TString& bgmFilePath)
{
	m_pSoundBgm = acut::SoundUtility::loadSound(
		pSoundManager,
		bgmFilePath
	);
	if (NULL == m_pSoundBgm) {
		throw acut::Exception(__FILEW__, __LINE__, __FUNCTIONW__);
	}
}


/**
 * @brief BGM�̍Đ��J�n
 *
 */
void AbstractScene::playBgm()
{
	V_THROW(m_pSoundBgm->Reset());
	V_THROW(m_pSoundBgm->Play(0, DSBPLAY_LOOPING, 0));
}


/**
 * @brief BGM�̒�~
 *
 */
void AbstractScene::stopBgm()
{
	V_THROW(m_pSoundBgm->Stop());
}
