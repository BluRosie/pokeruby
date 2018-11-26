LittlerootTown_BrendansHouse_2F_MapScripts:: @ 81527A4
	map_script 2, LittlerootTown_BrendansHouse_2F_MapScript1_1527AF
	.byte 0

LittlerootTown_BrendansHouse_2F_MapScript1_1527AF:: @ 81527AF
	map_script_2 EVENT_VAR_MELTANHACK, 0, TownOne_PlayerHouse2F_LevelScript
	.2byte 0

TownOne_PlayerHouse2F_LevelScript::
	lockall
	applymovement 0xFF, PlayerMovementHouse1
	msgbox TownOne_PlayerHouse2F_MomTextA, 4
	waitmovement
	msgbox TownOne_PlayerHouse2F_MomTextB, 4
	setvar EVENT_VAR_MELTANHACK, 1
	setflag FLAG_RECEIVED_RUNNING_SHOES
	releaseall
	end

PlayerMovementHouse1::
    face_right
    end_movement

TownOne_PlayerHouse2F_MomTextA::
.string "Mom: {PLAYER}, dear!\nIts time to get ready!"

TownOne_PlayerHouse2F_MomTextB::
.string "Come downstairs when youre ready!\nIts your big day!"

gUnknown_0815281E:: @ 815281E
gBrendanHouse_TurnPCOff:: @ 8152850
    setvar VAR_SPECIAL_4, 1
    playse SE_PC_OFF
    special DoPCTurnOffEffect
    releaseall
    end

