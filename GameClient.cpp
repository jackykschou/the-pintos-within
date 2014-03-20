GameClient::GameClient(char *host) {
	_host = malloc(strlen(host));
	strcpy(_host, host);
	state = GameClientReady;
}

int GameClient::connect(int port) {
	if (state != GameClientReady) return -1;

	state = GameClientConnecting;

	/* Resolve server name  */
	if (SDLNet_ResolveHost(&srvadd, _host, port) == -1)
	{
		fprintf(stderr, "SDLNet_ResolveHost(%s %d): %s\n", argv[1], atoi(argv[2]), SDLNet_GetError());
		return -1;
	}

	/* Open a socket on random port */
	if (!(_socket = SDLNet_UDP_Open(0)))
	{
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	return 0;
}

int GameClient::joinGame() {
	UDPpacket *p;
	if (!(p = SDLNet_AllocPacket(5))) {
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		return -1;
	}

	p->address.host = srvadd.host;	/* Set the destination host */
	p->address.port = srvadd.port;	/* And destination port */
	p->data = "j";
	p->len = strlen((char *)p->data) + 1;
	SDLNet_UDP_Send(_socket, -1, p); /* This sets the p->channel */

	SDLNet_FreePacket(p);

	state = GameClientRunning;

	return 0;
}

// let's see what packets the server has sent us
int GameClient::consumePackets() {
	UDPpacket *p;

	if (!(p = SDLNet_AllocPacket(10))) {
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		return -1;
	}

	while (SDLNet_UDP_Recv(_socket, p)) {
		processPacket(p);
	}

	SDLNet_FreePacket(p);
	return 0;
}

int GameClient::processPacket(UDPpacket *p) {
	// update the game state based on contents of p
}