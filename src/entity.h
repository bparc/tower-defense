typedef struct
{
	v2 dir;
	v2 p;
	f32 radius;
} turret_t;

typedef struct
{
	v2 p;
	u8 occupied;
} entry_point_t;

typedef struct
{
	s32 count;
	entry_point_t *points;
	v2 p;
} entrance_t;

typedef struct
{
	path_t path;
	f32 countdown;
} spawn_point_t;

typedef enum
{
	entity_flag_Idle = 1 << 1,
	entity_flag_Wait = 1 << 2,
} entity_flag_t;

typedef struct
{
	s32 flags;

	v2 p;
	s32 segment_index;
	
	u8 path_offset;
	path_t *path;

	v2 direction;
	v2 attachment;
	v2 displacement;
} entity_t;

fn void SetEntityFlag(entity_t *entity, entity_flag_t flag)
{
	entity->flags |= flag;
}

fn b32 IsEntityFlagSet(const entity_t *entity, entity_flag_t flag)
{
	return (entity->flags & flag);
}