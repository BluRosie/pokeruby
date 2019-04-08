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
MOD_GOOD_FRIENDSHIP | MOD_OKAY_FRIENDSHIP | MOD_BAD_FRIENDSHIP | PP_MAX    EV_SP_ATK | EV_SP_DEF | EV_SPEED | EV_DEFENSE
[6]
amount hp/pp/ev restored/added on
[7]
amount to raise friendship by when horrible, is signed
[8]
amount to raise friendship by when okay, is signed
[9]
amount to raise friendship by when great, is signed
*/

const u8 gItemEffect_Restore10PP[]  = {0, 
                                       0, 
                                       0, 
                                       0, 
                                       LIMITED_PP_RESTORE_ITEM, 
                                       0, 
                                       10};

const u8 gItemEffect_MaxEther[]     = {0, 
                                       0, 
                                       0, 
                                       0, 
                                       LIMITED_PP_RESTORE_ITEM, 
                                       0, 
                                       HEAL_FULL_PP};

const u8 gItemEffect_Elixir[]       = {0, 
                                       0, 
                                       0, 
                                       0, 
                                       PP_ITEM, 
                                       0, 
                                       10};

const u8 gItemEffect_MaxElixir[]    = {0, 
                                       0, 
                                       0, 
                                       0, 
                                       PP_ITEM, 
                                       0, 
                                       HEAL_FULL_PP};

const u8 gItemEffect_SacredAsh[]    = {SACRED_ASH, 
                                       0, 
                                       0, 
                                       0, 
                                       CAN_REVIVE_MON, 
                                       0, 
                                       HEAL_FULL_HP};

const u8 gItemEffect_HPUp[]         = {0x00, 
                                       0x00, 
                                       0x00, 
                                       0x00, 
                                       0x01, 
                                       0xe0, 
                                       0x0a, 
                                       0x05, 
                                       0x03, 
                                       0x02};

const u8 gItemEffect_Protein[]      = {0x00, 
                                       0x00, 
                                       0x00, 
                                       0x00, 
                                       0x02, 
                                       0xe0, 
                                       0x0a, 
                                       0x05, 
                                       0x03, 
                                       0x02};

const u8 gItemEffect_Iron[]         = {0x00, 
                                       0x00, 
                                       0x00, 
                                       0x00, 
                                       0x00, 
                                       0xe1, 
                                       0x0a, 
                                       0x05, 
                                       0x03, 
                                       0x02};

const u8 gItemEffect_Carbos[]       = {0x00, 
                                       0x00, 
                                       0x00, 
                                       0x00, 
                                       0x00, 
                                       0xe2, 
                                       0x0a, 
                                       0x05, 
                                       0x03, 
                                       0x02};

const u8 gItemEffect_Calcium[]      = {0x00, 
                                       0x00, 
                                       0x00, 
                                       0x00, 
                                       0x00, 
                                       0xe8, 
                                       0x0a, 
                                       0x05, 
                                       0x03, 
                                       0x02};

const u8 gItemEffect_RareCandy[]    = {0x00, 
                                       0x00, 
                                       0x00, 
                                       0x40, 
                                       0x44, 
                                       0xe0, 
                                       0xfd, 
                                       0x05, 
                                       0x03, 
                                       0x02};

const u8 gItemEffect_PPUp[]         = {0x00, 
                                       0x00, 
                                       0x00, 
                                       0x00, 
                                       0x20, 
                                       0xe0, 
                                       0x05, 
                                       0x03, 
                                       0x02};

const u8 gItemEffect_Zinc[]         = {0x00, 
                                       0x00, 
                                       0x00, 
                                       0x00, 
                                       0x00, 
                                       0xe4, 
                                       0x0a, 
                                       0x05, 
                                       0x03, 
                                       0x02};

const u8 gItemEffect_PPMax[]        = {0x00, 
                                       0x00, 
                                       0x00, 
                                       0x00, 
                                       0x00, 
                                       0xf0, 
                                       0x05, 
                                       0x03, 
                                       0x02};

const u8 gItemEffect_GuardSpec[]    = {0x00, 
                                       0x00, 
                                       0x00, 
                                       0x80, 
                                       0x00, 
                                       0x60, 
                                       0x01, 
                                       0x01};

const u8 gItemEffect_DireHit[]      = {0x20, 
                                       0x00, 
                                       0x00, 
                                       0x00, 
                                       0x00, 
                                       0x60, 
                                       0x01, 
                                       0x01};

const u8 gItemEffect_XAttack[]      = {0x01, 
                                       0x00, 
                                       0x00, 
                                       0x00, 
                                       0x00, 
                                       0x60, 
                                       0x01, 
                                       0x01};

const u8 gItemEffect_XDefend[]      = {0x00, 
                                       0x10, 
                                       0x00, 
                                       0x00, 
                                       0x00, 
                                       0x60, 
                                       0x01, 
                                       0x01};

const u8 gItemEffect_XSpeed[]       = {0x00, 
                                       0x01, 
                                       0x00, 
                                       0x00, 
                                       0x00, 
                                       0x60, 
                                       0x01, 
                                       0x01};

const u8 gItemEffect_XAccuracy[]    = {0x00, 
                                       0x00, 
                                       0x10, 
                                       0x00, 
                                       0x00, 
                                       0x60, 
                                       0x01, 
                                       0x01};

const u8 gItemEffect_XSpecial[]     = {0x00, 
                                       0x00, 
                                       0x01, 
                                       0x00, 
                                       0x00, 
                                       0x60, 
                                       0x01, 
                                       0x01};

const u8 gItemEffect_Stone[]        = {0x00, 
                                       0x00, 
                                       0x00, 
                                       0x00, 
                                       0x80, 
                                       0x00};

const u8 gItemEffect_OranBerry[]    = {0x00, 
                                       0x00, 
                                       0x00, 
                                       0x00, 
                                       0x04, 
                                       0x00, 
                                       10};

const u8 gItemEffect_SitrusBerry[]  = {0x00, 
                                       0x00, 
                                       0x00, 
                                       0x00, 
                                       0x04, 
                                       0x00, 
                                       0xFC};

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

const u8 gItemEffect_Restore50HP[]  = {0, 
                                       0, 
                                       0, 
                                       0, 
                                       HEALING_ITEM, 
                                       0, 
                                       50};

const u8 gItemEffect_FullHeal[]     = {0, 
                                       0, 
                                       0, 
                                       CURE_SLEEP | CURE_POISON | CURE_BURN | CURE_ICE | CURE_PARALYSIS | CURE_CONFUSION, 
                                       0, 
                                       0};

const u8 gItemEffect_Revive[]       = {0, 
                                       0, 
                                       0, 
                                       0, 
                                       CAN_REVIVE_MON, 
                                       0, 
                                       HEAL_HALF_HP};

const u8 gItemEffect_MaxRevive[]    = {0, 
                                       0, 
                                       0, 
                                       0, 
                                       CAN_REVIVE_MON, 
                                       0, 
                                       HEAL_FULL_HP};

const u8 gItemEffect_SodaPop[]      = {0, 
                                       0, 
                                       0, 
                                       0, 
                                       HEALING_ITEM, 
                                       0, 
                                       60};

const u8 gItemEffect_Lemonade[]     = {0, 
                                       0, 
                                       0, 
                                       0, 
                                       HEALING_ITEM, 
                                       0, 
                                       80};

const u8 gItemEffect_MoomooMilk[]   = {0, 
                                       0, 
                                       0, 
                                       0, 
                                       HEALING_ITEM, 
                                       0, 
                                       100};

const u8 gItemEffect_EnergyPowder[] = {0, 
                                       0, 
                                       0, 
                                       0, 
                                       HEALING_ITEM, 
                                       MOD_GOOD_FRIENDSHIP | MOD_OKAY_FRIENDSHIP | MOD_BAD_FRIENDSHIP, 
                                       50,   
                                       -5, 
                                       -5, 
                                       -10};

const u8 gItemEffect_EnergyRoot[]   = {0, 
                                       0, 
                                       0, 
                                       0, 
                                       HEALING_ITEM, 
                                       MOD_GOOD_FRIENDSHIP | MOD_OKAY_FRIENDSHIP | MOD_BAD_FRIENDSHIP, 
                                       200,  
                                       -10, 
                                       -10, 
                                       -15};

const u8 gItemEffect_HealPowder[]   = {0, 
                                       0, 
                                       0, 
                                       CURE_SLEEP | CURE_POISON | CURE_BURN | CURE_ICE | CURE_PARALYSIS | CURE_CONFUSION, 
                                       00, 
                                       MOD_GOOD_FRIENDSHIP | MOD_OKAY_FRIENDSHIP | MOD_BAD_FRIENDSHIP, 
                                       -5, 
                                       -5, 
                                       -10};

const u8 gItemEffect_RevivalHerb[]  = {0, 
                                       0, 
                                       0, 
                                       0, 
                                       CAN_REVIVE_MON, 
                                       MOD_GOOD_FRIENDSHIP | MOD_OKAY_FRIENDSHIP | MOD_BAD_FRIENDSHIP, 
                                       HEAL_FULL_HP, 
                                       -15, 
                                       -15, 
                                       -20};

#endif //GUARD_ITEM_EFFECTS_H
