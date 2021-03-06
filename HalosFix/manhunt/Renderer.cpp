#include "Renderer.h"
#include "core.h"

void CRenderer::PushRenderStateBlend()
{
	Call<0x5F5CF0>();
}


void CRenderer::PushAndSetRenderState(RwRenderState state, void* value)
{
	Call<0x5F5E60, RwRenderState, void*>(state, value);
}

void CRenderer::RenderStateSetBlend(RwBlendFunction state, RwBlendFunction value)
{
	Call<0x5F5C90, RwBlendFunction, RwBlendFunction>(state, value);
}

void CRenderer::PopRenderStateAll()
{
	Call<0x5F5E10>();
}

void CRenderer::PopRenderStateBlend()
{
	Call<0x5F5D30>();
}

void CRenderer::DrawQuad2d(float posX, float posY, float scaleX, float scaleY, int red, int green, int blue, int alpha, int pTexture)
{
	Call<0x5F96F0, float, float, float, float, int, int, int, int, int>(posX, posY, scaleX, scaleY, red, green, blue, alpha, pTexture);
}
