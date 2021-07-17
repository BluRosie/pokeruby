#include "global.h"
#include "rom_8077ABC.h"
#include "trig.h"
#include "battle_anim.h"
#include "constants/battle.h"

extern s16 gBattleAnimArgs[];
extern u8 gBattleAnimAttacker;
extern u8 gBattleAnimTarget;

extern void sub_80DA48C(struct Sprite *);

void AnimWorrySeed(struct Sprite* sprite);
static void AnimWorrySeedStep(struct Sprite* sprite);
static void PinkCloudAroundTarget(struct Sprite *sprite); // for the cloud
void AnimLeechSeed(struct Sprite* sprite);
static void AnimLeechSeedStep(struct Sprite* sprite);
static void AnimLeechSeedSprouts(struct Sprite* sprite);

const union AnimCmd gSpriteAnim_83D63AC[] =
{
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_END,
};

const union AnimCmd gSpriteAnim_83D63B4[] =
{
    ANIMCMD_FRAME(4, 7),
    ANIMCMD_FRAME(8, 7),
    ANIMCMD_JUMP(0),
};

const union AnimCmd *const gSpriteAnimTable_83D63C0[] =
{
    gSpriteAnim_83D63AC,
    gSpriteAnim_83D63B4,
};

const struct SpriteTemplate gLeechSeedSpriteTemplate =
{
    .tileTag = ANIM_TAG_SEED,
    .paletteTag = ANIM_TAG_SEED,
    .oam = &gOamData_837DF2C,
    .anims = gSpriteAnimTable_83D63C0,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimLeechSeed,
};

const union AffineAnimCmd gSpriteAffineAnim_WorrySeedCloud1[] =
{
    AFFINEANIMCMD_FRAME(0x80, 0x80, 0, 0),
    AFFINEANIMCMD_FRAME(0xFFFC, 0xFFFA, 0, 16),
    AFFINEANIMCMD_FRAME(0x4, 0x6, 0, 16),
    AFFINEANIMCMD_JUMP(0),
};

const union AffineAnimCmd gSpriteAffineAnim_WorrySeedCloud2[] =
{
    AFFINEANIMCMD_FRAME(0xA0, 0xA0, 0, 0),
    AFFINEANIMCMD_FRAME(0x4, 0x6, 0, 16),
    AFFINEANIMCMD_FRAME(0xFFFC, 0xFFFA, 0, 16),
    AFFINEANIMCMD_JUMP(0),
};

const struct SpriteTemplate gWorrySeedSpriteTemplate =
{
    .tileTag = ANIM_TAG_SEED,
    .paletteTag = ANIM_TAG_SEED,
    .oam = &gOamData_837DF2C,
    .anims = gSpriteAnimTable_83D63C0,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimWorrySeed,
};

const struct SpriteTemplate gHeartSwapSpriteTemplate =
{
    .tileTag = ANIM_TAG_RED_ORB,
    .paletteTag = ANIM_TAG_RED_ORB,
    .oam = &gOamData_837DF2C,
    .anims = gSpriteAnimTable_83D63C0,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimWorrySeed,
};

const union AffineAnimCmd *const gSpriteAffineAnimTable_WorrySeedCloud[] =
{
    gSpriteAffineAnim_WorrySeedCloud1,
    gSpriteAffineAnim_WorrySeedCloud2,
};

const struct OamData gWorrySeedCloudOAM =
{
    .affineMode = 3,
    .objMode = 0,
    .shape = 0,
    .size = 2,
    .priority = 1,
};

//arg[0]: affine anim table entry (0-1)
//arg[1]: x
//arg[2]: y
//arg[3]: ??? (time on screen?)
const struct SpriteTemplate gWorrySeedCloudTemplate =
{
    .tileTag = ANIM_TAG_PINK_CLOUD,
    .paletteTag = ANIM_TAG_PINK_CLOUD,
    .oam = &gWorrySeedCloudOAM,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gSpriteAffineAnimTable_WorrySeedCloud,
    .callback = PinkCloudAroundTarget,
};

// seed thrown
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: target x pixel offset
// arg 3: target y pixel offset
// arg 4: duration
// arg 5: wave amplitude
void AnimWorrySeed(struct Sprite* sprite)
{
    InitSpritePosToAnimAttacker(sprite, 1);
    if (GetBattlerSide(gBattleAnimAttacker))
    {
        gBattleAnimArgs[2] = -gBattleAnimArgs[2];
    }

    sprite->data[0] = gBattleAnimArgs[4];
    sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, 0) + gBattleAnimArgs[2];
    sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, 1) + gBattleAnimArgs[3];
    sprite->data[5] = gBattleAnimArgs[5];
    InitAnimArcTranslation(sprite);
    sprite->callback = AnimWorrySeedStep;
}

static void AnimWorrySeedStep(struct Sprite* sprite)
{
    if (TranslateAnimArc(sprite))
    {
        sprite->invisible = 1;
        sprite->data[0] = 10;
        sprite->callback = DestroyAnimSprite;
    }
}

static void PinkCloudAroundTarget(struct Sprite *sprite)
{
    sprite->data[0] = gBattleAnimArgs[3];
    StartSpriteAffineAnim(sprite, gBattleAnimArgs[0]);
    if (GetBattlerSide(gBattleAnimTarget) != B_SIDE_OPPONENT)
        gBattleAnimArgs[1] = -gBattleAnimArgs[1];

    sprite->pos1.x = GetBattlerSpriteCoord(gBattleAnimTarget, 2) + gBattleAnimArgs[1];
    sprite->pos1.y = GetBattlerSpriteCoord(gBattleAnimTarget, 3) + gBattleAnimArgs[2];
    sprite->callback = sub_80DA48C;
}

// seed (sprouts a sapling from a seed.)
// Used by Leech Seed.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: target x pixel offset
// arg 3: target y pixel offset
// arg 4: duration
// arg 5: wave amplitude
void AnimLeechSeed(struct Sprite* sprite)
{
    InitSpritePosToAnimAttacker(sprite, 1);
    if (GetBattlerSide(gBattleAnimAttacker))
    {
        gBattleAnimArgs[2] = -gBattleAnimArgs[2];
    }

    sprite->data[0] = gBattleAnimArgs[4];
    sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, 0) + gBattleAnimArgs[2];
    sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, 1) + gBattleAnimArgs[3];
    sprite->data[5] = gBattleAnimArgs[5];
    InitAnimArcTranslation(sprite);
    sprite->callback = AnimLeechSeedStep;
}

static void AnimLeechSeedStep(struct Sprite* sprite)
{
    if (TranslateAnimArc(sprite))
    {
        sprite->invisible = TRUE;
        sprite->data[0] = 10;
        sprite->callback = WaitAnimForDuration;
        StoreSpriteCallbackInData(sprite, AnimLeechSeedSprouts);
    }
}

static void AnimLeechSeedSprouts(struct Sprite* sprite)
{
    sprite->invisible = FALSE;
    StartSpriteAnim(sprite, 1);
    sprite->data[0] = 60;
    sprite->callback = WaitAnimForDuration;
    StoreSpriteCallbackInData(sprite, DestroyAnimSprite);
}
