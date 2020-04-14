#include "../../features.hpp"

// the resolver can get the job done ig

/*
im prty sure u can do shit with server layer 6 playback rate / cycle delta but im not smart enough to figure it out
but you could probably make a rly good moving resolver out of it
*/

namespace Resolver
{
	float flOldGoalFeetYaw[ MAX_NETWORKID_LENGTH ];
	float flGoalFeetYaw[ MAX_NETWORKID_LENGTH ];
	Vec3D vOldEyeAng[ MAX_NETWORKID_LENGTH ];
	bool bUseFreestandAngle[ MAX_NETWORKID_LENGTH ];
	float flFreestandAngle[ MAX_NETWORKID_LENGTH ];
	float flLastFreestandAngle[ MAX_NETWORKID_LENGTH ];
	float flGoalFeetYawB[ MAX_NETWORKID_LENGTH ];
	float flLbyB[ MAX_NETWORKID_LENGTH ];
	std::string ResolverMode[ MAX_NETWORKID_LENGTH ];
}

void Resolver::PreResolver( CPlayerAnimstate* AnimState, CBaseEntity* pEntity, bool bShot )
{
	if ( !Menu::Config.resolver )
		return;

	if ( !AnimState )
		return;

	if ( !pEntity )
		return;

	auto RemapVal = [ ] ( float val, float A, float B, float C, float D ) -> float
	{
		if ( A == B )
			return val >= B ? D : C;
		return C + ( D - C ) * ( val - A ) / ( B - A );
	};

	int iIndex = pEntity->entindex( );
	auto pLocalEnt = Interfaces::User( );

	ResolverMode[ iIndex ] = "r:none";
	static int ResolveInt = 0;
	static int LastResolveMode[ MAX_NETWORKID_LENGTH ];

	static int iMode[ MAX_NETWORKID_LENGTH ];

	float flLastResolveYaw[ MAX_NETWORKID_LENGTH ];

	flGoalFeetYawB[ iIndex ] = 0.f;
	flLbyB[ iIndex ] = pEntity->m_flLowerBodyYaw( );

	int ShotDelta = Globals::iMissedShots[iIndex];

	if ( pLocalEnt && pLocalEnt->m_bIsAlive( ) && pLocalEnt != pEntity )
	{
		float angToLocal = Math::m_flNormalizeYaw( Math::CalcAngle( pLocalEnt->m_vecOrigin( ), pEntity->m_vecOrigin( ) ).y );

		float Back = Math::m_flNormalizeYaw( angToLocal );

		float Brute = AnimState->m_flGoalFeetYaw;

		if ( !isnan( angToLocal ) && !isinf( angToLocal ) && pEntity != pLocalEnt && pEntity->GetTeam( ) != pLocalEnt->GetTeam( ) )
		{
			float AntiSide = 0.f;

			float EyeDelta = fabs( Math::m_flNormalizeYaw( vOldEyeAng[ iIndex ].y - pEntity->m_angEyeAngles( ).y ) );
			
			if ( bUseFreestandAngle[ iIndex ] )
			{
				ResolveInt = 1;

				if ( LastResolveMode[ iIndex ] != ResolveInt )
				{
					Globals::iMissedShots[ iIndex ] = 0;
					ShotDelta = 0;
					LastResolveMode[ iIndex ] = ResolveInt;
				}

				ResolverMode[ iIndex ] = "r:smart";
				Brute = Math::m_flNormalizeYaw( Back + flLastFreestandAngle[ iIndex ] );

				if ( (ShotDelta % 3) == 2 )
				{
					ResolverMode[ iIndex ] += " bf";
					Brute = Math::m_flNormalizeYaw( Math::m_flNormalizeYaw(Back + flLastFreestandAngle[ iIndex ]) + 180.f );
				}
				else
				{
					if ( flLastFreestandAngle[ iIndex ] > 0 )
						ResolverMode[ iIndex ] += " <";
					else
						ResolverMode[ iIndex ] += " >";
				}

				if ( fabs( Math::m_flNormalizeYaw( Math::m_flNormalizeYaw( Back + flLastFreestandAngle[ iIndex ] ) - pEntity->m_angEyeAngles( ).y ) ) <= 90.f && fabs( Math::m_flNormalizeYaw( Math::m_flNormalizeYaw( Back ) - pEntity->m_angEyeAngles( ).y ) ) <= 30.f )
					iMode[ iIndex ] = 1;
				else
					iMode[ iIndex ] = 0;
			}
			else if ( fabs( Math::m_flNormalizeYaw( pEntity->m_angEyeAngles( ).y - Back ) ) >= 60.f && fabs( Math::m_flNormalizeYaw( pEntity->m_angEyeAngles( ).y - Math::m_flNormalizeYaw( Back + 180 ) ) ) >= 60.f &&  EyeDelta > 50 )
			{
				ResolveInt = 2;

				if ( LastResolveMode[ iIndex ] != ResolveInt )
				{
					Globals::iMissedShots[ iIndex ] = 0;
					ShotDelta = 0;
					LastResolveMode[ iIndex ] = ResolveInt;
				}

				ResolverMode[ iIndex ] = "r:jitter";

				if ( fabs( Math::m_flNormalizeYaw( pEntity->m_angEyeAngles( ).y - Math::m_flNormalizeYaw( Back ) ) ) <= 105.f )
					AntiSide = 0.f;
				else if ( fabs( Math::m_flNormalizeYaw( pEntity->m_angEyeAngles( ).y - Math::m_flNormalizeYaw( Back + 180 ) ) ) <= 75.f )
					AntiSide = 180.f;

				Brute = Math::m_flNormalizeYaw( Back + AntiSide );

				iMode[ iIndex ] = 0;
			}
			else
			{
				ResolveInt = 3;

				if ( LastResolveMode[ iIndex ] != ResolveInt )
				{
					Globals::iMissedShots[ iIndex ] = 0;
					ShotDelta = 0;
					LastResolveMode[ iIndex ] = ResolveInt;
				}

				if ( iMode[ iIndex ] == 0 )
				{
					ResolverMode[ iIndex ] = "r:inverse";
					switch ( ShotDelta % 2 )
					{
					case 0:
						if ( Math::m_flNormalizeYaw( pEntity->m_angEyeAngles( ).y - Back ) > 0.f )
						{
							ResolverMode[ iIndex ] += " >";
							AntiSide = -90.f;
						}
						else if ( Math::m_flNormalizeYaw( pEntity->m_angEyeAngles( ).y - Back ) < 0.f )
						{
							ResolverMode[ iIndex ] += " <";
							AntiSide = 90.f;
						}
						break;

					case 1:
						if ( Math::m_flNormalizeYaw( pEntity->m_angEyeAngles( ).y - Back ) > 0.f )
						{
							ResolverMode[ iIndex ] += " < bf";
							AntiSide = 90.f;
						}
						else if ( Math::m_flNormalizeYaw( pEntity->m_angEyeAngles( ).y - Back ) < 0.f )
						{
							ResolverMode[ iIndex ] += " > bf";
							AntiSide = -90.f;
						}

						break;
					}
				}
				else if ( iMode[ iIndex ] == 1 )
				{
					ResolverMode[ iIndex ] = "r:center";

					switch ( ShotDelta % 2 )
					{
					case 0:
						if ( fabs( Math::m_flNormalizeYaw( pEntity->m_angEyeAngles( ).y - Math::m_flNormalizeYaw( Back ) ) ) <= 105.f )
						{
							ResolverMode[ iIndex ] += " ^";
							AntiSide = 0.f;
						}
						else if ( fabs( Math::m_flNormalizeYaw( pEntity->m_angEyeAngles( ).y - Math::m_flNormalizeYaw( Back + 180 ) ) ) <= 75.f )
						{
							ResolverMode[ iIndex ] += " v";
							AntiSide = 180.f;
						}

						break;
					case 1:
						if ( fabs( Math::m_flNormalizeYaw( pEntity->m_angEyeAngles( ).y - Math::m_flNormalizeYaw( Back ) ) ) <= 105.f )
						{
							ResolverMode[ iIndex ] += " v bf";
							AntiSide = 180.f;
						}
						else if ( fabs( Math::m_flNormalizeYaw( pEntity->m_angEyeAngles( ).y - Math::m_flNormalizeYaw( Back + 180 ) ) ) <= 75.f )
						{
							ResolverMode[ iIndex ] += " ^ bf";
							AntiSide = 0.f;
						}
						break;
					}
				}

				Brute = Math::m_flNormalizeYaw( Back + AntiSide );
			}

			//--------------------------------------------------------------------------------------------------------------------

			if ( pEntity != pLocalEnt && pEntity->GetTeam( ) != pLocalEnt->GetTeam( ) && ( pEntity->m_fFlags( ) & FL_ONGROUND ) )
			{
				if ( bShot )
				{
					ResolveInt = 4;
					ResolverMode[ iIndex ] = "r:shot";
			
					Brute = vOldEyeAng[ iIndex ].y;
				}
				else
					flLastResolveYaw[ iIndex ] = Brute;

				if ( fabs( Math::m_flNormalizeYaw( pEntity->m_angEyeAngles( ).y - Brute ) ) > 58.f )
				{
					if ( Math::m_flNormalizeYaw( Brute - pEntity->m_angEyeAngles( ).y ) > 0 )
					{
						Brute = Math::m_flNormalizeYaw( pEntity->m_angEyeAngles( ).y + 58.f );
					}
					else
					{
						Brute = Math::m_flNormalizeYaw( pEntity->m_angEyeAngles( ).y - 58.f );
					}
				}

				AnimState->m_flGoalFeetYaw = Brute;
				flGoalFeetYawB[ iIndex ] = AnimState->m_flGoalFeetYaw;
				flLbyB[ iIndex ] = pEntity->m_flLowerBodyYaw( );
				pEntity->m_flLowerBodyYaw( ) = Brute;
			}
		}
	}

	if ( !bShot )
		vOldEyeAng[ iIndex ] = pEntity->m_angEyeAngles( );
}

void Resolver::PostResolver( CPlayerAnimstate* AnimState, CBaseEntity* pEntity, bool bShot )
{
	if ( !AnimState )
		return;

	if ( !pEntity )
		return;

	flGoalFeetYaw[ pEntity->entindex( ) ] = AnimState->m_flGoalFeetYaw;

	if ( !Menu::Config.resolver )
		return;

	int iIndex = pEntity->entindex( );

	auto pLocalEnt = Interfaces::User( );

	if ( !pLocalEnt )
		return;

	if ( pEntity != pLocalEnt )
	{
		pEntity->m_flLowerBodyYaw( ) = flLbyB[ iIndex ];
		if ( flGoalFeetYawB[ iIndex ] != 0.f )
			AnimState->m_flGoalFeetYaw = flGoalFeetYawB[ iIndex ];
	}
}

void Resolver::ResolverPoints( ) // still my old reverse freestanding
{
	if ( Globals::FreeStandPoints.size( ) > 0 )
		Globals::FreeStandPoints.clear( );

	if ( Menu::Config.legitback )
		return;

	if ( !Menu::Config.resolver )
		return;

	auto pLocalEnt = Interfaces::User( );
	
	if ( !pLocalEnt )
		return;

	if ( !pLocalEnt->m_bIsAlive( ) )
		return;

	if ( !pLocalEnt->GetActiveWeapon( ) )
		return;

	for ( int index = 1; index < MAX_NETWORKID_LENGTH; ++index )
	{
		CBaseEntity* pPlayerEntity = Interfaces::EntityList->m_pGetClientEntity( index );

		if ( !pPlayerEntity
			|| !pPlayerEntity->m_bIsAlive( )
			|| !pPlayerEntity->m_bIsPlayer( )
			|| pLocalEnt->entindex( ) == index
			|| pPlayerEntity->IsDormant( )
			|| pLocalEnt->GetTeam( ) == pPlayerEntity->GetTeam( )
			|| !LagComp::pMatrix[index] )
		{
			continue;
		}

		bUseFreestandAngle[ index ] = false;

	//	if ( !bUseFreestandAngle[ index ] )
		{
			bool Autowalled = false, HitSide1 = false, HitSide2 = false;

			float angToLocal = Math::CalcAngle( pLocalEnt->m_vecOrigin( ), pPlayerEntity->m_vecOrigin( ) ).y;
			Vec3D ViewPoint = pLocalEnt->m_vecOrigin( ) + Vec3D( 0, 0, 80 );

			Vec2D Side1 = Vec2D( ( 40 * sin( Math::m_flDegToRad( angToLocal ) ) ), ( 40 * cos( Math::m_flDegToRad( angToLocal ) ) ) );
			Vec2D Side2 = Vec2D( ( 40 * sin( Math::m_flDegToRad( angToLocal + 180 ) ) ), ( 40 * cos( Math::m_flDegToRad( angToLocal + 180 ) ) ) );

			Vec2D Side3 = Vec2D( ( 50 * sin( Math::m_flDegToRad( angToLocal ) ) ), ( 50 * cos( Math::m_flDegToRad( angToLocal ) ) ) );
			Vec2D Side4 = Vec2D( ( 50 * sin( Math::m_flDegToRad( angToLocal + 180 ) ) ), ( 50 * cos( Math::m_flDegToRad( angToLocal + 180 ) ) ) );

			Vec3D Origin = pPlayerEntity->m_vecOrigin( );

			Vec2D OriginLeftRight[ ] = { Vec2D( Side1.x, Side1.y ), Vec2D( Side2.x, Side2.y ) };

			Vec2D OriginLeftRightLocal[ ] = { Vec2D( Side3.x, Side3.y ), Vec2D( Side4.x, Side4.y ) };

			if ( !Autowall::CanHitFloatingPoint( LagComp::pMatrix[ index ][ 8 ].GetOrigin( ), pLocalEnt->GetEyePosition(), pPlayerEntity ) )
			{
				for ( int side = 0; side < 2; side++ )
				{
					Vec3D OriginAutowall = Vec3D( Origin.x + OriginLeftRight[ side ].x, Origin.y - OriginLeftRight[ side ].y, LagComp::pMatrix[ index ][ 8 ].GetOrigin( ).z );
					Vec3D OriginAutowall2 = Vec3D( ViewPoint.x + OriginLeftRightLocal[ side ].x, ViewPoint.y - OriginLeftRightLocal[ side ].y, ViewPoint.z );

					Globals::FreeStandPoints.push_back( OriginAutowall );

					Globals::FreeStandPoints.push_back( OriginAutowall2 );

					if ( Autowall::CanHitFloatingPoint( OriginAutowall, ViewPoint, pPlayerEntity ) )
					{
						if ( side == 0 )
						{
							HitSide1 = true;
							flFreestandAngle[ index ] = 90;
						}
						else if ( side == 1 )
						{
							HitSide2 = true;
							flFreestandAngle[ index ] = -90;
						}

						Autowalled = true;
					}
					else
					{
						for ( int side222 = 0; side222 < 2; side222++ )
						{
							Vec3D OriginAutowall222 = Vec3D( Origin.x + OriginLeftRight[ side222 ].x, Origin.y - OriginLeftRight[ side222 ].y, LagComp::pMatrix[ index ][ 8 ].GetOrigin( ).z );

							Globals::FreeStandPoints.push_back( OriginAutowall222 );

							if ( Autowall::CanHitFloatingPoint( OriginAutowall222, OriginAutowall2, pPlayerEntity ) )
							{
								if ( side222 == 0 )
								{
									HitSide1 = true;
									flFreestandAngle[ index ] = 90;
								}
								else if ( side222 == 1 )
								{
									HitSide2 = true;
									flFreestandAngle[ index ] = -90;
								}

								Autowalled = true;
							}
						}
					}
				}
			}
			

			if ( Autowalled )
			{
				if ( HitSide1 && HitSide2 )
					bUseFreestandAngle[ index ] = false;
				else
				{
					bUseFreestandAngle[ index ] = true;
					flLastFreestandAngle[ index ] = flFreestandAngle[ index ];
				}
			}
		}
	}
}