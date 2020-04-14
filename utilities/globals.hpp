#pragma once

struct BulletHit
{
public:
	Vec3D ShootPos;
	Vec3D Hit;
	float timeShot;
	bool HitPlayer = false;
};

namespace Globals
{
	extern bool UpdateAnimations;
	extern Vec3D MaxsMins[ 19 ][ 2 ];
	extern std::deque<Vec3D> AimPoints;
	extern std::deque<Vec3D> FreeStandPoints;
	extern bool bSendPacket;
	extern Vec3D AimPunchAng;
	extern CViewSetup ViewSetup;
	extern std::deque<BulletHit> Bullet;
	extern matrix3x4_t mLocalMatrix[ 128 ];
	extern int iTickBaseShift;
	extern int iMissedShots[ 65 ];
	extern bool bShot;
	extern bool bHit;


	typedef void( __cdecl* MsgFn )( const char* msg, va_list );
	static void Msg( const char* msg, ... ) // unknowncheats.me unknowncheats.me unknowncheats.me unknowncheats.me unknowncheats.me unknowncheats.me unknowncheats.me unknowncheats.me unknowncheats.me unknowncheats.me unknowncheats.me unknowncheats.me unknowncheats.me unknowncheats.me unknowncheats.me unknowncheats.me unknowncheats.me unknowncheats.me unknowncheats.me unknowncheats.me unknowncheats.me unknowncheats.me unknowncheats.me unknowncheats.me 
	{
		if ( msg == nullptr )
			return; //If no string was passed, or it was null then don't do anything
		static MsgFn fn = ( MsgFn ) GetProcAddress( GetModuleHandle( "tier0.dll" ), "Msg" ); //This gets the address of export "Msg" in the dll "tier0.dll". The static keyword means it's only called once and then isn't called again (but the variable is still there)
		char buffer[ 989 ];
		va_list list; //Normal varargs stuff http://stackoverflow.com/questions/10482960/varargs-to-printf-all-arguments
		va_start( list, msg );
		vsprintf( buffer, msg, list );
		va_end( list );
		fn( buffer, list ); //Calls the function, we got the address above.
	}
}