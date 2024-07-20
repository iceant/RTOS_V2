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
    int result =  rang_rand(2, 3);
    printf("Action[%s] Update, Return %d\n", action->object.name, result);
    return result;
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

#define AI_FSM_TRANSITION(F, T, Fn) \
    ai_fsm_transition_t F##$##T##_Transition; \
    ai_evaluator_t F##$##T##_Eval;       \
    ai_evaluator_init(&F##$##T##_Eval, STRINGIFY(F##$##T##_Eval), Fn, 0); \
    ai_fsm_transition_init(&F##$##T##_Transition, #T, &F##$##T##_Eval); \
    ai_fsm_state_add_transition(&ai_fsm__state_##F, &F##$##T##_Transition)

   
////////////////////////////////////////////////////////////////////////////////
////
static int Random_Evaluator(ai_evaluator_t* eval, void* ud)
{
    int ret =  rang_rand(0, 1);
    printf("Evaluator[%s] result = %d\n", eval->object.name, ret);
    return ret;
}

////////////////////////////////////////////////////////////////////////////////
////


int main(int argc, char** argv){
    srand(time(0));
    
    ai_fsm_t fsm;
    ai_fsm_init(&fsm, 0);
    
    AI_FSM_STATE(idle, Action_Initialize, Action_Update, Action_Cleanup);
    AI_FSM_STATE(die, Action_Initialize, Action_Update, Action_Exit);
    AI_FSM_STATE(flee, Action_Initialize, Action_Update, Action_Cleanup);
    AI_FSM_STATE(reload, Action_Initialize, Action_Update, Action_Cleanup);
    AI_FSM_STATE(shoot, Action_Initialize, Action_Update, Action_Cleanup);
    AI_FSM_STATE(pursue, Action_Initialize, Action_Update, Action_Cleanup);
    AI_FSM_STATE(randomMove, Action_Initialize, Action_Update, Action_Cleanup);
    AI_FSM_STATE(moveToPosition, Action_Initialize, Action_Update, Action_Cleanup);
    AI_FSM_STATE(move, Action_Initialize, Action_Update, Action_Cleanup);
    
    ai_fsm_add_state(&fsm, &ai_fsm__state_idle);
    ai_fsm_add_state(&fsm, &ai_fsm__state_die);
    ai_fsm_add_state(&fsm, &ai_fsm__state_flee);
    ai_fsm_add_state(&fsm, &ai_fsm__state_reload);
    ai_fsm_add_state(&fsm, &ai_fsm__state_shoot);
    ai_fsm_add_state(&fsm, &ai_fsm__state_pursue);
    ai_fsm_add_state(&fsm, &ai_fsm__state_randomMove);
    ai_fsm_add_state(&fsm, &ai_fsm__state_moveToPosition);
    ai_fsm_add_state(&fsm, &ai_fsm__state_move);
    
    AI_FSM_TRANSITION(idle, die, Random_Evaluator);
    AI_FSM_TRANSITION(idle, flee, Random_Evaluator);
    AI_FSM_TRANSITION(idle, reload, Random_Evaluator);
    AI_FSM_TRANSITION(idle, shoot, Random_Evaluator);
    AI_FSM_TRANSITION(idle, pursue, Random_Evaluator);
    AI_FSM_TRANSITION(idle, randomMove, Random_Evaluator);
    AI_FSM_TRANSITION(idle, idle, Random_Evaluator);
    
    AI_FSM_TRANSITION(moveToPosition, die, Random_Evaluator);
    AI_FSM_TRANSITION(moveToPosition, flee, Random_Evaluator);
    AI_FSM_TRANSITION(moveToPosition, reload, Random_Evaluator);
    AI_FSM_TRANSITION(moveToPosition, shoot, Random_Evaluator);
    AI_FSM_TRANSITION(moveToPosition, pursue, Random_Evaluator);
    AI_FSM_TRANSITION(moveToPosition, move, Random_Evaluator);
    AI_FSM_TRANSITION(moveToPosition, randomMove, Random_Evaluator);
    AI_FSM_TRANSITION(moveToPosition, idle, Random_Evaluator);
    
    AI_FSM_TRANSITION(randomMove, die, Random_Evaluator);
    AI_FSM_TRANSITION(randomMove, moveToPosition, Random_Evaluator);
    
    AI_FSM_TRANSITION(shoot, die, Random_Evaluator);
    AI_FSM_TRANSITION(shoot, flee, Random_Evaluator);
    AI_FSM_TRANSITION(shoot, reload, Random_Evaluator);
    AI_FSM_TRANSITION(shoot, shoot, Random_Evaluator);
    AI_FSM_TRANSITION(shoot, pursue, Random_Evaluator);
    AI_FSM_TRANSITION(shoot, randomMove, Random_Evaluator);
    AI_FSM_TRANSITION(shoot, idle, Random_Evaluator);
    
    AI_FSM_TRANSITION(flee, die, Random_Evaluator);
    AI_FSM_TRANSITION(flee, flee, Random_Evaluator);
    
    AI_FSM_TRANSITION(pursue, die, Random_Evaluator);
    AI_FSM_TRANSITION(pursue, flee, Random_Evaluator);
    AI_FSM_TRANSITION(pursue, shoot, Random_Evaluator);
    AI_FSM_TRANSITION(pursue, idle, Random_Evaluator);
    
    AI_FSM_TRANSITION(reload, die, Random_Evaluator);
    AI_FSM_TRANSITION(reload, shoot, Random_Evaluator);
    AI_FSM_TRANSITION(reload, pursue, Random_Evaluator);
    AI_FSM_TRANSITION(reload, randomMove, Random_Evaluator);
    AI_FSM_TRANSITION(reload, idle, Random_Evaluator);
    
    ai_fsm_set_state(&fsm, "idle");
    
    while(1){
        ai_fsm_update(&fsm, 0);
    }
    
    return 0;
}