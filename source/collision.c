inline b32 AABBCollision (v2 p1, v2 half_size_1, v2 p2, v2 half_size_2) 
{
    
    b32 horizontal_intersect = 
        ((p1.x + half_size_1.x) > (p2.x - half_size_2.x)) &&
        ((p1.x - half_size_1.x) < (p2.x + half_size_2.x));
    
    b32 vertical_intersect = 
        ((p1.y + half_size_1.y) > (p2.y - half_size_2.y)) &&
        ((p1.y - half_size_1.y) < (p2.y + half_size_2.y));
    
    
    return horizontal_intersect && vertical_intersect;
}


inline void DoBlockVsBallCollision (Block* block, v2 block_half_size, v2 ball_p, v2 ball_desired_p)
{
    
    
    if (AABBSweep(ball_p, ball_desired_p, block->p, block_half_size))
    {
        ball_dp.y *= -1;
    }
    
    
#if 0
    if (AABBCollision(block->p, block_half_size, ball_p, ball_half_size))
    {
        
        
        if (old_ball_p.y < (block->p.y - block_half_size.y) || old_ball_p.y > (block->p.y + block_half_size.y))
        {
            ball_dp.y *= -1;
        }
        
        if (old_ball_p.x < (block->p.x - block_half_size.x) || old_ball_p.x > (block->p.x + block_half_size.x))
        {
            ball_dp.x *= -1
        }
        
    }
#endif
}

inline b32 AABBSweep(v2 start, v2 end, v2 object_p, v2 object_half_size) 
{
    v2 delta = SubV2(end, start);
    
    f32 ty = (object_p.y - start.y) / delta.y;
    
    if (ty >= 0 && ty <= 1)
    {
        return true;
    }
    else 
    {
        return false;
    }
    
    //f32 tx = (object_p.x - start.x) / delta.x;
    
}