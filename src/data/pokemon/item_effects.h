#ifndef GUARD_ITEM_EFFECTS_H
#define GUARD_ITEM_EFFECTS_H

/*
[0]
FREE | SACRED_ASH | FREE | FREE    FREE | FREE | FREE | FREE
[1]
RAISE_ATTACK | RAISE_DEFENSE | RAISE_SPEED | RAISE_SP_ATK    RAISE_SP_DEF | RAISE_CRITICAL | RAISE_ACCURACY | PREVENT_STAT_LOSS
[2]
FREE | FREE | EV_HP | EV_ATTACK    EV_DEFENSE | EV_SPEED | EV_SP_ATK | EV_SP_DEF
[3]
RAISE_LEVEL | CURE_INFATUATION | CURE_SLEEP | CURE_POISON   CURE_BURN | CURE_ICE | CURE_PARALYSIS | CURE_CONFUSION
[4]
EVO_STONE | REVIVES | PP_UP | LIMITED_PP_RESTORE_ITEM    PP_ITEM | HEALING_ITEM | FREE | FREE
[5]
MOD_GOOD_FRIENDSHIP | MOD_OKAY_FRIENDSHIP | MOD_BAD_FRIENDSHIP | PP_MAX    FREE | FREE | FREE | FREE
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

const u8 gItemEffect_AttractHeal[]  = {0, 
                                       0, 
                                       0, 
                                       CURE_INFATUATION, 
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

const u8 gItemEffect_Restore10PP[]  = {0, 
                                       0, 
                                       0, 
                                       0, 
                                       PP_LIMITED_ITEM, 
                                       0, 
                                       10};

const u8 gItemEffect_MaxEther[]     = {0, 
                                       0, 
                                       0, 
                                       0, 
                                       PP_LIMITED_ITEM, 
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

const u8 gItemEffect_HPUp[]         = {0, 
                                       0, 
                                       EV_HP, 
                                       0, 
                                       0, 
                                       MOD_GOOD_FRIENDSHIP | MOD_OKAY_FRIENDSHIP | MOD_BAD_FRIENDSHIP, 
                                       10, 
                                       5, 
                                       3, 
                                       2};

const u8 gItemEffect_Protein[]      = {0, 
                                       0, 
                                       EV_ATTACK, 
                                       0, 
                                       0, 
                                       MOD_GOOD_FRIENDSHIP | MOD_OKAY_FRIENDSHIP | MOD_BAD_FRIENDSHIP, 
                                       10, 
                                       5, 
                                       3, 
                                       2};

const u8 gItemEffect_Iron[]         = {0, 
                                       0, 
                                       EV_DEFENSE, 
                                       0, 
                                       0, 
                                       MOD_GOOD_FRIENDSHIP | MOD_OKAY_FRIENDSHIP | MOD_BAD_FRIENDSHIP, 
                                       10, 
                                       5, 
                                       3, 
                                       2};

const u8 gItemEffect_Carbos[]       = {0, 
                                       0, 
                                       EV_SPEED, 
                                       0, 
                                       0, 
                                       MOD_GOOD_FRIENDSHIP | MOD_OKAY_FRIENDSHIP | MOD_BAD_FRIENDSHIP, 
                                       10, 
                                       5, 
                                       3, 
                                       2};

const u8 gItemEffect_Calcium[]      = {0, 
                                       0, 
                                       EV_SP_ATK, 
                                       0, 
                                       0, 
                                       MOD_GOOD_FRIENDSHIP | MOD_OKAY_FRIENDSHIP | MOD_BAD_FRIENDSHIP, 
                                       10, 
                                       5, 
                                       3, 
                                       2};

const u8 gItemEffect_Zinc[]         = {0, 
                                       0, 
                                       EV_SP_DEF, 
                                       0, 
                                       0, 
                                       MOD_GOOD_FRIENDSHIP | MOD_OKAY_FRIENDSHIP | MOD_BAD_FRIENDSHIP, 
                                       10, 
                                       5, 
                                       3, 
                                       2};

const u8 gItemEffect_RareCandy[]    = {0, 
                                       0, 
                                       0, 
                                       RAISE_LEVEL, 
                                       CAN_REVIVE_MON,
                                       MOD_GOOD_FRIENDSHIP | MOD_OKAY_FRIENDSHIP | MOD_BAD_FRIENDSHIP, 
                                       KEEP_HP_CONSTANT, 
                                       5, 
                                       3, 
                                       2};

const u8 gItemEffect_PPUp[]         = {0, 
                                       0, 
                                       0, 
                                       0, 
                                       PP_UP, 
                                       MOD_GOOD_FRIENDSHIP | MOD_OKAY_FRIENDSHIP | MOD_BAD_FRIENDSHIP, 
                                       5, 
                                       3, 
                                       2};

const u8 gItemEffect_PPMax[]        = {0, 
                                       0, 
                                       0, 
                                       0, 
                                       0, 
                                       MOD_GOOD_FRIENDSHIP | MOD_OKAY_FRIENDSHIP | MOD_BAD_FRIENDSHIP | PP_MAX, 
                                       5, 
                                       3, 
                                       2};

const u8 gItemEffect_GuardSpec[]    = {0, 
                                       PREVENT_STAT_LOSS, 
                                       0, 
                                       0, 
                                       0, 
                                       MOD_OKAY_FRIENDSHIP | MOD_BAD_FRIENDSHIP, 
                                       1, 
                                       1};

const u8 gItemEffect_DireHit[]      = {0, 
                                       RAISE_CRITICAL, 
                                       0, 
                                       0, 
                                       0, 
                                       MOD_OKAY_FRIENDSHIP | MOD_BAD_FRIENDSHIP, 
                                       1, 
                                       1};

const u8 gItemEffect_XAttack[]      = {0, 
                                       RAISE_ATTACK, 
                                       0, 
                                       0, 
                                       0, 
                                       MOD_OKAY_FRIENDSHIP | MOD_BAD_FRIENDSHIP, 
                                       1, 
                                       1};

const u8 gItemEffect_XDefend[]      = {0, 
                                       RAISE_DEFENSE, 
                                       0, 
                                       0, 
                                       0, 
                                       MOD_OKAY_FRIENDSHIP | MOD_BAD_FRIENDSHIP, 
                                       1, 
                                       1};

const u8 gItemEffect_XSpeed[]       = {0, 
                                       RAISE_SPEED, 
                                       0, 
                                       0, 
                                       0, 
                                       MOD_OKAY_FRIENDSHIP | MOD_BAD_FRIENDSHIP, 
                                       1, 
                                       1};

const u8 gItemEffect_XAccuracy[]    = {0, 
                                       RAISE_ACCURACY, 
                                       0, 
                                       0, 
                                       0, 
                                       MOD_OKAY_FRIENDSHIP | MOD_BAD_FRIENDSHIP, 
                                       1, 
                                       1};

const u8 gItemEffect_XSpecial[]     = {0, 
                                       RAISE_SP_ATK, 
                                       0, 
                                       0, 
                                       0, 
                                       MOD_OKAY_FRIENDSHIP | MOD_BAD_FRIENDSHIP, 
                                       1, 
                                       1};

const u8 gItemEffect_XSpDef[]       = {0, 
                                       RAISE_SP_DEF, 
                                       0, 
                                       0, 
                                       0, 
                                       MOD_OKAY_FRIENDSHIP | MOD_BAD_FRIENDSHIP, 
                                       1, 
                                       1};

const u8 gItemEffect_Stone[]        = {0, 
                                       0, 
                                       0, 
                                       0, 
                                       EVO_STONE, 
                                       0};

const u8 gItemEffect_OranBerry[]    = {0, 
                                       0, 
                                       0, 
                                       0, 
                                       HEALING_ITEM, 
                                       0, 
                                       10};

const u8 gItemEffect_SitrusBerry[]  = {0, 
                                       0, 
                                       0, 
                                       0, 
                                       HEALING_ITEM, 
                                       0, 
                                       HEAL_QUARTER_HP};

#endif //GUARD_ITEM_EFFECTS_H
