#include <ai_behaivortree.h>
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

static int Random_Evaluator(ai_evaluator_t* eval, void* ud)
{
    int ret =  rang_rand(0, 1);
    printf("Evaluator[%s] result = %d\n", eval->object.name, ret);
    return ret;
}


int main(int argc, char** argv)
{
    srand(time(0));
    
    ai_behaviortree_t tree;
    ai_behaviortree_init(&tree, 0);
    
    AI_BehaviorTree_Selector(root);
    tree.root = &ai_behaviortree_node__root;
    
    AI_BehaviorTree_AddSequenceChild(root, die_sequence);
    AI_BehaviorTree_AddSequenceChild(root, flee_sequence);
    AI_BehaviorTree_AddSequenceChild(root, has_enemy_sequence);
    AI_BehaviorTree_AddSequenceChild(root, has_move_position_sequence);
    AI_BehaviorTree_AddSequenceChild(root, 50_50_chance_sequence);
    AI_BehaviorTree_AddActionChild(root, idle, Action_Initialize, Action_Update, Action_Cleanup);
    
    AI_BehaviorTree_AddConditionChild(die_sequence, is_not_alive, Random_Evaluator);
    AI_BehaviorTree_AddActionChild(die_sequence, die, Action_Initialize, Action_Update, Action_Exit);
    
    AI_BehaviorTree_AddConditionChild(flee_sequence, has_critical_health, Random_Evaluator);
    AI_BehaviorTree_AddActionChild(flee_sequence, flee, Action_Initialize, Action_Update, Action_Cleanup);
    
    
    AI_BehaviorTree_AddConditionChild(has_enemy_sequence, has_enemy, Random_Evaluator);
    AI_BehaviorTree_AddSelectorChild(has_enemy_sequence, reload_shoot_pursue_selector);
    
    AI_BehaviorTree_AddSequenceChild(reload_shoot_pursue_selector, reload_sequence);
    AI_BehaviorTree_AddSequenceChild(reload_shoot_pursue_selector, shoot_sequence);
    AI_BehaviorTree_AddActionChild(reload_shoot_pursue_selector, pursue, Action_Initialize, Action_Update, Action_Cleanup);
    
    AI_BehaviorTree_AddConditionChild(reload_sequence, has_no_ammo, Random_Evaluator);
    AI_BehaviorTree_AddActionChild(reload_sequence, reload, Action_Initialize, Action_Update, Action_Cleanup);
    
    AI_BehaviorTree_AddConditionChild(shoot_sequence, can_shoot_enemy, Random_Evaluator);
    AI_BehaviorTree_AddActionChild(shoot_sequence, shoot, Action_Initialize, Action_Update, Action_Cleanup);
    
    AI_BehaviorTree_AddConditionChild(has_move_position_sequence, has_move_position, Random_Evaluator);
    AI_BehaviorTree_AddActionChild(has_move_position_sequence, move_to_position, Action_Initialize, Action_Update, Action_Cleanup);
    
    
    AI_BehaviorTree_AddConditionChild(50_50_chance_sequence, 50_50_chance, Random_Evaluator);
    AI_BehaviorTree_AddActionChild(50_50_chance_sequence, random_move, Action_Initialize, Action_Update, Action_Cleanup);
    
    while(1){
        ai_behaviortree_update(&tree, 0);
    }
    
    return 0;
}
