import PeerChat


def test_discover_peer_via_get_user():
    # Register alice
    PeerChat.connect("py_alice", "alice_pass")
    PeerChat.receive(500)  # INIT_RESPONSE
    PeerChat.disconnect()

    # Connect bob and discover alice
    PeerChat.connect("py_bob", "bob_pass")
    PeerChat.receive(500)  # INIT_RESPONSE

    PeerChat.get_user("py_alice", "alice_pass")
    pkt = PeerChat.receive(1000)

    # START_PINGING_PEER = 4
    assert pkt == 4

    PeerChat.disconnect()


def test_send_message_to_discovered_peer():
    # Register receiver
    PeerChat.connect("py_receiver", "recv_pass")
    PeerChat.receive(500)
    PeerChat.disconnect()

    # Connect sender and discover receiver
    PeerChat.connect("py_sender", "sender_pass")
    PeerChat.receive(500)
    PeerChat.get_user("py_receiver", "recv_pass")
    pkt = PeerChat.receive(1000)
    assert pkt == 4

    # Should not raise
    PeerChat.send_message("py_receiver", "Hello from pytest!")

    PeerChat.disconnect()


def test_ping_discovered_peer():
    # Register target
    PeerChat.connect("py_ping_target", "pt_pass")
    PeerChat.receive(500)
    PeerChat.disconnect()

    # Connect pinger and discover target
    PeerChat.connect("py_pinger", "pinger_pass")
    PeerChat.receive(500)
    PeerChat.get_user("py_ping_target", "pt_pass")
    pkt = PeerChat.receive(1000)
    assert pkt == 4

    # Should not raise
    PeerChat.send_ping("py_ping_target")

    PeerChat.disconnect()
