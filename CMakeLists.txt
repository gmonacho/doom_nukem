cmake_minimum_required(VERSION 3.10)
project(doom-nukem)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/includes ${CMAKE_CURRENT_SOURCE_DIR}/libft/includes ${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})

add_executable(${PROJECT_NAME} srcs/data/delete_linedef.c
                                srcs/data/export_sector.c
                                srcs/data/ft_utils.c
                                srcs/data/is_next_to_linedef.c
                                srcs/data/linedef.c
                                srcs/data/map_editor.c
                                srcs/data/png_parser.c
                                srcs/data/sector.c
                                srcs/data/selected_linedef.c
                                srcs/display/button.c
                                srcs/display/color_picker.c
                                srcs/display/frame.c
                                srcs/display/renderer.c
                                srcs/display/simple_draw.c
                                srcs/display/text.c
                                srcs/display/texture.c
                                srcs/display/window.c
                                srcs/editor_loop/editor_display.c
                                srcs/editor_loop/editor_event.c
                                srcs/editor_loop/editor_loop.c
                                srcs/editor_loop/ui_left_press.c
                                srcs/editor_loop/ui_left_release.c
                                srcs/event/event.c
                                srcs/event/mouse.c
                                srcs/event/text_input.c
                                srcs/event/time.c
                                srcs/game_loop/draw.c
                                srcs/game_loop/events.c
                                srcs/game_loop/game_loop.c
                                srcs/menu_loops/main_menu.c
                                srcs/parsing/parse_error.c
                                srcs/parsing/parse_utils.c
                                srcs/parsing/parsing_player.c
                                srcs/parsing/parsing.c
                                srcs/parsing/parsing_object.c
                                srcs/physics/collisions.c
                                srcs/physics/raycasting.c
                                srcs/physics/teleportation.c
                                srcs/error.c
                                srcs/main.c)

set(LIBRARIES -L/${CMAKE_CURRENT_SOURCE_DIR}/libft/ -lft ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY} -lm)
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})