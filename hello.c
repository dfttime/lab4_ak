// SPDX-License-Identifier: (GPL-2.0 OR BSD-3-Clause)
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/slab.h>

MODULE_AUTHOR("Vasiliev Vlad <dfttime@ukr.net>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static uint paramValue = 1;
module_param(paramValue, uint, 0444);
MODULE_PARM_DESC(paramValue, "My description");

struct myStruct {
	struct list_head list;
	ktime_t myTime;
};

static LIST_HEAD(myList);
int counter;

static int __init hello_init(void)
{
	if (paramValue > 10) {
		pr_err("Error: paramValue cannot be greater than 10\n");
		return -EINVAL;
	}

	if (paramValue == 0 || (paramValue <= 10 && paramValue >= 5))
		pr_emerg("Warning: %u\n", paramValue);

	counter = 0;
	while (counter != paramValue) {
		struct myStruct *ptr = kmalloc(sizeof(*ptr), GFP_KERNEL);

		if (!ptr)
			return -ENOMEM;

		ptr->myTime = ktime_get();
		list_add_tail(&ptr->list, &myList);
		pr_emerg("Hello world!\n");
		counter++;
	}

	return 0;
}

static void __exit hello_exit(void)
{
	struct myStruct *ptr, *next;

	list_for_each_entry_safe(ptr, next, &myList, list) {
		pr_emerg("Time in nanoseconds: %lld\n", ktime_to_ns(ptr->myTime));
		list_del(&ptr->list);
		kfree(ptr);
	}
}

module_init(hello_init);
module_exit(hello_exit);

