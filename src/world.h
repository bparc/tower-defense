#include "entity.h"
typedef struct
{
	entrance_t entrances[4]; // NOTE(): Base entrances for all 4 cardinal directions.
	v2 base_p;

	map_t map;

	s32 num_spawners;
	spawn_point_t spawners[16];

	s32 num_entities;
	entity_t entities[20];

	s32 num_turrets;
	turret_t turrets[32];
} game_world_t;
#include "world.c"

fn void Setup(game_world_t *world, memory_t *memory);
fn void Update(game_world_t *state, f32 dt, client_input_t input);
fn void DrawFrame(const game_world_t *state, command_buffer_t *out);

fn void Setup(game_world_t *state, memory_t *memory)
{
	map_t *map = &state->map;
	*map = CreateTestMap(memory);

	// NOTE(): Spawn entities
	for (s32 y = 0; y < map->y; y++)
	{
		for (s32 x = 0; x < map->x; x++)
		{
			tile_t *tile = GetTile(map, x, y);

			v2 tile_center = GetTileCenter(map, V2s(x, y));
			if (tile->value == 3) // A spawn point
				CreateSpawnPoint(state, x, y);
			else
			if (tile->value == 4) // A turret
				CreateTurret(state, tile_center);
			else
			if (tile->value == 2) // Set the player position.
				state->base_p = tile_center;
		}
	}
	// NOTE(): Generate entry points for the player.
	for (s32 entrance_index = 0;
		entrance_index < ArraySize(state->entrances);
		entrance_index++)
	{
		v2 directions[4] = { V2(+0.0f, -1.0f), V2(+0.0f, +1.0f), V2(-1.0f, +0.0f), V2(+1.0f, 0.0f) };
		v2 direction = directions[entrance_index];
		v2 normal = Perp(direction);

		s32 row_count = 4;
		s32 points_per_row = 4;

		entrance_t *entrance = &state->entrances[entrance_index];
		entrance->count = points_per_row * row_count;
		entrance->points = PushArray(entry_point_t, memory, entrance->count);
		Assert(entrance->points);
		entrance->p = Add(state->base_p, Scale(direction, 60.0f));

		for (s32 entry_index = 0;
			entry_index < (entrance->count * row_count);
			entry_index++)
		{
			s32 row_index = entry_index / points_per_row;

			entry_point_t *entry = &entrance->points[entry_index];
			entry->occupied = 0;
			
			f32 spacing = 15.0f;
			f32 extends = 20.0f;

			f32 distance = 30.0f + ((f32)row_index * 10.0f);
			v2 center = Add(state->base_p, Scale(direction, distance));
			
			v2 min = Sub(center, Scale(normal, extends));
			f32 offset = (f32)(entry_index % points_per_row);
			offset *= spacing;
			min = Add(min, Scale(normal, offset));
			entry->p = min;
		}
	}
}

fn void Update(game_world_t *state, f32 dt, client_input_t input)
{
	//DebugText(V2(8.0f, 5.0f), input.gpu_driver_desc);
	DebugPrint("driver   = %s", input.gpu_driver_desc);
	DebugPrint("viewport = %.0f,%.0f", input.viewport[0], input.viewport[1]);
	DebugPrint("cursor   = %.0f,%.0f", input.mouse[0], input.mouse[1]);
	
	// NOTE(): Spawners
	for (s32 index = 0; index < state->num_spawners; index++)
	{
		spawn_point_t *spawner = &state->spawners[index];
		spawner->countdown += dt;
		if (spawner->countdown >= 2.0f)
		{
			CreateEnemy(state, spawner->path.points[0], &spawner->path);
			spawner->countdown = 0.0f;
		}
	}

	// NOTE(): Entities
	for (s32 index = 0; index < state->num_entities; index++)
	{
		entity_t *entity = &state->entities[index];
		if (IsEntityFlagSet(entity, entity_flag_Idle))
		{
			entity->p = Arrive(entity->p, entity->attachment, 50.0f * dt, 10.0f);
		}
		else
		{
			if ((IsEntityFlagSet(entity, entity_flag_Wait) == false))
				MoveEntity(entity, 50.0f * dt);
			if (Distance(state->base_p, entity->p) <= 80.0f)
			{
				entry_point_t *entry = FindClosestUnoccupiedEntrance(state, entity->p);
				if (entry)
				{
					entry->occupied = true;

					SetEntityFlag(entity, entity_flag_Idle);
					entity->attachment = entry->p;
				}
				else
				{
					// NOTE(): Couldn't find a valid entrance - wait for now...
					SetEntityFlag(entity, entity_flag_Wait);
				}
			}
		}
	}

	// NOTE(): Turrets
	for (s32 index = 0; index < state->num_turrets; index++)
	{
		turret_t *turret = &state->turrets[index];
		entity_t *enemy = FindClosestEnemy(state, turret->p);
		if (enemy)
			turret->dir = GetDirection(turret->p, enemy->p);
	}
}

fn void DrawFrame(const game_world_t *state, command_buffer_t *out)
{
	// NOTE(): Map
	const map_t *map = &state->map;
	for (s32 y = 0; y < map->y; y++)
	for (s32 x = 0; x < map->x; x++)
	{
		v4 colors[] = { DarkGrey(), DarkBlue(), DarkGreen(), Red(), Pink() };
		s32 value = GetTileValue(map, x, y) % ArraySize(colors);
		DrawBounds(out, GetTileBounds(map, x, y), colors[value]);
	}

	// NOTE(): Walkways
#if 1
	for (s32 path_index = 0;
		 path_index < state->num_spawners;
		 path_index++)
	{
		const path_t *path = &state->spawners[path_index].path;
		// NOTE(): Segments
		for (s32 index = 0; index < (path->count - 1); index++)
			DrawLine(out, path->points[index], path->points[index + 1], Orange());
		// NOTE(): Points
		for (s32 index = 0; index < path->count; index++)
			DebugPoint(path->points[index], Blue());
	}
#endif

	// NOTE(): Entities	
	for (s32 index = 0; index < state->num_entities; index++)
	{
		const entity_t *entity = &state->entities[index];
		v2 p = Add(entity->p, entity->displacement);
		v2 bitmap_sz = V2(14.0f, 24.0f);
		v2 pivot = V2(0.5f, 1.0f);
		v2 bitmap_p = Sub(p, Mul(bitmap_sz, pivot));
		DrawRectOutline(out, bitmap_p, bitmap_sz, Orange());
		DebugPoint(p, Red());
	}

	// NOTE(): Turrets
	for (s32 index = 0; index < state->num_turrets; index++)
	{
		const turret_t *turret = &state->turrets[index];
		DrawCircleOutline(out, turret->p, turret->radius, Green());
		DebugVector(turret->p, turret->dir, Green());
	}
	// NOTE(): Player
	DrawCircleOutline(out, state->base_p, 80.0f, Pink());
}