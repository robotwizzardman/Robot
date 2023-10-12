#pragma once


#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <iostream>
#include <string>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>



#include <iostream>
#include <vector>
#include <string>
#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>

using namespace std;
namespace asio = boost::asio;
namespace beast = boost::beast;
#include <iostream>
#include <vector>
#include <string>
#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>

using namespace std;
namespace asio = boost::asio;
namespace beast = boost::beast;
using tcp = asio::ip::tcp;

bool checkPassword(const string& response, const string& password) {
    return response == password;
}

string verify(const vector<string>& addresses, const string& password) {
    asio::io_context ioContext;
    tcp::resolver resolver(ioContext);
    
    for (const string& ipAddress : addresses) {
        try {
            // creating stuff necessary for websocket
            tcp::resolver::results_type endpoints = resolver.resolve(ipAddress, "80");
            beast::websocket::stream<tcp::socket> ws(ioContext);
            //connect to websocket
            auto ep = net::connect(ws.next_layer(),endpoints);
            string host = ipAddress;
            host += ':' + std::to_string(ep.port());

            ws.set_option(websocket::stream_base::decorator(
                [](websocket::request_type& req)
                {
                    req.set(http::field::user_agent,
                        std::string(BOOST_BEAST_VERSION_STRING) +
                        " websocket-client-coro");
                }));

            //printf("I am here \n");
            ws.handshake(ipAddress, "/");

            if (!ws.is_open()) { continue; }

            else{
                std::cout << "hey I succeded with ip :" << ipAddress << std::endl;
                ws.write(asio::buffer("identify yourself"));
                beast::flat_buffer buffer;
                ws.read(buffer);
                string response(beast::buffers_to_string(buffer.data()));

                // Close the WebSocket connection
                ws.close(beast::websocket::close_code::normal);
                // return ip
                if (checkPassword(response, password))
                    return ipAddress; // Matching password found, return the IP address

            }
            
        }
        catch (const exception& e) {
            // Handle errors and continue to the next IP address
            #ifdef DEBUG
            cerr << "Error accuted in trying to connect with : " << ipAddress << "  :  " << e.what() << endl;
            #endif // DEBUG
        }
    }

    // No matching password found, return an empty string
    return "";
}