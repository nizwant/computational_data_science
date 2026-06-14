# Send a message to a peer

Sends a UDP message to a previously discovered peer. The peer must have
been registered via
[`peer_get_user`](https://nizwant.github.io/computational_data_science/reference/peer_get_user.md)
first.

## Usage

``` r
peer_send_message(recipient, message)
```

## Arguments

- recipient:

  Character string. The username of the recipient.

- message:

  Character string. The message to send (max 1024 bytes).

## Value

Invisible `NULL`. Raises an error if the peer is not found or the send
fails.

## Examples

``` r
if (FALSE) { # \dontrun{
peer_send_message("max", "skibidi!")
} # }
```
