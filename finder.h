#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <cstdlib>


// Function to run a command and capture its output
std::string executeCommand(const char* cmd) {
    std::string result;
    char buffer[128];

    FILE* pipe = _popen(cmd, "r");
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    int exitCode = _pclose(pipe);
    if (exitCode != 0) {
        throw std::runtime_error("Command execution failed.");
    }

    return result;
}

// Function to parse IP addresses from the arp -a output
std::vector<std::string> parseIPAddresses(const std::string& arpOutput) {
    std::vector<std::string> ipAddresses;

    // Define a regular expression pattern to match IPv4 addresses
    std::regex pattern(R"((\d+\.\d+\.\d+\.\d+))");
    std::smatch matches;

    // Search for matches in the arp output
    std::string::const_iterator searchStart(arpOutput.cbegin());
    while (std::regex_search(searchStart, arpOutput.cend(), matches, pattern)) {
        ipAddresses.push_back(matches[1]);
        searchStart = matches.suffix().first;
    }

    return ipAddresses;
}