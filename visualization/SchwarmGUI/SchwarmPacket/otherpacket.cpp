#define _CRT_SECURE_NO_WARNINGS
#include "packet.h"
#include <cstring>
#include <algorithm>

using namespace Schwarm;

/* EXIT PACKET */
ExitPacket::ExitPacket(const ExitPacket& other)
{
    *this = other;
}

ExitPacket::ExitPacket(ExitPacket&& other)
{
    *this = other;
}

packet_error ExitPacket::encode(void)
{
    return this->internal_encode();
}

packet_error ExitPacket::decode(void)
{
    return this->internal_decode();
}

ExitPacket& ExitPacket::operator=(const ExitPacket& other)
{
    Packet::operator=(other);
    return *this;
}

ExitPacket& ExitPacket::operator=(ExitPacket&& other)
{
    Packet::operator=(other);
    return *this;
}

/* ACNOLEDGE PACKET */

AcnPacket::AcnPacket(const AcnPacket& other)
{
    *this = other;
}

AcnPacket::AcnPacket(AcnPacket&& other)
{
    *this = other;
}

packet_error AcnPacket::encode(void)
{
    return this->internal_encode();
}

packet_error AcnPacket::decode(void)
{
    return this->internal_decode();
}

AcnPacket& AcnPacket::operator=(const AcnPacket& other)
{
    Packet::operator=(other);
    return *this;
}

AcnPacket& AcnPacket::operator=(AcnPacket&& other)
{
    Packet::operator=(other);
    return *this;
}

/* ERROR PACKET */

ErrorPacket::ErrorPacket(void)
{
    this->error_code = packet_error::PACKET_NONE;
}

ErrorPacket::ErrorPacket(const ErrorPacket& other)
{
    *this = other;
}

ErrorPacket::ErrorPacket(ErrorPacket&& other)
{
    *this = other;
}

packet_error ErrorPacket::encode(void)
{
    packet_error err = this->internal_encode();
    uint8_t* dataptr = this->internal_data_ptr();
    *((packet_error*)dataptr /* +0 */) = this->error_code;
    return err;
}

packet_error ErrorPacket::decode(void)
{
    packet_error err = this->internal_decode();
    uint8_t* dataptr = this->internal_data_ptr();
    this->error_code = *((packet_error*)dataptr /* +0 */);
    return err;
}

void ErrorPacket::set_code(packet_error err) noexcept
{
    this->error_code = err;
}

packet_error ErrorPacket::get_code(void) const noexcept
{
    return this->error_code;
}

ErrorPacket& ErrorPacket::operator=(const ErrorPacket& other)
{
    Packet::operator=(other);
    this->error_code = other.error_code;
    return *this;
}

ErrorPacket& ErrorPacket::operator=(ErrorPacket&& other)
{
    Packet::operator=(other);
    this->error_code = other.error_code;
    other.error_code = packet_error::PACKET_NONE;
    return *this;
}

/* PATH GENERATE PACKET */
PathGeneratePacket::PathGeneratePacket(void)
{
    this->num_goals = 0;
    this->filepath = nullptr;
    this->fp_allocsize = 0;
    this->vehicle_id = 0;
    this->invert = false;
}

PathGeneratePacket::PathGeneratePacket(const PathGeneratePacket& other)
{
    *this = other;
}

PathGeneratePacket::PathGeneratePacket(PathGeneratePacket&& other)
{
    *this = other;
}

PathGeneratePacket::~PathGeneratePacket(void)
{
    this->free_fp();
}

void PathGeneratePacket::alloc_fp(uint32_t s)
{
    if(this->filepath == nullptr)
    {
        this->filepath = new char[s];
        this->fp_allocsize = s;
    }
}

void PathGeneratePacket::free_fp(void)
{
    if(this->filepath != nullptr)
    {
        delete(this->filepath);
        this->filepath = nullptr;
        this->fp_allocsize = 0;
    }
}

packet_error PathGeneratePacket::encode(void)
{
    packet_error err = this->internal_encode();
    if(err == packet_error::PACKET_NONE)
    {
        uint8_t* const dataptr = this->internal_data_ptr();
        const uint32_t remaining_size = this->size() - this->min_size();
        const uint32_t fp_size = this->filepath_size();
        *((unsigned int*)(dataptr /* +0 */)) = this->num_goals;
        *((int*)(dataptr + SIZE_NUM_GOALS)) = this->vehicle_id;
        *((bool*)(dataptr + SIZE_NUM_GOALS + SIZE_VEHICLE_ID)) = this->invert;
        memcpy((char*)(dataptr + SIZE_NUM_GOALS + SIZE_VEHICLE_ID + SIZE_INVERT), this->filepath, (remaining_size < fp_size) ? ((remaining_size == 0) ? 0 : remaining_size - 1) : fp_size);
        if(remaining_size < fp_size && remaining_size > 0)
            *((char*)(dataptr + SIZE_NUM_GOALS + SIZE_INVERT + remaining_size - 1)) = '\0';
    }
    return err;
}

packet_error PathGeneratePacket::decode(void)
{
    packet_error err = this->internal_decode();
    if(err == packet_error::PACKET_NONE)
    {
        uint8_t* dataptr = this->internal_data_ptr();
        uint32_t remaining_size = this->size() - this->min_size();
        this->num_goals = *((unsigned int*)(dataptr /* +0 */));
        this->vehicle_id = *((int*)(dataptr + SIZE_NUM_GOALS));
        this->invert = *((bool*)(dataptr + SIZE_NUM_GOALS + SIZE_VEHICLE_ID));
        this->free_fp();
        this->alloc_fp(remaining_size);
        memcpy(this->filepath, (char*)(dataptr + SIZE_NUM_GOALS + SIZE_VEHICLE_ID + SIZE_INVERT), remaining_size);
    }
    return err;
}

void PathGeneratePacket::set_num_goals(unsigned int n) noexcept
{
    this->num_goals = n;
}

unsigned int PathGeneratePacket::get_num_goals(void) const noexcept
{
    return this->num_goals;
}

void PathGeneratePacket::set_filepath(const char* path) noexcept
{
    const uint32_t s_path = strlen(path);
    if(s_path >= this->fp_allocsize)
    {
        this->free_fp();
        this->alloc_fp(s_path + 1);
    }
    strcpy(this->filepath, path);
}

const char* PathGeneratePacket::get_filepath(void) const noexcept
{
    return this->filepath;
}

uint32_t PathGeneratePacket::filepath_size(void) const noexcept
{
    return strlen(this->filepath) + 1;   // +1 for \0
}

void PathGeneratePacket::set_vehicle_id(int id) noexcept
{
    this->vehicle_id = id;
}

int PathGeneratePacket::get_vehicle_id(void) const noexcept
{
    return this->vehicle_id;
}

bool& PathGeneratePacket::should_invert(void) noexcept
{
    return this->invert;
}

bool PathGeneratePacket::should_invert(void) const noexcept
{
    return this->invert;
}

PathGeneratePacket& PathGeneratePacket::operator=(const PathGeneratePacket& other)
{
    Packet::operator=(other);
    this->num_goals = other.num_goals;
    this->vehicle_id = other.vehicle_id;
    this->invert = other.invert;
    this->fp_allocsize = other.fp_allocsize;
    this->alloc_fp(other.fp_allocsize);
    memcpy(this->filepath, other.filepath, other.fp_allocsize);
    return *this;
}

PathGeneratePacket& PathGeneratePacket::operator=(PathGeneratePacket&& other)
{
    Packet::operator=(other);
    this->num_goals = other.num_goals;
    other.num_goals = 0;

    this->vehicle_id = other.vehicle_id;
    other.vehicle_id = 0;

    this->invert = other.invert;
    other.invert = false;

    this->fp_allocsize = other.fp_allocsize;
    this->alloc_fp(other.fp_allocsize);
    memcpy(this->filepath, other.filepath, other.fp_allocsize);
    other.fp_allocsize = 0;
    other.free_fp();
    
    return *this;
}

/* GOAL REQUEST PACKET */
GoalReqPacket::GoalReqPacket(void)
{
    this->goal_idx = 0;
    this->vehicle_id = 0;
}

GoalReqPacket::GoalReqPacket(const GoalReqPacket& other)
{
    *this = other;
}

GoalReqPacket::GoalReqPacket(GoalReqPacket&& other)
{
    *this = other;
}

packet_error GoalReqPacket::encode(void)
{
    packet_error err = this->internal_encode();
    if(err == packet_error::PACKET_NONE)
    {
        uint8_t* dataptr = this->internal_data_ptr();
        *((unsigned int*)(dataptr /* +0 */)) = this->goal_idx;
        *((int*)(dataptr + SIZE_GOAL_IDX)) = this->vehicle_id;
    }
    return err;
}

packet_error GoalReqPacket::decode(void)
{
    packet_error err = this->internal_decode();
    if(err == packet_error::PACKET_NONE)
    {
        uint8_t* dataptr = this->internal_data_ptr();
        this->goal_idx = *((unsigned int*)(dataptr));
        this->vehicle_id = *((int*)(dataptr + SIZE_GOAL_IDX));
    }
    return err;
}

void GoalReqPacket::set_goal_index(uint32_t i) noexcept
{
    this->goal_idx = i;
}

uint32_t GoalReqPacket::get_goal_index(void) const noexcept
{
    return this->goal_idx;
}

void GoalReqPacket::set_vehicle_id(int id) noexcept
{
    this->vehicle_id = id;
}

int GoalReqPacket::get_vehicle_id(void) const noexcept
{
    return this->vehicle_id;
}

GoalReqPacket& GoalReqPacket::operator=(const GoalReqPacket& other)
{
    Packet::operator=(other);
    this->goal_idx = other.goal_idx;
    this->vehicle_id = other.vehicle_id;
    return *this;
}

GoalReqPacket& GoalReqPacket::operator=(GoalReqPacket&& other)
{
    Packet::operator=(other);
    this->goal_idx = other.goal_idx;
    other.goal_idx = 0;

    this->vehicle_id = other.vehicle_id;
    other.vehicle_id = 0;

    return *this;
}

/* GOAL PACKET */
GoalPacket::GoalPacket(void)
{
    this->goal_x = 0;
    this->goal_y = 0;
    this->vehicle_id = 0;
}

GoalPacket::GoalPacket(const GoalPacket& other)
{
    *this = other;
}

GoalPacket::GoalPacket(GoalPacket&& other)
{
    *this = other;
}

packet_error GoalPacket::encode(void)
{
    packet_error err = this->internal_encode();
    if(err == packet_error::PACKET_NONE)
    {
        uint8_t* dataptr = this->internal_data_ptr();
        *((float*)(dataptr /* +0 */)) = this->goal_x;
        *((float*)(dataptr + sizeof(float))) = this->goal_y;
        *((int*)(dataptr + SIZE_GOAL)) = this->vehicle_id;
    }
    return err;
}

packet_error GoalPacket::decode(void)
{
    packet_error err = this->internal_decode();
    if(err == packet_error::PACKET_NONE)
    {
        uint8_t* dataptr = this->internal_data_ptr();
        this->goal_x = *((float*)(dataptr /* +0 */));
        this->goal_y = *((float*)(dataptr + sizeof(float)));
        this->vehicle_id = *((int*)(dataptr + SIZE_GOAL));
    }
    return err;
}

void GoalPacket::set_goal(float x, float y) noexcept
{
    this->goal_x = x;
    this->goal_y = y;
}

float GoalPacket::get_goal_x(void) const noexcept
{
    return this->goal_x;
}

float GoalPacket::get_goal_y(void) const noexcept
{
    return this->goal_y;
}

void GoalPacket::set_vehicle_id(int id) noexcept
{
    this->vehicle_id = id;
}

int GoalPacket::get_vehicle_id(void) const noexcept
{
    return this->vehicle_id;
}

GoalPacket& GoalPacket::operator=(const GoalPacket& other)
{
    Packet::operator=(other);
    this->goal_x = other.goal_x;
    this->goal_y = other.goal_y;
    this->vehicle_id = other.vehicle_id;
    return *this;
}

GoalPacket& GoalPacket::operator=(GoalPacket&& other)
{
    Packet::operator=(other);
    this->goal_x = other.goal_x;
    other.goal_x = 0;

    this->goal_y = other.goal_y;
    other.goal_y = 0;

    this->vehicle_id = other.vehicle_id;
    other.vehicle_id = 0;

    return *this;
}

/* VEHICLE COMMAND PACKET */

VehicleCommandPacket::VehicleCommandPacket(void)
{
    this->vehicle_id = 0;
    this->angle = 0.0f;
    this->length = 0.0f;
}

VehicleCommandPacket::VehicleCommandPacket(const GoalPacket& other)
{
    *this = other;
}

VehicleCommandPacket::VehicleCommandPacket(GoalPacket&& other)
{
    *this = std::move(other);
}

packet_error VehicleCommandPacket::encode(void)
{
    packet_error err = internal_encode();
    if (err == packet_error::PACKET_NONE)
    {
        uint8_t* data = internal_data_ptr();
        *((uint32_t*)(data))                                = this->vehicle_id;
        *((float*)(data + SIZE_VEHICLE_ID))                 = this->angle;
        *((float*)(data + SIZE_VEHICLE_ID + SIZE_ANGLE))    = this->length;
    }
    return err;
}

packet_error VehicleCommandPacket::decode(void)
{
    packet_error err = internal_decode();
    if (err == packet_error::PACKET_NONE)
    {
        uint8_t* data = internal_data_ptr();
        this->vehicle_id    = *((uint32_t*)(data));
        this->angle         = *((float*)(data + SIZE_VEHICLE_ID));
        this->length        = *((float*)(data + SIZE_VEHICLE_ID + SIZE_ANGLE));
    }
    return err;
}

void VehicleCommandPacket::set_vehicle_id(uint32_t id) noexcept
{
    this->vehicle_id = id;
}

uint32_t VehicleCommandPacket::get_vehicle_id(void) const noexcept
{
    return this->vehicle_id;
}

void VehicleCommandPacket::set_angle(float angle) noexcept
{
    this->angle = angle;
}

float VehicleCommandPacket::get_angle(void) const noexcept
{
    return this->angle;
}

void VehicleCommandPacket::set_length(float length) noexcept
{
    this->length = length;
}

float VehicleCommandPacket::get_length(void) const noexcept
{
    return this->length;
}

VehicleCommandPacket& VehicleCommandPacket::operator=(const VehicleCommandPacket& other)
{
    Packet::operator=(other);
    this->vehicle_id = other.vehicle_id;
    this->angle = other.angle;
    this->length = other.length;
    return *this;
}

VehicleCommandPacket& VehicleCommandPacket::operator=(VehicleCommandPacket&& other)
{
    Packet::operator=(std::move(other));
    this->vehicle_id = other.vehicle_id;
    other.vehicle_id = 0;
    
    this->angle = other.angle;
    other.angle = 0.0f;

    this->length = other.length;
    other.length = 0.0f;
    return *this;
}