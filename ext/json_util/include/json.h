#pragma once

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/istreamwrapper.h>

#include <vector>
#include <array>
#include <fstream>
#include <iostream>

// Macro to be used within ISerializeable::Serialize
#define SERIALIZE_JSON(value)												\
	if (serializer.IsWriting())												\
		Json::Write(serializer.GetWriter(), #value, value);					\
	else Json::Read(serializer.GetReader(), #value, value)

#define SERIALIZE_JSON_IF_NOT_EMPTY(value)									\
	if (serializer.IsWriting())												\
	{																		\
		if (!value.empty())													\
			Json::Write(serializer.GetWriter(), #value, value);				\
	}																		\
	else Json::Read(serializer.GetReader(), #value, value)

namespace Json
{
	/// <summary> Wraps the read/write type. </summary>
	class Serializer
	{
	public:
		Serializer(const rapidjson::Value& json)
			: json(&json)
			, writer(nullptr)
		{}
		Serializer(rapidjson::Writer<rapidjson::StringBuffer>& writer)
			: json(nullptr)
			, writer(&writer)
		{}

		bool IsWriting() const { return writer != nullptr; }
		const rapidjson::Value& GetReader() const { return *json; }
		rapidjson::Writer<rapidjson::StringBuffer>& GetWriter() { return *writer; }

	private:
		const rapidjson::Value* json;
		rapidjson::Writer<rapidjson::StringBuffer>* writer;
	};

	/// <summary> Interface for serializable objects. </summary>
	class ISerializeable
	{
	public:
		// Note: you must retain the same parameter name in order to use the macro
		virtual void Serialize(Serializer& serializer) = 0;
	};

	//
	// Reading
	//

	#define READ_FUNC(T, TYPE)																\
	inline void Read(const rapidjson::Value& json, T& value)								\
	{																						\
		if (json.Is##TYPE##()) value = static_cast<T>(json.Get##TYPE##());					\
	}

	#define READ_NUMERIC_FUNC(T)															\
	inline void Read(const rapidjson::Value& json, T& value)								\
	{																						\
		if (json.IsNumber()) value = static_cast<T>(json.GetDouble());						\
	}

	READ_NUMERIC_FUNC(int8_t);
	READ_NUMERIC_FUNC(int16_t);
	READ_NUMERIC_FUNC(int32_t);
	READ_NUMERIC_FUNC(int64_t);

	READ_NUMERIC_FUNC(uint8_t);
	READ_NUMERIC_FUNC(uint16_t);
	READ_NUMERIC_FUNC(uint32_t);
	READ_NUMERIC_FUNC(uint64_t);

	READ_NUMERIC_FUNC(float);
	READ_NUMERIC_FUNC(double);

	READ_FUNC(bool, Bool);

	READ_FUNC(char, Int);
	READ_FUNC(std::string, String);

	inline void Read(const rapidjson::Value& json, Json::ISerializeable& value)
	{
		if (json.IsObject())
		{
			Json::Serializer s(json);
			value.Serialize(s);
		}
	}

	// Gernic single value read using Json::As
	template <typename T>
	inline void Read(const rapidjson::Value& json, const char* name, T& value)
	{
		if (json.HasMember(name)) Read(json[name], value);
	}

	// Fixed size array generic read
	template <typename T, size_t N>
	inline void Read(const rapidjson::Value& json, const char* name, T(&value)[N])
	{
		if (json.HasMember(name) && json[name].IsArray())
		{
			const rapidjson::Value& array = json[name];
			const rapidjson::SizeType count = json[name].Size();

			for (rapidjson::SizeType i = 0; i < N && i < count; ++i)
			{
				Read(json[name][i], value[i]);
			}
		}
	}

	// std::vector generic read
	template <typename T>
	inline void Read(const rapidjson::Value& json, std::vector<T>& value)
	{
		if (json.IsArray())
		{
			const rapidjson::SizeType count = json.Size();

			value.clear();
			value.resize(count);

			for (rapidjson::SizeType i = 0; i < count; ++i)
			{
				Read(json[i], value[i]);
			}
		}
	}
	template <typename T>
	inline void Read(const rapidjson::Value& json, const char* name, std::vector<T>& value)
	{
		if (json.HasMember(name) && json[name].IsArray())
		{
			const rapidjson::Value& array = json[name];
			const rapidjson::SizeType count = json[name].Size();

			value.clear();
			value.resize(count);

			for (rapidjson::SizeType i = 0; i < count; ++i)
			{
				Read(array[i], value[i]);
			}
		}
	}

	template <typename T, size_t Size>
	inline void Read(const rapidjson::Value& json, std::array<T, Size>& value)
	{
		if (json.IsArray())
		{
			const rapidjson::SizeType count = json.Size();
			
			value.clear();

			for (rapidjson::SizeType i = 0; i < count; ++i)
			{
				Read(json[i], value[i]);
			}
		}
	}
	template <typename T, size_t Size>
	inline void Read(const rapidjson::Value& json, const char* name, std::array<T, Size>& value)
	{
		if (json.HasMember(name) && json[name].IsArray())
		{
			const rapidjson::Value& array = json[name];
			const rapidjson::SizeType count = json[name].Size();

			for (rapidjson::SizeType i = 0; i < count; ++i)
			{
				Read(array[i], value[i]);
			}
		}
	}

	inline bool ReadFile(const char* filename, ISerializeable& object)
	{
		std::ifstream ifs(filename);
		if (ifs.is_open())
		{
			rapidjson::IStreamWrapper isw(ifs);
			rapidjson::Document d;
			d.ParseStream(isw);
			Serializer s(d);
			object.Serialize(s);
			return true;
		}
		return false;
	}
	inline bool ReadFile(const std::string& filename, ISerializeable& object)
	{
		return ReadFile(filename.c_str(), object);
	}

	inline void ReadText(const std::string& text, ISerializeable& object)
	{
		rapidjson::Document doc;
		doc.Parse(text.c_str());
		Serializer s(doc);
		object.Serialize(s);
	}

	//
	// Writing
	//

#define WRITE_FUNC(T, TYPE)																					\
inline void Write(rapidjson::Writer<rapidjson::StringBuffer>& writer, const T value)						\
	{																										\
		writer.##TYPE##(value);																				\
	}																										\
	inline void Write(rapidjson::Writer<rapidjson::StringBuffer>& writer, const char* name, const T value)	\
	{																										\
		writer.Key(name);																					\
		Write(writer, value);																				\
	}
	

	WRITE_FUNC(int8_t, Int);
	WRITE_FUNC(int16_t, Int);
	WRITE_FUNC(int32_t, Int);
	WRITE_FUNC(int64_t, Int64);

	WRITE_FUNC(uint8_t, Uint);
	WRITE_FUNC(uint16_t, Uint);
	WRITE_FUNC(uint32_t, Uint);
	WRITE_FUNC(uint64_t, Uint64);

	WRITE_FUNC(bool, Bool);

	WRITE_FUNC(float, Double);
	WRITE_FUNC(double, Double);

	WRITE_FUNC(char*, String);

	inline void Write(rapidjson::Writer<rapidjson::StringBuffer>& writer, const std::string& value)
	{
		Write(writer, value.c_str());
	}
	inline void Write(rapidjson::Writer<rapidjson::StringBuffer>& writer, const char* name, const std::string& value)
	{
		Write(writer, name, value.c_str());
	}

	inline void Write(rapidjson::Writer<rapidjson::StringBuffer>& writer, const ISerializeable& value)
	{
		writer.StartObject();
		Serializer s(writer);
		const_cast<ISerializeable&>(value).Serialize(s);
		writer.EndObject();
	}

	inline void Write(rapidjson::Writer<rapidjson::StringBuffer>& writer, const char* name, const ISerializeable& value)
	{
		writer.Key(name);
		Write(writer, value);
	}

	template <typename T, size_t N>
	inline void Write(rapidjson::Writer<rapidjson::StringBuffer>& writer, const char* name, const T(&value)[N])
	{
		writer.Key(name);
		writer.StartArray();
		for (size_t i = 0; i < N; ++i)
			Write(writer, value[i]);
		writer.EndArray();
	}

	template <typename T, size_t N>
	inline void Write(rapidjson::Writer<rapidjson::StringBuffer>& writer, const char* name, const T*(&value)[N])
	{
		writer.Key(name);
		writer.StartArray();
		for (size_t i = 0; i < N; ++i)
			Write(writer, *value[i]);
		writer.EndArray();
	}

	template <typename T>
	inline void Write(rapidjson::Writer<rapidjson::StringBuffer>& writer, const std::vector<T>& value)
	{
		writer.StartArray();
		for (size_t i = 0; i < value.size(); ++i)
		{
			Write(writer, value[i]);
		}
		writer.EndArray();
	}

	template <typename T>
	inline void Write(rapidjson::Writer<rapidjson::StringBuffer>& writer, const char* name, const std::vector<T>& value)
	{
		writer.Key(name);
		Write(writer, value);
	}

	template <typename T, size_t Size>
	inline void Write(rapidjson::Writer<rapidjson::StringBuffer>& writer, const std::array<T, Size>& value)
	{
		writer.StartArray();
		for (size_t i = 0; i < value.size(); ++i)
		{
			Write(writer, value[i]);
		}
		writer.EndArray();
	}

	template <typename T, size_t Size>
	inline void Write(rapidjson::Writer<rapidjson::StringBuffer>& writer, const char* name, const std::array<T, Size>& value)
	{
		writer.Key(name);
		Write(writer, value);
	}

	template <typename TFunctor>
	inline std::string Write(const TFunctor& func)
	{
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		writer.StartObject();
		func(writer);
		writer.EndObject();
		return std::string(buffer.GetString());
	}

	inline bool WriteFile(const char* filename, const ISerializeable& object)
	{
		std::ofstream ofs(filename);
		if (ofs.is_open())
		{
			ofs <<
			Write([&object](auto& writer)
			{
				Serializer s(writer);
				const_cast<ISerializeable&>(object).Serialize(s);
			}).c_str();
			return true;
		}
		return false;
	}
}
