#pragma once

#include "json.h"
#include "player_id.h"

#include <string>

void InitializeHandlers();
std::string HandleAction(const std::string& actionName, const rapidjson::Value& action, player_id playerID);