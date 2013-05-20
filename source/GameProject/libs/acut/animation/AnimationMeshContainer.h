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

	    LPD3DXMESH m_pOriginMesh; //!< Bone�Ȃ�Ƃ�()���s���O�̃��b�V��
	    DWORD m_numberOfPaletteEntries; //!< TODO:�g�������킩��Ȃ�
		DWORD m_maxNumberOfFaceInfl; //!< �{�[���ő�e���� �ʖ��FNumInfl
		DWORD m_numberOfBoneCombinations; //!< �{�[���R���r�l�[�V������ �ʖ��FNumAttributeGroups
		LPD3DXBUFFER m_pBoneCombinationTable; //!< �{�[���R���r�l�[�V�����\���̔z��ւ̃|�C���^

		std::vector<LPD3DXMATRIX> m_boneMatrixPointers; //!< �{�[���s��ւ̃|�C���^�̔z��
		std::vector<D3DXMATRIX> m_boneOffsetMatrices; //!< �{�[���I�t�Z�b�g�s��̔z��

		std::vector<LPDIRECT3DTEXTURE9> m_textures; // ppTextures�̑���
		//LPDIRECT3DTEXTURE9* ppTextures; // array of textures, entries are TODO: �܂��Q�l�Ɏc���Ă���


		bool UseSoftwareVP; //!< TODO: ?
		UINT iAttributeSW; //!< TODO: ?
	};


};



#endif // #ifndef ACUT_ANIMATION_MESH_CONTAINER_H__
