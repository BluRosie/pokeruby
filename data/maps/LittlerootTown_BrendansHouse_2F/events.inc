LittlerootTown_BrendansHouse_2F_EventObjects::
	object_event 1, EVENT_OBJ_GFX_MOM, 0, 7, 1, 3, MOVEMENT_TYPE_FACE_DOWN, 1, 0, 0, 0, 0x0, FLAG_HIDE_MOM_UPSTAIRS
	object_event 2, EVENT_OBJ_GFX_MELTAN, 0, 0, 0, 3, MOVEMENT_TYPE_FOLLOWER, 0, 0, FALSE, 0, PetalburgWoods_Follower_EventScript, FLAG_HIDE_MOM_LITTLEROOT

LittlerootTown_BrendansHouse_2F_MapWarps::
	warp_def 7, 1, 0, 2, MAP_LITTLEROOT_TOWN_BRENDANS_HOUSE_1F

LittlerootTown_BrendansHouse_2F_MapEvents::
	map_events LittlerootTown_BrendansHouse_2F_EventObjects, LittlerootTown_BrendansHouse_2F_MapWarps, 0x0, 0x0
