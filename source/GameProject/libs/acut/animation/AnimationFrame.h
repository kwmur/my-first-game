#ifndef ACUT_ANIMATION_FRAME_H__
#define ACUT_ANIMATION_FRAME_H__



namespace acut {


	/**
	 * @brief �A�j���[�V�����t���[���N���X
	 *
	 */
	struct AnimationFrame : public D3DXFRAME {
	public:
		AnimationFrame(); // constructor

		D3DXMATRIX WorldTransformationMatrix; //!< �t���[�����g�̃��[���h�ϊ��s��
		D3DXMATRIX CombinedTransformationMatrix; //!< �����s��
	};


};



#endif // #ifndef ACUT_ANIMATION_FRAME_H__
