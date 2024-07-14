#include <ai_decisiontree.h>

int ai_decision_tree_branch_init(ai_decision_tree_branch_t * branch
                                 , ai_decision_tree_branch_type_t type)
{
    branch->type = type;
    sdk_vector_init(&branch->children, 0);
    return 0;
}

void ai_decision_tree_branch_destroy(ai_decision_tree_branch_t* branch)
{
    sdk_vector_destroy(&branch->children);
}

int ai_decision_tree_branch_add_child(ai_decision_tree_branch_t* branch, sdk_size_t idx, ai_decision_tree_branch_t* child)
{
    return sdk_vector_resize_add(&branch->children, idx, child);
}

int ai_decision_tree_branch_append_child(ai_decision_tree_branch_t* branch, ai_decision_tree_branch_t* child)
{
    return sdk_vector_resize_append(&branch->children, child);
}

int ai_decision_tree_branch_set_evaluator(ai_decision_tree_branch_t* branch, const char* name, ai_evaluator_function_t function, void* userdata)
{
    return ai_evaluator_init(&branch->evaluator, name, function, userdata);
}

int ai_decision_tree_branch_set_action(ai_decision_tree_branch_t* branch, const char* name
        , ai_action_function_t initialize_function
        , ai_action_function_t update_function
        , ai_action_function_t cleanup_function
        , void* userdata)
{
    return ai_action_init(&branch->action, name, initialize_function, update_function, cleanup_function, userdata);
}

int ai_decision_tree_branch_evaluate(ai_decision_tree_branch_t* branch, void* userdata, ai_decision_tree_branch_t** result){
    int eval = ai_evaluator_evaluate(&branch->evaluator, userdata);
    ai_decision_tree_branch_t * choice = sdk_vector_get(&branch->children, eval);
    if(choice->type==kAI_DecisionTreeBranchType_Branch){
        return ai_decision_tree_branch_evaluate(choice, userdata, result);
    }else{
        if(result){
            *result = choice;
        }
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
////

int ai_decision_tree_init(ai_decision_tree_t* tree)
{
    tree->branch = 0;
    tree->current_action = 0;
    return 0;
}

int ai_decision_tree_set_branch(ai_decision_tree_t* tree, ai_decision_tree_branch_t* branch)
{
    tree->branch = branch;
    return 0;
}

int ai_decision_tree_update(ai_decision_tree_t* tree, void* userdata)
{
    if(tree->branch == 0){
        return 0;
    }
    
    ai_decision_tree_branch_t *action_branch;
    
    if(tree->current_action==0){
        ai_decision_tree_branch_evaluate(tree->branch, userdata, &action_branch);
        tree->current_action = &action_branch->action;
        ai_action_initialize(tree->current_action, userdata);
    }
    
    ai_action_status_t status = ai_action_update(tree->current_action, userdata);
    if(status==kAI_ActionStatus_TERMINATED){
        ai_action_cleanup(tree->current_action, userdata);
        tree->current_action = 0;
    }
    
    return 0;
}

int ai_decision_tree_branch_evaluator(ai_decision_tree_branch_t* branch, const char* name, ai_evaluator_function_t function, void* userdata)
{
    ai_decision_tree_branch_init(branch, kAI_DecisionTreeBranchType_Branch);
    return ai_decision_tree_branch_set_evaluator(branch, name, function, userdata);
}

int ai_decision_tree_branch_action(ai_decision_tree_branch_t* branch, const char* name
        , ai_action_function_t initialize_function
        , ai_action_function_t update_function
        , ai_action_function_t cleanup_function
        , void* userdata)
{
    ai_decision_tree_branch_init(branch, kAI_DecisionTreeBranchType_Action);
    return ai_decision_tree_branch_set_action(branch, name, initialize_function, update_function, cleanup_function, userdata);
}
