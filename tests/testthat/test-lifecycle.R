test_that("peer_get_fd returns -1 before connect", {
  skip_if_not(server_is_running(), "test server not available")
  # Ensure clean state
  peer_disconnect()
  expect_equal(peer_get_fd(), -1L)
})

test_that("peer_connect and peer_disconnect work", {
  skip_if_not(server_is_running(), "test server not available")

  expect_no_error(peer_connect("test_user_1", "pass1"))

  fd <- peer_get_fd()
  expect_true(fd > 0)

  # Wait for INIT_RESPONSE
  peer_receive(500L)

  peer_disconnect()
  expect_equal(peer_get_fd(), -1L)
})

test_that("peer_receive returns -1 on timeout with no data", {
  skip_if_not(server_is_running(), "test server not available")

  peer_connect("test_user_timeout", "pass_timeout")
  peer_receive(500L)  # consume INIT_RESPONSE

  result <- peer_receive(100L)
  expect_equal(result, -1L)

  peer_disconnect()
})

test_that("peer_send_message errors for unknown peer", {
  skip_if_not(server_is_running(), "test server not available")

  peer_connect("test_user_err", "pass_err")
  peer_receive(500L)

  expect_error(peer_send_message("nonexistent_peer", "hello"))

  peer_disconnect()
})

test_that("peer_send_ping errors for unknown peer", {
  skip_if_not(server_is_running(), "test server not available")

  peer_connect("test_user_ping_err", "pass_ping_err")
  peer_receive(500L)

  expect_error(peer_send_ping("nonexistent_peer"))

  peer_disconnect()
})
