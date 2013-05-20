#ifndef ACUT_PLAYER_H__
#define ACUT_PLAYER_H__


/**
 * @brief 視点の回転量を保持する構造体
 *
 */
struct Facing {
	/**
	 * @brief constructor
	 *
	 */
	Facing() : yaw(0.0f), pitch(0.0f)
	{
		// 特に処理は無し
	}

	/**
	 * @brief 初期化
	 *
	 */
	void initialize()
	{
		yaw = pitch = 0.0f;
	}

	float yaw; //!< 左右の回転量
	float pitch; //!< 上下の回転量
};


class Player {
public:
	Player();
	~Player();
	void initialize(
		const D3DXVECTOR3& position,
		const D3DXVECTOR3& scale,
		const D3DXVECTOR3& rotation 
	);
	void finalize();

	void onDeviceCreated();
	void onDeviceDestroyed();
	void render(const float& elapsedTime);
	void act(const float& elapsedTime);
	void collide();
	D3DXVECTOR3 getPosition() const;
	D3DXVECTOR3 getPreviousPosition() const;
	D3DXVECTOR3 getCollisionPosition() const;
	D3DXVECTOR3 getPreviousCollisionPosition() const;
	D3DXVECTOR3 getFacing() const;
	int getHealth() const;
	void changeAnimation(const int index);

private:
	// コピー未対応
	Player(const Player&);
	Player& operator=(const Player&);

	D3DXVECTOR3 getFacingYaw() const;
	POINT updateMouseDelta();

	/**
	 * @brief デフォルトのスケールと向きを表す行列
	 *
	 * スケール行列とローテーション行列を合成した行列
	 * 
	 */
	D3DXMATRIX m_orientationMatrix;

	D3DXVECTOR3 m_position; //!< プレイヤーキャラクターの位置
	D3DXVECTOR3 m_previousPosition; //!< 前のフレーム時の位置
	Facing m_facing; //!< プレイヤーキャラクターのdirection
	POINT m_lastMousePosition; //!< 直近のマウスポインタの位置
	acut::AnimationManager m_animationManager; //!< アニメーション処理を行うクラス
	int m_health; //!< 身体の状態
	const float m_speed; //!< 移動速度
	const float m_turnSpeed; //!< 回転速度かつ視点移動速度

#ifdef DEBUG
	acut::DebugCollision m_debugCollision;
#endif

	// static メンバ
	static CSound* s_pSoundCollision; //!< 敵にぶつかった時の効果音
};



#endif // #ifndef ACUT_PLAYER_H__
