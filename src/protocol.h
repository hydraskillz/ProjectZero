#pragma once

#include <string>
#include <sstream>
#include "json.h"
#include "player_info.h"
#include "game_result.h"
#include "result_rewards.h"
#include "support.h"
#include "support_gacha_result.h"
#include "cms_data.h"
#include "iap_info.h"
#include "attendance.h"
#include "item_and_shop_data.h"
#include "mail.h"
#include "rank_data.h"
#include "response_data.h"
#include "player_id.h"

#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

namespace Protocol
{
	struct Base : public Json::ISerializeable
	{
		virtual const char* GetID() const = 0;

		void Serialize(Json::Serializer& serializer) override
		{
			if (serializer.IsWriting())
			{
				Json::Write(serializer.GetWriter(), "action", GetID());
			}
		}
	};

	struct Request : public Base
	{
		void Serialize(Json::Serializer& serializer) override
		{
			Base::Serialize(serializer);
		}

		// { "action" : action }
		void ToJson(rapidjson::Writer<rapidjson::StringBuffer>& writer) const
		{
			writer.StartObject();

			Json::Serializer s(writer);
			const_cast<Request&>(*this).Serialize(s);

			writer.EndObject();
		}
	};

	struct Response : public Base
	{
		std::string status;
		int status_code = -1;
		std::string status_value;

		void Serialize(Json::Serializer& serializer) override
		{
			Base::Serialize(serializer);
			SERIALIZE_JSON(status);
			SERIALIZE_JSON(status_code);
			// Only write value if the code was an error
			if (status_code != 0)
			{
				SERIALIZE_JSON(status_value);
			}
		}

		// { "action" : action }
		void ToJson(rapidjson::Writer<rapidjson::StringBuffer>& writer) const
		{
			writer.StartObject();

			Json::Serializer s(writer);
			const_cast<Response&>(*this).Serialize(s);

			writer.EndObject();
		}

		virtual bool HandleRequest(const rapidjson::Value& action, player_id playerID) { return true; }

		void SetSuccess()
		{
			status = "success";
			status_code = 0;
		}

		void SetFailure()
		{
			status = "success";			// must always report success or the client will hang
			status_code = 0x0001869F;	// Server error
		}
	};

	namespace CheckServerConnection
	{
		constexpr const char* ID = "check_server_status";

		struct Aparams : public Json::ISerializeable
		{
			std::string platform;
			std::string version;

			void Serialize(Json::Serializer& serializer) override
			{
				SERIALIZE_JSON(platform);
				SERIALIZE_JSON(version);
			}
		};

		struct Request : Protocol::Request
		{
			Aparams aparams;

			const char* GetID() const override { return ID; }

			// {"aparams": {"platform" : platform, "version": version}}
			void Serialize(Json::Serializer& serializer) override
			{
				Protocol::Request::Serialize(serializer);
				SERIALIZE_JSON(aparams);
			}
		};

		struct Response : Protocol::Response
		{
			std::string version;
			std::string store;
			std::string status_msg;

			const char* GetID() const override { return ID; }

			void Serialize(Json::Serializer& serializer) override
			{
				Protocol::Response::Serialize(serializer);
				SERIALIZE_JSON(version);
				SERIALIZE_JSON(store);
				SERIALIZE_JSON(status_msg);
			}

			bool HandleRequest(const rapidjson::Value& action, player_id playerID) override;
		};
	}

	namespace CreateAndLogin
	{
		constexpr const char* ID = "create_and_login";

		struct Request : Protocol::Request
		{
			const char* GetID() const override { return ID; }
		};

		struct Response : Protocol::Response
		{
			::player_id player_id = INVALID_PLAYER_ID;

			const char* GetID() const override { return ID; }

			void Serialize(Json::Serializer& serializer) override
			{
				Protocol::Response::Serialize(serializer);
				SERIALIZE_JSON(player_id);
			}

			bool HandleRequest(const rapidjson::Value& action, ::player_id playerID) override;
		};
	}

	namespace Login
	{
		constexpr const char* ID = "login";

		struct Request : Protocol::Request
		{
			const char* GetID() const override { return ID; }
		};

		struct Response : Protocol::Response
		{
			const char* GetID() const override { return ID; }
			bool HandleRequest(const rapidjson::Value& action, player_id playerID) override;
		};
	}

	namespace CreateAndLoginPlatform
	{
		constexpr const char* ID = "create_and_login_platform";

		struct Aparams : public Json::ISerializeable
		{
			player_id pid = INVALID_PLAYER_ID;
			std::string platform;
			std::string id;
			std::string username;

			void Serialize(Json::Serializer& serializer) override
			{
				SERIALIZE_JSON(pid);
				SERIALIZE_JSON(platform);
				SERIALIZE_JSON(id);
				SERIALIZE_JSON(username);
			}
		};

		struct Request : Protocol::Request
		{
			Aparams aparams;

			const char* GetID() const override { return ID; }

			// {"aparams": {"platform" : platform, "version": version}}
			void Serialize(Json::Serializer& serializer) override
			{
				Protocol::Request::Serialize(serializer);
				SERIALIZE_JSON(aparams);
			}
		};

		struct Response : Protocol::Response
		{
			::player_id player_id = INVALID_PLAYER_ID;
			bool new_player = false;

			const char* GetID() const override { return ID; }

			void Serialize(Json::Serializer& serializer) override
			{
				Protocol::Response::Serialize(serializer);
				SERIALIZE_JSON(player_id);
				SERIALIZE_JSON(new_player);
			}
			bool HandleRequest(const rapidjson::Value& action, ::player_id playerID) override;
		};
	}

	namespace GetPlayerInfo
	{
		constexpr const char* ID = "get_player_info";

		struct Request : Protocol::Request
		{
			const char* GetID() const override { return ID; }
		};

		struct Response : Protocol::Response
		{
			PlayerInfo player_info;

			const char* GetID() const override { return ID; }

			void Serialize(Json::Serializer& serializer) override
			{
				Protocol::Response::Serialize(serializer);
				SERIALIZE_JSON(player_info);
			}

			bool HandleRequest(const rapidjson::Value& action, player_id playerID) override;
		};
	}

	namespace GetGameResult
	{
		constexpr const char* ID = "get_game_result";

		struct Request : Protocol::Request
		{
			const char* GetID() const override { return ID; }
		};

		struct Response : Protocol::Response
		{
			std::vector<GameResult> gameResults;

			const char* GetID() const override { return ID; }

			void Serialize(Json::Serializer& serializer) override
			{
				Protocol::Response::Serialize(serializer);
				SERIALIZE_JSON(gameResults);
			}

			bool HandleRequest(const rapidjson::Value& action, player_id playerID) override;
		};
	}

	namespace DoClearStandard
	{
		constexpr const char* ID = "do_clear_standard";

		struct Aparams : public Json::ISerializeable
		{
			GameResult result;
			std::string EquipedDesignation;

			void Serialize(Json::Serializer& serializer) override
			{
				SERIALIZE_JSON(result);
				SERIALIZE_JSON(EquipedDesignation);
			}
		};

		struct Request : Protocol::Request
		{
			Aparams aparams;

			const char* GetID() const override { return ID; }

			void Serialize(Json::Serializer& serializer) override
			{
				Protocol::Request::Serialize(serializer);
				SERIALIZE_JSON(aparams);
			}
		};

		struct Response : Protocol::Response
		{
			PlayerInfo PlayerInfo;
			std::vector<Player_Item> ItemList;
			std::vector<Player_Support> SupportList;
			std::vector<Player_Achievement> PlayerAchievement;
			std::vector<Player_Quest> PlayerQuest;
			ResponseData ResponseData;
			ResultRewards ResultRewards;
			bool isGetSuppoterCard;
			SupportGachaResult DoSupportGachaResult;
			CMS_Support support;

			const char* GetID() const override { return ID; }

			void Serialize(Json::Serializer& serializer) override
			{
				Protocol::Response::Serialize(serializer);
				SERIALIZE_JSON(PlayerInfo);
				SERIALIZE_JSON(isGetSuppoterCard);
				SERIALIZE_JSON(ResultRewards);
				SERIALIZE_JSON(ItemList);
				SERIALIZE_JSON(SupportList);
				SERIALIZE_JSON(PlayerAchievement);
				SERIALIZE_JSON(PlayerQuest);
				SERIALIZE_JSON(ResponseData);
				SERIALIZE_JSON(ResultRewards);
				SERIALIZE_JSON(DoSupportGachaResult);
				SERIALIZE_JSON(support);
			}

			bool HandleRequest(const rapidjson::Value& action, player_id playerID) override;
		};
	}

	namespace DoSupportGacha
	{
		constexpr const char* ID = "do_support_gacha";

		struct Aparams : public Json::ISerializeable
		{
			std::string gachaWay;

			void Serialize(Json::Serializer& serializer) override
			{
				SERIALIZE_JSON(gachaWay);
			}
		};

		struct Request : Protocol::Request
		{
			Aparams aparams;

			const char* GetID() const override { return ID; }

			void Serialize(Json::Serializer& serializer) override
			{
				Protocol::Request::Serialize(serializer);
				SERIALIZE_JSON(aparams);
			}
		};

		struct Response : Protocol::Response
		{
			SupportGachaResult DoSupportGachaResult;
			std::vector<Player_Quest> PlayerQuest;
			std::vector<Player_Achievement> PlayerAchievement;

			const char* GetID() const override { return ID; }

			void Serialize(Json::Serializer& serializer) override
			{
				Protocol::Response::Serialize(serializer);
				SERIALIZE_JSON(DoSupportGachaResult);
				SERIALIZE_JSON(PlayerQuest);
				SERIALIZE_JSON(PlayerAchievement);
			}

			bool HandleRequest(const rapidjson::Value& action, player_id playerID) override;
		};
	}

	namespace DoSupportLevelUp
	{
		constexpr const char* ID = "do_supporter_levelup";

		struct Aparams : public Json::ISerializeable
		{
			std::string supportID;

			void Serialize(Json::Serializer& serializer) override
			{
				SERIALIZE_JSON(supportID);
			}
		};

		struct Request : Protocol::Request
		{
			Aparams aparams;

			const char* GetID() const override { return ID; }

			void Serialize(Json::Serializer& serializer) override
			{
				Protocol::Request::Serialize(serializer);
				SERIALIZE_JSON(aparams);
			}
		};

		struct Response : Protocol::Response
		{
			// TODO

			const char* GetID() const override { return ID; }
			bool HandleRequest(const rapidjson::Value& action, player_id playerID) override;
		};
	}

	namespace DoGemSupportLevelUp
	{
		constexpr const char* ID = "do_gemsupporter_levelup";

		struct Aparams : public Json::ISerializeable
		{
			std::string supportID;
			std::vector<SupportMaterial> materialList;

			void Serialize(Json::Serializer& serializer) override
			{
				SERIALIZE_JSON(supportID);
				SERIALIZE_JSON(materialList);
			}
		};

		struct Request : Protocol::Request
		{
			Aparams aparams;

			const char* GetID() const override { return ID; }

			void Serialize(Json::Serializer& serializer) override
			{
				Protocol::Request::Serialize(serializer);
				SERIALIZE_JSON(aparams);
			}
		};

		struct Response : Protocol::Response
		{
			// TODO

			const char* GetID() const override { return ID; }
			bool HandleRequest(const rapidjson::Value& action, player_id playerID) override;
		};
	}

	namespace CheckUpdateTable
	{
		constexpr const char* ID = "check_update_table";

		struct Aparams : public Json::ISerializeable
		{
			std::vector<CMS_Table_Update> TableList;

			void Serialize(Json::Serializer& serializer) override
			{
				SERIALIZE_JSON(TableList);
			}
		};

		struct Request : Protocol::Request
		{
			Aparams aparams;

			const char* GetID() const override { return ID; }

		};

		struct Response : Protocol::Response
		{
			CMSData TableList;

			const char* GetID() const override { return ID; }

			void Serialize(Json::Serializer& serializer) override
			{
				Protocol::Response::Serialize(serializer);
				SERIALIZE_JSON(TableList);
			}
			bool HandleRequest(const rapidjson::Value& action, player_id playerID) override;
		};
	}

	namespace GetMasterShopInfo
	{
		constexpr const char* ID = "master_info_shop";

		struct Aparams : public Json::ISerializeable
		{
			void Serialize(Json::Serializer& serializer) override
			{
			}
		};

		struct Request : Protocol::Request
		{
			Aparams aparams;

			const char* GetID() const override { return ID; }
		};

		struct Response : Protocol::Response
		{
			std::vector<InAppPurchaseInfo> purchaseInfoList;

			const char* GetID() const override { return ID; }

			void Serialize(Json::Serializer& serializer) override
			{
				Protocol::Response::Serialize(serializer);
				SERIALIZE_JSON(purchaseInfoList);
			}
			bool HandleRequest(const rapidjson::Value& action, player_id playerID) override;
		};
	}

	namespace GetUserStatusInfo
	{
		constexpr const char* ID = "get_user_status_info";

		struct Request : Protocol::Request
		{
			const char* GetID() const override { return ID; }

		};

		struct Response : Protocol::Response
		{
			ResponseData responseData;

			const char* GetID() const override { return ID; }

			void Serialize(Json::Serializer& serializer) override
			{
				Protocol::Response::Serialize(serializer);
				SERIALIZE_JSON(responseData);
			}
			bool HandleRequest(const rapidjson::Value& action, player_id playerID) override;
		};
	}

	namespace GetUserItemAndShopInfo
	{
		constexpr const char* ID = "get_user_item_shop_info";

		struct Request : Protocol::Request
		{
			const char* GetID() const override { return ID; }
		};

		struct Response : Protocol::Response
		{
			UserItemAndShop_Datainfo responseData;

			const char* GetID() const override { return ID; }

			void Serialize(Json::Serializer& serializer) override
			{
				Protocol::Response::Serialize(serializer);
				SERIALIZE_JSON(responseData);
			}
			bool HandleRequest(const rapidjson::Value& action, player_id playerID) override;
		};
	}

	namespace GetAttendance
	{
		constexpr const char* ID = "get_attendance";

		struct Request : Protocol::Request
		{
			const char* GetID() const override { return ID; }
		};

		struct Response : Protocol::Response
		{
			AttendanceResult result;
			PlayerInfo player_info;
			UserItemAndShop_Datainfo userItemAndShop_Datainfo;

			const char* GetID() const override { return ID; }

			void Serialize(Json::Serializer& serializer) override
			{
				Protocol::Response::Serialize(serializer);
				SERIALIZE_JSON(result);
				SERIALIZE_JSON(player_info);
				SERIALIZE_JSON(userItemAndShop_Datainfo);
			}
			bool HandleRequest(const rapidjson::Value& action, player_id playerID) override;
		};
	}

	namespace GetPlayerMailBox
	{
		constexpr const char* ID = "get_player_mailbox";

		struct Request : Protocol::Request
		{
			const char* GetID() const override { return ID; }
		};

		struct Response : Protocol::Response
		{
			std::vector<Mail> mailList;

			const char* GetID() const override { return ID; }

			void Serialize(Json::Serializer& serializer) override
			{
				Protocol::Response::Serialize(serializer);
				SERIALIZE_JSON(mailList);
			}
			bool HandleRequest(const rapidjson::Value& action, player_id playerID) override;
		};
	}

	namespace GetRankInfo
	{
		constexpr const char* ID = "get_rank_info";

		struct Request : Protocol::Request
		{
			const char* GetID() const override { return ID; }
		};

		struct Response : Protocol::Response
		{
			int MyRank;
			std::vector<RankData> RankList;

			const char* GetID() const override { return ID; }

			void Serialize(Json::Serializer& serializer) override
			{
				Protocol::Response::Serialize(serializer);
				SERIALIZE_JSON(MyRank);
				SERIALIZE_JSON(RankList);
			}
			bool HandleRequest(const rapidjson::Value& action, player_id playerID) override;
		};
	}

	namespace ReportFlagChange
	{
		constexpr const char* ID = "report_flag_change";

		struct Aparams : public Json::ISerializeable
		{
			std::string flagKey;
			int flagValue;

			void Serialize(Json::Serializer& serializer) override
			{
				SERIALIZE_JSON(flagKey);
				SERIALIZE_JSON(flagValue);
			}
		};

		struct Request : Protocol::Request
		{
			const char* GetID() const override { return ID; }

			Aparams aparams;

			void Serialize(Json::Serializer& serializer) override
			{
				Protocol::Request::Serialize(serializer);
				SERIALIZE_JSON(aparams);
			}
		};

		struct Response : Protocol::Response
		{
			const char* GetID() const override { return ID; }
			bool HandleRequest(const rapidjson::Value& action, player_id playerID) override;
		};
	}

	namespace ReportUserValueChange
	{
		constexpr const char* ID = "report_user_value_change";

		struct Aparams : public Json::ISerializeable
		{
			int isString;
			std::string valueKey;
			int intValue;
			std::string strValue;

			void Serialize(Json::Serializer& serializer) override
			{
				SERIALIZE_JSON(isString);
				SERIALIZE_JSON(valueKey);
				SERIALIZE_JSON(intValue);
				SERIALIZE_JSON(strValue);
			}
		};

		struct Request : Protocol::Request
		{
			const char* GetID() const override { return ID; }

			Aparams aparams;

			void Serialize(Json::Serializer& serializer) override
			{
				Protocol::Request::Serialize(serializer);
				SERIALIZE_JSON(aparams);
			}
		};

		struct Response : Protocol::Response
		{
			const char* GetID() const override { return ID; }
			bool HandleRequest(const rapidjson::Value& action, player_id playerID) override;
		};
	}

	namespace DoStartStory
	{
		constexpr const char* ID = "do_start_story";

		struct Aparams : public Json::ISerializeable
		{
			int StoryNo;

			void Serialize(Json::Serializer& serializer) override
			{
				SERIALIZE_JSON(StoryNo);
			}
		};

		struct Request : Protocol::Request
		{
			const char* GetID() const override { return ID; }

			Aparams aparams;

			void Serialize(Json::Serializer& serializer) override
			{
				Protocol::Request::Serialize(serializer);
				SERIALIZE_JSON(aparams);
			}
		};

		struct Response : Protocol::Response
		{
			const char* GetID() const override { return ID; }

			PlayerInfo PlayerInfo;
			std::vector<Player_Item> ItemList;

			void Serialize(Json::Serializer& serializer) override
			{
				Protocol::Response::Serialize(serializer);
				SERIALIZE_JSON(PlayerInfo);
				SERIALIZE_JSON(ItemList);
			}
			bool HandleRequest(const rapidjson::Value& action, player_id playerID) override;
		};
	}

	namespace DoClearStory
	{
		constexpr const char* ID = "do_clear_story";

		struct Aparams : public Json::ISerializeable
		{
			GameResult result;
			std::string EquipedDesignation;

			void Serialize(Json::Serializer& serializer) override
			{
				SERIALIZE_JSON(result);
				SERIALIZE_JSON(EquipedDesignation);
			}
		};

		struct Request : Protocol::Request
		{
			const char* GetID() const override { return ID; }

			Aparams aparams;

			void Serialize(Json::Serializer& serializer) override
			{
				Protocol::Request::Serialize(serializer);
				SERIALIZE_JSON(aparams);
			}
		};

		struct Response : Protocol::Response
		{
			const char* GetID() const override { return ID; }

			PlayerInfo PlayerInfo;
			std::vector<PlayerData_ArcadeStage> StageList;
			std::vector<Player_Item> ItemList;
			std::vector<Player_Support> SupportList;
			std::vector<Player_Achievement> PlayerAchievement;
			std::vector<Player_Quest> PlayerQuest;
			ResponseDataClearResult ResponseData;
			ResultRewards ResultRewards;
			bool isGetSuppoterCard;
			SupportGachaResult DoSupportGachaResult;

			void Serialize(Json::Serializer& serializer) override
			{
				Protocol::Response::Serialize(serializer);
				SERIALIZE_JSON(PlayerInfo);
				SERIALIZE_JSON(StageList);
				SERIALIZE_JSON(ItemList);
				SERIALIZE_JSON(SupportList);
				SERIALIZE_JSON(PlayerAchievement);
				SERIALIZE_JSON(PlayerQuest);
				SERIALIZE_JSON(ResponseData);
				SERIALIZE_JSON(ResultRewards);
				SERIALIZE_JSON(isGetSuppoterCard);
				SERIALIZE_JSON(DoSupportGachaResult);
			}
			bool HandleRequest(const rapidjson::Value& action, player_id playerID) override;
		};
	}

	namespace BuyArcade
	{
		constexpr const char* ID = "buy_arcade";

		struct Aparams : public Json::ISerializeable
		{
			std::string buyWay;
			int arcadeStageNo;

			void Serialize(Json::Serializer& serializer) override
			{
				SERIALIZE_JSON(buyWay);
				SERIALIZE_JSON(arcadeStageNo);
			}
		};

		struct Request : Protocol::Request
		{
			const char* GetID() const override { return ID; }

			Aparams aparams;

			void Serialize(Json::Serializer& serializer) override
			{
				Protocol::Request::Serialize(serializer);
				SERIALIZE_JSON(aparams);
			}
		};

		struct Response : Protocol::Response
		{
			const char* GetID() const override { return ID; }

			struct BuyArcadeResult : public Json::ISerializeable
			{
				bool isValidRequest = false;
				PlayerInfo playerInfo;
				std::vector<Player_UserValue> userValueList;
				std::vector<Player_Item> items;
				std::vector<PlayerData_ArcadeStage> playerArcadeList;
				std::vector<PlayerData_Story> playerStoryList;
				std::vector<Player_Quest> playerQuestList;

				void Serialize(Json::Serializer& serializer) override
				{
					SERIALIZE_JSON(isValidRequest);
					SERIALIZE_JSON(playerInfo);
					SERIALIZE_JSON(userValueList);
					SERIALIZE_JSON(items);
					SERIALIZE_JSON(playerArcadeList);
					SERIALIZE_JSON(playerStoryList);
					SERIALIZE_JSON(playerQuestList);
				}
			};

			BuyArcadeResult buyArcadeResult;
			
			void Serialize(Json::Serializer& serializer) override
			{
				Protocol::Response::Serialize(serializer);
				SERIALIZE_JSON(buyArcadeResult);
			}
			bool HandleRequest(const rapidjson::Value& action, player_id playerID) override;
		};
	}

	// Messages to add
	// buy_package
	// change username
	// clear_chaotic
	// clear_episode
	// clear_standard
	// start_chaotic
	// start_episode
	// start_standard
	// support level up
	// achievment reward
	// mail
	// quest reward
	// support buy
	// get weekly rating
}