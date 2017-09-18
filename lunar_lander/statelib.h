typedef int (*ActionHandler)(void);

typedef struct State State;

struct State {
    ActionHandler action;
    State **transitions;
};

State generateState(ActionHandler handler, State **transitions);
