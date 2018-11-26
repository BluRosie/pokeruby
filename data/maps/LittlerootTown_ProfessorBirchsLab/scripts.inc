LittlerootTown_ProfessorBirchsLab_MapScripts:: @ 8152C82
	.byte 0

TownOne_ProfLab_AideNPC::
    msgbox TownOne_ProfLab_AideNPC_Text, 2
    end

TownOne_ProfLab_AideNPC_Text::
    .string "It’s just me and the Prof.\nIn this lab...\lSometimes it gets really lonely.$"

TownOne_ProfLab_ProfNPC::
	lock
	faceplayer
	goto_if_set FLAG_HIDE_MOM_LITTLEROOT, TownOne_ProfLab_ProfNPC_TextA
	msgbox TownOne_ProfLab_ProfNPC_FoundMeltan, 4
	closemessage
    end

TownOne_ProfLab_ProfNPC_FoundMeltan::
	msgbox TownOne_ProfLab_ProfNPC_TextB, 4
	setvar VAR_LITTLEROOT_INTRO_STATE, 3
	closemessage
    end

TownOne_ProfLab_ProfNPC_TextA::
    .string "{PLAYER}!\pAs my new assistant, I have a very\nimportant task for you!\p"
	.string "You must go into the Foil Forest\nand find a rare Pokemon I’ve\lwitnessed there recently!\p"
	.string "The job is yours, {PLAYER}!$"

TownOne_ProfLab_ProfNPC_TextB::
    .string "{PLAYER}!\pThats it!\nThats the Mythical Pokemon Meltan\lI saw in Foil Forest!\p"
	.string "I knew I could count on you being\nmy assistant. Good job!\p"
	.string "But this is just the beginning,\n{PLAYER}.\lNow the real work begins.\pA whole new set of mysteries await\nto be uncovered.\p"
	.string "And you’ll be there to help me\nuncover them!\p"
	.string "For now though, why don’t you play\nwith that Meltan and get to know\lit some more?$"

