#pragma once

#include "cms_data.h"
#include "player_info.h"
#include "response_data.h"

struct SupportGachaResult : public Json::ISerializeable
{
	bool isValidRequest;
	std::vector<CMS_Support> resultSupporter;
	std::vector<NoticeMessage> noticeMessages;
	PlayerInfo playerInfo;
	std::vector<Player_Item> items;
	std::vector<Player_Support> support;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(isValidRequest);
		SERIALIZE_JSON(resultSupporter);
		SERIALIZE_JSON(noticeMessages);
		SERIALIZE_JSON(playerInfo);
		SERIALIZE_JSON(items);
		SERIALIZE_JSON(support);
	}
};

struct SupportGachaResultClear : public Json::ISerializeable
{
	std::vector<CMS_Support> resultSupporter;
	std::vector<Player_Support> support;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(resultSupporter);
		SERIALIZE_JSON(support);
	}
};