# PeerChat

UDP peer-to-peer messaging library backed by a C networking core.
Provides R and Python bindings for creating sockets, discovering peers
via a central synchronization server written in C, and exchanging
messages directly between clients without relaying through the server.
It leverages a custom made data protocol.

## How it works

1.  Client registers with the server via username and password
2.  Client pings the server and known peers for connection to persist
3.  To message someone, request their address from the server via
    [`peer_get_user()`](https://nizwant.github.io/computational_data_science/reference/peer_get_user.md)
4.  Server validates credentials and introduces both peers to each other
5.  Peers communicate directly (no relay through server)

## Installation

### R

Install the development version from GitHub:

``` r

# install.packages("devtools")
devtools::install_github("nizwant/computational_data_science")
```

### Python

``` bash
pip install git+https://github.com/nizwant/computational_data_science.git
```

### From source

``` bash
git clone https://github.com/nizwant/computational_data_science.git
cd computational_data_science

# R
R CMD INSTALL .

# Python
pip install .
```

## Quick start (R)

``` r

library(PeerChat)

# Connect to the server
peer_connect("my_username", "my_password")

# Discover a peer
peer_get_user("friend", "friends_password")

# Send a message
peer_send_message("friend", "Hello my friend!")

# Receive incoming packets
peer_receive(1000)

# Disconnect
peer_disconnect()
```

## Computational Data Science

This repo is for WUT course aimed at learning C with Data Science
algorithms in mind. It features a peer-to-peer messaging built on UDP
with a central synchronization server for peer discovery.

## Course materials

Materials found in `classes` directory were made by the author of the
course [Prof. Marek Gagolewski](https://www.gagolewski.com/index.html).
