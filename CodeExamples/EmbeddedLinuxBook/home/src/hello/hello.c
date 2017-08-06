/*
    Trivial example of a loadable kernel module illustrating module
    parameters.
*/
#include <linux/module.h>
#include <linux/kernel.h>

char *my_string = "Homer";
int my_int = 1;

MODULE_AUTHOR("Doug Abbott, doug@intellimetrix.us");
MODULE_DESCRIPTION("Trivial loadable module illustrating module parameters");

module_param(my_string, charp, S_IRUGO | S_IWUSR);    // make them writable
MODULE_PARM_DESC(my_string, "Sample string parameter");
module_param(my_int, int, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(my_int, "Sample integer parameter");

int init_module (void)
{
    printk ("<1>Hello %s, you are number %d\n", my_string, my_int);
    return 0;
}

void cleanup_module (void)
{
    printk ("<1>Goodbye %s, you are now number %d\n", my_string, my_int);
}

