/*
int main() {
    const char* cmd = "arp -a";

    try {
        std::string arpOutput = executeCommand(cmd);
        std::vector<std::string> ipAddresses = parseIPAddresses(arpOutput);

        // Print the list of IP addresses
        for (const std::string& ipAddress : ipAddresses) {
            std::cout << "IP Address: " << ipAddress << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
*/
