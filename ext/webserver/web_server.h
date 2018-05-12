#pragma once

#include <string>

namespace http
{
	enum class Method
	{
		INVALID,
		GET,
		POST,
	};

	struct Base
	{
		int content_length = 0;
		std::string content_type;
		std::string body;
	};

	struct Request : public Base
	{
		Method method = Method::INVALID;
		std::string path;
	};

	struct Respone : public Base
	{
		std::string status = "200 OK";
	};
}

namespace webserver
{
	typedef void(RequestHandler)(const http::Request& req, http::Respone& res);
	void Listen(unsigned int port, RequestHandler handler);
}