#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "net.h"
#ifdef WIN32
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))
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
#include <string.h>
inline void closesocket(SOCKET s)
{
	close(s);
}
#endif

#include <assert.h>
#include <thread>
#include <glm/glm.hpp>
#include <blib/util/Log.h>
#include <blib/util/Thread.h>




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

				netRunning = true;
				netThread = std::thread([]()
				{
					blib::util::Thread::setThreadName("Socketthread");
					SOCKET highsock = 0;
					fd_set socks;
					timeval timeout;
					timeout.tv_sec = 1000;
					timeout.tv_usec = 0;

					while (netRunning)
					{
						netMutex.lock();
						FD_ZERO(&socks);
						highsock = 0;
						for (NetTask* task : tasks)
						{
							FD_SET(task->s, &socks);
                            if(task->s > highsock)
                                highsock = task->s;
						}
						select(highsock + 1, &socks, (fd_set*)0, (fd_set*)0, &timeout);

						std::set<NetTask*> toRemove;

						std::set<NetTask*> tmpTasks = tasks;

						for (NetTask* task : tmpTasks)
						{
							if (FD_ISSET(task->s, &socks))
							{
								task->callback();
								toRemove.insert(task);
							}
						}
						if (!toRemove.empty())
						{
							for (NetTask* task : toRemove)
								tasks.erase(task);
							toRemove.clear();
						}
						netMutex.unLock();
						//update tasks
					}

					Log::out << "Socketthread terminating..." << Log::newline;
				});

			
			}


			void dispose()
			{
				netRunning = false;
				for (NetTask* task : tasks)
					closesocket(task->s);
				Log::out << "Socketthread joining..." << Log::newline;
				netThread.join();
				Log::out << "Socketthread done joining..." << Log::newline;
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
				inet_aton("0.0.0.0", &sin.sin_addr);
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
				if ((int)newSocket <= 0)
					return NULL;
				return new TcpClient(newSocket, inet_ntoa(client.sin_addr));

			}

			void TcpListener::stopAccept()
			{
				closesocket(s);
			}





			TcpClient::TcpClient(SOCKET s, std::string ip)
			{
				this->s = s;
				this->ip = ip;
				this->asyncData = NULL;
				this->asyncDataLen = 0;
				this->recving = false;
			}

			TcpClient::TcpClient()
			{
				this->s = 0;
				this->ip = "";
				this->asyncData = NULL;
				this->asyncDataLen = 0;
				this->recving = false;
			}

			TcpClient::~TcpClient()
			{
				closesocket(s);
				if (asyncData != NULL)
					delete[] asyncData;
				asyncData = NULL;
			}

			void TcpClient::recvAsync(int len, const std::function<void(char* data, int len)> &callback)
			{
				assert(!recving);

				if (asyncDataLen != len || asyncData == NULL)
				{
					if (asyncData != NULL)
						delete asyncData;
					asyncData = new char[len];
					asyncDataLen = len;
				}
				recving = true;

				netMutex.lock();
				tasks.insert(new NetTask(s, [this, callback]()
				{
					int rc = ::recv(s, asyncData, asyncDataLen, 0);
					printf("RECV:");
					for (int i = 0; i < glm::min(2, rc); i++)
						printf("%02x", asyncData[i]);
					printf("\t");
					for (int i = 2; i < rc; i++)
						printf("%c", asyncData[i]);
					printf("\n\n");
					printf("\n\n");
					recving = false;
					callback(asyncData, rc);
				}));
				netMutex.unLock();
				

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
                    //int rc = ::connect(s, (struct sockaddr*) &sin, sizeof(sockaddr_in));
                    ::connect(s, (struct sockaddr*) &sin, sizeof(sockaddr_in));

					callback();


				}, 0).detach();

			}

			void TcpClient::send(std::string data)
			{
				sendMutex.lock();
				printf("SEND:");

				for (size_t i = 0; i < glm::min((size_t)2, data.length()); i++)
					printf("%02x", data[i]);
				printf("\t");
				for (size_t i = 2; i < data.length(); i++)
					printf("%c", data[i]);
				printf("\n\n");
				::send(s, data.c_str(), data.length(), 0);
				sendMutex.unLock();
			}

			void TcpClient::disconnect()
			{
				closesocket(s);
				s = 0;
			}

		}
	}
}
