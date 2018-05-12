#pragma once

#include <string>

struct SupportMaterial : public Json::ISerializeable
{
	std::string supportID;
	int count;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(supportID);
		SERIALIZE_JSON(count);
	}
};
