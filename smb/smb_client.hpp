#pragma once

#include <iostream>
#include <smb2/libsmb2.h>
#include <smb2/smb2.h>
#include <fcntl.h>

class SMBClient {
public:
    SMBClient();
    ~SMBClient();

    void initialize_context();
    void parse_url(const std::string &url_str);
    void connect_to_server();
    void mkdir(const std::string &dir_name);
    smb2fh* create(const std::string &file_name);
    void write(smb2fh* fh, const std::string &data);
    void read(smb2fh* fh, std::string &data, size_t buffer_size);
    void close(smb2fh* fh);

private:
    struct smb2_context *smb2;
    struct smb2_url *url;
};
