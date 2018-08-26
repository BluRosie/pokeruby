	.include "include/macros.inc"
	.include "constants/constants.inc"

	.section .rodata

	.align 2
gBallOpenParticleSpriteSheets:: @ 840B258
	obj_tiles gBattleAnimSpriteSheet_Particles, 0x100, 55020
	obj_tiles gBattleAnimSpriteSheet_Particles, 0x100, 55021
	obj_tiles gBattleAnimSpriteSheet_Particles, 0x100, 55022
	obj_tiles gBattleAnimSpriteSheet_Particles, 0x100, 55023
	obj_tiles gBattleAnimSpriteSheet_Particles, 0x100, 55024
	obj_tiles gBattleAnimSpriteSheet_Particles, 0x100, 55025
	obj_tiles gBattleAnimSpriteSheet_Particles, 0x100, 55026
	obj_tiles gBattleAnimSpriteSheet_Particles, 0x100, 55027
	obj_tiles gBattleAnimSpriteSheet_Particles, 0x100, 55028
	obj_tiles gBattleAnimSpriteSheet_Particles, 0x100, 55029
	obj_tiles gBattleAnimSpriteSheet_Particles, 0x100, 55030
	obj_tiles gBattleAnimSpriteSheet_Particles, 0x100, 55031
	obj_tiles gBattleAnimSpriteSheet_Particles, 0x100, 65295
	obj_tiles gBattleAnimSpriteSheet_Particles, 0x100, 65296
	obj_tiles gBattleAnimSpriteSheet_Particles, 0x100, 65297
	obj_tiles gBattleAnimSpriteSheet_Particles, 0x100, 65298
	obj_tiles gBattleAnimSpriteSheet_Particles, 0x100, 65299
	obj_tiles gBattleAnimSpriteSheet_Particles, 0x100, 65300
	obj_tiles gBattleAnimSpriteSheet_Particles, 0x100, 65301
	obj_tiles gBattleAnimSpriteSheet_Particles, 0x100, 65302
	obj_tiles gBattleAnimSpriteSheet_Particles, 0x100, 65303
	obj_tiles gBattleAnimSpriteSheet_Particles, 0x100, 65304
	obj_tiles gBattleAnimSpriteSheet_Particles, 0x100, 65305
	obj_tiles gBattleAnimSpriteSheet_Particles, 0x100, 65306
	obj_tiles gBattleAnimSpriteSheet_Particles, 0x100, 65307
	obj_tiles gBattleAnimSpriteSheet_Particles, 0x100, 65308
	obj_tiles gBattleAnimSpriteSheet_Particles, 0x100, 65309

	.align 2
gBallOpenParticlePalettes:: @ 840B2B8
	obj_pal gBattleAnimSpritePalette_136, 55020
	obj_pal gBattleAnimSpritePalette_136, 55021
	obj_pal gBattleAnimSpritePalette_136, 55022
	obj_pal gBattleAnimSpritePalette_136, 55023
	obj_pal gBattleAnimSpritePalette_136, 55024
	obj_pal gBattleAnimSpritePalette_136, 55025
	obj_pal gBattleAnimSpritePalette_136, 55026
	obj_pal gBattleAnimSpritePalette_136, 55027
	obj_pal gBattleAnimSpritePalette_136, 55028
	obj_pal gBattleAnimSpritePalette_136, 55029
	obj_pal gBattleAnimSpritePalette_136, 55030
	obj_pal gBattleAnimSpritePalette_136, 55031
	obj_pal gBattleAnimSpritePalette_136, 65295
	obj_pal gBattleAnimSpritePalette_136, 65296
	obj_pal gBattleAnimSpritePalette_136, 65297
	obj_pal gBattleAnimSpritePalette_136, 65298
	obj_pal gBattleAnimSpritePalette_136, 65299
	obj_pal gBattleAnimSpritePalette_136, 65300
	obj_pal gBattleAnimSpritePalette_136, 65301
	obj_pal gBattleAnimSpritePalette_136, 65302
	obj_pal gBattleAnimSpritePalette_136, 65303
	obj_pal gBattleAnimSpritePalette_136, 65304
	obj_pal gBattleAnimSpritePalette_136, 65305
	obj_pal gBattleAnimSpritePalette_136, 65306
	obj_pal gBattleAnimSpritePalette_136, 65307
	obj_pal gBattleAnimSpritePalette_136, 65308
	obj_pal gBattleAnimSpritePalette_136, 65309

	.align 2
gSpriteAnim_840B318:: @ 840B318
	obj_image_anim_frame 0, 1
	obj_image_anim_frame 1, 1
	obj_image_anim_frame 2, 1
	obj_image_anim_frame 0, 1, OBJ_IMAGE_ANIM_H_FLIP
	obj_image_anim_frame 2, 1
	obj_image_anim_frame 1, 1
	obj_image_anim_jump 0

	.align 2
gSpriteAnim_840B334:: @ 840B334
	obj_image_anim_frame 3, 1
	obj_image_anim_end

	.align 2
gSpriteAnim_840B33C:: @ 840B33C
	obj_image_anim_frame 4, 1
	obj_image_anim_end

	.align 2
gSpriteAnim_840B344:: @ 840B344
	obj_image_anim_frame 5, 1
	obj_image_anim_end

	.align 2
gSpriteAnim_840B34C:: @ 840B34C
	obj_image_anim_frame 6, 4
	obj_image_anim_frame 7, 4
	obj_image_anim_jump 0

	.align 2
gSpriteAnim_840B358:: @ 840B358
	obj_image_anim_frame 7, 4
	obj_image_anim_end

	.align 2
gSpriteAnimTable_840B360:: @ 840B360
	.4byte gSpriteAnim_840B318
	.4byte gSpriteAnim_840B334
	.4byte gSpriteAnim_840B33C
	.4byte gSpriteAnim_840B344
	.4byte gSpriteAnim_840B34C
	.4byte gSpriteAnim_840B358

gBallOpenParticleAnimNums:: @ 840B378 @ type of particle per ball
	.byte 0, 0, 0, 5, 1, 2, 2, 3, 5, 5, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1

	.align 2
gBallOpenParticleAnimationFuncs:: @ 840B384 @ particle animation table?  particle animation table per ball
	.4byte PokeBallOpenParticleAnimation
	.4byte GreatBallOpenParticleAnimation
	.4byte SafariBallOpenParticleAnimation
	.4byte UltraBallOpenParticleAnimation
	.4byte MasterBallOpenParticleAnimation
	.4byte SafariBallOpenParticleAnimation
	.4byte DiveBallOpenParticleAnimation
	.4byte UltraBallOpenParticleAnimation
	.4byte RepeatBallOpenParticleAnimation
	.4byte TimerBallOpenParticleAnimation
	.4byte GreatBallOpenParticleAnimation
	.4byte PremierBallOpenParticleAnimation
	.4byte GreatBallOpenParticleAnimation @ level ball
	.4byte GreatBallOpenParticleAnimation @ lure ball
	.4byte GreatBallOpenParticleAnimation @ moon ball
	.4byte GreatBallOpenParticleAnimation @ friend ball
	.4byte GreatBallOpenParticleAnimation @ love ball
	.4byte GreatBallOpenParticleAnimation @ heavy ball
	.4byte GreatBallOpenParticleAnimation @ fast ball
	.4byte GreatBallOpenParticleAnimation @ sport ball
	.4byte GreatBallOpenParticleAnimation @ heal ball
	.4byte GreatBallOpenParticleAnimation @ quick ball
	.4byte GreatBallOpenParticleAnimation @ dusk ball
	.4byte GreatBallOpenParticleAnimation @ cherish ball
	.4byte GreatBallOpenParticleAnimation @ park ball
	.4byte GreatBallOpenParticleAnimation @ dream ball
	.4byte GreatBallOpenParticleAnimation @ cherish ball

	.align 2
gSpriteTemplates_840B3B4:: @ 840B3B4
	spr_template 55020, 55020, gOamData_837DF24, gSpriteAnimTable_840B360, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
	spr_template 55021, 55021, gOamData_837DF24, gSpriteAnimTable_840B360, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
	spr_template 55022, 55022, gOamData_837DF24, gSpriteAnimTable_840B360, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
	spr_template 55023, 55023, gOamData_837DF24, gSpriteAnimTable_840B360, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
	spr_template 55024, 55024, gOamData_837DF24, gSpriteAnimTable_840B360, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
	spr_template 55025, 55025, gOamData_837DF24, gSpriteAnimTable_840B360, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
	spr_template 55026, 55026, gOamData_837DF24, gSpriteAnimTable_840B360, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
	spr_template 55027, 55027, gOamData_837DF24, gSpriteAnimTable_840B360, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
	spr_template 55028, 55028, gOamData_837DF24, gSpriteAnimTable_840B360, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
	spr_template 55029, 55029, gOamData_837DF24, gSpriteAnimTable_840B360, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
	spr_template 55030, 55030, gOamData_837DF24, gSpriteAnimTable_840B360, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
	spr_template 55031, 55031, gOamData_837DF24, gSpriteAnimTable_840B360, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
	spr_template 65295, 65295, gOamData_837DF24, gSpriteAnimTable_840B360, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
	spr_template 65296, 65296, gOamData_837DF24, gSpriteAnimTable_840B360, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
	spr_template 65297, 65297, gOamData_837DF24, gSpriteAnimTable_840B360, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
	spr_template 65298, 65298, gOamData_837DF24, gSpriteAnimTable_840B360, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
	spr_template 65299, 65299, gOamData_837DF24, gSpriteAnimTable_840B360, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
	spr_template 65300, 65300, gOamData_837DF24, gSpriteAnimTable_840B360, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
	spr_template 65301, 65301, gOamData_837DF24, gSpriteAnimTable_840B360, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
	spr_template 65302, 65302, gOamData_837DF24, gSpriteAnimTable_840B360, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
	spr_template 65303, 65303, gOamData_837DF24, gSpriteAnimTable_840B360, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
	spr_template 65304, 65304, gOamData_837DF24, gSpriteAnimTable_840B360, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
	spr_template 65305, 65305, gOamData_837DF24, gSpriteAnimTable_840B360, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
	spr_template 65306, 65306, gOamData_837DF24, gSpriteAnimTable_840B360, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
	spr_template 65307, 65307, gOamData_837DF24, gSpriteAnimTable_840B360, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
	spr_template 65308, 65308, gOamData_837DF24, gSpriteAnimTable_840B360, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
	spr_template 65309, 65309, gOamData_837DF24, gSpriteAnimTable_840B360, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy

	.align 1
gUnknown_0840B4D4:: @ 840B4D4
	.2byte 0x7ADF
	.2byte 0x7AF0
	.2byte 0x53D7
	.2byte 0x3FFF
	.2byte 0x7297
	.2byte 0x67F5
	.2byte 0x7B2C
	.2byte 0x2B7E
	.2byte 0x431F
	.2byte 0x7BDD
	.2byte 0x2A3F
	.2byte 0x293F
	.2byte    0x0
	.2byte  0x201
	.2byte  0x403
	.2byte  0x101
	.2byte  0x100
	.2byte  0x503
	.2byte  0x506
	.2byte    0x4

	.align 2
gBattleAnimSpriteTemplate_840B4FC:: @ 840B4FC
	spr_template 10269, 10269, gOamData_837DF2C, gDummySpriteAnimTable, NULL, gDummySpriteAffineAnimTable, sub_8141C30
