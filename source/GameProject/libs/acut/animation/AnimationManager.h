#ifndef ACUT_ANIMATION_MANAGER_H__
#define ACUT_ANIMATION_MANAGER_H__



namespace acut {


	class AnimationManager {
	public:
		AnimationManager();
		~AnimationManager();
		void loadMeshHierarchyFromX(const acut::TString& meshFilePath);
		void cleanup();
		void render(D3DXMATRIX& worldMatrix);
		UINT getAnimationSetIndex(const std::string& animationSetName);
		void changeAnimationSet(const int index);
		HRESULT animate(float elapsedTime); // �A�j���[�V�����X�V

	private:
		HRESULT createAnimationSetList();

		// skinned mesh�̊֐�
		HRESULT setupBoneMatrixPointers(LPD3DXFRAME pFrame);
		HRESULT setupBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER pMeshContainerBase);
		void updateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix);
		void renderFrame(LPD3DXFRAME pFrame);
		void renderMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase);

		// �A�j���[�V�����p
		acut::AllocateHierarchy m_allocateHierarchy; //!< �t���[���K�w�\�z�A����p
		acut::AnimationFrame* m_pRootFrame; //!< Root Frame �ւ̃|�C���^
		LPD3DXANIMATIONCONTROLLER m_pAnimationController; //!< �A�j���[�V�����R���g���[���ւ̃|�C���^
		std::vector<LPD3DXANIMATIONSET> m_animationSetList; //!< X�t�@�C���Ɋ܂܂��A�j���[�V�����Z�b�g�̃��X�g
	};



};



#endif // #ifndef ACUT_ANIMATION_MANAGER_H__
