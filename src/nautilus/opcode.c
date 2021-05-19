#include <nautilus/nautilus.h>
#include <nautilus/cpuid.h>
#include <nautilus/naut_string.h>
#include <nautilus/shell.h>

#define CPUID_FEAT_EDX_MSR 1<<5
#define RESERVED 1<<20
/*
	Opcode branch functions to play with CPUID and MSR
*/














/*
	CPUID play functions


	Reads from CPUID and prints it out 
	TODO: reads from the MSR bit 
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
    nk_vc_printf("Contents of CPUID with 1 as func: 0x%08x\n", test_bit); //reading from the edx register
}

static int
handle_read_cpuid(char * buf, char * priv) {
    read_cpuid();
    return 0; 
}

static struct shell_cmd_impl read_cpuid_impl = {
    .cmd      = "readcpu",
    .help_str = "readcpu",
    .handler  = handle_read_cpuid,
};
nk_register_shell_cmd(read_cpuid_impl);


