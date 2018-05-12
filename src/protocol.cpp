#include "protocol.h"
#include "server.h"
#include "player_database.h"

namespace
{
	void DeserializeRequest(const rapidjson::Value& action, Protocol::Request& req)
	{
		Json::Serializer s(action);
		req.Serialize(s);
	}
}

bool Protocol::CheckServerConnection::Response::HandleRequest(const rapidjson::Value& action, player_id playerID)
{
	version = Server::GetVersion();
	return true;
}

bool Protocol::CreateAndLogin::Response::HandleRequest(const rapidjson::Value& action, ::player_id playerID)
{
	// Make a new player
	player_id = PlayerDB::CreateNewPlayer();
	return player_id != INVALID_PLAYER_ID;
}

bool Protocol::Login::Response::HandleRequest(const rapidjson::Value& action, player_id playerID)
{
	// This just forces a player creation on the given id
	// Note secure or good, but neither is anything else here
	return PlayerDB::CreateOrGetPlayer(playerID) != INVALID_PLAYER_ID;
}

bool Protocol::CreateAndLoginPlatform::Response::HandleRequest(const rapidjson::Value& action, ::player_id playerID)
{
	Request req;
	DeserializeRequest(action, req);
	req.aparams.id; // some unique id, such as google play id

	// TODO
	// Look up a player id by platform service id?

	return false;
}

bool Protocol::GetPlayerInfo::Response::HandleRequest(const rapidjson::Value& action, player_id playerID)
{
	PlayerInfo* playerInfo = PlayerDB::GetPlayerInfo(playerID);
	if (playerInfo)
	{
		player_info = *playerInfo;
	}
	return playerInfo != nullptr;
}

bool Protocol::GetGameResult::Response::HandleRequest(const rapidjson::Value& action, player_id playerID)
{
	// TODO
	return true;
}

bool Protocol::DoClearStandard::Response::HandleRequest(const rapidjson::Value& action, player_id playerID)
{
	// TODO
	return true;
}

bool Protocol::DoSupportGacha::Response::HandleRequest(const rapidjson::Value& action, player_id playerID)
{
	// TODO
	return true;
}

bool Protocol::DoSupportLevelUp::Response::HandleRequest(const rapidjson::Value& action, player_id playerID)
{
	// TODO
	return true;
}

bool Protocol::DoGemSupportLevelUp::Response::HandleRequest(const rapidjson::Value& action, player_id playerID)
{
	// TODO
	return true;
}

bool Protocol::CheckUpdateTable::Response::HandleRequest(const rapidjson::Value& action, player_id playerID)
{
	// TODO - get partial table update data. this will speed up the initial login process
	TableList = Server::GetCMSData();
	TableList.isUpdate = true;

	return true;
}

bool Protocol::GetMasterShopInfo::Response::HandleRequest(const rapidjson::Value& action, player_id playerID)
{
	// TODO
	return true;
}

bool Protocol::GetUserStatusInfo::Response::HandleRequest(const rapidjson::Value& action, player_id playerID)
{
	ResponseData* playerState = PlayerDB::GetPlayerState(playerID);
	if (playerState)
	{
		responseData = *playerState;
	}
	return playerState != nullptr;
}

bool Protocol::GetUserItemAndShopInfo::Response::HandleRequest(const rapidjson::Value& action, player_id playerID)
{
	UserItemAndShop_Datainfo* itemData = PlayerDB::GetUserItemAndShop_Datainfo(playerID);
	if (itemData)
	{
		responseData = *itemData;
	}
	return itemData != nullptr;
}

bool Protocol::GetAttendance::Response::HandleRequest(const rapidjson::Value& action, player_id playerID)
{
	PlayerInfo* playerInfo = PlayerDB::GetPlayerInfo(playerID);
	UserItemAndShop_Datainfo* itemData = PlayerDB::GetUserItemAndShop_Datainfo(playerID);

	if (playerInfo && itemData)
	{
		player_info = *playerInfo;
		userItemAndShop_Datainfo = *itemData;
		result.IsAttendance = true;

		return true;
	}
	
	return false;
}

bool Protocol::GetPlayerMailBox::Response::HandleRequest(const rapidjson::Value& action, player_id playerID)
{
	// TODO
	return true;
}

bool Protocol::GetRankInfo::Response::HandleRequest(const rapidjson::Value& action, player_id playerID)
{
	// TODO
	MyRank = 1; // Everyone is #1!!
	return true;
}

bool Protocol::ReportFlagChange::Response::HandleRequest(const rapidjson::Value& action, player_id playerID)
{
	Request req;
	DeserializeRequest(action, req);
	PlayerDB::SetPlayerFlag(playerID, req.aparams.flagKey, req.aparams.flagValue);
	return true;
}

bool Protocol::ReportUserValueChange::Response::HandleRequest(const rapidjson::Value& action, player_id playerID)
{
	Request req;
	DeserializeRequest(action, req);
	PlayerDB::SetUserValue(playerID, req.aparams.valueKey, req.aparams.intValue, req.aparams.strValue);
	return true;
}

bool Protocol::DoStartStory::Response::HandleRequest(const rapidjson::Value& action, player_id playerID)
{
	Request req;
	DeserializeRequest(action, req);

	// TODO: block it if they don't have it unlocked?

	::PlayerInfo* playerInfo = PlayerDB::GetPlayerInfo(playerID);
	UserItemAndShop_Datainfo* itemData = PlayerDB::GetUserItemAndShop_Datainfo(playerID);
	if (playerInfo && itemData)
	{
		PlayerInfo = *playerInfo;
		ItemList = itemData->items;

		return true;
	}
	return false;
}

bool Protocol::DoClearStory::Response::HandleRequest(const rapidjson::Value& action, player_id playerID)
{
	Request req;
	DeserializeRequest(action, req);

	// TODO: block it if they don't have it unlocked?
	// TODO: check if they even passed it?

	::PlayerInfo* playerInfo = PlayerDB::GetPlayerInfo(playerID);
	UserItemAndShop_Datainfo* itemData = PlayerDB::GetUserItemAndShop_Datainfo(playerID);
	::ResponseData* playerState = PlayerDB::GetPlayerState(playerID);
	bool success = false;
	if (playerInfo && itemData && playerState)
	{
		success = true;

		itemData->items.push_back(Player_Item("GOLD", 500));
		itemData->items.push_back(Player_Item("StarCube", 100));

		playerInfo->gold += 500;
		playerInfo->jewel += 100;

		// Advance the tutorial once the first story mission is done
		if (playerInfo->TutorialNo < 1)
		{
			playerInfo->TutorialNo = 1;
		}

		// TODO: Compute rewards
		// 
		ResultRewards.BasicRewards.push_back(RewardItem("GOLD", 500));
		ResultRewards.SupportRewards.push_back(RewardItem("StarCube", 100));

		// TODO: Roll for support get
		//
		isGetSuppoterCard = false; // Nothing for now...

		PlayerDB::SaveAllPlayerData();

		PlayerInfo = *playerInfo;

		// Full sync on items, achievs, quest, stages, supports
		ItemList = itemData->items;
		PlayerAchievement = playerState->player_Achievement;
		PlayerQuest = playerState->player_Quest;
		StageList = playerState->playerData_ArcadeStage;
		SupportList = itemData->support;

		// Better send the whole item list twice
		ResponseData.items = itemData->items;
		// What even is this
		ResponseData.player_MetaResult_SavePeople = playerState->player_MetaResult_SavePeople;
		// Send all the quest again because why not
		ResponseData.player_Quest = playerState->player_Quest;
	}

	return success;
}
