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
		HRESULT animate(float elapsedTime); // アニメーション更新

	private:
		HRESULT createAnimationSetList();

		// skinned meshの関数
		HRESULT setupBoneMatrixPointers(LPD3DXFRAME pFrame);
		HRESULT setupBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER pMeshContainerBase);
		void updateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix);
		void renderFrame(LPD3DXFRAME pFrame);
		void renderMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase);

		// アニメーション用
		acut::AllocateHierarchy m_allocateHierarchy; //!< フレーム階層構築、解放用
		acut::AnimationFrame* m_pRootFrame; //!< Root Frame へのポインタ
		LPD3DXANIMATIONCONTROLLER m_pAnimationController; //!< アニメーションコントローラへのポインタ
		std::vector<LPD3DXANIMATIONSET> m_animationSetList; //!< Xファイルに含まれるアニメーションセットのリスト
	};



};



#endif // #ifndef ACUT_ANIMATION_MANAGER_H__
