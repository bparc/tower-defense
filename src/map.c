fn tile_t *GetTile(map_t *map, s32 x, s32 y)
{
	tile_t *result = 0;
	if ((x >= 0 && y >= 0) && (x < map->x && y < map->y))
		result = (map->tiles + (y * map->x + x));
	return result;
}

fn u8 GetTileValue(const map_t *map, s32 x, s32 y)
{
	tile_t *tile = GetTile((map_t *)map, x, y);
	return (tile ? tile->value : 0);
}

fn b32 IsTileTraversable(map_t *map, s32 x, s32 y)
{
	b32 result = (GetTileValue(map, x, y) > 0);
	return result;
}

fn void SetTileValue(map_t *map, s32 x, s32 y, u8 value)
{
	tile_t *tile= GetTile(map, x, y);
	if (tile)
		tile->value = value;
}

fn void SetTileDistance(map_t *map, s32 x, s32 y, s16 value)
{
	tile_t *tile = GetTile(map, x, y);
	if (tile)
		tile->distance = value;
}

fn s16 GetTileDistance(const map_t *map, s32 x, s32 y)
{
	tile_t *tile = GetTile((map_t *)map, x, y);
	return (tile ? tile->distance : 0);
}

fn bb_t GetTileBounds(const map_t *map, s32 x, s32 y)
{
	v2 min = sV2(x, y);
	min = Mul(min, map->tile_sz);
	return RectToBB(min, map->tile_sz);
}

fn v2 GetTileCenter(const map_t *map, v2s p)
{
	v2 result = GetCenter(GetTileBounds(map, p.x, p.y));
	return result;
}

fn map_t CreateTestMap(memory_t *memory)
{
#define X 25
#define Y 20
	const u8 Tiles[Y][X] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 3,
		3, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 1, 0, 0, 1, 1, 1, 0, 0,
		0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0,
		0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0,
		0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 4, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 3,
		0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		3, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};
	map_t result = {0};
	result.tile_sz = V2(32.0f, 32.0f);
	result.x = X;
	result.y = Y;
	result.tiles = PushArray(tile_t, memory, X * Y);
	Assert(result.tiles);
	for (s32 y = 0; y < Y; y++)
	for (s32 x = 0; x < X; x++)
		SetTileValue(&result, x, y, Tiles[y][x]);
	ComputeDistances(&result, 12, 12, *memory);
#undef X
#undef Y
	return result;
}