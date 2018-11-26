LittlerootTown_BrendansHouse_1F_MapScripts:: @ 815263B
	map_script 2, LittlerootTown_BrendansHouse_1F_MapScript2_1526D0
	.byte 0

LittlerootTown_BrendansHouse_1F_MapScript2_1526D0:: @ 81526D0
	map_script_2 VAR_LITTLEROOT_INTRO_STATE, 1, TownOne_PlayerHouse1F_LevelScript
	.2byte 0

TownOne_PlayerHouse1F_LevelScript::
	lockall
	applymovement 255, PlayerMovementHouse2
	waitmovement 0
	msgbox TownOne_PlayerHouse1F_MomTextA, 4
	applymovement 2, HoundourHop
	msgbox TownOne_PlayerHouse1F_HoundourA, 4
	waitmovement 0
	msgbox TownOne_PlayerHouse1F_MomTextB, 4
	applymovement 2, HoundourHop
	msgbox TownOne_PlayerHouse1F_HoundourB, 4
	waitmovement 0
	msgbox TownOne_PlayerHouse1F_MomTextC, 4
	applymovement 255, WalktoHoundour
	waitmovement 0
	removeobject 2
	playmoncry SPECIES_HOUNDOUR, 0
	playfanfare MUS_FANFA4
	message HoundourJoined
	waitfanfare
	waitmessage
	givemon SPECIES_HOUNDOUR, 15, ITEM_NONE, 0x0, 0x0, 0
	setflag FLAG_HIDE_BELDUM_BALL_STEVENS_HOUSE
	setflag FLAG_SYS_POKEMON_GET
	setvar VAR_LITTLEROOT_INTRO_STATE, 2
	releaseall
	end

PlayerMovementHouse2::
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	end_movement

WalktoHoundour::
	walk_right
	walk_down
	end_movement

HoundourHop::
	jump_in_place_left
	end_movement

TownOne_PlayerHouse1F_MomTextA::
.string "Mom: {PLAYER}, you must be excited!\pIt’s your first day as\nProf. Alloy’s assistant!$"

TownOne_PlayerHouse1F_HoundourA::
.string "HOUNDOUR: Gruff!$"

TownOne_PlayerHouse1F_MomTextB::
.string "Mom: See? Even Houndour’s excited!\p"
.string "Actually, Prof. Alloy called a\nlittle while ago.\lHe wants to see you right away!\p"
.string "If you’re starting out on a\njourney as the Professors\lassistant, {PLAYER}, you really\lshould take HOUNDOUR along.\p"
.string "When you’re just at home, things\nare different.\p"
.string "But if you’re out traveling, you\nshould know Tellum isn’t like\lother regions.\p"
.string "Theres all kinds of strong\nPokémon out there.\p"
.string "The Tellum frontier is not for the\nfaint of heart!\p"
.string "I know that first-hand from my own\ndays as a Trainer!$"

TownOne_PlayerHouse1F_HoundourB::
.string "HOUNDOUR: Grrruff!!$"

TownOne_PlayerHouse1F_MomTextC::
.string "Mom: I think thats HOUNDOUR’s way\n of saying, Count me in!\p"
.string "Mom: You two will make an\nexcellent team!\pNow, you better get going.\pYou dont want to keep the\nProfessor waiting!$"

HoundourJoined::
.string "{PLAYER} brought HOUNDOUR along!$"

