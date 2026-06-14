import PeerChat


def test_get_fd_before_connect():
    PeerChat.disconnect()
    assert PeerChat.get_fd() == -1


def test_connect_and_disconnect():
    PeerChat.connect("py_test_user_1", "pass1")
    fd = PeerChat.get_fd()
    assert fd > 0

    # consume INIT_RESPONSE
    PeerChat.receive(500)

    PeerChat.disconnect()
    assert PeerChat.get_fd() == -1


def test_receive_timeout_returns_minus_one():
    PeerChat.connect("py_test_timeout", "pass_timeout")
    PeerChat.receive(500)  # consume INIT_RESPONSE

    result = PeerChat.receive(100)
    assert result == -1

    PeerChat.disconnect()


def test_send_message_unknown_peer_raises():
    PeerChat.connect("py_test_err", "pass_err")
    PeerChat.receive(500)

    try:
        PeerChat.send_message("nonexistent_peer", "hello")
        assert False, "Expected RuntimeError"
    except RuntimeError:
        pass

    PeerChat.disconnect()


def test_send_ping_unknown_peer_raises():
    PeerChat.connect("py_test_ping_err", "pass_ping_err")
    PeerChat.receive(500)

    try:
        PeerChat.send_ping("nonexistent_peer")
        assert False, "Expected RuntimeError"
    except RuntimeError:
        pass

    PeerChat.disconnect()
