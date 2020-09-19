v2 ball_p;
v2 ball_dp;
v2 ball_half_size;
f32 ball_base_speed;
f32 ball_speed_multiplier;

v2 player_p;
v2 player_dp;
v2 player_half_size;

struct 
{
    v2 p;
    v2 half_size;
    f32 ball_speed_multiplier;
    int life;
    u32 color;
}typedef Block;
Block blocks[256];

int next_block;

#include "collision.c"

enum 
{
    GM_NORMAL,
    GM_WALL,
    GM_COUNT,
    GM_CONSTRUCTION,
    GM_SPACED,
} typedef GameMode;

GameMode current_game_mode;

v2 arena_half_size;

b32 first_ball_movement;
b32 initialized = false;



internal void StartGame(GameMode game_mode)
{
    if (game_mode >= GM_COUNT) game_mode = 0;
    else if (game_mode < 0) game_mode = GM_COUNT-1;
    
    current_game_mode = game_mode;
    
    ball_dp.x = 0;
    
    ball_half_size = (v2) {1, 1};
    ball_base_speed = 50;
    ball_p = (v2){0,0};
    ball_dp.y = -ball_base_speed;
    ball_speed_multiplier = 1;
    
    player_p.y = -40;
    player_half_size = (v2) {15, 2};
    
    arena_half_size = (v2) {80, 50};
    
    next_block = 0;
    
    for (Block* block = blocks; block != blocks + ArrayCount(blocks); block++)
        block->life = 0;
    
    switch (game_mode)
    {
        case GM_NORMAL:
        {
            int num_x = 17;
            int num_y = 9;
            
            f32 block_x_half_size = 4.0f;
            
            f32 x_offset = (f32)num_x * block_x_half_size * 1.1f - block_x_half_size;
            f32 y_offset = -4;
            
            for (int y = 0; y < num_y; y++)
            {
                for (int x = 0; x < num_x; x++)
                {
                    Block* block = blocks + next_block++;
                    if (next_block >= ArrayCount(blocks))
                        next_block = 0;
                    
                    block->life = 1;
                    block->half_size = (v2) {block_x_half_size, 2.0f};
                    
                    block->p.x = x * block->half_size.x * 2.2f - x_offset;
                    block->p.y = y * block->half_size.y * 2.2f - y_offset;
                    
                    block->color = MakeColorFromGray(y * 256 / num_y);
                    
                    block->ball_speed_multiplier = 1 + ((f32) (y * 1.5f) / (f32) num_y);
                }
            }
            
        }
        break;
        case GM_WALL:
        {
            int num_x = 17;
            int num_y = 9;
            
            f32 block_x_half_size = 4.0f;
            
            f32 x_offset = (f32)num_x * block_x_half_size * 1.1f - block_x_half_size;
            f32 y_offset = -4;
            
            for (int y = 0; y < num_y; y++)
            {
                for (int x = 0; x < num_x; x++)
                {
                    Block* block = blocks + next_block++;
                    if (next_block >= ArrayCount(blocks))
                        next_block = 0;
                    
                    block->life = 1;
                    block->half_size = (v2) {block_x_half_size, 2.0f};
                    
                    block->p.x = x * block->half_size.x * 2.2f - x_offset;
                    block->p.y = y * block->half_size.y * 2.2f - y_offset;
                    
                    block->color = MakeColorFromGray((x+y) * 8);
                    
                    block->ball_speed_multiplier = 1 + ((f32) (y * 1.5f) / (f32) num_y);
                }
            }
            
        }
        break;
        
        invalid_default_case;
    }
    
    
}

internal SimulateGame (Input* input, f32 dt)
{
    if (!initialized) 
    {
        initialized = true;
        current_game_mode = 0;
        StartGame(current_game_mode);
    }
    
    v2 player_desired_p;
    player_desired_p.x = PixelsToWorld(input->mouse).x;
    player_desired_p.y = player_p.y;
    
    v2 ball_desired_p = AddV2(ball_p, MulV2 (ball_dp, dt)); 
    
    
    if (ball_dp.y < 0 && AABBCollision(player_p, player_half_size, ball_desired_p, ball_half_size))
    {
        ball_dp.y *= -1;
        ball_dp.x = (ball_p.x - player_p.x) * 10;
        ball_desired_p.y = player_p.y + player_half_size.y;
        first_ball_movement = false;
        ball_dp.x += Clamp(player_dp.x, -10, 10);
    }
    else if (ball_desired_p.x + ball_half_size.x > arena_half_size.x)
    {
        ball_desired_p.x = arena_half_size.x - ball_half_size.x;
        ball_dp.x *= -1;
    }
    else if (ball_desired_p.x - ball_half_size.x < -arena_half_size.x)
    {
        ball_desired_p.x = -arena_half_size.x + ball_half_size.x;
        ball_dp.x *= -1;
    }
    
    if (ball_desired_p.y + ball_half_size.y > arena_half_size.y)
    {
        ball_desired_p.y = arena_half_size.y - ball_half_size.y;
        ball_dp.y *= -1;
    }
    
    ClearScreenAndDrawRect((v2){0,0}, arena_half_size, 0xffcc66, 0x805500);
    
    for (Block* block = blocks; block != blocks + ArrayCount(blocks); block++)
    {
        if (!first_ball_movement) {
            
            
            if (!block->life) continue;
            
            b32 moving_up = ball_dp.y > 0;
            
            v2 delta = SubV2(ball_desired_p, ball_p);
            
            if (delta.y != 0) 
            {
                f32 collision_point = (moving_up) ? block->p.y - block->half_size.y - ball_half_size.y : block->p.y + block->half_size.y + ball_half_size.y; 
                
                f32 ty = (collision_point - ball_p.y ) / delta.y;
                
                if (ty >= 0 && ty <= 1)
                {
                    f32 target_x = Lerp(ball_p.x, ball_desired_p.x, ty);
                    if (target_x + ball_half_size.x > block->p.x - block->half_size.x &&
                        target_x - ball_half_size.x < block->p.x + block->half_size.x)
                    {
                        ball_desired_p.y = Lerp(ball_p.y, ball_desired_p.y, ty);
                        
                        if (block->ball_speed_multiplier > ball_speed_multiplier) ball_speed_multiplier = block->ball_speed_multiplier;
                        ball_dp.y = (moving_up) ? ball_base_speed * -ball_speed_multiplier : ball_base_speed * ball_speed_multiplier; 
                        
                        block->life--;
                        
                    }
                }
            }
            
            if (delta.x != 0) 
            {
                b32 moving_right = ball_dp.x > 0;
                
                f32 collision_point = (moving_right) ? block->p.x - block->half_size.x - ball_half_size.x : block->p.x + block->half_size.x + ball_half_size.x;;
                
                f32 tx = (collision_point - ball_p.x ) / delta.x;
                
                if (tx >= 0 && tx <= 1)
                {
                    f32 target_y = Lerp(ball_p.y, ball_desired_p.y, tx);
                    if (target_y + ball_half_size.y > block->p.y - block->half_size.y &&
                        target_y - ball_half_size.y < block->p.y + block->half_size.y)
                    {
                        ball_desired_p.x = Lerp(ball_p.x, ball_desired_p.x, tx);
                        
                        if (block->ball_speed_multiplier > ball_speed_multiplier) ball_speed_multiplier = block->ball_speed_multiplier;
                        ball_dp.y  = (moving_up) ? ball_base_speed * ball_speed_multiplier : ball_base_speed * -ball_speed_multiplier;
                        
                        block->life--;
                    }
                }
            }
            
        }
        DrawRect(block->p, block->half_size, block->color);
        
    }
    
    ball_p   = ball_desired_p;
    
    player_dp.x = (player_desired_p.x - player_p.x) / dt;
    player_p = player_desired_p;
    
    DrawRect(ball_p, ball_half_size, 0xff0000);DrawRect(player_p, player_half_size, 0xffffff);
    
    
    // Lose life
    if (ball_p.y - ball_half_size.y < -50)
    {
        StartGame(current_game_mode);
    }
    
#if DEVELOPMENT
    if (Pressed(BUTTON_LEFT))  StartGame(current_game_mode-1);
    if (Pressed(BUTTON_RIGHT)) StartGame(current_game_mode+1);
#endif
    
} 

