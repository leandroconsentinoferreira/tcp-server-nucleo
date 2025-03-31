#if !FEATURE_LWIP
    #error [NOT_SUPPORTED] LWIP not supported for this target
#endif

#include "mbed.h"
#include "EthernetInterface.h"
#include "TCPServer.h"
#include "TCPSocket.h"

#define HTTP_STATUS_LINE "HTTP/1.0 200 OK"
#define HTTP_HEADER_FIELDS "Content-Type: text/html; charset=utf-8"
#define HTTP_MESSAGE_BODY ""                                     \
"<html>" "\r\n"                                                  \
"  <body style=\"display:flex;text-align:center\">" "\r\n"       \
"    <div style=\"margin:auto\">" "\r\n"                         \
"      <h1>Hello World</h1>" "\r\n"                              \
"      <p>It works !</p>" "\r\n"                                 \
"    </div>" "\r\n"                                              \
"  </body>" "\r\n"                                               \
"</html>"

Serial pc(USBTX, USBRX);

#define HTTP_RESPONSE HTTP_STATUS_LINE "\r\n"   \
                      HTTP_HEADER_FIELDS "\r\n" \
                      "\r\n"                    \
                      HTTP_MESSAGE_BODY "\r\n"  \
                      "\r\n"

int main()
{
    pc.printf("Basic HTTP server example");
    
    EthernetInterface eth;
    eth.connect();
    
    pc.printf("The target IP address is %d.%d.%d.%d", eth.get_ip_address()[0],eth.get_ip_address()[1],eth.get_ip_address()[2],eth.get_ip_address()[3]);
    
    TCPServer srv;
    TCPSocket clt_sock;
    SocketAddress clt_addr;
    
    /* Open the server on ethernet stack */
    srv.open(&eth);
    
    /* Bind the HTTP port (TCP 80) to the server */
    srv.bind(eth.get_ip_address(), 80);
    
    /* Can handle 5 simultaneous connections */
    srv.listen(5);
    
    while (true) {
        srv.accept(&clt_sock, &clt_addr);
        pc.printf("Endereco %d.%d.%d.%d:%d", clt_addr.get_ip_address()[0],clt_addr.get_ip_address()[1],clt_addr.get_ip_address()[2],clt_addr.get_ip_address()[3], clt_addr.get_port());
        clt_sock.send(HTTP_RESPONSE, strlen(HTTP_RESPONSE));
    }
}
