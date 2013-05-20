#ifndef ACUT_PLAYER_H__
#define ACUT_PLAYER_H__



namespace acut {


	struct Facing {
		/**
		 * @brief constructor
		 *
		 */
		Facing() : yaw(0.0f), pitch(0.0f)
		{
			// 特に処理は無し
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
			const D3DXVECTOR3& rotation, 
			const acut::TString& meshFilePath
		);
		void finalize();
		void render(const float& elapsedTime);
		void act(const float& elapsedTime);
		D3DXVECTOR3 getPosition();
		D3DXVECTOR3 getFacing();

	private:
		D3DXVECTOR3 getFacingYaw();
		POINT updateMouseDelta();

		/**
		 * @brief デフォルトのスケールと向きを表す行列
		 *
		 * スケール行列とローテーション行列を合成した行列
		 * 
		 */
		D3DXMATRIX m_orientationMatrix;

		D3DXVECTOR3 m_position; //!< プレイヤーキャラクターの位置
		Facing m_facing; //!< プレイヤーキャラクターのdirection
		POINT m_lastMousePosition; //!< 直近のマウスポインタの位置
		acut::AnimationManager m_animationManager; //!< アニメーション処理を行うクラス

		const float m_speed; //!< 移動速度
		const float m_turnSpeed; //!< 回転速度かつ視点移動速度
	};


};



#endif // #ifndef ACUT_PLAYER_H__
