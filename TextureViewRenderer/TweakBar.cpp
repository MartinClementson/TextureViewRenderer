
#include "pch.h"
#include "TweakBar.h"
using namespace UI;

TweakBar::TweakBar()
{
}
TweakBar::~TweakBar()
{
	TwTerminate();
}

UI::TweakBar* UI::TweakBar::GetInstance()
{
	static TweakBar instance;
	return &instance;
}

int UI::TweakBar::Initialize(ID3D11Device * gDevice, float windowWidth, float windowHeight)
{
	int result = 0;
	result= TwInit(TwGraphAPI::TW_DIRECT3D11, gDevice);
	result= TwWindowSize(windowWidth, windowHeight);
	m_barHandle = TwNewBar("Tweak Ui");

	//TwAddButton(m_barHandle, "Rotation On/Off", TweakBar::RotationCall, NULL, "group = Mesh");
	TwAddVarCB(m_barHandle, "Rotation", TW_TYPE_BOOLCPP, SetRotation, GetRotation, NULL, "group=Mesh key=r");
	TwAddVarRW(m_barHandle, "Camera Distance", TW_TYPE_FLOAT, &m_Uidata.cameraDistance, "group = Camera step = 1.0 min =2.0");
	//TwAddButton(m_barHandle, "Bounce", BounceCall, NULL, "group = Rotation");
	//TwAddVarRW(m_barHandle, "Rotation", TW_TYPE_FLOAT, &animSpeed, "step = 0.001 group = Rotation");
	//TwAddVarRW(myBar, "Rotation Speed", TW_TYPE_FLOAT, &speed, "group = Rotation step = 0.000001 min =0");
	//TwAddVarRW(myBar, "Background Color", TW_TYPE_COLOR4F, &clearColor, "");
	//TwAddVarRW(myBar, "Light Position", TW_TYPE_DIR3F, &lightPos, "group = Light");
	//TwAddVarRW(myBar, "Light Color", TW_TYPE_COLOR4F, &lightColor, "group = Light");
	//TwAddVarRW(myBar, "Light Intensity", TW_TYPE_FLOAT, &lightInt, "group = Light min= 0.0 max = 1.0 step = 0.05");

	TwEnumVal shapeEV[NUM_MESH_TYPES] = { { PLANE, "Plane" },{ CUBE, "Cube" },{ COMPLEX, "JOHNS_ANSIKTE" } };
	// Create a type for the enum shapeEV
	TwType shapeType = TwDefineEnum("ShapeType", shapeEV, NUM_MESH_TYPES);
	// add 'g_CurrentShape' to 'bar': this is a variable of type ShapeType. Its key shortcuts are [<] and [>].
	TwAddVarRW(m_barHandle, "Shape", shapeType, &m_Uidata.currentMesh, "group = Mesh keyIncr='<' keyDecr='>' help='Change object shape.' ");
	return result;
}


void TweakBar::Render()
{
	TwDraw();
}


