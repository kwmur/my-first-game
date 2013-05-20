#ifndef ACUT_ENEMY_H__
#define ACUT_ENEMY_H__



class Enemy {
public:
	typedef std::vector<Enemy>::iterator VectorIterator;

	Enemy();
	~Enemy();

	// ���t���[���Ă΂�郁���o�֐�
	void act(const float elapsedTime);
	void render();

	// getter and setter
	void setPosition(const D3DXVECTOR3& position);
	D3DXVECTOR3 getPosition() const;
	D3DXVECTOR3 getPreviousPosition() const;

	void collide();
	bool isDead() const;
	bool isExist() const;
	void generate();

private:
	D3DXVECTOR3 m_position; //!< �ʒu
	D3DXVECTOR3 m_previousPosition; //!< �O�̃t���[�����̈ʒu
	int m_health; //!< �g�̂̏��
	float m_dieOutTime; //!< health��0�ɂȂ��Ă�����ł���܂ł̎���


// static �����o
public:
	static void initialize();
	static void finalize();
	static void setPlayerPosition(const D3DXVECTOR3& position);
	static void onDeviceCreated();
	static void onDeviceDestroyed();
	static float s_generateInterval; //!< ���ɓG�����������܂ł̎���

private:
	static D3DXVECTOR3 s_playerPosition; //!< Player�̈ʒu
	// ���b�V��
	static LPD3DXMESH s_pMesh;
	static DWORD s_numberOfMaterials;
	static std::vector<D3DMATERIAL9> s_meshMaterials;
	static std::vector<LPDIRECT3DTEXTURE9> s_meshTexturePointers;
	static D3DMATERIAL9 s_deadMaterial;
	// sound
	static CSound* s_pSoundCollision; //!< �G�ɂԂ��������̌��ʉ�

#ifdef DEBUG
	static acut::DebugCollision s_debugCollision;
#endif
};



#endif // #ifndef ACUT_ENEMY_H__
