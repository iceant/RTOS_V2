#ifndef INCLUDED_AI_BEHAIVORTREE_H
#define INCLUDED_AI_BEHAIVORTREE_H

////////////////////////////////////////////////////////////////////////////////
////

#ifndef INCLUDED_AI_ACTION_H
#include <ai_action.h>
#endif /*INCLUDED_AI_ACTION_H*/

#ifndef INCLUDED_AI_EVALUATOR_H
#include <ai_evaluator.h>
#endif /*INCLUDED_AI_EVALUATOR_H*/

#ifndef INCLUDED_SDK_VECTOR_H
#include <sdk_vector.h>
#endif /*INCLUDED_SDK_VECTOR_H*/

#ifndef INCLUDED_SDK_MACROS_H
#include <sdk_macros.h>
#endif /*INCLUDED_SDK_MACROS_H*/

////////////////////////////////////////////////////////////////////////////////
////
typedef enum ai_behaviortree_node_type_enum{
    kAI_BehaviorTreeNodeType_Action=0,
    kAI_BehaviorTreeNodeType_Condition,
    kAI_BehaviorTreeNodeType_Selector,
    kAI_BehaviorTreeNodeType_Sequence,
}ai_behaviortree_node_type_t;

typedef struct ai_behaviortree_node_s{
    ai_behaviortree_node_type_t type;
    char name[AI_NAME_SIZE];
    struct ai_behaviortree_node_s* parent;
    ai_action_t* action;
    ai_evaluator_t * evaluator;
    sdk_vector_t children;
}ai_behaviortree_node_t;

typedef struct ai_behaviortree_s{
    ai_behaviortree_node_t * current_node;
    ai_behaviortree_node_t * root;
    void* userdata;
}ai_behaviortree_t;

////////////////////////////////////////////////////////////////////////////////
////
int ai_behaviortree_node_init(ai_behaviortree_node_t* node, const char* name, ai_behaviortree_node_type_t type);

int ai_behaviortree_node_add_child(ai_behaviortree_node_t* self, ai_behaviortree_node_t* child, sdk_size_t idx);

int ai_behaviortree_node_append_child(ai_behaviortree_node_t* self, ai_behaviortree_node_t* child);

int ai_behaviortree_node_set_action(ai_behaviortree_node_t* self, ai_action_t* action);

int ai_behaviortree_node_set_evaluator(ai_behaviortree_node_t* self, ai_evaluator_t * evaluator);

int ai_behaviortree_init(ai_behaviortree_t* tree, void* userdata);

int ai_behaviortree_update(ai_behaviortree_t* tree, void* userdata);
////////////////////////////////////////////////////////////////////////////////
////

#define AI_BehaviorTree_Action(n, fnInit, fnUpdate, fnCleanup) \
    ai_action_t ai_action__##n;                                 \
    ai_action_init(&ai_action__##n, SDK_STRINGIFY(SDK_CAT(n, Action)), fnInit, fnUpdate, fnCleanup, 0); \
    ai_behaviortree_node_t ai_behaviortree_node__##n;          \
    ai_behaviortree_node_init(&ai_behaviortree_node__##n, #n, kAI_BehaviorTreeNodeType_Action);     \
    ai_behaviortree_node_set_action(&ai_behaviortree_node__##n, &ai_action__##n)

#define AI_BehaviorTree_Condition(n, fnEval) \
    ai_evaluator_t ai_evaluator__##n; \
    ai_evaluator_init(& ai_evaluator__##n, SDK_STRINGIFY(SDK_CAT(n, Evaluator)), fnEval, 0); \
    ai_behaviortree_node_t ai_behaviortree_node__##n;          \
    ai_behaviortree_node_init(&ai_behaviortree_node__##n, #n, kAI_BehaviorTreeNodeType_Condition);     \
    ai_behaviortree_node_set_evaluator(&ai_behaviortree_node__##n, &ai_evaluator__##n)

#define AI_BehaviorTree_Selector(n) \
    ai_behaviortree_node_t ai_behaviortree_node__##n;          \
    ai_behaviortree_node_init(&ai_behaviortree_node__##n, #n, kAI_BehaviorTreeNodeType_Selector)

#define AI_BehaviorTree_Sequence(n) \
    ai_behaviortree_node_t ai_behaviortree_node__##n;          \
    ai_behaviortree_node_init(&ai_behaviortree_node__##n, #n, kAI_BehaviorTreeNodeType_Sequence)
    
    
#define AI_BehaviorTree_AddSequenceChild(p, c) \
    AI_BehaviorTree_Sequence(c);               \
    ai_behaviortree_node_append_child(&ai_behaviortree_node__##p, &ai_behaviortree_node__##c)

#define AI_BehaviorTree_AddSelectorChild(p, c) \
    AI_BehaviorTree_Selector(c);               \
    ai_behaviortree_node_append_child(&ai_behaviortree_node__##p, &ai_behaviortree_node__##c)

#define AI_BehaviorTree_AddConditionChild(p, c, fnEval) \
    AI_BehaviorTree_Condition(c, fnEval);               \
    ai_behaviortree_node_append_child(&ai_behaviortree_node__##p, &ai_behaviortree_node__##c)

#define AI_BehaviorTree_AddActionChild(p, c, fnInit, fnUpdate, fnCleanup) \
    AI_BehaviorTree_Action(c,  fnInit, fnUpdate, fnCleanup);               \
    ai_behaviortree_node_append_child(&ai_behaviortree_node__##p, &ai_behaviortree_node__##c)
    
#endif /* INCLUDED_AI_BEHAIVORTREE_H */
