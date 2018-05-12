#pragma once

#include "player_id.h"
#include <string>

struct PlayerInfo;
struct ResponseData;
struct UserItemAndShop_Datainfo;

namespace PlayerDB
{
	bool LoadAllPlayerData();
	bool SaveAllPlayerData();
	bool SavePlayerData(player_id id);

	player_id CreateNewPlayer();
	player_id CreateOrGetPlayer(player_id playerID);
	PlayerInfo* GetPlayerInfo(player_id playerID);
	ResponseData* GetPlayerState(player_id playerID);
	UserItemAndShop_Datainfo* GetUserItemAndShop_Datainfo(player_id playerID);
	void SetPlayerFlag(player_id playerID, const std::string& key, int value);
	void SetUserValue(player_id playerID, const std::string& key, int valueInt, const std::string& valueStr);
}