#pragma once

class Offset
{
public:

	//
	int m_nTickBase = 0x3430;
	int m_flC4Blow = 0x2990;
	int m_angEyeAngles = 0xB36C; // x32dbg: 0xB36C
	int m_iHealth = 0x100;
	int m_lifeState = 0x25F;
	int m_fFlags = 0x104;
	int m_vecVelocity = 0x114;
	int m_flSimulationTime = 0x268;
	int m_bClientSideAnimation = 0x289C; // x32dbg: 0x289C (https://github.com/iisp33dii/CSGO-Netvar-Class-Dump/blob/master/DT_BaseAnimating.txt)
	int m_Collision = 0x320;
	int m_iTeamNum = 0xF4;
	int m_nSurvivalTeam = 0xA3B0;
	int m_aimPunchAngle = 0x302C;
	int m_aimPunchAngleVel = 0x3038;
	int m_viewPunchAngle = 0x3020;
	int m_vecOrigin = 0x138;
	int m_bHasHeavyArmor = 0xB35D; // x32dbg: 0xB35D (https://github.com/iisp33dii/CSGO-Netvar-Class-Dump/blob/master/DT_CSPlayer.txt)
	int m_ArmorValue = 0xB368;
	int m_bHasHelmet = 0xB35C;
	int m_hConstraintEntity = 0x3344; // x32dbg: 0x3344 (https://github.com/iisp33dii/CSGO-Netvar-Class-Dump/blob/master/DT_BasePlayer.txt)
	int m_flPoseParameter = 0x2774; // x32dbg: 0x2774 (https://github.com/iisp33dii/CSGO-Netvar-Class-Dump/blob/master/DT_BaseAnimating.txt)
	int m_flModelScale = 0x2748; // x32dbg: 0x2748
	int m_flLowerBodyYawTarget = 0x3A7C;
	int m_bClientSideFrameReset = 0x26C0; // x32dbg: 0x26C0
	int m_bDrawViewmodel = 0x91; // x32dbg: 0x91
	int m_angRotation = 0x12C; // x32dbg: 0x12C IDK (https://github.com/iisp33dii/CSGO-Netvar-Class-Dump/blob/master/DT_BaseEntity.txt)

	//
	int m_fAccuracyPenalty = 0x3330;
	int m_fLastShotTime = 0x33A8; // x32dbg: 0x33A8
	int m_hActiveWeapon = 0x2EF8;
	int m_flNextPrimaryAttack = 0x3238;
	int m_flNextAttack = 0x2D70;
	int m_iClip1 = 0x3264;

	void Init( )
	{
		// player offsets
		m_nTickBase = g_pNetvars->GetOffset( "DT_BasePlayer", "localdata", "m_nTickBase" );
		m_flC4Blow = g_pNetvars->GetOffset( "DT_PlantedC4", "m_flC4Blow" );
		m_angEyeAngles = g_pNetvars->GetOffset( "DT_CSPlayer", "m_angEyeAngles" );
		m_iHealth = g_pNetvars->GetOffset( "DT_BasePlayer", "m_iHealth" );
		m_lifeState = g_pNetvars->GetOffset( "DT_BasePlayer", "m_lifeState" );
		m_fFlags = g_pNetvars->GetOffset( "DT_BasePlayer", "m_fFlags" );
		m_vecVelocity = g_pNetvars->GetOffset( "DT_BasePlayer", "localdata", "m_vecVelocity[0]" );
		m_flSimulationTime = g_pNetvars->GetOffset( "DT_BaseEntity", "m_flSimulationTime" );
		m_bClientSideAnimation = g_pNetvars->GetOffset( "DT_BaseAnimating", "m_bClientSideAnimation" );
		m_Collision = g_pNetvars->GetOffset( "DT_BaseEntity", "m_Collision" );
		m_iTeamNum = g_pNetvars->GetOffset( "DT_BaseEntity", "m_iTeamNum" );
		m_nSurvivalTeam = g_pNetvars->GetOffset( "DT_CSPlayer", "m_nSurvivalTeam" );
		m_aimPunchAngle = g_pNetvars->GetOffset( "DT_BasePlayer", "localdata", "m_Local", "m_aimPunchAngle" );
		m_viewPunchAngle = g_pNetvars->GetOffset( "DT_BasePlayer", "localdata", "m_Local", "m_viewPunchAngle" );
		m_vecOrigin = g_pNetvars->GetOffset( "DT_BaseEntity", "m_vecOrigin" );
		m_bHasHeavyArmor = g_pNetvars->GetOffset( "DT_CSPlayer", "m_bHasHeavyArmor" );
		m_ArmorValue = g_pNetvars->GetOffset( "DT_CSPlayer", "m_ArmorValue" );
		m_bHasHelmet = g_pNetvars->GetOffset( "DT_CSPlayer", "m_bHasHelmet" );
		m_hConstraintEntity = g_pNetvars->GetOffset( "DT_BasePlayer", "localdata", "m_hConstraintEntity" );
		m_flPoseParameter = g_pNetvars->GetOffset( "DT_BaseAnimating", "m_flPoseParameter" );
		m_flModelScale = g_pNetvars->GetOffset( "DT_BaseAnimating", "m_flModelScale" );
		m_flLowerBodyYawTarget = g_pNetvars->GetOffset( "DT_CSPlayer", "m_flLowerBodyYawTarget" );
		m_aimPunchAngleVel = g_pNetvars->GetOffset( "DT_BasePlayer", "localdata", "m_Local", "m_aimPunchAngleVel" );
		m_bClientSideFrameReset = g_pNetvars->GetOffset( "DT_BaseAnimating", "m_bClientSideFrameReset" );
		m_bDrawViewmodel = g_pNetvars->GetOffset( "DT_BasePlayer", "localdata", "m_Local", "m_bDrawViewmodel" );
		m_angRotation = g_pNetvars->GetOffset( "DT_BaseEntity", "m_angRotation" );//m_angRotation

		//weapon offsets
		m_fAccuracyPenalty = g_pNetvars->GetOffset( "DT_WeaponCSBase", "m_fAccuracyPenalty" );
		m_fLastShotTime = g_pNetvars->GetOffset( "DT_WeaponCSBase", "m_fLastShotTime" );
		m_hActiveWeapon = g_pNetvars->GetOffset( "DT_BaseCombatCharacter", "m_hActiveWeapon" );
		m_flNextPrimaryAttack = g_pNetvars->GetOffset( "DT_BaseCombatWeapon", "LocalActiveWeaponData", "m_flNextPrimaryAttack" );
		m_flNextAttack = g_pNetvars->GetOffset( "DT_BaseCombatCharacter", "bcc_localdata", "m_flNextAttack" );
	//	m_iClip1 = g_pNetvars->GetOffset( "DT_BaseCombatWeapon", "LocalWeaponData" ,"LocalActiveWeaponData", "m_iClip1" );

		// maybe u can still auto calc somehow idk lmao

		Globals::MaxsMins[ 0 ][ 0 ] = Vec3D( -2.7713f, -2.8783f, -3.103f ); // head mins
		Globals::MaxsMins[ 0 ][ 1 ] = Vec3D( 6.955f, 3.5203f, 3.0067f ); // head maxs

		Globals::MaxsMins[ 1 ][ 0 ] = Vec3D( 0.31f, -2.82f, -2.29f ); // neck mins
		Globals::MaxsMins[ 1 ][ 1 ] = Vec3D( 3.18f, 2.26f, 2.20f ); // neck maxs

		Globals::MaxsMins[ 2 ][ 0 ] = Vec3D( -6.42f, -5.7459f, -6.8587f ); // pelvis mins
		Globals::MaxsMins[ 2 ][ 1 ] = Vec3D( 4.5796f, 4.5796f, 6.8373f ); // pelvis maxs

		Globals::MaxsMins[ 3 ][ 0 ] = Vec3D( -4.28f, -4.5796f, -6.3879f ); // belly mins
		Globals::MaxsMins[ 3 ][ 1 ] = Vec3D( 3.21f, 5.885f, 6.2809f ); // belly maxs

		Globals::MaxsMins[ 4 ][ 0 ] = Vec3D( -4.28f, -5.029f, -6.0883f ); // thorax mins
		Globals::MaxsMins[ 4 ][ 1 ] = Vec3D( 3.21f, 5.885f, 5.9813f ); // thorax maxs

		Globals::MaxsMins[ 5 ][ 0 ] = Vec3D( -4.28f, -5.35f, -5.885f ); // lowerchest mins
		Globals::MaxsMins[ 5 ][ 1 ] = Vec3D( 2.9211f, 5.1467f, 5.885f ); // lowerchest maxs

		Globals::MaxsMins[ 6 ][ 0 ] = Vec3D( -6.42f, -4.7459f, -6.9587f ); // upperchest mins
		Globals::MaxsMins[ 6 ][ 1 ] = Vec3D( 7.4796f, 4.5796f, 6.8373f ); // upperchest maxs

		Globals::MaxsMins[ 7 ][ 0 ] = Vec3D( 1.70f, -3.51f, -4.28f ); // rightthigh mins
		Globals::MaxsMins[ 7 ][ 1 ] = Vec3D( 20.70f, 3.20f, 2.00f ); // rightthigh maxs

		Globals::MaxsMins[ 8 ][ 0 ] = Vec3D( 1.70f, -3.70f, -2.00f ); // leftthigh mins
		Globals::MaxsMins[ 8 ][ 1 ] = Vec3D( 20.70f, 3.20f, 4.28f ); // leftthigh maxs

		Globals::MaxsMins[ 9 ][ 0 ] = Vec3D( 2.0758f, -3.21f, -2.8462f ); // rightcalf mins
		Globals::MaxsMins[ 9 ][ 1 ] = Vec3D( 19.26f, 2.675f, 2.247f ); // rightcalf maxs

		Globals::MaxsMins[ 10 ][ 0 ] = Vec3D( 2.0758f, -3.21f, -2.1507f ); // leftcalf mins
		Globals::MaxsMins[ 10 ][ 1 ] = Vec3D( 19.26f, 2.675f, 3.0495f ); // leftcalf maxs

		Globals::MaxsMins[ 11 ][ 0 ] = Vec3D( 0, 0, 0 );
		Globals::MaxsMins[ 11 ][ 1 ] = Vec3D( 0, 0, 0 );

		Globals::MaxsMins[ 12 ][ 0 ] = Vec3D( 0, 0, 0 );
		Globals::MaxsMins[ 12 ][ 1 ] = Vec3D( 0, 0, 0 );

		Globals::MaxsMins[ 13 ][ 0 ] = Vec3D( 0, 0, 0 );
		Globals::MaxsMins[ 13 ][ 1 ] = Vec3D( 0, 0, 0 );

		Globals::MaxsMins[ 14 ][ 0 ] = Vec3D( 0, 0, 0 );
		Globals::MaxsMins[ 14 ][ 1 ] = Vec3D( 0, 0, 0 );

		Globals::MaxsMins[ 15 ][ 0 ] = Vec3D( -2.675f, -3.21f, -2.14f ); // rightupperarm mins
		Globals::MaxsMins[ 15 ][ 1 ] = Vec3D( 12.84f, 3.21f, 2.14f ); // rightupperarm maxs

		Globals::MaxsMins[ 16 ][ 0 ] = Vec3D( -0.f, -2.14f, -2.14f ); // rightforearm mins
		Globals::MaxsMins[ 16 ][ 1 ] = Vec3D( 9.63f, 2.14f, 2.14f ); // rightforearm maxs

		Globals::MaxsMins[ 17 ][ 0 ] = Vec3D( -2.675f, -3.21f, -2.14f ); // leftupperarm mins
		Globals::MaxsMins[ 17 ][ 1 ] = Vec3D( 12.84f, 3.21f, 2.14f ); // leftupperarm maxs

		Globals::MaxsMins[ 18 ][ 0 ] = Vec3D( -0.f, -2.14f, -2.14f ); // leftforearm mins
		Globals::MaxsMins[ 18 ][ 1 ] = Vec3D( 9.63f, 2.14f, 2.14f ); // leftforearm maxs
	}
};

extern Offset * gOffsets;