#pragma once

#include "json.h"

struct Mail : public Json::ISerializeable
{
	void Serialize(Json::Serializer& serializer) override
	{}
};