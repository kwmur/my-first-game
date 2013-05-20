#ifndef ACUT_ALLOCATE_HIERARCHY_H__
#define ACUT_ALLOCATE_HIERARCHY_H__



namespace acut {


	/**
	 * @brief
	 * Inheriting from ID3DXAllocateHierarchy, this class handles the
	 * allocation and release of the memory used by animation frames and
	 * meshes.  Applications derive their own version of this class so
	 * that they can customize the behavior of allocation and release.
	 *
	 */
	class AllocateHierarchy : public ID3DXAllocateHierarchy {
	public:
		// callback to create a D3DXFRAME-derived object and initialize it
		STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME* ppNewFrame);

		// callback to create a D3DXMESHCONTAINER-derived object and initialize it
		STDMETHOD(CreateMeshContainer)(THIS_
			LPCSTR Name,
			CONST D3DXMESHDATA* pMeshData,
			CONST D3DXMATERIAL* pMaterials,
			CONST D3DXEFFECTINSTANCE* pEffectInstances,
			DWORD NumMaterials,
			CONST DWORD* pAdjacency,
			LPD3DXSKININFO pSkinInfo,
			LPD3DXMESHCONTAINER* ppNewMeshContainer
		);

		// callback to release a D3DXFRAME-derived object
		STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);

		// callback to release a D3DXMESHCONTAINER-derived object
		STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree);

	private:
		// skinned mesh Çê∂ê¨ÅAê›íËÇ∑ÇÈ
		static HRESULT generateSkinnedMesh(acut::AnimationMeshContainer* pMeshContainer);
	};


};



#endif // #ifndef ACUT_ALLOCATE_HIERARCHY_H__
