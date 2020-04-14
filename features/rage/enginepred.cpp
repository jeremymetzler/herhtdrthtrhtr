#include "../../features.hpp"

// i will never know how todo a proper engine pred

/*
this is all old shitty stuff still none is mine rly
*/

float Rage::flCurTime = 0.f;
float Rage::flFrameTime = 0.f;
int Rage::iTickCount = 0;

int* m_pPredictionRandomSeed;
int* m_pSetPredictionPlayer;

CMoveData m_MoveData;
static void* cMoveData = nullptr;

bool EnginePredRan = false; // was used for when there was a post engine pred step

void Rage::PreEnginePred( CUserCmd* pCmd )
{
	EnginePredRan = false;

	if ( Menu::Config.legitback )
		return;

	if ( !Interfaces::MoveHelper )
		return;

	auto pLocalEnt = Interfaces::User( );

	if ( !pLocalEnt || !pLocalEnt->m_bIsAlive( ) )
		return;

	if ( !pLocalEnt->GetActiveWeapon( ) )
		return;

	pLocalEnt->GetActiveWeapon( )->m_UpdateAccuracyPenalty( );

	static bool bInit = false;
	if ( !bInit ) {

		/*
		  if ( a3 )
			dword_10CB5C04 = *(_DWORD *)(a3 + 64);
		 else
			dword_10CB5C04 = -1;

			 if ( v22 )
	CVProfile::EnterScope(
	  g_VProfCurrentProfile,
	  "CPrediction::ProcessMovement",
	  0,
	  "CPrediction::ProcessMovement",
	  0,
	  4);
		*/

		m_pPredictionRandomSeed = *reinterpret_cast< int** >( Memory::Signature<uintptr_t>( GetModuleHandleA( "client_panorama.dll"), "A3 ? ? ? ? 66 0F 6E 86" ) + 1 );
		m_pSetPredictionPlayer = *reinterpret_cast< int** >( Memory::Signature<uintptr_t>( GetModuleHandleA( "client_panorama.dll"), "89 35 ? ? ? ? F3 0F 10 48 20" ) + 2 );
		cMoveData = malloc( 184 );
		bInit = true;
	}

	*m_pPredictionRandomSeed = MD5_PseudoRandom( pCmd->command_number ) & 0x7FFFFFFF;
	*m_pSetPredictionPlayer = reinterpret_cast< int >( pLocalEnt );
	
	flCurTime = Interfaces::GlobalVars->curtime;
	flFrameTime = Interfaces::GlobalVars->frametime;

	Interfaces::GlobalVars->curtime = float( pLocalEnt->m_iTickBase( ) ) * Interfaces::GlobalVars->interval_per_tick;
	Interfaces::GlobalVars->frametime = Interfaces::GlobalVars->interval_per_tick;

	Interfaces::GameMovement->StartTrackPredictionErrors( pLocalEnt );

	Interfaces::MoveHelper->SetHost( pLocalEnt );

	Interfaces::Prediction->SetupMove( pLocalEnt, pCmd, Interfaces::MoveHelper, reinterpret_cast< CMoveData* >( cMoveData ) );
	Interfaces::GameMovement->ProcessMovement( pLocalEnt, reinterpret_cast< CMoveData* >( cMoveData ) );
	Interfaces::Prediction->FinishMove( pLocalEnt, pCmd, reinterpret_cast< CMoveData* >( cMoveData ) );

	Interfaces::GameMovement->FinishTrackPredictionErrors( pLocalEnt );
	Interfaces::MoveHelper->SetHost( nullptr );

	Interfaces::GlobalVars->curtime = flCurTime;
	Interfaces::GlobalVars->frametime = flFrameTime;

	*m_pPredictionRandomSeed = -1;
	*m_pSetPredictionPlayer = 0;


	EnginePredRan = true;
}