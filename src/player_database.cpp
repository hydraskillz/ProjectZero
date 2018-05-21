#include "player_database.h"
#include "server.h"
#include "cms_data.h"
#include "util.h"

#include <map>
#include <sstream>
#include <iostream>

static const int PLAYER_ID_BASELINE = 333000;
static const int PLAYER_ID_STEP = 6;

static std::map<player_id, PlayerDataBlob*> gPlayers;

namespace
{
	bool SavePlayerData(player_id id, PlayerDataBlob* data)
	{
		std::stringstream ss;
		ss << "users/" << id << ".json";
		return Json::WriteFile(ss.str().c_str(), *data);
	}
}

void PlayerDataBlob::AddItem(const std::string& itemCode, int quantity)
{
	const CMS_Item* item = Server::GetCMSData().FindDataByKey<CMS_Item>(itemCode);
	if (item)
	{
		if (item->IsInventoryItem())
		{
			bool found = false;
			for (Player_Item& itr : itemData.items)
			{
				if (itr.ItemCode == itemCode)
				{
					itr.Quantity += quantity;
					found = true;
				}
			}

			if (!found)
			{
				itemData.items.push_back(Player_Item(itemCode, quantity));
			}
		}

		switch (item->ItemType)
		{
			case CMS_Item::Gold:
			{
				playerInfo.gold += quantity;
			}
			break;

			case CMS_Item::StartCube:
			{
				playerInfo.jewel += quantity;
			}
			break;

			case CMS_Item::StandardSong:
			{
				PlayerData_ArcadeStage stage;
				stage.MusicNo = std::atoi(item->ItemName.c_str());
				if (!playerState.HasArcadeStage(stage.MusicNo))
				{
					playerState.playerData_ArcadeStage.push_back(stage);
				}
			}
			break;

			// TODO - handle special stuff for other item additions

			default: break;
		}
	}
}

void PlayerDataBlob::RemoveItem(const std::string& itemCode, int quantity)
{
	const CMS_Item* item = Server::GetCMSData().FindDataByKey<CMS_Item>(itemCode);
	if (item)
	{
		if (item->IsInventoryItem())
		{
			bool found = false;
			for (Player_Item& itr : itemData.items)
			{
				if (itr.ItemCode == itemCode)
				{
					// Remove as many as we can
					itr.Quantity -= quantity;
					if (itr.Quantity < 0)
					{
						itr.Quantity = 0;
					}
					found = true;
				}
			}
		}

		switch (item->ItemType)
		{
			case CMS_Item::Gold:
			{
				playerInfo.gold -= quantity;
				if (playerInfo.gold < 0)
				{
					playerInfo.gold = 0;
				}
			}
			break;

			case CMS_Item::StartCube:
			{
				playerInfo.jewel -= quantity;
				if (playerInfo.jewel < 0)
				{
					playerInfo.jewel = 0;
				}
			}
			break;

			// TODO - handle special stuff for other item additions

			default: break;
		}
	}
}

// Reduce the price if we own items within the package
int PlayerDataBlob::GetPackagePrice(const std::string& packageCode, int basePrice) const
{
	int price = basePrice;
	const CMSData& cmsData = Server::GetCMSData();
	auto itr = cmsData.shopPackageDetailLists.find(packageCode);
	if (itr != cmsData.shopPackageDetailLists.end())
	{
		const std::vector<std::reference_wrapper<CMS_Shop_PackageDetail>>& details = itr->second;
		if (!details.empty())
		{
			int x = basePrice / details.size();
			int y = 0;
			for (const auto& detail : details)
			{
				if (!playerState.HasArcadeStage(detail.get().MusicNo))
				{
					++y;
				}
			}
			price -= x * y;
		}
	}
	return price;
}

void PlayerDataBlob::AddPackage(const std::string& packageCode)
{
	const CMSData& cmsData = Server::GetCMSData();
	auto itr = cmsData.shopPackageDetailLists.find(packageCode);
	if (itr != cmsData.shopPackageDetailLists.end())
	{
		const std::vector<std::reference_wrapper<CMS_Shop_PackageDetail>>& details = itr->second;
		for (const auto& detail : details)
		{
			if (!playerState.HasArcadeStage(detail.get().MusicNo))
			{
				PlayerData_ArcadeStage stage;
				stage.MusicNo = detail.get().MusicNo;
				playerState.playerData_ArcadeStage.push_back(stage);
			}
		}
	}
}

void PlayerDataBlob::AddXP(int amount, std::vector<RewardItem>& rewards)
{
	playerInfo.exp += amount;

	// Handle level up
	const CMSData& cmsData = Server::GetCMSData();
	for (const CMS_UserLevel& levelData : cmsData.cms_UserLevel)
	{
		if (levelData.Level > playerInfo.level && playerInfo.exp >= levelData.TotalNeedExp)
		{
			playerInfo.level = levelData.Level;

			AddItem(levelData.RewardItemCode, levelData.RewardItemCount);
			rewards.push_back(RewardItem(levelData.RewardItemCode, levelData.RewardItemCount));
		}
	}
}

// Update the achievement points. will need to call after each acievement unlock
void PlayerDataBlob::RefreshAchievementPoints()
{
	const CMSData& cmsData = Server::GetCMSData();
	for (const Player_Achievement& achiev : playerState.player_Achievement)
	{
		if (achiev.IsClear)
		{
			const CMS_Achievement* achievData = cmsData.FindDataByKey<CMS_Achievement>(achiev.AchievementId);
			if (achievData)
			{
				AchievementPoints += achievData->AchievementScore;
			}
		}
	}
}

int PlayerDataBlob::GetSupportPower(const std::string& supportID) const
{
	int power = 0;
	for (const Player_Support& support : itemData.support)
	{
		if (support.SupportID == supportID)
		{
			const CMSData& cmsData = Server::GetCMSData();
			power = cmsData.GetSupportPower(support.SupportID, support.Level);
		}
	}
	return power;
}

float PlayerDataBlob::GetTitleSupportBonus() const
{
	// TODO - get bonus from equipped title
	return 0.0f;
}

int PlayerDataBlob::GetLPForSong(int musicNo) const
{
	int LP = 0;
	for (const Player_MetaResult_SavePeople& res : playerState.player_MetaResult_SavePeople)
	{
		if (res.ArcadeStageNo == musicNo)
		{
			LP = res.SavePeopleCount;
		}
	}
	return LP;
}

void PlayerDataBlob::SetLPForSong(int musicNo, int LP)
{
	bool found = false;
	for (Player_MetaResult_SavePeople& res : playerState.player_MetaResult_SavePeople)
	{
		if (res.ArcadeStageNo == musicNo)
		{
			res.SavePeopleCount = LP;
			found = true;
			break;
		}
	}
	if (!found)
	{
		Player_MetaResult_SavePeople res;
		res.ArcadeStageNo = musicNo;
		res.SavePeopleCount = LP;
		playerState.player_MetaResult_SavePeople.push_back(res);
	}
}

GameResult* PlayerDataBlob::GetGameResult(int musicNo, int patternNo)
{
	GameResult* result = nullptr;
	for (GameResult& itr : gameResults)
	{
		if (itr.musicNo == musicNo && itr.patternNo == patternNo)
		{
			result = &itr;
			break;
		}
	}
	// Add a new result for this music/pattern
	if (result == nullptr)
	{
		gameResults.push_back(GameResult());
		result = &gameResults.back();
	}
	return result;
}

void PlayerDataBlob::GetItemRewardForSupport(const std::string& supportID, std::vector<RewardItem>& rewards)
{
	Player_Item reward;
	for (const Player_Support& support : itemData.support)
	{
		if (support.SupportID == supportID)
		{
			const CMSData& cmsData = Server::GetCMSData();
			const CMS_Support* supportData = cmsData.FindDataByKey<CMS_Support>(supportID);
			if (supportData)
			{
				const int quantityMult = (support.Grade - 1) * support.Level;
				// TODO - something better + move to data
				switch (supportData->Category)
				{
					case CMS_Support::TIER_1:
						reward = Player_Item("Mic_1", 1 * quantityMult);
						break;

					case CMS_Support::TIER_2:
						reward = Player_Item("Mic_2", 1 * quantityMult);
						break;

					case CMS_Support::TIER_3:
						if (support.Grade < 4)
							reward = Player_Item("Steel_1", 1 * quantityMult);
						else if (support.Grade < 5)
							reward = Player_Item("Steel_2", 1 * quantityMult);
						else
							reward = Player_Item("Steel_3", 1 * quantityMult);
						break;

					case CMS_Support::TIER_5:
						if (support.Grade < 4)
							reward = Player_Item("Fabric_1", 1 * quantityMult);
						else if (support.Grade < 5)
							reward = Player_Item("Fabric_2", 1 * quantityMult);
						else
							reward = Player_Item("Fabric_3", 1 * quantityMult);
						break;

					case CMS_Support::TIER_4:
					case CMS_Support::TIER_6:
						if (support.Grade < 4)
							reward = Player_Item("Chaotic_Coin_C", 1 * quantityMult);
						else if (support.Grade < 5)
							reward = Player_Item("Chaotic_Coin_B", 1 * quantityMult);
						else
							reward = Player_Item("Chaotic_Coin_A", 1 * quantityMult);
						break;

					case CMS_Support::TIER_7:
						reward = Player_Item("StarCube", 1 * quantityMult);
						break;

					case CMS_Support::TIER_8:
						reward = Player_Item("GOLD", 100 * quantityMult);
						break;

					default:
						break;
				}
			}
			break;
		}
	}

	if (reward.Quantity > 0)
	{
		AddItem(reward.ItemCode, reward.Quantity);
		rewards.push_back(RewardItem(reward.ItemCode, reward.Quantity));
	}
}

bool PlayerDataBlob::DoClear(const GameResult& result, ResultRewards& rewards, std::vector<CMS_Support>& supports)
{
	// Update game results
	GameResult* currentResult = GetGameResult(result.musicNo, result.patternNo);
	if (!currentResult)
	{
		return false;
	}

	const CMSData& cmsData = Server::GetCMSData();

	const bool isFirstTimeCleared = !currentResult->isCleared;
	currentResult->isCleared = true;
	currentResult->playCount++;

	// TODO - verify the stuff here looks legit
	if (result.score > currentResult->score)
	{
		currentResult->score = result.score;
		currentResult->maxCombo = result.maxCombo;
		currentResult->judgeTable = result.judgeTable;
		currentResult->enemyBreakCount = result.enemyBreakCount;
		currentResult->rating = result.rating;
		currentResult->enemyRating = result.enemyRating;
		// Just give out the medals for now
		for (size_t i = 0; i < result.acquiredMedalIds.size(); ++i)
		{
			if (result.acquiredMedalIds[i])
			{
				currentResult->acquiredMedalIds[i] = 1;
			}
		}
	}

	// Calc LP
	const CMS_Music_Pattern* pattern = cmsData.GetMusicPattern(result.musicNo, result.patternNo);
	// Bad/missing data?
	if (!pattern)
	{
		return false;
	}
	const CMS_Support_StageSet* supportStageSet = cmsData.FindDataByKey<CMS_Support_StageSet>(result.musicNo);
	if (!supportStageSet)
	{
		return false;
	}

	const int rankLP = result.GetLPFromRank();
	const int levelLP = pattern->Level * 50;
	const int achievLP = AchievementPoints * 10;

	int supportPower = 0;
	supportPower += GetSupportPower(supportStageSet->Left);
	supportPower += GetSupportPower(supportStageSet->LeftMid);
	supportPower += GetSupportPower(supportStageSet->Right);
	supportPower += GetSupportPower(supportStageSet->RightMid);
	supportPower = static_cast<int>(std::ceil(supportPower / 10.0f)); // round up
	const int supportLP = supportPower;

	const int titleBonus = static_cast<int>(GetTitleSupportBonus() * supportPower);

	const int LP = rankLP + levelLP + achievLP + supportLP + titleBonus;
	const int currentLP = GetLPForSong(result.musicNo);
	const int diff = currentLP - LP;
	if (diff > 0)
	{
		AddXP(diff, rewards.BasicRewards);
		SetLPForSong(result.musicNo, LP);
	}

	rewards.Lp = LP;
	rewards.DesigLp = titleBonus;
	rewards.AchivementPointLp = achievLP;
	rewards.SupporterLp = supportLP;
	rewards.StageClearLp = rankLP;

	// Base reward - TODO - should other items drop here too?
	const int goldReward = result.GetGoldForClear();
	AddItem(CMSData::ItemConstants::Gold, goldReward);
	rewards.BasicRewards.push_back(RewardItem(CMSData::ItemConstants::Gold, goldReward));

	// First time clear bonus
	if (isFirstTimeCleared)
	{
		const int firstTimeClearCubes = pattern->Level * 10;
		AddItem(CMSData::ItemConstants::StarCube, firstTimeClearCubes);
		rewards.BasicRewards.push_back(RewardItem(CMSData::ItemConstants::StarCube, firstTimeClearCubes));
	}

	// Support rewards
	GetItemRewardForSupport(supportStageSet->Left, rewards.SupportRewards);
	GetItemRewardForSupport(supportStageSet->LeftMid, rewards.SupportRewards);
	GetItemRewardForSupport(supportStageSet->Right, rewards.SupportRewards);
	GetItemRewardForSupport(supportStageSet->RightMid, rewards.SupportRewards);

	// Support roll
	supports.push_back(cmsData.cms_Support[0]);

	// Save changes
	PlayerDB::SaveAllPlayerData();

	return true;
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

		// Starting gold
		blob->AddItem(CMSData::ItemConstants::Gold, 100);

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
		playerState.SetUserValue("Current_QuestID", 0, "Tutorial010");
		playerState.SetUserValue("FingerTutor_Step", 1, "");
		playerState.SetUserValue("FirstChapter_Order_No", 1, "");
		playerState.SetUserValue("Language", 0, "EN");

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

PlayerDataBlob* PlayerDB::FindPlayerDataBlob(player_id playerID)
{
	PlayerDataBlob* blob = nullptr;
	auto itr = gPlayers.find(playerID);
	if (itr != gPlayers.end())
	{
		blob = itr->second;
	}
	return blob;
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

GameResult* PlayerDB::GetGameResult(player_id playerID, int musicNo, int patternNo)
{
	GameResult* result = nullptr;
	PlayerDataBlob* blob = FindPlayerDataBlob(playerID);
	if (blob)
	{
		result = blob->GetGameResult(musicNo, patternNo);
	}
	return result;
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