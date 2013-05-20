#ifndef ACUT_MEMORY_UTILITY_H__
#define ACUT_MEMORY_UTILITY_H__



namespace acut {

	/**
	 * @brief memory関係の処理をまとめたファンクタクラス
	 *
	 * memoryの管理は全く行わない
	 *
	 */
	class MemoryUtility {
	public:
		static char* copyStringToHeap(const char* const pSourceName);
	private:
		MemoryUtility(); // インスタンス化未対応
	};

};



#endif // #ifndef ACUT_MEMORY_UTILITY_H__
