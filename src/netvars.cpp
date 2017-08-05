
#include "xorstring.hpp"

/*
a * entity.cpp
 *
 *  Created on: Oct 6, 2016
 *      Author: nullifiedcat
 */

#include "copypasted/Netvar.h"
#include "common.h"

NetVars netvar;

void NetVars::Init() {
	this->iFlags = gNetvars.get_offset(XStr("DT_BasePlayer"), XStr("m_fFlags"));
	this->iHealth = gNetvars.get_offset(XStr("DT_BasePlayer"), XStr("m_iHealth"));
	this->iLifeState = gNetvars.get_offset(XStr("DT_BasePlayer"), XStr("m_lifeState"));
	this->iTeamNum = gNetvars.get_offset(XStr("DT_BaseEntity"), XStr("m_iTeamNum"));
	this->vViewOffset = gNetvars.get_offset(XStr("DT_BasePlayer"), XStr("localdata"), XStr("m_vecViewOffset[0]"));
	this->hActiveWeapon = gNetvars.get_offset(XStr("DT_BaseCombatCharacter"), XStr("m_hActiveWeapon"));
	this->hMyWeapons = gNetvars.get_offset(XStr("DT_BaseCombatCharacter"), XStr("m_hMyWeapons"));
	this->iHitboxSet = gNetvars.get_offset(XStr("DT_BaseAnimating"), XStr("m_nHitboxSet"));
	this->vVelocity = gNetvars.get_offset(XStr("DT_BasePlayer"), XStr("localdata"), XStr("m_vecVelocity[0]"));
	this->movetype = gNetvars.get_offset(XStr("DT_BaseEntity"), XStr("movetype"));
	this->m_iAmmo = gNetvars.get_offset(XStr("DT_BasePlayer"), XStr("localdata"), XStr("m_iAmmo"));
	this->m_Collision = gNetvars.get_offset(XStr("DT_BaseEntity"), XStr("m_Collision"));
	m_flSimulationTime = gNetvars.get_offset(XStr("DT_BaseEntity"), XStr("m_flSimulationTime"));
	IF_GAME (IsTF2()) {
		res_iTeam = gNetvars.get_offset(XStr("DT_TFPlayerResource"), XStr("baseclass"), XStr("m_iTeam"));
		this->res_iMaxBuffedHealth = gNetvars.get_offset(XStr("DT_TFPlayerResource"), XStr("m_iMaxBuffedHealth"));
		m_angEyeAngles = gNetvars.get_offset(XStr("DT_TFPlayer"), XStr("tfnonlocaldata"), XStr("m_angEyeAngles[0]"));
		m_angEyeAnglesLocal = gNetvars.get_offset(XStr("DT_TFPlayer"), XStr("tflocaldata"), XStr("m_angEyeAngles[0]"));
		this->bGlowEnabled = gNetvars.get_offset(XStr("DT_TFPlayer"), XStr("m_bGlowEnabled"));
		//this->iDecapitations = gNetvars.get_offset(XStr("DT_TFPlayer"), XStr("m_Shared"), XStr("m_iDecapitations"));
		//this->flReloadPriorNextFire = gNetvars.get_offset(XStr("DT_TFWeaponBase"), XStr("LocalActiveTFWeaponData"), XStr("m_flReloadPriorNextFire"));
		//this->flObservedCritChance = gNetvars.get_offset(XStr("DT_TFWeaponBase"), XStr("LocalActiveTFWeaponData"), XStr("m_flObservedCritChance"));
		this->iItemDefinitionIndex = gNetvars.get_offset(XStr("DT_EconEntity"), XStr("m_AttributeManager"), XStr("m_Item"), XStr("m_iItemDefinitionIndex"));
		this->AttributeList = gNetvars.get_offset(XStr("DT_EconEntity"), XStr("m_AttributeManager"), XStr("m_Item"), XStr("m_AttributeList")); // hmmm
		this->flChargeBeginTime = gNetvars.get_offset(XStr("DT_WeaponPipebombLauncher"), XStr("PipebombLauncherLocalData"), XStr("m_flChargeBeginTime"));
		this->flLastFireTime = gNetvars.get_offset(XStr("DT_TFWeaponBase"), XStr("LocalActiveTFWeaponData"), XStr("m_flLastFireTime"));
		this->bDistributed = gNetvars.get_offset(XStr("DT_CurrencyPack"), XStr("m_bDistributed"));
		this->_condition_bits = gNetvars.get_offset(XStr("DT_TFPlayer"), XStr("m_Shared"), XStr("m_ConditionList"), XStr("_condition_bits"));
		this->m_flStealthNoAttackExpire = gNetvars.get_offset(XStr("DT_TFPlayer"), XStr("m_Shared"), XStr("tfsharedlocaldata"), XStr("m_flStealthNoAttackExpire"));
		this->m_nChargeResistType = gNetvars.get_offset(XStr("DT_WeaponMedigun"), XStr("m_nChargeResistType"));
		this->m_flChargeLevel = gNetvars.get_offset(XStr("DT_WeaponMedigun"), XStr("NonLocalTFWeaponMedigunData"), XStr("m_flChargeLevel"));
		m_bFeignDeathReady = gNetvars.get_offset(XStr("DT_TFPlayer"), XStr("m_Shared"), XStr("m_bFeignDeathReady"));
		m_bCarryingObject = gNetvars.get_offset(XStr("DT_TFPlayer"), XStr("m_Shared"), XStr("m_bCarryingObject"));
		m_nSequence = gNetvars.get_offset(XStr("DT_BaseAnimating"), XStr("m_nSequence"));
		m_iTauntIndex = gNetvars.get_offset(XStr("DT_TFPlayer"), XStr("m_Shared"), XStr("m_iTauntIndex"));
		m_iTauntConcept = gNetvars.get_offset(XStr("DT_TFPlayer"), XStr("m_Shared"), XStr("m_iTauntConcept"));
	}
	this->res_iScore = gNetvars.get_offset(XStr("DT_TFPlayerResource"), XStr("baseclass"), XStr("m_iScore"));
	IF_GAME (IsTF()) {
		this->res_iMaxHealth = gNetvars.get_offset(XStr("DT_TFPlayerResource"), XStr("m_iMaxHealth"));
		this->res_iPlayerClass = gNetvars.get_offset(XStr("DT_TFPlayerResource"), XStr("m_iPlayerClass"));
		this->m_bReadyToBackstab = gNetvars.get_offset(XStr("DT_TFWeaponKnife"), XStr("m_bReadyToBackstab"));
		this->m_bDucked = gNetvars.get_offset(XStr("DT_TFPlayer"), XStr("localdata"), XStr("m_Local"), XStr("m_bDucked"));
		this->m_flDuckTimer = gNetvars.get_offset(XStr("DT_TFPlayer"), XStr("m_Shared"), XStr("m_flDuckTimer"));
		this->iCond = gNetvars.get_offset(XStr("DT_TFPlayer"), XStr("m_Shared"), XStr("m_nPlayerCond"));
		this->iCond1 = gNetvars.get_offset(XStr("DT_TFPlayer"), XStr("m_Shared"), XStr("m_nPlayerCondEx"));
		this->iCond2 = gNetvars.get_offset(XStr("DT_TFPlayer"), XStr("m_Shared"), XStr("m_nPlayerCondEx2"));
		this->iCond3 = gNetvars.get_offset(XStr("DT_TFPlayer"), XStr("m_Shared"), XStr("m_nPlayerCondEx3"));
		this->iClass = gNetvars.get_offset(XStr("DT_TFPlayer"), XStr("m_PlayerClass"), XStr("m_iClass"));
		this->flChargedDamage = gNetvars.get_offset(XStr("DT_TFSniperRifle"), XStr("SniperRifleLocalData"), XStr("m_flChargedDamage"));
		this->iUpgradeLevel = gNetvars.get_offset(XStr("DT_BaseObject"), XStr("m_iUpgradeLevel"));
		this->iPipeType = gNetvars.get_offset(XStr("DT_TFProjectile_Pipebomb"), XStr("m_iType"));
		this->iBuildingHealth = gNetvars.get_offset(XStr("DT_BaseObject"), XStr("m_iHealth"));
		this->iBuildingMaxHealth = gNetvars.get_offset(XStr("DT_BaseObject"), XStr("m_iMaxHealth"));
		this->iReloadMode = gNetvars.get_offset(XStr("DT_TFWeaponBase"), XStr("m_iReloadMode"));
		this->Rocket_iDeflected = gNetvars.get_offset(XStr("DT_TFBaseRocket"), XStr("m_iDeflected"));
		this->Grenade_iDeflected = gNetvars.get_offset(XStr("DT_TFWeaponBaseGrenadeProj"), XStr("m_iDeflected"));
		this->nForceTauntCam = gNetvars.get_offset(XStr("DT_TFPlayer"), XStr("m_nForceTauntCam"));
		this->Rocket_bCritical = gNetvars.get_offset(XStr("DT_TFProjectile_Rocket"), XStr("m_bCritical"));
		this->Grenade_bCritical = gNetvars.get_offset(XStr("DT_TFWeaponBaseGrenadeProj"), XStr("m_bCritical"));
		this->angEyeAngles = gNetvars.get_offset(XStr("DT_TFPlayer"), XStr("tfnonlocaldata"), XStr("m_angEyeAngles[0]"));
		this->iWeaponState = gNetvars.get_offset(XStr("DT_WeaponMinigun"), XStr("m_iWeaponState"));
		this->flChargeLevel = gNetvars.get_offset(XStr("DT_WeaponMedigun"), XStr("NonLocalTFWeaponMedigunData"), XStr("m_flChargeLevel"));
		this->bChargeRelease = gNetvars.get_offset(XStr("DT_WeaponMedigun"), XStr("m_bChargeRelease"));
	}
	IF_GAME (IsTF2C()) {
		this->iCritMult = gNetvars.get_offset(XStr("DT_TFPlayer"), XStr("m_Shared"), XStr("m_iCritMult"));
		this->bRespawning = gNetvars.get_offset(XStr("DT_WeaponSpawner"), XStr("m_bRespawning"));
	}
	this->flNextAttack = gNetvars.get_offset(XStr("DT_BaseCombatCharacter"), XStr("bcc_localdata"), XStr("m_flNextAttack"));
	this->flNextPrimaryAttack = gNetvars.get_offset(XStr("DT_BaseCombatWeapon"), XStr("LocalActiveWeaponData"), XStr("m_flNextPrimaryAttack"));
	this->iNextThinkTick = gNetvars.get_offset(XStr("DT_BaseCombatWeapon"), XStr("LocalActiveWeaponData"), XStr("m_nNextThinkTick"));
	this->nTickBase = gNetvars.get_offset(XStr("DT_BasePlayer"), XStr("localdata"), XStr("m_nTickBase"));
	this->vecPunchAngle = gNetvars.get_offset(XStr("DT_BasePlayer"), XStr("localdata"), XStr("m_Local"), XStr("m_vecPunchAngle"));
	this->vecPunchAngleVel = gNetvars.get_offset(XStr("DT_BasePlayer"), XStr("localdata"), XStr("m_Local"), XStr("m_vecPunchAngleVel"));
	this->hThrower = gNetvars.get_offset(XStr("DT_BaseGrenade"), XStr("m_hThrower"));
	this->iObserverMode = gNetvars.get_offset(XStr("DT_BasePlayer"), XStr("m_iObserverMode"));
	this->hObserverTarget = gNetvars.get_offset(XStr("DT_BasePlayer"), XStr("m_hObserverTarget"));
	this->deadflag = gNetvars.get_offset(XStr("DT_BasePlayer"), XStr("pl"), XStr("deadflag"));
	this->iFOV = gNetvars.get_offset(XStr("DT_BasePlayer"), XStr("m_iFOV"));
	this->iDefaultFOV = gNetvars.get_offset(XStr("DT_BasePlayer"), XStr("m_iDefaultFOV"));
	this->hOwner = gNetvars.get_offset(XStr("DT_BaseCombatWeapon"), XStr("m_hOwner"));
	this->m_rgflCoordinateFrame = 0x324;
}

void InitNetVars() {
	netvar.Init();
}
