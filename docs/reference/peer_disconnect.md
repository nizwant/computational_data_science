# Disconnect from PeerChat

Closes the UDP socket and frees all internal state including the peer
hashmap. Call this when done using PeerChat.

## Usage

``` r
peer_disconnect()
```

## Value

Invisible `NULL`.

## Examples

``` r
if (FALSE) { # \dontrun{
peer_disconnect()
} # }
```
