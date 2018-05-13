#pragma once

#include "player_id.h"

#include "player_info.h"
#include "response_data.h"
#include "item_and_shop_data.h"
#include "game_result.h"

#include <string>

struct PlayerDataBlob : public Json::ISerializeable
{
	PlayerInfo playerInfo;
	ResponseData playerState;
	UserItemAndShop_Datainfo itemData;
	std::vector<GameResult> gameResults;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(playerInfo);
		SERIALIZE_JSON(playerState);
		SERIALIZE_JSON(itemData);
	}

	void AddItem(const std::string& itemCode, int quantity);
};

namespace PlayerDB
{
	bool LoadAllPlayerData();
	bool SaveAllPlayerData();
	bool SavePlayerData(player_id id);

	player_id CreateNewPlayer();
	player_id CreateOrGetPlayer(player_id playerID);
	PlayerDataBlob* FindPlayerDataBlob(player_id playerID);
	PlayerInfo* GetPlayerInfo(player_id playerID);
	ResponseData* GetPlayerState(player_id playerID);
	UserItemAndShop_Datainfo* GetUserItemAndShop_Datainfo(player_id playerID);
	GameResult* GetGameResult(player_id playerID, int musicNo, int patternNo);
	void SetPlayerFlag(player_id playerID, const std::string& key, int value);
	void SetUserValue(player_id playerID, const std::string& key, int valueInt, const std::string& valueStr);
}