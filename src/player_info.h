#pragma once

#include "json.h"
#include "date_time.h"

struct PlayerInfo : public Json::ISerializeable
{
	std::string name;				// Player name
	std::string Language = "EN";	// unused
	int jewel = 0;					// star cubes
	int gold = 0;					// gold
	int level = 0;					// level/rank
	int exp = 0;					// experience
	int OpenStoryNo = 1;			// the current number of story missions?
	int ClearStoryNo = 0;			// how many story missions have been cleared?
	int MusicByGoldCount = 0;		// ???
	int TutorialNo = 0;				// 0 = not started, 1 = arcade tutorial, 2 = tutorial complete
	int SupportByGoldCount = 0;		// how many support you bought with gold today?
	int IsRating = 0;				// ???
	int AchievementScore = 0;		// achievment points
	int NameChangeCount = 0;		// how many times you changed your name
	DateTime LastActionDate;		// last login
	DateTime SupportByGoldTime;		// last time you bought a support with gold?
	DateTime CreateDate;			// when your accout was created

	// Non-serialized values
	int currentStory = -1;			// The story mission they are playing

	void Serialize(Json::Serializer& serializer) override
	{
		// Skip sending if name is missing
		if (serializer.IsWriting() && name.empty())
		{
			return;
		}

		// Skip language if not set. I don't think it ever gets set, which is why the voices ingame are korean always
		// TODO: set this correctly?
		if (serializer.IsWriting() && !Language.empty())
		{
			SERIALIZE_JSON(Language);
		}

		SERIALIZE_JSON(name);
		SERIALIZE_JSON(jewel);
		SERIALIZE_JSON(gold);
		SERIALIZE_JSON(level);
		SERIALIZE_JSON(exp);
		SERIALIZE_JSON(OpenStoryNo);
		SERIALIZE_JSON(ClearStoryNo);
		SERIALIZE_JSON(MusicByGoldCount);
		SERIALIZE_JSON(TutorialNo);
		SERIALIZE_JSON(SupportByGoldCount);
		SERIALIZE_JSON(IsRating);
		SERIALIZE_JSON(AchievementScore);
		SERIALIZE_JSON(NameChangeCount);
		SERIALIZE_JSON(LastActionDate);
		SERIALIZE_JSON(SupportByGoldTime);
		SERIALIZE_JSON(CreateDate);
	}
};