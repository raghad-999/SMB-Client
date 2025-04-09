# SMB Client

This project implements a simple **SMB client** using the SMB2 protocol to interact with a Samba server. It provides basic file operations like creating directories, creating files, writing data, and reading data from the server.

## Features
- Initialize and manage an SMB2 context.
- Parse a given URL to connect to a Samba server.
- Connect to a Samba server and interact with shared directories.
- Create directories and files on the server.
- Read and write data to files on the server.

## Prerequisites
- A **Samba server** running.
- The **libsmb2** library installed for SMB2 protocol support.
