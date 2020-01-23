cmake_minimum_required(VERSION 3.10)
project(ui C)
# set (CMAKE_C_COMPILER "/usr/bin/gcc")
set (CMAKE_C_FLAGS "-g3 -fsanitize=address")
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake_files/)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/libft/includes ${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})

set(LIBRARIES ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY} -L${CMAKE_CURRENT_SOURCE_DIR}/libft/ -lft)

execute_process(COMMAND bash "make_libft.sh")

add_library(${PROJECT_NAME} src/button/ui_button.c
src/button/ui_simple_button.c
src/button/ui_text_area.c
src/button/ui_text_entry_button.c
src/draw
src/draw/draw_shape.c
src/draw/draw.c
src/event
src/event/ui_event.c
src/frame
src/frame/ui_frame.c
src/frame/ui_simple_frame.c
src/load_ui
src/load_ui/ui_check_line_name.c
src/load_ui/ui_general_get.c
src/load_ui/ui_get_b_type.c
src/load_ui/ui_get_flags.c
src/load_ui/ui_get_type.c
src/load_ui/ui_load.c
src/load_ui/ui_parse_button.c
src/load_ui/ui_parse_buttons.c
src/load_ui/ui_parse_frame_data.c
src/load_ui/ui_parse_frame.c
src/load_ui/ui_parse_simple_button.c
src/load_ui/ui_parse_simple_frame.c
src/load_ui/ui_parse_text_area.c
src/load_ui/ui_parse_text_entry_button.c
src/load_ui/ui_parse_text_entry_cut.c
src/load_ui/ui_parse_texture.c
src/load_ui/ui_parse_type.c
src/shape/ui_rect.c
src/texture
src/texture/ui_draw_one_line_return.c
src/texture/ui_draw_text_line_return.c
src/texture/ui_draw_text_line.c
src/texture/ui_draw_text_rect.c
src/texture/ui_free_texture_set.c
src/texture/ui_renderer.c
src/texture/ui_text.c
src/texture/ui_texture_list.c
src/texture/ui_texture.c
src/win/ui_call_button_function.c
src/win/ui_display_buttons.c
src/win/ui_display_frames.c
src/win/ui_display_simple.c
src/win/ui_display_text_area.c
src/win/ui_display_text_entry.c
src/win/ui_get_side_rect.c
src/win/ui_mouse.c
src/win/ui_resolve_buttons_event.c
src/win/ui_resolve_text_entry_button.c
src/win/ui_set_button_function.c
src/win/ui_update_buttons.c
src/win/ui_update_frames.c
src/win/ui_win_size.c
src/win/ui_win_update_ui.c
src/win/ui_win.c
src/win/ui.c
src/main.c
src/ui_error.c
src/ui_init.c)

target_link_libraries(${PROJECT_NAME} ${LIBRARIES})