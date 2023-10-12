#define resolver
#ifndef resolver

#include <iostream>
#include <boost/asio.hpp>

int main() {
    try {
        boost::asio::io_context io_context;
        boost::asio::ip::tcp::resolver resolver(io_context);

        // Replace "device_name" with the hostname of the device you want to resolve.
        std::string device_name = "lewe_oko";

        // Resolve the hostname to an IP address.
        boost::asio::ip::tcp::resolver::results_type results =
            resolver.resolve(device_name, "");

        for (const auto& result : results) {
            // The IP address can be retrieved from the resolved result.
            boost::asio::ip::address endpoint = result.endpoint().address();

            if (endpoint.is_v4()) {
                std::cout << "IPv4 Address: " << endpoint.to_string() << std::endl;
            }
            else if (endpoint.is_v6()) {
                std::cout << "IPv6 Address: " << endpoint.to_string() << std::endl;
            }
        }
    }
    catch (const boost::system::system_error& ex) {
        // Access error code and message from the boost::system::system_error
        const boost::system::error_code& errorCode = ex.code();
        const std::string& errorMessage = ex.what();

        // Handle the error as needed
        std::cerr << "Boost.Asio error code: " << errorCode << std::endl;
        std::cerr << "Error message: " << errorMessage << std::endl;
        // You can take appropriate actions based on the error code and message.
    }

    return 0;
}
#endif