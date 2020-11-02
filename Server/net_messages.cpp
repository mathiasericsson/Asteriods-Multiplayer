#include "common.cpp"

static void deserialise_u8(uint8** buffer, uint8* u)
{
	*u = **buffer;
	++(*buffer);
}

static void deserialise_u32(uint8** buffer, uint32* u)
{
	memcpy(u, *buffer, sizeof(*u));
	*buffer += sizeof(*u);
}

static void deserialise_f32(uint8** buffer, float32* f)
{
	memcpy(f, *buffer, sizeof(*f));
	*buffer += sizeof(*f);
}

static void deserialise_u16(uint8** buffer, uint16* u)
{
	memcpy(u, *buffer, sizeof(*u));
	*buffer += sizeof(*u);
}
