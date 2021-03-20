#ifndef __schwarm_packet_h__
#define __schwarm_packet_h__

#include <cstdint>

namespace Schwarm
{
    enum packet_error
    {
        PACKET_NONE,
        PACKET_INVALID_ID,
        PACKET_NULL,

        PACKET_FAILED_GENERATING_PATH,
        PACKET_INVALID_GOAL,
        PACKET_SERVER_BUSY
    };

    class Packet
    {
    private:
        uint8_t* __data;
        size_t data_size;

        void free(void);

    protected:
        uint8_t* internal_data_ptr(void);
        packet_error internal_encode(void);
        packet_error internal_decode(void);

    public:
        static constexpr size_t SIZE_ID            = sizeof(uint8_t);
        static constexpr size_t SIZE_PACKET_LENGTH = sizeof(size_t);

        Packet(void);
        Packet(const Packet&);
        Packet(Packet&&);
        virtual ~Packet(void);

        void allocate(size_t);
        packet_error set(uint8_t*, size_t = 0);

        virtual packet_error encode(void) = 0;
        virtual packet_error decode(void) = 0;

        virtual uint8_t id(void)        const noexcept = 0;
        size_t          size(void)      const noexcept;
        const uint8_t*  data(void)      const;
        const uint8_t*  rawdata(void)   const;
        virtual inline size_t min_size(void) const noexcept {return SIZE_ID + SIZE_PACKET_LENGTH;}

        static const uint8_t*   id_ptr(const uint8_t*);
        static const size_t*    size_ptr(const uint8_t*);
        static const uint8_t*   data_ptr(const uint8_t*);

        static const char* strerror(packet_error) noexcept;

        Packet& operator=(const Packet&);
        Packet& operator=(Packet&&);
    };

    /*  DATA STRUCTURE:
    *       id | length | (no extra data)
    *       1B | 4B     | 0B  
    */

    class ExitPacket : public Packet
    {
    public:
        static constexpr uint8_t PACKET_ID = 0;

        ExitPacket(void) = default;
        ExitPacket(const ExitPacket&);
        ExitPacket(ExitPacket&&);
        virtual ~ExitPacket(void) {/*dtor*/}

        virtual packet_error encode(void);
        virtual packet_error decode(void);

        virtual inline uint8_t id(void) const noexcept  {return PACKET_ID;}

        ExitPacket& operator=(const ExitPacket&);
        ExitPacket& operator=(ExitPacket&&);
    };

    /*  DATA STRUCTURE:
    *       id | length | (no extra data)
    *       1B | 4B     | 0B  
    */

    class AcnPacket : public Packet
    {
    public:
        static constexpr uint8_t PACKET_ID = 1;

        AcnPacket(void) = default;
        AcnPacket(const AcnPacket&);
        AcnPacket(AcnPacket&&);
        virtual ~AcnPacket(void) {/*dtor*/}

        virtual packet_error encode(void);
        virtual packet_error decode(void);

        virtual inline uint8_t id(void) const noexcept  {return PACKET_ID;}

        AcnPacket& operator=(const AcnPacket&);
        AcnPacket& operator=(AcnPacket&&);
    };

    /*  DATA STRUCTURE:
    *       id | length | error code
    *       1B | 4B     | 4B  
    */

    class ErrorPacket : public Packet
    {
    private:
        /*  POSSIBLE ERROR CODES THAT ARE TRANSMITTED:
        *       PACKET_NONE:                    NO ERROR
        *       PACKET_FAILED_GENERATING_PATH:  COULD NOT GENERATE PATH (FILE NOT FOUND)
        *       PACKET_INVALID_GOAL:            INVALID INDEX FOR GOAL
        *       PACKET_SERVER_BUSY:             SERVER IS BUSY GENERATING PATH
        */
        packet_error error_code;

    public:
        static constexpr uint8_t PACKET_ID = 2;
        static constexpr size_t ERROR_CODE_SIZE = sizeof(packet_error);

        ErrorPacket(void);
        ErrorPacket(const ErrorPacket&);
        ErrorPacket(ErrorPacket&&);
        virtual ~ErrorPacket(void) {/*dtor*/}

        virtual packet_error encode(void);
        virtual packet_error decode(void);

        virtual inline uint8_t id(void) const noexcept      {return PACKET_ID;}
        virtual inline size_t min_size(void) const noexcept {return SIZE_ID + SIZE_PACKET_LENGTH + ERROR_CODE_SIZE;}

        void            set_code(packet_error)  noexcept;
        packet_error    get_code(void)          const noexcept;

        ErrorPacket& operator=(const ErrorPacket&);
        ErrorPacket& operator=(ErrorPacket&&);
    };

    /*  DATA STRUCTURE:
    *       id | length | num goals | vehicle id | invert |filename / path
    *       1B | 4B     | 4B        | 4B         | 1B     | x bytes
    */

    class PathGeneratePacket : public Packet
    {
    private:
        unsigned int num_goals;
        int vehicle_id;
        bool invert;
        char* filepath;
        size_t fp_allocsize;

        void alloc_fp(size_t);
        void free_fp(void);

    public:
        static constexpr uint8_t PACKET_ID = 3;
        static constexpr size_t SIZE_NUM_GOALS = sizeof(unsigned int);
        static constexpr size_t SIZE_INVERT = sizeof(bool);
        static constexpr size_t SIZE_VEHICLE_ID = sizeof(int);

        PathGeneratePacket(void);
        PathGeneratePacket(const PathGeneratePacket&);
        PathGeneratePacket(PathGeneratePacket&&);
        virtual ~PathGeneratePacket(void);

        virtual packet_error encode(void);
        virtual packet_error decode(void);

        virtual inline uint8_t id(void) const noexcept      {return PACKET_ID;}
        virtual inline size_t min_size(void) const noexcept {return SIZE_ID + SIZE_PACKET_LENGTH + SIZE_NUM_GOALS + SIZE_VEHICLE_ID + SIZE_INVERT;}

        void            set_num_goals(unsigned int) noexcept;
        unsigned int    get_num_goals(void)         const noexcept;

        void set_vehicle_id(int)    noexcept;
        int  get_vehicle_id(void)   const noexcept;

        bool& should_invert(void)   noexcept;
        bool  should_invert(void)   const noexcept;

        void        set_filepath(const char*)   noexcept;
        const char* get_filepath(void)          const noexcept;
        size_t      filepath_size(void)         const noexcept;

        PathGeneratePacket& operator=(const PathGeneratePacket&);
        PathGeneratePacket& operator=(PathGeneratePacket&&);
    };

    /*  DATA STRUCTURE:
    *       id | length | goal index | vehicle id 
    *       1B | 4B     | 4B         | 4B
    */

    class GoalReqPacket : public Packet
    {
    private:
        unsigned int goal_idx;
        int vehicle_id;

    public:
        static constexpr uint8_t PACKET_ID = 4;
        static constexpr size_t SIZE_GOAL_IDX = sizeof(size_t);
        static constexpr size_t SIZE_VEHICLE_ID = sizeof(int);

        GoalReqPacket(void);
        GoalReqPacket(const GoalReqPacket&);
        GoalReqPacket(GoalReqPacket&&);
        virtual ~GoalReqPacket(void) {/*dtor*/}

        virtual packet_error encode(void);
        virtual packet_error decode(void);

        virtual inline uint8_t id(void) const noexcept      {return PACKET_ID;}
        virtual inline size_t min_size(void) const noexcept {return SIZE_ID + SIZE_PACKET_LENGTH + SIZE_GOAL_IDX + SIZE_VEHICLE_ID;}

        void    set_goal_index(size_t)  noexcept;
        size_t  get_goal_index(void)    const noexcept;

        void set_vehicle_id(int)    noexcept;
        int  get_vehicle_id(void)   const noexcept;

        GoalReqPacket& operator=(const GoalReqPacket&);
        GoalReqPacket& operator=(GoalReqPacket&&);
    };

    /*  DATA STRUCTURE:
    *       id | length | goal | vehicle id
    *       1B | 4B     | 8B   | 4B
    */

    class GoalPacket : public Packet
    {
    private:
        float goal_x, goal_y;
        int vehicle_id;

    public:
        static constexpr uint8_t PACKET_ID = 5;
        static constexpr size_t SIZE_GOAL = 2 * sizeof(float);
        static constexpr size_t SIZE_VEHICLE_ID = sizeof(int);

        GoalPacket(void);
        GoalPacket(const GoalPacket&);
        GoalPacket(GoalPacket&&);
        virtual ~GoalPacket(void) {/*dtor*/}

        virtual packet_error encode(void);
        virtual packet_error decode(void);

        virtual inline uint8_t id(void) const noexcept      {return PACKET_ID;}
        virtual inline size_t min_size(void) const noexcept {return SIZE_ID + SIZE_PACKET_LENGTH + SIZE_GOAL + SIZE_VEHICLE_ID;}

        void    set_goal(float, float)  noexcept;
        float   get_goal_x(void)        const noexcept;
        float   get_goal_y(void)        const noexcept;

        void set_vehicle_id(int)    noexcept;
        int  get_vehicle_id(void)   const noexcept;

        GoalPacket& operator=(const GoalPacket&);
        GoalPacket& operator=(GoalPacket&&);
    };
};

#endif //__schwarm_packet_h__