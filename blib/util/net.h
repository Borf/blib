#pragma once

#include <functional>
#include <list>
#include <streambuf>

#ifdef WIN32
#include <winsock2.h>
#else
typedef int SOCKET;
#endif

namespace blib
{
	namespace util
	{
		namespace net
		{
			void init();
			class TcpClient;

			class TcpListener
			{
				SOCKET s;
			public:
				TcpListener(int port);

				TcpClient* accept();
				void acceptAsync(const std::function<void()> &callback);
				void stopAccept();
				friend class TcpClient;
			};


			class TcpClient
			{
				TcpClient(const TcpClient &);
				TcpClient &operator= (const TcpClient &);

				TcpListener* listener;
				SOCKET s;

				char* asyncData;
				int asyncDataLen;

			public:
				TcpClient(SOCKET s, std::string ip);
				TcpClient();
				~TcpClient();

				void connect(std::string hostname, int port);
				void connectAsync(std::string hostname, int port, const std::function<void()> &callback);

				void disconnect();

				int recv(char* data, int len);
				void recvAsync(int len, const std::function<void(char* data, int len)> &callback);

				void send(std::string data);

				bool isConnected();
				std::string ip;

			};

		}
	}
}