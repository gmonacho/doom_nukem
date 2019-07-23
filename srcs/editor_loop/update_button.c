#include "doom_nukem.h"

// static int  update_simple(t_win *win, t_button *b, t_button_state state)
// {
//     SDL_Texture *text;
//     SDL_Texture *bg;
//     int         w;
//     int         h;
//     t_simple_button *data;
    
//     data = (t_simple_button*)b->data;
//     if (!(text = generate_text(win->rend, win->font, data->name,  (SDL_Color){200, 200, 200, 255})))
//         return (ret_error("text generation failed in update_simple"));
//     if (state == BUTTON_STATE_NONE)
//         bg = win->ed_texture.button;
//     else if (state == BUTTON_STATE_ON_MOUSE)
//         bg = win->ed_texture.on_mouse_button;
//     else if (state == BUTTON_STATE_CLICKED)
//         bg = win->ed_texture.clicked_button;
//     SDL_QueryTexture(bg, NULL, NULL, &w, &h);
//     if (!(b->texture = blit_text(win->rend, bg, text, &(SDL_Rect){w / 6, h / 6, w - w / 6 * 2, h - h / 6 * 2})))
//         return (ret_error("blit text failed in update simple"));
// }

static int  update_simple(t_win *win, t_button *b, t_button_state state)
{
    SDL_Texture     *text;
    SDL_Texture     *bg;
    int             w;
    int             h;
    t_simple_button *data;

    bg = NULL;
    data = (t_simple_button*)b->data;
    if (!(text = generate_text(win->rend, win->font, data->name, (SDL_Color){200, 200, 200, 255})))
        return (ret_error("text generation failed in update_simple"));
    if (state == BUTTON_STATE_NONE)
        bg = win->ed_texture.button;
    else if (state == BUTTON_STATE_ON_MOUSE)
        bg = win->ed_texture.on_mouse_button;
    else if (state == BUTTON_STATE_CLICKED)
        bg = win->ed_texture.clicked_button;
    SDL_QueryTexture(bg, NULL, NULL, &w, &h);
    if (!(b->texture = blit_text(win->rend, bg, text, &(SDL_Rect){w / 6, h / 6, w - w / 6 * 2, h - h / 6 * 2})))
        return (ret_error("blit text failed in update simple"));
    return (0);
}

int     update_button(t_win *win, t_button *b, t_button_state state)
{
    if (b->flags & BUTTON_SIMPLE)
        update_simple(win, b, state);
    return (0);
}