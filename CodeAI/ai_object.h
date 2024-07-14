#ifndef INCLUDED_AI_OBJECT_H
#define INCLUDED_AI_OBJECT_H

#ifndef INCLUDED_STDINT_H
#define INCLUDED_STDINT_H
#include <stdint.h>
#endif /*INCLUDED_STDINT_H*/

#ifndef INCLUDED_AI_DEFINITIONS_H
#include <ai_definitions.h>
#endif /*INCLUDED_AI_DEFINITIONS_H*/


////////////////////////////////////////////////////////////////////////////////
////

typedef enum ai_object_type_enum{
    kAI_ObjectType_Action = 0,
    kAI_ObjectType_Evaluator,
}ai_object_type_t;

typedef struct ai_object_s{
    char name[AI_NAME_SIZE];
    ai_object_type_t type;
    void* userdata;
}ai_object_t;

////////////////////////////////////////////////////////////////////////////////
////

int ai_object_init(ai_object_t* object, const char* name, ai_object_type_t type, void* userdata);

#endif /* INCLUDED_AI_OBJECT_H */
