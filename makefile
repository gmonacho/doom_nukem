# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gal <gal@student.42lyon.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/10/02 16:01:28 by gmonacho          #+#    #+#              #
#    Updated: 2020/05/28 18:26:49 by gal              ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

EXEC = doom-nukem
CC = clang
SRCS =	srcs/credit.c\
		srcs/error.c\
		srcs/init.c\
		srcs/init_player_maths.c\
		srcs/init_polygone.c\
		srcs/main_free.c\
		srcs/main.c\
		srcs/main_free_entities.c\
		srcs/animation/animation.c\
		srcs/data/poly.c\
		srcs/data/delete_poly.c\
		srcs/display/renderer.c\
		srcs/display/simple_draw.c\
		srcs/display/draw_s.c\
		srcs/display/text.c\
		srcs/display/texture.c\
		srcs/display/window.c\
		srcs/editor_loop/ed_action.c\
		srcs/editor_loop/ed_convert_value.c\
		srcs/editor_loop/ed_delete_map.c\
		srcs/editor_loop/ed_delete_mob.c\
		srcs/editor_loop/ed_delete_object.c\
		srcs/editor_loop/ed_display.c\
		srcs/editor_loop/ed_display_inclined.c\
		srcs/editor_loop/ed_display_mobs.c\
		srcs/editor_loop/ed_display_objects.c\
		srcs/editor_loop/ed_display_player.c\
		srcs/editor_loop/ed_display_poly.c\
		srcs/editor_loop/ed_display_polys.c\
		srcs/editor_loop/ed_display_selected.c\
		srcs/editor_loop/ed_display_list_selected.c\
		srcs/editor_loop/ed_event.c\
		srcs/editor_loop/ed_export.c\
		srcs/editor_loop/ed_export_fonts.c\
		srcs/editor_loop/ed_export_sounds.c\
		srcs/editor_loop/ed_export_textures.c\
		srcs/editor_loop/ed_get_display_color.c\
		srcs/editor_loop/ed_get_object_poly_count.c\
		srcs/editor_loop/ed_get_z_limits.c\
		srcs/editor_loop/ed_init.c\
		srcs/editor_loop/ed_is_next_to_poly.c\
		srcs/editor_loop/ed_is_poly_printable.c\
		srcs/editor_loop/ed_loop.c\
		srcs/editor_loop/ed_manage_property.c\
		srcs/editor_loop/ed_place_door.c\
		srcs/editor_loop/ed_place_flat.c\
		srcs/editor_loop/ed_place_inclined.c\
		srcs/editor_loop/ed_place_item.c\
		srcs/editor_loop/ed_place_player.c\
		srcs/editor_loop/ed_place_poly_base.c\
		srcs/editor_loop/ed_place_wall.c\
		srcs/editor_loop/ed_poly_type.c\
		srcs/editor_loop/ed_selection.c\
		srcs/editor_loop/ed_selection_get.c\
		srcs/editor_loop/ed_selected.c\
		srcs/editor_loop/ed_select_poly.c\
		srcs/editor_loop/ed_set_buttons_mob.c\
		srcs/editor_loop/ed_set_buttons_properties.c\
		srcs/editor_loop/ed_set_flags.c\
		srcs/editor_loop/ed_set_functions.c\
		srcs/editor_loop/ed_set_menu_button_function.c\
		srcs/editor_loop/ed_set_z_functions.c\
		srcs/editor_loop/ed_export_write.c\
		srcs/editor_loop/ed_export_write_cont.c\
		srcs/editor_loop/ed_export_write_item.c\
		srcs/event/time.c\
		srcs/game_loop/events_game_loop.c \
		srcs/game_loop/events.c\
		srcs/game_loop/game_loop.c\
		srcs/game_loop/events_others.c \
		srcs/hud/dead.c\
		srcs/hud/hud.c\
		srcs/hud/hud_utils.c\
		srcs/hud/inventory_hud.c\
		srcs/hud/win.c\
		srcs/hud/weapon.c\
		srcs/inventory/ammo.c\
		srcs/inventory/inventory.c\
		srcs/inventory/inventory_utils.c\
		srcs/libui/ui_error.c\
		srcs/libui/ui_init.c\
		srcs/menu_loops/end.c\
		srcs/menu_loops/main_menu.c\
		srcs/menu_loops/main_menu_init_cond.c\
		srcs/mobs/hit_mobs.c\
		srcs/mobs/mobs_move.c\
		srcs/objects/box_object.c\
		srcs/objects/door_object.c\
		srcs/objects/object.c\
		srcs/objects/objects_actions.c\
		srcs/parsing/parse_error.c\
		srcs/parsing/parse_files.c\
		srcs/parsing/parsing.c\
		srcs/parsing/parsing_mob.c\
		srcs/parsing/parsing_mob_utils.c\
		srcs/parsing/parsing_music.c\
		srcs/parsing/parsing_object.c\
		srcs/parsing/parsing_object_utils.c\
		srcs/parsing/parsing_player.c\
		srcs/parsing/parsing_sphere.c\
		srcs/parsing/parsing_texture.c\
		srcs/parsing/parsing_utils2.c\
		srcs/parsing/parsing_utils.c\
		srcs/parsing/fill_poly_mob.c\
		srcs/parsing/find_texture.c\
		srcs/parsing/add_object_norm2.c\
		srcs/parsing/stock_texture.c\
		srcs/parsing/free_stock_texture.c\
		srcs/physics/collisions_sphere.c\
		srcs/physics/draws.c\
		srcs/physics/fts_maths_algebra.c\
		srcs/physics/fts_maths_fdots_3d_2.c\
		srcs/physics/fts_maths_fdots_3d.c\
		srcs/physics/fts_maths_geometry.c\
		srcs/physics/fts_poly.c\
		srcs/physics/lights.c\
		srcs/physics/projection_2d.c\
		srcs/physics/rotate.c\
		srcs/physics/rotate_rotz_only.c\
		srcs/physics/rotations_x.c\
		srcs/physics/rotations_y.c\
		srcs/physics/rotations_z.c\
		srcs/physics/sky_box.c\
		srcs/physics/slide.c\
		srcs/physics/translate.c\
		srcs/physics/translate_rotz_only.c\
		srcs/physics/raytracing.c\
		srcs/player/init_rays.c\
		srcs/player/player.c\
		srcs/time/time.c\
		srcs/libui/button/ui_button.c\
		srcs/libui/button/ui_simple_button.c\
		srcs/libui/button/ui_text_area.c\
		srcs/libui/button/ui_text_entry_button.c\
		srcs/libui/draw/draw.c\
		srcs/libui/draw/draw_shape.c\
		srcs/libui/event/ui_event.c\
		srcs/libui/frame/ui_frame.c\
		srcs/libui/frame/ui_simple_frame.c\
		srcs/libui/load_ui/ui_check_line_name.c\
		srcs/libui/load_ui/ui_general_get.c\
		srcs/libui/load_ui/ui_get_b_type.c\
		srcs/libui/load_ui/ui_get_flags.c\
		srcs/libui/load_ui/ui_get_type.c\
		srcs/libui/load_ui/ui_load.c\
		srcs/libui/load_ui/ui_parse_button.c\
		srcs/libui/load_ui/ui_parse_buttons.c\
		srcs/libui/load_ui/ui_parse_frame.c\
		srcs/libui/load_ui/ui_parse_frame_data.c\
		srcs/libui/load_ui/ui_parse_simple_button.c\
		srcs/libui/load_ui/ui_parse_simple_frame.c\
		srcs/libui/load_ui/ui_parse_text_area.c\
		srcs/libui/load_ui/ui_parse_text_entry_button.c\
		srcs/libui/load_ui/ui_parse_text_entry_cut.c\
		srcs/libui/load_ui/ui_parse_texture.c\
		srcs/libui/load_ui/ui_parse_type.c\
		srcs/libui/shape/ui_rect.c\
		srcs/libui/texture/ui_draw_one_line_return.c\
		srcs/libui/texture/ui_draw_text_line.c\
		srcs/libui/texture/ui_draw_text_line_return.c\
		srcs/libui/texture/ui_free_texture_set.c\
		srcs/libui/texture/ui_get_dst_y.c\
		srcs/libui/texture/ui_renderer.c\
		srcs/libui/texture/ui_text.c\
		srcs/libui/texture/ui_texture.c\
		srcs/libui/texture/ui_texture_list.c\
		srcs/libui/win/ui.c\
		srcs/libui/win/ui_call_button_function.c\
		srcs/libui/win/ui_check_text_entry_button.c\
		srcs/libui/win/ui_display_buttons.c\
		srcs/libui/win/ui_display_frames.c\
		srcs/libui/win/ui_display_simple.c\
		srcs/libui/win/ui_display_text_area.c\
		srcs/libui/win/ui_display_text_entry.c\
		srcs/libui/win/ui_get_button_by_id.c\
		srcs/libui/win/ui_get_side_rect.c\
		srcs/libui/win/ui_mouse.c\
		srcs/libui/win/ui_resolve_buttons_event.c\
		srcs/libui/win/ui_resolve_text_entry_button.c\
		srcs/libui/win/ui_set_button_function.c\
		srcs/libui/win/ui_update_buttons.c\
		srcs/libui/win/ui_update_frames.c\
		srcs/libui/win/ui_win.c\
		srcs/libui/win/ui_win_size.c\
		srcs/libui/win/ui_win_update_ui.c


HEADERS = -Ilibft/includes/\
		 -Iincludes/\
		 -Iincludes/libui

OBJS = $(SRCS:.c=.o)
LDFLAGS= -lm -pthread -Llibft/ -lft -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image
CFLAGS += -Wall -Wextra -Werror -O3 -ffast-math -march=native 
# -flto 
all: $(EXEC)

$(EXEC): $(OBJS)
	@$(CC) -o $@ $^ $(LDFLAGS)

main.o:	libft.h\
		bordel_proto.h\
		bordel_struct.h\
		doom_nukem.h\
		ui_button.h\
		ui_draw.h\
		ui_error.h\
		ui_event.h\
		ui_frame.h\
		ui.h\
		ui_shape.h\
		ui_texture.h\
		ui_win.h

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(HEADERS)

.PHONY: clean
		fclean

clean:
	@rm -rf $(OBJS)
	@echo "objects deleted"

fclean: clean
	rm -rf $(EXEC)
	@echo "$(EXEC) is deleted"

re: fclean
	all
