#pragma once

#include "json.h"
#include "date_time.h"

struct PlayerAttendance : public Json::ISerializeable
{
	int AttendanceID = 1;
	int LastDayNumber = 0;
	//DateTime LastCheckDateTime;
	int DayCount = 10;
	int IsActive = 1;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(AttendanceID);
		SERIALIZE_JSON(LastDayNumber);
		//SERIALIZE_JSON(LastCheckDateTime);
		SERIALIZE_JSON(DayCount);
		SERIALIZE_JSON(IsActive);
	}
};

struct AttendanceResult : public Json::ISerializeable
{
	bool IsAttendance = false;
	PlayerAttendance Attendance;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(IsAttendance);
		SERIALIZE_JSON(Attendance);
	}
};