fn v2 QuadraticBezier(v2 p0, v2 p1, v2 p2, f32 t)
{
	v2 a = Lerp2(p0, p1, t);
	v2 b = Lerp2(p2, p1, 1.0f - t);
	return (Lerp2(a, b, t));
}

fn void InsertWaypoint(path_t *path, v2 p)
{
	if (path->count < ArraySize(path->points))
		path->points[path->count++] = p;
}

fn path_segment_t GetSegment(const path_t *path, s32 point_index)
{
	Assert(path->count > 0);
	path_segment_t result = {0};
	result.a = path->points[point_index];
	result.b = result.a;
	if ((point_index + 1) < path->count)
		result.b = path->points[(point_index + 1)];
	result.normal = Sub(result.b, result.a);
	result.normal = Normalize(result.normal);
	return result;
}

fn v2 ClampToSegment(path_segment_t segment, v2 p)
{
	return ClampToLine(segment.a, segment.b, p);
}

fn void GeneratePathFromSource(map_t *map, v2s source, path_t *path)
{
	const s32 CURVE_POINT_COUNT = 5;
	path->count = 0;

	if (!GetTile(map, source.x, source.y))
		return;

	v2s at = source;
	while (GetTileDistance(map, at.x, at.y) > 0)
	{
		v2s next, delta;
		InsertWaypoint(path, GetTileCenter(map, at));
		next = FindNearestTile(map, at);
		delta = SubS(at, next);
		at = next;
		
		// NOTE(): Check if we're at a turn...
		if ((path->count > 2) && (GetTileDistance(map, at.x, at.y) >= 2))
		{
			next = FindNearestTile(map, at);
			v2s d1 = delta;
			v2s d2 = SubS(at, next);
			b32 x_turn = (d1.y != 0) && (d2.x != 0);
			b32 y_turn = (d1.x != 0) && (d2.y != 0);
			if (x_turn || y_turn)
			{
				v2 a = path->points[path->count - 1];
				v2 b = GetTileCenter(map, at);
				v2 c = GetTileCenter(map, next);
				path->count--;
				
				for (s32 index = 0; index <= CURVE_POINT_COUNT; index++)
				{
					f32 t = ((f32)index / (f32)CURVE_POINT_COUNT);
					InsertWaypoint(path, QuadraticBezier(a, b, c, t));
				}
				at = next;
			}
		}
	}
}

fn segment_pos_t MoveAlongPath(v2 offset, s32 index, const path_t *path, f32 amount)
{
	segment_pos_t result = {0};
	result.offset = offset;
	result.index = index;
	result = _MoveAlongPath(result, path, amount);
	return result;
}

fn segment_pos_t _MoveAlongPath(segment_pos_t p, const path_t *path, f32 amount)
{
	// NOTE(): We're past the last point: do an early-out.
	if (p.index >= path->count)
	{
		p.offset = path->points[path->count - 1];
		return p;
	}
	// NOTE(): Ensure that we're starting at a valid segment point.
	p.offset = ClampToSegment(GetSegment(path, p.index), p.offset);

	f32 remaining = amount;
	while (remaining > 0.0f)
	{
		const path_segment_t segment = GetSegment(path, p.index);

		// NOTE(): Move the point by the requested/remaining amount.
		const v2 offset = Scale(segment.normal, remaining);
		const v2 prev_p = p.offset;
		p.offset = Add(p.offset, offset);

		f32 distance_moved = amount;
		// NOTE(): Check if we've moved past the segment's end.
		const v2 v = Sub(p.offset, segment.a);
		f32 t = 1.0f;
		const f32 length = Length(Sub(segment.b, segment.a));
		if (length)
			t = Dot(v, segment.normal) / length;

		if (t >= 1.0f)
		{
			// NOTE(): If so, clamp the point to the
			// current segment, then advance to the next segment.
			p.offset = segment.b;
			if (p.index < path->count)
				p.index++;
			distance_moved = Length(Sub(prev_p, p.offset));
		}

		remaining -= distance_moved;
	}

	return (p);
}