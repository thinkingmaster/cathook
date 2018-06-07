/*
 * votelogger.cpp
 *
 *  Created on: Dec 31, 2017
 *      Author: nullifiedcat
 */

#include "common.hpp"
#include <boost/algorithm/string.hpp>

namespace votelogger
{

static CatVar enabled(CV_SWITCH, "votelog", "0", "Log votes");
static CatVar requeue(CV_SWITCH, "votelog_requeue", "1",
                      "Auto requeue on vote kick", "Auto requeue on vote kick");
static CatVar anti_votekick(CV_SWITCH, "anti_votekick", "0", "anti-votekick",
                            "Prevent votekicks by lagging the server in a way "
                            "that every vote comes is delayed.\ndo not forget "
                            "to enable votelog and that this\nmakes the server "
                            "be down for about 30 seconds\ncl_timeout 60 is a "
                            "must");
static CatVar kick_msg(CV_STRING, "anti_votekick_string",
                       "Everyone thank $NAME for initiating the votekick! The "
                       "server will now be shut down!",
                       "anti-votekick message",
                       "Send this message on Votekick attempts against "
                       "you.\n$NAME gets replaced with their name\n$CLASS with "
                       "their class.");
Timer antikick{};
bool active = false;

const std::string tf_classes[] = { "class",   "scout",   "sniper", "soldier",
                                   "demoman", "medic",   "heavy",  "pyro",
                                   "spy",     "engineer" };
void user_message(bf_read &buffer, int type)
{

    bool islocalplayer = false;
    if (!enabled)
        return;
    switch (type)
    {
    case 45:
        // Call Vote Failed
        break;
    case 46:
    {
        islocalplayer        = false;
        unsigned char caller = buffer.ReadByte();
        // unknown
        buffer.ReadByte();
        char reason[64];
        char name[64];
        buffer.ReadString(reason, 64, false, nullptr);
        buffer.ReadString(name, 64, false, nullptr);
        unsigned char eid = buffer.ReadByte();
        buffer.Seek(0);
        eid >>= 1;

        unsigned steamID = 0;
        player_info_s info;
        if (g_IEngine->GetPlayerInfo(eid, &info))
            steamID = info.friendsID;
        if (eid == LOCAL_E->m_IDX ||
            playerlist::AccessData(steamID).state ==
                playerlist::k_EState::FRIEND)
        {
            islocalplayer = true;
            if (anti_votekick && !active)
            {
                active = true;
                antikick.update();
                std::string msg(kick_msg.GetString());
                ReplaceString(msg, "$NAME", format(info.name));
                if (CE_GOOD(ENTITY(eid)))
                {
                    int clz = g_pPlayerResource->GetClass(ENTITY(eid));
                    ReplaceString(msg, "$CLASS", format(tf_classes[clz]));
                }
                NET_StringCmd senddata(format("say ", msg).c_str());
                INetChannel *ch =
                    (INetChannel *) g_IEngine->GetNetChannelInfo();
                senddata.SetNetChannel(ch);
                senddata.SetReliable(true);
                ch->SendNetMsg(senddata, true);
                ch->Transmit();
            }
        }

        logging::Info("Vote called to kick %s [U:1:%u] for %s", name, steamID,
                      reason);
        break;
    }
    case 47:
        logging::Info("Vote passed");
        if (islocalplayer && requeue)
            tfmm::queue_start();
        break;
    case 48:
        logging::Info("Vote failed");
        break;
    case 49:
        logging::Info("VoteSetup?");
        break;
    }
}
}
