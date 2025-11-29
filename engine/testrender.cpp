#include "engine/testrender.h"
#include "engine/engine2.h"
#include "engine/structs.h"
#include "stdio.h"

class g_pRenderDevice *g_pRenderDevice = new class g_pRenderDevice;

class CTestRendering: public ITestRendering
{
public:
	IRenderContext *pRenderContext;

	virtual void Init() override;
	virtual void Frame(SwapChainHandle_t hSwapChain) override;
};

void CTestRendering::Init()
{
	pRenderContext = new IRenderContext();
	pRenderContext->m_pSelf = (void*)g_pRenderDevice->CreateRenderContext(0);
	RenderViewport_t renderViewport = pRenderContext->GetViewport();
}

void CTestRendering::Frame(SwapChainHandle_t hSwapChain)
{
	pRenderContext->BindRenderTargets(hSwapChain, 1, 0);
	pRenderContext->Clear((Vector4D){1,0.5,0,1},1,0,0);
	pRenderContext->Submit();
	g_pRenderDevice->Present(hSwapChain);
}
static CTestRendering s_testRendering;
ITestRendering *pTestRendering = &s_testRendering;
