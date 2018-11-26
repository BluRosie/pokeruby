LittlerootTown_MaysHouse_1F_EventObjects::
	object_event 1, EVENT_OBJ_GFX_WOMAN_4, 0, 8, 6, 3, MOVEMENT_TYPE_FACE_LEFT, 0, 0, 0, 0, TownOne_ProfHouse_WifeNPC, 0
	object_event 2, EVENT_OBJ_GFX_MELTAN, 0, 0, 0, 3, MOVEMENT_TYPE_FOLLOWER, 0, 0, FALSE, 0, PetalburgWoods_Follower_EventScript, FLAG_HIDE_MOM_LITTLEROOT

LittlerootTown_MaysHouse_1F_MapWarps::
	warp_def 1, 8, 0, 0, MAP_LITTLEROOT_TOWN
	warp_def 2, 8, 0, 0, MAP_LITTLEROOT_TOWN
	warp_def 2, 2, 0, 0, MAP_LITTLEROOT_TOWN_MAYS_HOUSE_2F

LittlerootTown_MaysHouse_1F_MapEvents::
	map_events LittlerootTown_MaysHouse_1F_EventObjects, LittlerootTown_MaysHouse_1F_MapWarps, 0x0, 0x0
