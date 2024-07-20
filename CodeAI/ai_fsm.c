#include <ai_fsm.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
////

static const char* ai_fsm__evaluate_transitions(ai_fsm_t* fsm, sdk_vector_t * transitions, void* userdata){
    sdk_size_t i;
    for(i=0; i<transitions->size; i++){
        ai_fsm_transition_t * t = sdk_vector_get(transitions, i);
        if(ai_evaluator_evaluate(t->evaluator, userdata)){
            return t->to_state_name;
        }
    }
    return 0;
}
////////////////////////////////////////////////////////////////////////////////
////

int ai_fsm_state_init(ai_fsm_state_t* state, const char* name, ai_action_t* action)
{
    sdk_size_t name_size = strlen(name);
    name_size = (name_size > (sizeof(state->name)-1))?(sizeof(state->name)-1):name_size;
    memcpy(state->name, name, name_size);
    state->name[name_size]='\0';
    
    state->action = action;
    sdk_vector_init(&state->transitions, 0);
    
    return 0;
}

int ai_fsm_transition_init(ai_fsm_transition_t * transition, const char* to_state_name, ai_evaluator_t* evaluator)
{
    sdk_size_t name_size = strlen(to_state_name);
    name_size = (name_size > (sizeof(transition->to_state_name)-1))?(sizeof(transition->to_state_name)-1):name_size;
    memcpy(transition->to_state_name, to_state_name, name_size);
    transition->to_state_name[name_size]='\0';
    
    transition->evaluator = evaluator;
    
    return 0;
}

int ai_fsm_state_add_transition(ai_fsm_state_t* state, ai_fsm_transition_t* transition)
{
    return sdk_vector_resize_append(&state->transitions, transition);
}

int ai_fsm_init(ai_fsm_t* fsm, void* userdata)
{
    fsm->current_state = 0;
    fsm->userdata = userdata;
    return sdk_vector_init(&fsm->states, 0);
}

sdk_bool_t ai_fsm_contain_state(ai_fsm_t* fsm, const char* name){
    for(sdk_size_t i=0 ;i<fsm->states.size; i++){
        ai_fsm_state_t * state = sdk_vector_get(&fsm->states, i);
        if(strcmp(name, state->name)==0){
            return SDK_TRUE;
        }
    }
    return SDK_FALSE;
}

ai_fsm_state_t * ai_fsm_find_state(ai_fsm_t * fsm, const char* state_name){
    for(sdk_size_t i=0 ;i<fsm->states.size; i++){
        ai_fsm_state_t * state = sdk_vector_get(&fsm->states, i);
        if(strcmp(state_name, state->name)==0){
            return state;
        }
    }
    return 0;
}

ai_fsm_transition_t * ai_fsm_find_transition(ai_fsm_t* fsm, const char* from_state_name, const char* to_state_name){
    ai_fsm_state_t * from_state = ai_fsm_find_state(fsm, from_state_name);
    if(from_state==0){
        return 0;
    }
    
    for(sdk_size_t i=0; i<from_state->transitions.size; i++){
        ai_fsm_transition_t * p = sdk_vector_get(&from_state->transitions, i);
        if(strcmp(p->to_state_name, to_state_name)==0){
            return p;
        }
    }
    return 0;
}

int ai_fsm_set_state(ai_fsm_t* fsm, const char* state_name){
    ai_fsm_state_t * state = ai_fsm_find_state(fsm, state_name);
    if(state==0){
        return -1;
    }
    
    if(fsm->current_state){
        ai_action_cleanup(fsm->current_state->action, 0);
    }
    fsm->current_state = state;
    ai_action_initialize(fsm->current_state->action, 0);
    return 0;
}

int ai_fsm_add_state(ai_fsm_t* fsm, ai_fsm_state_t* state){
    return sdk_vector_resize_append(&fsm->states, state);
}

void ai_fsm_update(ai_fsm_t* fsm, void* userdata){
    if(fsm->current_state){
        ai_action_status_t status = fsm->current_state->action->status;
        if(status==kAI_ActionStatus_RUNNING){
            ai_action_update(fsm->current_state->action, userdata);
        }else if(status == kAI_ActionStatus_TERMINATED){
            ai_action_cleanup(fsm->current_state->action, userdata);
            
            const char* to_state_name = ai_fsm__evaluate_transitions(fsm, &fsm->current_state->transitions, userdata);
            if(to_state_name){
                ai_fsm_set_state(fsm, to_state_name);
                ai_action_initialize(fsm->current_state->action, userdata);
            }
        }else if(status==kAI_ActionStatus_UNINITIALIZED){
            const char* to_state_name = ai_fsm__evaluate_transitions(fsm, &fsm->current_state->transitions, userdata);
            if(to_state_name){
                ai_fsm_set_state(fsm, to_state_name);
                ai_action_initialize(fsm->current_state->action, userdata);
            }
        }
    }
}
