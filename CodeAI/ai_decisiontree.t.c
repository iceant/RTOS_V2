#include <ai_decisiontree.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define AI_DecisionTreeBranch_Evaluator(n, fn) \
    ai_decision_tree_branch_t ai_DecisionTreeBranch_Evaluator_##n; \
    ai_decision_tree_branch_evaluator(&ai_DecisionTreeBranch_Evaluator_##n, #n, fn, 0)

#define AI_DecisionTreeBranch_Action(n, init, update, cleanup) \
    ai_decision_tree_branch_t ai_DecisionTreeBranch_Action_##n; \
    ai_decision_tree_branch_action(&ai_DecisionTreeBranch_Action_##n, #n, init, update, cleanup, 0)

#define AI_DecisionTree_AddEvaluator(T, n, fn) \
    AI_DecisionTreeBranch_Evaluator(n, fn); \
    ai_decision_tree_set_branch(&T, &ai_DecisionTreeBranch_Evaluator_##n)

#define AI_DecisionTreeBranch_AddEvaluator(P, C, fn) \
    AI_DecisionTreeBranch_Evaluator(C, fn);      \
    ai_decision_tree_branch_append_child(&ai_DecisionTreeBranch_Evaluator_##P, &ai_DecisionTreeBranch_Evaluator_##C)
    

#define AI_DecisionTreeBranch_AddAction(P, n, init, update, cleanup) \
    AI_DecisionTreeBranch_Action(n, init, update, cleanup);          \
    ai_decision_tree_branch_append_child(&ai_DecisionTreeBranch_Evaluator_##P, &ai_DecisionTreeBranch_Action_##n);


////////////////////////////////////////////////////////////////////////////////
////


int range_rand(int min, int max){
    return  rand() % (max - min + 1) + min;
}

static ai_action_status_t Action_Initialize(ai_action_t* action, void* userdata){
    printf("Action[%s].Initialize\n", action->object.name);
    return kAI_ActionStatus_RUNNING;
}

static ai_action_status_t Action_Update(ai_action_t* action, void* userdata){
    int result =  range_rand(2, 3);
    printf("Action[%s].Update, ret=%d\n", action->object.name, result);
    return result;
    
}

static ai_action_status_t Action_Cleanup(ai_action_t* action, void* userdata){
    printf("Action[%s].Cleanup\n", action->object.name);
    return kAI_ActionStatus_TERMINATED;
}

static ai_action_status_t Action_Exit(ai_action_t* action, void* userdata){
    printf("Action[%s].Exit\n", action->object.name);
    exit(0);
    return kAI_ActionStatus_TERMINATED;
}

static int Evaluator_Random(ai_evaluator_t* evaluator, void* userdata)
{
    int result =  range_rand(0, 1);
    printf("Evaluator[%s] = %d\n", evaluator->object.name, result);
    return result;
}

int main(int argc, char** argv)
{
    srand(time(0));
    
    ai_decision_tree_t tree;
    ai_decision_tree_init(&tree);
    
    
    AI_DecisionTree_AddEvaluator(tree, is_alive, Evaluator_Random);
    
    AI_DecisionTreeBranch_AddEvaluator(is_alive, has_critical_health, Evaluator_Random);
    AI_DecisionTreeBranch_AddAction(is_alive, die, Action_Initialize, Action_Update, Action_Exit);
    
    AI_DecisionTreeBranch_AddEvaluator(has_critical_health, has_move_position, Evaluator_Random);
    AI_DecisionTreeBranch_AddEvaluator(has_critical_health, has_enemy, Evaluator_Random);
    
    AI_DecisionTreeBranch_AddAction(has_move_position, move_to_position, Action_Initialize, Action_Update, Action_Cleanup);
    AI_DecisionTreeBranch_AddAction(has_move_position, flee, Action_Initialize, Action_Update, Action_Cleanup);
    
    AI_DecisionTreeBranch_AddEvaluator(has_enemy, has_ammo, Evaluator_Random);
    AI_DecisionTreeBranch_AddEvaluator(has_enemy, has_move_position2, Evaluator_Random);
    
    AI_DecisionTreeBranch_AddEvaluator(has_ammo, can_shoot_enemy, Evaluator_Random);
    AI_DecisionTreeBranch_AddAction(has_ammo, reload, Action_Initialize, Action_Update, Action_Cleanup);
    
    AI_DecisionTreeBranch_AddAction(can_shoot_enemy, shoot, Action_Initialize, Action_Update, Action_Cleanup);
    AI_DecisionTreeBranch_AddAction(can_shoot_enemy, pursue, Action_Initialize, Action_Update, Action_Cleanup);
    
    AI_DecisionTreeBranch_AddAction(has_move_position2, move_to_position2, Action_Initialize, Action_Update, Action_Cleanup);
    AI_DecisionTreeBranch_AddEvaluator(has_move_position2, 50_50_chance, Evaluator_Random);
    
    AI_DecisionTreeBranch_AddAction(50_50_chance, random_move, Action_Initialize, Action_Update, Action_Cleanup);
    AI_DecisionTreeBranch_AddAction(50_50_chance, idle, Action_Initialize, Action_Update, Action_Cleanup);
    
    
    while(1){
        ai_decision_tree_update(&tree, 0);
    }
    
    return 0;
}