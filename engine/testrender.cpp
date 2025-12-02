#include "engine/structs.h"
#include "engine/testrender.h"
#include "engine/engine2.h"
#include "engine/engine2vars.h"
#include "engine/handle.h"
#include "engine/stringtoken.h"
#include "stdio.h"
#include "math.h"
#include <wingdi.h>


class CTestRendering: public ITestRendering
{
public:
	int hWorld;
	ISceneWorld *pSceneWorld;
	CCameraRenderer *pCameraRenderer;
	CRenderAttributes *pRenderAttributes;
	HModelStrong *pModel;

	CREATE_NATIVE(IRenderContext, pRenderContext);
	CREATE_NATIVE(CSceneObject, pSceneObject);

	virtual void Init() override;
	virtual void Frame(SwapChainHandle_t hSwapChain) override;
};

void CTestRendering::Init()
{
	SET_NATIVE(pRenderContext, g_pRenderDevice->CreateRenderContext(0));

	pCameraRenderer = new CCameraRenderer("RenderToSwapChain",1);
	printf("pCameraRenderer %p\n",pCameraRenderer->m_pSelf);
	pRenderAttributes = new CRenderAttributes;


	pSceneWorld = (ISceneWorld*)AcquireNextHandle(g_pSceneSystem->CreateWorld("testworld"));
	printf("pSceneWorld %p\n",pSceneWorld->m_pSelf);
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

	SET_NATIVE(pSceneObject, AcquireNextHandle(g_pMeshSystem->CreateSceneObject((HModelStrong*)pModel, transform, NULL, uint64_t(SceneObjectFlags::CastShadows) | uint64_t(SceneObjectFlags::IsLoaded), 0, GN(pSceneWorld), 1)));
	printf("pCameraRenderer %p\n",pCameraRenderer->m_pSelf);
	printf("pSceneWorld2 %p\n",pSceneWorld->m_pSelf);
}

void CTestRendering::Frame(SwapChainHandle_t hSwapChain)
{
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
	pCameraRenderer->SetRenderAttributes(GN(pRenderAttributes));
	pCameraRenderer->ClearExcludeTags();
	pCameraRenderer->ClearRenderTags();

	pCameraRenderer->ViewUniqueId = 100;
	pCameraRenderer->CameraPosition = (Vector){-10,0,0};
	pCameraRenderer->CameraRotation = (QAngle){0,0,0};
	pCameraRenderer->ZNear = 0.01;
	pCameraRenderer->ZFar = 100;
	pCameraRenderer->FieldOfView = 90;
	pCameraRenderer->Rect = (Rect_t){0,0,1,1};
	pCameraRenderer->Viewport = (Vector4D){0,0,1,1};
	pCameraRenderer->Ortho = false;
	pCameraRenderer->ClipSpaceBounds = (Vector4D){-1, -1, 1, 1};
	pCameraRenderer->EnableEngineOverlays = false;
	pCameraRenderer->EnablePostprocessing = false;
	pCameraRenderer->FlipX = false;
	pCameraRenderer->FlipY = false;

	pCameraRenderer->AddSceneWorld(GN(pSceneWorld));
	pCameraRenderer->Render(hSwapChain);

	printf("Hi\n");
}
static CTestRendering s_testRendering;
ITestRendering *pTestRendering = &s_testRendering;
