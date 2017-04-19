#include "pch.h"

namespace UI
{
	struct UIelements {

		bool  rotate		 = false;
		float cameraDistance = 5.0f;
	 };


	class TweakBar
	{
	public:
		~TweakBar();
		static std::shared_ptr<UI::TweakBar> GetInstance();
		int Initialize(ID3D11Device* gDevice, float windowWidth, float windowHeight);
		void Render();
		UIelements* GetUiData() { return &m_Uidata; };
	private:
		UIelements m_Uidata;
		TweakBar();
		TwBar *m_barHandle;
	public: // callback functions
		static void TW_CALL RotationCall(void* data);
	};

}
