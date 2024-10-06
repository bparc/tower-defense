fn s32 StringLength(const char *string)
{
	s32 result = 0;
	while (*string++)
		result++;
	return result;
}

fn f32 Sine(f32 x)
{
	f32 result = sinf(M_TAU32 * x);
	return result;
}

fn f32 Cosine(f32 x)
{
	f32 result = cosf(M_TAU32 * x);
	return result;
}

fn f32 SqRoot(f32 x)
{
	f32 result = sqrtf(x);
	return result;
}

fn f32 Lerp(f32 a, f32 b, f32 t)
{
	f32 result = a;
	if (t <= 0.0f)
	{
		result = a;
	}
	else
	if (t >= 1.0f)
	{
		result = b;
	}
	else
	{
		result = (b * t) + ((1.0f - t) * a);
	}
	return result;
}

fn s32 RandomInt(void)
{
	s32 result = rand();
	return result;
}

#define MIN(a, b) (a < b ? a : b) 
#define MAX(a, b) (a > b ? a : b)

fn s32 MinS32(s32 a, s32 b)
{
	s32 result = MIN(a, b);
	return result;
}

fn f32 MinF32(f32 a, f32 b)
{
	f32 result = MIN(a, b);
	return result;
}

fn f32 MaxF32(f32 a, f32 b)
{
	f32 result = MAX(a, b);
	return result;
}