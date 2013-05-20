#ifndef ACUT_BULLET_H__
#define ACUT_BULLET_H__



class Bullet {
public:
	typedef std::vector<Bullet>::iterator VectorIterator;

	Bullet();
	~Bullet();

	void act(const float elapsedTime);
	void render();
	void shot(const D3DXVECTOR3& position, const D3DXVECTOR3& direction);
	bool isExist() const;
	void collide();
	void setPosition(const D3DXVECTOR3& position);
	D3DXVECTOR3 getPosition() const;
	D3DXVECTOR3 getPreviousPosition() const;

private:
	D3DXVECTOR3 m_position; //!< �ʒu
	D3DXVECTOR3 m_previousPosition; //!< �O�̃t���[�����̈ʒu
	D3DXVECTOR3 m_direction; //!< �e�̔�Ԍ���
	float m_existenceTime; //!< �e���\������Ă���̎���


// static�����o
public:
	static void initialize();
	static void finalize();
	static void onDeviceCreated();
	static void onDeviceDestroyed();
	static bool s_shot; //!< �V���b�g�̔��˃t���O
	static double s_lastShotTime; //!< �Ō�ɒe�𔭎˂�������

private:
	// ���b�V���Ȃ�
	static LPD3DXMESH s_pMesh; //!< ���b�V��
	static DWORD s_numberOfMaterials; // �}�e���A����
	static std::vector<D3DMATERIAL9> s_meshMaterials; //!< �}�e���A��
	static std::vector<LPDIRECT3DTEXTURE9> s_meshTexturePointers; //!< �e�N�X�`��
	// sound
	static CSound* s_pSoundShot; //!< �e�𔭎˂������̌��ʉ�

#ifdef DEBUG
	static acut::DebugCollision s_debugCollision; //!< �Փ˔���̉���
#endif

};



#endif // #ifndef ACUT_BULLET_H__
