#include "DataStructs.h"
namespace UI
{
	struct UIelements {

		bool  rotate		 = false;
		float cameraDistance = 5.0f;
		MeshType currentMesh;
		float rot_x = 0.0, rot_y = 0.0;
		int  mip0 = 32, mip1 = 32, mip2 = 32,
			mip3 = 32, mip4 = 32, mip5 = 32,
			mip6 = 32, mip7 = 32, mip8 = 32,
			mip9 = 32, mip10 = 32, mip11 = 32;
	 };


	class TweakBar
	{
	public:
		~TweakBar();
		static UI::TweakBar* GetInstance();
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
			UI::TweakBar* tweakBar = TweakBar::GetInstance();
			tweakBar->m_Uidata.rotate = *static_cast<const bool *>(value);
			
		}
		static void TW_CALL GetRotation(void *value, void * /*clientData*/)
		{
			UI::TweakBar* tweakBar = TweakBar::GetInstance();
			*static_cast<bool *>(value) = tweakBar->m_Uidata.rotate;
		}

	};

}
