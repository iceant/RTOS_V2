#ifndef INCLUDED_AI_FSM_H
#define INCLUDED_AI_FSM_H

////////////////////////////////////////////////////////////////////////////////
////

#ifndef INCLUDED_AI_OBJECT_H
#include <ai_object.h>
#endif /*INCLUDED_AI_OBJECT_H*/

#ifndef INCLUDED_AI_ACTION_H
#include <ai_action.h>
#endif /*INCLUDED_AI_ACTION_H*/

#ifndef INCLUDED_AI_EVALUATOR_H
#include <ai_evaluator.h>
#endif /*INCLUDED_AI_EVALUATOR_H*/

#ifndef INCLUDED_SDK_VECTOR_H
#include <sdk_vector.h>
#endif /*INCLUDED_SDK_VECTOR_H*/



////////////////////////////////////////////////////////////////////////////////
////
typedef struct ai_fsm_transition_s{
    char to_state_name[AI_NAME_SIZE];
    ai_evaluator_t * evaluator;
}ai_fsm_transition_t;

typedef struct ai_fsm_state_s{
    char name[AI_NAME_SIZE];
    ai_action_t* action;
    sdk_vector_t transitions;
}ai_fsm_state_t;

typedef struct ai_fsm_s{
    ai_fsm_state_t * current_state;
    sdk_vector_t states;
    void* userdata;
}ai_fsm_t;

////////////////////////////////////////////////////////////////////////////////
////

int ai_fsm_state_init(ai_fsm_state_t* state, const char* name, ai_action_t* action);

int ai_fsm_transition_init(ai_fsm_transition_t * transition, const char* to_state_name, ai_evaluator_t* evaluator);

int ai_fsm_state_add_transition(ai_fsm_state_t* state, ai_fsm_transition_t* transition);

int ai_fsm_init(ai_fsm_t* fsm, void* userdata);

sdk_bool_t ai_fsm_contain_state(ai_fsm_t* fsm, const char* name);

ai_fsm_state_t * ai_fsm_find_state(ai_fsm_t * fsm, const char* state_name);

ai_fsm_transition_t * ai_fsm_find_transition(ai_fsm_t* fsm, const char* from_state_name, const char* to_state_name);

int ai_fsm_set_state(ai_fsm_t* fsm, const char* state_name);

int ai_fsm_add_state(ai_fsm_t* fsm, ai_fsm_state_t* state);

void ai_fsm_update(ai_fsm_t* fsm, void* userdata);

#endif /* INCLUDED_AI_FSM_H */
