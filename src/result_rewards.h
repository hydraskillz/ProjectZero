#pragma once

#include "json.h"

#include <vector>
#include <string>

struct RewardItem : public Json::ISerializeable
{
	RewardItem(const std::string& itemCode = "", int count = 0)
		: ItemCode(itemCode)
		, ItemCount(count)
	{}

	std::string ItemCode;		// matches CMS data
	int ItemCount;				// quantity

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(ItemCode);
		SERIALIZE_JSON(ItemCount);
	}
};

struct ResultRewards : public Json::ISerializeable
{
	int Lp = 0;										// total LP
	int StageClearLp = 0;							// rank LP
	int AchivementPointLp = 0;						// achievment LP
	int SupporterLp = 0;							// supporter LP
	int DesigLp = 0;								// title LP
	std::vector<float> SupporterLpDesigFactors;		// ???
	std::vector<RewardItem> BasicRewards;			// rewards from clearing
	std::vector<RewardItem> SupportRewards;			// bonus rewards from supports

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(Lp);
		SERIALIZE_JSON(StageClearLp);
		SERIALIZE_JSON(AchivementPointLp);
		SERIALIZE_JSON(SupporterLp);
		SERIALIZE_JSON(DesigLp);
		SERIALIZE_JSON(SupporterLpDesigFactors);
		SERIALIZE_JSON(BasicRewards);
		SERIALIZE_JSON(SupportRewards);
	}
};