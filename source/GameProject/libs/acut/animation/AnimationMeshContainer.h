#ifndef ACUT_ANIMATION_MESH_CONTAINER_H__
#define ACUT_ANIMATION_MESH_CONTAINER_H__



namespace acut {


	/**
	 * @brief
	 * Inherits from D3DXMESHCONTAINER.
	 * This represents a mesh object that gets its vertices blended
	 * and rendered based on the frame information in its hierarchy.
	 *
	 */
	struct AnimationMeshContainer : public D3DXMESHCONTAINER {
		AnimationMeshContainer(); // constructor

	    LPD3DXMESH m_pOriginMesh; //!< Boneなんとか()を行う前のメッシュ
	    DWORD m_numberOfPaletteEntries; //!< TODO:使い方がわからない
		DWORD m_maxNumberOfFaceInfl; //!< ボーン最大影響数 別名：NumInfl
		DWORD m_numberOfBoneCombinations; //!< ボーンコンビネーション数 別名：NumAttributeGroups
		LPD3DXBUFFER m_pBoneCombinationTable; //!< ボーンコンビネーション構造体配列へのポインタ

		std::vector<LPD3DXMATRIX> m_boneMatrixPointers; //!< ボーン行列へのポインタの配列
		std::vector<D3DXMATRIX> m_boneOffsetMatrices; //!< ボーンオフセット行列の配列

		std::vector<LPDIRECT3DTEXTURE9> m_textures; // ppTexturesの代わり
		//LPDIRECT3DTEXTURE9* ppTextures; // array of textures, entries are TODO: まだ参考に残しておく


		bool UseSoftwareVP; //!< TODO: ?
		UINT iAttributeSW; //!< TODO: ?
	};


};



#endif // #ifndef ACUT_ANIMATION_MESH_CONTAINER_H__
