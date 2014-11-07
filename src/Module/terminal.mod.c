#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x60ac67a6, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x9f0dd7e2, __VMLINUX_SYMBOL_STR(single_release) },
	{ 0xab0e5175, __VMLINUX_SYMBOL_STR(seq_read) },
	{ 0xf69be43a, __VMLINUX_SYMBOL_STR(remove_proc_entry) },
	{ 0x4041587b, __VMLINUX_SYMBOL_STR(kthread_stop) },
	{ 0x49351084, __VMLINUX_SYMBOL_STR(STUB_issue_request) },
	{ 0x750a30, __VMLINUX_SYMBOL_STR(STUB_stop_shuttle) },
	{ 0xa1346a90, __VMLINUX_SYMBOL_STR(STUB_start_shuttle) },
	{ 0xf01a8493, __VMLINUX_SYMBOL_STR(__mutex_init) },
	{ 0x507aea29, __VMLINUX_SYMBOL_STR(proc_create_data) },
	{ 0xf9a482f9, __VMLINUX_SYMBOL_STR(msleep) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x91831d70, __VMLINUX_SYMBOL_STR(seq_printf) },
	{ 0x11faba9a, __VMLINUX_SYMBOL_STR(single_open) },
	{ 0xf94457bb, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0xfa62b081, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x13c5932b, __VMLINUX_SYMBOL_STR(wake_up_process) },
	{ 0x2407fb79, __VMLINUX_SYMBOL_STR(kthread_create_on_node) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x6c52d35d, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0x17e5d6d0, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "818E236FA2318AA13B30B5A");
