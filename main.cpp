
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
        // proszê system operacyjny podanie wszystkich adresów z którymi jest po³¹czony
        std::string arpOutput = executeCommand(cmd); 
        // parsuje otrzymany string na wektor zawieraj¹cy same adressy
        std::vector<std::string> ipAddresses = parseIPAddresses(arpOutput);
        // pytam siê ka¿dego kim s¹ i sprawdzam odpowiedŸ, z tego wiem kim s¹
        string ip_lewe_oko = verify(ipAddresses, haslo_lewe_oko);     

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

        // wyœlij wiadomoœæ
        /*
        std::string text;
        std::cout << "prosze podaæ polecenie";
        std::cin >> text;
        ws.write(net::buffer(std::string(text)));

        // odczytaj widomoœæ
        beast::flat_buffer buffer;
        ws.read(buffer);

        saveFlatBufferAsGrayscaleJPEG(buffer, width, height, filename);
        */
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
