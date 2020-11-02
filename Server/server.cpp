/*
Supported message types and what information is expected

Client_Message: Join
int8: Message type

Server_Message: Join_Result
int8: Message type
int8: 1 or 0 (success or fail)
int8: ID to be used when sending other message from client

Client_Message: leave
int8: Message type
int8: ID

Client_Message: Input
int8: Message type
int8: ID
int8: Input (up, down, right and/or left)

Server_Message: State
int8: Message type
For each client
int8: ClientId
float32: Position X
float32: Position Y
int16: Rotation

*/

/*
TODO: 

- Implement max speed
- Implement
- Fake lag
- Prediction and correction support for objects (and on client)
- Make this an ECS

- Server authorative asteroids
- Collission
- Bullets
- Health


*/


#define _USE_MATH_DEFINES
#include <math.h>

#include "common.cpp"
#include "common_net.cpp"
#include "server_net.cpp"
#include "object_component.cpp"
#include "object_system.cpp"

constexpr uint16 c_turn_speed = 5;	// how fast player turns
constexpr float32 c_max_speed = 50.f; //TODO: Implement so that we have a max speed
constexpr float32 c_client_timeout = 60.f;


struct Player_Input
{
	bool32 thurst, rotateLeft, rotateRight;
};


void main()
{
	printf("Server started\n");

	if (!Net::init())
	{
		printf("Net::init failed\n");
		return;
	}

	Net::Socket sock;
	if (!Net::socket_create(&sock))
	{
		printf("socket_create failed\n");
		return;
	}

	Net::IP_Endpoint local_endpoint = {};
	local_endpoint.address = INADDR_ANY;
	local_endpoint.port = c_port;
	if (!Net::socket_bind(&sock, &local_endpoint))
	{
		printf("socket_bind failed");
		return;
	}

	Timing_Info timing_info = timing_info_create();

	uint8 buffer[c_socket_buffer_size];
	Net::IP_Endpoint client_endpoints[c_max_clients];
	float32 time_since_heard_from_clients[c_max_clients];
	objectComponent client_objects[c_max_clients];
	Player_Input client_inputs[c_max_clients];

	for (uint16 i = 0; i < c_max_clients; ++i)
	{
		client_endpoints[i] = {};
	}

	bool32 is_running = 1;
	while (is_running)
	{
		LARGE_INTEGER tick_start_time;
		QueryPerformanceCounter(&tick_start_time);

		// read all available packets
		Net::IP_Endpoint from;
		uint32 bytes_received;
		while (Net::socket_receive(&sock, buffer, c_socket_buffer_size, &from, &bytes_received))
		{

			//Check type of message, first byte describes that
			switch (buffer[0])
			{
			case Client_Message::Join:
			{
				printf("Client joining...");

				//Check if there are any empty slots
				uint8 slot = (uint8)-1;
				for (uint8 i = 0; i < c_max_clients; ++i)
				{
					if (client_endpoints[i].address == 0)
					{
						slot = i;
						break;
					}
				}

				buffer[0] = (int8)Server_Message::Join_Result;

				//If available slot assign and reply back
				if (slot != (uint8)-1)
				{
					printf("client slot = %hu\n", slot);

					buffer[0] = Server_Message::Join_Result;
					
					//Set second byte to 1 for indicating success 
					buffer[1] = 1;

					//Set third byte to the client ID, so that the client can use that when sending input message
					memcpy(&buffer[2], &slot, 2);

					if (Net::socket_send(&sock, buffer, c_socket_buffer_size, &from))
					{
						objectComponent newObject;

						//Set random rotation and add velocity, only so that I can test with many clients!
						newObject.rotation = rand() % 360;
						newObject.velocityX += (sin(((float32)newObject.rotation / 180 * M_PI))) ;
						newObject.velocityY += (-cos(((float32)newObject.rotation / 180 * M_PI))) ;


						client_endpoints[slot] = from;
						time_since_heard_from_clients[slot] = 0.0f;
						client_objects[slot] = newObject;
						client_inputs[slot] = {};
					}
					else
					{
						printf("sendto failed: %d\n", WSAGetLastError());
					}
				}
				else
				{
					printf("could not find a slot for player\n");
					buffer[1] = 0;

					if (!Net::socket_send(&sock, buffer, 2, &from))
					{
						printf("sendto failed: %d\n", WSAGetLastError());
					}
				}

				

			}
			break;

			case Client_Message::Leave:
			{
				uint8 slot;
				memcpy(&slot, &buffer[1], 2);

				if (Net::ip_endpoint_equal(&client_endpoints[slot], &from))
				{
					client_endpoints[slot] = {};
					printf("Client_Message::Leave from %hu(%u:%hu)\n",
						slot, from.address, from.port);
				}
				else
				{
					printf("Client_Message::Leave from %hu(%u:%hu), espected (%u:%hu)",
						slot, from.address, from.port,
						client_endpoints[slot].address, client_endpoints[slot].port);
				}
			}
			break;

			case Client_Message::Input:
			{
				uint8 clientId;
				memcpy(&clientId, &buffer[1], sizeof(&buffer[1]));

				

				//Check so that the ID matches where it comes from
				if (Net::ip_endpoint_equal(&client_endpoints[clientId], &from))
				{
					uint8 input = buffer[2];

					client_inputs[clientId].thurst = input & 0x1;
					client_inputs[clientId].rotateLeft = input & 0x2;
					client_inputs[clientId].rotateRight = input & 0x4;

					time_since_heard_from_clients[clientId] = 0.0f;

				}
				else
				{
					printf("Client address not matching i_Message::Input discarded, was from %u:%hu but expected %u:%hu\n", from.address, from.port, client_endpoints[clientId].address, client_endpoints[clientId].port);
				}
			}
			break;
			}
		}

		// process input and update state
		for (uint8 i = 0; i < c_max_clients; ++i)
		{

			if (client_endpoints[i].address)
			{
				if (client_inputs[i].thurst)
				{
					//Increase velocity towards current rotation
					client_objects[i].velocityX  += (sin(((float32)client_objects[i].rotation / 180 * M_PI))) / 10;
					client_objects[i].velocityY += (-cos(((float32)client_objects[i].rotation / 180 * M_PI))) / 10;
				}
				
				if (client_inputs[i].rotateLeft)
				{
					//Ensure correct degrees
					if (client_objects[i].rotation == 0)
						client_objects[i].rotation = 360 - c_turn_speed;
					else
						client_objects[i].rotation -= c_turn_speed;
				
				}
				if (client_inputs[i].rotateRight)
				{
					//Ensure correct degrees
					if(client_objects[i].rotation == 360)
						client_objects[i].rotation = c_turn_speed;
					else
						client_objects[i].rotation += c_turn_speed;
					
				}

				time_since_heard_from_clients[i] += c_seconds_per_tick;
				if (time_since_heard_from_clients[i] > c_client_timeout)
				{
					printf("client %hu timed out\n", i);
					client_endpoints[i] = {};
				}

				
				//Updatet this objects position
				updateServerObjectPosition(client_objects[i]);

			}
		}

		// create state packet (1 byte)
		buffer[0] = (int8)Server_Message::State;
		int32 bytes_written = 1;

		for (uint8 i = 0; i < c_max_clients; ++i)
		{
			if (client_endpoints[i].address)
			{

				//Set client ID (1 byte)
				memcpy(&buffer[bytes_written], &i, sizeof(i));
				bytes_written += sizeof(i);

				//Set client X position (4 byte)
				memcpy(&buffer[bytes_written], &client_objects[i].positionX, sizeof(client_objects[i].positionX));
				bytes_written += sizeof(client_objects[i].positionX);

				//Set client Y position (4 byte)
				memcpy(&buffer[bytes_written], &client_objects[i].positionY, sizeof(client_objects[i].positionY));
				bytes_written += sizeof(client_objects[i].positionY);

				//Set client rotation (2 byte)
				memcpy(&buffer[bytes_written], &client_objects[i].rotation, sizeof(client_objects[i].rotation));
				bytes_written += sizeof(client_objects[i].rotation);

			}

			//Reset client inputs once per frame
			client_inputs[i] = {};

		}

		// send back to clients
		for (uint8 i = 0; i < c_max_clients; ++i)
		{
			if (client_endpoints[i].address)
			{
				if (!Net::socket_send(&sock, buffer, bytes_written, &client_endpoints[i]))
				{
					printf("sendto failed: %d\n", WSAGetLastError());
				}
			}
		}
		
		wait_for_tick_end(tick_start_time, &timing_info);
	}
}