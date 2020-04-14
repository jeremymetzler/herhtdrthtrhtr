#include "../../features.hpp"

// shitty anti aim as usual

/*
could have made better anti aim just rly dont feel like it ngl
*/

Vec3D vRealAngle = Vec3D(0,0,0);
bool FreestandingSide;

void NewFreeStanding( CUserCmd* pCmd, CBaseEntity* pLocal ) // idk wtf i was trying todo here just ignore it
{
	static std::deque<Vec3D> EnemyShootPoses;
	static int Size = 0;

	Vec3D ViewAngle;
	Interfaces::Engine->GetViewAngles( ViewAngle );

	static bool BlackList[ 8 ];

	if ( Size < 0 )
	{

		for ( int Degree = 1; Degree < 8; Degree++ )
		{
			if ( !BlackList[ Degree ] )
			{
				float Angle = Math::m_flNormalizeYaw( ViewAngle.y + Math::m_flNormalizeYaw( Degree * 45.f ) );
				pCmd->viewangles.y = Angle;
			}
		}

		EnemyShootPoses.clear( );

		for ( int index = 1; index < MAX_NETWORKID_LENGTH; ++index )
		{
			CBaseEntity* pPlayerEntity = Interfaces::EntityList->m_pGetClientEntity( index );

			if ( !pPlayerEntity
				|| !pPlayerEntity->m_bIsPlayer( )
				|| !pPlayerEntity->m_bIsAlive( )
				|| pPlayerEntity->IsDormant( )
				|| pLocal->GetTeam( ) == pPlayerEntity->GetTeam( ) )
				continue;

			EnemyShootPoses.push_back( pPlayerEntity->m_vecOrigin( ) + Vec3D( 0, 0, 85.f ) );
		}

		Size = EnemyShootPoses.size( ) - 1;

		for ( auto &OK : BlackList )
			OK = false;
	}

	if ( Size >= 0 )
	{
		for ( int Degree = 1; Degree < 8; Degree++ )
		{
			float Angle = Math::m_flNormalizeYaw( ViewAngle.y + Math::m_flNormalizeYaw( Degree * 45.f ) );
			Vec3D Forward;
			Math::AngleVectors( Vec3D( 0.f, Math::m_flNormalizeYaw( Angle ), 0.f ), &Forward );
			Math::NormalizeAngles( Forward );

			Vec3D Points = Vec3D( pLocal->GetEyePosition( ) + ( Forward * 45 ) );

			if ( Autowall::CanHitFloatingPoint( Points, EnemyShootPoses.at( Size ) ) )
			{
				BlackList[ Degree ] = true;
			}
		}
	}

	Size -= 1;
}

void FreeStanding( CUserCmd* pCmd, CBaseEntity* pLocal) // should have rewritten
{
	static float FinalAngle;
	bool bside1 = false;
	bool bside2 = false;
	bool autowalld = false;

//	float tempdis = 999999999.f;
	Vec2D LocalOrg = Vec2D( pLocal->m_vecOrigin( ).x, pLocal->m_vecOrigin( ).y );

//	float closeYaw = 999.f;

	for ( int index = 1; index < MAX_NETWORKID_LENGTH; ++index )
	{
		CBaseEntity* pPlayerEntity = Interfaces::EntityList->m_pGetClientEntity( index );

		if ( !pPlayerEntity
			|| !pPlayerEntity->m_bIsPlayer( )
			|| !pPlayerEntity->m_bIsAlive( )
			|| pPlayerEntity->IsDormant( )
			|| pLocal->GetTeam( ) == pPlayerEntity->GetTeam( ) )
			continue;

		Vec2D EnemyOrg = Vec2D( pPlayerEntity->m_vecOrigin( ).x, pPlayerEntity->m_vecOrigin( ).y );

	//	if ( tempdis > fabs( Math::Distance2D( EnemyOrg, LocalOrg ) ) )
	//	{
	//		closeYaw = Math::m_flNormalizeYaw( Math::CalcAngle( pLocal->m_vecOrigin( ), pPlayerEntity->m_vecOrigin( ) ).y );
	//		tempdis = fabs( Math::Distance2D( EnemyOrg, LocalOrg ) );
	//	}

		float angToLocal = Math::CalcAngle( pLocal->m_vecOrigin( ), pPlayerEntity->m_vecOrigin( ) ).y;
		Vec3D ViewPoint = pPlayerEntity->m_vecOrigin( ) + Vec3D( 0, 0, 90 );

		Vec2D Side1 = Vec2D(( 45 * sin( Math::m_flDegToRad( angToLocal ) ) ),( 45 * cos( Math::m_flDegToRad( angToLocal ) ) ) );
		Vec2D Side2 = Vec2D(( 45 * sin( Math::m_flDegToRad( angToLocal + 180 ) ) ) ,( 45 * cos( Math::m_flDegToRad( angToLocal + 180 ) ) ) );

		Vec2D Side3 = Vec2D(( 50 * sin( Math::m_flDegToRad( angToLocal ) ) ),( 50 * cos( Math::m_flDegToRad( angToLocal ) ) ) );
		Vec2D Side4 = Vec2D(( 50 * sin( Math::m_flDegToRad( angToLocal + 180 ) ) ) ,( 50 * cos( Math::m_flDegToRad( angToLocal + 180 ) ) ) );

		Vec3D Origin = pLocal->m_vecOrigin( );

		Vec2D OriginLeftRight[ ] = { Vec2D( Side1.x, Side1.y ), Vec2D( Side2.x, Side2.y ) };

		Vec2D OriginLeftRightLocal[ ] = { Vec2D( Side3.x, Side3.y ), Vec2D( Side4.x, Side4.y ) };

		for ( int side = 0; side < 2; side++ )
		{
			Vec3D OriginAutowall = Vec3D( Origin.x + OriginLeftRight[ side ].x,  Origin.y - OriginLeftRight[ side ].y , Origin.z + 80 );
			Vec3D OriginAutowall2 = Vec3D( ViewPoint.x + OriginLeftRightLocal[ side ].x,  ViewPoint.y - OriginLeftRightLocal[ side ].y , ViewPoint.z );

			if ( Autowall::CanHitFloatingPoint( OriginAutowall, ViewPoint, pPlayerEntity ) )
			{
				if ( side == 0 )
				{
					FreestandingSide = true;
					bside1 = true;
					FinalAngle = angToLocal + 90;
				}
				else if ( side == 1 )
				{
					FreestandingSide = false;
					bside2 = true;
					FinalAngle = angToLocal - 90;
				}
				autowalld = true;
			}
			else
			{
				for ( int side222 = 0; side222 < 2; side222++ )
				{
					Vec3D OriginAutowall222 = Vec3D( Origin.x + OriginLeftRight[ side222 ].x,  Origin.y - OriginLeftRight[ side222 ].y , Origin.z + 80 );

					if ( Autowall::CanHitFloatingPoint( OriginAutowall222, OriginAutowall2, pPlayerEntity ) )
					{
						if ( side222 == 0 )
						{
							FreestandingSide = true;
							bside1 = true;
							FinalAngle = angToLocal + 90;
						}
						else if ( side222 == 1 )
						{
							FreestandingSide = false;
							bside2 = true;
							FinalAngle = angToLocal - 90;
						}
						autowalld = true;
					}
				}
			}
		}
	}

	if ( !autowalld || ( bside1 && bside2 ) )
	{
	//	if ( closeYaw != 999.f && Menu.Config.AtTarget )
	//		g::pCmd->viewangles.y = g_Math.NormalizeYaw( closeYaw + SentYaw );
	//	else
		//	g::pCmd->viewangles.y += SentYaw;
		pCmd->viewangles.y += 180.f;
	}
	else
		pCmd->viewangles.y = Math::m_flNormalizeYaw( FinalAngle );
}


void Rage::AntiAim( CUserCmd* pCmd )
{
	if ( !Interfaces::Engine->IsInGame( ) || !Interfaces::Engine->IsConnected( ) )
		return;

	if ( Menu::Config.legitback )
		return;

	if ( !Menu::Config.antiaim )
		return;

	auto pLocalEnt = Interfaces::User( );

	if ( !pLocalEnt || !pLocalEnt->m_bIsAlive( ) )
		return;

	auto pWeapon = pLocalEnt->GetActiveWeapon( );

	if ( !pWeapon )
		return;

	if ( pWeapon->IsKnifeorNade( ) )
	{
		Globals::bSendPacket = true;
		return;
	}

	if ( pCmd->buttons & IN_ATTACK )
		return;

	if ( pCmd->buttons & IN_USE )
	{
		Globals::bSendPacket = true;
		return;
	}

	pCmd->viewangles.x = 89.99999f;


	FreeStanding( pCmd, pLocalEnt );

	bool bCommandSwitch = pCmd->command_number % 2;

	if ( Menu::Config.desync ) // ew wtf
	{
		if ( Menu::Config.desyncmove < 2 )
		{
			float &flMoveType = ((Menu::Config.desyncmove) ? pCmd->sidemove : pCmd->forwardmove);

			if ( !( GetAsyncKeyState( VK_SPACE ) ) && !( pCmd->buttons & IN_MOVELEFT ) && !( pCmd->buttons & IN_MOVELEFT ) && !( pCmd->buttons & IN_MOVERIGHT ) && !( pCmd->buttons & IN_FORWARD ) && !( pCmd->buttons & IN_BACK ) )
				if ( pCmd->buttons & IN_DUCK )
					( bCommandSwitch ) ? flMoveType += -4.6 : flMoveType += 4.6;
				else
					( bCommandSwitch ) ? flMoveType += -1.6 : flMoveType += 1.6;
		}
		else
		{
			static bool bSwitch = false;
			if ( bCommandSwitch )
				bSwitch = !bSwitch;
			float &flMoveType = ( ( bSwitch ) ? pCmd->sidemove : pCmd->forwardmove );
			if ( !( GetAsyncKeyState( VK_SPACE ) ) && !( pCmd->buttons & IN_MOVELEFT ) && !( pCmd->buttons & IN_MOVELEFT ) && !( pCmd->buttons & IN_MOVERIGHT ) && !( pCmd->buttons & IN_FORWARD ) && !( pCmd->buttons & IN_BACK ) )
				if ( pCmd->buttons & IN_DUCK )
					( bCommandSwitch ) ? flMoveType += -4.6 : flMoveType += 4.6;
				else
					( bCommandSwitch ) ? flMoveType += -1.6 : flMoveType += 1.6;
		}

		if ( !Globals::bSendPacket )
		{
			pCmd->viewangles.y += Menu::Config.desyncoffset;
		}
	}
}