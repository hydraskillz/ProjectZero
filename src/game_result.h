#pragma once

#include "json.h"
#include <vector>
#include <array>

struct GameResult : public Json::ISerializeable
{
	int rankIndex = -1;						// ranking index, i.e. SS, S, A, B, C
	int musicNo = 0;						// song id
	int patternNo = 0;						// chart id
	int64_t score = 0;						// score
	int maxCombo = 0;						// max combo
	std::array<int, 3> judgeTable;			// miss/good/perfect
	int enemyBreakCount = 0;				// kill count
	bool isCleared = false;					// cleared?
	float rating = 0.0f;					// accuracy?
	float enemyRating = 0.0f;				// kill ratio?
	float leftHealth = 0.0f;				// enemy health?
	int feverCount = 0;						// fever activations
	int mineTouchCount = 0;					// mines?
	int playCount = 0;						// total times played
	std::array<int, 4> acquiredMedalIds;	// metals acquired

	GameResult()
	{
		judgeTable[0] = 0;
		judgeTable[1] = 0;
		judgeTable[2] = 0;

		acquiredMedalIds[0] = 0;
		acquiredMedalIds[1] = 0;
		acquiredMedalIds[2] = 0;
		acquiredMedalIds[3] = 0;
	}

	int GetLPFromRank() const
	{
		int LP = 0;
		switch (rankIndex)
		{
			case 5: LP = 9000; break;
			case 4: LP = 8500; break;
			case 3: LP = 8000; break;
			case 2: case 1: LP = 7000; break;
			default: break;
		}
		return LP;
	}

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(musicNo);
		SERIALIZE_JSON(patternNo);
		SERIALIZE_JSON(score);
		SERIALIZE_JSON(maxCombo);
		SERIALIZE_JSON(judgeTable);
		SERIALIZE_JSON(enemyBreakCount);
		SERIALIZE_JSON(isCleared);
		SERIALIZE_JSON(rankIndex);
		SERIALIZE_JSON(rating);
		SERIALIZE_JSON(enemyRating);
		SERIALIZE_JSON(leftHealth);
		SERIALIZE_JSON(feverCount);
		SERIALIZE_JSON(mineTouchCount);
		SERIALIZE_JSON(playCount);
		SERIALIZE_JSON(acquiredMedalIds);
	}
};
