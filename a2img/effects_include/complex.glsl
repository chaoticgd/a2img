#define complex vec2
#define complex_re(z) z.x
#define complex_im(z) z.y
#define complex_abs(z) length(z)
#define complex_pow2(z) vec2(z.x * z.x - z.y * z.y, 2 * z.x * z.y)
