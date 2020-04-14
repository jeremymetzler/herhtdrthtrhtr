#include "../../features.hpp"

float __fastcall Hooked::ViewModelFov( void* ecx, void* edx )
{
	if ( Menu::Config.viewmodelfov != 0.0 )
		return Menu::Config.viewmodelfov;
	else
		return 60.f;
}

void __fastcall Hooked::RunCommand( void* ecx, void* edx, CBaseEntity* pEntity, CUserCmd* pCmd, IMoveHelper* helper )
{
	Original::Prediction->Get< decltype( &Hooked::RunCommand ) >( 19 )( ecx, edx, pEntity, pCmd, helper );
}

void __fastcall Hooked::FrameStageNotify( void *ecx, void *edx, int iStage ) 
{
	if ( !Interfaces::Engine->IsInGame( ) || !Interfaces::Engine->IsConnected( ) )
		return Original::Client->Get< decltype( &Hooked::FrameStageNotify ) >( 37 )( ecx, edx, iStage );

	try // i dont rly know how this works pls dont roast me i tried
	{
		LagComp::FrameStage( iStage );

		Original::Client->Get< decltype( &Hooked::FrameStageNotify ) >( 37 )( ecx, edx, iStage );
	}
	catch ( const std::exception& ex ) 
	{
		LagComp::pPlayer->clear();
	}
}

bool __fastcall Hooked::CreateMove( IClientMode* ecx, void* edx, float frametime, CUserCmd* pCmd ) 
{
	if ( !pCmd || !pCmd->command_number )
		return true;

	if ( !GetKeyState( VK_INSERT ) ) // OP
		pCmd->buttons &= ~IN_ATTACK;

	ConVar *sv_cheats = Interfaces::Cvar->FindVar( "sv_cheats" ); // hacker

	if ( sv_cheats->ReadInt( ) == 0 )
		sv_cheats->SetInt( 1 );

	Globals::bSendPacket = true;

	Vec3D vOldang = pCmd->viewangles;

	auto pLocalEnt = Interfaces::User( );

	if ( pLocalEnt && pLocalEnt->m_bIsAlive( ) )
		Globals::AimPunchAng = Rage::DecayAimPunchAngleReversed( pLocalEnt );

	static int nSinceUse = 0;

	Rage::FakeLag( );

	Movement::Bunnyhop( pCmd );
	Movement::AutoStrafe( pCmd );

	if ( GetAsyncKeyState( Menu::Config.slwwlk ) )
		Rage::Stop( pCmd );

	Rage::PreEnginePred( pCmd );
	Resolver::ResolverPoints( );
	Rage::AntiAim( pCmd );
	Legit::Backtrack( pCmd );
	Rage::Aimbot( pCmd );

	pCmd->viewangles.Clamp( );

	if ( !Menu::Config.legitback )
	{
		uintptr_t *dwFrame;
		__asm mov dwFrame, ebp;
		*reinterpret_cast< bool* >( *dwFrame - 0x1C ) = Globals::bSendPacket;

		Rage::CorrectMovement( vOldang, pCmd );
	}

	return false;
}

void __fastcall Hooked::PaintTraverse( void* pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce )
{
	static unsigned int vguiPanelID = NULL;
	Original::Panel->Get< decltype( &Hooked::PaintTraverse ) >( 41 )( pPanels, edx, vguiPanel, forceRepaint, allowForce );

	if ( !vguiPanelID && !strcmp( "MatSystemTopPanel", Interfaces::Panel->GetName( vguiPanel ) ) ) vguiPanelID = vguiPanel; // dont fucking touch with ur dumbass format stacker
	else if ( vguiPanel != vguiPanelID )
		return;

	Visuals::Esp( );

	Menu::Render( );

	Menu::PopUpHandle( );
}

void __fastcall Hooked::DrawModelExecute( void* ecx, void* edx, IMatRenderContext* context, const DrawModelState_t& state, const ModelRenderInfo_t& renderInfo, matrix3x4_t* matrix )
{
	if ( !Interfaces::Engine->IsInGame( ) || !Interfaces::Engine->IsConnected( ) )
		return Original::ModelRender->Get< decltype( &Hooked::DrawModelExecute ) >( 21 )( ecx, edx, context, state, renderInfo, matrix );

	const char* ModelName = Interfaces::ModelInfo->GetModelName( ( model_t* ) renderInfo.pModel );
	CBaseEntity* pPlayerEntity = Interfaces::EntityList->m_pGetClientEntity( renderInfo.index );

	if ( strstr( ModelName, "models/player/contactshadow" ) )
		return;

	auto pLocalEnt = Interfaces::User( );

	if ( !pLocalEnt )
		return Original::ModelRender->Get< decltype( &Hooked::DrawModelExecute ) >( 21 )( ecx, edx, context, state, renderInfo, matrix );

	if ( !pPlayerEntity
		|| !pPlayerEntity->m_bIsAlive( )
		|| !pPlayerEntity->m_bIsPlayer( )
		|| pPlayerEntity->IsDormant( )
		|| !strstr( ModelName, "models/player" ) )
		return Original::ModelRender->Get< decltype( &Hooked::DrawModelExecute ) >( 21 )( ecx, edx, context, state, renderInfo, matrix );

	static matrix3x4_t mBackup[ 128 ];
	matrix3x4_t* pMatrix = LagComp::pMatrix[ renderInfo.index ];

	if ( pPlayerEntity->entindex( ) == pLocalEnt->entindex( ) )
	{
		memcpy( mBackup, pMatrix, ( sizeof( matrix3x4_t ) * 128 ) );
		for ( int i = 0; i < 128; i++ )
			mBackup[ i ].SetOrigin( ( mBackup[ i ].GetOrigin( ) - LagComp::pOrigin[ renderInfo.index ] ) + pPlayerEntity->GetAbsOrigin( ) );

		return Original::ModelRender->Get< decltype( &Hooked::DrawModelExecute ) >( 21 )( ecx, edx, context, state, renderInfo, mBackup );
	}
	else if ( Menu::Config.chams && pPlayerEntity->GetTeam() != pLocalEnt->GetTeam( ) )
		return;

	return Original::ModelRender->Get< decltype( &Hooked::DrawModelExecute ) >( 21 )( ecx, edx, context, state, renderInfo, pMatrix );
}

void __fastcall Hooked::LockCursor( ISurface* ecx, void* edx )
{
	if ( GetKeyState( VK_INSERT ) )
	{
		return Original::Surface->Get< decltype( &Hooked::LockCursor ) >( 67 )( ecx, edx );
	}

	Interfaces::Surface->UnLockCursor( );
}

CBaseEntity* UTIL_PlayerByIndex( int index )
{
	typedef CBaseEntity*( __fastcall* PlayerByIndex )( int );
	static PlayerByIndex UTIL_PlayerByIndex = reinterpret_cast< PlayerByIndex >( Memory::Signature<uintptr_t>( GetModuleHandleA( "server.dll" ), "85 C9 7E 2A A1" ) );

	if ( !UTIL_PlayerByIndex )
		return nullptr;

	return UTIL_PlayerByIndex( index );
}

void __fastcall Hooked::BeginFrame( void* ecx ) // weird flicker idk how to fix
{
	Original::StudioRender->Get< decltype( &Hooked::BeginFrame ) >( 9 )( ecx );

	if ( !Interfaces::Engine->IsInGame( ) || !Interfaces::Engine->IsConnected( ) )
		return;

	if ( Menu::Config.bullettrails && Globals::Bullet.size( ) > 0 )
	{
		for ( int i = 0; i < Globals::Bullet.size( ); i++ )
		{
			BulletHit Bullet = Globals::Bullet.at( i );

			if ( Bullet.HitPlayer )
			{
				Interfaces::DebugOverlay->AddLineOverlayAlpha( Bullet.ShootPos, Bullet.Hit, 0,255,0, 255, true, 1.f );
			}
			else
			{
				Interfaces::DebugOverlay->AddLineOverlayAlpha( Bullet.ShootPos, Bullet.Hit, 255, 0, 0, 255, true, 1.f );
			}

			Globals::Bullet.erase( Globals::Bullet.begin( ) + i );
		}
	}

	auto pLocalEnt = Interfaces::User( );

	if ( !pLocalEnt )
		return;
	
	if ( Menu::Config.chams )
	{
		for ( int index = 1; index < MAX_NETWORKID_LENGTH; ++index )
		{
			CBaseEntity* pPlayerEntity = Interfaces::EntityList->m_pGetClientEntity( index );

			if ( !pPlayerEntity
				|| !pPlayerEntity->m_bIsAlive( )
				|| !pPlayerEntity->m_bIsPlayer( )
				
				|| pPlayerEntity->IsDormant( )
				|| ( pLocalEnt->GetTeam( ) == pPlayerEntity->GetTeam( ) && index != pLocalEnt->entindex( ) ) )
			{
				continue;
			}

			if ( Menu::Config.shotcham && Rage::iTargetMatrix.second == index )
			{
				pPlayerEntity->DrawHitboxes( Rage::iTargetMatrix.first, Color( Menu::Config.shotchamscolor.red, Menu::Config.shotchamscolor.green, Menu::Config.shotchamscolor.blue, 100 ), Color( Menu::Config.shotchamscolor.red, Menu::Config.shotchamscolor.green, Menu::Config.shotchamscolor.blue, 255 ), .5f );
				Rage::iTargetMatrix.second = -1;
			}

			if ( pLocalEnt->entindex( ) == index )
			{
				if ( false ) //  i just do true when i wanna use it
				{
					static uintptr_t pCall = Memory::Signature<uintptr_t>( GetModuleHandleA( "server.dll" ), "55 8B EC 81 EC ? ? ? ? 53 56 8B 35 ? ? ? ? 8B D9 57 8B CE" );
					static float fDuration = -1.f;

					PVOID pEntity = nullptr;
					pEntity = UTIL_PlayerByIndex( index );

					if ( pEntity )
					{
						__asm
						{
							pushad
							movss xmm1, fDuration
							push 0 //bool monoColor
							mov ecx, pEntity
							call pCall
							popad
						}
					}
				}
				if ( GetKeyState( Menu::Config.tpkey ) )
					pPlayerEntity->DrawHitboxes( LagComp::pMatrix[ index ], Color( 255, 255, 255, 150 ), Color( 255, 255, 255, 150 ), -1.f );
			}
			else
				pPlayerEntity->DrawHitboxes( LagComp::pMatrix[ index ], Color( Menu::Config.chamscolor.red, Menu::Config.chamscolor.green, Menu::Config.chamscolor.blue, 255 ), Color( Menu::Config.chamscolor.red, Menu::Config.chamscolor.green, Menu::Config.chamscolor.blue, 255 ), -1.f );
		}
	}
}

bool __fastcall Hooked::WriteUserCmdDeltaToBuffer( void* ecx, void* edx, int iSlot, bf_write* wBuffer, int iFrom, int iTo, bool bIsNewCommand ) // i tried todo tick base manip from a bunch of pasting but i just rly couldnt
{
	return Original::Client->Get< decltype( &Hooked::WriteUserCmdDeltaToBuffer ) >( 24 )( ecx, edx, iSlot, wBuffer, iFrom, iTo, bIsNewCommand );
}

void __fastcall Hooked::OverrideView( void* ecx, void* edx, CViewSetup* pSetup )
{
	Original::ClientMode->Get< decltype( &Hooked::OverrideView ) >( 18 )( ecx, edx, pSetup );

	if ( !Interfaces::Engine->IsInGame( ) || !Interfaces::Engine->IsConnected( ) )
		return;

	auto pLocalEnt = Interfaces::User( );
	static bool died = false;
	if ( pLocalEnt && pLocalEnt->m_bIsAlive( ) )
	{
		died = false;

		if ( GetKeyState( Menu::Config.tpkey ) )
			Interfaces::Input->m_bCameraInThirdperson = true;
		else
			Interfaces::Input->m_bCameraInThirdperson = false;

		if ( Menu::Config.novisrecoil && !GetKeyState( Menu::Config.tpkey ) )
		{
			Vec3D viewPunch = pLocalEnt->m_viewPunchAngle( );
			Vec3D aimPunch = pLocalEnt->m_aimPunchAngle( );

			float RecoilScale = Interfaces::Cvar->FindVar( "weapon_recoil_scale" )->ReadFloat( );

			pSetup->angles -= ( viewPunch + ( aimPunch * RecoilScale * 0.4499999f ) );
		}

		if ( Menu::Config.fov != 0.0 )
		{
			pSetup->fov = Menu::Config.fov;
		}

		if ( GetKeyState( Menu::Config.tpkey ) ) // my old thirdperson
		{
			Vec3D ViewAngles, ViewForward;
			Interfaces::Engine->GetViewAngles( ViewAngles );

			pSetup->angles = ViewAngles;

			ViewAngles.x = -ViewAngles.x;
			ViewAngles.y = Math::m_flNormalizeYaw( ViewAngles.y + 180 );

			Math::AngleVectors( ViewAngles, &ViewForward );
			Math::NormalizeAngles( ViewForward );

			Vec3D Origin = pLocalEnt->GetAbsOrigin( ) + pLocalEnt->GetViewOffset( );

			trace_t TraceInit;
			CTraceFilter filter1( pLocalEnt, TRACE_WORLD_ONLY );
			Interfaces::Trace->TraceRay( Ray_t( Origin, ( Origin + ( ViewForward * Menu::Config.tpdistance ) ) ), MASK_SOLID, &filter1, &TraceInit );

			ViewForward = Origin + ( ViewForward * ( TraceInit.m_flFraction *  (Menu::Config.tpdistance - 15) ) );

			pSetup->origin = ViewForward;
		}
	}
	else if ( !died )
	{
		died = true;
		Interfaces::Input->m_bCameraInThirdperson = false;
	}

	Globals::ViewSetup = *pSetup;
}


// :))))
void __fastcall Hooked::EmitSound( void* ecx, void* edx, void* filter, int iEntIndex, int iChannel, const char *pSoundEntry, unsigned int nSoundEntryHash, const char *pSample, float flVolume, float flAttenuation, int nSeed, int iFlags, int iPitch, const Vec3D *pOrigin, const Vec3D *pDirection, Vec3D * pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, StartSoundParams_t& params )
{
	if (strstr( pSample, "weapon" ) )
	{
		if ( iEntIndex == Interfaces::Engine->GetLocalPlayer( ) )
		{
			if ( Menu::Config.ragebot && !Menu::Config.legitback )
				return;
		}
	}

	Original::EngineSound->Get< decltype( &Hooked::EmitSound ) >( 5 )( ecx, edx, filter, iEntIndex, iChannel, pSoundEntry, nSoundEntryHash, pSample, flVolume, flAttenuation, nSeed, iFlags, iPitch, pOrigin, pDirection, pUtlVecOrigins, bUpdatePositions, soundtime, speakerentity, params );
}