#include "../acut.h"



namespace acut {


	/**
	 * @brief constructor
	 *
	 */
	AnimationFrame::AnimationFrame()
	{
		// ���N���X�̃����o�ϐ���������
		this->Name = NULL;
		D3DXMatrixIdentity(&this->TransformationMatrix);
		this->pMeshContainer = NULL;
		this->pFrameSibling = NULL;
		this->pFrameFirstChild = NULL;
		// AnimationFrame�����o�ϐ���������
		D3DXMatrixIdentity(&this->WorldTransformationMatrix);
		D3DXMatrixIdentity(&this->CombinedTransformationMatrix);
	}


};