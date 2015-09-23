/*
 * concurrent_linked_list_test.cpp
 *
 * Testing the concurrent_linked_list.h implementation.
 *
 */

#include <algorithm>
#include <iostream>

#include <cstdlib>
#include <cilk/cilk.h>

// Included from util directory. 
#include <check_sort.h>
#include "concurrent_linked_list.h"

// Function repeated mallocs an array of size 100, increments the pth
// element of that array, and then frees the array.
void repeated_malloc_free(unsigned int n, int p) {
    int z = 0;
  
    for(unsigned int i = 0; i < n; i++) {
        int* f;
        f = (int*)malloc(sizeof(int)*100);
        f[p]++;
        z += f[p];
        free(f);
    }
    printf("z is %d\n", z);
}


// Checks whether the list L contains the elements [0, 1, ..R-1].
bool check_list_insert(ConcurrentLinkedList* L, int R) {
    int final_size = 0;
    long long* a = L->get_keys(&final_size);
    std::sort(a, a+final_size);
    check_sort(a, final_size);
    assert(final_size == R);
  
    delete [] a;
    return true;
}


// Tries to insert elements [0, 1, .. R-1] into the list. 
void all_list_insert(ConcurrentLinkedList* L, int R) {

    int statusCounts[OP_LAST];
  
    for (int i = OP_NULL; i < OP_LAST; i++) {
        statusCounts[i] = 0;
    }

    //  printf("Adding remainder\n");
    for (int i = 0; i < R; i++) {
        LOpStatus code = OP_FAILED;
        void* return_val;

        // Retry insert until return code is not OP_FAILED.
        while (code == OP_FAILED) {
	    // Cast the int to a larger type.
	    // Yes, we really meant to convert an int into a void*
	    void* val = (void*)(size_t)i;
            return_val = L->insert_if_absent(i,
					     val, 
                                             &code);
            statusCounts[code]++;
        }
	void* final_val = (void*)(size_t)i;
        assert(return_val == final_val);
    }

    //  L->print_list();
    printf("Final code results: ");
    for (int i = OP_NULL; i < OP_LAST; i++) {
        printf("(%d, %d)  ",
               i, statusCounts[i]);
    }
    printf("\n");
}

// Tries to insert n random elements into L, with each element being
// chosen uniformly from the range [0 .. R-1].
void test_list_insert(ConcurrentLinkedList* L, int R, int n) {

    int statusCounts[OP_LAST];
    for (int i = OP_NULL; i < OP_LAST; i++) {
        statusCounts[i] = 0;
    }
  
    printf("Test list insert: R = %d, n = %d\n",
           R, n);
 
    for (int i = 0; i < n; i++) {


        int rand_num = rand() % R;
	void* rand_val = (void*)(size_t)rand_num;
        LOpStatus code = OP_FAILED;
        void* return_val;



        while (code == OP_FAILED) {
            return_val = L->insert_if_absent(rand_num,
					     rand_val, 
                                             &code);
            statusCounts[code]++;
        }
        assert(return_val == rand_val);
    }
  
    printf("Code results: ");
    for (int i = OP_NULL; i < OP_LAST; i++) {
        printf("(%d, %d)  ",
               i, statusCounts[i]);
    }
    printf("\n");
    //  L->print_list();
}


int main(int argc, char *argv[])
{

     int R = 10000;
     if (argc >= 2) {
       R = atoi(argv[1]);
     }
     printf("Value of R: %d\n", R);


     ConcurrentLinkedList* L = new ConcurrentLinkedList();
     printf("An empty linked list\n");
     L->print_list();

     for (int i = 0; i < 20; i++) {
       cilk_spawn test_list_insert(L, R, 5*R);
     }
     cilk_sync;

     all_list_insert(L, R);
     check_list_insert(L, R);
     
     printf("Deleting list: \n");
     delete L;
     printf("Done with delete\n");
     printf("PASSED\n");
     
     return 0;
}

