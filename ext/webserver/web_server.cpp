#include "web_server.h"

#include <WinSock2.h>
#include <sstream>
#include <ctime>

namespace utils
{
	std::string DecodeUrl(const std::string& data)
	{
		std::string decoded;
		for (size_t i = 0; i < data.size(); ++i)
		{
			if (data[i] != '%')
			{
				// '+' -> ' '
				if (data[i] == '+')
				{
					decoded += ' ';
				}
				else
				{
					decoded += data[i];
				}
			}
			// '%XX' -> 'Y'
			else
			{
				int hexCode = 0;
				sscanf(data.substr(i + 1, 2).c_str(), "%x", &hexCode);
				const char c = static_cast<char>(hexCode);
				decoded += c;
				i += 2; // advances 3 characters
			}
		}
		return decoded;
	}

	std::string MakeTimeStamp()
	{
		time_t ltime;
		time(&ltime);
		tm* gmt= gmtime(&ltime);
		std::string timestamp(asctime(gmt));
		timestamp.pop_back(); // remove \n
		return timestamp + " GMT";
	}

	void ParseRequestHeader(const std::string& line, http::Request& req)
	{
		size_t start = line.find_first_not_of(" ", 3);
		std::string header = line.substr(start);

		// Remove trailing newlines
		while (!header.empty() && (header.back() == '\n' || header.back() == '\r'))
		{
			header.pop_back();
		}

		// Remove potential trailing HTTP/1.x
		if (header.size() > 7)
		{
			if (header.substr(header.size() - 8, 7) == "HTTP/1.")
			{
				header = header.substr(0, header.size() - 9);
			}
		}

		std::string::size_type q = header.find("?");
		if (q != std::string::npos)
		{
			std::string url_params = header.substr(q + 1);

			req.path = header.substr(0, q);

			// TODO parse key/value pairs
			//
		}
		else
		{
			req.path = header;
		}
	}
}

namespace
{
	constexpr const char* CRLF = "\x0a\x0d";
	const std::string content_type("Content-Type: ");
	const std::string content_length("Content-Length: ");

	std::string ServerName("Server");

	std::string ReadLine(SOCKET socket)
	{
		std::string line;
		int ret = 0;
		char c = '\0';
		do
		{
			if (c != '\0')
			{
				line += c;
			}
			ret = recv(socket, &c, 1, 0);
		} while (ret > 0 && c != '\n');
		return line;
	}

	std::string Read(SOCKET socket)
	{
		constexpr unsigned BUFFER_SIZE = 1024;
		char buffer[BUFFER_SIZE] = { 0 };
		std::string data;

		for (;;)
		{
			u_long value = 0;
			if (ioctlsocket(socket, FIONREAD, &value) == SOCKET_ERROR)
			{
				break;
			}

			if (value == 0)
			{
				break;
			}

			if (value > BUFFER_SIZE)
			{
				value = BUFFER_SIZE;
			}

			int len = recv(socket, buffer, value, 0);
			if (len <= 0)
			{
				break;
			}

			data += std::string(buffer, len);
		}
		return data;
	}

	void Send(SOCKET socket, const std::string& data)
	{
		send(socket, data.c_str(), static_cast<int>(data.size()), 0);
	}
}

void webserver::Listen(unsigned int port, RequestHandler handler)
{
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);

	SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server == INVALID_SOCKET)
	{
		//ConsolePrintf(CONSOLE_ERROR, "Invalid Socket");
		return;
	}

	SOCKADDR_IN address;
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = INADDR_ANY;

	if (bind(server, (sockaddr*) &address, sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		//ConsolePrintf(CONSOLE_ERROR, "Error\nbind failed: %d\n", WSAGetLastError());
		closesocket(server);
		return;
	}

	// Non-blocking?
	//u_long mode = 1;
	//ioctlsocket(server, FIONBIO, &mode);

	listen(server, 5 /*SOMAXCONN*/);

	// Receive connections
	do
	{
		SOCKET client = accept(server, nullptr, nullptr);
		if (client == INVALID_SOCKET)
		{
			const int error = WSAGetLastError();
			if (error == WSAEWOULDBLOCK)
			{
				continue;
			}
			else
			{
				//ConsolePrintf(CONSOLE_ERROR, "Error\nbind failed: %d\n", WSAGetLastError());
				break;
			}
		}

		// TODO - job off the connection handler?

		std::string line = ReadLine(client);
		if (!line.empty())
		{
			http::Request req;

			if (line.find("GET") == 0)
			{
				req.method = http::Method::GET;
			}
			else if (line.find("POST") == 0)
			{
				req.method = http::Method::POST;
			}

			if (req.method != http::Method::INVALID)
			{
				utils::ParseRequestHeader(line, req);

				while (1)
				{
					line = ReadLine(client);
					if (line.empty())
					{
						break;
					}

					size_t pos_endl = line.find_first_of(CRLF);
					if (pos_endl == 0)
					{
						break;
					}

					line = line.substr(0, pos_endl);

					if (line.substr(0, content_length.size()) == content_length)
					{
						req.content_length = std::atoi(line.substr(content_length.size()).c_str());
					}
					else if (line.substr(0, content_type.size()) == content_type)
					{
						req.content_type = line.substr(content_type.size()).c_str();
					}
				}

				// We have a body
				if (req.content_length > 0)
				{
					std::string data = Read(client);
					req.body = utils::DecodeUrl(data);
				}

				http::Respone res;
				handler(req, res);

				// Set content length correctly
				res.content_length = static_cast<int>(res.body.size());

				// Build response header
				std::stringstream header;
				header << "HTTP/1.1 " << res.status << std::endl;
				header << "Content-Type: " << res.content_type << std::endl;
				header << "Date: " << utils::MakeTimeStamp() << std::endl;
				header << "Server: " << ServerName << std::endl;
				header << "Connection: close" << std::endl;
				header << "Content-Length: " << res.content_length << std::endl;
				header << std::endl;

				// Beging response transmission
				Send(client, header.str());
				Send(client, res.body);
				Send(client, "\n");
			}
		}

		closesocket(client);
	} while (true);

	closesocket(server);

	WSACleanup();
}