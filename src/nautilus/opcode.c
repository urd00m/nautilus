#include <nautilus/nautilus.h>
#include <nautilus/cpuid.h>
#include <nautilus/naut_string.h>
#include <nautilus/shell.h>
#include <nautilus/msr.h>
#include <nautilus/idt.h>
#include <nautilus/opcode.h>

/*
	Opcode branch functions to play with CPUID and MSR
*/
int faulted_opcode;
uint32_t msr_found;
/*
    Scans all 4 billion possible MSRs and prints out valid MSRs to the screen for IO redirection to file
*/
static int
handle_scan_msr(char * buf, void * priv) {
    uint32_t location;
    msr_found = 0;
    for(location = 0; location < 0xffffffff; location++) { //scanning the 4 billion possible MSRs, our general protection fault catcher will print it to the screen, where we redirect it to a file 
        read_msr_at(location); 
    }
    return 0;
}


/*
    Read function for MSR 
*/
void
read_msr_at(uint32_t location) {
    uint32_t lo, hi;
    uint64_t ret; 

    set_fault(); //turn on our catcher
    asm volatile("rdmsr" : "=a"(lo), "=d"(hi) : "c"(location)); //rdmsr 
    reset_fault(); //turn off our catcher 

    ret = ((uint64_t)hi << 32) | lo;
    if(!faulted_opcode) {
        nk_vc_printf("RETURNED---- Hi order (edx): 0x%08x Lo order (eax): 0x%08x\n", hi, lo);
        nk_vc_printf("RETURNED---- MSR Register value: 0x%llx\n", ret);
        nk_vc_printf("IMPORTANT--- MSR address 0x%08x\n", location); 
        msr_found = msr_found+1;
    }
#ifdef DEBUG
    nk_vc_printf("INFO---- Faulted value %d\n", faulted_opcode); 
    nk_vc_printf("INFO---- Hi order (edx): 0x%08x Lo order (eax): 0x%08x\n", hi, lo);
    nk_vc_printf("INFO---- MSR Register value: 0x%llx\n", ret);
    nk_vc_printf("STATUS---- MSR address 0x%08x\n", location); 
    nk_vc_printf("SKIP-----------------------------------\n\n");  
#endif

#ifdef STATUS
    if(faulted_opcode == 0 || location%STATUS == 0) { //provides updates every STATUS instructions or upon a new MSR
         nk_vc_printf("STATUS---- Current Address 0x%08x      MSRs found: %d\n", location, msr_found);
    }
#endif

    faulted_opcode = 0; //reset fault 
}

static int
handle_test_msr(char * buf, void * priv) {
    uint32_t location; 

    //Should print the APIC BASE value (uses 32 bits)
    location = 0x1B; 
    read_msr_at(location);

    //Should fault, tries to read msr from 0x02 which doesn't exist 
    location = 0x02; 
    read_msr_at(location); 

    //Should fault, tries to read msr from 0x02 which doesn't exist 
    location = 0x03; 
    read_msr_at(location); 

    //Test run 1, address stalled the processor
    location = 0x83e;
    read_msr_at(location);


    //begin testing write msr 
    nk_vc_printf("\nTesting write MSR\n\n");

    //testing the guest crash msr
    struct msr_guest_crash data; 
    location = MSR_GUEST_CRASH;
    data.b0 = 1;
    read_msr_at(location);  //confirm 
    msr_write(location, data.val);
    read_msr_at(location); //confirm 
    nk_vc_printf("Tested location 0x%08x with value 0x%016llx\n\n", location, data.val); 

    //testing the msr 0 
    location = 0x00000010; 
    data.val = 0xffffffff; 
    read_msr_at(location); 
    msr_write(location, data.val); //TODO: switch to a correct struct
    read_msr_at(location); 
    nk_vc_printf("Tested location 0x%08x with value 0x%016llx\n", location, data.val); 

    return 0;
}

/*
	CPUID play functions
	Reads from CPUID and prints it out whether MSR are turned on for the CPU, also reads from a reserved section which should be zero 
*/
void
read_cpuid() {
    int msr_bit; 
    int test_bit; 
    cpuid_ret_t ret;
    cpuid(CPUID_FEATURE_INFO, &ret);
    msr_bit = ret.d & CPUID_FEAT_EDX_MSR;
    msr_bit = !!msr_bit;
    nk_vc_printf("MSR on: 0x%01x\n", msr_bit); //reading from the edx register
    test_bit = ret.d & RESERVED;
    test_bit = !!test_bit;
    nk_vc_printf("Contents of CPUID reserved bit (should be zero): 0x%08x\n", test_bit); //reading from the edx register
}

static int
handle_read_cpuid(char * buf, void * priv) {
    read_cpuid();
    return 0; 
}


/*
    register commands to shell
*/
static struct shell_cmd_impl read_cpuid_impl = {
    .cmd      = "checkmsr",
    .help_str = "checkmsr",
    .handler  = handle_read_cpuid,
};
nk_register_shell_cmd(read_cpuid_impl);

static struct shell_cmd_impl msr_test_impl = {
    .cmd      = "testmsr",
    .help_str = "testmsr",
    .handler  = handle_test_msr,
};
nk_register_shell_cmd(msr_test_impl);

static struct shell_cmd_impl msr_scan_impl = {
    .cmd      = "scanmsr",
    .help_str = "scanmsr",
    .handler  = handle_scan_msr,
};
nk_register_shell_cmd(msr_scan_impl);


/*
    Lets the exception handler tell us we faulted 
*/
void 
invalid_msr() {
    faulted_opcode = 1; 
}
