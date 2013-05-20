#ifndef ACUT_EXCEPTION_HANDLER_H__
#define ACUT_EXCEPTION_HANDLER_H__



namespace acut {

	/**
	 * @brief ��O�̋��ʏ������s���N���X
	 *
	 */
	class ExceptionHandler {    
	public:
		static void handleException(const std::exception& ex);
		static void handleException(const acut::Exception& ex);
	private:
		ExceptionHandler(); // �C���X�^���X�����Ή�
	};

};



#endif // #ifndef ACUT_EXCEPTION_HANDLER_H__
