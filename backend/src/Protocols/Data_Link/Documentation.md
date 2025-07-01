# Data Link Layer

The Data Link Layer ensures reliable communication between directly connected devices.  
It is responsible for the following key functions:

- **Framing**: Defining how data is packaged into frames for transmission.  
- **Addressing**: Assigning unique IDs to devices so data is sent to the correct recipient.  
- **Error Detection**: Ensuring data is received correctly without corruption.  

---

## Protocols Implemented in the Data Link Layer

1. [SDLC – Synchronous Data Link Control](#1-sdlc--synchronous-data-link-control)  
2. [HDLC – High-Level Data Link Control](#2-hdlc--high-level-data-link-control)  
3. [SLIP – Serial Line Internet Protocol](#3-slip--serial-line-internet-protocol)  
4. [PPP – Point-to-Point Protocol](#4-ppp--point-to-point-protocol)  
5. [LCP – Link Control Protocol](#5-lcp--link-control-protocol)  
6. [LAP – Link Access Procedure](#6-lap--link-access-procedure)  
7. [NCP – Network Control Protocol](#7-ncp--network-control-protocol)  

---

## 1. SDLC – Synchronous Data Link Control

SDLC is a bit-oriented synchronous protocol developed by IBM. It operates in a master-slave configuration, where one primary station controls the communication. It ensures reliable and ordered data delivery over synchronous serial links.  
While historically significant, SDLC is largely obsolete in modern Ethernet or wireless environments.

---

## 2. HDLC – High-Level Data Link Control

HDLC is a refined version of SDLC, standardized by ISO. It supports both point-to-point and multipoint configurations and includes mechanisms for framing, flow control, and error detection.  
HDLC is commonly used in synchronous networks and forms the basis for many other protocols, including PPP. Though not used directly in Wi-Fi, its principles influence other wireless data link protocols.

---

## 3. SLIP – Serial Line Internet Protocol

SLIP is a very simple protocol used to encapsulate IP packets over serial connections. It offers no error detection, compression, or framing beyond marking packet boundaries.  
Due to its limitations, SLIP has been deprecated and replaced by more robust protocols like PPP. It is not suitable for Wi-Fi or modern wired communication.

---

## 4. PPP – Point-to-Point Protocol

PPP is a widely used protocol that encapsulates network layer protocol information over point-to-point links. It supports authentication, compression, and error detection.  
PPP is used over both wired (serial cables, DSL) and some wireless links. It is suitable for transmitting data over modems and certain wireless links like Wi-Fi when no higher protocol is in use.

---

## 5. LCP – Link Control Protocol

LCP is a sub-protocol of PPP. It is responsible for establishing, configuring, and testing the data link connection.  
LCP negotiates options such as authentication methods and link parameters before actual data transmission. While not a standalone protocol, LCP plays a critical role in making PPP function reliably over both wired and wireless links.

---

## 6. LAP – Link Access Procedure

LAP refers to several variations used in different network technologies (e.g., LAP-B in X.25). It provides error correction and retransmission for reliable data delivery over point-to-point links.  
LAP protocols are less common today but influenced modern protocol designs. Their application in Wi-Fi and modern Ethernet is limited.

---

## 7. NCP – Network Control Protocol

NCP operates within the PPP framework and is used to configure and control different network layer protocols (such as IP or IPv6) over a PPP link.  
Each network protocol has a corresponding NCP to negotiate and set up necessary parameters. NCP is essential for enabling IP communication over PPP in wired or wireless contexts.

---

## Focus Summary

For modern communication over Wi-Fi or Ethernet, **PPP and its components (LCP, NCP)** are the most relevant in this list.  
They enable point-to-point connectivity and are foundational for DSL, mobile networks, and older dial-up systems.  
**HDLC** remains important historically and conceptually, especially in systems that evolved into current data link technologies.

> **Note**: In general, it is not necessary to implement these protocols from the start.  
> A custom Data Link Layer with proper functionality (framing, addressing, error detection) will suffice for most controlled implementations.

---

# Frame Construction and Transmission Design

To support the transmission of arbitrary data sizes over a custom network stack, this Data Link Layer is designed to:

- Handle fragmentation of large data into multiple frames.
- Enable re-synchronization and error detection using structured headers.
- Maintain efficient parsing using index-based field access instead of structs.
- Operate independently of external libraries or frameworks.

---

## Frame Format

Each transmitted unit (frame) follows a consistent format. All fields are accessed via fixed indexes in a byte array.

| Field     | Size (Bytes) | Description                                       |
|-----------|---------------|---------------------------------------------------|
| `SOF`     | 1             | Start-of-Frame marker (e.g., `0xA5`)              |
| `DEST`    | 1             | Destination node ID                               |
| `SRC`     | 1             | Source node ID                                    |
| `LENGTH`  | 2             | Length of the payload in bytes                    |
| `SEQ_NUM` | 4             | Frame sequence number for ordering                |
| `FLAGS`   | 1             | Frame flags (e.g., end-of-message indicator)      |
| `PAYLOAD` | N             | Data segment (length defined by `LENGTH`)         |
| `CHECKSUM`| 2             | CRC16 checksum over payload                       |

> **Total Frame Size = 12 bytes (header) + N bytes (payload)** <br>
> **Frame Size = 12 + N**
---

## Implementation Details

### Index-Based Frame Construction

- Frames are built using raw `uint8_t[]` arrays for low-level control.
- Each byte index is mapped to a specific field for fast parsing.
- This avoids the memory overhead and portability issues of using C structs.

### Dynamic Payload Allocation

- Payloads are allocated dynamically based on message size.
- Messages are split into multiple fixed-size frames (e.g., 1024 bytes per frame) if they exceed the frame capacity.

### Sequence Numbering

- Each frame is assigned an incremental `SEQ_NUM`.
- This enables correct reassembly of the full message at the receiver.
- Missing or duplicate frames can be detected by comparing sequence numbers.

### Flags Field

- A `FLAGS` byte is used to mark special properties.
  - `0x01`: Indicates this is the final frame in a message sequence.
- Additional bits may support:
  - Re-transmission
  - Priority levels
  - Acknowledgment control

### Error Detection

- A CRC-16 check-sum is calculated over the payload and appended to each frame.
- The receiver verifies the check_sum before accepting the frame.
- Corrupted frames are discarded or marked for re-transmission.

### Start-of-Frame (SOF) Synchronization

- A unique SOF byte (e.g., `0xA5`) is used to identify the start of each frame.
- The receiver scans for this marker to align parsing after noise or data loss.
- This enables recovery from mid-stream corruption or misalignment.

---

## Reassembly at the Receiver

1. The receiver reads bytes from a stream or socket.
2. Upon detecting a valid `SOF`, it reads the frame based on `LENGTH`.
3. Using `SEQ_NUM`, the receiver places the payload in correct order.
4. When a frame with the `final frame` flag is received, the message is considered complete.

---

## Efficiency Considerations

- Frame headers are fixed-size, allowing fast and consistent parsing.
- When using large payloads (e.g., 1500 bytes), header overhead becomes negligible.
- Dynamic memory is allocated only when necessary, and reclaimed after use.
- Only essential meta data is included per frame to maintain performance.

---

## Example Use Case: Sending a Large File

1. The file is split into payload-sized chunks.
2. Each chunk is wrapped into a frame with headers and check-sum.
3. Frames are sent over the communication channel in sequence.
4. The receiver uses `SEQ_NUM` and `FLAGS` to reassemble the original file.
5. Completion is detected when the `final frame` flag is received.

