
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include "finder.h"
#include "chooser.h"
#include "converter.h"
#include "Camera.h"
#include "frame.h"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>


int main()         
{
    const char* cmd = "arp -a";
    std::string haslo_lewe_oko = "I_am_left_eye";
    std::string haslo_prawe_oko = "I_am_right_eye";

    int width = 320;  // Width of FRAMESIZE_QVGA
    int height = 240; // Height of FRAMESIZE_QVGA
    std::string filename = "output.jpg";

    try
    {
        // znajduje ip do urz¹dzenia
        std::vector<std::string> ip_Addresses = gimme_ip_adresses();
        string ip_lewe_oko = verify(ip_Addresses, haslo_lewe_oko);     

        // obiekty potrzebne do utworzenia websocketa
        net::io_context ioc;
        tcp::resolver resolver{ ioc };
        websocket::stream<tcp::socket> ws{ ioc };

        // szukam adresu wprost do portu w urz¹dzeniu
        std::string host = ip_lewe_oko;
        auto const results = resolver.resolve(host, "80");

        // ³¹cze siê do otrzymanego adresu
        tcp::endpoint ep = net::connect(ws.next_layer(), results);        
        host += ':' + std::to_string(ep.port());

        // Tworze dekorator potrzebny do podania rêki "handshake"
        ws.set_option(websocket::stream_base::decorator(
            [](websocket::request_type& req)
            {
                req.set(http::field::user_agent,
                    std::string(BOOST_BEAST_VERSION_STRING) +
                    " websocket-client-coro");
            }));

        // podanie rêki
        ws.handshake(host, "/");
        // potwierdzam ¿e siê po³¹czy³em
        if (ws.is_open())
            printf("I connected to the avalible websocket \n");
        else
            throw std::runtime_error("Nie uda³o siê po³¹czyæ");

        Camera(ws, width, height);
        
        // Close the WebSocket connection
        ws.close(websocket::close_code::normal);

    }
    catch (std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
