#ifndef GUARD_ITEM_EFFECTS_H
#define GUARD_ITEM_EFFECTS_H

/*
[0]
CURE_INFATUATION | SACRED_ASH | RAISE_CRITICAL | FREE    FREE | FREE | FREE | X_ATTACK
[1]
FREE | FREE | FREE | RAISE_DEFENSE    FREE | FREE | FREE | RAISE_SPEED
[2]
FREE | FREE | FREE | RAISE_ACCURACY    FREE | FREE | FREE | RAISE_SP_ATK
[3]
PREVENT_STAT_LOSS | RAISE_LEVEL | CURE_SLEEP | CURE_POISON   CURE_BURN | CURE_ICE | CURE_PARALYSIS | CURE_CONFUSION
[4]
EVO_STONE | REVIVES | PP_UP | LIMITED_PP_RESTORE_ITEM    PP_ITEM | HEALING_ITEM | EV_ATTACK | EV_HP
[5]
what | the | fuck | are these    EV_SP_ATK | EV_SP_DEF | EV_SPEED | EV_DEFENSE
[6]
amount hp/pp/ev restored/added on
[7]
amount to raise friendship by when horrible, is signed
[8]
amount to raise friendship by when okay, is signed
[9]
amount to raise friendship by when great, is signed
*/

const u8 gItemEffect_Potion[]       = {0, 
                                       0, 
                                       0, 
                                       0, 
                                       HEALING_ITEM, 
                                       0, 
                                       20};

const u8 gItemEffect_AttractHeal[]  = {CURE_INFATUATION, 
                                       0, 
                                       0, 
                                       0, 
                                       0,
                                       0};

const u8 gItemEffect_ConfuseHeal[]  = {0, 
                                       0, 
                                       0, 
                                       CURE_CONFUSION, 
                                       0, 
                                       0};

const u8 gItemEffect_PoisonHeal[]   = {0, 
                                       0, 
                                       0, 
                                       CURE_POISON, 
                                       0, 
                                       0};

const u8 gItemEffect_BurnHeal[]     = {0, 
                                       0, 
                                       0, 
                                       CURE_BURN, 
                                       0, 
                                       0};

const u8 gItemEffect_IceHeal[]      = {0, 
                                       0, 
                                       0, 
                                       CURE_ICE, 
                                       0, 
                                       0};

const u8 gItemEffect_SleepHeal[]    = {0, 
                                       0, 
                                       0, 
                                       CURE_SLEEP, 
                                       0, 
                                       0};

const u8 gItemEffect_ParalyzeHeal[] = {0, 
                                       0, 
                                       0, 
                                       CURE_PARALYSIS, 
                                       0, 
                                       0};

const u8 gItemEffect_FullRestore[]  = {0, 
                                       0, 
                                       0, 
                                       CURE_SLEEP | CURE_POISON | CURE_BURN | CURE_ICE | CURE_PARALYSIS | CURE_CONFUSION, 
                                       HEALING_ITEM, 
                                       0, 
                                       HEAL_FULL_HP};

const u8 gItemEffect_MaxPotion[]    = {0, 
                                       0, 
                                       0, 
                                       0, 
                                       HEALING_ITEM, 
                                       0, 
                                       HEAL_FULL_HP};

const u8 gItemEffect_HyperPotion[]  = {0, 
                                       0, 
                                       0, 
                                       0, 
                                       HEALING_ITEM, 
                                       0, 
                                       200};

const u8 gItemEffect_SuperPotion[]  = {0, 
                                       0, 
                                       0, 
                                       0, 
                                       HEALING_ITEM, 
                                       0, 
                                       50};

const u8 gItemEffect_FullHeal[]     = {0x00, 0x00, 0x00, 0x3f, 0x00, 0x00};

const u8 gItemEffect_Revive[]       = {0x00, 0x00, 0x00, 0x00, 0x44, 0x00, 0xfe};

const u8 gItemEffect_MaxRevive[]    = {0x00, 0x00, 0x00, 0x00, 0x44, 0x00, 0xff};

const u8 gItemEffect_FreshWater[]   = {0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 50};

const u8 gItemEffect_SodaPop[]      = {0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 60};

const u8 gItemEffect_Lemonade[]     = {0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 80};

const u8 gItemEffect_MoomooMilk[]   = {0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 100};

const u8 gItemEffect_EnergyPowder[] = {0x00, 0x00, 0x00, 0x00, 0x04, 0xe0, 50,   0xfb, 0xfb, 0xf6};

const u8 gItemEffect_EnergyRoot[]   = {0x00, 0x00, 0x00, 0x00, 0x04, 0xe0, 200,  0xf6, 0xf6, 0xf1};

const u8 gItemEffect_HealPowder[]   = {0x00, 0x00, 0x00, 0x3f, 0x00, 0xe0, 0xfb, 0xfb, 0xf6};

const u8 gItemEffect_RevivalHerb[]  = {0x00, 0x00, 0x00, 0x00, 0x44, 0xe0, 0xfe, 0xf1, 0xf1, 0xec};

const u8 gItemEffect_Restore10PP[]  = {0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x0a};

const u8 gItemEffect_MaxEther[]     = {0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x7f};

const u8 gItemEffect_Elixir[]       = {0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x0a};

const u8 gItemEffect_MaxElixir[]    = {0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x7f};

const u8 gItemEffect_SacredAsh[]    = {0x40, 0x00, 0x00, 0x00, 0x44, 0x00, 0xff};

const u8 gItemEffect_HPUp[]         = {0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x0a, 0x05, 0x03, 0x02};

const u8 gItemEffect_Protein[]      = {0x00, 0x00, 0x00, 0x00, 0x02, 0xe0, 0x0a, 0x05, 0x03, 0x02};

const u8 gItemEffect_Iron[]         = {0x00, 0x00, 0x00, 0x00, 0x00, 0xe1, 0x0a, 0x05, 0x03, 0x02};

const u8 gItemEffect_Carbos[]       = {0x00, 0x00, 0x00, 0x00, 0x00, 0xe2, 0x0a, 0x05, 0x03, 0x02};

const u8 gItemEffect_Calcium[]      = {0x00, 0x00, 0x00, 0x00, 0x00, 0xe8, 0x0a, 0x05, 0x03, 0x02};

const u8 gItemEffect_RareCandy[]    = {0x00, 0x00, 0x00, 0x40, 0x44, 0xe0, 0xfd, 0x05, 0x03, 0x02};

const u8 gItemEffect_PPUp[]         = {0x00, 0x00, 0x00, 0x00, 0x20, 0xe0, 0x05, 0x03, 0x02};

const u8 gItemEffect_Zinc[]         = {0x00, 0x00, 0x00, 0x00, 0x00, 0xe4, 0x0a, 0x05, 0x03, 0x02};

const u8 gItemEffect_PPMax[]        = {0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x05, 0x03, 0x02};

const u8 gItemEffect_GuardSpec[]    = {0x00, 0x00, 0x00, 0x80, 0x00, 0x60, 0x01, 0x01};

const u8 gItemEffect_DireHit[]      = {0x20, 0x00, 0x00, 0x00, 0x00, 0x60, 0x01, 0x01};

const u8 gItemEffect_XAttack[]      = {0x01, 0x00, 0x00, 0x00, 0x00, 0x60, 0x01, 0x01};

const u8 gItemEffect_XDefend[]      = {0x00, 0x10, 0x00, 0x00, 0x00, 0x60, 0x01, 0x01};

const u8 gItemEffect_XSpeed[]       = {0x00, 0x01, 0x00, 0x00, 0x00, 0x60, 0x01, 0x01};

const u8 gItemEffect_XAccuracy[]    = {0x00, 0x00, 0x10, 0x00, 0x00, 0x60, 0x01, 0x01};

const u8 gItemEffect_XSpecial[]     = {0x00, 0x00, 0x01, 0x00, 0x00, 0x60, 0x01, 0x01};

const u8 gItemEffect_Stone[]        = {0x00, 0x00, 0x00, 0x00, 0x80, 0x00};

const u8 gItemEffect_OranBerry[]    = {0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 10};

const u8 gItemEffect_SitrusBerry[]  = {0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0xFC};

#endif //GUARD_ITEM_EFFECTS_H
