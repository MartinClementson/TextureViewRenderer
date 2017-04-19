
#include "pch.h"
#include "TweakBar.h"
using namespace UI;
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//
//	if (TwEventWin(hWnd, message, wParam, lParam)) // send event message to AntTweakBar
//		return 0; // event has been handled by AntTweakBar
//	return 0;
//}
TweakBar::TweakBar()
{
}

void TW_CALL TweakBar::RotationCall(void * data)
{
	std::shared_ptr<UI::TweakBar> tweakBar = TweakBar::GetInstance();
	if (tweakBar->m_Uidata.rotate)
	{
		tweakBar->m_Uidata.rotate = false;
	}
	else {
		tweakBar->m_Uidata.rotate = true;
	}
}

TweakBar::~TweakBar()
{
	TwTerminate();
}

std::shared_ptr<UI::TweakBar> UI::TweakBar::GetInstance()
{
	static TweakBar* instance =  new TweakBar();
	static std::shared_ptr<UI::TweakBar> ptr = std::shared_ptr<UI::TweakBar>(instance);
	return ptr;
}

int UI::TweakBar::Initialize(ID3D11Device * gDevice, float windowWidth, float windowHeight)
{
	int result = 0;
	result= TwInit(TwGraphAPI::TW_DIRECT3D11, gDevice);
	result= TwWindowSize(windowWidth, windowHeight);
	m_barHandle = TwNewBar("Tweak Ui");

	TwAddButton(m_barHandle, "Rotation On/Off", TweakBar::RotationCall, NULL, "group = Mesh");
	TwAddVarRW(m_barHandle, "Camera Distance", TW_TYPE_FLOAT, &m_Uidata.cameraDistance, "group = Camera step = 1.0 min =5.0");
	//TwAddButton(m_barHandle, "Bounce", BounceCall, NULL, "group = Rotation");
	//TwAddVarRW(m_barHandle, "Rotation", TW_TYPE_FLOAT, &animSpeed, "step = 0.001 group = Rotation");
	//TwAddVarRW(myBar, "Rotation Speed", TW_TYPE_FLOAT, &speed, "group = Rotation step = 0.000001 min =0");
	//TwAddVarRW(myBar, "Background Color", TW_TYPE_COLOR4F, &clearColor, "");
	//TwAddVarRW(myBar, "Light Position", TW_TYPE_DIR3F, &lightPos, "group = Light");
	//TwAddVarRW(myBar, "Light Color", TW_TYPE_COLOR4F, &lightColor, "group = Light");
	//TwAddVarRW(myBar, "Light Intensity", TW_TYPE_FLOAT, &lightInt, "group = Light min= 0.0 max = 1.0 step = 0.05");

//TwEnumVal shapeEV[NUM_SHAPES] = { { SHAPE_TEAPOT, "Teapot" },{ SHAPE_TORUS, "Torus" },{ SHAPE_CONE, "Cone" } };
//// Create a type for the enum shapeEV
//TwType shapeType = TwDefineEnum("ShapeType", shapeEV, NUM_SHAPES);
//// add 'g_CurrentShape' to 'bar': this is a variable of type ShapeType. Its key shortcuts are [<] and [>].
//TwAddVarRW(bar, "Shape", shapeType, &g_CurrentShape, " keyIncr='<' keyDecr='>' help='Change object shape.' ");
	return result;
}


void TweakBar::Render()
{
	TwDraw();
}
