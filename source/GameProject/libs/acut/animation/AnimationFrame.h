#ifndef ACUT_ANIMATION_FRAME_H__
#define ACUT_ANIMATION_FRAME_H__



namespace acut {


	/**
	 * @brief アニメーションフレームクラス
	 *
	 */
	struct AnimationFrame : public D3DXFRAME {
	public:
		AnimationFrame(); // constructor

		D3DXMATRIX WorldTransformationMatrix; //!< フレーム自身のワールド変換行列
		D3DXMATRIX CombinedTransformationMatrix; //!< 合成行列
	};


};



#endif // #ifndef ACUT_ANIMATION_FRAME_H__
