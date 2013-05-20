#include "../acut.h"



namespace acut {


	/**
	 * @brief constructor
	 *
	 */
	AnimationFrame::AnimationFrame()
	{
		// 基底クラスのメンバ変数を初期化
		this->Name = NULL;
		D3DXMatrixIdentity(&this->TransformationMatrix);
		this->pMeshContainer = NULL;
		this->pFrameSibling = NULL;
		this->pFrameFirstChild = NULL;
		// AnimationFrameメンバ変数を初期化
		D3DXMatrixIdentity(&this->WorldTransformationMatrix);
		D3DXMatrixIdentity(&this->CombinedTransformationMatrix);
	}


};