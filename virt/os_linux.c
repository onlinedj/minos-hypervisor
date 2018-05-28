#include <minos/minos.h>
#include <minos/os.h>
#include <minos/sched.h>
#include <minos/arch.h>
#include <virt/virt.h>
#include <minos/init.h>

static void linux_vcpu_init(struct vcpu *vcpu)
{
	gp_regs *regs = vcpu_to_gp_regs(vcpu);

	/* fill the dtb address to x0 */
	if (get_vcpu_id(vcpu) == 0) {
		regs->x0 = vcpu->vm->setup_data;
		vcpu_online(vcpu);
	}
}

static void linux_vcpu_power_on(struct vcpu *vcpu, unsigned long entry)
{
	gp_regs *regs = vcpu_to_gp_regs(vcpu);

	regs->elr_elx = entry;
	regs->x0 = 0;
	regs->x1 = 0;
	regs->x2 = 0;
	regs->x3 = 0;
	vcpu_online(vcpu);
}

struct os_ops linux_os_ops = {
	.vcpu_init = linux_vcpu_init,
	.vcpu_power_on = linux_vcpu_power_on,
};

static int os_linux_init(void)
{
	struct os *os;

	os = alloc_os("linux");
	if (!os)
		return -EINVAL;

	os->ops = &linux_os_ops;

	return register_os(os);
}

module_initcall(os_linux_init);
