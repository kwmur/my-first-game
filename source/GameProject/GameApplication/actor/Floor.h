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
	// �R�s�[���Ή�
	Floor(const Floor&);
	Floor& operator=(const Floor&);

	// mesh
	D3DXMATRIXA16 m_worldMatrix; //!< �\���ʒu�p�s��
	LPD3DXMESH m_pMesh; //!< ���b�V��
	DWORD m_numberOfMaterials; //!< �}�e���A����
	std::vector<D3DMATERIAL9> m_meshMaterials; //!< �}�e���A��
	std::vector<LPDIRECT3DTEXTURE9> m_meshTexturePointers; //!< �e�N�X�`��
};



#endif // #ifndef ACUT_FLOOR_H__
