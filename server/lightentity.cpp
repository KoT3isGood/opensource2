#include "lightentity.h"
#include "datamap.h"

void CLightEntity::Spawn()
{
	m_pLightObject->SetRenderDiffuse(true);
	m_pLightObject->SetRenderSpecular(true);
	m_pLightObject->SetUsesIndexedBakedLighting(false);
};

BEGIN_DATADESC(CLightEntity)
	
	DEFINE_KEYFIELD(m_lightColor, FIELD_COLOR255, "color")
	DEFINE_KEYFIELD(m_lightColor, FIELD_INTEGER, "shadows")

END_DATADESC()
