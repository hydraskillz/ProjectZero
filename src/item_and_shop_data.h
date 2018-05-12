#pragma once

#include "json.h"

struct PlayerData_Shop_Package : public Json::ISerializeable
{
	std::string PackageCode;
	//DateTime BuyDate;
	std::string HowBuy;

	void Serialize(Json::Serializer& serializer) override
	{}
};

struct Player_Item : public Json::ISerializeable
{
	Player_Item(const std::string itemCode = "", int quantity = 1)
		: ItemCode(itemCode)
		, Quantity(quantity)
	{}

	std::string ItemCode;
	int Quantity;
	//DateTime InsertDate;
	//DateTime UpdateDate;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(ItemCode);
		SERIALIZE_JSON(Quantity);
	}
};

struct Player_Support : public Json::ISerializeable
{
	Player_Support(const std::string& ID = "")
		: SupportID(ID)
	{}

	std::string SupportID;
	int Grade = 2;
	int Level = 1;
	int Quantity = 0;
	int NextRequireQuantity = 10;
//	DateTime InsertDate;
//	DateTime UpdateDate;
//	DateTime LastUseDate;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(SupportID);
		SERIALIZE_JSON(Grade);
		SERIALIZE_JSON(Level);
		SERIALIZE_JSON(Quantity);
		SERIALIZE_JSON(NextRequireQuantity);
	}
};

struct UserItemAndShop_Datainfo : public Json::ISerializeable
{
	std::vector<PlayerData_Shop_Package> shopPackages;
	std::vector<Player_Item> items;
	std::vector<Player_Support> support;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(shopPackages);
		SERIALIZE_JSON(items);
		SERIALIZE_JSON(support);
	}
};