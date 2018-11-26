LittlerootTown_EventObjects::
	object_event 1, EVENT_OBJ_GFX_FAT_MAN, 0, 10, 14, 3, MOVEMENT_TYPE_WANDER_AROUND, 2, 1, 0, 0, LittlerootTown_EventScript_14D6CD, FLAG_HIDE_FAT_MAN_LITTLEROOT
	object_event 2, EVENT_OBJ_GFX_MELTAN, 0, 0, 0, 3, MOVEMENT_TYPE_FOLLOWER, 0, 0, FALSE, 0, PetalburgWoods_Follower_EventScript, FLAG_HIDE_MOM_LITTLEROOT

LittlerootTown_MapWarps::
	warp_def 18, 14, 0, 1, MAP_LITTLEROOT_TOWN_MAYS_HOUSE_1F
	warp_def 8, 8, 0, 1, MAP_LITTLEROOT_TOWN_BRENDANS_HOUSE_1F
	warp_def 16, 6, 0, 0, MAP_LITTLEROOT_TOWN_PROFESSOR_BIRCHS_LAB

LittlerootTown_MapCoordEvents::
	coord_event 10, 1, 3, EVENT_VAR_MELTANHACK, 2, LittlerootTown_Guy_EventScript
	coord_event 11, 1, 3, VAR_LITTLEROOT_STATE, 0, LittlerootTown_EventScript_14D797
	coord_event 11, 1, 3, EVENT_VAR_MELTANHACK, 2, LittlerootTown_Guy_EventScript
	coord_event 13, 1, 3, EVENT_VAR_MELTANHACK, 2, LittlerootTown_Guy_EventScript
	coord_event 12, 1, 3, EVENT_VAR_MELTANHACK, 2, LittlerootTown_Guy_EventScript

LittlerootTown_MapBGEvents::
	bg_event 14, 14, 0, BG_EVENT_PLAYER_FACING_ANY, LittlerootTown_EventScript_14D7F6
	bg_event 14, 7, 0, BG_EVENT_PLAYER_FACING_ANY, LittlerootTown_EventScript_14D7FF
	bg_event 10, 8, 3, BG_EVENT_PLAYER_FACING_ANY, LittlerootTown_EventScript_14D808
	bg_event 9, 12, 3, BG_EVENT_PLAYER_FACING_ANY, LittlerootTown_EventScript_14D834

LittlerootTown_MapEvents::
	map_events LittlerootTown_EventObjects, LittlerootTown_MapWarps, LittlerootTown_MapCoordEvents, LittlerootTown_MapBGEvents
