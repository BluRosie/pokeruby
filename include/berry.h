#ifndef GUARD_BERRY_H
#define GUARD_BERRY_H

struct Berry
{
    u8 name[7];
    u8 firmness;
    u16 size;
    u8 maxYield;
    u8 minYield;
    u8 *description1;
    u8 *description2;
    u8 stageDuration;
    u8 spicy;
    u8 dry;
    u8 sweet;
    u8 bitter;
    u8 sour;
    u8 smoothness;
};

struct EnigmaBerry
{
    struct Berry berry;
    u8 pic[(6 * 6) * TILE_SIZE_4BPP];
    u16 palette[16];
    u8 description1[45];
    u8 description2[45];
    u8 itemEffect[18];
    u8 holdEffect;
    u32 checksum;
};

struct BattleEnigmaBerry
{
    u8 name[7];
    u8 holdEffect;
    u8 itemEffect[18];
    u8 holdEffectParam;
};

#endif // GUARD_BERRY_H
