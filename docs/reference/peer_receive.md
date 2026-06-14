# Receive an incoming packet

Waits for an incoming UDP packet with the given timeout. Automatically
processes the packet (e.g., stores newly discovered peers, prints
received messages).

## Usage

``` r
peer_receive(timeout_ms = 100L)
```

## Arguments

- timeout_ms:

  Integer. Maximum time to wait in milliseconds. Use 0 for non-blocking.
  Default is 100.

## Value

Integer. The packet type on success (`>=0`), or `-1` on timeout or
error.

## Examples

``` r
if (FALSE) { # \dontrun{
packet_type <- peer_receive(100)
} # }
```
