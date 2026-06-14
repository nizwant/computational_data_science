# Discover a peer by username

Sends a request to the server to look up a peer's network address. On
success, the peer is stored internally and later on can be messaged or
pinged. Requires prior "registration" via
[`peer_connect`](https://nizwant.github.io/computational_data_science/reference/peer_connect.md).

## Usage

``` r
peer_get_user(username, password)
```

## Arguments

- username:

  Character string. The username of the peer to discover.

- password:

  Character string. The password of the peer.

## Value

Invisible `NULL`. Raises an error on failure.

## Examples

``` r
if (FALSE) { # \dontrun{
peer_get_user("max", "max_password")
} # }
```
