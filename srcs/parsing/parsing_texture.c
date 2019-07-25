#include "doom_nukem.h"

void        attribute_texture(t_texHud *texHud, t_win *win)
{
    texHud->tex[0] = load_texture(win->rend, "textures/ammoIcon.png");
    texHud->tex[1] = load_texture(win->rend, "textures/healthBar.png");
    texHud->tex[2] = load_texture(win->rend, "textures/healthPixel.png");
    texHud->tex[3] = load_texture(win->rend, "textures/shieldBar.png");
    texHud->tex[4] = load_texture(win->rend, "textures/shieldPixel.png");
    texHud->tex[5] = load_texture(win->rend, "textures/slotIcon.png");
    texHud->tex[6] = load_texture(win->rend, "textures/selectedSlot.png");
    texHud->tex[7] = load_texture(win->rend, "textures/healthPotion.png");
    texHud->tex[8] = load_texture(win->rend, "textures/shieldPotion.png");
    texHud->tex[9] = load_texture(win->rend, "textures/pq.png");
    texHud->tex[10] = load_texture(win->rend, "textures/weaponHud.png");
    texHud->tex[11] = load_texture(win->rend, "textures/crosshairIcon.png");
    texHud->police = TTF_OpenFont("TTF/DooM.ttf", 65);

}

t_texHud     *define_texHud(t_win *win)
{
    t_texHud     *texHud;

    if (!(texHud = (t_texHud *)ft_memalloc(sizeof(t_texHud))))
		  return (ret_null_error("texHud allocation failed in t_text_entry"));
    attribute_texture(texHud, win);
    ft_putendl("texture_parse done →");
    return (texHud);
}