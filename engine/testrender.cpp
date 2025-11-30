#include "engine/structs.h"
#include "engine/testrender.h"
#include "engine/engine2.h"
#include "engine/engine2vars.h"
#include "engine/handle.h"
#include "engine/stringtoken.h"
#include "stdio.h"
#include "math.h"


class CTestRendering: public ITestRendering
{
public:
	int hWorld;
	ISceneWorld *pSceneWorld;
	CCameraRenderer *pCameraRenderer;
	CRenderAttributes *pRenderAttributes;
	HModelStrong *pModel;

	CREATE_NATIVE(IRenderContext, pRenderContext);

	virtual void Init() override;
	virtual void Frame(SwapChainHandle_t hSwapChain) override;
};

void CTestRendering::Init()
{
	
	pCameraRenderer = new CCameraRenderer("RenderToSwapChain",1);
	pRenderAttributes = new CRenderAttributes;


	pSceneWorld = (ISceneWorld*)AcquireNextHandle(g_pSceneSystem->CreateWorld("testworld"));
	pModel =Glue::Resources::GetModel("models/dev/error.vmdl");

	CTransformUnaligned transform;
	transform.m_scale = {
		1,
		1,
		1,
	};
	transform.m_position = {
		0,0,0
	};
	transform.m_rotation = {
		0,0,0,0
	};
	CREATE_NATIVE(CModel, model);
	model->m_pSelf = pModel;
	printf("%s\n",model->GetModelName());
	printf("a %p\n",*(const void**)model->GetBindingPtr());
	printf("%p\n", model->m_pSelf);
	printf("%i %i %i %i\n",model->IsStrongHandleLoaded(),model->GetNumMeshes(), model->IsStrongHandleValid(), model->HasSceneObjects());

	int h = g_pMeshSystem->CreateSceneObject((HModelStrong*)pModel, transform, NULL, uint64_t(SceneObjectFlags::CastShadows) | uint64_t(SceneObjectFlags::IsLoaded), 0, GN(pSceneWorld), 1);
	printf("%i\n", h);
}

void CTestRendering::Frame(SwapChainHandle_t hSwapChain)
{
	pRenderAttributes->Clear(0, 0);
	pRenderAttributes->SetBoolValue(StringToken("renderOpaque"), true);
	pRenderAttributes->SetBoolValue(StringToken("renderTranslucent"), true);
	pRenderAttributes->SetBoolValue(StringToken("directLighting"), true);
	pRenderAttributes->SetBoolValue(StringToken("indirectLighting"), true);
	pRenderAttributes->SetBoolValue(StringToken("renderSun"), true);
	pRenderAttributes->SetBoolValue(StringToken("drawShadows"), true);
	pRenderAttributes->SetVector4DValue(StringToken("ambientColor"), (Vector4D){0.7,0.7,0.7,1});
	pRenderAttributes->SetVector4DValue(StringToken("clearColor"), (Vector4D){1,1,1,1});
	pRenderAttributes->SetIntValue(StringToken("clearFlags"), 0x3FF);
	
	pCameraRenderer->ClearSceneWorlds();
	pCameraRenderer->ClearExcludeTags();
	pCameraRenderer->ClearRenderTags();
	pCameraRenderer->AddSceneWorld(GN(pSceneWorld));
	pCameraRenderer->SetRenderAttributes(GN(pRenderAttributes));


	g_pRenderDevice->Present(hSwapChain);
}
static CTestRendering s_testRendering;
ITestRendering *pTestRendering = &s_testRendering;
