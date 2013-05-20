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
			// ���ɏ����͖���
		}

		float yaw; //!< ���E�̉�]��
		float pitch; //!< �㉺�̉�]��
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
		 * @brief �f�t�H���g�̃X�P�[���ƌ�����\���s��
		 *
		 * �X�P�[���s��ƃ��[�e�[�V�����s������������s��
		 * 
		 */
		D3DXMATRIX m_orientationMatrix;

		D3DXVECTOR3 m_position; //!< �v���C���[�L�����N�^�[�̈ʒu
		Facing m_facing; //!< �v���C���[�L�����N�^�[��direction
		POINT m_lastMousePosition; //!< ���߂̃}�E�X�|�C���^�̈ʒu
		acut::AnimationManager m_animationManager; //!< �A�j���[�V�����������s���N���X

		const float m_speed; //!< �ړ����x
		const float m_turnSpeed; //!< ��]���x�����_�ړ����x
	};


};



#endif // #ifndef ACUT_PLAYER_H__
