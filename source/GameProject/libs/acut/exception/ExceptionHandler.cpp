#include "../acut.h"



namespace acut {


	/**
	 * @brief ��O�̋��ʏ���static�����o�֐�
	 *
	 */
	void ExceptionHandler::handleException(const std::exception& ex)
	{
		DEBUG_OUTPUTF("%s : %s \n", __FUNCTIONW__, ex.what());

		// TODO: ���O�o�͂̌���

		// TODO: �A�v���P�[�V������O�Ń��b�v����throw���邩����
		//throw acut::ApplicationException(ex);
	}


	/**
	 * @brief ��O�̋��ʏ���static�����o�֐�
	 *
	 */
	void ExceptionHandler::handleException(const acut::Exception& ex)
	{
		DEBUG_OUTPUTF("ex.what() : %s\n", ex.what().c_str());
		

		// TODO: ���O�o�͂̌���

		// TODO: �A�v���P�[�V������O�Ń��b�v����throw���邩����
		//throw acut::ApplicationException(ex);
	}


};
