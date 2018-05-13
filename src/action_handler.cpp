#include "action_handler.h"
#include "protocol.h"

#include <map>

namespace
{
	typedef Protocol::Response*(*ResponseBuilder)();
	std::map<std::string, ResponseBuilder> gBuilders;

	template<typename T>
	Protocol::Response* BuilderTemplate()
	{
		return new T;
	}

	std::string BuildMessageString(const Protocol::Response& req)
	{
		return Json::Write([&req](auto& writer)
		{
			const Protocol::Response* actions[] = { &req };
			Json::Write(writer, "actions", actions);
		});
	}
}

void InitializeHandlers()
{
	if (gBuilders.empty())
	{
#define BUILDER(N) gBuilders[Protocol:: N ::ID	] = BuilderTemplate<Protocol:: N ::Response>

		BUILDER(BuyArcade);
		BUILDER(CheckServerConnection);
		BUILDER(CheckUpdateTable);
		BUILDER(CreateAndLogin);
		BUILDER(CreateAndLoginPlatform);
		BUILDER(DoClearStandard);
		BUILDER(DoClearStory);
		BUILDER(DoGemSupportLevelUp);
		BUILDER(DoStartStory);
		BUILDER(DoSupportGacha);
		BUILDER(DoSupportLevelUp);
		BUILDER(GetAttendance);
		BUILDER(GetGameResult);
		BUILDER(GetMasterShopInfo);
		BUILDER(GetPlayerInfo);
		BUILDER(GetPlayerMailBox);
		BUILDER(GetRankInfo);
		BUILDER(GetUserItemAndShopInfo);
		BUILDER(GetUserStatusInfo);
		BUILDER(Login);
		BUILDER(ReportFlagChange);
		BUILDER(ReportUserValueChange);
		
#undef BUILDER
	}
}

std::string HandleAction(const std::string& actionName, const rapidjson::Value& action, player_id playerID)
{
	std::string body;
	auto itr = gBuilders.find(actionName);
	if (itr != gBuilders.end())
	{
		Protocol::Response* res = itr->second();

		if (res->HandleRequest(action, playerID))
		{
			res->SetSuccess();
		}
		else
		{
			res->SetFailure();
		}
		
		body = Json::Write([res](auto& writer)
		{
			const Protocol::Response* actions[] = { res };
			Json::Write(writer, "actions", actions);
		});

		delete res;
	}
	else
	{
		std::cout << "Unhandled action: " << actionName << std::endl;
	}
	return body;
}