#include "../acut.h"



namespace acut {


	/**
	 * @brief 例外の共通処理staticメンバ関数
	 *
	 */
	void ExceptionHandler::handleException(const std::exception& ex)
	{
		DEBUG_OUTPUTF("%s : %s \n", __FUNCTIONW__, ex.what());

		// TODO: ログ出力の検討

		// TODO: アプリケーション例外でラップしてthrowするか検討
		//throw acut::ApplicationException(ex);
	}


	/**
	 * @brief 例外の共通処理staticメンバ関数
	 *
	 */
	void ExceptionHandler::handleException(const acut::Exception& ex)
	{
		DEBUG_OUTPUTF("ex.what() : %s\n", ex.what().c_str());
		

		// TODO: ログ出力の検討

		// TODO: アプリケーション例外でラップしてthrowするか検討
		//throw acut::ApplicationException(ex);
	}


};
