#include <iostream>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>

using std::cout;
using std::endl;

int ti_ErrNo = 0;

void Udp_Connect()
{
    int tfd_Sock = socket(AF_INET6, SOCK_DGRAM, 0);
    if(-1 == tfd_Sock)
    {
        ti_ErrNo = errno;
        cout << "Create socekt failed: " << strerror(ti_ErrNo)  << endl;
        return;
    }

    sockaddr_in6 my_addr;
    memset(&my_addr, 0, sizeof(my_addr));

    my_addr.sin6_family = AF_INET6;
    my_addr.sin6_addr = IN6ADDR_LOOPBACK_INIT;

    int td_BindRet = bind(tfd_Sock, (struct sockaddr*) &my_addr, sizeof(my_addr));
    if(td_BindRet < 0)
    {
        ti_ErrNo = errno;
        cout << "Bind socekt failed: " << strerror(ti_ErrNo)  << endl;
        return;        
    }

    const char* ts_Send = "#Send from ubuntu";
    sendto(td_BindRet, ts_Send, sizeof(ts_Send), );

    cout << "Udp_Connetc suc..." << endl;
}

int main()
{
    cout << "-- Start --" << endl;

    Udp_Connect();

    cout << "-- End --" << endl;
    return 0;
}