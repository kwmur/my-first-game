#ifndef ACUT_FLOOR_H__
#define ACUT_FLOOR_H__



class Floor {
public:
	Floor();
	~Floor();
	void initialize();
	void onDeviceCreated();
	void onDeviceDestroyed();
	void render();

private:
	// コピー未対応
	Floor(const Floor&);
	Floor& operator=(const Floor&);

	// mesh
	D3DXMATRIXA16 m_worldMatrix; //!< 表示位置用行列
	LPD3DXMESH m_pMesh; //!< メッシュ
	DWORD m_numberOfMaterials; //!< マテリアル数
	std::vector<D3DMATERIAL9> m_meshMaterials; //!< マテリアル
	std::vector<LPDIRECT3DTEXTURE9> m_meshTexturePointers; //!< テクスチャ
};



#endif // #ifndef ACUT_FLOOR_H__
