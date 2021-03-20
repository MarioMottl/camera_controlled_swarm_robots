#include "packet.h"
#include <cstring>
#include <new>

using namespace Schwarm;

Packet::Packet(void)
{
    this->__data = nullptr;
    this->data_size = 0;
}

Packet::Packet(const Packet& other)
{
    *this = other;
}

Packet::Packet(Packet&& other)
{
    *this = other;
}

Packet::~Packet(void)
{
    this->free();
}

void Packet::allocate(size_t s)
{
    if(s >= Packet::min_size())
    {
        if(this->__data != nullptr)
            this->free();
    
        this->__data = new uint8_t[s];
        if(this->__data == nullptr)
            throw std::bad_alloc();
        this->data_size = s;
    }
}

void Packet::free(void)
{
    if(this->__data != nullptr)
    {
        delete(this->__data);
        this->__data = nullptr;
        this->data_size = 0;
    }
}

packet_error Packet::set(uint8_t* __data, size_t custom_size)
{
    if(this->__data == nullptr || __data == nullptr)
        return packet_error::PACKET_NULL;

    if(*Packet::id_ptr(__data) != this->id())
        return packet_error::PACKET_INVALID_ID;
    
    memcpy(this->__data, __data, (custom_size >= Packet::min_size()) ? custom_size : this->data_size);
    return packet_error::PACKET_NONE;
}

uint8_t* Packet::internal_data_ptr(void)
{
    if(this->__data == nullptr)
        return nullptr;

    return this->__data + SIZE_ID + SIZE_PACKET_LENGTH;
}

packet_error Packet::internal_encode(void)
{
    if(this->__data == nullptr)
        return packet_error::PACKET_NULL;

    *(this->__data + 0) = this->id();
    *((size_t*)(this->__data + SIZE_ID)) = this->data_size;
    return packet_error::PACKET_NONE;
}

packet_error Packet::internal_decode(void)
{
    return (this->__data == nullptr) ? packet_error::PACKET_NULL : packet_error::PACKET_NONE;
}

size_t Packet::size(void) const noexcept
{
    return this->data_size;
}

const uint8_t* Packet::data(void) const
{
    if(this->__data == nullptr)
        return nullptr;

    return this->__data + SIZE_ID + SIZE_PACKET_LENGTH;
}

const uint8_t* Packet::rawdata(void) const
{
    return this->__data;
}

const uint8_t* Packet::id_ptr(const uint8_t* data)
{
    if(data == nullptr)
        return nullptr;
    
    return data;
}

const size_t* Packet::size_ptr(const uint8_t* data)
{  
    if(data == nullptr)
        return nullptr;

    return (size_t*)(data + SIZE_ID);
}

const uint8_t* Packet::data_ptr(const uint8_t* data)
{
    if(data == nullptr)
        return nullptr;

    return data + SIZE_ID + SIZE_PACKET_LENGTH;
}

const char* Packet::strerror(packet_error error) noexcept
{
    switch(error)
    {
    case packet_error::PACKET_NONE:
        return "No error occured.\n";
    case packet_error::PACKET_INVALID_ID:
        return "Packed received an invalid id.\n";
    case packet_error::PACKET_NULL:
        return "Packet data (own or parameter) is NULL.\n";
    case packet_error::PACKET_INVALID_GOAL:
        return "Received invalid index for goal.\n";
    case packet_error::PACKET_FAILED_GENERATING_PATH:
        return "Failed to generate path.\n";
    case packet_error::PACKET_SERVER_BUSY:
        return "Server is busy.\n";
    };
    return "Unknown error.";
}


Packet& Packet::operator=(const Packet& other)
{
    this->data_size = other.data_size;
    this->allocate(other.data_size);
    memcpy(this->__data, other.__data, other.data_size);
    return *this;
}

Packet& Packet::operator=(Packet&& other)
{
    this->data_size = other.data_size;
    this->allocate(other.data_size);
    memcpy(this->__data, other.__data, other.data_size);

    other.data_size = 0;
    other.free();
    return *this;
}