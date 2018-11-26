LittlerootTown_MaysHouse_2F_EventObjects::
	object_event 1, EVENT_OBJ_GFX_GIRL_2, 0, 5, 4, 3, MOVEMENT_TYPE_WANDER_AROUND, 2, 2, 0, 0, TownOne_ProfHouse_ChildNPC, FLAG_HIDE_MAY_UPSTAIRS
	object_event 2, EVENT_OBJ_GFX_PICHU_DOLL, 0, 3, 4, 4, MOVEMENT_TYPE_FACE_DOWN, 1, 1, 0, 0, 0x0, FLAG_HIDE_MAY_PICHU_DOLL
	object_event 3, EVENT_OBJ_GFX_MELTAN, 0, 0, 0, 3, MOVEMENT_TYPE_FOLLOWER, 0, 0, FALSE, 0, PetalburgWoods_Follower_EventScript, FLAG_HIDE_MOM_LITTLEROOT

LittlerootTown_MaysHouse_2F_MapWarps::
	warp_def 1, 1, 0, 2, MAP_LITTLEROOT_TOWN_MAYS_HOUSE_1F

LittlerootTown_MaysHouse_2F_MapEvents::
	map_events LittlerootTown_MaysHouse_2F_EventObjects, LittlerootTown_MaysHouse_2F_MapWarps, 0x0, 0x0
