LittlerootTown_ProfessorBirchsLab_MapScripts:: @ 8152C82
	.byte 0

TownOne_ProfLab_AideNPC::
    msgbox TownOne_ProfLab_AideNPC_Text, 2
    end

TownOne_ProfLab_AideNPC_Text::
    .string "It’s just me and the Prof.\nIn this lab...\lSometimes it gets really lonely.$"

TownOne_ProfLab_ProfNPC::
	lockall
	faceplayer
	goto_if_unset FLAG_HIDE_MOM_LITTLEROOT, TownOne_ProfLab_ProfNPC_FoundMeltan
	compare VAR_LITTLEROOT_INTRO_STATE, 2
	goto_if_eq TownOne_ProfLab_ProfNPC_IntroMeltan
	compare VAR_LITTLEROOT_INTRO_STATE, 3
	goto_if_eq TownOne_ProfLab_ProfNPC_FindMeltan
	releaseall
    end

TownOne_ProfLab_ProfNPC_IntroMeltan::
	msgbox TownOne_ProfLab_ProfNPC_TextA, 4
	setvar VAR_LITTLEROOT_INTRO_STATE, 3
	clearflag FLAG_HIDE_FAT_MAN_LITTLEROOT
	closemessage
	release
	end

TownOne_ProfLab_ProfNPC_FindMeltan::
	msgbox TownOne_ProfLab_ProfNPC_TextC, 4
	closemessage
	release
    end

TownOne_ProfLab_ProfNPC_FoundMeltan::
	msgbox TownOne_ProfLab_ProfNPC_TextB, 4
	closemessage
	release
    end

TownOne_ProfLab_ProfNPC_TextA::
    .string "{PLAYER}!\pAs my new assistant, I have a\n very important task for you!\p"
	.string "You must go into the Foil Forest\nand find a rare POKéMON\lI’ve witnessed there recently!\p"
	.string "The job is yours, {PLAYER}!$"

TownOne_ProfLab_ProfNPC_TextC::
	.string "Go to the Foil Forest!\pFind that rare POKéMON\nI’ve witnessed there!\p"
	.string "You can do it {PLAYER}!$"

TownOne_ProfLab_ProfNPC_TextB::
    .string "{PLAYER}!\pThats it!\pThats the Mythical POKéMON MELTAN\nI saw in Foil Forest!\p"
	.string "I knew I could count on you being\nmy assistant. Good job!\p"
	.string "But this is just the beginning,\n{PLAYER}.\pNow the real work begins.\pA whole new set of mysteries await\nto be uncovered.\p"
	.string "And you’ll be there to help me\nuncover them!\p"
	.string "Why don’t you \nplay with MELTAN \land get to know it some more?$"

