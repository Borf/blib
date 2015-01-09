#pragma once

#include <functional>
#include <list>
#include <streambuf>
#include <set>
#include <thread>

#ifdef WIN32
#include <winsock2.h>
#else
typedef int SOCKET;
#endif

#include <blib/util/Signal.h>
#include <blib/util/Mutex.h>

namespace blib
{
	namespace util
	{
		namespace net
		{
			class NetTask
			{
			public:
				SOCKET s;
				const std::function<void()> callback;

				NetTask(SOCKET s, const std::function<void()>& callback) : s(s), callback(callback)
				{

				}
			};

			static std::set<NetTask*> tasks;
			static blib::util::Signal netSignal;
			static blib::util::Mutex netMutex;
			void init();
			void dispose();
			static std::thread netThread;
			static bool netRunning;

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
				blib::util::Mutex sendMutex;
				TcpListener* listener;
				SOCKET s;

				char* asyncData;
				int asyncDataLen;
				bool recving;

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