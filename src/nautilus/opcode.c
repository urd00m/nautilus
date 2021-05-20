#include <nautilus/nautilus.h>
#include <nautilus/cpuid.h>
#include <nautilus/naut_string.h>
#include <nautilus/shell.h>
#include <nautilus/msr.h>
#include <nautilus/idt.h> //in order to specify return address here 


#define CPUID_FEAT_EDX_MSR 1<<5
#define RESERVED 1<<20
/*
	Opcode branch functions to play with CPUID and MSR
*/

/*
   Return function for general protection fault 
*/
static void
gp_handler() {
    nk_vc_printf("Got a general protection fault\n");
    change_ret_addr(0); 
    return 0; 
}









//TODO: The redirection from the exception handler isn't properly working


/*
    TODO: Test function for reading from MSRs 
*/
void
read_msr_at(uint32_t location) {
    uint32_t lo, hi;
    uint64_t ret; 
    asm volatile("rdmsr" : "=a"(lo), "=d"(hi) : "c"(location)); //rdmsr 
    ret = ((uint64_t)hi << 32) | lo;
    nk_vc_printf("MSR Register value: 0x%08x\n", ret);
}

static int
handle_test_msr(char * buf, void * priv) {
    uint32_t location; 
    change_ret_addr(gp_handler); //set it to our handler 

    //Should print the APIC BASE value 
    location = 0x1B; 
    read_msr_at(location);

    //Should fault, tries to read msr from 0x02 which doesn't exist 
    location = 0x02; 
    read_msr_at(location); 

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

