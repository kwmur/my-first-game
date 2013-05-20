#include "../acut.h"



namespace acut {


	/**
	 * @brief constructor
	 *
	 */
	Exception::Exception() :
		m_file(_T("")),
		m_line(0),
		m_function(_T(""))
	{
	}


	/**
	 * @brief constructor
	 *
	 */
	Exception::Exception(const acut::TString file, const int line, const acut::TString function) :
		m_file(file),
		m_line(line),
		m_function(function)
	{
	}


	/**
	 * @brief destructor
	 *
	 */
	Exception::~Exception()
	{
	}


	/**
	 * @brief 例外発生場所を文字列にして返す
	 *
	 */
	acut::TString Exception::what() const
	{
		// __FILE__ はフルパスなので、ファイル名のみ取り出す
		TCHAR drive[MAX_PATH];
		TCHAR directory[MAX_PATH];
		TCHAR fileName[MAX_PATH];
		TCHAR extension[MAX_PATH];
		_tsplitpath_s(m_file.c_str(), drive, directory, fileName, extension);

		acut::tstringstream ss;
		ss << _T("FILE = ") << fileName << extension
			<< _T(", LINE = ") << m_line
			<< _T(", FUNCTION = ") << m_function
			<< std::endl;

		return ss.str();
	}


};
