//Dalton Meny
//CS 470
//Homework 1

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h> //library for lists
#include <linux/types.h> //library for list_head
#include <linux/slab.h> //library for kmalloc

//Struct for items of the list
struct birthday {
  int day;
  int month;
  int year;
  struct list_head list;
};

//Reference to the head of the list
static LIST_HEAD(birthday_list);

/* This function is called when the module is loaded. */
int simple_init(void)
{
  struct birthday *person;
  struct birthday *ptr;
  
  printk(KERN_INFO "Loading Module\n"); 

  //Creating the first item in the list
  person = kmalloc(sizeof(*person), GFP_KERNEL);
  person->day = 2;
  person->month = 8;
  person->year = 1995;
  INIT_LIST_HEAD(&person->list);
  list_add_tail(&person->list, &birthday_list);

  //Second birthday
  person = kmalloc(sizeof(*person), GFP_KERNEL);
  person->day = 2;
  person->month = 6;
  person->year = 1997;
  INIT_LIST_HEAD(&person->list);
  list_add_tail(&person->list, &birthday_list);

  //Third birthday
  person = kmalloc(sizeof(*person), GFP_KERNEL);
  person->day = 1;
  person->month = 1;
  person->year = 2001;
  INIT_LIST_HEAD(&person->list);
  list_add_tail(&person->list, &birthday_list);

  //Fourth birthday
  person = kmalloc(sizeof(*person), GFP_KERNEL);
  person->day = 10;
  person->month = 21;
  person->year = 1975;
  INIT_LIST_HEAD(&person->list);
  list_add_tail(&person->list, &birthday_list);

  //Fifth birthday
  person = kmalloc(sizeof(*person), GFP_KERNEL);
  person->day = 7;
  person->month = 4;
  person->year = 1776;
  INIT_LIST_HEAD(&person->list);
  list_add_tail(&person->list, &birthday_list);
  

  //Printing out each item in the list
  list_for_each_entry(ptr, &birthday_list, list) {
    printk(KERN_INFO "%d/%d/%d\n", ptr->day,
	   ptr->month, ptr->year);
  }

  return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) {

  struct birthday *ptr, *next;

  
  printk(KERN_INFO "Removing Module\n");

  
  //Printing and then deleting every item in the list
  list_for_each_entry_safe(ptr, next, &birthday_list, list) {
    printk(KERN_INFO "Removing %d/%d/%d\n", ptr->day,
	   ptr->month, ptr->year);
    list_del(&ptr->list);
    kfree(ptr);
  }	
}


  


/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");

