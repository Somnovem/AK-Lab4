/* SPDX-License-Identifier: GPL-2.0 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include <linux/errno.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("IO-35 Zhmura Artem");
MODULE_DESCRIPTION("Hello, world module with parameter and time list");
MODULE_VERSION("0.1");

static unsigned int count = 1;
module_param(count, uint, 0444);
MODULE_PARM_DESC(count, "How many times to print 'Hello, world!' (uint)");

struct hello_item {
	struct list_head list;
	ktime_t ts;
};

static LIST_HEAD(hello_head);

static int __init hello_init(void)
{
	unsigned int i;

	if (count == 0 || (count >= 5 && count <= 10))
		pr_warn("hello: count=%u (warning range)\n", count);

	if (count > 10) {
		pr_err("hello: count=%u (error range)\n", count);
		return -EINVAL;
    }

	for (i = 0; i < count; i++) {
		struct hello_item *item = kmalloc(sizeof(*item), GFP_KERNEL);

		if (!item)
			return -ENOMEM;

		item->ts = ktime_get();
		list_add_tail(&item->list, &hello_head);
		pr_info("Hello, world! (%u)\n", i + 1);
	}

	return 0;
}

static void __exit hello_exit(void)
{
	struct hello_item *item, *tmp;

	list_for_each_entry_safe(item, tmp, &hello_head, list) {
		s64 ns = ktime_to_ns(item->ts);

		pr_info("goodbye: ts=%lld ns\n", (long long)ns);
		list_del(&item->list);
		kfree(item);
	}
}

module_init(hello_init);
module_exit(hello_exit);
