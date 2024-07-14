#ifndef INCLUDED_AI_ACTION_H
#define INCLUDED_AI_ACTION_H

////////////////////////////////////////////////////////////////////////////////
////

#ifndef INCLUDED_AI_OBJECT_H
#include <ai_object.h>
#endif /*INCLUDED_AI_OBJECT_H*/

////////////////////////////////////////////////////////////////////////////////
////

typedef enum ai_action_status_enum{
    kAI_ActionStatus_UNKNOWN = 0,
    kAI_ActionStatus_UNINITIALIZED=1,
    kAI_ActionStatus_RUNNING=2,
    kAI_ActionStatus_TERMINATED=3,
}ai_action_status_t;

typedef struct ai_action_s ai_action_t;

typedef ai_action_status_t (*ai_action_function_t)(ai_action_t* action, void* userdata);

struct ai_action_s{
    ai_object_t object;
    ai_action_status_t status;
    ai_action_function_t initialize_function;
    ai_action_function_t update_function;
    ai_action_function_t cleanup_function;
};

////////////////////////////////////////////////////////////////////////////////
////

int ai_action_init(ai_action_t* action, const char* name
                   , ai_action_function_t initialize_function
                   , ai_action_function_t update_function
                   , ai_action_function_t cleanup_function
                   , void* userdata
                   );

ai_action_status_t ai_action_initialize(ai_action_t* action, void* userdata);

ai_action_status_t ai_action_update(ai_action_t* action, void* userdata);

ai_action_status_t ai_action_cleanup(ai_action_t* action, void* userdata);

#endif /* INCLUDED_AI_ACTION_H */
