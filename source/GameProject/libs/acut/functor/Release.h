#ifndef ACUT_RELEASE_H__
#define ACUT_RELEASE_H__



namespace acut {


	/**
	 * @brief SEFE_RELFEASE�p�N���X
	 *
	 */
	template <typename Type>
	struct Release {
		void operator()(Type& pointer) { SAFE_RELEASE(pointer); }
	};


};



#endif // #ifndef ACUT_RELEASE_H__
