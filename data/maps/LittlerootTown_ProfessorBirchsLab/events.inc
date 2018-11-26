LittlerootTown_ProfessorBirchsLab_EventObjects::
	object_event 1, EVENT_OBJ_GFX_SCIENTIST_1, 0, 9, 8, 3, MOVEMENT_TYPE_WANDER_AROUND, 1, 1, 0, 0, TownOne_ProfLab_AideNPC, 0
	object_event 2, EVENT_OBJ_GFX_PROF_BIRCH, 0, 6, 4, 3, MOVEMENT_TYPE_FACE_DOWN, 0, 0, 0, 0, TownOne_ProfLab_ProfNPC, 0
	object_event 3, EVENT_OBJ_GFX_MELTAN, 0, 0, 0, 3, MOVEMENT_TYPE_FOLLOWER, 0, 0, FALSE, 0, PetalburgWoods_Follower_EventScript, FLAG_HIDE_MOM_LITTLEROOT

LittlerootTown_ProfessorBirchsLab_MapWarps::
	warp_def 6, 12, 0, 2, MAP_LITTLEROOT_TOWN
	warp_def 7, 12, 0, 2, MAP_LITTLEROOT_TOWN

LittlerootTown_ProfessorBirchsLab_MapEvents::
	map_events LittlerootTown_ProfessorBirchsLab_EventObjects, LittlerootTown_ProfessorBirchsLab_MapWarps, 0x0, 0x0
