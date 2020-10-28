#pragma once
#include "Includes.h"
#include "imem.h"
#include "offsets.h"
#pragma warning(disable:4996)
static int S_width = 1920, S_height = 1080;
double GetCrossDistance(double x1, double y1, double x2, double y2);
class Player {
private:
	DWORD_PTR pClientPlayer = 0x0;
public:
	Player() {}
	Player(DWORD_PTR _ptr) {
		this->ptr = _ptr;
		pClientPlayer = this->ptr;
		if (Mem::IsValid((void*)pClientPlayer)) {
			DWORD_PTR AttachedControllable = Mem::Read<DWORD_PTR>(pClientPlayer + m_pAttachedControllable);
			if (Mem::IsValid((void*)AttachedControllable))
			{
				(AttachedControllable == 0) ? this->InVehicle = false : this->InVehicle = true;
				DWORD_PTR ClientSoldier = Mem::Read<DWORD_PTR>(Mem::Read<DWORD_PTR>(pClientPlayer + m_character)) - 8L;
				if (Mem::IsValid((void*)ClientSoldier))
				{
					//if (Mem::Read<INT>(pClientPlayer + m_attachedEntryId) == 0)
					//{
						DWORD_PTR pDynamicPhysicsEntity = Mem::Read<DWORD_PTR>(AttachedControllable + m_pPhysicsEntity);
						if (Mem::IsValid((void*)pDynamicPhysicsEntity))
						{
							DWORD_PTR pPhysicsEntity = Mem::Read<DWORD_PTR>(pDynamicPhysicsEntity + m_EntityTransform);
							this->VehicleTranfsorm = Mem::Read<D3DXMATRIXA16>(pPhysicsEntity + m_Transform);
							this->VehicleAABB = Mem::Read<AxisAlignedBox>(AttachedControllable + m_childrenAABB);
						}
						DWORD_PTR _EntityData = Mem::Read<DWORD_PTR>(AttachedControllable + m_data);
						if (Mem::IsValid((void*)_EntityData))
						{
							DWORD_PTR _NameSid = Mem::Read<DWORD_PTR>(_EntityData + m_NameSid);
							DWORD_PTR pAttachedClient = Mem::Read<DWORD_PTR>(ClientSoldier + m_pPlayer);
							DWORD_PTR p = Mem::Read<DWORD_PTR>(pAttachedClient + m_pAttachedControllable);
							DWORD_PTR p2 = Mem::Read<DWORD_PTR>(p + m_pHealthComponent);
							std::string strName = Mem::ReadString(_NameSid, 20);
							if (strName.length() > 11)
							{
								this->VehicleHealth = Mem::Read<FLOAT>(p2 + m_vehicleHealth);
								this->VehicleMaxHealth = Mem::Read<FLOAT>(_EntityData + m_FrontMaxHealth);
								this->VehicleName = strName.erase(0, 11);
							}
						}
					//}
				}
			}
			DWORD_PTR pClientSoldier = Mem::Read<DWORD_PTR>(pClientPlayer + m_pControlledControllable);
			if (Mem::IsValid((void*)pClientSoldier))
			{
				DWORD_PTR pHealthComponent = Mem::Read<DWORD_PTR>(pClientSoldier + m_pHealthComponent);
				if (Mem::IsValid((void*)pHealthComponent))
				{
					DWORD_PTR pSoldierPrediction = Mem::Read<DWORD_PTR>(pClientSoldier + m_pPredictedController);
					
						this->Health = Mem::Read<FLOAT>(pHealthComponent + m_Health);
						this->MaxHealth = Mem::Read<FLOAT>(pHealthComponent + m_MaxHealth);
						this->Location = Mem::Read<D3DXVECTOR3>(pSoldierPrediction + m_Position);
						this->Visible = 1 - Mem::Read<uint8_t>(pClientSoldier + m_occluded);
						this->PoseType = std::abs(Mem::Read<uint8_t>(pClientSoldier + m_poseType));
						this->Yaw = Mem::Read<FLOAT>(pClientSoldier + m_authorativeYaw);
						this->Name = Mem::ReadString(pClientPlayer + szName,20);
						this->Team = Mem::Read<INT>(pClientPlayer + m_teamId);
						this->isSpectator =(bool)Mem::Read<uint8_t>(pClientPlayer + m_isSpectator);
				}
			}
		}
	}
	std::string Name = "";
	std::string VehicleName = "";
	DWORD_PTR ptr = 0x0;
	FLOAT Health = 0;
	FLOAT MaxHealth = 0;
	FLOAT VehicleHealth = 0;
	FLOAT VehicleMaxHealth = 0;
	FLOAT Yaw = 0;
	INT Team = 0;
	INT PoseType = 0;
	BOOL isSpectator = false;
	BOOL Visible = false;
	BOOL InVehicle = false;
	D3DXVECTOR3 Location = D3DXVECTOR3();
	D3DXMATRIXA16 VehicleTranfsorm;
	AxisAlignedBox VehicleAABB;
	FLOAT Distance = 0;
	bool GetBoneById(int Id, D3DXVECTOR3* _World)
	{
		D3DXVECTOR3 Temp = D3DXVECTOR3();
		DWORD_PTR pClientSoldier = Mem::Read<DWORD_PTR>(pClientPlayer + m_pControlledControllable);
		DWORD_PTR pRagdollComp = Mem::Read<DWORD_PTR>(pClientSoldier + m_ragdollComponent);

		byte ValidTransforms = Mem::Read<BYTE>(pRagdollComp + (m_ragdollTransforms + m_ValidTransforms));
		if (ValidTransforms != 1)
			return false;

		DWORD_PTR pQuatTransform = Mem::Read<DWORD_PTR>(pRagdollComp + (m_ragdollTransforms + m_ActiveWorldTransforms));

		Temp = Mem::Read<D3DXVECTOR3>(pQuatTransform + Id * 0x20);
		_World->x = Temp.x;
		_World->y = Temp.y;
		_World->z = Temp.z;
		return true;
	}
	AxisAlignedBox GetAABB()
	{
		AxisAlignedBox aabb = AxisAlignedBox();
		if (this->PoseType == 0) // standing
		{
			aabb.Min = D3DXVECTOR4(-0.350000f, 0.000000f, -0.350000f, 0);
			aabb.Max = D3DXVECTOR4(0.350000f, 1.700000f, 0.350000f, 0);
		}
		if (this->PoseType == 1) // crouching
		{
			aabb.Min = D3DXVECTOR4(-0.350000f, 0.000000f, -0.350000f, 0);
			aabb.Max = D3DXVECTOR4(0.350000f, 1.150000f, 0.350000f, 0);
		}
		if (this->PoseType == 2) // prone
		{
			aabb.Min = D3DXVECTOR4(-0.350000f, 0.000000f, -0.350000f, 0);
			aabb.Max = D3DXVECTOR4(0.350000f, 0.400000f, 0.350000f, 0);
		}
		return aabb;
	}
	BOOL IsValid() {
		if ((this->Location.x != NULL) &&
			(this->Team != 0) &&
			(this->ptr != NULL)) return true;
		return false;
	}
};
class Manager {
private:
	DWORD_PTR pGR = Mem::Read<DWORD_PTR>(OFFSET_GAMERENDERER);
	DWORD_PTR pRenderView = Mem::Read<DWORD_PTR>(pGR + m_pRenderView);
public:
	Player LocalPlayer = NULL;
	std::vector<Player> Players = std::vector<Player>();
	Player ClosestVisibleEnemy = NULL;
	Player ClosestCrosshairEnemy = NULL;
	Player SpectatorList = NULL;
	INT Total = 0;

	Manager(int width, int height) {
		S_width = width;
		S_height = height;

		DWORD_PTR pStaticGameContext = Mem::Read<DWORD_PTR>(OFFSET_CLIENTGAMECONTEXT);
		if (!Mem::IsValid((void*)pStaticGameContext)) return;
		
		DWORD_PTR pPlayerManager = Mem::Read<DWORD_PTR>(pStaticGameContext + m_pPlayerManager);
		if (!Mem::IsValid((void*)pPlayerManager)) return;

		this->LocalPlayer = Player(Mem::Read<DWORD_PTR>(pPlayerManager + m_pLocalPlayer));
		for (int i = 0; i < 64; i++) {
			Player player(Mem::Read<DWORD_PTR>(pPlayerManager + m_ppPlayer + (i * 0x8)));
			if (player.ptr != 0);// break;
			if (player.IsValid() && player.ptr != this->LocalPlayer.ptr) {
				player.Distance = D3DXVec3Length(&(this->LocalPlayer.Location - player.Location));
				this->Players.push_back(player);
			}
		}
		float closestcrosshair = FLT_MAX;
		for (int i = 0; i < this->Players.size(); i++) {
				if (this->Players[i].Visible || this->Players[i].InVehicle) {
					D3DXVECTOR3 out = this->Players[i].Location;
					if (WorldToScreen(out)) {
						float crossdis = GetCrossDistance(out.x, out.y, S_width / 2, S_height / 2);
						if ((crossdis < closestcrosshair)) {
							if ((((this->LocalPlayer.Team == this->Players[i].Team) && g_Options.AIMBOT.aimbotteam) || ((this->LocalPlayer.Team != this->Players[i].Team) && g_Options.AIMBOT.aimbotenemy))) {

								if (this->Players[i].Health > 0) {
									ClosestCrosshairEnemy = this->Players[i];
									closestcrosshair = crossdis;
								}
							}
						}
					}
				}
			
		}
		float closestdis = FLT_MAX;
		for (int i = 0; i < this->Players.size(); i++) {
			if ((this->Players[i].Distance < closestdis)) {
				if (this->Players[i].Team != this->LocalPlayer.Team) {
					ClosestVisibleEnemy = this->Players[i];
					closestdis = this->Players[i].Distance;
				}
			}
		}
		for (int i = 0; i < this->Players.size(); i++) {
			if (this->Players[i].isSpectator == true) {
					SpectatorList = this->Players[i];
			}
		}

		for (int i = 0; i < this->Players.size(); i++) {
				Total = this->Players.size();
			
		}

	}
	bool WorldToScreen(D3DXVECTOR3& vLocVec4)
	{
		D3DXMATRIXA16 ViewProj = Mem::Read<D3DXMATRIXA16>(pRenderView + m_viewProj);
		float mX = (S_width) / 2;
		float mY = (S_height) / 2;

		float w =
			ViewProj(0, 3) * vLocVec4.x +
			ViewProj(1, 3) * vLocVec4.y +
			ViewProj(2, 3) * vLocVec4.z +
			ViewProj(3, 3);

		if (w < 0.65f)
		{
			vLocVec4.z = w;
			return false;
		}

		float x =
			ViewProj(0, 0) * vLocVec4.x +
			ViewProj(1, 0) * vLocVec4.y +
			ViewProj(2, 0) * vLocVec4.z +
			ViewProj(3, 0);

		float y =
			ViewProj(0, 1) * vLocVec4.x +
			ViewProj(1, 1) * vLocVec4.y +
			ViewProj(2, 1) * vLocVec4.z +
			ViewProj(3, 1);

		vLocVec4.x = (mX + mX * x / w);
		vLocVec4.y = (mY - mY * y / w);
		vLocVec4.z = w;
		if (vLocVec4.x > S_width || vLocVec4.y > S_height || vLocVec4.x < 0 || vLocVec4.y < 0)
			return false;
		return true;
	}
	D3DXVECTOR3 CalcSoldierFuturePos(D3DXVECTOR3 InVec)
	{
		D3DXVECTOR3 NewPos;
		if (WorldToScreen(InVec)) {
			NewPos.x = InVec.x;
			NewPos.y = InVec.y;
			NewPos.z = InVec.z;
		}
		else
		{
			NewPos.x = 0;
			NewPos.y = 0;
			NewPos.z = 0;
		}
		return NewPos;
	}
};

D3DXVECTOR3 Multiply(D3DXVECTOR3 vector, D3DXMATRIXA16 mat)
{
	return  D3DXVECTOR3(mat._11 * vector.x + mat._21 * vector.y + mat._31 * vector.z,
		mat._21 * vector.x + mat._22 * vector.y + mat._32 * vector.z,
		mat._13 * vector.x + mat._23 * vector.y + mat._33 * vector.z);
}
class MOUSE
{
private:
	// coordinate variable
	POINT point;
public:
	//Mobile class function
	void Move(int x, int y);
	void RelativeMove(int cx, int cy);
	void SavePos();
	void RestorePos();
	//Lock enabled class
	void Lock();
	void Unlock();

};

// Move the mouse to the absolute position(X coordinate, Y coordinate)
void MOUSE::Move(int x, int y)
{
	this->point.x = x;
	this->point.y = y;
	::SetCursorPos(x, y);
}

// Move the mouse to the relative position(X displacement, Y displacement)
void MOUSE::RelativeMove(int cx, int cy)
{
	::ShowCursor(0);
	::GetCursorPos(&this->point);
	this->point.x += cx;
	this->point.y += cy;
	::SetCursorPos(this->point.x, this->point.y);
}

// Save the current location()
void MOUSE::SavePos()
{
	::GetCursorPos(&this->point);
}

// Restore mouse position()
void MOUSE::RestorePos()
{
	::SetCursorPos(this->point.x, this->point.y);
}

// Lock the mouse()
void MOUSE::Lock()
{
	POINT pt;
	RECT rt;
	::GetCursorPos(&pt);
	rt.left = rt.right = pt.x;
	rt.top = rt.bottom = pt.y;
	rt.right++;
	rt.bottom++;
	::ClipCursor(&rt);

}

//Unlock the mouse ()
void MOUSE::Unlock()
{
	::ClipCursor(NULL);
}
class Inputs {
private:
	static DWORD SendScanCode(WORD scan, BOOL up)
	{
		INPUT inp = { 0 };
		::ZeroMemory(&inp, sizeof(INPUT));
		inp.type = INPUT_KEYBOARD;
		inp.ki.wScan = scan;
		inp.ki.dwFlags = KEYEVENTF_SCANCODE | (up ? KEYEVENTF_KEYUP : 0);
		return SendInput(1, &inp, sizeof(inp)) ? NO_ERROR : GetLastError();
	}
public:
	static bool mouse_move(int x, int y)
	{
		INPUT input = { 0 };
		::ZeroMemory(&input, sizeof(INPUT));
		input.type = INPUT_MOUSE;
		input.mi.mouseData = 0;
		input.mi.time = 0;
		input.mi.dx = x;
		input.mi.dy = y;
		input.mi.dwFlags = MOUSEEVENTF_MOVE;
		SendInput(1, &input, sizeof(input));
		return true;
	}
	static DWORD SendVirtualKey(UINT vk, BOOL up)
	{
		UINT scan = MapVirtualKey(vk, MAPVK_VK_TO_VSC);
		return scan ? SendScanCode(scan, up) : ERROR_NO_UNICODE_TRANSLATION;
	}
	bool static keydown(int key)
	{
		return (GetAsyncKeyState(key) & 0x8000) != 0;
	}
};
float Distance3D(D3DXVECTOR3  v1, D3DXVECTOR3 v2)
{
	float x_d = (v2.x - v1.x);
	float y_d = (v2.y - v1.y);
	float z_d = (v2.z - v1.z);
	return sqrt((x_d * x_d) + (y_d * y_d) + (z_d * z_d));
}
float Distance(D3DXVECTOR3 EntityPos, D3DXVECTOR3 PlayerPos)
{
	float Dist = sqrt((PlayerPos.x - EntityPos.x) * (PlayerPos.x - EntityPos.x)
		+ (PlayerPos.y - EntityPos.y) * (PlayerPos.y - EntityPos.y)
		+ (PlayerPos.z - EntityPos.z) * (PlayerPos.z - EntityPos.z));
	return Dist * 0.01905f;
}
double GetCrossDistance(double x1, double y1, double x2, double y2)
{
	return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}
void AimAtPosV2(float x, float y, float speed,float lock, bool smooth)
{
	MOUSE move = MOUSE();

	int ScreenCenterX = S_width / 2, ScreenCenterY = S_height / 2;

	float AimSpeed = (float)speed;
	float TargetX = 0;
	float TargetY = 0;

	//X Axis
	if (x != 0)
	{
		if (x > ScreenCenterX)
		{
			TargetX = -(ScreenCenterX - x);
			TargetX /= AimSpeed;
			TargetX += lock;
			if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
		}

		if (x < ScreenCenterX)
		{
			TargetX = x - ScreenCenterX;
			TargetX /= AimSpeed;
			TargetX -= lock;
			if (TargetX + ScreenCenterX < 0) TargetX = 0;
		}
	}

	//Y Axis

	if (y != 0)
	{
		if (y > ScreenCenterY)
		{
			TargetY = -(ScreenCenterY - y);
			TargetY /= AimSpeed;
			TargetY += lock;
			if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
		}

		if (y < ScreenCenterY)
		{
			TargetY = y - ScreenCenterY;
			TargetY /= AimSpeed;
			TargetY -= lock;
			if (TargetY + ScreenCenterY < 0) TargetY = 0;
		}
	}

	if (!smooth)
	{
		move.RelativeMove((int)TargetX, (int)(TargetY));
		return;
	}

	TargetX /= 10;
	TargetY /= 10;

	if (abs(TargetX) < 1)
	{
		if (TargetX > 0)
		{
			TargetX = 1;
		}
		if (TargetX < 0)
		{
			TargetX = -1;
		}
	}
	if (abs(TargetY) < 1)
	{
		if (TargetY > 0)
		{
			TargetY = 1;
		}
		if (TargetY < 0)
		{
			TargetY = -1;
		}
	}
	move.RelativeMove((int)TargetX, (int)(TargetY));

}

