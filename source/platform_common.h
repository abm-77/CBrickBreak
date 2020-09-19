struct 
{
    b32 is_down;
    b32 changed;
} typedef Button;


enum 
{
    BUTTON_LEFT,
    BUTTON_RIGHT,
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_COUNT,
};

struct 
{
    v2i mouse;
    Button buttons[BUTTON_COUNT];
} typedef Input;

#define Pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define Released(b) (!input->buttons[b].is_down && input->buttons[b].changed)
#define IsDown(b) (input->buttons[b].is_down)