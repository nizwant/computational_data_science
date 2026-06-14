#' Connect to the PeerChat server
#'
#' Establishes a UDP connection to the synchronization server and registers
#' the client with the given username and password. Must be called before
#' any other PeerChat function. It also creates hashmap that stores all necessary info.
#'
#' @param username Character string. The username to register with on the server.
#' @param password Character string. The password that other users needs to know to message you.
#' @return Invisible \code{NULL}. Raises an error on failure.
#' @export
#' @examples
#' \dontrun{
#' peer_connect("nizwan", "password123")
#' }
peer_connect <- function(username, password) {
    invisible(peer_connect_r(username, password))
}

#' Discover a peer by username
#'
#' Sends a request to the server to look up a peer's network address.
#' On success, the peer is stored internally and later on can be messaged or pinged.
#' Requires prior "registration" via \code{\link{peer_connect}}.
#'
#' @param username Character string. The username of the peer to discover.
#' @param password Character string. The password of the peer.
#' @return Invisible \code{NULL}. Raises an error on failure.
#' @export
#' @examples
#' \dontrun{
#' peer_get_user("max", "max_password")
#' }
peer_get_user <- function(username, password) {
    invisible(peer_get_user_r(username, password))
}

#' Send a message to a peer
#'
#' Sends a UDP message to a previously discovered peer. The peer must
#' have been registered via \code{\link{peer_get_user}} first.
#'
#' @param recipient Character string. The username of the recipient.
#' @param message Character string. The message to send (max 1024 bytes).
#' @return Invisible \code{NULL}. Raises an error if the peer is not found
#'   or the send fails.
#' @export
#' @examples
#' \dontrun{
#' peer_send_message("max", "skibidi!")
#' }
peer_send_message <- function(recipient, message) {
    invisible(peer_send_message_r(recipient, message))
}

#' Ping a peer
#'
#' Sends a ping packet to a previously discovered peer, works also on server.
#'
#' @param recipient Character string. The username of the peer to ping.
#' @return Invisible \code{NULL}. Raises an error if the peer is not found
#'   or the send fails.
#' @export
#' @examples
#' \dontrun{
#' peer_send_ping("max")
#' }
peer_send_ping <- function(recipient) {
    invisible(peer_send_ping_r(recipient))
}

#' Receive an incoming packet
#'
#' Waits for an incoming UDP packet with the given timeout. Automatically
#' processes the packet (e.g., stores newly discovered peers, prints
#' received messages).
#'
#' @param timeout_ms Integer. Maximum time to wait in milliseconds.
#'   Use 0 for non-blocking. Default is 100.
#' @return Integer. The packet type on success (\code{>=0}), or \code{-1}
#'   on timeout or error.
#' @export
#' @examples
#' \dontrun{
#' packet_type <- peer_receive(100)
#' }
peer_receive <- function(timeout_ms = 100L) {
    peer_receive_r(as.integer(timeout_ms))
}

#' Disconnect from PeerChat
#'
#' Closes the UDP socket and frees all internal state including the
#' peer hashmap. Call this when done using PeerChat.
#'
#' @return Invisible \code{NULL}.
#' @export
#' @examples
#' \dontrun{
#' peer_disconnect()
#' }
peer_disconnect <- function() {
    invisible(peer_disconnect_r())
}

#' Get the socket file descriptor
#'
#' Returns the internal UDP socket file descriptor. Intended for advanced
#' use cases such as custom select loops.
#'
#' @return Integer. The socket file descriptor, or \code{-1} if not connected.
#' @export
#' @examples
#' \dontrun{
#' fd <- peer_get_fd()
#' }
peer_get_fd <- function() {
    peer_get_fd_r()
}
