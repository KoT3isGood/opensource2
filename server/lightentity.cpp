#include "lightentity.h"
#include "datamap.h"
#include "engine/stringtoken.h"
#include "engine/structs.h"

void CLightEntity::Spawn()
{
	if (!m_pLightObject)
		return;


	printf("%f %f %f\n", m_transform.m_position.x, m_transform.m_position.y, m_transform.m_position.z);
	printf("%f %f %f %f\n", m_transform.m_rotation.x, m_transform.m_rotation.y, m_transform.m_rotation.z, m_transform.m_rotation.w);
	printf("%f %f %f\n", m_transform.m_scale.x, m_transform.m_scale.y, m_transform.m_scale.z);
	
	m_pLightObject->AddTag(StringToken("light"));
	m_pLightObject->SetWorldDirection(m_transform.m_rotation);

	m_pLightObject->SetLightFlags(m_pLightObject->GetLightFlags() | 32 );
	m_pLightObject->SetLightCookie(m_hLightCookie);
	m_pLightObject->SetColor((Vector){5, 5, 5});
	m_pLightObject->SetBounceColor((Vector){m_lightColor.x,m_lightColor.y,m_lightColor.z});
	m_pLightObject->SetRenderDiffuse(true);
	m_pLightObject->SetRenderSpecular(true);
	m_pLightObject->SetShadows(1);
};

BEGIN_DATADESC(CLightEntity)
	
	DEFINE_KEYFIELD(m_lightColor, FIELD_COLOR255, "color")
	DEFINE_KEYFIELD(m_nShadows, FIELD_INTEGER, "cast")
	DEFINE_KEYFIELD(m_nCascades, FIELD_INTEGER, "numcascades")
	DEFINE_KEYFIELD(m_hLightCookie, FIELD_TEXTURE, "lightcookie")
	DEFINE_KEYFIELD(m_fShadowCascadeDistanceScale, FIELD_FLOAT, "shadowcascadedistancescale")
	DEFINE_KEYFIELD(m_iBakeLightIndex, FIELD_INTEGER, "bakelightindex")
	DEFINE_KEYFIELD(m_iDirectLight, FIELD_INTEGER, "directlight")
END_DATADESC()
