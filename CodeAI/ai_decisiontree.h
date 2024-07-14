#ifndef INCLUDED_AI_DECISIONTREE_H
#define INCLUDED_AI_DECISIONTREE_H

////////////////////////////////////////////////////////////////////////////////
////

#ifndef INCLUDED_AI_OBJECT_H
#include <ai_object.h>
#endif /*INCLUDED_AI_OBJECT_H*/

#ifndef INCLUDED_SDK_VECTOR_H
#include <sdk_vector.h>
#endif /*INCLUDED_SDK_VECTOR_H*/

#ifndef INCLUDED_AI_EVALUATOR_H
#include <ai_evaluator.h>
#endif /*INCLUDED_AI_EVALUATOR_H*/

#ifndef INCLUDED_AI_ACTION_H
#include <ai_action.h>
#endif /*INCLUDED_AI_ACTION_H*/


////////////////////////////////////////////////////////////////////////////////
////

typedef enum ai_decision_tree_branch_type_enum{
    kAI_DecisionTreeBranchType_Branch = 0 ,
    kAI_DecisionTreeBranchType_Action = 1 ,
}ai_decision_tree_branch_type_t;

typedef struct ai_decision_tree_branch_s{
    ai_decision_tree_branch_type_t type;
    sdk_vector_t children;
    ai_evaluator_t evaluator;
    ai_action_t action;
}ai_decision_tree_branch_t;

typedef struct ai_decision_tree_s{
    ai_decision_tree_branch_t * branch;
    ai_action_t * current_action;
}ai_decision_tree_t;

////////////////////////////////////////////////////////////////////////////////
////

int ai_decision_tree_branch_init(ai_decision_tree_branch_t * branch, ai_decision_tree_branch_type_t type);
int ai_decision_tree_branch_set_evaluator(ai_decision_tree_branch_t* branch, const char* name, ai_evaluator_function_t function, void* userdata);
int ai_decision_tree_branch_set_action(ai_decision_tree_branch_t* branch, const char* name
                                       , ai_action_function_t initialize_function
                                       , ai_action_function_t update_function
                                       , ai_action_function_t cleanup_function
                                       , void* userdata);
int ai_decision_tree_branch_append_child(ai_decision_tree_branch_t* branch, ai_decision_tree_branch_t* child);
int ai_decision_tree_branch_add_child(ai_decision_tree_branch_t* branch,sdk_size_t idx, ai_decision_tree_branch_t* child);
int ai_decision_tree_branch_evaluate(ai_decision_tree_branch_t* branch, void* userdata, ai_decision_tree_branch_t** result);
void ai_decision_tree_branch_destroy(ai_decision_tree_branch_t* branch);

int ai_decision_tree_branch_evaluator(ai_decision_tree_branch_t* branch, const char* name, ai_evaluator_function_t function, void* userdata);
int ai_decision_tree_branch_action(ai_decision_tree_branch_t* branch, const char* name
                , ai_action_function_t initialize_function
                , ai_action_function_t update_function
                , ai_action_function_t cleanup_function
                , void* userdata);

int ai_decision_tree_init(ai_decision_tree_t* tree);
int ai_decision_tree_set_branch(ai_decision_tree_t* tree, ai_decision_tree_branch_t* branch);
int ai_decision_tree_update(ai_decision_tree_t* tree, void* userdata);

#endif /* INCLUDED_AI_DECISIONTREE_H */
