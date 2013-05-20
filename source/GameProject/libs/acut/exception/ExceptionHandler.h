#ifndef ACUT_EXCEPTION_HANDLER_H__
#define ACUT_EXCEPTION_HANDLER_H__



namespace acut {

	/**
	 * @brief 例外の共通処理を行うクラス
	 *
	 */
	class ExceptionHandler {    
	public:
		static void handleException(const std::exception& ex);
		static void handleException(const acut::Exception& ex);
	private:
		ExceptionHandler(); // インスタンス化未対応
	};

};



#endif // #ifndef ACUT_EXCEPTION_HANDLER_H__
