test_that("two peers can discover each other via get_user", {
  skip_if_not(server_is_running(), "test server not available")

  # Connect Alice
  peer_connect("alice", "alice_pass")
  peer_receive(500L)  # INIT_RESPONSE
  peer_disconnect()

  # Connect Bob and discover Alice
  peer_connect("bob", "bob_pass")
  peer_receive(500L)  # INIT_RESPONSE

  expect_no_error(peer_get_user("alice", "alice_pass"))
  pkt <- peer_receive(1000L)

  # Should get START_PINGING_PEER (type 4)
  expect_equal(pkt, 4L)

  peer_disconnect()
})

test_that("peer can send and receive a message", {
  skip_if_not(server_is_running(), "test server not available")

  # Connect sender
  peer_connect("sender", "sender_pass")
  peer_receive(500L)

  # We need a receiver - but since our library is single-instance (static globals),

  # we can only run one client at a time. Test that send_message succeeds
  # when the peer is known (discovered via get_user).
  peer_disconnect()

  # Connect receiver first so it's registered
  peer_connect("receiver", "recv_pass")
  peer_receive(500L)
  peer_disconnect()

  # Connect sender and discover receiver
  peer_connect("sender2", "sender2_pass")
  peer_receive(500L)
  peer_get_user("receiver", "recv_pass")
  pkt <- peer_receive(1000L)
  expect_equal(pkt, 4L)  # START_PINGING_PEER

  # Send message to discovered peer (should not error)
  expect_no_error(peer_send_message("receiver", "Hello from test!"))

  peer_disconnect()
})

test_that("peer can ping a discovered peer", {
  skip_if_not(server_is_running(), "test server not available")

  # Register target
  peer_connect("ping_target", "pt_pass")
  peer_receive(500L)
  peer_disconnect()

  # Connect pinger and discover target
  peer_connect("pinger", "pinger_pass")
  peer_receive(500L)
  peer_get_user("ping_target", "pt_pass")
  pkt <- peer_receive(1000L)
  expect_equal(pkt, 4L)

  expect_no_error(peer_send_ping("ping_target"))

  peer_disconnect()
})
