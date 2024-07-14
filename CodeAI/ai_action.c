#include <ai_action.h>

int ai_action_init(ai_action_t* action, const char* name
        , ai_action_function_t initialize_function
        , ai_action_function_t update_function
        , ai_action_function_t cleanup_function
        , void* userdata
)
{
    ai_object_init(&action->object, name, kAI_ObjectType_Action, userdata);
    action->initialize_function = initialize_function;
    action->update_function = update_function;
    action->cleanup_function = cleanup_function;
    action->status = kAI_ActionStatus_UNINITIALIZED;
    
    return 0;
}

ai_action_status_t ai_action_initialize(ai_action_t* action, void* userdata){
    if(action->status == kAI_ActionStatus_UNINITIALIZED){
        if(action->initialize_function){
            action->initialize_function(action, userdata);
        }
    }
    action->status = kAI_ActionStatus_RUNNING;
    return action->status;
}

ai_action_status_t ai_action_update(ai_action_t* action, void* userdata){
    if(action->status==kAI_ActionStatus_TERMINATED){
        return action->status;
    }else if(action->status == kAI_ActionStatus_RUNNING){
        if(action->update_function){
            action->status = action->update_function(action, userdata);
        }else{
            action->status =  kAI_ActionStatus_TERMINATED;
        }
    }
    return action->status;
}

ai_action_status_t ai_action_cleanup(ai_action_t* action, void* userdata){
    if(action->status == kAI_ActionStatus_TERMINATED){
        if(action->cleanup_function){
            action->cleanup_function(action, userdata);
        }
    }
    action->status = kAI_ActionStatus_UNINITIALIZED;
}

