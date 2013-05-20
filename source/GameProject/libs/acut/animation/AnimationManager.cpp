#include "../acut.h"



namespace acut {


	/**
	 * @brief constructor
	 *
	 */
	AnimationManager::AnimationManager() :
		m_allocateHierarchy(),
		m_pRootFrame(NULL), 
		m_pAnimationController(NULL),
		m_animationSetList()
	{
		// 特に処理は無し
	}


	/**
	 * @brief destructor
	 *
	 */
	AnimationManager::~AnimationManager()
	{
		// 特に処理は無し
	}


	/**
	 * @brief メッシュの読み込み
	 *
	 */
	void AnimationManager::loadMeshHierarchyFromX(const acut::TString& meshFilePath)
	{
		try {
			// mesh 読み込み
			V_THROW(D3DXLoadMeshHierarchyFromX(
				meshFilePath.c_str(),
				D3DXMESH_MANAGED,
				DXUTGetD3D9Device(),
				&m_allocateHierarchy,
				NULL,
				reinterpret_cast<LPD3DXFRAME*>(&m_pRootFrame),
				&m_pAnimationController
			));

			// ボーン変換行列へのポインタをセット
			V_THROW(this->setupBoneMatrixPointers(m_pRootFrame));

			// animation controller の trackを全てFALSEにする
			DWORD numTracks = m_pAnimationController->GetMaxNumTracks();
			DEBUG_OUTPUTF("m_pAnimationController->GetMaxNumTracks() : %d\n", numTracks);
			for (DWORD i = 0; i < numTracks; ++i) {
				V_THROW(m_pAnimationController->SetTrackEnable(i, FALSE));
			}

			V_THROW(this->createAnimationSetList());

			try {
				// tyny_4anim.xの場合
				// 0: 手を振る Wave
				// 1: 走る Jog
				// 2: 歩く Walk
				// 3: ふらふらする Loiter
				V_THROW(m_pAnimationController->SetTrackAnimationSet(0, m_animationSetList.at(1)));
				V_THROW(m_pAnimationController->SetTrackEnable(0, TRUE));
			} catch (std::out_of_range& ex) {
				acut::ExceptionHandler::handleException(ex);
				throw acut::Exception(__FILEW__, __LINE__, __FUNCTIONW__);
			}
		} catch (acut::Exception& ex) {
			acut::ExceptionHandler::handleException(ex);
			this->cleanup();
			throw;
		}
	}


	/**
	 * @brief 後片付け
	 *
	 */
	void AnimationManager::cleanup()
	{
		SAFE_RELEASE(m_pAnimationController);
		if (NULL == m_pRootFrame) {
			return;
		}
		V_THROW(m_allocateHierarchy.DestroyFrame(m_pRootFrame));
		m_pRootFrame = NULL;

		// Animation Setの解放
		std::for_each(m_animationSetList.begin(), m_animationSetList.end(), acut::Release<LPD3DXANIMATIONSET>());
		m_animationSetList.clear();
	}


	/**
	 * @brief
	 * 
	 * Returns the index of an animation set within this animation instance's
	 * animation controller given an animation set name.
	 *
	 */
	HRESULT AnimationManager::createAnimationSetList()
	{
		HRESULT hr = D3D_OK;
		LPD3DXANIMATIONSET pAnimationSet = NULL;

		for (DWORD i = 0; i < m_pAnimationController->GetNumAnimationSets(); ++i) {
			V(m_pAnimationController->GetAnimationSet(i, &pAnimationSet));
			if (FAILED(hr)) {
				continue;
			}
			m_animationSetList.push_back(pAnimationSet);
		}
		DEBUG_OUTPUTF("m_animationSetList.size() : %d\n", m_animationSetList.size());

		return hr;
	}


	/**
	 * @brief
	 * 引数に渡された名前と一致する
	 * アニメーションセットのインデックスを返す
	 * 
	 */
	UINT AnimationManager::getAnimationSetIndex(const std::string& animationSetName)
	{
		LPD3DXANIMATIONSET pAnimationSet = 0;

		for (UINT i = 0; i < m_pAnimationController->GetNumAnimationSets(); ++i) {
			V_THROW(m_pAnimationController->GetAnimationSet(i, &pAnimationSet));

			if (animationSetName == std::string(pAnimationSet->GetName())) {
				pAnimationSet->Release();
				return i;
			}

			pAnimationSet->Release();
		}

		return 0xffffff;
	}


	/**
	 * @brief アニメーションの切り替え
	 *
	 */
	void AnimationManager::changeAnimationSet(const int index)
	{
		try {
			//m_pAnimationController->ResetTime();
			//V_THROW(m_pAnimationController->SetTrackEnable(0, FALSE));
			//V_THROW(m_pAnimationController->KeyTrackEnable(0, FALSE, 0.0));
			m_pAnimationController->UnkeyAllTrackEvents(0);
			V_THROW(m_pAnimationController->SetTrackAnimationSet(0, m_animationSetList.at(index)));
			V_THROW(m_pAnimationController->SetTrackEnable(0, TRUE));
			//m_pAnimationController->ResetTime();
		} catch (std::out_of_range& ex) {
			acut::ExceptionHandler::handleException(ex);
			throw acut::Exception(__FILEW__, __LINE__, __FUNCTIONW__);
		}
	}


	/**
	 * @brief
	 * Initialize a new track in the animation controller for the movement
	 * animation (run or walk), and set up the smooth transition from the idle
	 * animation (current track) to it (new track).
	 *
	 */
	/*
	void AnimationManager::setMoveKey()
	{
		DWORD dwNewTrack = ( m_dwCurrentTrack == 0 ? 1 : 0 );
		LPD3DXANIMATIONSET pAnimationSet;

		if (m_fSpeed == m_fSpeedWalk) {
			m_pAnimationController->GetAnimationSet(m_dwAnimIdxWalk, &pAnimationSet);
		}
		else {
			m_pAnimationController->GetAnimationSet(m_dwAnimIdxJog, &pAnimationSet);
		}

		m_pAnimationController->SetTrackAnimationSet(dwNewTrack, pAnimationSet);
		pAnimationSet->Release();

		m_pAnimationController->UnkeyAllTrackEvents(m_dwCurrentTrack);
		m_pAnimationController->UnkeyAllTrackEvents(dwNewTrack);

		m_pAnimationController->KeyTrackEnable(m_dwCurrentTrack, FALSE, m_dTimeCurrent + MOVE_TRANSITION_TIME );
		m_pAnimationController->KeyTrackSpeed(m_dwCurrentTrack, 0.0f, m_dTimeCurrent, MOVE_TRANSITION_TIME, D3DXTRANSITION_LINEAR);
		m_pAnimationController->KeyTrackWeight( m_dwCurrentTrack, 0.0f, m_dTimeCurrent, MOVE_TRANSITION_TIME, D3DXTRANSITION_LINEAR);
		m_pAnimationController->SetTrackEnable(dwNewTrack, TRUE);
		m_pAnimationController->KeyTrackSpeed(dwNewTrack, 1.0f, m_dTimeCurrent, MOVE_TRANSITION_TIME, D3DXTRANSITION_LINEAR );
		m_pAnimationController->KeyTrackWeight(dwNewTrack, 1.0f, m_dTimeCurrent, MOVE_TRANSITION_TIME, D3DXTRANSITION_LINEAR);

		m_dwCurrentTrack = dwNewTrack;
	}
	*/


	/**
	 * @brief skinned mesh animation を進める
	 *
	 * @param elapsedTime 経過時間
	 * @return AdvanceTime()の実行結果
	 *
	 */
	HRESULT AnimationManager::animate(float elapsedTime)
	{
#if defined(DEBUG) | defined(_DEBUG)
		HRESULT hr = m_pAnimationController->AdvanceTime(elapsedTime, NULL);
		V(hr);
		return hr;
#else
		return m_pAnimationController->AdvanceTime(elapsedTime, NULL);
#endif
	}


	/**
	 * @brief 描画処理
	 *
	 */
	void AnimationManager::render(D3DXMATRIX& worldMatrix)
	{
		// skinned mesh
		this->updateFrameMatrices(m_pRootFrame, &worldMatrix);
		this->renderFrame(m_pRootFrame);
	}


	/**
	 * @brief skinned mesh
	 * 
	 * Called to setup the pointers for a given bone to its transformation matrix
	 *
	 */
	HRESULT AnimationManager::setupBoneMatrixPointers(LPD3DXFRAME pFrame)
	{
		HRESULT hr = S_OK;

		if (NULL != pFrame->pMeshContainer) {
			V_RETURN(this->setupBoneMatrixPointersOnMesh(pFrame->pMeshContainer));
		}

		if (NULL != pFrame->pFrameSibling) {
			V_RETURN(this->setupBoneMatrixPointers(pFrame->pFrameSibling));
		}

		if (NULL != pFrame->pFrameFirstChild) {
			V_RETURN(this->setupBoneMatrixPointers(pFrame->pFrameFirstChild));
		}

		return hr;
	}


	/**
	 * @brief skinned mesh
	 * 
	 * Called to setup the pointers for a given bone to its transformation matrix
	 *
	 */
	HRESULT AnimationManager::setupBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER pMeshContainerBase)
	{
		acut::AnimationMeshContainer* pMeshContainer = static_cast<acut::AnimationMeshContainer*>(pMeshContainerBase);

		// もしスキンメッシュなら、ボーン行列をセットアップする
	    if (NULL != pMeshContainer->pSkinInfo) {
	        DWORD numberOfBones = pMeshContainer->pSkinInfo->GetNumBones();

			// ボーン行列のポインタへの配列を作成
			pMeshContainer->m_boneMatrixPointers.reserve(numberOfBones);
	        for (DWORD iBone = 0; iBone < numberOfBones; ++iBone) {
				acut::AnimationFrame* pFrame = static_cast<acut::AnimationFrame*>(D3DXFrameFind(
					m_pRootFrame,
	                pMeshContainer->pSkinInfo->GetBoneName(iBone)
				));
				if (pFrame == NULL) {
	                return E_FAIL;
				}

	            pMeshContainer->m_boneMatrixPointers.push_back(&pFrame->CombinedTransformationMatrix);
	        }
	    }

	    return S_OK;
	}


	/**
	 * @brief skinned mesh
	 *
	 * update the frame matrices
	 *
	 * @param
	 * @param
	 *
	 */
	void AnimationManager::updateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
	{
		acut::AnimationFrame* pFrame = static_cast<acut::AnimationFrame*>(pFrameBase);

		if (NULL != pParentMatrix) {
	        D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);
		}
		else {
	        pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;
		}

	    if (NULL != pFrame->pFrameSibling) {
	        this->updateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
	    }

	    if (NULL != pFrame->pFrameFirstChild) {
	        this->updateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
	    }
	}


	/**
	 * @brief skinned mesh
	 *
	 */
	void AnimationManager::renderFrame(LPD3DXFRAME pFrame)
	{
		LPD3DXMESHCONTAINER pMeshContainer = pFrame->pMeshContainer;
		while (NULL != pMeshContainer) {
			this->renderMeshContainer(pMeshContainer, pFrame);
			pMeshContainer = pMeshContainer->pNextMeshContainer;
		}

		if (NULL != pFrame->pFrameSibling) {
			this->renderFrame(pFrame->pFrameSibling);
		}

		if (NULL != pFrame->pFrameFirstChild) {
			this->renderFrame(pFrame->pFrameFirstChild);
		}
	}


	/**
	 * @brief skinned mesh
	 *
	 * Called to render a mesh in the hierarchy
	 *
	 */
	void AnimationManager::renderMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase)
	{
		LPDIRECT3DDEVICE9 pd3dDevice = DXUTGetD3D9Device();
		acut::AnimationMeshContainer* pMeshContainer = static_cast<acut::AnimationMeshContainer*>(pMeshContainerBase);
		acut::AnimationFrame* pFrame = static_cast<acut::AnimationFrame*>(pFrameBase);
		HRESULT hr;

	    // first check for skinning
	    if (NULL != pMeshContainer->pSkinInfo) {
            DWORD AttribIdPrev = UNUSED32;
            LPD3DXBONECOMBINATION pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(
				pMeshContainer->m_pBoneCombinationTable->GetBufferPointer()
			);

			D3DCAPS9 d3dCaps;
			pd3dDevice->GetDeviceCaps(&d3dCaps);
            // Draw using default vtx processing of the device (typically HW)
            for (UINT iAttrib = 0; iAttrib < pMeshContainer->m_numberOfBoneCombinations; ++iAttrib) {
                UINT NumBlend = 0;
                for (DWORD i = 0; i < pMeshContainer->m_maxNumberOfFaceInfl; ++i) {
                    if (pBoneComb[iAttrib].BoneId[i] != UINT_MAX) {
                        NumBlend = i;
                    }
                }

                if (NumBlend + 1 <= d3dCaps.MaxVertexBlendMatrices) {
                    // first calculate the world matrices for the current set of blend weights and get the accurate count of the number of blends
                    for (DWORD i = 0; i < pMeshContainer->m_maxNumberOfFaceInfl; ++i) {
                        UINT iMatrixIndex = pBoneComb[iAttrib].BoneId[i];
                        if (iMatrixIndex != UINT_MAX) {
							D3DXMATRIXA16 matTemp;
                            D3DXMatrixMultiply(
								&matTemp,
								&pMeshContainer->m_boneOffsetMatrices[iMatrixIndex],
								pMeshContainer->m_boneMatrixPointers[iMatrixIndex]
							);
                            V(pd3dDevice->SetTransform(D3DTS_WORLDMATRIX(i), &matTemp));
                        }
                    }

                    V(pd3dDevice->SetRenderState(D3DRS_VERTEXBLEND, NumBlend));

                    // lookup the material used for this subset of faces
                    if ((AttribIdPrev != pBoneComb[iAttrib].AttribId) || (AttribIdPrev == UNUSED32)) {
                        V(pd3dDevice->SetMaterial(&pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D));
                        V(pd3dDevice->SetTexture(0, pMeshContainer->m_textures[pBoneComb[iAttrib].AttribId]));
                        AttribIdPrev = pBoneComb[iAttrib].AttribId;
                    }

                    // draw the subset now that the correct material and matrices are loaded
                    V(pMeshContainer->MeshData.pMesh->DrawSubset(iAttrib));
                }
            }

            V(pd3dDevice->SetRenderState(D3DRS_VERTEXBLEND, 0));
	    }
	    else { // standard mesh, just draw it after setting material properties
			// ここは通らないらしい
			// throw acut::Exception();
			V(pd3dDevice->SetTransform(D3DTS_WORLD, &pFrame->CombinedTransformationMatrix));
			for (DWORD iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; ++iMaterial) {
				V(pd3dDevice->SetMaterial(&pMeshContainer->pMaterials[iMaterial].MatD3D));
				V(pd3dDevice->SetTexture(0, pMeshContainer->m_textures[iMaterial]));
				V(pMeshContainer->MeshData.pMesh->DrawSubset(iMaterial));
			}
		}
	}


};

