#pragma once
#define OFFSET_CLIENTGAMECONTEXT 0x142670d80
#define OFFSET_GAMERENDERER 0x142672378
#define OFFSET_LOCALWEAPONS 0x1423B2EC8
#define m_pRenderView 0x60
#define m_viewProj 0x0420
#define m_pHealthComponent 0x0140
#define m_authorativeYaw 0x04D8
#define m_poseType 0x04F0
#define m_pPredictedController 0x0490
#define m_ragdollComponent 0x0580
#define m_ragdollTransforms 0x0088
#define m_ValidTransforms 0x0040
#define m_ActiveWorldTransforms 0x0028
#define m_occluded 0x05B1
#define m_Health 0x0020
#define m_MaxHealth 0x0024
#define m_vehicleHealth 0x0038
#define m_Position 0x0030
#define m_pLocalPlayer 0x540
#define m_ppPlayer 0x548
#define szName 0x40
#define m_teamId 0x13CC
#define m_character 0x14B0
#define m_pPhysicsEntity 0x0238
#define m_EntityTransform 0xA0
#define m_pAttachedControllable 0x14C0
#define m_pControlledControllable 0x14D0
#define m_pPlayerManager 0x60
#define m_Transform 0x00
#define m_childrenAABB 0x0250
#define m_data 0x0030
#define m_NameSid 0x0248
#define m_FrontMaxHealth 0x148
#define m_pPlayer 0x01E0
#define m_attachedEntryId 0x14C8
#define m_isSpectator 0x13C9
enum BONES
{
    BONE_HEAD = 104,
    BONE_NECK = 142,
    BONE_SPINE2 = 7,
    BONE_SPINE1 = 6,
    BONE_SPINE = 5,
    BONE_LEFTSHOULDER = 9,
    BONE_RIGHTSHOULDER = 109,
    BONE_LEFTELBOWROLL = 11,
    BONE_RIGHTELBOWROLL = 111,
    BONE_LEFTHAND = 15,
    BONE_RIGHTHAND = 115,
    BONE_LEFTKNEEROLL = 188,
    BONE_RIGHTKNEEROLL = 197,
    BONE_LEFTFOOT = 184,
    BONE_RIGHTFOOT = 198
};
class AxisAlignedBox
{
public:
    D3DXVECTOR4 Min;
    D3DXVECTOR4 Max;
};