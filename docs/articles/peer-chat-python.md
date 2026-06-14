# Interactive Client in Python

This vignette shows how to use the PeerChat Python bindings — a CPython
C extension that wraps the same networking core used by the R package.

## Installation

``` bash
pip install .
```

This compiles the C extension (`py_peer_chat.c`, `client_lib.c`,
`c_common.c`) and installs the `PeerChat` package.

## API Reference

All functions are available directly from the `PeerChat` namespace:

| Function | Signature | Description |
|----|----|----|
| `connect` | `connect(username, password)` | Connect to the server and register. Raises `ConnectionError` on failure. |
| `get_user` | `get_user(username, password)` | Request a peer’s address from the server. Raises `RuntimeError` on failure. |
| `send_message` | `send_message(recipient, message)` | Send a message to a discovered peer. Raises `RuntimeError` if peer unknown. |
| `send_ping` | `send_ping(recipient)` | Ping a discovered peer. Raises `RuntimeError` if peer unknown. |
| `receive` | `receive(timeout_ms=100)` | Receive one incoming packet. Returns the packet type (≥0) or −1 on timeout. |
| `disconnect` | `disconnect()` | Close connection and free all internal state. |
| `get_fd` | `get_fd()` | Return the internal socket file descriptor (−1 if not connected). |

## Interactive Client

The Python script below replicates the C client’s interactive loop using
`select()` to multiplex between stdin and the UDP socket.

``` python
import sys
import select
import PeerChat


def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <username> <password>")
        sys.exit(1)

    username, password = sys.argv[1], sys.argv[2]

    PeerChat.connect(username, password)
    print(f"Connected as '{username}'. Type /help for commands.")

    sock_fd = PeerChat.get_fd()

    try:
        while True:
            # Wait for input on stdin or the UDP socket
            readable, _, _ = select.select([sys.stdin, sock_fd], [], [], 0.1)

            if sock_fd in readable:
                PeerChat.receive(0)

            if sys.stdin in readable:
                line = sys.stdin.readline().strip()
                if not line:
                    continue

                if line == "/quit":
                    break
                elif line == "/help":
                    print(
                        "Commands:\n"
                        "  /get_user <username> <password>  — discover a peer\n"
                        "  /message  <username> <text>      — send a message\n"
                        "  /ping     <username>             — ping a peer\n"
                        "  /quit                            — disconnect and exit"
                    )
                elif line.startswith("/get_user"):
                    parts = line.split()
                    if len(parts) == 3:
                        PeerChat.get_user(parts[1], parts[2])
                    else:
                        print("Usage: /get_user <username> <password>")
                elif line.startswith("/message"):
                    parts = line.split(maxsplit=2)
                    if len(parts) >= 3:
                        try:
                            PeerChat.send_message(parts[1], parts[2])
                        except RuntimeError:
                            print(f"Peer '{parts[1]}' not found. Use /get_user first.")
                    else:
                        print("Usage: /message <username> <text>")
                elif line.startswith("/ping"):
                    parts = line.split()
                    if len(parts) == 2:
                        try:
                            PeerChat.send_ping(parts[1])
                        except RuntimeError:
                            print(f"Peer '{parts[1]}' not found. Use /get_user first.")
                    else:
                        print("Usage: /ping <username>")
                else:
                    print("Unknown command. Type /help for available commands.")
    finally:
        PeerChat.disconnect()
        print("Disconnected.")


if __name__ == "__main__":
    main()
```

## Quick example

``` python
import PeerChat

# Register and connect
PeerChat.connect("alice", "secret123")

# Discover a peer
PeerChat.get_user("bob", "bob_pass")
PeerChat.receive(1000)  # wait for server response

# Chat
PeerChat.send_message("bob", "Hello from Python!")

# Clean up
PeerChat.disconnect()
```
