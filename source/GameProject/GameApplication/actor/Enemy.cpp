#include "Enemy.h"
#include "../GameConfiguration.h"
#include "../GameApplication.h"



// staticメンバ変数
D3DXVECTOR3 Enemy::s_playerPosition(0.0f, 0.0f, 0.0f);
float Enemy::s_generateInterval = 0.0f;
// mesh
LPD3DXMESH Enemy::s_pMesh = NULL;
DWORD Enemy::s_numberOfMaterials = 0;
std::vector<D3DMATERIAL9> Enemy::s_meshMaterials;
std::vector<LPDIRECT3DTEXTURE9> Enemy::s_meshTexturePointers;
D3DMATERIAL9 Enemy::s_deadMaterial;
// sound
CSound* Enemy::s_pSoundCollision = 0;
// debug
#ifdef DEBUG
acut::DebugCollision Enemy::s_debugCollision;
#endif



/**
 * @brief constructor
 *
 */
Enemy::Enemy()
{
	this->generate();
}


/**
 * @brief destructor
 *
 */
Enemy::~Enemy()
{
	this->finalize();
}


/**
 * @brief Initialize enemy
 *
 * @return
 *
 */
void Enemy::initialize()
{
}


/**
 * @brief 終了処理
 *
 */
void Enemy::finalize()
{
}


/**
 * @brief デバイス作成後に呼び出される
 *
 */
void Enemy::onDeviceCreated()
{
	// mesh読み込み
	acut::MeshUtility::loadMeshFromX(
		&s_pMesh,
		&s_numberOfMaterials,
		&s_meshMaterials,
		&s_meshTexturePointers,
		GameConfiguration::s_ENEMY_MESH_FILE_PATH,
		GameConfiguration::s_ENEMY_MESH_FOLDER_PATH,
		true
	);

#ifdef DEBUG
	s_debugCollision.changeMeshSphere(GameConfiguration::s_ENEMY_COLLISION_RADIUS);
#endif

	// setup dead material color
	// diffuse
	s_deadMaterial.Diffuse.a = 0.0f;
	s_deadMaterial.Diffuse.r = 1.0f;
	s_deadMaterial.Diffuse.g = 0.0f;
	s_deadMaterial.Diffuse.b = 0.0f;
	// ambient
	s_deadMaterial.Ambient.a = 0.0f;
	s_deadMaterial.Ambient.r = 1.0f;
	s_deadMaterial.Ambient.g = 0.0f;
	s_deadMaterial.Ambient.b = 0.0f;
	// specular
	s_deadMaterial.Specular.r = 0.0f;
	s_deadMaterial.Specular.g = 0.0f;
	s_deadMaterial.Specular.b = 0.0f;
	// emissive
	s_deadMaterial.Emissive.r = 0.0f;
	s_deadMaterial.Emissive.g = 0.0f;
	s_deadMaterial.Emissive.b = 0.0f;

	// sound
	s_pSoundCollision = acut::SoundUtility::loadSound(
		&GameApplication::s_soundManager,
		GameConfiguration::s_ENEMY_COLLISION_SOUND_FILE_PATH
	);
	if (NULL == s_pSoundCollision) {
		OutputDebugString(_T("ENEMY_COLLISION_SOUND_FILE not found; continuing without sound.\n"));
		throw acut::Exception(__FILEW__, __LINE__, __FUNCTIONW__);
	}

	// 次の生成までの時間
	s_generateInterval = GameConfiguration::s_ENEMY_GENERATE_INTERVAL;
}


/**
 * @brief Direct3D device の解体後に呼ばれる
 *
 * onDeviceCreated() で取得したリソースをここで解放する
 *
 */
void Enemy::onDeviceDestroyed()
{
	SAFE_DELETE(s_pSoundCollision);

	std::for_each(s_meshTexturePointers.begin(), s_meshTexturePointers.end(), acut::Release<LPDIRECT3DTEXTURE9>());
	s_meshTexturePointers.clear();
	SAFE_RELEASE(s_pMesh);

#ifdef DEBUG
	s_debugCollision.finalize();
#endif
}


/**
 * @brief playerに接近する
 *
 */
void Enemy::act(const float elapsedTime)
{
	if (this->isDead()) {
		if (0 < m_dieOutTime) {
			m_dieOutTime -= elapsedTime;
		}
		return;
	}

	m_previousPosition = m_position;
	D3DXVECTOR3 normalVector;
	D3DXVec3Normalize(&normalVector, &(s_playerPosition - m_position));
	m_position += normalVector * GameConfiguration::s_ENEMY_SPEED * elapsedTime;

	// 地面にめり込まないようにy座標を修正
	if (m_position.y < GameConfiguration::s_ENEMY_COLLISION_RADIUS) {
		m_position.y = GameConfiguration::s_ENEMY_COLLISION_RADIUS;
	}
}


/**
 * @brief 衝突時の処理
 *
 */
void Enemy::collide()
{
	DEBUG_OUTPUTF("%s : \n", __FUNCTIONW__);
	if (0 < m_health) {
		--m_health;
	}

	// TODO: healthの初期値が1以上の処理
//	if (m_health <= 0) {
//		this->die();
//	}

	s_pSoundCollision->Reset();
	s_pSoundCollision->Play(0, 0, 0);
}


/**
 * @brief 位置をセット
 *
 * @param position 位置
 *
 */
void Enemy::setPosition(const D3DXVECTOR3& position)
{
	m_position = position;
}


/**
 * @brief 位置を取得
 *
 * @return 位置
 *
 */
D3DXVECTOR3 Enemy::getPosition() const
{
	return m_position;
}


/**
 * @brief 前のフレーム時の位置を取得
 *
 * @return 前のフレーム時の位置
 *
 */
D3DXVECTOR3 Enemy::getPreviousPosition() const
{
	return m_previousPosition;
}


/**
 * @brief 死亡確認
 *
 * @return 死亡している場合true, 死亡していない場合false
 *
 */
bool Enemy::isDead() const
{
	if (m_health <= 0) {
		return true;
	}
	else if (0 < m_health) {
		return false;
	}
	else {
		throw acut::Exception();
	}
}


/**
 * @brief 画面上に存在しているか確認する
 *
 * @return true or false
 *
 */
bool Enemy::isExist() const
{
	if (m_health <= 0 && m_dieOutTime <= 0.0f) {
		return false;
	}

	return true;
}


/**
 * @brief 画面上に敵を生成する
 *
 */
void Enemy::generate()
{
	D3DXVECTOR3 position(
		static_cast<float>(std::rand() % GameConfiguration::s_ENEMY_GENERATE_RANDOM_DIVISION - GameConfiguration::s_ENEMY_GENERATE_RANDOM_SUBTRACTION + s_playerPosition.x),
		GameConfiguration::s_ENEMY_GENERATE_POSITION_Y_1,
		static_cast<float>(std::rand() % GameConfiguration::s_ENEMY_GENERATE_RANDOM_DIVISION - GameConfiguration::s_ENEMY_GENERATE_RANDOM_SUBTRACTION + s_playerPosition.z)
	);
	while (true) {
		// 生成される位置がプレイヤーと一定の距離をとっているかチェック
		if (GameConfiguration::s_ENEMY_GENERATE_DISTANCE < D3DXVec3Length(&(s_playerPosition - position))) {
			break;
		}
		position.x = static_cast<float>(std::rand() % GameConfiguration::s_ENEMY_GENERATE_RANDOM_DIVISION - GameConfiguration::s_ENEMY_GENERATE_RANDOM_SUBTRACTION + s_playerPosition.x);
		position.y = GameConfiguration::s_ENEMY_GENERATE_POSITION_Y_2;
		position.z = static_cast<float>(std::rand() % GameConfiguration::s_ENEMY_GENERATE_RANDOM_DIVISION - GameConfiguration::s_ENEMY_GENERATE_RANDOM_SUBTRACTION + s_playerPosition.z);
	}

	m_position = position;
	m_previousPosition = position;
	m_health = GameConfiguration::s_ENEMY_HEALTH;
	m_dieOutTime = GameConfiguration::s_ENEMY_DIE_OUT_TIME;

	// 次の生成までの時間
	s_generateInterval = GameConfiguration::s_ENEMY_GENERATE_INTERVAL;
}


/**
 * @brief playerの位置をセット
 *
 */
void Enemy::setPlayerPosition(const D3DXVECTOR3& position)
{
	s_playerPosition = position;
}


/**
 * @brief 敵を描画する
 * 
 */
void Enemy::render()
{
	LPDIRECT3DDEVICE9 pDevice = DXUTGetD3D9Device();

	D3DXMATRIXA16 worldMatrix;
	D3DXMatrixTranslation(&worldMatrix, m_position.x, m_position.y, m_position.z);
	pDevice->SetTransform(D3DTS_WORLD, &worldMatrix);

	for (DWORD i = 0; i < s_numberOfMaterials; ++i) {
		if (this->isDead()) {
			if (m_dieOutTime <= 0) {
				return;
			}

			// 死亡時は、消滅するまで徐々に赤から色を黒くしていく
			D3DMATERIAL9 deadMaterial = s_deadMaterial;
			float red = m_dieOutTime / GameConfiguration::s_ENEMY_DIE_OUT_TIME;
			if (red < 0.50f) { red = 0.5f; }
			deadMaterial.Diffuse.r = deadMaterial.Ambient.r = red;
			pDevice->SetMaterial(&deadMaterial);
		}
		else {
			pDevice->SetMaterial(&s_meshMaterials.at(i));
		}
		pDevice->SetTexture(0, s_meshTexturePointers.at(i));

		s_pMesh->DrawSubset(i);
	}

#ifdef DEBUG
	s_debugCollision.render();
#endif
}

