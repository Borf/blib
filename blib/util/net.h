#pragma once

#include <functional>
#include <list>

#ifdef WIN32
#include <winsock2.h>
#else

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

				friend class TcpClient;
			};


			class TcpClient
			{
				TcpListener* listener;
				SOCKET s;

				char* asyncData;
				int asyncDataLen;

			public:
				TcpClient(SOCKET s, std::string ip);
				~TcpClient();

				void connect(std::string hostname, int port);
				void connectAsync(std::string hostname, int port, const std::function<void()> &callback);

				int recv(char* data, int len);
				void recvAsync(int len, const std::function<void(char* data, int len)> &callback);

				bool isConnected();
				std::string ip;

			};

		}
	}
}