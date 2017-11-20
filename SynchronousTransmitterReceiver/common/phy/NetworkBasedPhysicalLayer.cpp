#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <cstring>
#include "NetworkBasedPhysicalLayer.h"

/**
 * The phy class contains a definition for close() which is ambiguous
 * with the close syscall. This is just a wrapper for closing the specified
 * file descriptor number
 *
 * @param fd the descriptor to close
 * @return the result of <code>close(fd)</code>
 */
int close_sock(int fd) { return close(fd); }

/**
 * Extract the socket address from the specified struct
 *
 * @param s the struct to extract the address from
 * @return a pointer to the extracted socket address
 */
void * getInAddr(struct sockaddr *s)
{
    if (s->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in*)s)->sin_addr);
    }
    else
    {
        return &(((struct sockaddr_in6*)s)->sin6_addr);
    }
}

void libsts::phy::NetworkBasedPhysicalLayer::open()
{
    if(remote.empty())
    {
        open_server();
    }
    else
    {
        open_client();
    }
}

void libsts::phy::NetworkBasedPhysicalLayer::close()
{
    if(mySock != -1) close_sock(mySock);
    if(listenSock != -1) close_sock(listenSock);

    mySock = listenSock = -1;
}

void libsts::phy::NetworkBasedPhysicalLayer::waitForConnection()
{
    struct sockaddr_storage remoteAddr{};
    socklen_t sin_size = sizeof remoteAddr;

    std::cerr << "Waiting for connection on port " << port << std::endl;

    mySock = accept(listenSock, reinterpret_cast<sockaddr *>(&remoteAddr), &sin_size);
    if (mySock == -1)
    {
        throw libsts::phy::BindException("Failed create socket for client: " + std::string(strerror(errno)));
    }

    char s[INET6_ADDRSTRLEN];
    inet_ntop(
            remoteAddr.ss_family,
            getInAddr(
                    reinterpret_cast<struct sockaddr *>(&remoteAddr)
            ),
            s,
            INET6_ADDRSTRLEN
    );

    std::cerr << s << " connected" << std::endl;
}

void libsts::phy::NetworkBasedPhysicalLayer::open_client()
{
    struct addrinfo hints{}, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if((rv = getaddrinfo(remote.c_str(), std::to_string(port).c_str(), &hints, &servinfo)) != 0)
    {
        throw libsts::phy::BindException("Failed to get address info of remote: " + std::string(gai_strerror(rv)));
    }

    for(p = servinfo; p != nullptr; p = p->ai_next)
    {
        if ((mySock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) continue;

        if (connect(mySock, p->ai_addr, p->ai_addrlen) == -1)
        {
            close_sock(mySock);
            continue;
        }

        break;
    }

    if (p == nullptr)
    {
        throw libsts::phy::BindException("Failed to connect to remote");
    }

    inet_ntop(p->ai_family, getInAddr(p->ai_addr), s, INET6_ADDRSTRLEN);

    std::cerr << "Connected to " << s << std::endl;
    freeaddrinfo(servinfo);
}

void libsts::phy::NetworkBasedPhysicalLayer::open_server()
{
    struct addrinfo hints{}, *servinfo, *p;
    int yes=1;
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(nullptr, std::to_string(port).c_str(), &hints, &servinfo)) != 0)
    {
        throw libsts::phy::BindException("getaddrinfo failed: " + std::string(gai_strerror(rv)));
    }

    // bind to the first available address
    for(p = servinfo; p != nullptr; p = p->ai_next)
    {
        if ((listenSock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            std::cerr << "Failed to create server socket: " << strerror(errno) << std::endl;
            continue;
        }

        if (setsockopt(listenSock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        {
            throw libsts::phy::BindException("Failed to set socket opt: " + std::string(strerror(errno)));
        }

        if (bind(listenSock, p->ai_addr, p->ai_addrlen) == -1)
        {
            close_sock(listenSock);
            std::cerr << "Failed to bind server socket: " << strerror(errno) << std::endl;
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo);

    if (p == nullptr)
    {
        throw libsts::phy::BindException("Failed to bind to a socket");
    }

    if (listen(listenSock, 1) == -1)
    {
        throw libsts::phy::BindException("Failed to listen on socket:" + std::string(strerror(errno)));
    }
}

void libsts::phy::NetworkBasedPhysicalLayer::write(const char *data, size_t len)
{
    size_t remaining = len;
    size_t off = 0;
    ssize_t res;
    while((res = send(mySock, data + off, remaining, 0)) != 0)
    {
        if(res == -1)
        {
            throw libsts::phy::IOException("Failed to send: " + std::string(strerror(errno)));
        }

        remaining -= res;
        off += res;
    }
}

size_t libsts::phy::NetworkBasedPhysicalLayer::read(char *buff, size_t len)
{
    if(mySock == -1) waitForConnection();

    ssize_t res = 0;
    if((res = recv(mySock, buff, len, 0)) == -1)
    {
        throw libsts::phy::IOException("Failed to receive: " + std::string(strerror(errno)));
    }

    return static_cast<size_t>(res);
}

uint32_t libsts::phy::NetworkBasedPhysicalLayer::flush()
{
    return 0;
}

bool libsts::phy::NetworkBasedPhysicalLayer::eof()
{
    return mySock == -1 && listenSock == -1;
}

libsts::Direction libsts::phy::NetworkBasedPhysicalLayer::getDirection()
{
    return libsts::Direction::BOTH;
}
