#include "Player.h"
#include "../GameConfiguration.h"
#include "../GameApplication.h"



// static メンバ変数
CSound* Player::s_pSoundCollision = 0;


/**
 * @brief constructor
 *
 */
Player::Player() :
	m_position(0.0f, 0.0f, 0.0f),
	m_facing(),
	m_animationManager(),
	m_speed(GameConfiguration::s_PLAYER_SPEED),
	m_turnSpeed(GameConfiguration::s_PLAYER_TURN_SPEED),
	m_health(GameConfiguration::s_PLAYER_HEALTH)
{
	D3DXMatrixIdentity(&m_orientationMatrix);
	m_lastMousePosition.x = 0;
	m_lastMousePosition.y = 0;
}


/**
 * @brief destructor
 *
 */
Player::~Player()
{
	this->finalize();
}


/**
 * @brief プレイヤーの初期化処理
 *
 * メッシュを読み込み、
 * 初期位置、スケール、向きを設定する
 *
 */
void Player::initialize(
	const D3DXVECTOR3& position,
	const D3DXVECTOR3& scale,
	const D3DXVECTOR3& rotation
)
{
	m_position = position; // 位置を設定

	// scale
	{
		D3DXMATRIX scalingMatrix;
		D3DXMatrixIdentity(&scalingMatrix);
		D3DXMatrixScaling(&scalingMatrix, scale.x, scale.y, scale.z);

		m_orientationMatrix = scalingMatrix;
	}

	// 回転
	{
		// x軸で回転
		D3DXMATRIX rotationXMatrix;
		D3DXMatrixRotationX(&rotationXMatrix, rotation.x);
		D3DXMatrixMultiply(&m_orientationMatrix, &m_orientationMatrix, &rotationXMatrix);
	}
	{
		// y軸で回転
		D3DXMATRIX rotationYMatrix;
		D3DXMatrixRotationY(&rotationYMatrix, rotation.y);
		D3DXMatrixMultiply(&m_orientationMatrix, &m_orientationMatrix, &rotationYMatrix);
	}
	{
		// z軸で回転
		D3DXMATRIX rotationZMatrix;
		D3DXMatrixRotationZ(&rotationZMatrix, rotation.z);
		D3DXMatrixMultiply(&m_orientationMatrix, &m_orientationMatrix, &rotationZMatrix);
	}

	// 向きの初期化
	m_facing.initialize();
	m_facing.pitch = 7.f;
	// 健康値の初期化
	m_health = GameConfiguration::s_PLAYER_HEALTH;
	// マウスの初期化
	m_lastMousePosition.x = 0;
	m_lastMousePosition.y = 0;
}


/**
 * @brief プレイヤーの終了処理
 *
 */
void Player::finalize()
{
}


/**
 * @brief デバイス作成後に呼び出される
 *
 */
void Player::onDeviceCreated()
{
	// 先にリスクの高いメッシュの読み込みを行う
	m_animationManager.loadMeshHierarchyFromX(GameConfiguration::s_PLAYER_MESH_FILE_PATH);

#ifdef DEBUG
	m_debugCollision.changeMeshSphere(GameConfiguration::s_PLAYER_COLLISION_RADIUS);
#endif

	// sound
	s_pSoundCollision = acut::SoundUtility::loadSound(
		&GameApplication::s_soundManager,
		GameConfiguration::s_PLAYER_COLLISION_SOUND_FILE_PATH
	);
	if (NULL == s_pSoundCollision) {
		OutputDebugString(_T("PLAYER_COLLISION_SOUND_FILE not found; continuing without sound.\n"));
		throw acut::Exception(__FILEW__, __LINE__, __FUNCTIONW__);
	}
}


/**
 * @brief Direct3D device の解体後に呼ばれる
 *
 * onDeviceCreated() で取得したリソースをここで解放する
 *
 */
void Player::onDeviceDestroyed()
{
	SAFE_DELETE(s_pSoundCollision);

#ifdef DEBUG
	m_debugCollision.finalize();
#endif

	m_animationManager.cleanup();
}


/**
 * @brief プレイヤーの描画 
 *
 */
void Player::render(const float& elapsedTime)
{
	m_animationManager.animate(elapsedTime);

	// Y軸の回転行列を作成
	D3DXMATRIX rotationYMatrix;
	D3DXMatrixRotationY(&rotationYMatrix, -m_facing.yaw); 
	// 平行移動行列を作成
	D3DXMATRIX translationMatrix;
	D3DXMatrixTranslation(&translationMatrix, m_position.x, m_position.y, m_position.z);
	// ワールド行列の作成
	D3DXMATRIX worldMatrix;
	D3DXMatrixMultiply(&worldMatrix, &rotationYMatrix, &translationMatrix); // 回転、移動 行列を作成
	D3DXMatrixMultiply(&worldMatrix, &m_orientationMatrix, &worldMatrix); // 縮尺、初期方向へ回転、回転、移動行列を作成

	m_animationManager.render(worldMatrix);

#ifdef DEBUG
	D3DXVECTOR3 collPosition = this->getCollisionPosition();
	D3DXMATRIX collisionMatrix;
	D3DXMatrixTranslation(&collisionMatrix, collPosition.x, collPosition.y, collPosition.z);
	DXUTGetD3D9Device()->SetTransform(D3DTS_WORLD, &collisionMatrix);
	m_debugCollision.render();
#endif
}


/**
 * @brief プレイヤーの描画に関係するアニメーション更新処理
 *
 */
void Player::act(const float& elapsedTime)
{
	m_previousPosition = m_position;

	// SHORT GetKeyState(int)
	// 戻り値は、指定された仮想キーの状態を表します。各値は次のことを意味します。
	// •最上位ビットが 1 のときはキーが押されていることを、0 のときはキーが押されていないことを示します。

	enum PlayerAnimation {
		Player_Stand,
		Player_Run_Right,
		Player_Run_Left,
		Player_Run_Back,
		Player_Run_Forward
	};

	// キーボードの操作による移動を反映させる
	if (GetKeyState('W') < 0) {
		// 前へ進む
		D3DXVECTOR3 movePositionVector = this->getFacingYaw();
		D3DXVec3Scale(&movePositionVector, &movePositionVector, float(m_speed * elapsedTime));
		D3DXVec3Add(&movePositionVector, &movePositionVector, &m_position);
		if (D3DXVec3Length(&movePositionVector) < GameConfiguration::s_FLOOR_MOVE_RADIUS) {
			m_position = movePositionVector;
		}

		this->changeAnimation(Player_Run_Forward);
	}
	if (GetKeyState('S') < 0) {
		// 後ろへ進む
		D3DXVECTOR3 movePositionVector = this->getFacingYaw();
		D3DXVec3Scale(&movePositionVector, &movePositionVector, float(-m_speed * elapsedTime));
		D3DXVec3Add(&movePositionVector, &movePositionVector, &m_position);
		if (D3DXVec3Length(&movePositionVector) < GameConfiguration::s_FLOOR_MOVE_RADIUS) {
			m_position = movePositionVector;
		}

		this->changeAnimation(Player_Run_Back);
	}
	if (GetKeyState('A') < 0) {
		// 左へ進む
		D3DXVECTOR3 movePositionVector = this->getFacingYaw();
		D3DXVec3Cross(&movePositionVector, &movePositionVector, &(D3DXVECTOR3(0.0f, 1.0f, 0.0f)));
		D3DXVec3Scale(&movePositionVector, &movePositionVector, float(m_speed * elapsedTime));
		D3DXVec3Add(&movePositionVector, &movePositionVector, &m_position);
		if (D3DXVec3Length(&movePositionVector) < GameConfiguration::s_FLOOR_MOVE_RADIUS) {
			m_position = movePositionVector;
		}

		if (GetKeyState('S') < 0) {
			this->changeAnimation(Player_Run_Right);
		}
		else {
			this->changeAnimation(Player_Run_Left);
		}
	}
	if (GetKeyState('D') < 0) {
		// 右へ進む
		D3DXVECTOR3 movePositionVector = this->getFacingYaw();
		D3DXVec3Cross(&movePositionVector, &movePositionVector, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
		D3DXVec3Scale(&movePositionVector, &movePositionVector, float(-m_speed * elapsedTime));
		D3DXVec3Add(&movePositionVector, &movePositionVector, &m_position);
		if (D3DXVec3Length(&movePositionVector) < GameConfiguration::s_FLOOR_MOVE_RADIUS) {
			m_position = movePositionVector;
		}

		if (GetKeyState('S') < 0) {
			this->changeAnimation(Player_Run_Left);
		}
		else {
			this->changeAnimation(Player_Run_Right);
		}
	}

	// 移動キー入力していない場合、立ちポーズ
	if (0 <= GetKeyState('W') && 0 <= GetKeyState('S') && 0 <= GetKeyState('A') && 0 <= GetKeyState('D')) {
		this->changeAnimation(Player_Stand);
	}

	// マウスの移動による視点の移動を設定
	POINT mouseDelta = this->updateMouseDelta();
	m_facing.yaw = float(m_facing.yaw - m_turnSpeed * elapsedTime * mouseDelta.x);
	m_facing.pitch = float(m_facing.pitch - m_turnSpeed * elapsedTime * mouseDelta.y);
	//DEBUG_OUTPUTF("m_facing.pitch : %f\n", m_facing.pitch);
	// 上下視点に制限を掛ける
	const float HIGH_PITCH = 0.65f;
	const float LOW_PITCH = - 0.40f;
	if (HIGH_PITCH < m_facing.pitch) {
		// 上方向
		m_facing.pitch = HIGH_PITCH;
	}
	else if (m_facing.pitch < LOW_PITCH) {
		// 下方向
		m_facing.pitch = LOW_PITCH;
	}
	//DEBUG_OUTPUTF("m_facing.pitch = %f\n", m_facing.pitch);
}


/**
 * @brief 衝突したら体力が減少する
 *
 */
void Player::collide()
{
	if (0 < m_health) {
		--m_health;
		s_pSoundCollision->Reset();
		s_pSoundCollision->Play(0, 0, 1);
	}

	DEBUG_OUTPUTF("Player.health = %d\n", m_health);
	
	if (m_health <= 0) {
		GameApplication::changeScene(&GameApplication::s_resultScene);
	}
}


/**
 * @brief Playerの位置を取得
 *
 * @return 位置
 *
 */
D3DXVECTOR3 Player::getPosition() const
{
	return m_position;
}


/**
 * @brief 前のフレーム時の位置を取得
 *
 * @return 前のフレーム時の位置
 *
 */
D3DXVECTOR3 Player::getPreviousPosition() const
{
	return m_previousPosition;
}


/**
 * @brief Playerの衝突判定の位置を取得
 *
 * @return 位置
 *
 */
D3DXVECTOR3 Player::getCollisionPosition() const
{
	return D3DXVECTOR3(m_position.x, m_position.y + 0.15f, m_position.z);
}


/**
 * @brief Playerの衝突判定の位置を取得
 *
 * @return 位置
 *
 */
D3DXVECTOR3 Player::getPreviousCollisionPosition() const
{
	return D3DXVECTOR3(m_previousPosition.x, m_previousPosition.y + 0.15f, m_previousPosition.z);
}


/**
 * @brief Playerの左右の向きを取得
 *
 * @return 左右の向き
 *
 */
D3DXVECTOR3 Player::getFacingYaw() const
{
	D3DXMATRIX rotationMatrix;
	D3DXVECTOR3 facing(0.0f, 0.0f, 1.0f);
	D3DXVec3TransformCoord(&facing, &facing, D3DXMatrixRotationY(&rotationMatrix, -m_facing.yaw));
	return facing;
}


/**
 * @brief Playerの向きを取得
 *
 * @return 向き
 *
 */
D3DXVECTOR3 Player::getFacing() const
{
	// 回転行列を作成
	D3DXMATRIX matrixY, matrixX;
	D3DXMatrixRotationY(&matrixX, -m_facing.yaw);
	D3DXMatrixRotationX(&matrixY, -m_facing.pitch);

	// Z軸を向いている方向ベクトルを回転させる
	D3DXMATRIX matrix;
	D3DXMatrixMultiply(&matrix, &matrixY, &matrixX);
	D3DXVECTOR3 facing(0.0f, 0.0f, 1.0f);
	D3DXVec3TransformCoord(&facing, &facing, &matrix);

	return facing;
}


/**
 * @brief マウス移動量を取得
 *
 * @return マウスの移動量
 *
 */
POINT Player::updateMouseDelta()
{
	// 現在のマウスの位置を取得
	POINT currentMousePosition;
	GetCursorPos(&currentMousePosition);

	// 前回のフレームからのマウスの移動量を計算
	POINT currentMouseDelta;
	currentMouseDelta.x = currentMousePosition.x - m_lastMousePosition.x;
	currentMouseDelta.y = currentMousePosition.y - m_lastMousePosition.y;

	// 現在のマウスの位置を保存
	m_lastMousePosition = currentMousePosition;

	GameApplication::initializeMousePosition();
	SetCursorPos(GameApplication::s_cursorPositionX, GameApplication::s_cursorPositionY);
	m_lastMousePosition.x = GameApplication::s_cursorPositionX;
	m_lastMousePosition.y = GameApplication::s_cursorPositionY;

	return currentMouseDelta;
}


/**
 * @brief 健康値取得
 *
 */
int Player::getHealth() const
{
	return m_health;
}


/**
 * @brief アニメーション切替
 *
 */
void Player::changeAnimation(const int index)
{
	m_animationManager.changeAnimationSet(index);
}

