/** @file
 * @brief
 * Implementation of the MultiAnimationAllocateHierarchy class, which
 * handles creating and destroying animation frames and mesh containers
 * for the MultiAnimationation library.
 *
 */



#include "../acut.h"



namespace acut {


	/**
	 * @brief
	 * Called by D3DX during the loading of a mesh hierarchy.  The app can
	 * customize its behavior.  At a minimum, the app should allocate a
	 * D3DXFRAME or a child of it and fill in the Name member.
	 *
	 * @param Name
	 * @param ppNewFrame
	 * @return
	 *
	 */
	HRESULT AllocateHierarchy::CreateFrame(THIS_ LPCSTR Name, LPD3DXFRAME* ppNewFrame)
	{
		acut::AnimationFrame* pFrame = NULL;
		try {
			// フレームを新しく生成する
			pFrame = new acut::AnimationFrame;
			if (NULL == pFrame) {
				throw acut::Exception();
			}

			// フレーム名のコピー
			pFrame->Name = MemoryUtility::copyStringToHeap(Name);
			if (NULL == pFrame->Name) {
				throw acut::Exception();
			}

			*ppNewFrame = pFrame;

			return D3D_OK;
		} catch (acut::Exception& ex) {
			ExceptionHandler::handleException(ex);
			if (NULL != pFrame) {
				if (FAILED(this->DestroyFrame(pFrame))) {
					DEBUG_OUTPUTF("%s() : %s\n", __FUNCTIONW__, "DestroyFrame() Error!!");
					// TODO: Log出力
				}
			}
			return E_OUTOFMEMORY;
		}
	}


	/**
	 * @brief
	 *
	 * Called by D3DX during the loading of a mesh hierarchy. At a minumum,
	 * the app should allocate a D3DXMESHCONTAINER or a child of it and fill
	 * in the members based on the parameters here.  The app can further
	 * customize the allocation behavior here.  In our case, we initialize
	 * m_amxBoneOffsets from the skin info for convenience reason.
	 * Then we call ConvertToIndexedBlendedMesh to obtain a new mesh object
	 * that's compatible with the palette size we have to work with.
	 *
	 * @param Name
	 * @param pMeshData,
	 * @param pMaterials,
	 * @param pEffectInstances,
	 * @param NumMaterials,
	 * @param pAdjacency,
	 * @param pSkinInfo,
	 * @param ppNewMeshContainer
	 * @return  
	 *
	 */
	HRESULT AllocateHierarchy::CreateMeshContainer(THIS_
	    LPCSTR Name,
	    CONST D3DXMESHDATA* pMeshData,
	    CONST D3DXMATERIAL* pMaterials,
	    CONST D3DXEFFECTINSTANCE* pEffectInstances,
	    DWORD NumMaterials,
	    CONST DWORD* pAdjacency,
	    LPD3DXSKININFO pSkinInfo,
	    LPD3DXMESHCONTAINER* ppNewMeshContainer
	)
	{
		acut::AnimationMeshContainer* pMeshContainer = NULL;
		try {
			pMeshContainer = new acut::AnimationMeshContainer;
			if (NULL == pMeshContainer) {
				throw acut::Exception();
			}

			// Nameのコピー
			pMeshContainer->Name = MemoryUtility::copyStringToHeap(Name);
			if (NULL == pMeshContainer->Name) {
				throw acut::Exception();
			}

			// Meshのコピー
			if (D3DXMESHTYPE_MESH == pMeshData->Type) {
				// 通常メッシュ
				pMeshContainer->MeshData.pMesh = pMeshData->pMesh;
				pMeshContainer->MeshData.pMesh->AddRef();
			}
			//else if (D3DXMESHTYPE_PMESH == pMeshData->Type) {
				// TODO: プログレッシブメッシュに対応する
			//	pMeshContainer->MeshData.pPMesh = pMeshData->pPMesh;
			//	pMeshContainer->MeshData.pPMesh->AddRef();
			//}
			//else if (D3DXMESHTYPE_PATCHMESH == pMeshData->Type) {
				// TODO: パッチメッシュに対応する
			//	pMeshContainer->MeshData.pPatchMesh = pMeshData->pPatchMesh;
			//	pMeshContainer->MeshData.pPatchMesh->AddRef();
			//}
			else {
				throw acut::Exception();
			}
		    pMeshContainer->MeshData.Type = pMeshData->Type;

			// MaterialsのコピーとTextureの作成
			if (0 < NumMaterials) {
				// Materialsのコピー
				pMeshContainer->pMaterials = new D3DXMATERIAL[NumMaterials];
				CopyMemory(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);

				// Textureの作成
				pMeshContainer->m_textures.resize(NumMaterials, NULL);
				for (DWORD i = 0; i < NumMaterials; ++i) {
		            if (NULL != pMeshContainer->pMaterials[i].pTextureFilename) {
		                WCHAR strTexturePath[MAX_PATH];
		                WCHAR wszBuf[MAX_PATH];
		                MultiByteToWideChar(CP_ACP, 0, pMeshContainer->pMaterials[i].pTextureFilename, -1, wszBuf, MAX_PATH);
		                wszBuf[MAX_PATH - 1] = L'\0';
		                DXUTFindDXSDKMediaFileCch(strTexturePath, MAX_PATH, wszBuf);
		                if (FAILED(D3DXCreateTextureFromFile(
							DXUTGetD3D9Device(),
							strTexturePath,
		                    &pMeshContainer->m_textures[i]
						))) {
		                    pMeshContainer->m_textures[i] = NULL;
						}
					    // don't remember a pointer into the dynamic memory, just forget the name after loading
		                pMeshContainer->pMaterials[i].pTextureFilename = NULL;
		            }
				}
			}

			// エフェクトデータのコピー
			pMeshContainer->pEffects = new D3DXEFFECTINSTANCE;
			if (NULL == pMeshContainer->pEffects) {
				throw acut::Exception();
			}
			pMeshContainer->pEffects->pEffectFilename = MemoryUtility::copyStringToHeap(pEffectInstances->pEffectFilename);
//			if (NULL == pMeshContainer->pEffects->pEffectFilename) {
//				throw acut::Exception();
//			}
			pMeshContainer->pEffects->NumDefaults = pEffectInstances->NumDefaults;
			pMeshContainer->pEffects->pDefaults = new D3DXEFFECTDEFAULT[pEffectInstances->NumDefaults];
			if (NULL == pMeshContainer->pEffects->pDefaults) {
				throw acut::Exception();
			}
			LPD3DXEFFECTDEFAULT pDefaultsSource = pEffectInstances->pDefaults; // コピー元
			LPD3DXEFFECTDEFAULT pDefaultsDestination = pMeshContainer->pEffects->pDefaults; // コピー先
			for (DWORD i = 0; i < pEffectInstances->NumDefaults; ++i) {
				pDefaultsDestination[i].pParamName = MemoryUtility::copyStringToHeap(pDefaultsSource[i].pParamName);
				if (NULL == pDefaultsDestination[i].pParamName) {
					throw acut::Exception();
				}
				pDefaultsDestination[i].NumBytes = pDefaultsSource[i].NumBytes;
				pDefaultsDestination[i].Type = pDefaultsSource[i].Type;
				if (pDefaultsSource[i].Type <= D3DXEDT_DWORD) {
					pDefaultsDestination[i].pValue = new DWORD[pDefaultsDestination[i].NumBytes];
					if (NULL == pDefaultsDestination[i].pValue) {
						throw acut::Exception();
					}
					CopyMemory(pDefaultsDestination[i].pValue, pDefaultsSource[i].pValue, pDefaultsDestination[i].NumBytes);
				}
			}

			// マテリアルの数のコピー
			pMeshContainer->NumMaterials = NumMaterials;

			// 隣接ポリゴンインデックスのコピー
			const unsigned Triangle_Polygon_Vertex_Num = 3;
			DWORD NumPolygon = pMeshData->pMesh->GetNumFaces(); // ポリゴン数
			pMeshContainer->pAdjacency = new DWORD[NumPolygon * Triangle_Polygon_Vertex_Num];
			if (NULL == pMeshContainer->pAdjacency) {
				throw acut::Exception();
			}
			CopyMemory(pMeshContainer->pAdjacency, pAdjacency, NumPolygon * Triangle_Polygon_Vertex_Num * sizeof(DWORD));

			// スキン情報のコピー
			if (NULL != pSkinInfo) {
				pMeshContainer->pSkinInfo = pSkinInfo;
				pMeshContainer->pSkinInfo->AddRef();

				pMeshContainer->m_pOriginMesh = pMeshContainer->MeshData.pMesh;
				pMeshContainer->MeshData.pMesh = NULL;

				// get each of the bone offset matrices so that we don't need to get them later
				pMeshContainer->m_boneOffsetMatrices.reserve(pSkinInfo->GetNumBones());
				for (DWORD iBone = 0; iBone < pSkinInfo->GetNumBones(); ++iBone) {
					pMeshContainer->m_boneOffsetMatrices.push_back(*(pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(iBone)));
				}

				AllocateHierarchy::generateSkinnedMesh(pMeshContainer);
			}

			*ppNewMeshContainer = pMeshContainer;
			pMeshContainer = NULL;

			return D3D_OK;
		} catch (acut::Exception& ex) {
			ExceptionHandler::handleException(ex);
	        if (NULL != pMeshContainer) {
	            this->DestroyMeshContainer(pMeshContainer);
			}
			return E_OUTOFMEMORY;
		}

/*
	    assert(m_pMultiAnimation);

	    *ppNewMeshContainer = NULL;
	    HRESULT hr = S_OK;
	    MultiAnimationMeshContainer* pMeshContainer = new MultiAnimationMeshContainer;
	    if (NULL == pMeshContainer) {
			hr = E_OUTOFMEMORY;
			goto e_Exit;
		}

	    ZeroMemory(pMeshContainer, sizeof(MultiAnimationMeshContainer));

	    // if this is a static mesh, exit early; we're only interested in skinned meshes
	    if (NULL == pSkinInfo) { // skinned meshesでなぁE??吁EULLなのだろうか！E
	        hr = S_OK;
	        goto e_Exit;
	    }

	    // only support mesh type
	    if (pMeshData->Type != D3DXMESHTYPE_MESH) {
			hr = E_FAIL; 
			goto e_Exit;
		}

		if (Name) {
	        pMeshContainer->Name = (CHAR*)HeapCopy((CHAR*)Name);
		}
		else {
	        pMeshContainer->Name = (CHAR*)HeapCopy("<no_name>");
		}

	    // copy the mesh over
	    pMeshContainer->MeshData.Type = pMeshData->Type;
	    pMeshContainer->MeshData.pMesh = pMeshData->pMesh;
	    pMeshContainer->MeshData.pMesh->AddRef();

	    // copy adjacency over
		{
	        DWORD dwNumFaces = pMeshContainer->MeshData.pMesh->GetNumFaces();
	        pMeshContainer->pAdjacency = new DWORD[3 * dwNumFaces];
	        if (pMeshContainer->pAdjacency == NULL) {
				hr = E_OUTOFMEMORY;
				goto e_Exit; 
			}

	        CopyMemory(pMeshContainer->pAdjacency, pAdjacency, 3 * sizeof(DWORD) * dwNumFaces);
	    }

	    // ignore effects instances
	    pMeshContainer->pEffects = NULL;

	    // alloc and copy materials
	    pMeshContainer->NumMaterials = max(1, NumMaterials);
	    pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
	    pMeshContainer->m_apTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
	    if (pMeshContainer->pMaterials == NULL || pMeshContainer->m_apTextures == NULL) {
			hr = E_OUTOFMEMORY; 
			goto e_Exit; 
		}

	    if (0 < NumMaterials) {
	        CopyMemory(pMeshContainer->pMaterials, pMaterials, NumMaterials * sizeof(D3DXMATERIAL));
	        for (DWORD i = 0; i < NumMaterials; ++i) {
				if (pMeshContainer->pMaterials[i].pTextureFilename) {
				    // CALLBACK to get valid filename
	                WCHAR sNewPath[MAX_PATH];
	                WCHAR wszTexName[MAX_PATH];
					if (MultiByteToWideChar(
							CP_ACP,
							0, 
							pMeshContainer->pMaterials[i].pTextureFilename,
							-1, 
							wszTexName, 
							MAX_PATH
						) == 0
					) {
	                    pMeshContainer->m_apTextures[i] = NULL;
					}
	                else if (SUCCEEDED(DXUTFindDXSDKMediaFileCch(sNewPath, MAX_PATH, wszTexName))) {
						// create the D3D texture
	                    if (FAILED(D3DXCreateTextureFromFile(
							m_pMultiAnimation->m_pDevice,
	                        sNewPath,
	                        &pMeshContainer->m_apTextures[i]))
						) {
	                        pMeshContainer->m_apTextures[i] = NULL;
	                    }
					}
					else {
	                    pMeshContainer->m_apTextures[i] = NULL;
					}
	            }
				else {
	                pMeshContainer->m_apTextures[i] = NULL;
				}
		        
		    }
		}
		else { // mock up a default material and set it
	        ZeroMemory(&pMeshContainer->pMaterials[0].MatD3D, sizeof(D3DMATERIAL9));
	        pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
	        pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
	        pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
	        pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
	        pMeshContainer->pMaterials[0].pTextureFilename = NULL;
	    }

	    // save the skininfo object
	    pMeshContainer->pSkinInfo = pSkinInfo;
	    pSkinInfo->AddRef();

	    // Get the bone offset matrices from the skin info
	    pMeshContainer->m_amxBoneOffsets = new D3DXMATRIX[pSkinInfo->GetNumBones()];
	    if (pMeshContainer->m_amxBoneOffsets == NULL) {
			hr = E_OUTOFMEMORY;
			goto e_Exit;
		}
		{
			for (DWORD i = 0; i < pSkinInfo->GetNumBones(); ++i) {
	            pMeshContainer->m_amxBoneOffsets[i] = *(D3DXMATRIX*)pSkinInfo->GetBoneOffsetMatrix(i);
			}
	    }

	    //
	    // Determine the palette size we need to work with, then call ConvertToIndexedBlendedMesh
	    // to set up a new mesh that is compatible with the palette size.
	    //
		{
	        UINT iPaletteSize = 0;
	        m_pMultiAnimation->m_pEffect->GetInt("MATRIX_PALETTE_SIZE", (INT*)&iPaletteSize);
			pMeshContainer->m_dwNumPaletteEntries = __min(iPaletteSize, pMeshContainer->pSkinInfo->GetNumBones());
	    }

	    // generate the skinned mesh - creates a mesh with blend weights and indices
	    hr = pMeshContainer->pSkinInfo->ConvertToIndexedBlendedMesh(
			pMeshContainer->MeshData.pMesh,
	        D3DXMESH_MANAGED | D3DXMESHOPT_VERTEXCACHE,
	        pMeshContainer->m_dwNumPaletteEntries,
	        pMeshContainer->pAdjacency,
	        NULL,
	        NULL,
	        NULL,
	        &pMeshContainer->m_dwMaxNumFaceInfls,
	        &pMeshContainer->m_dwNumAttrGroups,
	        &pMeshContainer->m_pBufBoneCombos,
	        &pMeshContainer->m_pWorkingMesh );
		if (FAILED(hr)) {
	        goto e_Exit;
		}

	    // Make sure the working set is large enough for this mesh.
	    // This is a bone array used for all mesh containers as a working
	    // set during drawing.  If one was created previously that isn't 
	    // large enough for this mesh, we have to reallocate.
	    if (m_pMultiAnimation->m_dwWorkingPaletteSize < pMeshContainer->m_dwNumPaletteEntries) {
			if (m_pMultiAnimation->m_amxWorkingPalette) {
	            delete [] m_pMultiAnimation->m_amxWorkingPalette;
			}
	        m_pMultiAnimation->m_dwWorkingPaletteSize = pMeshContainer->m_dwNumPaletteEntries;
	        m_pMultiAnimation->m_amxWorkingPalette = new D3DXMATRIX[ m_pMultiAnimation->m_dwWorkingPaletteSize ];
	        if (m_pMultiAnimation->m_amxWorkingPalette == NULL) {
	            m_pMultiAnimation->m_dwWorkingPaletteSize = 0;
	            hr = E_OUTOFMEMORY;
	            goto e_Exit;
	        }
	    }

	    // ensure the proper vertex format for the mesh
		{
	        DWORD dwOldFVF = pMeshContainer->m_pWorkingMesh->GetFVF();
	        DWORD dwNewFVF = ( dwOldFVF & D3DFVF_POSITION_MASK ) | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_LASTBETA_UBYTE4;
	        if (dwNewFVF != dwOldFVF) {
	            LPD3DXMESH pMesh;
	            hr = pMeshContainer->m_pWorkingMesh->CloneMeshFVF(
					pMeshContainer->m_pWorkingMesh->GetOptions(),
	                dwNewFVF,
	                m_pMultiAnimation->m_pDevice,
	                &pMesh
				);
				if (FAILED(hr)) {
	                goto e_Exit;
				}

	            pMeshContainer->m_pWorkingMesh->Release();
	            pMeshContainer->m_pWorkingMesh = pMesh;

			    // if the loaded mesh didn't contain normals, compute them here
	            if (!(dwOldFVF & D3DFVF_NORMAL)) {
	                hr = D3DXComputeNormals(pMeshContainer->m_pWorkingMesh, NULL);
					if (FAILED(hr)) {
	                    goto e_Exit;
					}
	            }
	        }
	    }

	    // Interpret the UBYTE4 as a D3DCOLOR.
	    // The GeForce3 doesn't support the UBYTE4 decl type.  So, we convert any
	    // blend indices to a D3DCOLOR semantic, and later in the shader convert
	    // it back using the D3DCOLORtoUBYTE4() intrinsic.  Note that we don't
	    // convert any data, just the declaration.
	    D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];
	    D3DVERTEXELEMENT9 * pDeclCur;
	    hr = pMeshContainer->m_pWorkingMesh->GetDeclaration(pDecl);
		if (FAILED(hr)) {
	        goto e_Exit;
		}

	    pDeclCur = pDecl;
	    while (pDeclCur->Stream != 0xff) {
			if ((pDeclCur->Usage == D3DDECLUSAGE_BLENDINDICES) && (pDeclCur->UsageIndex == 0)) {
	            pDeclCur->Type = D3DDECLTYPE_D3DCOLOR;
			}
	        pDeclCur++;
	    }

	    hr = pMeshContainer->m_pWorkingMesh->UpdateSemantics(pDecl);
	    if (FAILED(hr)) {
	        goto e_Exit;
		}

	e_Exit:

	    if (FAILED(hr)) {
	        if (pMeshContainer) {
	            DestroyMeshContainer(pMeshContainer);
			}
	    }
	    else {
	        *ppNewMeshContainer = pMeshContainer;
		}

	    return hr;


*/
	}


	/**
	 * @brief
	 *
	 * Called by D3DX during the release of a mesh hierarchy.  Here we should
	 * free all resources allocated in CreateFrame().
	 *
	 * @param pFrameToFree 解放するフレームへのポインタ
	 * @return 成功の場合D3D_OK。失敗の場合はエラーコード。
	 *
	 */
	HRESULT AllocateHierarchy::DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree)
	{
		_ASSERT_EXPR(NULL != pFrameToFree, _T("AllocateHierarchy::DestroyFrame() にNULLを渡さないでください!!"));
		if (NULL == pFrameToFree) {
			return E_INVALIDARG;
		}

		// Frame解放処理
		SAFE_DELETE_ARRAY(pFrameToFree->Name); // フレーム名解放
		HRESULT hr = D3D_OK;
		// メッシュコンテナの解放
		if (NULL != pFrameToFree->pMeshContainer) {
			HRESULT hrMesh = this->DestroyMeshContainer(pFrameToFree->pMeshContainer);
			if (FAILED(hrMesh)) {
				V(hrMesh);
				// TODO: Log
			}
		}
		// 兄弟フレームの解放
		if (NULL != pFrameToFree->pFrameSibling) {
			HRESULT hrSibling = this->DestroyFrame(pFrameToFree->pFrameSibling);
			if (FAILED(hrSibling)) {
				V(hrSibling);
				// TODO: Log
			}
		}
		// 子フレームの解放
		if (NULL != pFrameToFree->pFrameFirstChild) {
			HRESULT hrChild = this->DestroyFrame(pFrameToFree->pFrameFirstChild);
			if (FAILED(hrChild)) {
				V(hrChild);
				// TODO: Log
			}
		}

		SAFE_DELETE(pFrameToFree); // フレームの解放

		return hr;

		/*
	    assert(m_pMultiAnimation);

	    MultiAnimationFrame* pFrame = (MultiAnimationFrame*)pFrameToFree;

	    if (pFrame->Name) {
	        delete [] pFrame->Name;
		}

	    delete pFrame;

	    return S_OK;
		*/
	}


	/**
	 * @brief
	 * 
	 * Called by D3DX during the release of a mesh hierarchy.  Here we should
	 * free all resources allocated in CreateMeshContainer().
	 *
	 * @param pMeshContainerToFree 解放するメッシュコンテナへのポインタ
	 * @return D3D_OK or E_INVALIDARG
	 *
	 */
	HRESULT AllocateHierarchy::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
	{
		_ASSERT_EXPR(NULL != pMeshContainerToFree, _T("AllocateHierarchy::DestroyMeshContainer() にNULLを渡さないでください!!"));
		if (NULL == pMeshContainerToFree) {
			return E_INVALIDARG;
		}

		acut::AnimationMeshContainer* pMeshContainer = static_cast<acut::AnimationMeshContainer*>(pMeshContainerToFree);

		// D3DXMESHCONTAINER部分の解放
		SAFE_DELETE_ARRAY(pMeshContainer->Name); // 名前の解放
		SAFE_RELEASE(pMeshContainer->MeshData.pMesh); // メッシュデータの解放

		// テクスチャ名の解放
		for (DWORD i = 0; i < pMeshContainer->NumMaterials; ++i) {
			SAFE_DELETE_ARRAY(pMeshContainer->pMaterials[i].pTextureFilename);
		}

		SAFE_DELETE_ARRAY(pMeshContainer->pMaterials); // マテリアルの解放

		// エフェクトデータの解放
		for (DWORD i = 0; i < pMeshContainer->pEffects->NumDefaults; ++i) {
			SAFE_DELETE_ARRAY(pMeshContainer->pEffects->pDefaults[i].pParamName);
			SAFE_DELETE_ARRAY(pMeshContainer->pEffects->pDefaults[i].pValue);
		}
		SAFE_DELETE_ARRAY(pMeshContainer->pEffects->pDefaults);
		SAFE_DELETE_ARRAY(pMeshContainer->pEffects->pEffectFilename);
		SAFE_DELETE(pMeshContainer->pEffects);

		SAFE_DELETE_ARRAY(pMeshContainer->pAdjacency); // 隣接ポリゴンインデックスの解放
		SAFE_RELEASE(pMeshContainer->pSkinInfo); // スキン情報の解放

		// acut::AnimationMeshContainer部分の解放
		SAFE_RELEASE(pMeshContainer->m_pOriginMesh); // メッシュデータの解放
		SAFE_RELEASE(pMeshContainer->m_pBoneCombinationTable); // ボーンコンビネーションテーブルの解放
		// textureの解放
		std::for_each(pMeshContainer->m_textures.begin(), pMeshContainer->m_textures.end(), acut::Release<LPDIRECT3DTEXTURE9>());

		SAFE_DELETE(pMeshContainer);

		return D3D_OK;

/*
	    assert( m_pMultiAnimation );

	    MultiAnimationMeshContainer * pMeshContainer = (MultiAnimationMeshContainer*)pMeshContainerToFree;

	    if (pMeshContainer->Name) {
	        delete [] pMeshContainer->Name;
		}

	    if (pMeshContainer->MeshData.pMesh) {
	        pMeshContainer->MeshData.pMesh->Release();
		}

	    if (pMeshContainer->pAdjacency) {
	        delete [] pMeshContainer->pAdjacency;
		}

	    if (pMeshContainer->pMaterials) {
	        delete [] pMeshContainer->pMaterials;
		}

	    for (DWORD i = 0; i < pMeshContainer->NumMaterials; ++i) {
	        if (pMeshContainer->m_apTextures[i]) {
	            pMeshContainer->m_apTextures[i]->Release();
			}
	    }

	    if (pMeshContainer->m_apTextures) {
	        delete [] pMeshContainer->m_apTextures;
		}

	    if (pMeshContainer->pSkinInfo) {
	        pMeshContainer->pSkinInfo->Release();
		}

	    if (pMeshContainer->m_amxBoneOffsets) {
	        delete [] pMeshContainer->m_amxBoneOffsets;
		}

	    if (pMeshContainer->m_pWorkingMesh) {
	        pMeshContainer->m_pWorkingMesh->Release();
	        pMeshContainer->m_pWorkingMesh = NULL;
	    }

	    pMeshContainer->m_dwNumPaletteEntries = 0;
	    pMeshContainer->m_dwMaxNumFaceInfls = 0;
	    pMeshContainer->m_dwNumAttrGroups = 0;

	    if (pMeshContainer->m_pBufBoneCombos) {
	        pMeshContainer->m_pBufBoneCombos->Release();
	        pMeshContainer->m_pBufBoneCombos = NULL;
	    }

	    if (pMeshContainer->m_apmxBonePointers) {
	        delete [] pMeshContainer->m_apmxBonePointers;
		}

	    delete pMeshContainerToFree;

	    return S_OK;
*/
	}


	// Called either by CreateMeshContainer when loading a skin mesh, or when 
	// changing methods.  This function uses the pSkinInfo of the mesh 
	// container to generate the desired drawable mesh and bone combination 
	// table.
	HRESULT AllocateHierarchy::generateSkinnedMesh(acut::AnimationMeshContainer* pMeshContainer)
	{
		enum METHOD {
		    D3DNONINDEXED,
		    D3DINDEXED,
		    SOFTWARE,
		    D3DINDEXEDVS,
		    D3DINDEXEDHLSLVS,
		    NONE
		};
		METHOD g_SkinningMethod = D3DNONINDEXED;

	    bool g_bUseSoftwareVP = false;
		UINT g_NumBoneMatricesMax = 0;
		D3DXMATRIXA16* g_pBoneMatrices = NULL;

		LPDIRECT3DDEVICE9 pd3dDevice = DXUTGetD3D9Device();
	    D3DCAPS9 d3dCaps;
	    pd3dDevice->GetDeviceCaps( &d3dCaps );

	    HRESULT hr = S_OK;

		if (NULL == pMeshContainer->pSkinInfo) {
	        return hr;
		}

	    SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
	    SAFE_RELEASE(pMeshContainer->m_pBoneCombinationTable);

	    // if non-indexed skinning mode selected, use ConvertToBlendedMesh to generate drawable mesh
		if (D3DNONINDEXED == g_SkinningMethod) {
			HRESULT hr = pMeshContainer->pSkinInfo->ConvertToBlendedMesh(
				pMeshContainer->m_pOriginMesh,
				D3DXMESH_MANAGED | D3DXMESHOPT_VERTEXCACHE,
				pMeshContainer->pAdjacency,
				NULL,
				NULL,
				NULL,
				&pMeshContainer->m_maxNumberOfFaceInfl,
				&pMeshContainer->m_numberOfBoneCombinations,
				&pMeshContainer->m_pBoneCombinationTable,
				&pMeshContainer->MeshData.pMesh
			);
			if (FAILED(hr)) {
				V(hr);
				throw acut::Exception();	
			}

			LPD3DXBONECOMBINATION pBoneCompinationTable = reinterpret_cast<LPD3DXBONECOMBINATION>(
				pMeshContainer->m_pBoneCombinationTable->GetBufferPointer()
			);

			// look for any set of bone combinations that do not fit the caps
			for (
				pMeshContainer->iAttributeSW = 0;
				pMeshContainer->iAttributeSW < pMeshContainer->m_numberOfBoneCombinations;
				pMeshContainer->iAttributeSW++
			) {
				DWORD cInfl = 0;
				for (DWORD iInfl = 0; iInfl < pMeshContainer->m_maxNumberOfFaceInfl; ++iInfl) {
					if (UINT_MAX != pBoneCompinationTable[pMeshContainer->iAttributeSW].BoneId[iInfl]) {
						++cInfl;
					}
				}

				if (d3dCaps.MaxVertexBlendMatrices < cInfl) {
					break;
				}
			}

			// if there is both HW and SW, add the Software Processing flag
			if (pMeshContainer->iAttributeSW < pMeshContainer->m_numberOfBoneCombinations) {
				// ここは通らないかも
				throw acut::Exception();
				LPD3DXMESH pMeshTmp;

				hr = pMeshContainer->MeshData.pMesh->CloneMeshFVF(
					D3DXMESH_SOFTWAREPROCESSING | pMeshContainer->MeshData.pMesh->GetOptions(),
					pMeshContainer->MeshData.pMesh->GetFVF(),
					pd3dDevice, 
					&pMeshTmp
				);
				if (FAILED(hr)) {
					//goto e_Exit;
					throw acut::Exception();
				}

				pMeshContainer->MeshData.pMesh->Release();
				pMeshContainer->MeshData.pMesh = pMeshTmp;
				pMeshTmp = NULL;
			}
	    }
        // if indexed skinning mode selected, use ConvertToIndexedsBlendedMesh to generate drawable mesh
	    else if( g_SkinningMethod == D3DINDEXED )
	    {
	        DWORD NumMaxFaceInfl;
	        DWORD Flags = D3DXMESHOPT_VERTEXCACHE;

	        LPDIRECT3DINDEXBUFFER9 pIB;
	        hr = pMeshContainer->m_pOriginMesh->GetIndexBuffer( &pIB );
	        if( FAILED( hr ) )
	            goto e_Exit;

	        hr = pMeshContainer->pSkinInfo->GetMaxFaceInfluences( pIB,
	                                                              pMeshContainer->m_pOriginMesh->GetNumFaces(),
	                                                              &NumMaxFaceInfl );
	        pIB->Release();
	        if( FAILED( hr ) )
	            goto e_Exit;

	        // 12 entry palette guarantees that any triangle (4 independent influences per vertex of a tri)
	        // can be handled
	        NumMaxFaceInfl = min( NumMaxFaceInfl, 12 );

	        if( d3dCaps.MaxVertexBlendMatrixIndex + 1 < NumMaxFaceInfl )
	        {
	            // HW does not support indexed vertex blending. Use SW instead
	            pMeshContainer->m_numberOfPaletteEntries = min( 256, pMeshContainer->pSkinInfo->GetNumBones() );
	            pMeshContainer->UseSoftwareVP = true;
	            g_bUseSoftwareVP = true;
	            Flags |= D3DXMESH_SYSTEMMEM;
	        }
	        else
	        {
	            // using hardware - determine palette size from caps and number of bones
	            // If normals are present in the vertex data that needs to be blended for lighting, then 
	            // the number of matrices is half the number specified by MaxVertexBlendMatrixIndex.
	            pMeshContainer->m_numberOfPaletteEntries = min( ( d3dCaps.MaxVertexBlendMatrixIndex + 1 ) / 2,
	                                                     pMeshContainer->pSkinInfo->GetNumBones() );
	            pMeshContainer->UseSoftwareVP = false;
	            Flags |= D3DXMESH_MANAGED;
	        }

	        hr = pMeshContainer->pSkinInfo->ConvertToIndexedBlendedMesh
	            (
	            pMeshContainer->m_pOriginMesh,
	            Flags,
	            pMeshContainer->m_numberOfPaletteEntries,
	            pMeshContainer->pAdjacency,
	            NULL, NULL, NULL,
	            &pMeshContainer->m_maxNumberOfFaceInfl,
	            &pMeshContainer->m_numberOfBoneCombinations,
	            &pMeshContainer->m_pBoneCombinationTable,
	            &pMeshContainer->MeshData.pMesh );
	        if( FAILED( hr ) )
	            goto e_Exit;
	    }
        // if vertex shader indexed skinning mode selected, use ConvertToIndexedsBlendedMesh to generate drawable mesh
	    else if( ( g_SkinningMethod == D3DINDEXEDVS ) || ( g_SkinningMethod == D3DINDEXEDHLSLVS ) )
	    {
	        // Get palette size
	        // First 9 constants are used for other data.  Each 4x3 matrix takes up 3 constants.
	        // (96 - 9) /3 i.e. Maximum constant count - used constants 
	        UINT MaxMatrices = 26;
	        pMeshContainer->m_numberOfPaletteEntries = min( MaxMatrices, pMeshContainer->pSkinInfo->GetNumBones() );

	        DWORD Flags = D3DXMESHOPT_VERTEXCACHE;
	        if( d3dCaps.VertexShaderVersion >= D3DVS_VERSION( 1, 1 ) )
	        {
	            pMeshContainer->UseSoftwareVP = false;
	            Flags |= D3DXMESH_MANAGED;
	        }
	        else
	        {
	            pMeshContainer->UseSoftwareVP = true;
	            g_bUseSoftwareVP = true;
	            Flags |= D3DXMESH_SYSTEMMEM;
	        }

	        SAFE_RELEASE( pMeshContainer->MeshData.pMesh );

	        hr = pMeshContainer->pSkinInfo->ConvertToIndexedBlendedMesh
	            (
	            pMeshContainer->m_pOriginMesh,
	            Flags,
	            pMeshContainer->m_numberOfPaletteEntries,
	            pMeshContainer->pAdjacency,
	            NULL, NULL, NULL,
	            &pMeshContainer->m_maxNumberOfFaceInfl,
	            &pMeshContainer->m_numberOfBoneCombinations,
	            &pMeshContainer->m_pBoneCombinationTable,
	            &pMeshContainer->MeshData.pMesh );
	        if( FAILED( hr ) )
	            goto e_Exit;


	        // FVF has to match our declarator. Vertex shaders are not as forgiving as FF pipeline
	        DWORD NewFVF = ( pMeshContainer->MeshData.pMesh->GetFVF() & D3DFVF_POSITION_MASK ) | D3DFVF_NORMAL |
	            D3DFVF_TEX1 | D3DFVF_LASTBETA_UBYTE4;
	        if( NewFVF != pMeshContainer->MeshData.pMesh->GetFVF() )
	        {
	            LPD3DXMESH pMesh;
	            hr = pMeshContainer->MeshData.pMesh->CloneMeshFVF( pMeshContainer->MeshData.pMesh->GetOptions(), NewFVF,
	                                                               pd3dDevice, &pMesh );
	            if( !FAILED( hr ) )
	            {
	                pMeshContainer->MeshData.pMesh->Release();
	                pMeshContainer->MeshData.pMesh = pMesh;
	                pMesh = NULL;
	            }
	        }

	        D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];
	        LPD3DVERTEXELEMENT9 pDeclCur;
	        hr = pMeshContainer->MeshData.pMesh->GetDeclaration( pDecl );
	        if( FAILED( hr ) )
	            goto e_Exit;

	        // the vertex shader is expecting to interpret the UBYTE4 as a D3DCOLOR, so update the type 
	        //   NOTE: this cannot be done with CloneMesh, that would convert the UBYTE4 data to float and then to D3DCOLOR
	        //          this is more of a "cast" operation
	        pDeclCur = pDecl;
	        while( pDeclCur->Stream != 0xff )
	        {
	            if( ( pDeclCur->Usage == D3DDECLUSAGE_BLENDINDICES ) && ( pDeclCur->UsageIndex == 0 ) )
	                pDeclCur->Type = D3DDECLTYPE_D3DCOLOR;
	            pDeclCur++;
	        }

	        hr = pMeshContainer->MeshData.pMesh->UpdateSemantics( pDecl );
	        if( FAILED( hr ) )
	            goto e_Exit;

	        // allocate a buffer for bone matrices, but only if another mesh has not allocated one of the same size or larger
	        if( g_NumBoneMatricesMax < pMeshContainer->pSkinInfo->GetNumBones() )
	        {
	            g_NumBoneMatricesMax = pMeshContainer->pSkinInfo->GetNumBones();

	            // Allocate space for blend matrices
	            delete[] g_pBoneMatrices;
	            g_pBoneMatrices = new D3DXMATRIXA16[g_NumBoneMatricesMax];
	            if( g_pBoneMatrices == NULL )
	            {
	                hr = E_OUTOFMEMORY;
	                goto e_Exit;
	            }
	        }

	    }
	    else  // invalid g_SkinningMethod value
	    {
	        // return failure due to invalid skinning method value
	        hr = E_INVALIDARG;
	        goto e_Exit;
	    }

	e_Exit:
	    return hr;
	}


};
