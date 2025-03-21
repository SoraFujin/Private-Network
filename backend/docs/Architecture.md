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







# Network layers:
The network consists of layers each responsible for different tasks: <br>
1. **Application**: Interface for software (Web, messaging) (HTTP, FTP, SSH).
2. **Transport**: Reliable end-to-end communication (TCP, UDP)
3. **Network**: Routing and addressing (IP)
4. **Data Link**: Medium access, framing error detection (Ethernet, Wi-Fi)
5. **Physical**: Transmission of raw bits (Copper wire, fiber, radio)


To be able to build everything from scartch we need to start from the bottom up, therefore starting with the physical layer that focuses on how the bits are being transported through physical hardware.


# Physical Layer 
In this layer, is where data is phyiscally transmitted between devices<br>
The physical layer is the lowest layer of a network that is responsible for:
1. Sending and receiving raw bits (0s and 1s)
2. Defining voltage levels, timing and synchronization between devices 
3. Choosing a transmission medium (copper wire, fiber, Wireless)


### Choices for Physical Transmission:
Since I have two Arduinos the easist way to send data is through a wired connection but in order to do so there are different methods that can help:
1. GPIO Direct Signaling: Set HIGH (1) or LOW (0) on a pin, it is simple and needs precise timing
2. UART (Serial communication): Uses TX/RX pins that is faster and easier, but less control over signal.
3. SPI/I2C: Uses a clock signal Synchronization built in and is more complex to implement. 

In order to start we need 2 Arduinos (Sender and Receiver), One wire to connect to TX (Sender) -> RX (Receiver) and one common ground GND wire between them.

We use HIGH (5v) for 1 and low (0V) for 0 to be able to send data from the sender to the receiver. With a fixed timing delay (baud rate).
*baud rate: The measure of the number of changes to the signal per second that propagate througha transmisson medium.*
The change of high and low voltage from the sender to the reciever.


# Data Link Layer 
Responsible for framing to define how data is structured into pakcets, addressing adn assigning unique IDs (MAC addresses) to devices, detect error and handle transmission errors and the flow control Managing data speed so the receiver is not overwhelmed.
to implement this layer identify how the frames will be sent what protocol to use, how to use them and how the data is checked if there is an error, either to wait for an ack or sent multiple frames per message depending on the window frame size.
