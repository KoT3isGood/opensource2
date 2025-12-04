#include "engine/structs.h"
#include "engine/testrender.h"
#include "engine/engine2.h"
#include "engine/engine2vars.h"
#include "engine/handle.h"
#include "engine/stringtoken.h"
#include "engine/time.h"
#include "stdio.h"
#include "math.h"

#include "cglm/cglm.h"
#include "cglm/quat.h"
#include "cglm/mat4.h"
#include <cstdlib>

class CTestRendering: public ITestRendering
{
public:
	int hWorld;
	ISceneWorld *pSceneWorld;
	CCameraRenderer *pCameraRenderer;
	CRenderAttributes *pRenderAttributes;
	HModelStrong *pModel;
	HMaterialStrong *pMaterial;

	CREATE_NATIVE(IRenderContext, pRenderContext);
	CREATE_NATIVE(CSceneObject, pSceneObject);
	CREATE_NATIVE(IWorldReference, pWorldRef);
	CREATE_NATIVE(IPVS, pPVS);

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

	CTransformUnaligned transform;
	transform.m_scale = {
		1,
		1,
		1,
	};
	transform.m_position = {
		0,0,0
	};
	mat4 m;
	glm_mat4_identity(m);
	glm_mat4_quat(m, *(versor*)&transform.m_rotation);

	printf("%f %f %f %f\n", transform.m_rotation.x, transform.m_rotation.y, transform.m_rotation.z, transform.m_rotation.w);




	SET_NATIVE(pWorldRef,
		g_pWorldRendererMgr->CreateWorld(
			"maps/dev/preview_flat.vpk",
			GN(pSceneWorld),
			false,
			true,
			false,
			false,
			StringToken(pSceneWorld->GetWorldDebugName()),
			transform
			)
	);
	pWorldRef->PrecacheAllWorldNodes(0x0080);

	SET_NATIVE(pPVS,g_pEnginePVSManager->BuildPvs(GN(pWorldRef)));
	pSceneWorld->SetPVS(GN(pPVS));
	
	pModel = Glue::Resources::GetModel("models/dev/error.vmdl");
	CREATE_NATIVE(CModel, model);
	model->m_pSelf = pModel;

	pSceneObject = (CSceneObject*)AcquireNextHandle(g_pMeshSystem->CreateSceneObject(pModel, transform, NULL, 
					uint64_t(SceneObjectFlags::CastShadows) | 
					uint64_t(SceneObjectFlags::IsLoaded),
					0, GN(pSceneWorld), 1));


	pCameraRenderer->ClearSceneWorlds();
	pCameraRenderer->AddSceneWorld(GN(pSceneWorld));


}

void CTestRendering::Frame(SwapChainHandle_t hSwapChain)
{	
	pCameraRenderer->ClearExcludeTags();
	pCameraRenderer->ClearRenderTags();
	CTransformUnaligned transform;
	transform.m_scale = {
		1,
		1,
		1,
	};
	transform.m_position = {
		0,0,0
	};
	pSceneObject->SetTransform(transform);

	mat4 m;
	glm_mat4_identity(m);
	glm_mat4_quat(m, *(versor*)&transform.m_rotation);
	pWorldRef->SetWorldTransform(transform);
	glm_mat4_identity(m);
	glm_rotate(m, glm_rad(180), (vec3){0,0,1});
	glm_mat4_quat(m, *(versor*)&transform.m_rotation);

	void *pm = pSceneObject->GetModelHandle();
	CREATE_NATIVE(CModel, model);
	model->m_pSelf = pm;
	transform.m_scale = {
		3,
		3,
		3,
	};
	transform.m_position = {
		200,0,0
	};
	pSceneObject->SetTransform(transform);
	pSceneObject->SetTintRGBA((Vector4D){1,0,0,1});

	g_pWorldRendererMgr->UpdateObjectsForRendering(StringToken(pSceneWorld->GetWorldDebugName()), (Vector){}, 1,500);

	pRenderAttributes->SetBoolValue(StringToken("renderOpaque"), true);
	pRenderAttributes->SetBoolValue(StringToken("renderTranslucent"), true);
	pRenderAttributes->SetBoolValue(StringToken("directLighting"), true);
	pRenderAttributes->SetBoolValue(StringToken("indirectLighting"), true);
	pRenderAttributes->SetBoolValue(StringToken("renderSun"), true);
	pRenderAttributes->SetBoolValue(StringToken("drawShadows"), true);
	pRenderAttributes->SetVector4DValue(StringToken("ambientColor"), (Vector4D){0.7,0.7,0.7,1});
	pRenderAttributes->SetVector4DValue(StringToken("clearColor"), (Vector4D){1,1,1,0});
	pRenderAttributes->SetIntValue(StringToken("clearFlags"), 0x3FF);

	pCameraRenderer->SetRenderAttributes(GN(pRenderAttributes));

	pCameraRenderer->ViewUniqueId = 100;
	Vector campos = {
		.x = -120,
		.y = 0,
		.z = 150,
	};

	int w = 0, h = 0;
	g_pEngineServiceMgr->GetEngineSwapChainSize(&w, &h);
	float fAspect = (float)w/(float)h;
	pCameraRenderer->CameraPosition = campos;
	pCameraRenderer->CameraRotation = (QAngle){0,0,0};
	pCameraRenderer->ZNear = 1;
	pCameraRenderer->ZFar = 10000;
	pCameraRenderer->FieldOfView = 68;
	pCameraRenderer->Rect = (Rect_t){0,0,1,1};
	pCameraRenderer->Viewport = (Vector4D){0,0,1,1};
	pCameraRenderer->Ortho = false;
	pCameraRenderer->ClipSpaceBounds = (Vector4D){-1*fAspect, -1, 1*fAspect, 1};
	pCameraRenderer->EnableEngineOverlays = true;
	pCameraRenderer->EnablePostprocessing = false;
	pCameraRenderer->FlipX = false;
	pCameraRenderer->FlipY = false;

	pCameraRenderer->ClearSceneWorlds();
	pCameraRenderer->AddSceneWorld(GN(pSceneWorld));

	pCameraRenderer->Render(hSwapChain);
}
static CTestRendering s_testRendering;
ITestRendering *pTestRendering = &s_testRendering;
