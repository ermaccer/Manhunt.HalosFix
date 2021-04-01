// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "MemoryMgr.h"
#include "manhunt/Frontend.h"
#include "manhunt/Renderer.h"
#include "manhunt/Vector.h"
#include "manhunt/core.h"

using namespace Memory::VP;



int pFixedHaloTexture;

struct CStoredHalo {
	CVector position;
	float scale;
	int r;
	int g;
	int b;
	float radius;
};



void HookTexturesLoad()
{
	pFixedHaloTexture = 0;
	int pTextureDictionary = CallAndReturn<int, 0x5EA510, const char*>("pictures/halofix.txd");

	if (!pTextureDictionary)
	MessageBoxA(0, "Failed to load halofix.txd!", 0, MB_ICONERROR);

	if (pTextureDictionary)
	{
		pFixedHaloTexture = CFrontend::GetTextureFromTXD(pTextureDictionary, "halo");

		if (!pFixedHaloTexture)
			pFixedHaloTexture = 0;

	}

	Call<0x5A95B0>();
}

void DrawHalos()
{
	int texture = 0;
	if (pFixedHaloTexture)
		texture = pFixedHaloTexture;
	else
		texture = *(int*)(*(int*)0x7D37CC);


	CRenderer::PushRenderStateBlend();
	CRenderer::RenderStateSetBlend(rwBLENDONE, rwBLENDONE);
	CRenderer::PushAndSetRenderState(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
	CRenderer::PushAndSetRenderState(rwRENDERSTATESTENCILZFAIL, FALSE);
	CRenderer::PushAndSetRenderState(rwRENDERSTATEZWRITEENABLE, FALSE);
	CRenderer::PushAndSetRenderState(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
	CRenderer::PushAndSetRenderState(rwRENDERSTATETEXTUREADDRESS, (void*)rwTEXTUREADDRESSWRAP);
	CRenderer::PushAndSetRenderState(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERMIPLINEAR);
	CRenderer::RenderStateSetBlend(rwBLENDSRCALPHA, rwBLENDONE);
	CRenderer::PushAndSetRenderState(rwRENDERSTATETEXTURERASTER, (void*)texture);


	for (int i = 0; i < CFrontend::NumStoredHalos; i++)
	{
		CStoredHalo halo = *(CStoredHalo*)(0x7D5A30 + (sizeof(CStoredHalo) * i));

		CFrontend::DrawDisc2D(&halo.position, halo.scale, halo.r, halo.g, halo.b, halo.radius);
		CFrontend::DrawDisc3D(&halo.position, halo.scale, halo.r, halo.g, halo.b, halo.radius);
	}

	CRenderer::PopRenderStateBlend();
	CRenderer::PopRenderStateAll();

	CFrontend::NumStoredHalos = 0;
}



void Init()
{

	// load resource
	InjectHook(0x5EE349, HookTexturesLoad, PATCH_CALL);
	// disable raster call in halo2d
	Nop(0x5FB0A2, 3);
	// replace render function
	InjectHook(0x47610E, DrawHalos, PATCH_CALL);
}

extern "C"
{
	__declspec(dllexport) void InitializeASI()
	{
		if (*(int*)0x63BC93 != 0x24448B66)
		{
			MessageBoxA(0, "Invalid executable!", "HalosFix", 0);
		}
		else Init();
	}
}