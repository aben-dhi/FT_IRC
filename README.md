# IRC Server

This project is an implementation of an Internet Relay Chat (IRC) server. IRC is a protocol used for real-time text-based communication over the internet. This server allows multiple clients to connect, join channels, send messages, and perform various IRC commands.

---

## Project Structure

The project is organized into several directories and files:

![Project Structure](path-to-your-image/project-structure.png)

---

## Key Components

- **Server**: The core of the IRC server, handling client connections, requests, and managing channels.
- **Client**: Represents a connected user, managing their state and interactions.
- **Channel**: Represents a chat room where multiple clients can join and communicate.
- **Request**: Handles parsing and validation of client commands.
- **File**: Manages file transfer between clients.

---

## Main Files and Their Roles

- `main.cpp`: Entry point of the application. Initializes the server and starts it.
- `Makefile`: Contains build instructions for compiling the project.
- `ReadMe.txt`: Provides an overview of the IRC architecture and protocol.

---

## Source Files

| File                | Description                                                                                   |
|---------------------|-----------------------------------------------------------------------------------------------|
| `Channel.cpp`       | Implements the Channel class, managing operations like adding/removing members, topics, etc. |
| `Client.cpp`        | Manages client state, authentication, and channel memberships.                               |
| `commands.cpp`      | Implements IRC commands like JOIN, PART, KICK, PRIVMSG, etc.                                 |
| `File.cpp`          | Manages file transfer operations.                                                            |
| `getSocket.cpp`     | Functions for setting up and managing server sockets.                                        |
| `Join.cpp`          | Handles the JOIN command.                                                                   |
| `Kick.cpp`          | Handles the KICK command.                                                                   |
| `message.cpp`       | Functions for sending messages to clients and channels.                                      |
| `Part.cpp`          | Handles the PART command.                                                                   |
| `pollHandling.cpp`  | Manages the polling mechanism for handling multiple client connections.                      |
| `privMsg.cpp`       | Handles the PRIVMSG command.                                                                |
| `Request.cpp`       | Parses and validates client commands.                                                       |
| `requestHandling.cpp`| Functions for handling client requests and parsing commands.                                |
| `RoboServ.cpp`      | Bot service providing server statistics and information.                                     |
| `Server.cpp`        | Manages client connections, channels, and overall server operations.                        |
| `transferFile.cpp`  | Handles file transfer commands like SENDFILE and GETFILE.                                    |
| `utils.cpp`         | Utility functions used throughout the project.                                               |

---

## Header Files

- `Channel.hpp`: Declares the Channel class and its members.
- `Client.hpp`: Declares the Client class and its members.
- `File.hpp`: Declares the File class and its members.
- `Request.hpp`: Declares the Request class and its members.
- `Server.hpp`: Declares the Server class and its members, along with utility functions and constants.

---

## Building the Project

To build the project, use the provided Makefile. Run the following command in the terminal:

```bash
make
```
This will This will compile the source files and generate the executable ircserv.

## Running the Server

To run the server, use the following command:

```bash
./ircserv <port> <password>
```

Replace <port> with the desired port number and <password> with the server password.

---

## Features

- **Client Authentication** : Clients must authenticate using a password before joining the server.
- **Channel Management** : Clients can create, join, and leave channels. Channel operators can manage channel settings and kick users.
- **Private Messaging** : Clients can send private messages to other users or channels.
- **File Transfer** : Clients can send and receive files.
- **Bot Service** : Provides server statistics and information through a bot.

---

This project provides a comprehensive implementation of an IRC server, supporting various IRC commands and features. It is designed to handle multiple client connections and provide a robust and scalable chat service.
