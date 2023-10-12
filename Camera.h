#pragma once
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <conio.h>
#include <string>

void Camera(boost::beast::websocket::stream<boost::asio::ip::tcp::socket>& ws, int width, int height) {
    boost::beast::flat_buffer buffer;
    cv::Mat image;
    cv::namedWindow("Display Window", cv::WINDOW_AUTOSIZE);

    while (true) {
        // Send a "capture" message to request a new image
        ws.write(boost::beast::net::buffer(std::string("capture")));

        // Read the image data from the WebSocket
        ws.read(buffer);

        // Convert the received data to a cv::Mat image
        std::vector<uint8_t> imageData(boost::asio::buffers_begin(buffer.data()), boost::asio::buffers_end(buffer.data()));
        image = cv::Mat(height, width, CV_8UC1, imageData.data());

        // Display the image
        cv::imshow("Display Window", image);

        // Wait for a key press (or a longer time if needed)
        int key = cv::waitKey(1);

        // Clear the buffer to flush it
        buffer.consume(buffer.size());

        // Check if the ESC key (key code 27) is pressed
        if (key == 27) {
            break; // Exit the loop if ESC is pressed
        }
    }

    cv::destroyAllWindows();
}



void dual_vision(boost::beast::websocket::stream<boost::asio::ip::tcp::socket>& ws_left, boost::beast::websocket::stream<boost::asio::ip::tcp::socket>& ws_right, int width, int height) {
    if (!ws_left.is_open() || !ws_right.is_open())
        throw runtime_error("oh my ! one or two of the websockets is not open !");
    std::mutex mtx;
    boost::beast::flat_buffer buffer_left ,buffer_right;
    cv::Mat image_left,image_right, concatenated_image;
    cv::namedWindow("Display Window", cv::WINDOW_AUTOSIZE);
    while (true) {
        std::thread left([&ws_left,&buffer_left,&image_left,height,width]() {
            ws_left.write(boost::beast::net::buffer(std::string("capture")));
            ws_left.read(buffer_left);
            std::vector<uint8_t> imageData(boost::asio::buffers_begin(buffer_left.data()), boost::asio::buffers_end(buffer_left.data()));
            image_left = cv::Mat(height, width, CV_8UC1, imageData.data());
            });

        std::thread right([&ws_right, &buffer_right, &image_right, height, width]() {
            ws_right.write(boost::beast::net::buffer(std::string("capture")));
            ws_right.read(buffer_right);
            std::vector<uint8_t> imageData(boost::asio::buffers_begin(buffer_right.data()), boost::asio::buffers_end(buffer_right.data()));
            image_right = cv::Mat(height, width, CV_8UC1, imageData.data());
            });
        
        left.join();
        right.join();
        cv::Mat imagesToConcatenate;
        imagesToConcatenate.push_back(image_left);
        imagesToConcatenate.push_back(image_right);
        cv::hconcat(imagesToConcatenate, concatenated_image);
        
        // Display the image
        cv::imshow("Display Window", concatenated_image);

        // Wait for a key press (or a longer time if needed)
        int key = cv::waitKey(1);

        // Clear the buffer to flush it
        buffer_left.consume(buffer_left.size());
        buffer_right.consume(buffer_right.size());

        // Check if the ESC key (key code 27) is pressed
        if (key == 27) {
            break; // Exit the loop if ESC is pressed
        }
    }


    cv::destroyAllWindows();
}
