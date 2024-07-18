#include <ai_fsm.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
////////////////////////////////////////////////////////////////////////////////
////
static int rang_rand(int min, int max){
    return rand() % (max-min+1)+min;
}

static ai_action_status_t Action_Initialize(ai_action_t* action, void* ud){
    printf("Action[%s] Initialize\n", action->object.name);
    return kAI_ActionStatus_RUNNING;
}


static ai_action_status_t Action_Update(ai_action_t* action, void* ud){
    printf("Action[%s] Update\n", action->object.name);
    return rang_rand(2, 4);
}


static ai_action_status_t Action_Cleanup(ai_action_t* action, void* ud){
    printf("Action[%s] Cleanup\n", action->object.name);
    return kAI_ActionStatus_TERMINATED;
}

static ai_action_status_t Action_Exit(ai_action_t* action, void* ud){
    printf("Action[%s] Exit\n", action->object.name);
    exit(0);
    return kAI_ActionStatus_TERMINATED;
}
////////////////////////////////////////////////////////////////////////////////
////

#define STRINGIFY(x) STRINGIFY2(x)
#define STRINGIFY2(x) #x
#define CAT(a, b) CAT2(a, b)
#define CAT2(a, b) a##b

#define AI_FSM_STATE(n, fnI, fnU, fnC) \
    ai_fsm_state_t ai_fsm__state_##n;  \
    ai_action_t    ai_fsm__state_##n##_action; \
    ai_action_init(&ai_fsm__state_##n##_action, STRINGIFY(CAT(n, Action)), fnI, fnU, fnC, 0); \
    ai_fsm_state_init(&ai_fsm__state_##n, #n, &ai_fsm__state_##n##_action)

////////////////////////////////////////////////////////////////////////////////
////


int main(int argc, char** argv){
    srand(time(0));
    
    ai_fsm_t fsm;
    
    AI_FSM_STATE(idle, Action_Initialize, Action_Update, Action_Cleanup);
    AI_FSM_STATE(die, Action_Initialize, Action_Update, Action_Cleanup);
    AI_FSM_STATE(flee, Action_Initialize, Action_Update, Action_Cleanup);
    AI_FSM_STATE(reload, Action_Initialize, Action_Update, Action_Cleanup);
    AI_FSM_STATE(shoot, Action_Initialize, Action_Update, Action_Cleanup);
    AI_FSM_STATE(pursue, Action_Initialize, Action_Update, Action_Cleanup);
    AI_FSM_STATE(randomMove, Action_Initialize, Action_Update, Action_Cleanup);
    
    ai_fsm_transition_t transition_is_not_alive;
    ai_evaluator_t transition_is_not_alive_evaluator;
    ai_evaluator_init(&transition_is_not_alive_evaluator, "is not alive", );
    ai_fsm_transition_init(&transition_is_not_alive, "is not alive", );
    
    
    return 0l
}