#include "raylib.h"

#include "globals.h"
#include "level.h"
#include "player.h"
#include "graphics.h"
#include "assets.h"
#include "utilities.h"

void update_game() {
    game_frame++;

    if (game_state == GAME_STATE_GAME_OVER) {
        if (IsKeyPressed(KEY_ENTER)) {

            player_lives = 3;
            player_score = 0;
            level_index = 0;
            spawn_player();
            game_over = false;
            game_state = GAME_STATE;
            load_level(0);
        }
        return;
    }

    if (game_state == GAME_STATE_VICTORY) {
        if (IsKeyPressed(KEY_ENTER)) {
            level_index = 0;
            load_level(0);
            game_state = GAME_STATE;
        }
        return;
    }

    // Движение игрока и прочие механики
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        move_player_horizontally(MOVEMENT_SPEED);
    }

    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        move_player_horizontally(-MOVEMENT_SPEED);
    }

    is_player_on_ground = is_colliding({player_pos.x, player_pos.y + 0.1f}, WALL);
    if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W) || IsKeyDown(KEY_SPACE)) && is_player_on_ground) {
        player_y_velocity = -JUMP_STRENGTH;
    }

    if (IsKeyPressed(KEY_E) && is_colliding(player_pos, EXIT)) {
        load_level(1);
    }

    update_player();
    if (player_lives <= 0) {
        game_state = GAME_STATE_GAME_OVER;
    }
}

void draw_game() {
    ClearBackground(BLACK);

    if (game_state == GAME_STATE_VICTORY) {
        draw_victory_menu();  // Отображаем экран победы
        return;
    }

    draw_level();
    draw_game_overlay();
    draw_lives();
    draw_game_over();
}

int main() {
    InitWindow(1024, 480, "Platformer");
    SetTargetFPS(60);

    load_fonts();
    load_images();
    load_sounds();
    load_level(0);
    load_music();

    bool is_game_started = false;
    bool is_game_paused = false;

    while (!WindowShouldClose()) {
        UpdateMusicStream(main_menu_music);
        BeginDrawing();
        if (!is_game_started) {
            draw_menu();
            if (IsKeyPressed(KEY_ENTER)) {
                is_game_started = true;
            }
        } else if (!is_game_paused) {
            update_game();
            draw_game();

            if (IsKeyPressed(KEY_F1)) {
                if (game_state != GAME_STATE_VICTORY) {
                    is_game_paused = !is_game_paused;
                    game_state = is_game_paused ? GAME_STATE_PAUSED : GAME_STATE;
                }
            }
        } else {
            draw_pause_screen();

            if (IsKeyPressed(KEY_F1)) {
                is_game_paused = false;
                game_state = GAME_STATE;
            }
        }

        EndDrawing();
    }

    unload_music();
    unload_level();
    unload_sounds();
    unload_images();
    unload_fonts();

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
