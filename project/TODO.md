# TODO

1. research quic to use alongside udp (udp for nat traversal, when connected quic)
2. every 10sec ping synchronization server and peers
3. on startup pass nickname and pass
4. ask stun server who am I, or let synchronization server do it
5. synchronization server has hash table nicknames as keys, no duplicates, stores IP, port, last time seen, maybe public key
6. Query server for given nickname, pass it alongside password, if it is a match then return IP, port and notify other client as well that they need to start talking
7. If client haven't responded in 40 seconds remove him
8. Client only pings server, no need for response
9. Possibility of 2 passwords
	- secret to join public chanel
	- account pass
10. Maybe it will be easier to create groups instead of 1:1 chats, or add this feature
11. Make it secure - right now I can sniff it using wireshark

## items

1. initialize with client takes username, password and connects to server it can return error that the username is already taken, if not then I ping server every 10s and thats it.
2. server listens, based on request it adds user to its db, and responds either okay or invalid
3. then there is functionality: I want to talk with user x, they password y
	server then validates it and responds, sends to this user start talking with person that asked
4. From now on they start pinging each other every 10s
5. If one of the people disconnects then the messaging is stopped and after 40s its removed from server
6. No local database, everything is ephemeral, there are public keys stored in the central server to prevent MITM attacks but thats it
7. It should handle around 1M users simultaneously
