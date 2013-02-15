#include "idt.h"
#include "oops.h"

void do_divide_error(int err)	
{
	caos_printf("DEVIDE ERROR  ");
	caos_printf("ERROR=%d\n", err);
	//oops(0);

}
void do_debug(int err)
{
	//oops(1);
}
void do_nmi(int err)
{
	//oops(2);
}
void do_int3(int err)	
{
	//oops(3);
}
void do_overflow(int err)
{
	//oops(4);
}
void do_bounds(int err)
{
	//oops(5);
}
void do_invalid_op(int err)
{
	//oops(6);
}
void do_device_not_available(int err)
{
	//oops(7);
}
void do_doublefault_fn(int err)	
{
	caos_printf("DOUBLE FAULT, ERROR=%d\n", err);
	//oops(8);
}
void do_coprocessor_segment_overrun(int err)	
{
	//oops(9);
}
void do_invalid_TSS(int err)
{
	//oops(10);
}
void do_segment_not_present(int err)
{
	//oops(11);
}
void do_stack_segment(int err)
{
	//oops(12);
}
void do_general_protection(int err)
{
	caos_printf("ERROR=%x", err);
	//oops(13);
}
void do_page_fault(int err)
{
	//oops(14);

}
void do_coprocessor_error(int err)
{
	//oops(16);
}
void do_alignment_check(int err)	
{
	//oops(17);
}
void do_machine_check(int err)
{
	oops(18);
}
void do_simd_coprocessor_error(int err)
{
	oops(19);
}

