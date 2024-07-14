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
    
    
    AI_DecisionTreeBranch_Evaluator(is_alive, Evaluator_Random);
    
    
    ai_decision_tree_set_branch(&tree, &ai_DecisionTreeBranch_Evaluator_is_alive);
    
    AI_DecisionTreeBranch_Evaluator(has_critical_health, Evaluator_Random);
    AI_DecisionTreeBranch_Action(die, Action_Initialize, Action_Update, Action_Exit);
    
    ai_decision_tree_branch_append_child(&ai_DecisionTreeBranch_Evaluator_is_alive, &ai_DecisionTreeBranch_Evaluator_has_critical_health);
    ai_decision_tree_branch_append_child(&ai_DecisionTreeBranch_Evaluator_is_alive, &ai_DecisionTreeBranch_Action_die);
    
    AI_DecisionTreeBranch_Evaluator(has_move_position, Evaluator_Random);
    AI_DecisionTreeBranch_Evaluator(has_enemy, Evaluator_Random);
    
    ai_decision_tree_branch_append_child(&ai_DecisionTreeBranch_Evaluator_has_critical_health, &ai_DecisionTreeBranch_Evaluator_has_move_position);
    ai_decision_tree_branch_append_child(&ai_DecisionTreeBranch_Evaluator_has_critical_health, &ai_DecisionTreeBranch_Evaluator_has_enemy);
    
    AI_DecisionTreeBranch_Action(move_to_position, Action_Initialize, Action_Update, Action_Cleanup);
    AI_DecisionTreeBranch_Action(flee, Action_Initialize, Action_Update, Action_Cleanup);
    
    ai_decision_tree_branch_append_child(&ai_DecisionTreeBranch_Evaluator_has_move_position, &ai_DecisionTreeBranch_Action_move_to_position);
    ai_decision_tree_branch_append_child(&ai_DecisionTreeBranch_Evaluator_has_move_position, &ai_DecisionTreeBranch_Action_flee);
    
    AI_DecisionTreeBranch_Evaluator(has_ammo, Evaluator_Random);
    ai_decision_tree_branch_append_child(&ai_DecisionTreeBranch_Evaluator_has_enemy, &ai_DecisionTreeBranch_Evaluator_has_ammo);
    ai_decision_tree_branch_append_child(&ai_DecisionTreeBranch_Evaluator_has_enemy, &ai_DecisionTreeBranch_Evaluator_has_move_position);
    
    AI_DecisionTreeBranch_Evaluator(can_shoot_enemy, Evaluator_Random);
    AI_DecisionTreeBranch_Evaluator(50_50_chance, Evaluator_Random);
    AI_DecisionTreeBranch_Action(reload, Action_Initialize, Action_Update, Action_Cleanup);
    AI_DecisionTreeBranch_Action(shoot, Action_Initialize, Action_Update, Action_Cleanup);
    AI_DecisionTreeBranch_Action(pursue, Action_Initialize, Action_Update, Action_Cleanup);
    AI_DecisionTreeBranch_Action(random_move, Action_Initialize, Action_Update, Action_Cleanup);
    AI_DecisionTreeBranch_Action(idle, Action_Initialize, Action_Update, Action_Cleanup);
    
    ai_decision_tree_branch_append_child(&ai_DecisionTreeBranch_Evaluator_has_ammo, &ai_DecisionTreeBranch_Evaluator_can_shoot_enemy);
    ai_decision_tree_branch_append_child(&ai_DecisionTreeBranch_Evaluator_has_ammo, &ai_DecisionTreeBranch_Action_reload);
    ai_decision_tree_branch_append_child(&ai_DecisionTreeBranch_Evaluator_can_shoot_enemy, &ai_DecisionTreeBranch_Action_shoot);
    ai_decision_tree_branch_append_child(&ai_DecisionTreeBranch_Evaluator_can_shoot_enemy, &ai_DecisionTreeBranch_Action_pursue);
    ai_decision_tree_branch_append_child(&ai_DecisionTreeBranch_Evaluator_50_50_chance, &ai_DecisionTreeBranch_Action_random_move);
    ai_decision_tree_branch_append_child(&ai_DecisionTreeBranch_Evaluator_50_50_chance, &ai_DecisionTreeBranch_Action_idle);
    ai_decision_tree_branch_append_child(&ai_DecisionTreeBranch_Evaluator_has_move_position, &ai_DecisionTreeBranch_Evaluator_50_50_chance);
    
    while(1){
        ai_decision_tree_update(&tree, 0);
    }
    
    return 0;
}