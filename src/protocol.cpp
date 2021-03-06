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
	PlayerDataBlob* playerData = PlayerDB::FindPlayerDataBlob(playerID);
	if (playerData)
	{
		gameResults = playerData->gameResults;
		return true;
	}
	return false;
}

bool Protocol::DoClear::Response::HandleRequest(const Request& request, player_id playerID)
{
	// TODO: block it if they don't have it unlocked?
	// TODO: check if they even passed it?
	PlayerDataBlob* playerData = PlayerDB::FindPlayerDataBlob(playerID);
	bool success = false;
	if (playerData)
	{
		success = true;

		const GameResult& result = request.aparams.result;
		// Cleared it
		if (result.isCleared)
		{
			if (!playerData->DoClear(result, ResultRewards, DoSupportGachaResult.resultSupporter))
			{
				// Something went wrong
				return false;
			}

			// Assign results
			PlayerInfo = playerData->playerInfo;

			isGetSuppoterCard = !DoSupportGachaResult.resultSupporter.empty();
			if (isGetSuppoterCard)
			{
				// The support list is already synced below, so this is useless
				DoSupportGachaResult.support = playerData->itemData.support;
			}

			// Full sync on items, achievs, quest, stages, supports
			ItemList = playerData->itemData.items;
			PlayerAchievement = playerData->playerState.player_Achievement;
			PlayerQuest = playerData->playerState.player_Quest;
			SupportList = playerData->itemData.support;

			// Better send the whole item list twice
			ResponseData.items = playerData->itemData.items;
			// LP results
			ResponseData.player_MetaResult_SavePeople = playerData->playerState.player_MetaResult_SavePeople;
			// Send all the quest again because why not
			ResponseData.player_Quest = playerData->playerState.player_Quest;
		}
	}
	return success;
}

bool Protocol::DoClearStandard::Response::HandleRequest(const rapidjson::Value& action, player_id playerID)
{
	Request req;
	DeserializeRequest(action, req);
	return DoClear::Response::HandleRequest(req, playerID);
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

	::PlayerInfo* playerInfo = PlayerDB::GetPlayerInfo(playerID);
	UserItemAndShop_Datainfo* itemData = PlayerDB::GetUserItemAndShop_Datainfo(playerID);
	if (playerInfo && itemData)
	{
		const int requestedStory = req.aparams.StoryNo;
		// Block request if they don't have it unlocked
		if (playerInfo->OpenStoryNo >= requestedStory)
		{
			// Track what story mission they are playing
			playerInfo->currentStory = requestedStory;

			PlayerInfo = *playerInfo;
			ItemList = itemData->items;

			return true;
		}
	}
	return false;
}

bool Protocol::DoClearStory::Response::HandleRequest(const rapidjson::Value& action, player_id playerID)
{
	Request req;
	DeserializeRequest(action, req);

	// TODO: block it if they don't have it unlocked?
	// TODO: check if they even passed it?
	PlayerDataBlob* playerData = PlayerDB::FindPlayerDataBlob(playerID);
	bool success = false;
	if (playerData)
	{
		const CMSData& cmsData = Server::GetCMSData();
		const CMS_Mode_Story* story = cmsData.FindDataByKey<CMS_Mode_Story>(playerData->playerInfo.currentStory);
		// Make sure we actually were on a story
		if (story)
		{
			success = true;

			const GameResult& result = req.aparams.result;
			// Cleared it
			if (result.isCleared)
			{
				// Advance unlocked story
				if (story->StoryNo > playerData->playerInfo.ClearStoryNo)
				{
					playerData->playerInfo.ClearStoryNo = story->StoryNo;
				}

				// Advance the tutorial once the first story mission is done
				if (playerData->playerInfo.TutorialNo < 1)
				{
					playerData->playerInfo.TutorialNo = 1;
				}

				// Something wrong or cheating
				if (result.musicNo != story->MusicNo)
				{
					return false;
				}

				if (!DoClear::Response::HandleRequest(req, playerID))
				{
					// Something went wrong
					return false;
				}
			}
		}
	}

	return success;
}

bool Protocol::BuyArcade::Response::HandleRequest(const rapidjson::Value& action, player_id playerID)
{
	PlayerDataBlob* playerData = PlayerDB::FindPlayerDataBlob(playerID);
	if (playerData)
	{
		Request req;
		DeserializeRequest(action, req);

		bool isValidPurchase = false;
		// Find the stage they want to buy
		const CMS_Music* music = Server::GetCMSData().FindDataByKey<CMS_Music>(req.aparams.arcadeStageNo);
		if (music)
		{
			if (req.aparams.buyWay == CMSData::ItemConstants::Gold)
			{
				const CMS_Shop_BuyGold* shopBuyGold = Server::GetCMSData().FindDataByKey<CMS_Shop_BuyGold>(playerData->playerInfo.MusicByGoldCount + 1);
				const int cost = shopBuyGold ? shopBuyGold->MusicGold : 500000; // The client code would probably crash if the step exceed
				if (playerData->playerInfo.gold >= cost)
				{
					// Remove gold
					playerData->RemoveItem(CMSData::ItemConstants::Gold, cost);

					// Increment the music by gold count
					const int maxCount = Server::GetCMSData().GetMaxShopByGoldCount();
					playerData->playerInfo.MusicByGoldCount++;
					if (playerData->playerInfo.MusicByGoldCount > maxCount)
					{
						playerData->playerInfo.MusicByGoldCount = maxCount;
					}

					isValidPurchase = true;
				}
			}
			else if (req.aparams.buyWay == CMSData::ItemConstants::StarCube)
			{
				const CMS_MasterValue* masterValue = Server::GetCMSData().FindDataByKey<CMS_MasterValue>("STANDARD_MUSIC_STARCUBE_PRICE");
				const int cost = masterValue ? masterValue->IntValue : 0; // If the price isn't set consider it free
				if (playerData->playerInfo.jewel >= cost)
				{
					// Remove jewels
					playerData->RemoveItem(CMSData::ItemConstants::StarCube, cost);

					isValidPurchase = true;
				}
			}
			// can tickets buys songs?

			if (isValidPurchase)
			{
				// Advance the tutorial once the first song is purchased
				if (playerData->playerInfo.TutorialNo < 2)
				{
					playerData->playerInfo.TutorialNo = 2;
				}

				// add song
				PlayerData_ArcadeStage song;
				song.MusicNo = music->MusicNo;
				playerData->playerState.playerData_ArcadeStage.push_back(song);

				// Save
				PlayerDB::SavePlayerData(playerID);
			}
		}

		// Setup respone data
		buyArcadeResult.isValidRequest = isValidPurchase;
		if (isValidPurchase) // Don't both with other stuff if request is invalid
		{
			buyArcadeResult.playerInfo = playerData->playerInfo;
			buyArcadeResult.userValueList = playerData->playerState.player_UserValue;
			buyArcadeResult.items = playerData->itemData.items;
			buyArcadeResult.playerArcadeList = playerData->playerState.playerData_ArcadeStage;
			buyArcadeResult.playerStoryList = playerData->playerState.playerData_Story;
			buyArcadeResult.playerQuestList = playerData->playerState.player_Quest;
		}
	}
	return true;
}

bool Protocol::BuyPackage::Response::HandleRequest(const rapidjson::Value& action, player_id playerID)
{
	PlayerDataBlob* playerData = PlayerDB::FindPlayerDataBlob(playerID);
	if (playerData)
	{
		Request req;
		DeserializeRequest(action, req);

		isBuySuccess = 0;
		if (!playerData->itemData.HasPackage(req.aparams.ProductID))
		{
			const CMS_Shop_Package* package = Server::GetCMSData().FindDataByKey<CMS_Shop_Package>(req.aparams.ProductID);
			if (package)
			{
				if (req.aparams.BuyType == CMSData::ItemConstants::StarCube)
				{
					const int price = playerData->GetPackagePrice(package->PackageCode, package->Price_Now);
					if (playerData->playerInfo.jewel >= price)
					{
						playerData->RemoveItem(CMSData::ItemConstants::StarCube, price);
						playerData->AddPackage(package->PackageCode);
						isBuySuccess = 1;
					}
				}
				else if (req.aparams.BuyType == CMSData::ItemConstants::Ticket)
				{
					// TODO - buy with ticket
				}
			}
		}

		if (isBuySuccess)
		{
			responseData.playerInfo = playerData->playerInfo;
			responseData.playerArcadeList = playerData->playerState.playerData_ArcadeStage;
		}
	}
	return true;
}