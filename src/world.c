fn b32 IsAnyPointFree(const entrance_t *entrance)
{
	for (s32 index = 0; index < entrance->count; index++)
	{
		if (!entrance->points[index].occupied)
			return true;
	}
	return 0;
}

fn entrance_t *FindClosestEntrance(game_world_t *state, v2 p)
{
	f32 best_distance = FLT_MAX;
	entrance_t *result = 0;
	for (s32 index = 0;
		index < ArraySize(state->entrances);
		index++)
	{
		entrance_t *entrance = &state->entrances[index];
		f32 distance = Distance(entrance->p, p);
		if ((distance < best_distance) && IsAnyPointFree(entrance))
		{
			best_distance = distance;
			result = entrance;
		} 
	}
	return result;
}

fn entry_point_t *FindClosestUnoccupiedEntrance(game_world_t *state, v2 p)
{
	f32 best_distance = FLT_MAX;
	entry_point_t *result = 0;
	entrance_t *entrance = FindClosestEntrance(state, p);
	for (s32 index = 0;
		index < entrance->count;
		index++)
	{
		// TODO(): A quik hack to make the entities
		// prioritize the closest row. It will break if the
		// number of points isn't uniform across all rows!
		if (((index % 4) == 0) && result)
			break;

		entry_point_t *entry = &entrance->points[index];
		f32 distance = Distance(entry->p, p);
		if ((!entry->occupied) && (distance < best_distance))
		{
			result = entry;
			best_distance = distance;
		}
	}
	return result;
}

fn entity_t *FindClosestEnemy(game_world_t *state, v2 p)
{
	entity_t *result = 0;
	f32 best_distance = FLT_MAX;
	for (s32 index = 0; index < state->num_entities; index++)
	{
		entity_t *entity = &state->entities[index];
		f32 distance = Distance(p, entity->p);
		if (distance < best_distance)
		{
			best_distance = distance;
			result = entity;
		}
	}
	return result;
}

fn void CreateEnemy(game_world_t *state, v2 p, path_t *path)
{
	entity_t *result = 0;
	if (state->num_entities < ArraySize(state->entities))
		result = &state->entities[state->num_entities++];
	if (result)
	{
		ZeroStruct(result);
		result->path = path;
		result->p = p;
		result->path_offset = (rand() % 4);
	}
}

fn void CreateSpawnPoint(game_world_t *state, s32 x, s32 y)
{
	spawn_point_t *result = 0;
	if (state->num_spawners < ArraySize(state->spawners))
		result = &state->spawners[state->num_spawners++];
	if (result)
		GeneratePathFromSource(&state->map, V2s(x, y), &result->path);
}

fn void CreateTurret(game_world_t *state, v2 p)
{
	turret_t *result = 0;
	if (state->num_turrets < ArraySize(state->turrets))
		result = &state->turrets[state->num_turrets++];
	if (result)
	{
		result->dir = Down();
		result->radius = 100.0f;
		result->p = p;
	}
}

fn void MoveEntity(entity_t *entity, f32 amount)
{
	segment_pos_t p = MoveAlongPath(entity->p, entity->segment_index, entity->path, amount);
	entity->segment_index = p.index;
	entity->p = p.offset;
}