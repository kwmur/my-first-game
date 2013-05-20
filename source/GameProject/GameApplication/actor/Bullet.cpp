#include "Bullet.h"
#include "../GameConfiguration.h"
#include "../GameApplication.h"



// staticメンバ変数
bool Bullet::s_shot = false;
double Bullet::s_lastShotTime = 0.0;
// mesh
LPD3DXMESH Bullet::s_pMesh = NULL;
DWORD Bullet::s_numberOfMaterials = 0;
std::vector<D3DMATERIAL9> Bullet::s_meshMaterials;
std::vector<LPDIRECT3DTEXTURE9> Bullet::s_meshTexturePointers;
// sound
CSound* Bullet::s_pSoundShot = 0;

// debug
#ifdef DEBUG
acut::DebugCollision Bullet::s_debugCollision;
#endif



/**
 * @brief constructor
 *
 */
Bullet::Bullet() :
	m_position(0.0f, 0.0f, 0.0f),
	m_previousPosition(0.0f, 0.0f, 0.0f),
	m_direction(0.0f, 0.0f, 0.0f),
	m_existenceTime(GameConfiguration::s_BULLET_DISAPPEARANCE_TIME)
{
}


/**
 * @brief destructor
 *
 */
Bullet::~Bullet()
{
}


/**
 * @brief 初期化
 *
 */
void Bullet::initialize()
{
}


/**
 * @brief 終了処理
 *
 */
void Bullet::finalize()
{
}


/**
 * @brief デバイス作成後に呼び出される
 *
 */
void Bullet::onDeviceCreated()
{
	// mesh読み込み
	acut::MeshUtility::loadMeshFromX(
		&s_pMesh,
		&s_numberOfMaterials,
		&s_meshMaterials,
		&s_meshTexturePointers,
		GameConfiguration::s_BULLET_MESH_FILE_PATH,
		GameConfiguration::s_BULLET_MESH_FOLDER_PATH
	);

#ifdef DEBUG
	s_debugCollision.changeMeshSphere(GameConfiguration::s_BULLET_COLLISION_RADIUS);
#endif

	// sound
	s_pSoundShot = acut::SoundUtility::loadSound(
		&GameApplication::s_soundManager,
		GameConfiguration::s_BULLET_SHOT_SOUND_FILE_PATH
	);
	if (NULL == s_pSoundShot) {
		OutputDebugString(_T("BULLET_COLLISION_SOUND_FILE not found; continuing without sound.\n"));
		V_THROW(E_FAIL);
	}
}


/**
 * @brief Direct3D device の解体後に呼ばれる
 *
 * onDeviceCreated() で取得したリソースをここで解放する
 *
 */
void Bullet::onDeviceDestroyed()
{
	std::for_each(s_meshTexturePointers.begin(), s_meshTexturePointers.end(), acut::Release<LPDIRECT3DTEXTURE9>());
	s_meshTexturePointers.clear();
	SAFE_RELEASE(s_pMesh);
	SAFE_DELETE(s_pSoundShot);

#ifdef DEBUG
	s_debugCollision.finalize();
#endif
}


/**
 * @brief 向いている方向へ移動する
 *
 */
void Bullet::act(const float elapsedTime)
{
	if (!this->isExist()) {
		return;	
	}

	m_previousPosition = m_position;
	m_existenceTime += elapsedTime;
	D3DXVECTOR3 normalVector;
	D3DXVec3Normalize(&normalVector, &m_direction);
	m_position += normalVector * GameConfiguration::s_BULLET_SPEED * elapsedTime;
}


/**
 * @brief 弾を描画する
 *
 */
void Bullet::render()
{
	if (!this->isExist()) {
		return;
	}

	LPDIRECT3DDEVICE9 pDevice = DXUTGetD3D9Device();

	{
		// world行列のセット
		D3DXMATRIXA16 worldMatrix;
		V_THROW(pDevice->SetTransform(
			D3DTS_WORLD,
			D3DXMatrixTranslation(&worldMatrix, m_position.x, m_position.y, m_position.z)
		));
	}

	for (DWORD i = 0; i < s_numberOfMaterials; ++i) {
		// マテリアルとテクスチャをセット
		V_THROW(pDevice->SetMaterial(&s_meshMaterials.at(i)));
		V_THROW(pDevice->SetTexture(0, s_meshTexturePointers.at(i)));

		// mesh描画
		V_THROW(s_pMesh->DrawSubset(i));
	}

#ifdef DEBUG
	s_debugCollision.render();
#endif
}


/**
 * @brief 生存時間の範囲内か調べる
 *
 * @return 生存時間内: true、生存時間外: false
 * 
 */
bool Bullet::isExist() const
{
	if (GameConfiguration::s_BULLET_DISAPPEARANCE_TIME < m_existenceTime) {
		return false;
	}
	return true;
}


/**
 * @brief 弾を発射した状態にする
 *
 */
void Bullet::shot(const D3DXVECTOR3& position, const D3DXVECTOR3& direction)
{
	// 弾を右手から発射しているように見せるため、
	// プレイヤーに対しての横位置を調整する
	// プレイヤーに対しての右方向のベクトルを計算
	D3DXVECTOR3 vectorSide;
	D3DXVec3Cross(&vectorSide, &D3DXVECTOR3(0.f, 1.f, 0.f), &direction);
	D3DXVec3Normalize(&vectorSide, &vectorSide);
	// 発射位置の設定
	m_position = position;
	m_position.x += vectorSide.x * 0.08f;
	m_position.y += GameConfiguration::s_BULLET_OFFSET_POSITION_Y;
	m_position.z += vectorSide.z * 0.08f;

	// 発射方向の設定
	m_direction = direction;

	// 画面に存在する時間の初期化
	m_existenceTime = 0.0f;

	// shot時の効果音
	V_THROW(s_pSoundShot->Reset());
	V_THROW(s_pSoundShot->Play(0, 0, -1000));
}


/**
 * @brief 衝突時の処理
 *
 */
void Bullet::collide()
{
	DEBUG_OUTPUTF("%s : \n", __FUNCTIONW__);
	m_existenceTime = GameConfiguration::s_BULLET_DISAPPEARANCE_TIME;
}


/**
 * @brief 位置をセット
 *
 * @param position 位置
 *
 */
void Bullet::setPosition(const D3DXVECTOR3& position)
{
	m_position = position;
}


/**
 * @brief 位置を取得
 *
 * @return 位置
 *
 */
D3DXVECTOR3 Bullet::getPosition() const
{
	return m_position;
}


/**
 * @brief 前のフレーム時の位置を取得
 *
 * @return 前のフレーム時の位置
 *
 */
D3DXVECTOR3 Bullet::getPreviousPosition() const
{
	return m_previousPosition;
}

