#include "statelib.h"

State generate_state(ActionHandler handler, State **transitions) 
{
    State new_state = {
        .action = handler,
        .transitions = transitions
    };

    return new_state;
}
