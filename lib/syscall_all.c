#include "../drivers/gxconsole/dev_cons.h"
#include <mmu.h>
#include <env.h>
#include <printf.h>
#include <pmap.h>
#include <sched.h>

extern char *KERNEL_SP;
extern struct Env *curenv;

/* Overview:
 * 	This function is used to print a character on screen.
 *
 * Pre-Condition:
 * 	`c` is the character you want to print.
 */
void sys_putchar(int sysno, int c, int a2, int a3, int a4, int a5)
{
	printcharc((char) c);
	return ;
}

/* Overview:
 * 	This function enables you to copy content of `srcaddr` to `destaddr`.
 *
 * Pre-Condition:
 * 	`destaddr` and `srcaddr` can't be NULL. Also, the `srcaddr` area
 * 	shouldn't overlap the `destaddr`, otherwise the behavior of this
 * 	function is undefined.
 *
 * Post-Condition:
 * 	the content of `destaddr` area(from `destaddr` to `destaddr`+`len`) will
 * be same as that of `srcaddr` area.
 */
void *memcpy(void *destaddr, void const *srcaddr, u_int len)
{
	char *dest = destaddr;
	char const *src = srcaddr;

	while (len-- > 0) {
		*dest++ = *src++;
	}

	return destaddr;
}

/* Overview:
 *	This function provides the environment id of current process.
 *
 * Post-Condition:
 * 	return the current environment id
 */
u_int sys_getenvid(void)
{
	return curenv->env_id;
}

u_int sys_getfaid(int sysno, u_int envid)
{
	struct Env *e;
	envid2env(envid, &e, 0);
	return e->env_parent_id;
}

/* Overview:
 *	This function enables the current process to give up CPU.
 *
 * Post-Condition:
 * 	Deschedule current environment. This function will never return.
 */
/*** exercise 4.6 ***/
void sys_yield(void)
{
	bcopy((void *)(KERNEL_SP - sizeof(struct Trapframe)),
          (void *)(TIMESTACK - sizeof(struct Trapframe)),
		  sizeof(struct Trapframe));
	sched_yield();
}

/* Overview:
 * 	This function is used to destroy the current environment.
 *
 * Pre-Condition:
 * 	The parameter `envid` must be the environment id of a
 * process, which is either a child of the caller of this function
 * or the caller itself.
 *
 * Post-Condition:
 * 	Return 0 on success, < 0 when error occurs.
 */
int sys_env_destroy(int sysno, u_int envid)
{
	/*
		printf("[%08x] exiting gracefully\n", curenv->env_id);
		env_destroy(curenv);
	*/
	int r;
	struct Env *e;

	if ((r = envid2env(envid, &e, 1)) < 0) {
		return r;
	}

//	printf("[%08x] destroying %08x\n", curenv->env_id, e->env_id);
	env_destroy(e);
	return 0;
}

/* Overview:
 * 	Set envid's pagefault handler entry point and exception stack.
 *
 * Pre-Condition:
 * 	xstacktop points one byte past exception stack.
 *
 * Post-Condition:
 * 	The envid's pagefault handler will be set to `func` and its
 * 	exception stack will be set to `xstacktop`.
 * 	Returns 0 on success, < 0 on error.
 */
/*** exercise 4.12 ***/
int sys_set_pgfault_handler(int sysno, u_int envid, u_int func, u_int xstacktop)
{
	// Your code here.
	struct Env *env;
	int ret;
	if ((ret = envid2env(envid, &env, 1)) < 0) return ret;
	env->env_pgfault_handler = func;
	env->env_xstacktop = xstacktop;
	return 0;
	//	panic("sys_set_pgfault_handler not implemented");
}

/* Overview:
 * 	Allocate a page of memory and map it at 'va' with permission
 * 'perm' in the address space of 'envid'.
 *
 * 	If a page is already mapped at 'va', that page is unmapped as a
 * side-effect.
 *
 * Pre-Condition:
 * perm -- PTE_V is required,
 *         PTE_COW is not allowed(return -E_INVAL),
 *         other bits are optional.
 *
 * Post-Condition:
 * Return 0 on success, < 0 on error
 *	- va must be < UTOP
 *	- env may modify its own address space or the address space of its children
 */
/*** exercise 4.3 ***/
int sys_mem_alloc(int sysno, u_int envid, u_int va, u_int perm)
{
	// Your code here.
	struct Env *env;
	struct Page *ppage;
	int ret;
	ret = 0;
	if (va >= UTOP || (perm & PTE_COW) || !(perm & PTE_V)) return -E_INVAL;
	if ((ret = envid2env(envid, &env, 1)) < 0) return ret;
	if ((ret = page_alloc(&ppage)) < 0) return ret;
	if ((ret = page_insert(env->env_pgdir, ppage, va, perm)) < 0) return ret;
	return ret;
}

/* Overview:
 * 	Map the page of memory at 'srcva' in srcid's address space
 * at 'dstva' in dstid's address space with permission 'perm'.
 * Perm has the same restrictions as in sys_mem_alloc.
 * (Probably we should add a restriction that you can't go from
 * non-writable to writable?)
 *
 * Post-Condition:
 * 	Return 0 on success, < 0 on error.
 *
 * Note:
 * 	Cannot access pages above UTOP.
 */
/*** exercise 4.4 ***/
int sys_mem_map(int sysno, u_int srcid, u_int srcva, u_int dstid, u_int dstva,
				u_int perm)
{

	int ret;
	u_int round_srcva, round_dstva;
	struct Env *srcenv;
	struct Env *dstenv;
	struct Page *ppage;
	Pte *ppte;

	ppage = NULL;
	ret = 0;
	round_srcva = ROUNDDOWN(srcva, BY2PG);
	round_dstva = ROUNDDOWN(dstva, BY2PG);

    //your code here
	if (round_srcva >= UTOP || round_dstva >= UTOP || !(perm & PTE_V)) return -E_INVAL;
	if ((ret = envid2env(srcid, &srcenv, 0)) < 0) return ret;
	if ((ret = envid2env(dstid, &dstenv, 0)) < 0) return ret;
	if ((ppage = page_lookup(srcenv->env_pgdir, round_srcva, &ppte)) == NULL) return -E_INVAL;
	if (((*ppte & PTE_R) == 0) && ((perm & PTE_R) == 1)) return -E_INVAL;
	if ((ret = page_insert(dstenv->env_pgdir, ppage, round_dstva, perm)) < 0) return ret;
	return 0;
}

/* Overview:
 * 	Unmap the page of memory at 'va' in the address space of 'envid'
 * (if no page is mapped, the function silently succeeds)
 *
 * Post-Condition:
 * 	Return 0 on success, < 0 on error.
 *
 * Cannot unmap pages above UTOP.
 */
/*** exercise 4.5 ***/
int sys_mem_unmap(int sysno, u_int envid, u_int va)
{
	// Your code here.
	int ret;
	struct Env *env;
	if (va >= UTOP) return -E_INVAL;
	if ((ret = envid2env(envid, &env, 0)) < 0) return ret;
	page_remove(env->env_pgdir, va);
	return 0;
	//	panic("sys_mem_unmap not implemented");
}

/* Overview:
 * 	Allocate a new environment.
 *
 * Pre-Condition:
 * The new child is left as env_alloc created it, except that
 * status is set to ENV_NOT_RUNNABLE and the register set is copied
 * from the current environment.
 *
 * Post-Condition:
 * 	In the child, the register set is tweaked so sys_env_alloc returns 0.
 * 	Returns envid of new environment, or < 0 on error.
 */
/*** exercise 4.8 ***/
int sys_env_alloc(void)
{
	// Your code here.
	int r;
	struct Env *e;
	if ((r = env_alloc(&e, curenv->env_id)) < 0) return r;
	bcopy((void *) (KERNEL_SP - sizeof(struct Trapframe)), &(e->env_tf),
		  sizeof(struct Trapframe));
	e->env_tf.pc = e->env_tf.cp0_epc;
	e->env_tf.regs[2] = 0;	// $v0, return value should be set to 0 because this is a child env
	e->env_status = ENV_NOT_RUNNABLE;
	e->env_pri = curenv->env_pri;
	return e->env_id;
	//	panic("sys_env_alloc not implemented");
}

/* Overview:
 * 	Set envid's env_status to status.
 *
 * Pre-Condition:
 * 	status should be one of `ENV_RUNNABLE`, `ENV_NOT_RUNNABLE` and
 * `ENV_FREE`. Otherwise return -E_INVAL.
 *
 * Post-Condition:
 * 	Returns 0 on success, < 0 on error.
 * 	Return -E_INVAL if status is not a valid status for an environment.
 * 	The status of environment will be set to `status` on success.
 */
/*** exercise 4.14 ***/
int sys_set_env_status(int sysno, u_int envid, u_int status)
{
	// Your code here.
	struct Env *env;
	int ret;
	if (!(status >= 0 && status <= 2)) return -E_INVAL;
	if ((ret = envid2env(envid, &env, 1)) < 0) return ret;
	
	if (status == ENV_RUNNABLE && env->env_status != ENV_RUNNABLE) {
		LIST_INSERT_HEAD(env_sched_list, env, env_sched_link);
	}

	env->env_status = status;
	return 0;
	//	panic("sys_env_set_status not implemented");
}

/* Overview:
 * 	Set envid's trap frame to tf.
 *
 * Pre-Condition:
 * 	`tf` should be valid.
 *
 * Post-Condition:
 * 	Returns 0 on success, < 0 on error.
 * 	Return -E_INVAL if the environment cannot be manipulated.
 *
 * Note: This hasn't be used now?
 */
int sys_set_trapframe(int sysno, u_int envid, struct Trapframe *tf)
{

	return 0;
}

/* Overview:
 * 	Kernel panic with message `msg`.
 *
 * Pre-Condition:
 * 	msg can't be NULL
 *
 * Post-Condition:
 * 	This function will make the whole system stop.
 */
void sys_panic(int sysno, char *msg)
{
	// no page_fault_mode -- we are trying to panic!
	panic("%s", TRUP(msg));
}

/* Overview:
 * 	This function enables caller to receive message from
 * other process. To be more specific, it will flag
 * the current process so that other process could send
 * message to it.
 *
 * Pre-Condition:
 * 	`dstva` is valid (Note: NULL is also a valid value for `dstva`).
 *
 * Post-Condition:
 * 	This syscall will set the current process's status to
 * ENV_NOT_RUNNABLE, giving up cpu.
 */
/*** exercise 4.7 ***/
void sys_ipc_recv(int sysno, u_int dstva)
{
	if (dstva >= UTOP) {
		panic("sys_ipc_recv dstva is out of range!");
		return ;
	}
	curenv->env_ipc_recving = 1;
	curenv->env_ipc_dstva = dstva;
	curenv->env_status = ENV_NOT_RUNNABLE;
	sys_yield();
}

/* Overview:
 * 	Try to send 'value' to the target env 'envid'.
 *
 * 	The send fails with a return value of -E_IPC_NOT_RECV if the
 * target has not requested IPC with sys_ipc_recv.
 * 	Otherwise, the send succeeds, and the target's ipc fields are
 * updated as follows:
 *    env_ipc_recving is set to 0 to block future sends
 *    env_ipc_from is set to the sending envid
 *    env_ipc_value is set to the 'value' parameter
 * 	The target environment is marked runnable again.
 *
 * Post-Condition:
 * 	Return 0 on success, < 0 on error.
 *
 * Hint: the only function you need to call is envid2env.
 */
/*** exercise 4.7 ***/
int sys_ipc_can_send(int sysno, u_int envid, u_int value, u_int srcva,
					 u_int perm)
{
	int r;
	struct Env *e;
	struct Page *p;
	
	if (srcva >= UTOP) {
        panic("sys_ipc_can_send srcva is out of range!");
        return -E_IPC_NOT_RECV;
    }
	if ((r = envid2env(envid, &e, 0)) < 0) return r;
	if (!e->env_ipc_recving) return -E_IPC_NOT_RECV;
	e->env_ipc_recving = 0;
	e->env_ipc_perm = perm;
	e->env_ipc_from = curenv->env_id;
	e->env_ipc_value = value;
	e->env_status = ENV_RUNNABLE;
	if (srcva) {
		Pte *tmp;
		if ((p = page_lookup(curenv->env_pgdir, srcva, &tmp)) == NULL) return -E_IPC_NOT_RECV;
		if ((r = page_insert(e->env_pgdir, p, e->env_ipc_dstva, perm)) < 0) return r;
	}
	return 0;
}
/* Overview:
 * 	This function is used to write data to device, which is
 * 	represented by its mapped physical address.
 *	Remember to check the validity of device address (see Hint below);
 * 
 * Pre-Condition:
 *      'va' is the starting address of source data, 'len' is the
 *      length of data (in bytes), 'dev' is the physical address of
 *      the device
 * 	
 * Post-Condition:
 *      copy data from 'va' to 'dev' with length 'len'
 *      Return 0 on success.
 *	Return -E_INVAL on address error.
 *      
 * Hint: Use ummapped segment in kernel address space to perform MMIO.
 *	 Physical device address:
 *	* ---------------------------------*
 *	|   device   | start addr | length |
 *	* -----------+------------+--------*
 *	|  console   | 0x10000000 | 0x20   |
 *	|    IDE     | 0x13000000 | 0x4200 |
 *	|    rtc     | 0x15000000 | 0x200  |
 *	* ---------------------------------*
 */
 /*** exercise 5.1 ***/
int sys_write_dev(int sysno, u_int va, u_int dev, u_int len)
{
    // Your code here
	int dev_cnt = 3;
	u_int dev_start_addr[] = {0x10000000, 0x13000000, 0x15000000};
	u_int dev_length[] = {0x20, 0x4200, 0x200};
	u_int target_addr = dev + 0xa0000000;
	int i = 0;
	int checked = 0;
	
	// check
	if (va >= ULIM) return -E_INVAL;
	for (i = 0; i < dev_cnt; i++) {
		if (dev_start_addr[i] <= dev && dev + len - 1 < dev_start_addr[i] + dev_length[i]) {
			checked = 1;
			break;
		}
	}
	if (!checked) return -E_INVAL;
	
	// write
	bcopy((void *)va, (void *)target_addr, len);

	return 0;
}

/* Overview:
 * 	This function is used to read data from device, which is
 * 	represented by its mapped physical address.
 *	Remember to check the validity of device address (same as sys_write_dev)
 * 
 * Pre-Condition:
 *      'va' is the starting address of data buffer, 'len' is the
 *      length of data (in bytes), 'dev' is the physical address of
 *      the device
 * 
 * Post-Condition:
 *      copy data from 'dev' to 'va' with length 'len'
 *      Return 0 on success, < 0 on error
 *      
 * Hint: Use ummapped segment in kernel address space to perform MMIO.
 */
 /*** exercise 5.1 ***/
int sys_read_dev(int sysno, u_int va, u_int dev, u_int len)
{
    // Your code here
	int dev_cnt = 3;
	u_int dev_start_addr[] = {0x10000000, 0x13000000, 0x15000000};
    u_int dev_length[] = {0x20, 0x4200, 0x200};
    u_int target_addr = dev + 0xa0000000;
    int i = 0;
    int checked = 0;

    // check
    if (va >= ULIM) return -E_INVAL;
    for (i = 0; i < dev_cnt; i++) {
        if (dev_start_addr[i] <= dev && dev + len - 1 < dev_start_addr[i] + dev_length[i]) {
            checked = 1;
            break;
        }
    }
    if (!checked) return -E_INVAL;

	// read
	bcopy((void *)target_addr, (void *)va, len);

	return 0;
}

/* 0 -- create
 * 1 -- get
 * 2 -- set
 * 3 -- unset
 * 4 -- get list
 */
#define NVARS 256
struct Var {
	char name[64];
	char value[128];
	int readonly;
	int environ;
	int envid;
	int hold;
};
static struct Var vars[NVARS] = {0};

int sys_env_var(int sysno, u_int envid, char *name, char *value, u_int op, u_int mode) {
		
	// get list
    if (op == 4) {
		char buf[51200] = {0};
		int i;
        for (i = 0; i < NVARS; i++) {
			struct Var *v = &vars[i];
			if (v->hold) {
				if (v->environ || v->envid == envid) {
					strcat(buf, v->name);
					strcat(buf, "=");
					strcat(buf, v->value);
					strcat(buf, "\n");
				}
			}
		}
		if (!buf[0]) return -E_ENV_VAR_NOT_FOUND;
		strcpy(value, buf);
		return 0;
    }

    u_int pos = strhash(name);
	u_int init_pos = pos;
    while (1) {
		struct Var *v = &vars[pos];
		/*if (v->hold) {
			printf("env: %d, envid: %d(%d), name: %s(%s)\n", v->environ, v->envid, envid, v->name, name);
		}*/
        if (v->hold && (v->environ || v->envid == envid) &&
			strcmp(v->name, name) == 0) {
			// found
			// printf("found %s\n", name);
            break;
        } else {
			// create here if op=(create)	
			if (!v->hold && op == 0) break;
			// not found, go on to search
            ++pos;
            if (pos == NVARS) pos = 0;	// circle
			if (pos == init_pos) return -E_ENV_VAR_NOT_FOUND;
        }
    }

    if (op == 0) {
		// create the var, set and store it into table
        strcpy(vars[pos].name, name);
        strcpy(vars[pos].value, value);
		vars[pos].envid = envid;
		vars[pos].hold = 1;
    } 
	else if (op == 1) {
		// get value and return with "value"
        strcpy(value, vars[pos].value);
    } 
	else if (op == 2) {
		// set value if not read only
        if (vars[pos].readonly) return -E_ENV_VAR_RDONLY;
        strcpy(vars[pos].value, value);
    } 
	else if (op == 3) {
		// delete the var if not read only
		if (strcmp(vars[pos].name, "curpath") == 0) return;
        if (vars[pos].readonly) return -E_ENV_VAR_RDONLY;
		vars[pos].hold = 0;
    }

	if (mode & 0x001 && strcmp(vars[pos].name, "curpath") != 0)	// read only
		vars[pos].readonly = 1;
	if (mode & 0x002)
		vars[pos].environ = 1;

    return 0;
}

