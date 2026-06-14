# Connect to the PeerChat server

Establishes a UDP connection to the synchronization server and registers
the client with the given username and password. Must be called before
any other PeerChat function. It also creates hashmap that stores all
necessary info.

## Usage

``` r
peer_connect(username, password)
```

## Arguments

- username:

  Character string. The username to register with on the server.

- password:

  Character string. The password that other users needs to know to
  message you.

## Value

Invisible `NULL`. Raises an error on failure.

## Examples

``` r
if (FALSE) { # \dontrun{
peer_connect("nizwan", "password123")
} # }
```
