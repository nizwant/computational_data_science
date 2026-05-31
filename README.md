# Computational Data Science

WUT course aimed at learning C with Data Science algorithms in mind.

## Project: UDP P2P Chat

A peer-to-peer messaging built on UDP with a central synchronization server for peer discovery.

### How it works

1. Client registers with the server via username
2. Client pings the server and known peers for connection to persist
3. To message someone, request their address from the server via `/get_user`
4. Server validates credentials and introduces both peers to each other
5. Peers communicate directly (no relay through server)

## Course materials

Materials found in `classes` directory were made by the author of the course [Prof. Marek Gagolewski](https://www.gagolewski.com/index.html).
