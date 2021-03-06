#include "acut/acut.h"
#include "acut/actor/Player.h"



namespace acut {


	/**
	 * @brief constructor
	 *
	 */
	Player::Player() :
		m_position(0.0f, 0.0f, 0.0f),
		m_facing(),
		m_animationManager(),
		m_speed(1.0f),
		m_turnSpeed(0.5f)
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
		const D3DXVECTOR3& rotation,
		const acut::TString& meshFilePath
	)
	{
		// 先にリスクの高いメッシュの読み込みを行う
		m_animationManager.loadMeshHierarchyFromX(meshFilePath);

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
	}


	/**
	 * @brief プレイヤーの終了処理
	 *
	 */
	void Player::finalize()
	{
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
	}


	/**
	 * @brief プレイヤーの描画に関係するアニメーション更新処理
	 *
	 */
	void Player::act(const float& elapsedTime)
	{
		// SHORT GetKeyState(int)
		// 戻り値は、指定された仮想キーの状態を表します。各値は次のことを意味します。
		// •最上位ビットが 1 のときはキーが押されていることを、0 のときはキーが押されていないことを示します。

		// キーボードの操作による移動を反映させる
		// 前へ進む
		if (GetKeyState('W') < 0) {
			D3DXVECTOR3 movePositionVector = this->getFacingYaw();
			D3DXVec3Scale(&movePositionVector, &movePositionVector, float(m_speed * elapsedTime));
			D3DXVec3Add(&movePositionVector, &movePositionVector, &m_position);
			m_position = movePositionVector;
		}
		// 後ろへ進む
		if (GetKeyState('S') < 0) {
			D3DXVECTOR3 movePositionVector = this->getFacingYaw();
			D3DXVec3Scale(&movePositionVector, &movePositionVector, float(-m_speed * elapsedTime));
			D3DXVec3Add(&movePositionVector, &movePositionVector, &m_position);
			m_position = movePositionVector;
		}
		// 左へ進む
		if (GetKeyState('A') < 0) {
			D3DXVECTOR3 movePositionVector = this->getFacingYaw();
			D3DXVec3Cross(&movePositionVector, &movePositionVector, &(D3DXVECTOR3(0.0f, 1.0f, 0.0f)));
			D3DXVec3Scale(&movePositionVector, &movePositionVector, float(m_speed * elapsedTime));
			D3DXVec3Add(&movePositionVector, &movePositionVector, &m_position);
			m_position = movePositionVector;
		}
		// 右へ進む
		if (GetKeyState('D') < 0) {
			D3DXVECTOR3 movePositionVector = this->getFacingYaw();
			D3DXVec3Cross(&movePositionVector, &movePositionVector, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
			D3DXVec3Scale(&movePositionVector, &movePositionVector, float(-m_speed * elapsedTime));
			D3DXVec3Add(&movePositionVector, &movePositionVector, &m_position);
			m_position = movePositionVector;
		}

		// マウスの移動による視点の移動を設定
		POINT mouseDelta = this->updateMouseDelta();
		m_facing.yaw = float(m_facing.yaw - m_turnSpeed * elapsedTime * mouseDelta.x);
		m_facing.pitch = float(m_facing.pitch - m_turnSpeed * elapsedTime * mouseDelta.y);
		//Debug_outputf("m_facing.pitch : %f\n", m_facing.pitch);
		// 上下視点に制限を掛ける
		const float PITCH = 0.60f;
		if (PITCH < m_facing.pitch) {
			// 上方向
			m_facing.pitch = PITCH;
		}
		else if (m_facing.pitch < -PITCH + 0.40f) {
			// 下方向
			m_facing.pitch = -PITCH + 0.40f;
		}
		//Debug_outputf("m_facing.pitch = %f\n", m_facing.pitch);
	}


	/**
	 * @brief Playerの位置を取得
	 *
	 * @return 位置
	 *
	 */
	D3DXVECTOR3 Player::getPosition()
	{
		return m_position;
	}


	/**
	 * @brief Playerの左右の向きを取得
	 *
	 * @return 左右の向き
	 *
	 */
	D3DXVECTOR3 Player::getFacingYaw()
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
	D3DXVECTOR3 Player::getFacing()
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

		return currentMouseDelta;

/*
	    if( m_bResetCursorAfterMove && DXUTIsActive() )
	    {
	        // Set position of camera to center of desktop, 
	        // so it always has room to move.  This is very useful
	        // if the cursor is hidden.  If this isn't done and cursor is hidden, 
	        // then invisible cursor will hit the edge of the screen 
	        // and the user can't tell what happened
	        POINT ptCenter;

	        // Get the center of the current monitor
	        MONITORINFO mi;
	        mi.cbSize = sizeof( MONITORINFO );
	        DXUTGetMonitorInfo( DXUTMonitorFromWindow( DXUTGetHWND(), MONITOR_DEFAULTTONEAREST ), &mi );
	        ptCenter.x = ( mi.rcMonitor.left + mi.rcMonitor.right ) / 2;
	        ptCenter.y = ( mi.rcMonitor.top + mi.rcMonitor.bottom ) / 2;
	        SetCursorPos( ptCenter.x, ptCenter.y );
	        m_lastMousePosition = ptCenter;
	    }

	    // Smooth the relative mouse data over a few frames so it isn't 
	    // jerky when moving slowly at low frame rates.
	    float fPercentOfNew = 1.0f / m_fFramesToSmoothMouseData;
	    float fPercentOfOld = 1.0f - fPercentOfNew;
	    m_vMouseDelta.x = m_vMouseDelta.x * fPercentOfOld + ptCurMouseDelta.x * fPercentOfNew;
	    m_vMouseDelta.y = m_vMouseDelta.y * fPercentOfOld + ptCurMouseDelta.y * fPercentOfNew;

	    m_vRotVelocity = m_vMouseDelta * m_fRotationScaler;
*/
	}


};

