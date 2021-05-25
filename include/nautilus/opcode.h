#define CPUID_FEAT_EDX_MSR 1<<5
#define RESERVED 1<<20
//#define DEBUG 1

//provides status updates every [x] instructions
#define STATUS 65536

//For MSR 0x40000105 - Guest crash capabilities - related to hypervisor
#define MSR_GUEST_CRASH 0x40000105
struct msr_guest_crash {
    union {
        uint32_t val;
        struct {
            uint8_t b0       : 1;
            uint8_t b1       : 1;
            uint8_t b2       : 1;
            uint8_t b3       : 1;
            uint8_t b4       : 1;
            uint8_t b5       : 1;
            uint8_t b6       : 1;
            uint8_t b7       : 1;
            uint8_t b8       : 1;
            uint8_t b9       : 1;
            uint8_t b10      : 1; //first 11 bits are saved for ease of access
            uint8_t b11_63   : 1; //the next 63 bits are put for saving space
        } __packed;
    } __packed;
} __packed;





//Scans the MSR at the location and prints out the returned values along, or if faulted it prints fault when DEBUG is on
void read_msr_at(uint32_t location);

//Sets faulted_opcode value to 1 to let opcode.c know that the current location is an invalid MSR address
void invalid_msr();

