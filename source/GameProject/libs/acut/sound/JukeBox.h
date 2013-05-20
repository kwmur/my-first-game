#ifndef ACUT_JUKEBOX_H__
#define ACUT_JUKEBOX_H__



namespace acut {

	class JukeBox {
	public:
		JukeBox();
		~JukeBox();
	private:
		// コピーは未対応
		JukeBox(const JukeBox& rhs);
		JukeBox& operator=(const JukeBox& rhs);
	};

};



#endif // #ifndef ACUT_JUKEBOX_H__
