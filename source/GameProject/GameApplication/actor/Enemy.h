#ifndef ACUT_ENEMY_H__
#define ACUT_ENEMY_H__



class Enemy {
public:
	typedef std::vector<Enemy>::iterator VectorIterator;

	Enemy();
	~Enemy();

	// 毎フレーム呼ばれるメンバ関数
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
	D3DXVECTOR3 m_position; //!< 位置
	D3DXVECTOR3 m_previousPosition; //!< 前のフレーム時の位置
	int m_health; //!< 身体の状態
	float m_dieOutTime; //!< healthが0になってから消滅するまでの時間


// static メンバ
public:
	static void initialize();
	static void finalize();
	static void setPlayerPosition(const D3DXVECTOR3& position);
	static void onDeviceCreated();
	static void onDeviceDestroyed();
	static float s_generateInterval; //!< 次に敵が生成されるまでの時間

private:
	static D3DXVECTOR3 s_playerPosition; //!< Playerの位置
	// メッシュ
	static LPD3DXMESH s_pMesh;
	static DWORD s_numberOfMaterials;
	static std::vector<D3DMATERIAL9> s_meshMaterials;
	static std::vector<LPDIRECT3DTEXTURE9> s_meshTexturePointers;
	static D3DMATERIAL9 s_deadMaterial;
	// sound
	static CSound* s_pSoundCollision; //!< 敵にぶつかった時の効果音

#ifdef DEBUG
	static acut::DebugCollision s_debugCollision;
#endif
};



#endif // #ifndef ACUT_ENEMY_H__
