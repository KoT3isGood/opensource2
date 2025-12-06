//================= Copyright kotofyt, All rights reserved ==================//
//
// Purpose: Valve-styled entity data maps
//
// Note: This file is insipired by Valve's convention in Source SDK, but it
// was written for scratch with compatibility in mind.
//
//===========================================================================//

#ifndef DATAMAP_H
#define DATAMAP_H
#include "stddef.h"

enum fieldtype_t {
	FIELD_VOID = 0,
	FIELD_FLOAT,
	FIELD_STRING,
	FIELD_VECTOR2D,
	FIELD_VECTOR,
	FIELD_VECTOR4D,
	FIELD_QUATERNION,
	FIELD_QUATERNION_QANGLE,
	FIELD_COLOR255,
	FIELD_COLOR1,
	FIELD_INTEGER,
	FIELD_BOOLEAN,
};

struct typedescription_t
{
	const char *m_szFieldName;
	const char *m_szEditorName;
	fieldtype_t m_eFieldType;
	size_t m_iFieldOffset;
	unsigned int m_uFieldCount;
	unsigned short m_uFieldSize;
	unsigned int m_iFlags;
};

struct datamap_t
{
	struct datamap_t *m_pBase;

	typedescription_t *m_pData;
	int m_iNumFields;
};

#define DECLARE_DATADESC() \
	datamap_t *GetBaseMap(); \
	virtual datamap_t *GetDataMap() override; \
	virtual datamap_t DataMapInit() override;

#define DECLARE_DATADESC_NOBASE() \
	datamap_t *GetBaseMap(); \
	virtual datamap_t *GetDataMap(); \
	virtual datamap_t DataMapInit();

#define BEGIN_DATADESC( className ) \
	datamap_t *className::GetBaseMap() { return BaseClass::GetDataMap(); } \
	BEGIN_DATADESC_INTERNAL(className)
#define BEGIN_DATADESC_NOBASE( className ) \
	datamap_t *className::GetBaseMap() { return NULL; } \
	BEGIN_DATADESC_INTERNAL(className)

#define BEGIN_DATADESC_INTERNAL( className ) \
	datamap_t *className::GetDataMap() { static datamap_t s_DataMap = ThisClass::DataMapInit(); return &s_DataMap; } \
	datamap_t className::DataMapInit() \
	{ \
		datamap_t map; \
		map.m_pBase = ThisClass::GetBaseMap(); \
		static typedescription_t dataDesc[] \
		{\

#define END_DATADESC() \
		};\
		map.m_iNumFields = sizeof(dataDesc)/sizeof(typedescription_t); \
		map.m_pData = dataDesc; \
		return map; \
	}

#define IMPLEMENT_NULL_DATADESC( className ) \
	BEGIN_DATADESC(className) \
	END_DATADESC()

#define _class_offsetof( class, var ) ((size_t)&(((class*)0)->var))

#define _FIELD( name, fieldtype, count, flags, mapname, tolerance) { #name, mapname, fieldtype, _class_offsetof(ThisClass, name), count, sizeof(((ThisClass*)0)->name), flags },
#define DEFINE_KEYFIELD( name, fieldtype, mapname ) _FIELD( name, fieldtype, 1, FTYPEDESC_KEY, mapname, 0)

#define FTYPEDESC_KEY 0x0004


#endif
