OldaleTown_EventObjects::
	object_event 1, EVENT_OBJ_GFX_LASS, 0, 16, 11, 3, MOVEMENT_TYPE_WANDER_AROUND, 0, 0, 0, 0, OldaleTown_EventScript_14DDB3, 0
	object_event 2, EVENT_OBJ_GFX_MART_EMPLOYEE, 0, 13, 7, 3, MOVEMENT_TYPE_FACE_DOWN, 0, 0, 0, 0, OldaleTown_EventScript_14DDBC, 0
	object_event 3, EVENT_OBJ_GFX_MANIAC, 0, 1, 10, 3, MOVEMENT_TYPE_FACE_RIGHT, 0, 0, 0, 0, OldaleTown_EventScript_14DEDF, 0
	object_event 4, EVENT_OBJ_GFX_VAR_0, 0, 13, 19, 3, MOVEMENT_TYPE_FACE_UP, 1, 1, 0, 0, OldaleTown_EventScript_14DF30, FLAG_HIDE_RIVAL_OLDALE_TOWN
	object_event 5, EVENT_OBJ_GFX_MELTAN, 0, 0, 0, 3, MOVEMENT_TYPE_FOLLOWER, 0, 0, FALSE, 0, NULL, FLAG_HIDE_MOM_LITTLEROOT

OldaleTown_MapWarps::
	warp_def 5, 5, 0, 0, MAP_OLDALE_TOWN_HOUSE1
	warp_def 16, 15, 0, 0, MAP_OLDALE_TOWN_HOUSE2
	warp_def 17, 6, 0, 0, MAP_OLDALE_TOWN_POKEMON_CENTER_1F
	warp_def 5, 15, 0, 0, MAP_OLDALE_TOWN_MART

OldaleTown_MapCoordEvents::
	coord_event 0, 10, 3, VAR_ROUTE102_ACCESSIBLE, 0, OldaleTown_EventScript_14DEFF
	coord_event 11, 19, 3, VAR_OLDALE_STATE, 1, OldaleTown_EventScript_14DF41
	coord_event 10, 19, 3, VAR_OLDALE_STATE, 1, OldaleTown_EventScript_14DF5C
	coord_event 12, 19, 3, VAR_OLDALE_STATE, 1, OldaleTown_EventScript_14DF77

OldaleTown_MapBGEvents::
	bg_event 14, 15, 0, BG_EVENT_PLAYER_FACING_ANY, OldaleTown_EventScript_14DDAA
	bg_event 18, 6, 0, BG_EVENT_PLAYER_FACING_NORTH, OldaleTown_EventScript_1A00EA
	bg_event 6, 15, 0, BG_EVENT_PLAYER_FACING_NORTH, OldaleTown_EventScript_1A00E1
	bg_event 19, 6, 0, BG_EVENT_PLAYER_FACING_NORTH, OldaleTown_EventScript_1A00EA
	bg_event 7, 15, 0, BG_EVENT_PLAYER_FACING_NORTH, OldaleTown_EventScript_1A00E1

OldaleTown_MapEvents::
	map_events OldaleTown_EventObjects, OldaleTown_MapWarps, OldaleTown_MapCoordEvents, OldaleTown_MapBGEvents
