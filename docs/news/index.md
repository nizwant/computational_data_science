# Changelog

## PeerChat 0.6.0 (2026-06-15)

- Added Python unit tests
- Added GitHub Actions CI workflow for Python unit tests
- Added Python vignette documenting the Python API

## PeerChat 0.5.0 (2026-06-14)

- Added Python bindings via C/Python API (`src/py_peer_chat.c`)
- Added `PEERCHAT_SERVER_IP` environment variable override for
  configurable server ip address
- Added R unit tests in testthat

## PeerChat 0.4.0 (2026-06-14)

- Added pkgdown site
- Added interactive R client vignette mirroring the C client
- Added `.Rbuildignore` to exclude non-package files from the tarball
- Added GitHub Actions CI workflow for R CMD check on macOS and Linux
- Fixed R CMD check warnings
- Added R CMD check status badge to README

## PeerChat 0.3.0 (2026-06-13)

- Added R package bindings via Rcpp
- Generated roxygen2 documentation (`man/` pages)
- Added `Makevars` for native compilation of C sources within the R
  package

## PeerChat 0.2.0 (2026-06-10)

- Introduced `client_lib` API: `peer_connect`, `peer_send_message`,
  `peer_send_ping`, `peer_get_user`, `peer_receive`, `peer_disconnect`
- Hashmap internally in C - hidden from the users
- Added `DEBUG_PRINT` macro for compile-time debug output (`-DDEBUG`)
- Cleaned up project structure: removed `shared_patterns.h` and
  `client.h`

## PeerChat 0.1.0 (2026-05-31)

- Initial release
- UDP peer-to-peer chat with central synchronization server
- Client commands: /get_user, /message, /ping, /quit
- Peer discovery via server-mediated introduction
- hash table based peer tracking on the server
