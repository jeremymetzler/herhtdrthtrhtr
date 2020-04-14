#include "../../features.hpp"

// kinda like my old esp system 

/*
best part is my bounding box someone has done a similar thing b4 though
i just forgot about it
*/

RECT Visuals::BoundingBox = { NULL,NULL, NULL, NULL };
CBaseEntity* Visuals::pLocalEnt = nullptr;
int Visuals::YOffset = 0;
bool Visuals::bContinue = false;

void Visuals::Box( )
{
	Draw::OutlinedRect( BoundingBox.left - 1, BoundingBox.top - 1, BoundingBox.right - BoundingBox.left + 2, BoundingBox.bottom - BoundingBox.top + 2, Color( 0,0,0, 150 ) );
	Draw::OutlinedRect( BoundingBox.left + 1, BoundingBox.top + 1, BoundingBox.right - BoundingBox.left - 2, BoundingBox.bottom - BoundingBox.top - 2, Color( 0, 0, 0, 150 ) );
	Draw::OutlinedRect( BoundingBox.left, BoundingBox.top, BoundingBox.right - BoundingBox.left, BoundingBox.bottom - BoundingBox.top, Color( Menu::Config.boxcolor.red, Menu::Config.boxcolor.green, Menu::Config.boxcolor.blue, 150 ) );
}

void Visuals::Name( CBaseEntity* pEntity )
{
	int iIndex = pEntity->entindex( );
	PlayerInfo_t pInfo;
	Interfaces::Engine->GetPlayerInfo( iIndex, &pInfo );

	if ( !pInfo.m_cName )
		return;

	Draw::String( BoundingBox.right + 2, BoundingBox.top + YOffset, Draw::Font.Default, Color( 255, 255, 255, 150 ), std::string( pInfo.m_cName ) );

	YOffset += 10;
}

void Visuals::HealthBar( CBaseEntity* pEntity )
{
	float Delta = float( BoundingBox.bottom - BoundingBox.top )  / 100.f;
	int Position = ( 100 - pEntity->m_iHealth( )) * Delta;

	if ( pEntity->m_iHealth( ) >= 100 )
		Draw::Rect( BoundingBox.left - 6, BoundingBox.top, 3, ( BoundingBox.bottom - BoundingBox.top ), Color( 0, 255, 0, 150 ) );
	else
	{
		Draw::Rect( BoundingBox.left - 6, BoundingBox.top, 3, Position, Color( 255, 0, 0, 150 ) );
		Draw::Rect( BoundingBox.left - 6, BoundingBox.top + Position, 3, ( BoundingBox.bottom - BoundingBox.top ) - Position, Color( 0, 255, 0, 150 ) );
	}

	Draw::OutlinedRect( BoundingBox.left - 7, BoundingBox.top - 1, 5, ( BoundingBox.bottom - BoundingBox.top) + 2 , Color( 0, 0, 0, 150 ) );
}

void Visuals::ResolverInfo( CBaseEntity* pEntity )
{
	int iIndex = pEntity->entindex( );
	Draw::String( BoundingBox.right + 2, BoundingBox.top + YOffset, Draw::Font.Default, Color( 255, 255, 255, 150 ), Resolver::ResolverMode[ iIndex ] );

	YOffset += 10;
/*
	float XOffset2 = 0.f;
	float YOffset2 = 0.f;

	if ( LagComp::pPlayer[ iIndex ].size( ) <= 0 )
		return;
	XOffset2 += 125;
	Record pPlayer = LagComp::pPlayer[ iIndex ].at( LagComp::pPlayer[ iIndex ].size( ) - 1 );

	CAnimationLayer* Layer = pPlayer.AnimLayers;
	CAnimationLayer* ClientLayer = pPlayer.ClientAnimLayers;
	CPlayerAnimstate* AnimState = pPlayer.AnimState;

	if ( !Layer )
		return;

	if ( !AnimState )
		return;

	for ( int layernum = 0; layernum < 13; layernum++ )
	{
		if ( layernum == 3 || layernum == 6 )
		{
			Draw::String( BoundingBox.right + 2 + XOffset2, BoundingBox.top + YOffset2, Draw::Font.Default, Color( 255, 255, 255, 150 ), "(" + std::to_string( layernum ) + ") m_iOrder = " + std::to_string( Layer[ layernum ].m_iOrder ) );
			YOffset2 += 10;
			Draw::String( BoundingBox.right + 2 + XOffset2, BoundingBox.top + YOffset2, Draw::Font.Default, Color( 255, 255, 255, 150 ), "(" + std::to_string( layernum ) + ") m_iSequence = " + std::to_string( Layer[ layernum ].m_iSequence ) );
			YOffset2 += 10;
			Draw::String( BoundingBox.right + 2 + XOffset2, BoundingBox.top + YOffset2, Draw::Font.Default, Color( 255, 255, 255, 150 ), "(" + std::to_string( layernum ) + ") m_flPrevCycle = " + std::to_string( Layer[ layernum ].m_flPrevCycle ) );
			YOffset2 += 10;
			Draw::String( BoundingBox.right + 2 + XOffset2, BoundingBox.top + YOffset2, Draw::Font.Default, Color( 255, 255, 255, 150 ), "(" + std::to_string( layernum ) + ") m_flWeight = " + std::to_string( Layer[ layernum ].m_flWeight ) );
			YOffset2 += 10;
			Draw::String( BoundingBox.right + 2 + XOffset2, BoundingBox.top + YOffset2, Draw::Font.Default, Color( 255, 255, 255, 150 ), "(" + std::to_string( layernum ) + ") m_flWeightDeltaRate = " + std::to_string( Layer[ layernum ].m_flWeightDeltaRate ) );
			YOffset2 += 10;
			Draw::String( BoundingBox.right + 2 + XOffset2, BoundingBox.top + YOffset2, Draw::Font.Default, Color( 255, 255, 255, 150 ), "(" + std::to_string( layernum ) + ") m_flPlaybackRate = " + std::to_string( Layer[ layernum ].m_flPlaybackRate ) );
			YOffset2 += 10;
			Draw::String( BoundingBox.right + 2 + XOffset2, BoundingBox.top + YOffset2, Draw::Font.Default, Color( 255, 255, 255, 150 ), "(" + std::to_string( layernum ) + ") m_flCycle = " + std::to_string( Layer[ layernum ].m_flCycle ) );
			YOffset2 += 10;
		}
	}
	Draw::String( BoundingBox.right + 2 + XOffset2, BoundingBox.top + YOffset2, Draw::Font.Default, Color( 255, 255, 255, 150 ), "(6) CLIENT m_flPlaybackRate = " + std::to_string( ClientLayer[ 6 ].m_flPlaybackRate ) );
	YOffset2 += 10;
	Draw::String( BoundingBox.right + 2 + XOffset2, BoundingBox.top + YOffset2, Draw::Font.Default, Color( 255, 255, 255, 150 ), "(6) m_flPlaybackRateDelta = " + std::to_string( float( pPlayer.flCycleDelta / float(  pPlayer.flSimDelta / Interfaces::GlobalVars->interval_per_tick ) - Layer[ 6 ].m_flPlaybackRate ) ) );
	YOffset2 += 10;//AnimState

	YOffset2 = 0;
	XOffset2 += 125;

	Draw::String( BoundingBox.left - 10, BoundingBox.top + YOffset2, Draw::Font.Default, Color( 255, 255, 255, 150 ), " bUnknown = " + std::to_string( AnimState->bUnknown ), 2 );
	YOffset2 += 10;
	Draw::String( BoundingBox.left - 10, BoundingBox.top + YOffset2, Draw::Font.Default, Color( 255, 255, 255, 150 ), " m_flAnimUpdateDelta = " + std::to_string( AnimState->m_flAnimUpdateDelta ), 2 );
	YOffset2 += 10;
	Draw::String( BoundingBox.left - 10, BoundingBox.top + YOffset2, Draw::Font.Default, Color( 255, 255, 255, 150 ), " m_flDelta = " + std::to_string( AnimState->m_flDelta ), 2 );
	YOffset2 += 10;
	Draw::String( BoundingBox.left - 10, BoundingBox.top + YOffset2, Draw::Font.Default, Color( 255, 255, 255, 150 ), " m_flEyeYaw = " + std::to_string( AnimState->m_flEyeYaw ), 2 );
	YOffset2 += 10;
	Draw::String( BoundingBox.left - 10, BoundingBox.top + YOffset2, Draw::Font.Default, Color( 255, 255, 255, 150 ), " m_flCurrentFeetYaw = " + std::to_string( AnimState->m_flCurrentFeetYaw ), 2 );
	YOffset2 += 10;
	Draw::String( BoundingBox.left - 10, BoundingBox.top + YOffset2, Draw::Font.Default, Color( 255, 255, 255, 150 ), " m_flCurrentTorsoYaw = " + std::to_string( AnimState->m_flCurrentTorsoYaw ), 2 );
	YOffset2 += 10;
	Draw::String( BoundingBox.left - 10, BoundingBox.top + YOffset2, Draw::Font.Default, Color( 255, 255, 255, 150 ), " m_flUnknownVelocityLean = " + std::to_string( AnimState->m_flUnknownVelocityLean ), 2 );
	YOffset2 += 10;
	Draw::String( BoundingBox.left - 10, BoundingBox.top + YOffset2, Draw::Font.Default, Color( 255, 255, 255, 150 ), " m_flLeanAmount = " + std::to_string( AnimState->m_flLeanAmount ), 2 );
	YOffset2 += 10;
	Draw::String( BoundingBox.left - 10, BoundingBox.top + YOffset2, Draw::Font.Default, Color( 255, 255, 255, 150 ), " m_flFeetCycle = " + std::to_string( AnimState->m_flFeetCycle ), 2 );
	YOffset2 += 10;
	Draw::String( BoundingBox.left - 10, BoundingBox.top + YOffset2, Draw::Font.Default, Color( 255, 255, 255, 150 ), " m_flFeetYawRate = " + std::to_string( AnimState->m_flFeetYawRate ), 2 );
	YOffset2 += 10;
	Draw::String( BoundingBox.left - 10, BoundingBox.top + YOffset2, Draw::Font.Default, Color( 255, 255, 255, 150 ), " m_fUnknown2 = " + std::to_string( AnimState->m_fUnknown2 ), 2 );
	YOffset2 += 10;
	Draw::String( BoundingBox.left - 10, BoundingBox.top + YOffset2, Draw::Font.Default, Color( 255, 255, 255, 150 ), " m_fUnknown3 = " + std::to_string( AnimState->m_fUnknown3 ), 2 );
	YOffset2 += 10;
	Draw::String( BoundingBox.left - 10, BoundingBox.top + YOffset2, Draw::Font.Default, Color( 255, 255, 255, 150 ), " m_flUnknownFloat1 = " + std::to_string( AnimState->m_flUnknownFloat1 ), 2 );
	YOffset2 += 10;
	Draw::String( BoundingBox.left - 10, BoundingBox.top + YOffset2, Draw::Font.Default, Color( 255, 255, 255, 150 ), " m_flUnknownFloat2 = " + std::to_string( AnimState->m_flUnknownFloat2 ), 2 );
	YOffset2 += 10;
	Draw::String( BoundingBox.left - 10, BoundingBox.top + YOffset2, Draw::Font.Default, Color( 255, 255, 255, 150 ), " m_flUnknownFloat3 = " + std::to_string( AnimState->m_flUnknownFloat3 ), 2 );
	YOffset2 += 10;
	Draw::String( BoundingBox.left - 10, BoundingBox.top + YOffset2, Draw::Font.Default, Color( 255, 255, 255, 150 ), " m_unknown = " + std::to_string( AnimState->m_unknown ), 2 );
	YOffset2 += 10;
	Draw::String( BoundingBox.left - 10, BoundingBox.top + YOffset2, Draw::Font.Default, Color( 255, 255, 255, 150 ), " speed_2d = " + std::to_string( AnimState->speed_2d ), 2 );
	YOffset2 += 10;
	Draw::String( BoundingBox.left - 10, BoundingBox.top + YOffset2, Draw::Font.Default, Color( 255, 255, 255, 150 ), " m_flFeetSpeedForwardsOrSideWays = " + std::to_string( AnimState->m_flFeetSpeedForwardsOrSideWays ), 2 );
	YOffset2 += 10;
	Draw::String( BoundingBox.left - 10, BoundingBox.top + YOffset2, Draw::Font.Default, Color( 255, 255, 255, 150 ), " m_flFeetSpeedUnknownForwardOrSideways = " + std::to_string( AnimState->m_flFeetSpeedUnknownForwardOrSideways ), 2 );
	YOffset2 += 10;
	Draw::String( BoundingBox.left - 10, BoundingBox.top + YOffset2, Draw::Font.Default, Color( 255, 255, 255, 150 ), " m_flMagicFraction = " + std::to_string( AnimState->m_flMagicFraction ), 2 );
	YOffset2 += 10;
	Draw::String( BoundingBox.left - 10, BoundingBox.top + YOffset2, Draw::Font.Default, Color( 255, 255, 255, 150 ), " m_flWorldForce = " + std::to_string( AnimState->m_flWorldForce ), 2);
	YOffset2 += 10;*/
}

void Visuals::Weapon( CBaseEntity* pEntity )
{
	auto pWeapon = pEntity->GetActiveWeapon( );
	
	if ( !pWeapon )
		return;

	auto pWeaponData = pWeapon->m_pWeaponData( );

	if ( !pWeaponData )
		return;

	Draw::String( BoundingBox.right + 2, BoundingBox.top + YOffset, Draw::Font.Default, Color( 255, 255, 255, 150 ), std::string( pWeaponData->m_WeaponName ) + " (" + std::to_string( pWeaponData->m_iBullets ) + ")" );
	YOffset += 10;
}

void Visuals::BoundBox( CBaseEntity* pEntity ) // i thought i was genius by doing this but then remembered someone did this b4 but like with studio bones or smthn still whatever i like it and it is mine :)
{
	int iIndex = pEntity->entindex( );

	if ( !LagComp::pMatrix[ iIndex ] )
		return;

	int LeftInt = 999999, TopInt = 999999, RightInt = -999999, BottomInt = -999999;
	static Vec3D w2s;

	int BestHitboxes[ 4 ];

	bContinue = false;

	for ( int i = 0; i < 19; i++ )
	{
		Vec3D vHitBoxPos = pEntity->GetHitboxPosition( i, LagComp::pMatrix[ iIndex ]);
		if ( Interfaces::DebugOverlay->m_bWorldToScreen( vHitBoxPos, w2s ) )
		{
			if ( w2s.x < LeftInt )
			{
				BestHitboxes[ 0 ] = i;
				LeftInt = w2s.x;
			}

			if ( w2s.x > RightInt )
			{
				BestHitboxes[ 1 ] = i;
				RightInt = w2s.x;
			}

			if ( w2s.y < TopInt )
			{
				BestHitboxes[ 2 ] = i;
				TopInt = w2s.y;
			}

			if ( w2s.y > BottomInt )
			{
				BestHitboxes[ 3 ] = i;
				BottomInt = w2s.y;
			}
		}
		else
		{
			bContinue = true;
		}
	}

	Vec3D flAngToLocal = Globals::ViewSetup.angles;

	static Vec3D Bruh[ 4 ];

	Math::AngleVectors( Vec3D( 0.f, Math::m_flNormalizeYaw( flAngToLocal.y + 90.f ), 0.f ), &Bruh[0] );
	Math::NormalizeAngles( Bruh[ 0 ] );
	Math::AngleVectors( Vec3D( 0.f, Math::m_flNormalizeYaw( flAngToLocal.y - 90.f ), 0.f ), &Bruh[ 1 ] );
	Math::NormalizeAngles( Bruh[ 1 ] );
	Math::AngleVectors( Vec3D( flAngToLocal.x - 90.f, Math::m_flNormalizeYaw( flAngToLocal.y), 0.f ), &Bruh[ 2 ] );
	Math::NormalizeAngles( Bruh[ 2 ] );
	Math::AngleVectors( Vec3D( flAngToLocal.x + 90.f, Math::m_flNormalizeYaw( flAngToLocal.y), 0.f ), &Bruh[ 3 ] );
	Math::NormalizeAngles( Bruh[ 3 ] );

	static Vec2D OK[ 4 ];

	for ( int i = 0; i < 4; i++ )
	{
		Vec3D vHitBoxPos = pEntity->GetHitboxPosition( BestHitboxes[ i ], LagComp::pMatrix[ iIndex ] );

		vHitBoxPos += ( Bruh[ i ] * 20.f );
		if ( Interfaces::DebugOverlay->m_bWorldToScreen( vHitBoxPos, w2s ) )
		{
			OK[i] = Vec2D( w2s.x , w2s.y );
		}
	}

	BoundingBox.left = OK[0].x;
	BoundingBox.right = OK[1].x;
	BoundingBox.top = OK[2].y;
	BoundingBox.bottom = OK[3].y;
}

void Visuals::Esp( )
{
	if ( !Menu::Config.visuals )
		return;

	pLocalEnt = Interfaces::User( );

	if ( !pLocalEnt )
		return;

	if ( pLocalEnt->m_bIsAlive( ) )
	{
		Draw::String( 10, 10, Draw::Font.Default, Color( 255, 255, 255, 255 ), std::to_string( pLocalEnt->m_angRotation( ).y ) + ", " + std::to_string( pLocalEnt->m_angRotation( ).x ) + ", " + std::to_string( pLocalEnt->m_angRotation( ).z ) );
	}

	for ( int index = 1; index < MAX_NETWORKID_LENGTH; ++index )
	{
		CBaseEntity* pPlayerEntity = Interfaces::EntityList->m_pGetClientEntity( index );

		if ( !pPlayerEntity
			|| !pPlayerEntity->m_bIsAlive( )
			|| !pPlayerEntity->m_bIsPlayer( )
			|| pLocalEnt->entindex( ) == index
			|| pPlayerEntity->IsDormant( )
			|| pLocalEnt->GetTeam( ) == pPlayerEntity->GetTeam( ) )
		{
			continue;
		}

		YOffset = 0;

		BoundBox( pPlayerEntity );

		if ( bContinue )
			continue;

		if ( Menu::Config.box )
			Box( );

		if ( Menu::Config.name )
			Name( pPlayerEntity );

		if ( Menu::Config.weaponname )
			Weapon( pPlayerEntity );

		if ( Menu::Config.resolvermode )
			ResolverInfo( pPlayerEntity );

		if ( Menu::Config.healthbar )
			HealthBar( pPlayerEntity );
	}
}