typedef struct
{
	v2 a;
	v2 b;
	v2 normal;
} path_segment_t;

typedef struct
{
	v2 offset;
	s32 index;
} segment_pos_t;

typedef struct
{
	s32 count;
	v2 points[1024];
} path_t;

fn v2 QuadraticBezier(v2 p0, v2 p1, v2 p2, f32 t);

fn void InsertWaypoint(path_t *path, v2 p);
fn void GeneratePathFromSource(map_t *map, v2s source, path_t *path);

fn path_segment_t GetSegment(const path_t *path, s32 point_index);
fn segment_pos_t _MoveAlongPath(segment_pos_t p, const path_t *path, f32 offset);
fn segment_pos_t MoveAlongPath(v2 offset, s32 index, const path_t *path, f32 amount);