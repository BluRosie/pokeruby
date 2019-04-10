#ifndef GUARD_POKEMON_ITEM_EFFECT_H
#define GUARD_POKEMON_ITEM_EFFECT_H

// item importances
#define IMPORTANCE_CAN_BE_HELD   0
#define IMPORTANCE_CANT_BE_HELD  1
#define IMPORTANCE_NO_PC_DEPOSIT 2

// item types
#define TYPE_USE_ON_MON      1 // goes to party screen
#define TYPE_RETURN_TO_FIELD 2 // goes to field
#define TYPE_POKEBLOCK       3 // goes to block screen
#define TYPE_UNUSABLE        4 // does nothing

// item_effects.h
//[0]
#define SACRED_ASH      0x40
#define ORICORIO_NECTAR 0x01

#define X_ITEMS           1
//[1]
#define RAISE_ATTACK      0x80
#define RAISE_DEFENSE     0x40
#define RAISE_SPEED       0x20
#define RAISE_SP_ATK      0x10
#define RAISE_SP_DEF      0x08
#define RAISE_CRITICAL    0x04
#define RAISE_ACCURACY    0x02
#define PREVENT_STAT_LOSS 0x01

#define VITAMINS  2
//[2]
#define EV_HP      0x20
#define EV_ATTACK  0x10
#define EV_DEFENSE 0x08
#define EV_SPEED   0x04
#define EV_SP_ATK  0x02
#define EV_SP_DEF  0x01

#define STATUS_HEALING   3
//[3]
#define RAISE_LEVEL       0x80
#define CURE_INFATUATION  0x40
#define CURE_SLEEP        0x20
#define CURE_POISON       0x10
#define CURE_BURN         0x08
#define CURE_ICE          0x04
#define CURE_PARALYSIS    0x02
#define CURE_CONFUSION    0x01

#define MISCELLANEOUS            4
//[4]
#define EVO_STONE                0x80
#define REVIVES                  0x40
#define PP_UP                    0x20
#define LIMITED_PP_RESTORE_ITEM  0x10
#define PP_ITEM                  0x08
#define HEALING_ITEM             0x04

#define CAN_REVIVE_MON           (REVIVES | HEALING_ITEM)
#define PP_LIMITED_ITEM          (LIMITED_PP_RESTORE_ITEM | PP_ITEM)

//[5]
#define MOD_GOOD_FRIENDSHIP 0x80
#define MOD_OKAY_FRIENDSHIP 0x40
#define MOD_BAD_FRIENDSHIP  0x20
#define PP_MAX              0x10

#define PARAMETER 6
//[6]
// general quantity risen:  ff is max hp, fe is half, 7f is max pp
#define HEAL_FULL_HP 0xFF
#define HEAL_HALF_HP 0xFE
#define KEEP_HP_CONSTANT 0xFD
#define HEAL_QUARTER_HP 0xFC
#define HEAL_FULL_PP 0X7F
//[7]
// friendship raising when horrible
//[8]
// friendship raising when okay
//[9]
// friendship raising when great

extern const u8 gItemEffect_Potion[];
extern const u8 gItemEffect_AttractHeal[];
extern const u8 gItemEffect_ConfuseHeal[];
extern const u8 gItemEffect_PoisonHeal[];
extern const u8 gItemEffect_BurnHeal[];
extern const u8 gItemEffect_IceHeal[];
extern const u8 gItemEffect_SleepHeal[];
extern const u8 gItemEffect_ParalyzeHeal[];
extern const u8 gItemEffect_FullRestore[];
extern const u8 gItemEffect_MaxPotion[];
extern const u8 gItemEffect_HyperPotion[];
extern const u8 gItemEffect_Restore50HP[];
extern const u8 gItemEffect_FullHeal[];
extern const u8 gItemEffect_Revive[];
extern const u8 gItemEffect_MaxRevive[];
extern const u8 gItemEffect_SodaPop[];
extern const u8 gItemEffect_Lemonade[];
extern const u8 gItemEffect_MoomooMilk[];
extern const u8 gItemEffect_EnergyPowder[];
extern const u8 gItemEffect_EnergyRoot[];
extern const u8 gItemEffect_HealPowder[];
extern const u8 gItemEffect_RevivalHerb[];
extern const u8 gItemEffect_Restore10PP[];
extern const u8 gItemEffect_MaxEther[];
extern const u8 gItemEffect_Elixir[];
extern const u8 gItemEffect_MaxElixir[];
extern const u8 gItemEffect_BerryJuice[];
extern const u8 gItemEffect_SacredAsh[];
extern const u8 gItemEffect_HPUp[];
extern const u8 gItemEffect_Protein[];
extern const u8 gItemEffect_Iron[];
extern const u8 gItemEffect_Carbos[];
extern const u8 gItemEffect_Calcium[];
extern const u8 gItemEffect_Zinc[];
extern const u8 gItemEffect_HealthWing[];
extern const u8 gItemEffect_MuscleWing[];
extern const u8 gItemEffect_ResistWing[];
extern const u8 gItemEffect_SwiftWing[];
extern const u8 gItemEffect_GeniusWing[];
extern const u8 gItemEffect_CleverWing[];
extern const u8 gItemEffect_RareCandy[];
extern const u8 gItemEffect_PPUp[];
extern const u8 gItemEffect_PPMax[];
extern const u8 gItemEffect_GuardSpec[];
extern const u8 gItemEffect_DireHit[];
extern const u8 gItemEffect_XAttack[];
extern const u8 gItemEffect_XDefend[];
extern const u8 gItemEffect_XSpeed[];
extern const u8 gItemEffect_XAccuracy[];
extern const u8 gItemEffect_XSpecial[];
extern const u8 gItemEffect_XSpDef[];
extern const u8 gItemEffect_Stone[];
extern const u8 gItemEffect_OranBerry[];
extern const u8 gItemEffect_SitrusBerry[];

bool8 ExecuteTableBasedItemEffect_(struct Pokemon *mon, u16, u8, u8);

#endif // GUARD_POKEMON_ITEM_EFFECT_H
