#ifndef ACUT_MEMORY_UTILITY_H__
#define ACUT_MEMORY_UTILITY_H__



namespace acut {

	/**
	 * @brief memory�֌W�̏������܂Ƃ߂��t�@���N�^�N���X
	 *
	 * memory�̊Ǘ��͑S���s��Ȃ�
	 *
	 */
	class MemoryUtility {
	public:
		static char* copyStringToHeap(const char* const pSourceName);
	private:
		MemoryUtility(); // �C���X�^���X�����Ή�
	};

};



#endif // #ifndef ACUT_MEMORY_UTILITY_H__
