all: s c
	
s: 
	gcc UnixDomainSocket_Server.c -o unixdomainsocket_server -pthread

c:
	gcc UnixDomainSocket_Client.c -o unixdomainsocket_client -pthread

clean:
	@rm -f unixdomainsocket_client
	@rm -f unixdomainsocket_server