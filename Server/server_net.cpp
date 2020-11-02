#pragma once

#include "common_net.cpp"

namespace Net
{

	static bool ip_endpoint_equal(IP_Endpoint* a, IP_Endpoint* b)
	{
		return a->address == b->address && a->port == b->port;
	}

	static SOCKADDR_IN ip_endpoint_to_sockaddr_in(IP_Endpoint* ip_endpoint)
	{
		SOCKADDR_IN sockaddr_in;
		sockaddr_in.sin_family = AF_INET;
		sockaddr_in.sin_addr.s_addr = htonl(ip_endpoint->address);
		sockaddr_in.sin_port = htons(ip_endpoint->port);
		return sockaddr_in;
	}

	static bool socket_bind(Socket* sock, IP_Endpoint* local_endpoint)
	{
		SOCKADDR_IN local_address = ip_endpoint_to_sockaddr_in(local_endpoint);
		if (bind(sock->handle, (SOCKADDR*)&local_address, sizeof(local_address)) == SOCKET_ERROR)
		{
			printf("bind failed: %d\n", WSAGetLastError());
			return false;
		}

		return true;
	}


	static float32 time_since(LARGE_INTEGER t, LARGE_INTEGER frequency)
	{
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);

		return float32(now.QuadPart - t.QuadPart) / float32(frequency.QuadPart);
	}

} // namespace Net