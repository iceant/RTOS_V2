#include <ai_behaivortree.h>
#include <string.h>
#include <assert.h>
////////////////////////////////////////////////////////////////////////////////
////

int ai_behaviortree_node_init(ai_behaviortree_node_t* node, const char* name, ai_behaviortree_node_type_t type) {
    sdk_size_t name_size = strlen(name);
    name_size = (name_size > (sizeof(node->name) - 1)) ? (sizeof(node->name) - 1) : name_size;
    memcpy(node->name, name, name_size);
    node->name[name_size] = '\0';
    node->type = type;
    node->parent = 0;
    node->action = 0;
    node->evaluator = 0;
    sdk_vector_init(&node->children, 0);
    return 0;
}

int ai_behaviortree_node_add_child(ai_behaviortree_node_t* self, ai_behaviortree_node_t* child, sdk_size_t idx)
{
    return sdk_vector_resize_add(&self->children, idx, child);
}

int ai_behaviortree_node_append_child(ai_behaviortree_node_t* self, ai_behaviortree_node_t* child){
    return sdk_vector_resize_append(&self->children, child);
}

int ai_behaviortree_node_set_action(ai_behaviortree_node_t* self, ai_action_t* action){
    self->action = action;
    return 0;
}

int ai_behaviortree_node_set_evaluator(ai_behaviortree_node_t* self, ai_evaluator_t * evaluator){
    self->evaluator = evaluator;
    return 0;
}

int ai_behaviortree_init(ai_behaviortree_t* tree, void* userdata)
{
    tree->current_node = 0;
    tree->root = 0;
    tree->userdata  = userdata;
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
////
typedef struct ai_behaviortree_result_s{
    sdk_bool_t result;
    ai_behaviortree_node_t * node;
}ai_behaviortree_result_t;

#define AI_BehaviorTreeResult(r) ai_behaviortree_result_t r={.result=SDK_FALSE, .node=0}

static int ai_behaviortree__evaluate_selector(ai_behaviortree_t* self, ai_behaviortree_node_t* node
                                              , void* userdata, ai_behaviortree_result_t* result);
static int ai_behaviortree__evaluate_sequence(ai_behaviortree_t* self, ai_behaviortree_node_t* node
                                              , void* userdata, sdk_size_t idx, ai_behaviortree_result_t* result);

static int ai_behaviortree__evaluate_selector(ai_behaviortree_t* self, ai_behaviortree_node_t* node, void* userdata, ai_behaviortree_result_t* result)
{
    sdk_size_t  i;
    for(i=0; i<node->children.size; i++){
        ai_behaviortree_node_t * n = sdk_vector_get(&node->children, i);
        if(n!=0){
            if(n->type == kAI_BehaviorTreeNodeType_Action){
                result->node = n;
                result->result = SDK_TRUE;
                return 0;
            }else if(n->type == kAI_BehaviorTreeNodeType_Condition){
                result->result = SDK_FALSE;
                return 0;
            }else if(n->type == kAI_BehaviorTreeNodeType_Selector){
                ai_behaviortree__evaluate_selector(self, n, userdata, result);
                
                if(result->result==SDK_TRUE){
                    return 0;
                }
            }else if(n->type == kAI_BehaviorTreeNodeType_Sequence){
                ai_behaviortree__evaluate_sequence(self, n, userdata, 0, result);
                if(result->result==SDK_TRUE){
                    return 0;
                }
            }
        }
    }
    result->result = SDK_FALSE;
    return 0;
}

static int ai_behaviortree__evaluate_sequence(ai_behaviortree_t* self, ai_behaviortree_node_t* node, void* userdata
                                              , sdk_size_t idx, ai_behaviortree_result_t* result)
{
    
    sdk_size_t i;
    for(i=idx; i<node->children.size; i++){
        ai_behaviortree_node_t *child = sdk_vector_get(&node->children, i);
        if(child->type == kAI_BehaviorTreeNodeType_Action){
            result->node = child;
            result->result = SDK_TRUE;
            return 0;
        }else if(child->type == kAI_BehaviorTreeNodeType_Condition){
            int eval_result = ai_evaluator_evaluate(child->evaluator, userdata);
            if(!eval_result){
                result->result = SDK_FALSE;
                return 0;
            }
        }else if(child->type == kAI_BehaviorTreeNodeType_Selector){
            ai_behaviortree__evaluate_selector(self, child, userdata, result);
            if(!result->result){
                result->result = SDK_FALSE;
                return 0;
            }else if(result->result && result->node!=0){
                return 0;
            }
        }else if(child->type==kAI_BehaviorTreeNodeType_Sequence){
            ai_behaviortree__evaluate_sequence(self, child, userdata, 0, result);
            if(!result->result){
                result->result = SDK_FALSE;
                return 0;
            }else if(result->result && result->node!=0){
                return 0;
            }
        }
    }
    result->result = SDK_TRUE;
    return 0;
}

static ai_behaviortree_node_t * ai_behaviortree__evaluate_node(ai_behaviortree_t* self, ai_behaviortree_node_t * node, void* userdata){
    if(node->type == kAI_BehaviorTreeNodeType_Action){
        return node;
    }else if(node->type==kAI_BehaviorTreeNodeType_Condition){
        assert(false);
    }else if(node->type==kAI_BehaviorTreeNodeType_Selector){
        ai_behaviortree_result_t result;
        ai_behaviortree__evaluate_selector(self, node, userdata, &result);
        if(result.result){
            return result.node;
        }
    }else if(node->type==kAI_BehaviorTreeNodeType_Sequence){
        ai_behaviortree_result_t result;
        ai_behaviortree__evaluate_sequence(self, node, userdata, 0, &result);
        if(result.result){
            return result.node;
        }
    }
    
    return 0;
}

static sdk_size_t ai_behaviortree_node_get_child_index(ai_behaviortree_node_t* node, ai_behaviortree_node_t* child)
{
    sdk_size_t i;
    for(i=0; i<node->children.size; i++){
        ai_behaviortree_node_t* p = sdk_vector_get(&node->children, i);
        if(p==child){
            return i;
        }
    }
    return -1;
}

static int ai_behaviortree__continue_evaluate(ai_behaviortree_t* self, ai_behaviortree_node_t * node, void* userdata, ai_behaviortree_result_t* result)
{
    ai_behaviortree_node_t* parent = node->parent;
    ai_behaviortree_node_t* child = node;
    
    while(parent){
        if(parent->type==kAI_BehaviorTreeNodeType_Sequence){
            sdk_size_t idx = ai_behaviortree_node_get_child_index(parent, child);
            if(idx!=-1){
                return ai_behaviortree__evaluate_sequence(self, parent, userdata, idx+1, result);
            }
        }
        child = parent;
        parent = child->parent;
    }
    
    return -1;
    
}

////////////////////////////////////////////////////////////////////////////////
////


int ai_behaviortree_update(ai_behaviortree_t* tree, void* userdata)
{
    if(tree->current_node==0){
        tree->current_node = ai_behaviortree__evaluate_node(tree, tree->root, userdata);
    }
    
    if(tree->current_node){
        ai_action_status_t status = tree->current_node->action->status;
        if(status==kAI_ActionStatus_UNINITIALIZED){
            ai_action_initialize(tree->current_node->action, userdata);
        }else if(status==kAI_ActionStatus_TERMINATED){
            ai_action_cleanup(tree->current_node->action, userdata);
            ai_behaviortree_result_t result = {.result=SDK_FALSE, .node=0};
            ai_behaviortree__continue_evaluate(tree, tree->current_node, userdata, &result);
            tree->current_node = result.node;
        }else if(status==kAI_ActionStatus_RUNNING){
            ai_action_update(tree->current_node->action, userdata);
        }
    }
    
    return 0;
}
