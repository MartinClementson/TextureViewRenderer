#include "DataStructs.h"
namespace UI
{
	struct UIelements {

		bool  rotate		 = false;
		float cameraDistance = 5.0f;
		MeshType currentMesh;
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
		
		static void TW_CALL SetRotation(const void *value, void * /*clientData*/)
		{
			std::shared_ptr<UI::TweakBar> tweakBar = TweakBar::GetInstance();
			tweakBar->m_Uidata.rotate = *static_cast<const bool *>(value);
			
		}
		static void TW_CALL GetRotation(void *value, void * /*clientData*/)
		{
			std::shared_ptr<UI::TweakBar> tweakBar = TweakBar::GetInstance();
			*static_cast<bool *>(value) = tweakBar->m_Uidata.rotate;
		}

	};

}
