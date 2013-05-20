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
	D3DXVECTOR3 m_position; //!< 位置
	D3DXVECTOR3 m_previousPosition; //!< 前のフレーム時の位置
	D3DXVECTOR3 m_direction; //!< 弾の飛ぶ向き
	float m_existenceTime; //!< 弾が表示されてからの時間


// staticメンバ
public:
	static void initialize();
	static void finalize();
	static void onDeviceCreated();
	static void onDeviceDestroyed();
	static bool s_shot; //!< ショットの発射フラグ
	static double s_lastShotTime; //!< 最後に弾を発射した時間

private:
	// メッシュなど
	static LPD3DXMESH s_pMesh; //!< メッシュ
	static DWORD s_numberOfMaterials; // マテリアル数
	static std::vector<D3DMATERIAL9> s_meshMaterials; //!< マテリアル
	static std::vector<LPDIRECT3DTEXTURE9> s_meshTexturePointers; //!< テクスチャ
	// sound
	static CSound* s_pSoundShot; //!< 弾を発射した時の効果音

#ifdef DEBUG
	static acut::DebugCollision s_debugCollision; //!< 衝突判定の可視化
#endif

};



#endif // #ifndef ACUT_BULLET_H__
