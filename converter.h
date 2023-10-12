#pragma once
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <string>



void saveFlatBufferAsGrayscaleJPEG(const boost::beast::flat_buffer& buffer, int width, int height, const std::string& filename) {
    // Convert the data from boost::beast::flat_buffer to a std::vector
    std::vector<uint8_t> imageData(boost::asio::buffers_begin(buffer.data()), boost::asio::buffers_end(buffer.data()));

    // Create an OpenCV Mat object from the vector data
    cv::Mat image(height, width, CV_8UC1, imageData.data());

    // Write the image to a JPEG file
    cv::imwrite(filename, image);
}