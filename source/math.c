// Math
internal int Clamp (int val, int min, int max)
{
    if (val < min) return min;
    else if (val > max) return max;
    
    return val;
}

inline f32 Lerp (f32 a, f32 b, f32 t)
{
    return (1-t) * a + t * b; 
}

// Color
inline u32 MakeColorFromGray(u32 gray)
{
    return (gray << 0) | (gray << 8) | (gray << 16);
}

// Vector
struct 
{
    union 
    {
        struct
        {
            f32 x;
            f32 y;
        };
        
        f32 e[2];
    };
}typedef v2;

inline v2 AddV2(v2 a, v2 b)
{
    return (v2) {a.x + b.x, a.y + b.y};
}


inline v2 SubV2(v2 a, v2 b)
{
    return (v2) {a.x - b.x, a.y - b.y};
}


inline v2 MulV2(v2 a, f32 scalar)
{
    return (v2) {a.x * scalar, a.y * scalar};
}



struct 
{
    union 
    {
        struct
        {
            s32 x;
            s32 y;
        };
        
        s32 e[2];
    };
}typedef v2i;
