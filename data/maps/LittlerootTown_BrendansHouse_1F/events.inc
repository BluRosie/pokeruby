LittlerootTown_BrendansHouse_1F_EventObjects::
	object_event 1, EVENT_OBJ_GFX_WOMAN_5, 0, 2, 6, 3, MOVEMENT_TYPE_FACE_RIGHT, 0, 0, 0, 0, LittlerootTown_BrendansHouse_1F_EventScript_1B6BE9, FLAG_HIDE_BRENDAN_MOM_DOWNSTAIRS
	object_event 2, EVENT_OBJ_GFX_POOCHYENA, 0, 7, 7, 3, MOVEMENT_TYPE_LOOK_AROUND, 0, 0, 0, 0, TownOne_PlayerHouse1F_HoundourA, FLAG_HIDE_BELDUM_BALL_STEVENS_HOUSE
	object_event 3, EVENT_OBJ_GFX_MELTAN, 0, 0, 0, 3, MOVEMENT_TYPE_FOLLOWER, 0, 0, FALSE, 0, PetalburgWoods_Follower_EventScript, FLAG_HIDE_MOM_LITTLEROOT

LittlerootTown_BrendansHouse_1F_MapWarps::
	warp_def 9, 8, 0, 1, MAP_LITTLEROOT_TOWN
	warp_def 8, 8, 0, 1, MAP_LITTLEROOT_TOWN
	warp_def 8, 2, 0, 0, MAP_LITTLEROOT_TOWN_BRENDANS_HOUSE_2F

LittlerootTown_BrendansHouse_1F_MapEvents::
	map_events LittlerootTown_BrendansHouse_1F_EventObjects, LittlerootTown_BrendansHouse_1F_MapWarps, 0x0, 0x0
