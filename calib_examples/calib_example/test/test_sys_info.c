/*********************************************************************
 * $Id: 
 *********************************************************************/

#include <calib.h>
#include <sys_info.h>

int main(void)
{
    printf("== Platform information ==\n");
    printf("Found CPU is <%s>\n", sys_info_cpu_type());
    printf("sizeof(int)       = %d\n", (int32_t)sizeof(int));
    printf("sizeof(long)      = %d\n", (int32_t)sizeof(long));
    printf("sizeof(long long) = %d\n", (int32_t)sizeof(long long));

    printf("== Build information ==\n");
    printf("Build mode  = %s\n", build_info_build_mode());
    printf("Compile bit = %d\n", build_info_compile_bit());
    return 0;
}
