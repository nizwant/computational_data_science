# Ping a peer

Sends a ping packet to a previously discovered peer, works also on
server.

## Usage

``` r
peer_send_ping(recipient)
```

## Arguments

- recipient:

  Character string. The username of the peer to ping.

## Value

Invisible `NULL`. Raises an error if the peer is not found or the send
fails.

## Examples

``` r
if (FALSE) { # \dontrun{
peer_send_ping("max")
} # }
```
