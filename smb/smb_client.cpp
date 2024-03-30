#include "smb_client.hpp"

SMBClient::SMBClient() : smb2(nullptr), url(nullptr) {}

SMBClient::~SMBClient()
{
    if (smb2)
        smb2_destroy_context(smb2);
}

void SMBClient::initialize_context()
{
    std::cout << "Initializing smb2 context..." << std::endl;
    smb2 = smb2_init_context();
    if (!smb2) {
        std::cerr << "Failed to initialize smb2 context" << std::endl;
        exit(1);
    }
}

void SMBClient::parse_url(const std::string &url_str)
{
    std::cout << "Parsing URL..." << std::endl;
    url = smb2_parse_url(smb2, url_str.c_str());
    if (!url) {
        std::cerr << "Failed to parse URL: " << smb2_get_error(smb2) << std::endl;
        exit(1);
    }

    std::cout << "URL parsed successfully:" << std::endl;
    std::cout << "Server: " << url->server  << ", Share: " << url->share << ", User: " << url->user << std::endl;
}


void SMBClient::connect_to_server()
{
    std::cout << "Connecting to Samba server..." << std::endl;
    int connect_res = smb2_connect_share(smb2, url->server, url->share, url->user);
    if (connect_res) {
        std::cerr << "Failed to connect to Samba server: " << smb2_get_error(smb2) << std::endl;
        exit(1);
    }
    std::cout << "Connected successfully to Samba server" << std::endl;
}

void SMBClient::mkdir(const std::string &dir_name)
{
    std::cout << "Creating directory '" << dir_name << "'..." << std::endl;
    int mkdir_res = smb2_mkdir(smb2, dir_name.c_str());
    if (mkdir_res) {
        std::cerr << "Failed to create directory '" << dir_name << "': " << smb2_get_error(smb2) << std::endl;
        exit(1);
    }
    std::cout << "Directory '" << dir_name << "' created successfully" << std::endl;
}

smb2fh* SMBClient::create(const std::string &file_name)
{
    std::cout << "Creating new file '" << file_name << "'..." << std::endl;
    smb2fh* fh = smb2_open(smb2, file_name.c_str(), O_RDWR | O_CREAT);
    if (!fh) {
        std::cerr << "Failed to create file '" << file_name << "': " << smb2_get_error(smb2) << std::endl;
        exit(1);
    }
    std::cout << "File '" << file_name << "' created successfully" << std::endl;

    return fh;
}

void SMBClient::write(smb2fh* fh, const std::string &data)
{
    std::cout << "Writing data to file..." << std::endl;
    int bytes_written = smb2_write(smb2, fh, reinterpret_cast<const uint8_t*>(data.c_str()), data.size());
    if (bytes_written < 0) {
        std::cerr << "Failed to write data to file: " << smb2_get_error(smb2) << std::endl;
        exit(1);
    }
    std::cout << "Data written successfully to file" << std::endl;
}

void SMBClient::read(smb2fh* fh, std::string &data, size_t buffer_size) 
{
    // Seek to the beginning of the file
    std::cout << "Seeking to the beginning of the file..." << std::endl;
    if (smb2_lseek(smb2, fh, 0, SEEK_SET, NULL) < 0)
    {
        std::cerr << "Failed to seek to the beginning of the file: " << smb2_get_error(smb2) << std::endl;
        smb2_close(smb2, fh);
        smb2_destroy_context(smb2);
        return;
    }

    uint8_t read_data[buffer_size];
    std::cout << "Reading data from file..." << std::endl;
    int bytes_read = smb2_read(smb2, fh, read_data, buffer_size - 1);
    if (bytes_read < 0) {
        std::cerr << "Failed to read data from file: " << smb2_get_error(smb2) << std::endl;
        exit(1);
    }
    read_data[bytes_read] = '\0'; // Null-terminate the buffer
    data = reinterpret_cast<char*>(read_data); // Convert uint8_t array to string
    std::cout << "Read " << bytes_read << " bytes: " << data << std::endl;
}

void SMBClient::close(smb2fh* fh)
{
    std::cout << "Closing file..." << std::endl;
    smb2_close(smb2, fh);
}
