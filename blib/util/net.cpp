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

				return new TcpClient(s);

			}





			TcpClient::TcpClient(SOCKET s)
			{
				this->s = s;
			}

			TcpClient::~TcpClient()
			{
				closesocket(s);
			}

			void TcpClient::recvAsync(int len, const std::function<void(char* data, int len)> &callback)
			{

			}

		}
	}
}