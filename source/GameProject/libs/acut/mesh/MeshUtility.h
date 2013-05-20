#ifndef ACUT_MESH_UTILITY_H__
#define ACUT_MESH_UTILITY_H__



namespace acut {


	/**
	 * @brief Mesh�p�֐����܂Ƃ߂��t�@���N�^�N���X
	 *
	 */
	class MeshUtility {
	public:
		static void loadMeshFromX(
			LPD3DXMESH* ppMesh,
			DWORD* pNumberOfMaterials,
			std::vector<D3DMATERIAL9>* pMeshMaterials,
			std::vector<LPDIRECT3DTEXTURE9>* pMeshTexturePointers,
			const acut::TString& meshFilePath,
			const acut::TString& meshFolderPath,
			const bool ambientEqualDiffuse = false
		);

	private:
		// �C���X�^���X�����Ή�
		MeshUtility();
		~MeshUtility();
	};


};



#endif // #ifndef ACUT_MESH_UTILITY_H__
