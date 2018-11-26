OldaleTown_House1_EventObjects::
	object_event 1, EVENT_OBJ_GFX_WOMAN_3, 0, 6, 4, 3, MOVEMENT_TYPE_WANDER_AROUND, 1, 1, 0, 0, OldaleTown_House1_EventScript_152EEC, 0
	object_event 2, EVENT_OBJ_GFX_MELTAN, 0, 0, 0, 3, MOVEMENT_TYPE_LOOK_AROUND, 0, 0, FALSE, 0, NULL, FLAG_HIDE_MOM_LITTLEROOT

OldaleTown_House1_MapWarps::
	warp_def 3, 8, 0, 0, MAP_OLDALE_TOWN
	warp_def 4, 8, 0, 0, MAP_OLDALE_TOWN

OldaleTown_House1_MapEvents::
	map_events OldaleTown_House1_EventObjects, OldaleTown_House1_MapWarps, 0x0, 0x0
