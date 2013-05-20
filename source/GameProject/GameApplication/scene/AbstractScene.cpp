#include "AbstractScene.h"



/**
 * @brief constructor
 *
 */
AbstractScene::AbstractScene() :
	m_pSoundBgm(0)
{
	// 特に処理は無し
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
 * @brief シーンの論理的な初期化
 *
 */
void AbstractScene::initializeGameScene()
{
}


/**
 * @brief 終了時の処理
 *
 */
void AbstractScene::finalize()
{
	SAFE_DELETE(m_pSoundBgm);
}


/**
 * @brief BGMの読み込み
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
 * @brief BGMの再生開始
 *
 */
void AbstractScene::playBgm()
{
	V_THROW(m_pSoundBgm->Reset());
	V_THROW(m_pSoundBgm->Play(0, DSBPLAY_LOOPING, 0));
}


/**
 * @brief BGMの停止
 *
 */
void AbstractScene::stopBgm()
{
	V_THROW(m_pSoundBgm->Stop());
}
