LittlerootTown_MaysHouse_2F_MapScripts:: @ 8152A50
	.byte 0

TownOne_ProfHouse_ChildNPC::
    msgbox TownOne_ProfHouse_ChildNPC_Text, 2
    end

TownOne_ProfHouse_ChildNPC_Text::
    .string "When I grow up, I wanna be a\nresearcher like daddy!$"

gUnknown_08152C39:: @ 8152C39
gMayHouse_TurnPCOff:: @ 8152C75
    setvar VAR_SPECIAL_4, 2
    playse SE_PC_OFF
    special DoPCTurnOffEffect
    releaseall
    end

