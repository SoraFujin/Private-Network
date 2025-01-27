# Private Network
The Main coding Languiage Language that will be used to create this project is C

## Problem and the Scope:
### What is a Socket:
A socket is an endpoint for sending or receiving data accross a network. In a server-client architecture, the Server listens for incoming connection on aspecific port, and clients connect to it.

### Role of C in Networking:
C is a low-level language that is ideal fro network programming becuase it provides direct axcccess to system-level resources like sockets, memory and threads. For this project I will using POSIX system calls to create and mange sockets.

### Key concepts:
- **Networking Basics**: Protocols, ports, and IP addresses.
- **System calls**: Functions like socket(), bind(), listen(), and accept() for server operations.
- **Concurrency**: Handling multiple clients using multi-threading or asynchronous techniques.
- **Security**: Encrypting data (using libraries like OpenSSL).

---

## Basics of Networking:
### Network Protocols:
 - **TCP (Transmission Control Protocol)**: reliable connection-oriented protocol where data is sent in a stream.
 - **UDP (User Datagram Protocol)**: A faster but unreliable, connectionless protocol.

 ### Socket Basics:
 - A **Socket** is like a file descriptor that you use to send and receive data.
 - In C, sockets are represented by integer values, and you use system calss to manipulate them.

 ### Server-Client Model:
 - **Server**: Listens on a port for incoming connection.
 - **Clinet**: initiates a connections to the server.

 ---

## System Programming Concepts:
### System Calls for Sockets:
- **Socket related system calls**:
    1. **socket()**: Create a socket.
    2. **bind()**: Bind the socket to a port and IP.
    3. **listen()**: Put the socket in listening mode.
    4. **accept()**: Accept incoming connections.
    5. **send()/recv()**: Send and Receive data.
<!-- - **POSIX Standards**:
    - learn about  -->
Network Byte Order:
    Data needs to be sent in network byte order. Learn about htons() and ntohs() for converting between host and network byte order.

---

```
backend/
├── docs/                      # Documentation and explanations
├── include/                   # Header files for reusable code
│   ├── http_server.h          # Declarations for HTTP server functions
│   ├── websocket_server.h     # (Optional) WebSocket declarations
│   ├── json_handler.h         # JSON serialization/deserialization functions
│   └── utils.h                # General utilities (logging, helpers, etc.)
├── lib/                       # Libraries (optional)
│   └── (Add custom or third-party libraries like libmicrohttpd or cJSON here)
├── output/                    # Compiled executables
├── src/                       # Core source code
│   ├── application_layer/     # Application-specific logic (e.g., data processing)
│   ├── client/                # Logic for client-side operations (if needed)
│   ├── connection/            # Handles network connections
│   │   ├── http_server.c      # HTTP server implementation
│   │   └── websocket_server.c # WebSocket implementation (if used)
│   ├── data_link_layer/       # Data link layer operations (if applicable)
│   ├── encryption/            # Encryption algorithms and logic
│   ├── network_layer/         # Network-layer logic
│   ├── physical_layer/        # Physical-layer logic (e.g., hardware-specific code)
│   ├── protocols/             # Protocol-specific code
│   │   ├── rest_protocol.c    # REST API handling logic
│   │   ├── json_handler.c     # JSON serialization/deserialization
│   │   └── (Other protocols if needed)
│   ├── security/              # Security-related code (authentication, etc.)
│   ├── server/                # Server entry point
│   │   └── server_main.c      # Main server file
│   ├── tests/                 # Unit tests
│   ├── transport_layer/       # Transport layer operations
│   └── utils/                 # General utilities (logging, error handling, etc.)
└── server.c                   # High-level server entry point
```


**The Project will be cut into different Phases**:
# Phase 1 Milestones:
1. **Basic Socket Communication**: Server and client communicating over a specific port.
2. **Basic Logging**: Log file storing connection details (IPs, timestamps).
3. **Secure Communication**: TLS/SSL encryption, token-based authentication.
4. **Custom Protocol**: Define and implement a custom protocol for communication.
5. **File Access**: Encrypted file storage and retrieval with token-based access levels.

---

# 1. Basic Socket Communication: