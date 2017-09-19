typedef int (*ActionHandler)(void);

typedef struct State State;

// Outter most game states
enum {
    GAME_STATE_LOOP = 0,
    GAME_STATE_PLAY = 1,
    GAME_STATE_GAMEOVER = 2
};
// In game, playing states
enum {
    PLAY_STATE_LOOP = 0,
    PLAY_STATE_PLAY = 1,
    PLAY_STATE_CLEAR = 1,
    PLAY_STATE_CRASHED = 1,
    PLAY_STATE_LANDED = 2
};

struct State {
    ActionHandler action;
    State **transitions;
};

State generate_state(ActionHandler handler, State **transitions);
