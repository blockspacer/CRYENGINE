// Copyright 2001-2017 Crytek GmbH. All rights reserved.

#include "StdAfx.h"
#include "AIEntityComponent.h"

CAIEntityComponent::~CAIEntityComponent()
{
	m_pEntity->SetFlags(m_pEntity->GetFlags() & ~ENTITY_FLAG_HAS_AI);
	m_pEntity->SetAIObjectID(0);

	if (gEnv->pAISystem)
	{
		if (IAIObject* pAIObject = m_objectReference.GetAIObject())
		{
			gEnv->pAISystem->GetAIObjectManager()->RemoveObject(GetAIObjectID());
		}
	}
}

void CAIEntityComponent::Initialize()
{
	m_pEntity->SetFlags(m_pEntity->GetFlags() | ENTITY_FLAG_HAS_AI);

	m_objectReference.GetAIObject()->SetPos(m_pEntity->GetWorldPos(), m_pEntity->GetForwardDir());
	m_pEntity->SetAIObjectID(m_objectReference.GetObjectID());
}

void CAIEntityComponent::ProcessEvent(SEntityEvent& event)
{
	switch (event.event)
	{
		case ENTITY_EVENT_XFORM:
		{
			if (CAIObject* pAIObject = m_objectReference.GetAIObject())
			{
				pAIObject->SetPos(m_pEntity->GetWorldPos(), m_pEntity->GetForwardDir());
			}
		}
		break;
		case ENTITY_EVENT_SET_NAME:
		{
			if (CAIObject* pAIObject = m_objectReference.GetAIObject())
			{
				pAIObject->SetName(m_pEntity->GetName());
			}
		}
		break;
		case ENTITY_EVENT_ACTIVATED:
		{
			if (CAIObject* pAIObject = m_objectReference.GetAIObject())
			{
				if (IAIActorProxy* pProxy = pAIObject->GetProxy())
				{
					pProxy->EnableUpdate(true);
				}
			}
		}
		break;
		case ENTITY_EVENT_DEACTIVATED:
		{
			if (CAIObject* pAIObject = m_objectReference.GetAIObject())
			{
				if (IAIActorProxy* pProxy = pAIObject->GetProxy())
				{
					pProxy->EnableUpdate(false);
				}
			}
		}
		break;
	}
}

uint64 CAIEntityComponent::GetEventMask() const
{
	return BIT64(ENTITY_EVENT_XFORM) | BIT64(ENTITY_EVENT_SET_NAME) | BIT64(ENTITY_EVENT_ACTIVATED) | BIT64(ENTITY_EVENT_DEACTIVATED);
}