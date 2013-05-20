#ifndef ACUT_PLAYER_H__
#define ACUT_PLAYER_H__


/**
 * @brief ���_�̉�]�ʂ�ێ�����\����
 *
 */
struct Facing {
	/**
	 * @brief constructor
	 *
	 */
	Facing() : yaw(0.0f), pitch(0.0f)
	{
		// ���ɏ����͖���
	}

	/**
	 * @brief ������
	 *
	 */
	void initialize()
	{
		yaw = pitch = 0.0f;
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
	// �R�s�[���Ή�
	Player(const Player&);
	Player& operator=(const Player&);

	D3DXVECTOR3 getFacingYaw() const;
	POINT updateMouseDelta();

	/**
	 * @brief �f�t�H���g�̃X�P�[���ƌ�����\���s��
	 *
	 * �X�P�[���s��ƃ��[�e�[�V�����s������������s��
	 * 
	 */
	D3DXMATRIX m_orientationMatrix;

	D3DXVECTOR3 m_position; //!< �v���C���[�L�����N�^�[�̈ʒu
	D3DXVECTOR3 m_previousPosition; //!< �O�̃t���[�����̈ʒu
	Facing m_facing; //!< �v���C���[�L�����N�^�[��direction
	POINT m_lastMousePosition; //!< ���߂̃}�E�X�|�C���^�̈ʒu
	acut::AnimationManager m_animationManager; //!< �A�j���[�V�����������s���N���X
	int m_health; //!< �g�̂̏��
	const float m_speed; //!< �ړ����x
	const float m_turnSpeed; //!< ��]���x�����_�ړ����x

#ifdef DEBUG
	acut::DebugCollision m_debugCollision;
#endif

	// static �����o
	static CSound* s_pSoundCollision; //!< �G�ɂԂ��������̌��ʉ�
};



#endif // #ifndef ACUT_PLAYER_H__
