#include "smb_client.hpp"

int main() {
    SMBClient client;

    // Initialize the SMB client context
    client.initialize_context();

    // Parse the URL
    std::string urlStr =  "smb://raghad@127.0.0.1/myshare?sec=ntlmssp";
    client.parse_url(urlStr);

    // Connect to the Samba server
    client.connect_to_server();

    // Create a directory
    client.mkdir("test_dir");

    // Create a new file and get the file handle
    smb2fh* fh = client.create("test_file.txt");

    // Write data to the file
    std::string data = "Hello, Samba!";
    client.write(fh, data);

    // Read data from the file
    std::string readData;
    size_t bufferSize = 13; // Specify buffer size as needed
    client.read(fh, readData, bufferSize);

    // Close the file
    client.close(fh);

    return 0;
}
