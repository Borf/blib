#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "net.h"
#ifdef WIN32
#include <winsock2.h>
typedef int socklen_t;
#else
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
inline void closesocket(SOCKET s)
{
	close(s);
}
#endif


#include <thread>
#include <blib/util/Log.h>



namespace blib
{
	namespace util
	{
		namespace net
		{
			void init()
			{
#ifdef WIN32
				WORD wVersionRequested;
				WSADATA wsaData;
				int err;

				/* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
				wVersionRequested = MAKEWORD(2, 2);

				err = WSAStartup(wVersionRequested, &wsaData);
				if (err != 0) {
					/* Tell the user that we could not find a usable */
					/* Winsock DLL.                                  */
					printf("WSAStartup failed with error: %d\n", err);
				}
#endif
			}

			TcpListener::TcpListener(int port)
			{
				struct sockaddr_in sin;
				if ((s = socket(AF_INET, SOCK_STREAM, 0)) == 0)
				{
					Log::out << "TcpListener: Error creating socket!" << Log::newline;;
					return;
				}

				memset(&sin, 0, sizeof(sin));
				sin.sin_family = AF_INET;
#ifdef WIN32
				//TODO	sin.sin_addr = inet_addr(bindport.c_str());
#else
				inet_aton(ip.c_str(), &sin.sin_addr);
#endif
				sin.sin_port = htons(port);

				int t_reuse = 1;
				setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char*)&t_reuse, sizeof(t_reuse));
				if ((bind(s, (struct sockaddr*)&sin, sizeof(sin))))
				{
					Log::out << "TcpListener: Error binding socket" << port << Log::newline;
					closesocket(s);
					return;
				}

				if ((listen(s, 10)))
				{
					Log::out << "TcpListener: Error listening to port" << port << Log::newline;
					closesocket(s);
					return;
				}

			}

			TcpClient* TcpListener::accept()
			{
				struct sockaddr_in client;
				socklen_t size = sizeof(client);
				SOCKET newSocket = ::accept(s, (struct sockaddr*)&client, &size);

				return new TcpClient(newSocket, inet_ntoa(client.sin_addr));

			}





			TcpClient::TcpClient(SOCKET s, std::string ip)
			{
				this->s = s;
				this->ip = ip;
				this->asyncData = NULL;
				this->asyncDataLen = 0;
			}

			TcpClient::TcpClient()
			{
				this->s = 0;
				this->ip = "";
				this->asyncData = NULL;
				this->asyncDataLen = 0;
			}

			TcpClient::~TcpClient()
			{
				closesocket(s);
			}

			void TcpClient::recvAsync(int len, const std::function<void(char* data, int len)> &callback)
			{
				if (asyncDataLen != len || asyncData == NULL)
				{
					if (asyncData != NULL)
						delete asyncData;
					asyncData = new char[len];
					asyncDataLen = len;
				}
				//TODO: don't start a new thread every time !
				std::thread([this, callback](int)
				{
					int rc = ::recv(s, asyncData, asyncDataLen, 0);
					callback(asyncData, rc);
				}, 0).detach();
				

			}

			void TcpClient::connectAsync(std::string hostname, int port, const std::function<void()> &callback)
			{
				std::thread([this, callback, hostname, port](int)
				{
					struct hostent* host;
					host = gethostbyname(hostname.c_str());
					if (host == NULL)
					{
						Log::out<<"Error: Could not connect to " << hostname << ":" << port << Log::newline;
						return;
					}


					struct sockaddr_in sin = { 0 };
					sin.sin_port = htons(port);
					sin.sin_family = host->h_addrtype;
					memcpy((char*)&sin.sin_addr.s_addr, host->h_addr_list[0], host->h_length);

					s = socket(AF_INET, SOCK_STREAM, 0);
					int rc = ::connect(s, (struct sockaddr*) &sin, sizeof(sockaddr_in));

					callback();


				}, 0).detach();

			}

			void TcpClient::send(std::string data)
			{
				::send(s, data.c_str(), data.length(), 0);
			}

		}
	}
}