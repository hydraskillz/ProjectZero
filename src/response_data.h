#pragma once

#include "json.h"
#include "item_and_shop_data.h"

struct Player_Flag : public Json::ISerializeable
{
	Player_Flag(const std::string& key = "", int value = 0)
		: FlagKey(key)
		, value(value)
	{}

	std::string FlagKey;
	int value;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(FlagKey);
		SERIALIZE_JSON(value);
	}
};

struct Player_UserValue : public Json::ISerializeable
{
	Player_UserValue(const std::string& key = "", int valueInt = 0, const std::string& valueStr = "")
		: ValueKey(key)
		, intValue(valueInt)
		, stringValue(valueStr)
	{}

	std::string ValueKey;
	int intValue;
	std::string stringValue;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(ValueKey);
		SERIALIZE_JSON(intValue);
		SERIALIZE_JSON_IF_NOT_EMPTY(stringValue);
	}
};

struct PlayerData_ArcadeStage : public Json::ISerializeable
{
	PlayerData_ArcadeStage(int musicNo = 0)
		: MusicNo(musicNo)
	{}

	bool operator==(const PlayerData_ArcadeStage& rhs) const { return rhs.MusicNo == MusicNo; }

	int MusicNo;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(MusicNo);
	}
};

struct PlayerData_Story : public Json::ISerializeable
{
	int GroupNo = 1;
	int StoryNo = 1;
	bool isBlock = false;
	bool isClear = false;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(GroupNo);
		SERIALIZE_JSON(StoryNo);
		SERIALIZE_JSON(isBlock);
		SERIALIZE_JSON(isClear);
	}
};

struct Player_Quest : public Json::ISerializeable
{
	Player_Quest(int questID = 0)
		: QuestID(questID)
	{}

	int QuestID;
	bool IsClear = false;
	bool IsReward = false;
	int PerformCount = 0;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(QuestID);
		SERIALIZE_JSON(IsClear);
		SERIALIZE_JSON(IsReward);
		SERIALIZE_JSON(PerformCount);
	}
};

struct Player_Achievement : public Json::ISerializeable
{
	void Serialize(Json::Serializer& serializer) override
	{
	}
};

struct Player_AchievementReward : public Json::ISerializeable
{
	void Serialize(Json::Serializer& serializer) override
	{
	}
};

struct NoticeMessage : public Json::ISerializeable
{
	void Serialize(Json::Serializer& serializer) override
	{
	}
};

struct Player_ChaoticMission : public Json::ISerializeable
{
	void Serialize(Json::Serializer& serializer) override
	{
	}
};

struct Player_ChaoticStage : public Json::ISerializeable
{
	void Serialize(Json::Serializer& serializer) override
	{
	}
};

struct Player_Episode : public Json::ISerializeable
{
	void Serialize(Json::Serializer& serializer) override
	{
	}
};

struct Player_MetaResult_SavePeople : public Json::ISerializeable
{
	void Serialize(Json::Serializer& serializer) override
	{
	}
};

struct Player_Statistics : public Json::ISerializeable
{
	void Serialize(Json::Serializer& serializer) override
	{
	}
};

// A blob of random state data
struct ResponseData : public Json::ISerializeable
{
	std::vector<Player_Flag> player_Flag;
	std::vector<Player_UserValue> player_UserValue;
	std::vector<PlayerData_ArcadeStage> playerData_ArcadeStage;
	std::vector<PlayerData_Story> playerData_Story;
	std::vector<Player_Quest> player_Quest;
	std::vector<Player_Achievement> player_Achievement;
	std::vector<Player_AchievementReward> player_AchievementReward;
	std::vector<NoticeMessage> noticeMessage;
	std::vector<Player_ChaoticMission> player_ChaoticMission;
	std::vector<Player_ChaoticStage> player_ChaoticStage;
	std::vector<Player_Episode> player_Episode;
	std::vector<Player_MetaResult_SavePeople> player_MetaResult_SavePeople;
	std::vector<Player_Statistics> player_Statistics;

	void SetFlag(const std::string& key, int value)
	{
		bool found = false;
		for (auto& itr : player_Flag)
		{
			if (itr.FlagKey == key)
			{
				itr.value = value;
				found = true;
				break;
			}
		}
		if (!found)
		{
			player_Flag.push_back(Player_Flag(key, value));
		}
	}

	void SetUserValue(const std::string& key, int valueInt, const std::string& valueStr = "")
	{
		bool found = false;
		for (auto& itr : player_UserValue)
		{
			if (itr.ValueKey == key)
			{
				itr.intValue = valueInt;
				itr.stringValue = valueStr;
				found = true;
				break;
			}
		}
		if (!found)
		{
			player_UserValue.push_back(Player_UserValue(key, valueInt, valueStr));
		}
	}

	bool HasArcadeStage(int musicNo) const
	{
		return std::find(playerData_ArcadeStage.begin(), playerData_ArcadeStage.end(), musicNo) != playerData_ArcadeStage.end();
	}

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON_IF_NOT_EMPTY(player_Flag);
		SERIALIZE_JSON_IF_NOT_EMPTY(player_UserValue);
		SERIALIZE_JSON_IF_NOT_EMPTY(playerData_ArcadeStage);
		SERIALIZE_JSON_IF_NOT_EMPTY(playerData_Story);
		SERIALIZE_JSON_IF_NOT_EMPTY(player_Quest);
		SERIALIZE_JSON_IF_NOT_EMPTY(player_Achievement);
		SERIALIZE_JSON_IF_NOT_EMPTY(player_AchievementReward);
		SERIALIZE_JSON_IF_NOT_EMPTY(noticeMessage);
		SERIALIZE_JSON_IF_NOT_EMPTY(player_ChaoticMission);
		SERIALIZE_JSON_IF_NOT_EMPTY(player_ChaoticStage);
		SERIALIZE_JSON_IF_NOT_EMPTY(player_Episode);
		SERIALIZE_JSON_IF_NOT_EMPTY(player_MetaResult_SavePeople);
		SERIALIZE_JSON_IF_NOT_EMPTY(player_Statistics);
	}
};

// This is pretty fucking dumb
struct ResponseDataClearResult : public Json::ISerializeable
{
	std::vector<Player_Item> items;
	std::vector<Player_Quest> player_Quest;
	std::vector<Player_MetaResult_SavePeople> player_MetaResult_SavePeople;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(items);
		SERIALIZE_JSON(player_Quest);
		SERIALIZE_JSON(player_MetaResult_SavePeople);
	}
};