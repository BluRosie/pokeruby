PetalburgWoods_EventObjects::
	object_event 1, EVENT_OBJ_GFX_CUTTABLE_TREE, 0, 36, 21, 3, MOVEMENT_TYPE_LOOK_AROUND, 0, 0, 0, 0, S_CuttableTree, FLAG_TEMP_11
	object_event 2, EVENT_OBJ_GFX_CUTTABLE_TREE, 0, 37, 21, 3, MOVEMENT_TYPE_LOOK_AROUND, 0, 0, 0, 0, S_CuttableTree, FLAG_TEMP_12
	object_event 3, EVENT_OBJ_GFX_VAR_1, 0, 26, 17, 3, MOVEMENT_TYPE_FACE_RIGHT, 0, 0, 0, 0, 0x0, FLAG_HIDE_EVIL_TEAM_PETALBURG_WOODS
	object_event 4, EVENT_OBJ_GFX_MAN_2, 0, 25, 20, 3, MOVEMENT_TYPE_LOOK_AROUND, 0, 0, 0, 0, 0x0, FLAG_HIDE_DEVON_PETALBURG_WOODS
	object_event 5, EVENT_OBJ_GFX_ITEM_BALL, 0, 41, 6, 3, MOVEMENT_TYPE_FACE_DOWN, 0, 0, 0, 0, PetalburgWoods_EventScript_1B175F, FLAG_ITEM_PETALBURG_WOODS_2
	object_event 6, EVENT_OBJ_GFX_ITEM_BALL, 0, 34, 25, 3, MOVEMENT_TYPE_FACE_DOWN, 0, 0, 0, 0, PetalburgWoods_EventScript_1B1752, FLAG_ITEM_PETALBURG_WOODS_1
	object_event 7, EVENT_OBJ_GFX_ITEM_BALL, 0, 4, 7, 3, MOVEMENT_TYPE_LOOK_AROUND, 0, 0, 0, 0, PetalburgWoods_EventScript_1B176C, FLAG_ITEM_PETALBURG_WOODS_3
	object_event 8, EVENT_OBJ_GFX_BOY_2, 0, 19, 4, 3, MOVEMENT_TYPE_WANDER_UP_AND_DOWN, 1, 2, 0, 0, PetalburgWoods_EventScript_15CECA, 0
	object_event 9, EVENT_OBJ_GFX_BUG_CATCHER, 0, 7, 32, 3, MOVEMENT_TYPE_FACE_DOWN_LEFT_AND_RIGHT, 0, 0, 1, 3, PetalburgWoods_EventScript_15CF27, 0
	object_event 10, EVENT_OBJ_GFX_BUG_CATCHER, 0, 45, 22, 3, MOVEMENT_TYPE_FACE_DOWN_UP_AND_RIGHT, 0, 0, 1, 3, PetalburgWoods_EventScript_15CF3E, 0
	object_event 11, EVENT_OBJ_GFX_BOY_3, 0, 33, 36, 3, MOVEMENT_TYPE_WANDER_AROUND, 2, 1, 0, 0, PetalburgWoods_EventScript_15CED3, 0
	object_event 12, EVENT_OBJ_GFX_ITEM_BALL, 0, 35, 29, 3, MOVEMENT_TYPE_LOOK_AROUND, 1, 1, 0, 0, PetalburgWoods_EventScript_1B1779, FLAG_ITEM_PETALBURG_WOODS_4
	object_event 13, EVENT_OBJ_GFX_GIRL_2, 0, 32, 8, 3, MOVEMENT_TYPE_LOOK_AROUND, 1, 1, 0, 0, PetalburgWoods_EventScript_15CEDC, 0
	object_event 14, EVENT_OBJ_GFX_MELTAN, 0, 7, 17, 3, MOVEMENT_TYPE_NONE, 0, 0, FALSE, 0, NULL, 0

PetalburgWoods_MapWarps::
	warp_def 36, 38, 0, 6, MAP_ROUTE102
	warp_def 37, 38, 0, 7, MAP_ROUTE102

PetalburgWoods_MapCoordEvents::
	coord_event 13, 19, 3, VAR_PETALBURG_WOODS_STATE, 0, PetalburgWoods_EventScript_15CCA8
	coord_event 13, 18, 3, VAR_PETALBURG_WOODS_STATE, 0, PetalburgWoods_EventScript_15CD42

PetalburgWoods_MapBGEvents::
	bg_event 38, 35, 3, BG_EVENT_PLAYER_FACING_ANY, PetalburgWoods_EventScript_15CF15
	bg_hidden_item_event 43, 34, 3, ITEM_POTION, FLAG_HIDDEN_ITEM_3A
	bg_hidden_item_event 26, 6, 3, ITEM_TINY_MUSHROOM, FLAG_HIDDEN_ITEM_3B
	bg_hidden_item_event 4, 18, 3, ITEM_POKE_BALL, FLAG_HIDDEN_ITEM_3D

PetalburgWoods_MapEvents::
	map_events PetalburgWoods_EventObjects, PetalburgWoods_MapWarps, PetalburgWoods_MapCoordEvents, PetalburgWoods_MapBGEvents
