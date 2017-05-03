
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


	//TwAddVarRW(m_barHandle, "Mip 0", TW_TYPE_INT8, &m_Uidata.mip0, "group = MIPprecision step = 1 min = 0 max = 32");
	//TwAddVarRW(m_barHandle, "Mip 1", TW_TYPE_INT8, &m_Uidata.mip1, "group = MIPprecision step = 1 min = 0 max = 32");
	//TwAddVarRW(m_barHandle, "Mip 2", TW_TYPE_INT8, &m_Uidata.mip2, "group = MIPprecision step = 1 min = 0 max = 32");
	//TwAddVarRW(m_barHandle, "Mip 3", TW_TYPE_INT8, &m_Uidata.mip3, "group = MIPprecision step = 1 min = 0 max = 32");
	//TwAddVarRW(m_barHandle, "Mip 4", TW_TYPE_INT8, &m_Uidata.mip4, "group = MIPprecision step = 1 min = 0 max = 32");
	//TwAddVarRW(m_barHandle, "Mip 5", TW_TYPE_INT8, &m_Uidata.mip5, "group = MIPprecision step = 1 min = 0 max = 32");
	//TwAddVarRW(m_barHandle, "Mip 6", TW_TYPE_INT8, &m_Uidata.mip6, "group = MIPprecision step = 1 min = 0 max = 32");
	//TwAddVarRW(m_barHandle, "Mip 7", TW_TYPE_INT8, &m_Uidata.mip7, "group = MIPprecision step = 1 min = 0 max = 32");
	//TwAddVarRW(m_barHandle, "Mip 8", TW_TYPE_INT8, &m_Uidata.mip8, "group = MIPprecision step = 1 min = 0 max = 32");
	//TwAddVarRW(m_barHandle, "Mip 9", TW_TYPE_INT8, &m_Uidata.mip9, "group = MIPprecision step = 1 min = 0 max = 32");
	//TwAddVarRW(m_barHandle, "Mip 10", TW_TYPE_INT8, &m_Uidata.mip10, "group = MIPprecision step = 1 min = 0 max = 32");
	//TwAddVarRW(m_barHandle, "Mip 11", TW_TYPE_INT8, &m_Uidata.mip11, "group = MIPprecision step = 1 min = 0 max = 32");



	//TwAddButton(m_barHandle, "Bounce", BounceCall, NULL, "group = Rotation");
	//TwAddVarRW(m_barHandle, "Rotation", TW_TYPE_FLOAT, &animSpeed, "step = 0.001 group = Rotation");
	//TwAddVarRW(myBar, "Rotation Speed", TW_TYPE_FLOAT, &speed, "group = Rotation step = 0.000001 min =0");
	//TwAddVarRW(myBar, "Background Color", TW_TYPE_COLOR4F, &clearColor, "");
	//TwAddVarRW(myBar, "Light Position", TW_TYPE_DIR3F, &lightPos, "group = Light");
	//TwAddVarRW(myBar, "Light Color", TW_TYPE_COLOR4F, &lightColor, "group = Light");
	//TwAddVarRW(myBar, "Light Intensity", TW_TYPE_FLOAT, &lightInt, "group = Light min= 0.0 max = 1.0 step = 0.05");

	TwEnumVal shapeEV[NUM_MESH_TYPES] = { { SPHERE, "Sphere" },{ CUBE, "Cube" },{ COMPLEX, "Bunny" } };
	// Create a type for the enum shapeEV
	TwType shapeType = TwDefineEnum("ShapeType", shapeEV, NUM_MESH_TYPES);
	// add 'g_CurrentShape' to 'bar': this is a variable of type ShapeType. Its key shortcuts are [<] and [>].
	TwAddVarRW(m_barHandle, "Shape", shapeType, &m_Uidata.currentMesh, "group = Mesh keyIncr='<' keyDecr='>' help='Change object shape.' ");
	TwAddVarCB(m_barHandle, "Rotation", TW_TYPE_BOOLCPP, SetRotation, GetRotation, NULL, "group=Mesh key=r");
	TwAddVarRW(m_barHandle, "Camera Distance", TW_TYPE_FLOAT, &m_Uidata.cameraDistance, "group = Camera step = 0.5 min = 0.5");

	TwAddVarRW(m_barHandle, "Rot x", TW_TYPE_FLOAT, &m_Uidata.rot_x, "group = Mesh step = 1.0 min = 0.0 max = 360.0");
	TwAddVarRW(m_barHandle, "Rot y", TW_TYPE_FLOAT, &m_Uidata.rot_y, "group = Mesh step = 1.0 min = 0.0 max = 360.0");

	return result;
}


void TweakBar::Render()
{
	TwDraw();
}


