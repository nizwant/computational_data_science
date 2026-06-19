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