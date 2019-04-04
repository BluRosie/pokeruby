.if DEBUG

DebugScript_081C1CFE:: @ 81C1CFE
	setflag FLAG_SYS_GAME_CLEAR
	call EverGrandeCity_HallOfFame_EventScript_19FC13
	end

DebugScript_081C1D07:: @ 81C1D07
	checkitem ITEM_COIN_CASE, 1
	compare RESULT, 1
	goto_if_eq DebugScript_081C1D1D
	giveitem ITEM_COIN_CASE, 1
	end

DebugScript_081C1D1D:: @ 81C1D1D
	end

DebugScript_081C1D1E:: @ 81C1D1E
	goto Route103_EventScript_14ED46

DebugScript_081C1D23:: @ 81C1D23
	end

DebugScript_081C1D24:: @ 81C1D24
	setvar VAR_NEW_MAUVILLE_STATE, 1
	end

DebugScript_081C1D2A:: @ 81C1D2A
	giveitem ITEM_SS_TICKET, 1
	goto DebugScript_081C1CFE

DebugScript_081C1D34:: @ 81C1D34
	end

DebugScript_081C1D35:: @ 81C1D35
	getpartysize
	compare RESULT, 6
	goto_if_eq DebugScript_081C1D45
	giveegg SPECIES_KYOGRE
	end

DebugScript_081C1D45:: @ 81C1D45
	end

DebugScript_081C1D46:: @ 81C1D46
	giveitem ITEM_POTION, 99
	giveitem ITEM_ANTIDOTE, 99
	giveitem ITEM_BURN_HEAL, 99
	giveitem ITEM_ICE_HEAL, 99
	giveitem ITEM_AWAKENING, 99
	giveitem ITEM_PARALYZE_HEAL, 99
	giveitem ITEM_FULL_RESTORE, 99
	giveitem ITEM_MAX_POTION, 99
	giveitem ITEM_HYPER_POTION, 99
	giveitem ITEM_SUPER_POTION, 99
	giveitem ITEM_FULL_HEAL, 99
	giveitem ITEM_REVIVE, 99
	giveitem ITEM_MAX_REVIVE, 99
	giveitem ITEM_FRESH_WATER, 99
	giveitem ITEM_SODA_POP, 99
	giveitem ITEM_LEMONADE, 99
	giveitem ITEM_MOOMOO_MILK, 99
	giveitem ITEM_ENERGY_POWDER, 99
	giveitem ITEM_ENERGY_ROOT, 99
	giveitem ITEM_HEAL_POWDER, 99
	giveitem ITEM_ORANGE_MAIL, 92
	giveitem ITEM_RETRO_MAIL, 94
	giveitem ITEM_POTION, 89
	giveitem ITEM_MASTER_BALL, 99
	giveitem ITEM_ULTRA_BALL, 99
	giveitem ITEM_GREAT_BALL, 99
	giveitem ITEM_POKE_BALL, 99
	giveitem ITEM_NET_BALL, 99
	giveitem ITEM_DIVE_BALL, 99
	giveitem ITEM_NEST_BALL, 99
	giveitem ITEM_REPEAT_BALL, 99
	giveitem ITEM_TIMER_BALL, 99
	giveitem ITEM_LUXURY_BALL, 99
	giveitem ITEM_PREMIER_BALL, 99
	giveitem ITEM_MASTER_BALL, 99
	giveitem ITEM_MASTER_BALL, 99
	giveitem ITEM_MASTER_BALL, 99
	giveitem ITEM_MASTER_BALL, 99
	giveitem ITEM_MASTER_BALL, 99
	giveitem ITEM_MASTER_BALL, 99
	giveitem ITEM_MASTER_BALL, 99
	giveitem ITEM_MASTER_BALL, 99
	giveitem ITEM_MASTER_BALL, 99
	giveitem ITEM_GREAT_BALL, 79
	giveitem ITEM_ULTRA_BALL, 84
	giveitem ITEM_POKE_BALL, 89
	giveitem ITEM_MASTER_BALL, 74
	giveitem ITEM_TM01, 99
	giveitem ITEM_TM02, 99
	giveitem ITEM_TM03, 99
	giveitem ITEM_TM04, 99
	giveitem ITEM_TM05, 99
	giveitem ITEM_TM06, 99
	giveitem ITEM_TM07, 99
	giveitem ITEM_TM08, 99
	giveitem ITEM_TM09, 99
	giveitem ITEM_TM10, 99
	giveitem ITEM_TM11, 99
	giveitem ITEM_TM12, 99
	giveitem ITEM_TM13, 99
	giveitem ITEM_TM14, 99
	giveitem ITEM_TM15, 99
	giveitem ITEM_TM16, 99
	giveitem ITEM_TM17, 99
	giveitem ITEM_TM18, 99
	giveitem ITEM_TM19, 99
	giveitem ITEM_TM20, 99
	giveitem ITEM_TM21, 99
	giveitem ITEM_TM22, 99
	giveitem ITEM_TM23, 99
	giveitem ITEM_TM24, 99
	giveitem ITEM_TM25, 99
	giveitem ITEM_TM26, 99
	giveitem ITEM_TM27, 99
	giveitem ITEM_TM28, 99
	giveitem ITEM_TM29, 99
	giveitem ITEM_TM30, 99
	giveitem ITEM_TM31, 99
	giveitem ITEM_TM32, 99
	giveitem ITEM_TM33, 99
	giveitem ITEM_TM34, 99
	giveitem ITEM_TM35, 99
	giveitem ITEM_TM36, 99
	giveitem ITEM_TM37, 99
	giveitem ITEM_TM38, 99
	giveitem ITEM_TM39, 99
	giveitem ITEM_TM40, 99
	giveitem ITEM_TM41, 99
	giveitem ITEM_TM42, 99
	giveitem ITEM_TM43, 99
	giveitem ITEM_TM44, 99
	giveitem ITEM_TM45, 99
	giveitem ITEM_TM46, 99
	giveitem ITEM_TM47, 99
	giveitem ITEM_TM48, 99
	giveitem ITEM_TM49, 99
	giveitem ITEM_TM50, 99
	giveitem ITEM_CHERI_BERRY, 200
	giveitem ITEM_CHERI_BERRY, 200
	giveitem ITEM_CHERI_BERRY, 200
	giveitem ITEM_CHERI_BERRY, 200
	giveitem ITEM_CHERI_BERRY, 199
	giveitem ITEM_CHESTO_BERRY, 200
	giveitem ITEM_CHESTO_BERRY, 200
	giveitem ITEM_CHESTO_BERRY, 200
	giveitem ITEM_CHESTO_BERRY, 200
	giveitem ITEM_CHESTO_BERRY, 199
	giveitem ITEM_PECHA_BERRY, 200
	giveitem ITEM_PECHA_BERRY, 200
	giveitem ITEM_PECHA_BERRY, 200
	giveitem ITEM_PECHA_BERRY, 200
	giveitem ITEM_PECHA_BERRY, 199
	giveitem ITEM_RAWST_BERRY, 200
	giveitem ITEM_RAWST_BERRY, 200
	giveitem ITEM_RAWST_BERRY, 200
	giveitem ITEM_RAWST_BERRY, 200
	giveitem ITEM_RAWST_BERRY, 199
	giveitem ITEM_ASPEAR_BERRY, 200
	giveitem ITEM_ASPEAR_BERRY, 200
	giveitem ITEM_ASPEAR_BERRY, 200
	giveitem ITEM_ASPEAR_BERRY, 200
	giveitem ITEM_ASPEAR_BERRY, 199
	giveitem ITEM_LEPPA_BERRY, 200
	giveitem ITEM_LEPPA_BERRY, 200
	giveitem ITEM_LEPPA_BERRY, 200
	giveitem ITEM_LEPPA_BERRY, 200
	giveitem ITEM_LEPPA_BERRY, 199
	giveitem ITEM_ORAN_BERRY, 200
	giveitem ITEM_ORAN_BERRY, 200
	giveitem ITEM_ORAN_BERRY, 200
	giveitem ITEM_ORAN_BERRY, 200
	giveitem ITEM_ORAN_BERRY, 199
	giveitem ITEM_PERSIM_BERRY, 200
	giveitem ITEM_PERSIM_BERRY, 200
	giveitem ITEM_PERSIM_BERRY, 200
	giveitem ITEM_PERSIM_BERRY, 200
	giveitem ITEM_PERSIM_BERRY, 199
	giveitem ITEM_LUM_BERRY, 200
	giveitem ITEM_LUM_BERRY, 200
	giveitem ITEM_LUM_BERRY, 200
	giveitem ITEM_LUM_BERRY, 200
	giveitem ITEM_LUM_BERRY, 199
	giveitem ITEM_SITRUS_BERRY, 200
	giveitem ITEM_SITRUS_BERRY, 200
	giveitem ITEM_SITRUS_BERRY, 200
	giveitem ITEM_SITRUS_BERRY, 200
	giveitem ITEM_SITRUS_BERRY, 199
	giveitem ITEM_FIGY_BERRY, 200
	giveitem ITEM_FIGY_BERRY, 200
	giveitem ITEM_FIGY_BERRY, 200
	giveitem ITEM_FIGY_BERRY, 200
	giveitem ITEM_FIGY_BERRY, 199
	giveitem ITEM_WIKI_BERRY, 200
	giveitem ITEM_WIKI_BERRY, 200
	giveitem ITEM_WIKI_BERRY, 200
	giveitem ITEM_WIKI_BERRY, 200
	giveitem ITEM_WIKI_BERRY, 199
	giveitem ITEM_MAGO_BERRY, 200
	giveitem ITEM_MAGO_BERRY, 200
	giveitem ITEM_MAGO_BERRY, 200
	giveitem ITEM_MAGO_BERRY, 200
	giveitem ITEM_MAGO_BERRY, 199
	giveitem ITEM_AGUAV_BERRY, 200
	giveitem ITEM_AGUAV_BERRY, 200
	giveitem ITEM_AGUAV_BERRY, 200
	giveitem ITEM_AGUAV_BERRY, 200
	giveitem ITEM_AGUAV_BERRY, 199
	giveitem ITEM_IAPAPA_BERRY, 200
	giveitem ITEM_IAPAPA_BERRY, 200
	giveitem ITEM_IAPAPA_BERRY, 200
	giveitem ITEM_IAPAPA_BERRY, 200
	giveitem ITEM_IAPAPA_BERRY, 199
	giveitem ITEM_RAZZ_BERRY, 200
	giveitem ITEM_RAZZ_BERRY, 200
	giveitem ITEM_RAZZ_BERRY, 200
	giveitem ITEM_RAZZ_BERRY, 200
	giveitem ITEM_RAZZ_BERRY, 199
	giveitem ITEM_BLUK_BERRY, 200
	giveitem ITEM_BLUK_BERRY, 200
	giveitem ITEM_BLUK_BERRY, 200
	giveitem ITEM_BLUK_BERRY, 200
	giveitem ITEM_BLUK_BERRY, 199
	giveitem ITEM_NANAB_BERRY, 200
	giveitem ITEM_NANAB_BERRY, 200
	giveitem ITEM_NANAB_BERRY, 200
	giveitem ITEM_NANAB_BERRY, 200
	giveitem ITEM_NANAB_BERRY, 199
	giveitem ITEM_WEPEAR_BERRY, 200
	giveitem ITEM_WEPEAR_BERRY, 200
	giveitem ITEM_WEPEAR_BERRY, 200
	giveitem ITEM_WEPEAR_BERRY, 200
	giveitem ITEM_WEPEAR_BERRY, 199
	giveitem ITEM_PINAP_BERRY, 200
	giveitem ITEM_PINAP_BERRY, 200
	giveitem ITEM_PINAP_BERRY, 200
	giveitem ITEM_PINAP_BERRY, 200
	giveitem ITEM_PINAP_BERRY, 199
	giveitem ITEM_POMEG_BERRY, 200
	giveitem ITEM_POMEG_BERRY, 200
	giveitem ITEM_POMEG_BERRY, 200
	giveitem ITEM_POMEG_BERRY, 200
	giveitem ITEM_POMEG_BERRY, 199
	giveitem ITEM_KELPSY_BERRY, 200
	giveitem ITEM_KELPSY_BERRY, 200
	giveitem ITEM_KELPSY_BERRY, 200
	giveitem ITEM_KELPSY_BERRY, 200
	giveitem ITEM_KELPSY_BERRY, 199
	giveitem ITEM_QUALOT_BERRY, 200
	giveitem ITEM_QUALOT_BERRY, 200
	giveitem ITEM_QUALOT_BERRY, 200
	giveitem ITEM_QUALOT_BERRY, 200
	giveitem ITEM_QUALOT_BERRY, 199
	giveitem ITEM_HONDEW_BERRY, 200
	giveitem ITEM_HONDEW_BERRY, 200
	giveitem ITEM_HONDEW_BERRY, 200
	giveitem ITEM_HONDEW_BERRY, 200
	giveitem ITEM_HONDEW_BERRY, 199
	giveitem ITEM_GREPA_BERRY, 200
	giveitem ITEM_GREPA_BERRY, 200
	giveitem ITEM_GREPA_BERRY, 200
	giveitem ITEM_GREPA_BERRY, 200
	giveitem ITEM_GREPA_BERRY, 199
	giveitem ITEM_TAMATO_BERRY, 200
	giveitem ITEM_TAMATO_BERRY, 200
	giveitem ITEM_TAMATO_BERRY, 200
	giveitem ITEM_TAMATO_BERRY, 200
	giveitem ITEM_TAMATO_BERRY, 199
	giveitem ITEM_CORNN_BERRY, 200
	giveitem ITEM_CORNN_BERRY, 200
	giveitem ITEM_CORNN_BERRY, 200
	giveitem ITEM_CORNN_BERRY, 200
	giveitem ITEM_CORNN_BERRY, 199
	giveitem ITEM_MAGOST_BERRY, 999
	giveitem ITEM_RABUTA_BERRY, 999
	giveitem ITEM_NOMEL_BERRY, 999
	giveitem ITEM_SPELON_BERRY, 999
	giveitem ITEM_PAMTRE_BERRY, 999
	giveitem ITEM_WATMEL_BERRY, 999
	giveitem ITEM_DURIN_BERRY, 999
	giveitem ITEM_BELUE_BERRY, 999
	giveitem ITEM_LIECHI_BERRY, 999
	giveitem ITEM_GANLON_BERRY, 999
	giveitem ITEM_SALAC_BERRY, 999
	giveitem ITEM_PETAYA_BERRY, 999
	giveitem ITEM_APICOT_BERRY, 999
	giveitem ITEM_LANSAT_BERRY, 999
	giveitem ITEM_STARF_BERRY, 999
	giveitem ITEM_ENIGMA_BERRY, 999
	end

DebugScript_081C221F:: @ 81C221F
	givedecoration DECOR_SMALL_DESK
	givedecoration DECOR_SMALL_DESK
	givedecoration DECOR_SMALL_DESK
	givedecoration DECOR_SMALL_DESK
	givedecoration DECOR_SMALL_DESK
	givedecoration DECOR_SMALL_DESK
	givedecoration DECOR_SMALL_DESK
	givedecoration DECOR_SMALL_DESK
	givedecoration DECOR_SMALL_DESK
	givedecoration DECOR_SMALL_DESK
	givedecoration DECOR_SMALL_CHAIR
	givedecoration DECOR_SMALL_CHAIR
	givedecoration DECOR_SMALL_CHAIR
	givedecoration DECOR_SMALL_CHAIR
	givedecoration DECOR_SMALL_CHAIR
	givedecoration DECOR_SMALL_CHAIR
	givedecoration DECOR_SMALL_CHAIR
	givedecoration DECOR_SMALL_CHAIR
	givedecoration DECOR_SMALL_CHAIR
	givedecoration DECOR_SMALL_CHAIR
	givedecoration DECOR_RED_PLANT
	givedecoration DECOR_RED_PLANT
	givedecoration DECOR_RED_PLANT
	givedecoration DECOR_RED_PLANT
	givedecoration DECOR_RED_PLANT
	givedecoration DECOR_RED_PLANT
	givedecoration DECOR_RED_PLANT
	givedecoration DECOR_RED_PLANT
	givedecoration DECOR_RED_PLANT
	givedecoration DECOR_RED_PLANT
	givedecoration DECOR_RED_BRICK
	givedecoration DECOR_RED_BRICK
	givedecoration DECOR_RED_BRICK
	givedecoration DECOR_RED_BRICK
	givedecoration DECOR_RED_BRICK
	givedecoration DECOR_RED_BRICK
	givedecoration DECOR_RED_BRICK
	givedecoration DECOR_RED_BRICK
	givedecoration DECOR_RED_BRICK
	givedecoration DECOR_RED_BRICK
	givedecoration DECOR_RED_BRICK
	givedecoration DECOR_RED_BRICK
	givedecoration DECOR_RED_BRICK
	givedecoration DECOR_RED_BRICK
	givedecoration DECOR_RED_BRICK
	givedecoration DECOR_RED_BRICK
	givedecoration DECOR_RED_BRICK
	givedecoration DECOR_RED_BRICK
	givedecoration DECOR_RED_BRICK
	givedecoration DECOR_RED_BRICK
	givedecoration DECOR_RED_BRICK
	givedecoration DECOR_RED_BRICK
	givedecoration DECOR_RED_BRICK
	givedecoration DECOR_RED_BRICK
	givedecoration DECOR_RED_BRICK
	givedecoration DECOR_RED_BRICK
	givedecoration DECOR_RED_BRICK
	givedecoration DECOR_RED_BRICK
	givedecoration DECOR_RED_BRICK
	givedecoration DECOR_RED_BRICK
	givedecoration DECOR_JUMP_MAT
	givedecoration DECOR_JUMP_MAT
	givedecoration DECOR_JUMP_MAT
	givedecoration DECOR_JUMP_MAT
	givedecoration DECOR_JUMP_MAT
	givedecoration DECOR_JUMP_MAT
	givedecoration DECOR_JUMP_MAT
	givedecoration DECOR_JUMP_MAT
	givedecoration DECOR_JUMP_MAT
	givedecoration DECOR_JUMP_MAT
	givedecoration DECOR_JUMP_MAT
	givedecoration DECOR_JUMP_MAT
	givedecoration DECOR_JUMP_MAT
	givedecoration DECOR_JUMP_MAT
	givedecoration DECOR_JUMP_MAT
	givedecoration DECOR_JUMP_MAT
	givedecoration DECOR_JUMP_MAT
	givedecoration DECOR_JUMP_MAT
	givedecoration DECOR_JUMP_MAT
	givedecoration DECOR_JUMP_MAT
	givedecoration DECOR_JUMP_MAT
	givedecoration DECOR_JUMP_MAT
	givedecoration DECOR_JUMP_MAT
	givedecoration DECOR_JUMP_MAT
	givedecoration DECOR_JUMP_MAT
	givedecoration DECOR_JUMP_MAT
	givedecoration DECOR_JUMP_MAT
	givedecoration DECOR_JUMP_MAT
	givedecoration DECOR_JUMP_MAT
	givedecoration DECOR_JUMP_MAT
	givedecoration DECOR_BALL_POSTER
	givedecoration DECOR_BALL_POSTER
	givedecoration DECOR_BALL_POSTER
	givedecoration DECOR_BALL_POSTER
	givedecoration DECOR_BALL_POSTER
	givedecoration DECOR_BALL_POSTER
	givedecoration DECOR_BALL_POSTER
	givedecoration DECOR_BALL_POSTER
	givedecoration DECOR_BALL_POSTER
	givedecoration DECOR_BALL_POSTER
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_DUSKULL_DOLL
	givedecoration DECOR_KISS_CUSHION
	givedecoration DECOR_KISS_CUSHION
	givedecoration DECOR_KISS_CUSHION
	givedecoration DECOR_KISS_CUSHION
	givedecoration DECOR_KISS_CUSHION
	givedecoration DECOR_KISS_CUSHION
	givedecoration DECOR_KISS_CUSHION
	givedecoration DECOR_KISS_CUSHION
	givedecoration DECOR_KISS_CUSHION
	givedecoration DECOR_KISS_CUSHION
	end

DebugScript_081C23E2:: @ 81C23E2
	givecoins 9999
	end

DebugScript_081C23E6:: @ 81C23E6
	setflag FLAG_LEGEND_ESCAPED_SEAFLOOR_CAVERN
	setflag FLAG_LEGENDARY_BATTLE_COMPLETED
	warp MAP_SOOTOPOLIS_CITY, 0xFF, 31, 33
	waitstate
	end

DebugScript_081C23F6:: @ 81C23F6
	takemoney 999999, 0x00
	end

DebugScript_081C23FD:: @ 81C23FD
	getpartysize
	compare RESULT, 1
	goto_if_eq DebugScript_081C2436
	compare RESULT, 2
	goto_if_eq DebugScript_081C2445
	compare RESULT, 3
	goto_if_eq DebugScript_081C2454
	compare RESULT, 4
	goto_if_eq DebugScript_081C2463
	compare RESULT, 5
	goto_if_eq DebugScript_081C2472
	end

DebugScript_081C2436:: @ 81C2436
	givemon SPECIES_BARBOACH, 99, 0, 0, 0, 0
DebugScript_081C2445:
	givemon SPECIES_BARBOACH, 99, 0, 0, 0, 0
DebugScript_081C2454:
	givemon SPECIES_BARBOACH, 99, 0, 0, 0, 0
DebugScript_081C2463:
	givemon SPECIES_BARBOACH, 99, 0, 0, 0, 0
DebugScript_081C2472:
	givemon SPECIES_BARBOACH, 99, 0, 0, 0, 0
	end

DebugScript_081C2482:: @ 81C2482
	getpartysize
	compare RESULT, 1
	goto_if_eq DebugScript_081C24BB
	compare RESULT, 2
	goto_if_eq DebugScript_081C24CA
	compare RESULT, 3
	goto_if_eq DebugScript_081C24D9
	compare RESULT, 4
	goto_if_eq DebugScript_081C24E8
	compare RESULT, 5
	goto_if_eq DebugScript_081C24F7
	end

DebugScript_081C24BB:: @ 81C24BB
	givemon SPECIES_SHROOMISH, 99, 0, 0, 0, 0
DebugScript_081C24CA:
	givemon SPECIES_SHROOMISH, 99, 0, 0, 0, 0
DebugScript_081C24D9:
	givemon SPECIES_SHROOMISH, 99, 0, 0, 0, 0
DebugScript_081C24E8:
	givemon SPECIES_SHROOMISH, 99, 0, 0, 0, 0
DebugScript_081C24F7:
	givemon SPECIES_SHROOMISH, 99, 0, 0, 0, 0
	end

DebugScript_081C2507:: @ 81C2507
	getpartysize
	compare RESULT, 6
	goto_if_eq DebugScript_081C2517
	giveegg SPECIES_BARBOACH
	end

DebugScript_081C2517:: @ 81C2517
	end

DebugScript_081C2518:: @ 81C2518
	getpartysize
	compare RESULT, 6
	goto_if_eq DebugScript_081C2528
	giveegg SPECIES_SHROOMISH
	end

DebugScript_081C2528:: @ 81C2528
	end

.endif
