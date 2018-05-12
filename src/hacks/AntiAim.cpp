/*
 * AntiAim.cpp
 *
 *  Created on: Oct 26, 2016
 *      Author: nullifiedcat
 */

#include <hacks/hacklist.hpp>
#include "common.hpp"

namespace hacks
{
namespace shared
{
namespace antiaim
{

CatVar communicate(CV_SWITCH, "identify", "1", "identify",
                   "Auto identify for other cathook users");
CatVar enabled(CV_SWITCH, "aa_enabled", "0", "Anti-Aim",
               "Master AntiAim switch");
static CatVar trueang(CV_SWITCH, "aa_realfakes", "0", "Real fakes",
                      "Do real fakeangles (Unresolveable)");
static CatVar yaw(CV_FLOAT, "aa_yaw", "0.0", "Yaw", "Static yaw (left/right)",
                  360.0);
static CatVar pitch(CV_FLOAT, "aa_pitch", "-89.0", "Pitch",
                    "Static pitch (up/down)", -89.0, 89.0);
static CatVar yaw_real(CV_FLOAT, "aa_yaw_real", "0.0", "Real Yaw",
                       "Static yaw (left/right)", 360.0);
static CatVar pitch_real(CV_FLOAT, "aa_pitch_real", "-89.0", "Real Pitch",
                         "Static pitch (up/down)", -89.0, 89.0);
static CatEnum yaw_mode_enum({ "KEEP", "STATIC", "JITTER", "BIGRANDOM",
                               "RANDOM", "SPIN", "OFFSETKEEP", "EDGE", "HECK",
                               "FAKESIDEWAYS" });
static CatEnum pitch_mode_enum({ "KEEP", "STATIC", "JITTER", "RANDOM", "FLIP",
                                 "FAKEFLIP", "FAKEUP", "FAKEDOWN", "FAKECENTER",
                                 "UP", "DOWN", "HECK" });
CatVar yaw_mode(yaw_mode_enum, "aa_yaw_mode", "0", "Yaw mode", "Yaw mode");
static CatVar pitch_mode(pitch_mode_enum, "aa_pitch_mode", "0", "Pitch mode",
                         "Pitch mode");
static CatVar true_yaw_mode(yaw_mode_enum, "aa_yaw_mode_real", "0",
                            "The Real Yaw", "Yaw mode");
static CatVar true_pitch_mode(pitch_mode_enum, "aa_pitch_mode_real", "0",
                              "The Real Pitch", "Pitch mode");
static CatVar roll(CV_FLOAT, "aa_roll", "0", "Roll",
                   "Roll angle (viewangles.z)", -180, 180);
static CatVar
    no_clamping(CV_SWITCH, "aa_no_clamp", "0", "Don't clamp angles",
                "Use this with STATIC mode for unclamped manual angles");
static CatVar spin(CV_FLOAT, "aa_spin", "10.0", "Spin speed",
                   "Spin speed (degrees/second)");

static CatVar aaaa_enabled(CV_SWITCH, "aa_aaaa_enabled", "0", "Enable AAAA",
                           "Enable Anti-Anti-Anti-Aim (Overrides AA Pitch)");
static CatVar aaaa_interval(CV_FLOAT, "aa_aaaa_interval", "0", "Interval",
                            "Interval in seconds, 0 = random");
static CatVar aaaa_interval_random_high(CV_FLOAT, "aa_aaaa_interval_high", "15",
                                        "Interval Ceiling",
                                        "Upper bound for random AAAA interval");
static CatVar aaaa_interval_random_low(CV_FLOAT, "aa_aaaa_interval_low", "3",
                                       "Interval Floor",
                                       "Lower bound for random AAAA interval");
static CatEnum aaaa_modes_enum({ "(FAKE)UP", "(FAKE)DOWN" });
static CatVar aaaa_mode(aaaa_modes_enum, "aa_aaaa_mode", "0", "Mode",
                        "Anti-Anti-Anti-Aim Mode");
static CatVar aaaa_flip_key(CV_KEY, "aa_aaaa_flip_key", "0", "Flip key",
                            "If you press that key, current AA will change");

float cur_yaw  = 0.0f;
int safe_space = 0;

float aaaa_timer_start = 0.0f;
float aaaa_timer       = 0.0f;
int aaaa_stage         = 0;
bool aaaa_key_pressed  = false;

float GetAAAAPitch()
{
    if (!trueang)
    {
        switch ((int) aaaa_mode)
        {
        case 0:
            return aaaa_stage ? -271 : -89;
        case 1:
            return aaaa_stage ? 271 : 89;
        default:
            break;
        }
        return 0;
    }
    else
    {
        if (*bSendPackets == true)
        {
            switch ((int) aaaa_mode)
            {
            case 0:
                return aaaa_stage ? -271 : -89;
            case 1:
                return aaaa_stage ? 271 : 89;
            default:
                break;
            }
        }
        else if (*bSendPackets == false)
            return g_pUserCmd->viewangles.x;
        return 0;
    }
}

float GetAAAATimerLength()
{
    if (aaaa_interval)
    {
        return (float) aaaa_interval;
    }
    else
    {
        return RandFloatRange((float) aaaa_interval_random_low,
                              (float) aaaa_interval_random_high);
    }
}

void NextAAAA()
{
    aaaa_stage++;
    // TODO temporary..
    if (aaaa_stage > 1)
        aaaa_stage = 0;
}

void UpdateAAAAKey()
{
    if (g_IInputSystem->IsButtonDown((ButtonCode_t)(int) aaaa_flip_key))
    {
        if (!aaaa_key_pressed)
        {
            aaaa_key_pressed = true;
            NextAAAA();
        }
    }
    else
        aaaa_key_pressed = false;
}

void UpdateAAAATimer()
{
    const float &curtime = g_GlobalVars->curtime;
    if (aaaa_timer_start > curtime)
        aaaa_timer_start = 0.0f;
    if (!aaaa_timer || !aaaa_timer_start)
    {
        aaaa_timer       = GetAAAATimerLength();
        aaaa_timer_start = curtime;
    }
    else
    {
        if (curtime - aaaa_timer_start > aaaa_timer)
        {
            NextAAAA();
            aaaa_timer_start = curtime;
            aaaa_timer       = GetAAAATimerLength();
        }
    }
}

enum k_EFuckMode
{
    FM_INCREMENT,
    FM_RANDOMVARS,
    FM_JITTER,
    FM_SIGNFLIP,
    FM_COUNT
};

struct FuckData_s
{
    float fl1, fl2, fl3, fl4;
    int i1, i2;
    bool b1, b2;
};

/*
 * Not yet implemented.
 */

void FuckPitch(float &io_pitch)
{
    constexpr float min_pitch = -149489.97f;
    constexpr float max_pitch = 149489.97f;
    static FuckData_s fuck_data;
    static k_EFuckMode fuckmode = k_EFuckMode::FM_RANDOMVARS;
    static int fuckmode_ticks   = 0;

    /*if (!fuckmode_ticks) {
        fuckmode = rand() % k_EFuckMode::FM_COUNT;
        fuckmode_ticks = rand() % 333;
        switch (fuckmode) {
        case k_EFuckMode::FM_INCREMENT:
            fuck_data.fl1 = RandFloatRange(-400.0f, 400.0f);
            fuck_data.i1 = rand() % 3;
            break;
        case k_EFuckMode::FM_JITTER:
            fuck_data.fl1 = RandFloatRange(1.0f, 4.0f);
            break;
        case k_EFuckMode::FM_RANDOMVARS:
            break;
        }
    }*/

    switch (fuckmode)
    {
    case k_EFuckMode::FM_RANDOMVARS:
        io_pitch = RandFloatRange(min_pitch, max_pitch);
    default:
        break;
    }

    if (io_pitch < min_pitch)
        io_pitch = min_pitch;
    if (io_pitch > max_pitch)
        io_pitch = max_pitch;
}

void FuckYaw(float &io_yaw)
{
    constexpr float min_yaw = -359999.97f;
    constexpr float max_yaw = 359999.97f;

    static FuckData_s fuck_data;
    static k_EFuckMode fuckmode = k_EFuckMode::FM_RANDOMVARS;
    static int fuckmode_ticks   = 0;

    switch (fuckmode)
    {
    case k_EFuckMode::FM_RANDOMVARS:
        io_yaw = RandFloatRange(min_yaw, max_yaw);
    default:
        break;
    }

    if (io_yaw < min_yaw)
        io_yaw = min_yaw;
    if (io_yaw > max_yaw)
        io_yaw = max_yaw;
}

void SetSafeSpace(int safespace)
{
    if (safespace > safe_space)
        safe_space = safespace;
}

bool ShouldAA(CUserCmd *cmd)
{
    if (!enabled)
        return false;
    if (hacks::tf2::antibackstab::noaa)
        return false;
    if (cmd->buttons & IN_USE)
        return false;
    if ((cmd->buttons & IN_ATTACK) &&
        !(IsTF2() &&
          g_pLocalPlayer->weapon()->m_iClassID == CL_CLASS(CTFCompoundBow)) &&
        CanShoot())
    {
        return false;
    }
    if ((cmd->buttons & IN_ATTACK2) &&
        g_pLocalPlayer->weapon()->m_iClassID == CL_CLASS(CTFLunchBox))
        return false;
    switch (GetWeaponMode())
    {
    case weapon_projectile:
        if (g_pLocalPlayer->weapon()->m_iClassID == CL_CLASS(CTFCompoundBow))
        {
            if (!(cmd->buttons & IN_ATTACK))
            {
                if (g_pLocalPlayer->bAttackLastTick)
                    SetSafeSpace(4);
            }
            break;
        }
    /* no break */
    case weapon_melee:
    case weapon_throwable:
        if ((cmd->buttons & (IN_ATTACK | IN_ATTACK2)) ||
            g_pLocalPlayer->bAttackLastTick)
        {
            SetSafeSpace(8);
            return false;
        }
        break;
    default:
        break;
    }
    if (safe_space)
    {
        safe_space--;
        if (safe_space < 0)
            safe_space = 0;
        return false;
    }
    return true;
}

// Initialize Edge vars
float edgeYaw      = 0;
float edgeToEdgeOn = 0;

// Function to return distance from you to a yaw directed to
float edgeDistance(float edgeRayYaw)
{
    // Main ray tracing area
    trace_t trace;
    Ray_t ray;
    Vector forward;
    float sp, sy, cp, cy;
    sy        = sinf(DEG2RAD(edgeRayYaw)); // yaw
    cy        = cosf(DEG2RAD(edgeRayYaw));
    sp        = sinf(DEG2RAD(0)); // pitch
    cp        = cosf(DEG2RAD(0));
    forward.x = cp * cy;
    forward.y = cp * sy;
    forward.z = -sp;
    forward   = forward * 300.0f + g_pLocalPlayer->v_Eye;
    ray.Init(g_pLocalPlayer->v_Eye, forward);
    // trace::g_pFilterNoPlayer to only focus on the enviroment
    g_ITrace->TraceRay(ray, 0x4200400B, &trace::filter_no_player, &trace);
    // Pythagorean theorem to calculate distance
    float edgeDistance = (sqrt(pow(trace.startpos.x - trace.endpos.x, 2) +
                               pow(trace.startpos.y - trace.endpos.y, 2)));
    return edgeDistance;
}

// Function to Find an edge and report if one is found at all
bool findEdge(float edgeOrigYaw)
{
    // distance two vectors and report their combined distances
    float edgeLeftDist  = edgeDistance(edgeOrigYaw - 21);
    edgeLeftDist        = edgeLeftDist + edgeDistance(edgeOrigYaw - 27);
    float edgeRightDist = edgeDistance(edgeOrigYaw + 21);
    edgeRightDist       = edgeRightDist + edgeDistance(edgeOrigYaw + 27);

    // If the distance is too far, then set the distance to max so the angle
    // isnt used
    if (edgeLeftDist >= 260)
        edgeLeftDist = 999999999;
    if (edgeRightDist >= 260)
        edgeRightDist = 999999999;

    // If none of the vectors found a wall, then dont edge
    if (edgeLeftDist == edgeRightDist)
        return false;

    // Depending on the edge, choose a direction to face
    if (edgeRightDist < edgeLeftDist)
    {
        edgeToEdgeOn = 1;
        // Correction for pitches to keep the head behind walls
        if (((int) pitch_mode == 7) || ((int) pitch_mode == 2) ||
            ((int) pitch_mode == 8))
            edgeToEdgeOn = 2;
        return true;
    }
    else
    {
        edgeToEdgeOn = 2;
        // Same as above
        if (((int) pitch_mode == 7) || ((int) pitch_mode == 2) ||
            ((int) pitch_mode == 8))
            edgeToEdgeOn = 1;
        return true;
    }
}

// Function to give you a static angle to use
float useEdge(float edgeViewAngle)
{
    // Var to be disabled when a angle is choosen to prevent the others from
    // conflicting
    bool edgeTest = true;
    if (((edgeViewAngle < -135) || (edgeViewAngle > 135)) && edgeTest == true)
    {
        if (edgeToEdgeOn == 1)
            edgeYaw = (float) -90;
        if (edgeToEdgeOn == 2)
            edgeYaw = (float) 90;
        edgeTest    = false;
    }
    if ((edgeViewAngle >= -135) && (edgeViewAngle < -45) && edgeTest == true)
    {
        if (edgeToEdgeOn == 1)
            edgeYaw = (float) 0;
        if (edgeToEdgeOn == 2)
            edgeYaw = (float) 179;
        edgeTest    = false;
    }
    if ((edgeViewAngle >= -45) && (edgeViewAngle < 45) && edgeTest == true)
    {
        if (edgeToEdgeOn == 1)
            edgeYaw = (float) 90;
        if (edgeToEdgeOn == 2)
            edgeYaw = (float) -90;
        edgeTest    = false;
    }
    if ((edgeViewAngle <= 135) && (edgeViewAngle >= 45) && edgeTest == true)
    {
        if (edgeToEdgeOn == 1)
            edgeYaw = (float) 179;
        if (edgeToEdgeOn == 2)
            edgeYaw = (float) 0;
        edgeTest    = false;
    }
    // return with the angle choosen
    return edgeYaw;
}

Timer delay{};
int val       = 0;
int value[32] = { 0 };
void ProcessUserCmd(CUserCmd *cmd)
{
    if (communicate && CE_GOOD(LOCAL_E))
    {
        for (int i = 0; i < g_IEngine->GetMaxClients(); i++)
        {
            CachedEntity *ent = ENTITY(i);
            if (CE_GOOD(ent))
            {
                if ((int) (CE_FLOAT(ent, netvar.angEyeAngles)) == 33 &&
                    (int) (CE_FLOAT(ent, netvar.angEyeAngles + 4)) == 33)
                {
                    player_info_s info;
                    if (g_IEngine->GetPlayerInfo(ent->m_IDX, &info) &&
                        playerlist::AccessData(info.friendsID).state !=
                            playerlist::k_EState::CAT)
                    {
                        value[ent->m_IDX]++;
                        if (value[ent->m_IDX] > 5)
                        {
                            playerlist::AccessData(info.friendsID).state =
                                playerlist::k_EState::CAT;
                            cmd->viewangles.y                = 53;
                            cmd->viewangles.x                = 33;
                            g_pLocalPlayer->bUseSilentAngles = true;
                            return;
                        }
                    }
                }
            }
        }
        if (delay.test_and_set(180000) || val)
        {
            cmd->viewangles.y                = 33;
            cmd->viewangles.x                = 33;
            g_pLocalPlayer->bUseSilentAngles = true;
            val++;
            if (val == 3)
                val = 0;
            return;
        }
    }
    if (!ShouldAA(cmd))
        return;
    static bool angstate = true;
    if (trueang)
        angstate = !angstate;
    if (!LOCAL_E->m_bAlivePlayer)
        angstate = true;
    if (lagexploit::ExploitActive() || g_pUserCmd->buttons & IN_ATTACK ||
        g_pUserCmd->buttons & IN_ATTACK2)
        angstate     = true;
    *bSendPackets    = angstate;
    float &p         = cmd->viewangles.x;
    float &y         = cmd->viewangles.y;
    static bool flip = false;
    bool clamp       = !no_clamping;
    if (!*bSendPackets)
        switch ((int) true_yaw_mode)
        {
        case 1: // FIXED
            y = (float) yaw_real;
            break;
        case 2: // JITTER
            if (flip)
                y += 90;
            else
                y -= 90;
            break;
        case 3: // BIGRANDOM
            y     = RandFloatRange(-65536.0f, 65536.0f);
            clamp = false;
            break;
        case 4: // RANDOM
            y = RandFloatRange(-180.0f, 180.0f);
            break;
        case 5: // SPIN
            cur_yaw += (float) spin;
            if (cur_yaw > 180)
                cur_yaw = -180;
            if (cur_yaw < -180)
                cur_yaw = 180;
            y           = cur_yaw;
            break;
        case 6: // OFFSETKEEP
            y += (float) yaw_real;
            break;
        case 7: // Edge
            // Attemt to find an edge and if found, edge
            if (findEdge(y))
                y = useEdge(y);
            break;
        case 8:
            FuckYaw(y);
            clamp = false;
            break;
        default:
            break;
        }
    if (*bSendPackets)
        switch ((int) yaw_mode)
        {
        case 1: // FIXED
            y = (float) yaw;
            break;
        case 2: // JITTER
            if (flip)
                y += 90;
            else
                y -= 90;
            break;
        case 3: // BIGRANDOM
            y     = RandFloatRange(-65536.0f, 65536.0f);
            clamp = false;
            break;
        case 4: // RANDOM
            y = RandFloatRange(-180.0f, 180.0f);
            break;
        case 5: // SPIN
            cur_yaw += (float) spin;
            if (cur_yaw > 180)
                cur_yaw = -180;
            if (cur_yaw < -180)
                cur_yaw = 180;
            y           = cur_yaw;
            break;
        case 6: // OFFSETKEEP
            y += (float) yaw;
            break;
        case 7: // Edge
            // Attemt to find an edge and if found, edge
            if (findEdge(y))
                y = useEdge(y);
            break;
        case 8:
            FuckYaw(y);
            clamp = false;
            break;
        default:
            break;
        }
    if (yaw_mode == 9)
        y += *bSendPackets ? 90.0f : -90.0f;
    if (!*bSendPackets)
        switch (int(true_pitch_mode))
        {
        case 1:
            p = float(pitch_real);
            break;
        case 2:
            if (flip)
                p += 30.0f;
            else
                p -= 30.0f;
            break;
        case 3:
            p = RandFloatRange(-89.0f, 89.0f);
            break;
        case 4:
            p = flip ? 89.0f : -89.0f;
            break;
        case 5:
            p     = flip ? 271.0f : -271.0f;
            clamp = false;
            break;
        case 6:
            p     = -271.0f;
            clamp = false;
            break;
        case 7:
            p     = 271.0f;
            clamp = false;
            break;
        case 8:
            p     = -3256.0f;
            clamp = false;
            break;
        case 9:
            p = -89.0f;
            break;
        case 10:
            p = 89.0f;
            break;
        case 11:
            FuckPitch(p);
            clamp = false;
        }
    if (*bSendPackets)
        switch (int(pitch_mode))
        {
        case 1:
            p = float(pitch);
            break;
        case 2:
            if (flip)
                p += 30.0f;
            else
                p -= 30.0f;
            break;
        case 3:
            p = RandFloatRange(-89.0f, 89.0f);
            break;
        case 4:
            p = flip ? 89.0f : -89.0f;
            break;
        case 5:
            p     = flip ? 271.0f : -271.0f;
            clamp = false;
            break;
        case 6:
            p     = -271.0f;
            clamp = false;
            break;
        case 7:
            p     = 271.0f;
            clamp = false;
            break;
        case 8:
            p     = -3256.0f;
            clamp = false;
            break;
        case 9:
            p = -89.0f;
            break;
        case 10:
            p = 89.0f;
            break;
        case 11:
            FuckPitch(p);
            clamp = false;
        }
    flip = !flip;
    if (clamp)
        fClampAngle(cmd->viewangles);
    if (roll)
        cmd->viewangles.z = float(roll);
    if (aaaa_enabled)
    {
        UpdateAAAAKey();
        UpdateAAAATimer();
        p = GetAAAAPitch();
    }
    g_pLocalPlayer->bUseSilentAngles = true;
}
}
}
}
