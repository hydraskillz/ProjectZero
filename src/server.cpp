#include "server.h"
#include "web_server.h"
#include "cs_bridge.h"
#include "cms_data.h"
#include "player_id.h"
#include "action_handler.h"
#include "player_database.h"
#include "util.h"

#include <thread>
#include <map>
#include <fstream>
#include <sstream>

namespace
{
	const std::string Version("2");

	typedef std::map<std::string, std::string> CotentMap;

	struct Config : public Json::ISerializeable
	{
		int port;
		std::string cmsFile;
		std::string dllName;
		std::string cdnManifestName;

		void Serialize(Json::Serializer& serializer) override
		{
			SERIALIZE_JSON(port);
			SERIALIZE_JSON(cmsFile);
			SERIALIZE_JSON(dllName);
			SERIALIZE_JSON(cdnManifestName);
		}
	};

	struct ContentManifest : public Json::ISerializeable
	{
		std::vector<std::string> content;

		void Serialize(Json::Serializer& serializer) override
		{
			SERIALIZE_JSON(content);
		}
	};

	// Global data
	Config		gConfig;		// Server config
	CMSData		gCMSData;		// This is a database of all gamedata
	CotentMap	gContent;		// Additional assets

	void LoadContent()
	{
		gContent.clear();

		ContentManifest manifest;
		if (!Json::ReadFile(gConfig.cdnManifestName, manifest))
		{
			std::cout << "Failed to read manifest!" << std::endl;
			return;
		}

		const std::string workingDir = GetCurrentWorkingDir();
		for (const std::string& path : manifest.content)
		{
			std::ifstream ifs(workingDir + path, std::ios::binary);
			if (ifs.is_open())
			{
				std::stringstream ss;
				ss << ifs.rdbuf();
				gContent[path] = ss.str();
			}
			else
			{
				std::cout << "Missing file: " << path << std::endl;
			}
		}
	}

	std::string ProcessAction(const rapidjson::Value& action, player_id playerID)
	{
		std::string body;
		if (action.HasMember("action"))
		{
			std::string actionName = action["action"].GetString();
			std::cout << "Processing action \"" << actionName << "\" for player " << playerID << std::endl;
			body = HandleAction(actionName, action, playerID);
		}
		return body;
	}

	void Request_Handler(const http::Request& req, http::Respone& res)
	{
		// TODO - put the CDN stuff in seperate app or thread
		if (req.method == http::Method::GET)
		{
			std::cout << "GET: " << req.path;
			auto itr = gContent.find(req.path);
			if (itr != gContent.end())
			{
				res.body = itr->second;
				res.content_type = "text/html";
				std::cout << " OK" << std::endl;
			}
			else
			{
				res.status = "404";
				std::cout << " NOT FOUND" << std::endl;
			}
		}
		else if (req.method == http::Method::POST && req.content_length > 0)
		{
			std::string data = req.body.substr(req.body.find_first_of('=', 0) + 1);
			if (!data.empty())
			{
				std::string jsonText = DecryptString(data);

#ifdef VERBOSE
				if (jsonText.size() < 1024)
					std::cout << jsonText << std::endl;
				else
					std::cout << "<a lot of text>" << std::endl;
#endif

				rapidjson::Document doc;
				doc.Parse(jsonText.c_str());
				if (!doc.IsObject())
				{
					std::cout  << "INVALID JSON" << std::endl;
					return;
				}

				player_id id = INVALID_PLAYER_ID;
				if (doc.HasMember("player_id"))
				{
					const rapidjson::Value& pid = doc["player_id"];
					id = pid.GetInt64();
				}

				if (doc.HasMember("actions"))
				{
					const rapidjson::Value& a = doc["actions"];
					for (rapidjson::SizeType i = 0; i < a.Size(); ++i)
					{
						const rapidjson::Value& action = a[i];
						res.body = ProcessAction(action, id);
						res.content_type = "application/json; charset=utf-8";

#ifdef VERBOSE
						if (res.body.size() < 1024)
							std::cout << res.body << std::endl;
						else
							std::cout << "<a lot of text>" << std::endl;
#endif
						break;
					}
				}
			}
			else
			{
				std::cout  << "Unexpected data: " << req.body << std::endl;
			}
		}
	}

	void ListenerLoop()
	{
		webserver::Listen(gConfig.port, Request_Handler);
	}

	bool Initialize()
	{
		// Load config
		if (!Json::ReadFile("config.json", gConfig))
		{
			std::cout << "Failed to load config.json!" << std::endl;
			return false;
		}

		// Load dll hooks
		if (!CS::Initialize(gConfig.dllName))
		{
			std::cout << "Failed to load dll!" << std::endl;
			return false;
		}

		// Load gamedata
		if (!Json::ReadFile(gConfig.cmsFile.c_str(), gCMSData))
		{
			std::cout << "Failed to load CMS Data!" << std::endl;
			return false;
		}

		// Setup factories to handle actions
		InitializeHandlers();

		// Load assets
		LoadContent();

		// Load player save data
		PlayerDB::LoadAllPlayerData();

		return true;
	}

	void Shutdown()
	{
		CS::Shutdown();
	}
}

const std::string& Server::GetVersion()
{
	return Version;
}

const CMSData& Server::GetCMSData()
{
	return gCMSData;
}

void Server::Run()
{
	if (Initialize())
	{
		std::thread theServer(ListenerLoop);
		theServer.join();
	}
	Shutdown();
}