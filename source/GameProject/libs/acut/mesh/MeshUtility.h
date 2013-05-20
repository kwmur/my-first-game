#ifndef ACUT_MESH_UTILITY_H__
#define ACUT_MESH_UTILITY_H__



namespace acut {


	/**
	 * @brief Mesh用関数をまとめたファンクタクラス
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
		// インスタンス化未対応
		MeshUtility();
		~MeshUtility();
	};


};



#endif // #ifndef ACUT_MESH_UTILITY_H__
