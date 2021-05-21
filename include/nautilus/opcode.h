#define CPUID_FEAT_EDX_MSR 1<<5
#define RESERVED 1<<20
//#define DEBUG 1

//Scans the MSR at the location and prints out the returned values along, or if faulted it prints fault when DEBUG is on
void read_msr_at(uint32_t location);

//Sets faulted_opcode value to 1 to let opcode.c know that the current location is an invalid MSR address
void invalid_msr();

