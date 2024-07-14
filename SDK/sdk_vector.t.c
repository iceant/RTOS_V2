#include <sdk_vector.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv){
    sdk_vector_t v1;
    sdk_vector_init(&v1, 0);
    
    for(int i=0; i<10; i++){
        sdk_vector_resize_append(&v1, (void*)(i+10));
    }
    
    sdk_vector_resize_add(&v1, 2, (void*)120);
    
    for(int i=0; i<v1.size; i++){
        int value = (int)sdk_vector_get(&v1, i);
        printf("[%d] %d\n", i, value);
    }
    

    
    while(v1.size){
        sdk_vector_resize_remove(&v1, 0);
        printf("v.size=%d, v.capacity=%d\n", v1.size, v1.capacity);
    }
    
    
    
    sdk_vector_destroy(&v1);
    
    return 0;
}