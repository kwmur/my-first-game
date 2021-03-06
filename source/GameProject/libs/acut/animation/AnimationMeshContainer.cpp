#include "../acut.h"



namespace acut {


	/**
	 * @brief constructor
	 *
	 */
	AnimationMeshContainer::AnimationMeshContainer() :
		m_pOriginMesh(NULL),
	    m_numberOfPaletteEntries(0),
		m_maxNumberOfFaceInfl(0),
		m_numberOfBoneCombinations(0),
		m_pBoneCombinationTable(NULL)
	{
		// 基底クラスのメンバ変数を初期化
		this->Name = NULL;
		ZeroMemory(&this->MeshData, sizeof(D3DXMESHDATA));
		this->pMaterials = NULL;
		this->pEffects = NULL;
		this->NumMaterials = 0;
		this->pAdjacency = NULL;	
		this->pSkinInfo = NULL;
		this->pNextMeshContainer = NULL;
	}


};


