#include <ai_evaluator.h>
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
////

int ai_evaluator_init(ai_evaluator_t* evaluator, const char* name, ai_evaluator_function_t function, void* userdata)
{
    assert(function);
    ai_object_init(&evaluator->object, name, kAI_ObjectType_Evaluator, userdata);
    evaluator->function = function;
    
    return 0;
}

int ai_evaluator_evaluate(ai_evaluator_t* evaluator, void* userdata){
    return evaluator->function(evaluator, userdata);
}


