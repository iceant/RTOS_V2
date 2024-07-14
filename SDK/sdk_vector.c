#include <sdk_vector.h>
#include <sdk_memory.h>

////////////////////////////////////////////////////////////////////////////////
////
#define ELEMENT_SIZE sizeof(void*)

sdk_err_t sdk_vector_init(sdk_vector_t* vector, sdk_size_t capacity)
{
    vector->capacity = capacity;
    vector->size = 0;
    
    if(capacity>0){
        vector->items = SDK_ALLOC(ELEMENT_SIZE * capacity);
        if(!vector->items){
            return SDK_ENOMEM;
        }
    }else{
        vector->items = 0;
    }
    
    return SDK_EOK;
}

void sdk_vector_destroy(sdk_vector_t * vector)
{
    SDK_FREE(vector->items);
}

sdk_err_t sdk_vector_resize(sdk_vector_t* vector, sdk_size_t new_capacity)
{
    if(new_capacity==0){
        SDK_FREE(vector->items);
        vector->size = 0;
    }else if(new_capacity > vector->capacity){
        void* items = SDK_ALLOC(ELEMENT_SIZE * new_capacity);
        if(!items){
            return SDK_ENOMEM;
        }
        memcpy(items, vector->items, ELEMENT_SIZE * vector->size);
        SDK_FREE(vector->items);
        vector->items = items;
    }else{
        SDK_RESIZE(vector->items, ELEMENT_SIZE * new_capacity);
        if(!vector->items){
            return SDK_ENOMEM;
        }
        if(vector->size < new_capacity){
            vector->size = new_capacity;
        }
    }
    
    vector->capacity = new_capacity;
    
    return SDK_EOK;
}

sdk_err_t sdk_vector_resize_append(sdk_vector_t* vector, void* item){
    if(vector->size==vector->capacity){
        sdk_err_t  err = sdk_vector_resize(vector, (vector->capacity==0)?1:(2*vector->capacity));
        if(err!=SDK_EOK){
            return err;
        }
    }
    vector->items[vector->size++] = item;
    return SDK_EOK;
}

void* sdk_vector_get(sdk_vector_t* vector, sdk_size_t idx)
{
    if(idx>=vector->size){
        return 0;
    }
    
    return vector->items[idx];
}

sdk_err_t sdk_vector_remove(sdk_vector_t* vector, sdk_size_t idx)
{
    if(idx>=vector->size){
        return SDK_ENOMEM;
    }
    
    sdk_size_t i;
    for(i=idx; i<(vector->size-1); i++){
        vector->items[i] = vector->items[i+1];
    }
    vector->items[i] = 0;
    vector->size--;
    
    return SDK_EOK;
}

sdk_err_t sdk_vector_resize_remove(sdk_vector_t* vector, sdk_size_t idx)
{
    if(idx>=vector->size){
        return SDK_ENOMEM;
    }
    
    sdk_size_t i;
    for(i=idx; i<(vector->size-1); i++){
        vector->items[i] = vector->items[i+1];
    }
    vector->items[i] = 0;
    vector->size--;
    
    if(vector->size <= vector->capacity/4){
        sdk_err_t err = sdk_vector_resize(vector, vector->capacity/2);
        if(err!=SDK_EOK){
            return err;
        }
    }
    
    return SDK_EOK;
}

sdk_err_t sdk_vector_resize_add(sdk_vector_t *vector, sdk_size_t idx, void* item)
{
    if(vector->size == vector->capacity){
        sdk_err_t err = sdk_vector_resize(vector, (vector->capacity==0)?1:(vector->capacity*2));
        if(err!=SDK_EOK){
            return err;
        }
    }
    
    if(idx>=vector->size){
        return SDK_EINVAL;
    }
    sdk_size_t i;
    for(i=vector->size; i>idx; i--){
        vector->items[i] = vector->items[i-1];
    }
    
    vector->items[i] = item;
    vector->size++;
    
    return SDK_EOK;
}
