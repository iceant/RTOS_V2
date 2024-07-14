#ifndef INCLUDED_AI_EVALUATOR_H
#define INCLUDED_AI_EVALUATOR_H

#ifndef INCLUDED_AI_OBJECT_H
#include <ai_object.h>
#endif /*INCLUDED_AI_OBJECT_H*/

////////////////////////////////////////////////////////////////////////////////
////

typedef struct ai_evaluator_s ai_evaluator_t;

typedef int (*ai_evaluator_function_t)(ai_evaluator_t* evaluator, void* userdata);

struct ai_evaluator_s {
    ai_object_t object;
    ai_evaluator_function_t function;
};

////////////////////////////////////////////////////////////////////////////////
////

int ai_evaluator_init(ai_evaluator_t* evaluator, const char* name, ai_evaluator_function_t function, void* userdata);

int ai_evaluator_evaluate(ai_evaluator_t* evaluator, void* userdata);

#endif /* INCLUDED_AI_EVALUATOR_H */
