#include <ai_object.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
////

int ai_object_init(ai_object_t* object, const char* name, ai_object_type_t type, void* userdata) {
    
    size_t name_size = strlen(name);
    name_size = (name_size > (sizeof(object->name) - 1 ))?(sizeof(object->name) -1):name_size;
    memcpy(object->name, name, name_size);
    object->name[name_size]= '\0';
    
    object->type = type;
    object->userdata = userdata;
    
    return 0;
}

