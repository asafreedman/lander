#include "statelib.h"

State generateState(ActionHandler handler, State **transitions) {
    State newState = {
        .action = handler,
        .transitions = transitions
    };

    return newState;
}