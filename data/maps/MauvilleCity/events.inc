MauvilleCity_EventObjects::
	object_event 1, EVENT_OBJ_GFX_BOY_3, 0, 20, 18, 3, MOVEMENT_TYPE_WANDER_LEFT_AND_RIGHT, 1, 1, 0, 0, MauvilleCity_EventScript_14C05A, 0
	object_event 2, EVENT_OBJ_GFX_BOY_4, 0, 30, 10, 3, MOVEMENT_TYPE_WANDER_LEFT_AND_RIGHT, 1, 0, 0, 0, MauvilleCity_EventScript_14C075, 0
	object_event 3, EVENT_OBJ_GFX_MANIAC, 0, 34, 8, 3, MOVEMENT_TYPE_FACE_RIGHT, 0, 0, 0, 0, MauvilleCity_EventScript_14C063, 0
	object_event 4, EVENT_OBJ_GFX_WOMAN_5, 0, 10, 6, 3, MOVEMENT_TYPE_LOOK_AROUND, 0, 0, 0, 0, MauvilleCity_EventScript_14C06C, 0
	object_event 5, EVENT_OBJ_GFX_SCHOOL_KID_M, 0, 25, 15, 3, MOVEMENT_TYPE_FACE_DOWN, 0, 0, 0, 0, MauvilleCity_EventScript_14C0A2, 0
	object_event 6, EVENT_OBJ_GFX_WALLY, 0, 17, 9, 3, MOVEMENT_TYPE_FACE_UP, 0, 0, 0, 0, MauvilleCity_EventScript_14C0F9, FLAG_HIDE_WALLY_MAUVILLE
	object_event 7, EVENT_OBJ_GFX_MAN_3, 0, 18, 9, 3, MOVEMENT_TYPE_FACE_LEFT, 0, 0, 0, 0, MauvilleCity_EventScript_14C0C4, FLAG_HIDE_WALLY_FATHER_MAUVILLE
	object_event 8, EVENT_OBJ_GFX_WATTSON, 0, 6, 17, 3, MOVEMENT_TYPE_LOOK_AROUND, 0, 0, 0, 0, MauvilleCity_EventScript_14C2F2, FLAG_HIDE_WATTSON_MAUVILLE
	object_event 9, EVENT_OBJ_GFX_ITEM_BALL, 0, 2, 16, 3, MOVEMENT_TYPE_LOOK_AROUND, 1, 1, 0, 0, MauvilleCity_EventScript_1B171E, FLAG_ITEM_MAUVILLE_1

MauvilleCity_MapWarps::
	warp_def 8, 4, 0, 0, MAP_MAUVILLE_CITY_GYM
	warp_def 27, 14, 0, 0, MAP_MAUVILLE_CITY_POKEMON_CENTER_1F
	warp_def 35, 17, 0, 0, MAP_MAUVILLE_CITY_BIKE_SHOP
	warp_def 28, 6, 0, 0, MAP_MAUVILLE_CITY_MART
	warp_def 32, 14, 0, 0, MAP_MAUVILLE_CITY_HOUSE1
	warp_def 6, 14, 0, 0, MAP_MAUVILLE_CITY_GAME_CORNER
	warp_def 17, 6, 0, 0, MAP_MAUVILLE_CITY_HOUSE2

MauvilleCity_MapBGEvents::
	bg_event 29, 14, 0, BG_EVENT_PLAYER_FACING_NORTH, MauvilleCity_EventScript_1A00EA
	bg_event 14, 7, 3, BG_EVENT_PLAYER_FACING_ANY, MauvilleCity_EventScript_14C087
	bg_event 29, 6, 0, BG_EVENT_PLAYER_FACING_NORTH, MauvilleCity_EventScript_1A00E1
	bg_event 30, 6, 0, BG_EVENT_PLAYER_FACING_NORTH, MauvilleCity_EventScript_1A00E1
	bg_event 28, 14, 0, BG_EVENT_PLAYER_FACING_NORTH, MauvilleCity_EventScript_1A00EA
	bg_event 25, 7, 0, BG_EVENT_PLAYER_FACING_ANY, MauvilleCity_EventScript_14C07E
	bg_event 33, 6, 0, BG_EVENT_PLAYER_FACING_ANY, MauvilleCity_EventScript_14C090
	bg_event 11, 14, 0, BG_EVENT_PLAYER_FACING_ANY, MauvilleCity_EventScript_14C099

MauvilleCity_MapEvents::
	map_events MauvilleCity_EventObjects, MauvilleCity_MapWarps, 0x0, MauvilleCity_MapBGEvents
