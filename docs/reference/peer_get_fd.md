# Get the socket file descriptor

Returns the internal UDP socket file descriptor. Intended for advanced
use cases such as custom select loops.

## Usage

``` r
peer_get_fd()
```

## Value

Integer. The socket file descriptor, or `-1` if not connected.

## Examples

``` r
if (FALSE) { # \dontrun{
fd <- peer_get_fd()
} # }
```
