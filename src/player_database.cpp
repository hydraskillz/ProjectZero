#include "player_database.h"

#include "player_info.h"
#include "response_data.h"
#include "item_and_shop_data.h"
#include "util.h"

#include <map>
#include <sstream>
#include <iostream>

static const int PLAYER_ID_BASELINE = 333000;
static const int PLAYER_ID_STEP = 6;

struct PlayerDataBlob : public Json::ISerializeable
{
	PlayerInfo playerInfo;
	ResponseData playerState;
	UserItemAndShop_Datainfo itemData;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(playerInfo);
		SERIALIZE_JSON(playerState);
		SERIALIZE_JSON(itemData);
	}
};

static std::map<player_id, PlayerDataBlob*> gPlayers;

namespace
{
	PlayerDataBlob* FindPlayerDataBlob(player_id playerID)
	{
		PlayerDataBlob* blob = nullptr;
		auto itr = gPlayers.find(playerID);
		if (itr != gPlayers.end())
		{
			blob = itr->second;
		}
		return blob;
	}

	bool SavePlayerData(player_id id, PlayerDataBlob* data)
	{
		std::stringstream ss;
		ss << "users/" << id << ".json";
		return Json::WriteFile(ss.str().c_str(), *data);
	}
}

bool PlayerDB::LoadAllPlayerData()
{
	gPlayers.clear();
	// TODO - free memory

	std::vector<std::string> filenames = GetAllFileNamesInDirectory("users");
	for (auto& fn : filenames)
	{
		player_id id = INVALID_PLAYER_ID;
		sscanf(fn.c_str(), "%lld.json", &id);

		PlayerDataBlob* blob = new PlayerDataBlob;
		gPlayers[id] = blob;

		Json::ReadFile(std::string(std::string("users/") + fn).c_str(), *blob);
	}
	return true;
}

bool PlayerDB::SaveAllPlayerData()
{
	for (auto& pair : gPlayers)
	{
		const player_id id = pair.first;
		PlayerDataBlob* data = pair.second;
		::SavePlayerData(id, data);
	}
	return true;
}

bool PlayerDB::SavePlayerData(player_id id)
{
	PlayerDataBlob* data = FindPlayerDataBlob(id);
	if (data)
	{
		return ::SavePlayerData(id, data);
	}
	return false;
}

player_id PlayerDB::CreateNewPlayer()
{
	// Simple staggard id generation
	player_id id = static_cast<player_id>(PLAYER_ID_BASELINE + PLAYER_ID_STEP * (gPlayers.size() + 1));
	auto itr = gPlayers.find(id);
	if (itr == gPlayers.end())
	{
		PlayerDataBlob* blob = new PlayerDataBlob;
		gPlayers[id] = blob;

		PlayerInfo& playerInfo = blob->playerInfo;

		std::stringstream ss;
		ss << "R" << gPlayers.size();

		DateTime now = GetCurrentDateTime();

		playerInfo.name = ss.str();
		playerInfo.LastActionDate = now;
		playerInfo.SupportByGoldTime = "0001-01-01T00:00:00"; // i.e. never
		playerInfo.CreateDate = now;

		ResponseData& playerState = blob->playerState;

		// Default player flags - TODO: move to data file
		playerState.SetFlag("isArcadeOpen", 0);
		playerState.SetFlag("isBaseIntroduced", 0);
		playerState.SetFlag("isCommunityOpen", 0);
		playerState.SetFlag("isGetSpiny", 0);
		playerState.SetFlag("isGetV", 0);
		playerState.SetFlag("isGuideStart", 0);
		playerState.SetFlag("isMenuOpen", 0);
		playerState.SetFlag("isMissionOpen", 0);
		playerState.SetFlag("isNotCali", 0);
		playerState.SetFlag("isNotFirstPlay", 0);
		playerState.SetFlag("isShopOpen", 0);
		playerState.SetFlag("isStoryOpen", 0);

		// Default user values - TODO: move to data file
		playerState.SetUserValue("ArcadeClear_Count", 0, "");
		playerState.SetUserValue("BuyArcade_ByGold_Count", 1, "");
		playerState.SetUserValue("ValueKey=Current_QuestID", 0, "Tutorial010");
		playerState.SetUserValue("FingerTutor_Step", 1, "");
		playerState.SetUserValue("FirstChapter_Order_No", 1, "");
		playerState.SetUserValue("ValueKey=Language", 0, "KR");

		// I don't know what this is for
		playerState.playerData_Story.push_back(PlayerData_Story());

		// Quests - TODO: use the condition things to start these
		playerState.player_Quest.push_back(Player_Quest(1)); // 1 is the default tutorial quest

		UserItemAndShop_Datainfo& itemData = blob->itemData;

		// Add core supports
		itemData.support.push_back(Player_Support("ZCB01"));
		itemData.support.push_back(Player_Support("ZCG01"));
		itemData.support.push_back(Player_Support("ZCR01"));
		itemData.support.push_back(Player_Support("ZCY01"));

		// Add default items
		itemData.items.push_back(Player_Item("GOLD", 100));

		std::cout << "Created new player id=" << id << " name=" << playerInfo.name << std::endl;

		::SavePlayerData(id, blob);
	}
	else
	{
		// The player id was not unique!
		id = INVALID_PLAYER_ID;
	}
	return id;
}

player_id PlayerDB::CreateOrGetPlayer(player_id playerID)
{
	if (FindPlayerDataBlob(playerID) == nullptr)
	{
		return CreateNewPlayer();
	}
	return playerID;
}

PlayerInfo* PlayerDB::GetPlayerInfo(player_id playerID)
{
	PlayerInfo* playerInfo = nullptr;
	PlayerDataBlob* blob = FindPlayerDataBlob(playerID);
	if (blob)
	{
		playerInfo = &blob->playerInfo;
	}
	return playerInfo;
}

ResponseData* PlayerDB::GetPlayerState(player_id playerID)
{
	ResponseData* playerState = nullptr;
	PlayerDataBlob* blob = FindPlayerDataBlob(playerID);
	if (blob)
	{
		playerState = &blob->playerState;
	}
	return playerState;
}

UserItemAndShop_Datainfo* PlayerDB::GetUserItemAndShop_Datainfo(player_id playerID)
{
	UserItemAndShop_Datainfo* itemData = nullptr;
	PlayerDataBlob* blob = FindPlayerDataBlob(playerID);
	if (blob)
	{
		itemData = &blob->itemData;
	}
	return itemData;
}

void PlayerDB::SetPlayerFlag(player_id playerID, const std::string& key, int value)
{
	ResponseData* playerState = GetPlayerState(playerID);
	if (playerState)
	{
		playerState->SetFlag(key, value);
		SavePlayerData(playerID);
	}
}

void PlayerDB::SetUserValue(player_id playerID, const std::string& key, int valueInt, const std::string& valueStr)
{
	ResponseData* playerState = GetPlayerState(playerID);
	if (playerState)
	{
		playerState->SetUserValue(key, valueInt, valueStr);
		SavePlayerData(playerID);
	}
}