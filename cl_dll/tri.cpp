//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

// Triangle rendering, if any

#include "hud.h"
#include "cl_util.h"

// Triangle rendering apis are in gEngfuncs.pTriAPI

#include "const.h"
#include "entity_state.h"
#include "cl_entity.h"
#include "triangleapi.h"
#include "Exports.h"

#include "particleman.h"
#include "tri.h"

#include "studio.h"
#include "com_model.h"
#include "StudioModelRenderer.h"
#include "GameStudioModelRenderer.h"

extern CGameStudioModelRenderer g_StudioRenderer;

extern IParticleMan *g_pParticleMan;
void UpdateLaserSpot();

// Interpolate sprite objects
void InterpolateObjects()
{
	if (!g_StudioRenderer.m_pCvarOriginInterpolation->value)
		return;

	for (int i = 1; i < 2048; i++)
	{
		cl_entity_s* ent = gEngfuncs.GetEntityByIndex(i);

		if (ent && ent->model && ent->model->type == mod_sprite)
		{
			if (!ent->baseline.iuser4)
			{
				ent->baseline.vuser1 = ent->origin;
				ent->baseline.iuser4 = 1;
			}
			else
			{
				ent->baseline.vuser1[0] = lerp(ent->baseline.vuser1[0], ent->curstate.origin[0], gHUD.m_flTimeDelta * 15.5f);
				ent->baseline.vuser1[1] = lerp(ent->baseline.vuser1[1], ent->curstate.origin[1], gHUD.m_flTimeDelta * 15.5f);
				ent->baseline.vuser1[2] = lerp(ent->baseline.vuser1[2], ent->curstate.origin[2], gHUD.m_flTimeDelta * 15.5f);
				VectorCopy(ent->baseline.vuser1, ent->origin);
			}
		}
	}
}
/*
=================
HUD_DrawNormalTriangles

Non-transparent triangles-- add them here
=================
*/
void CL_DLLEXPORT HUD_DrawNormalTriangles( void )
{
//	RecClDrawNormalTriangles();

	InterpolateObjects();
	UpdateLaserSpot();

	gHUD.m_Spectator.DrawOverview();
}

#if defined( _TFC )
void RunEventList( void );
#endif

/*
=================
HUD_DrawTransparentTriangles

Render any triangles with transparent rendermode needs here
=================
*/
void CL_DLLEXPORT HUD_DrawTransparentTriangles( void )
{
//	RecClDrawTransparentTriangles();

#if defined( _TFC )
	RunEventList();
#endif

	InterpolateObjects();
	UpdateLaserSpot();

	if ( g_pParticleMan )
		 g_pParticleMan->Update();
}
