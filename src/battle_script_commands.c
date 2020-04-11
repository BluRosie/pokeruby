#include "global.h"
#include "battle.h"
#include "battle_message.h"
#include "battle_string_ids.h"
#include "battle_script_commands.h"
#include "battle_util.h"
#include "constants/battle_move_effects.h"
#include "constants/hold_effects.h"
#include "constants/moves.h"
#include "constants/abilities.h"
#include "item.h"
#include "constants/items.h"
#include "data2.h"
#include "constants/hold_effects.h"
#include "random.h"
#include "battle_controllers.h"
#include "constants/species.h"
#include "pokemon.h"
#include "text.h"
#include "palette.h"
#include "main.h"
#include "constants/songs.h"
#include "sound.h"
#include "task.h"
#include "decompress.h"
#include "string_util.h"
#include "pokemon_summary_screen.h"
#include "naming_screen.h"
#include "ewram.h"
#include "util.h"
#include "pokedex.h"

struct BerryTypeStruct
{
    u16 berry; // while 16 bits technically wouldn't be necessary for the berries alone, i'm putting it out there now in case i get lazy
    u8 type;
};

const struct BerryTypeStruct gBerryToType[] =
{
    {ITEM_TANGA_BERRY,    TYPE_BUG},
    {ITEM_BABIRI_BERRY,   TYPE_STEEL},
    {ITEM_SHUCA_BERRY,    TYPE_GROUND},
    {ITEM_CHARTI_BERRY,   TYPE_ROCK},
    {ITEM_RINDO_BERRY,    TYPE_GRASS},
    {ITEM_COLBUR_BERRY,   TYPE_DARK},
    {ITEM_CHOPLE_BERRY,   TYPE_FIGHTING},
    {ITEM_WACAN_BERRY,    TYPE_ELECTRIC},
    {ITEM_PASSHO_BERRY,   TYPE_WATER},
    {ITEM_COBA_BERRY,     TYPE_FLYING},
    {ITEM_KEBIA_BERRY,    TYPE_POISON},
    {ITEM_YACHE_BERRY,    TYPE_ICE},
    {ITEM_KASIB_BERRY,    TYPE_GHOST},
    {ITEM_PAYAPA_BERRY,   TYPE_PSYCHIC},
    {ITEM_OCCA_BERRY,     TYPE_FIRE},
    {ITEM_HABAN_BERRY,    TYPE_DRAGON},
    {ITEM_CHILAN_BERRY,   TYPE_NORMAL},
    {ITEM_ROSELI_BERRY,   TYPE_FAIRY}
};

struct NaturalGiftBerry
{
    u8 power;
    u8 type;
};

const struct NaturalGiftBerry gNaturalGiftBerries[] =
{
    [ ITEM_CHERI_BERRY - FIRST_BERRY ] = { 80, TYPE_FIRE },
    [ ITEM_CHESTO_BERRY - FIRST_BERRY ] = { 80, TYPE_WATER },
    [ ITEM_PECHA_BERRY - FIRST_BERRY ] = { 80, TYPE_ELECTRIC },
    [ ITEM_RAWST_BERRY - FIRST_BERRY ] = { 80, TYPE_GRASS },
    [ ITEM_ASPEAR_BERRY - FIRST_BERRY ] = { 80, TYPE_ICE },
    [ ITEM_LEPPA_BERRY - FIRST_BERRY ] = { 80, TYPE_FIGHTING },
    [ ITEM_ORAN_BERRY - FIRST_BERRY ] = { 80, TYPE_POISON },
    [ ITEM_PERSIM_BERRY - FIRST_BERRY ] = { 80, TYPE_GROUND },
    [ ITEM_LUM_BERRY - FIRST_BERRY ] = { 80, TYPE_FLYING },
    [ ITEM_SITRUS_BERRY - FIRST_BERRY ] = { 80, TYPE_PSYCHIC },
    [ ITEM_FIGY_BERRY - FIRST_BERRY ] = { 80, TYPE_BUG },
    [ ITEM_WIKI_BERRY - FIRST_BERRY ] = { 80, TYPE_ROCK },
    [ ITEM_MAGO_BERRY - FIRST_BERRY ] = { 80, TYPE_GHOST },
    [ ITEM_AGUAV_BERRY - FIRST_BERRY ] = { 80, TYPE_DRAGON },
    [ ITEM_IAPAPA_BERRY - FIRST_BERRY ] = { 80, TYPE_DARK },
    [ ITEM_RAZZ_BERRY - FIRST_BERRY ] = { 80, TYPE_STEEL },
    [ ITEM_BLUK_BERRY - FIRST_BERRY ] = { 90, TYPE_FIRE },
    [ ITEM_NANAB_BERRY - FIRST_BERRY ] = { 90, TYPE_WATER },
    [ ITEM_WEPEAR_BERRY - FIRST_BERRY ] = { 90, TYPE_ELECTRIC },
    [ ITEM_PINAP_BERRY - FIRST_BERRY ] = { 90, TYPE_GRASS },
    [ ITEM_POMEG_BERRY - FIRST_BERRY ] = { 90, TYPE_ICE },
    [ ITEM_KELPSY_BERRY - FIRST_BERRY ] = { 90, TYPE_FIGHTING },
    [ ITEM_QUALOT_BERRY - FIRST_BERRY ] = { 90, TYPE_POISON },
    [ ITEM_HONDEW_BERRY - FIRST_BERRY ] = { 90, TYPE_GROUND },
    [ ITEM_GREPA_BERRY - FIRST_BERRY ] = { 90, TYPE_FLYING },
    [ ITEM_TAMATO_BERRY - FIRST_BERRY ] = { 90, TYPE_PSYCHIC },
    [ ITEM_CORNN_BERRY - FIRST_BERRY ] = { 90, TYPE_BUG },
    [ ITEM_MAGOST_BERRY - FIRST_BERRY ] = { 90, TYPE_ROCK },
    [ ITEM_RABUTA_BERRY - FIRST_BERRY ] = { 90, TYPE_GHOST },
    [ ITEM_NOMEL_BERRY - FIRST_BERRY ] = { 90, TYPE_DRAGON },
    [ ITEM_SPELON_BERRY - FIRST_BERRY ] = { 90, TYPE_DARK },
    [ ITEM_PAMTRE_BERRY - FIRST_BERRY ] = { 90, TYPE_STEEL },
    [ ITEM_WATMEL_BERRY - FIRST_BERRY ] = { 100, TYPE_FIRE },
    [ ITEM_DURIN_BERRY - FIRST_BERRY ] = { 100, TYPE_WATER },
    [ ITEM_BELUE_BERRY - FIRST_BERRY ] = { 100, TYPE_ELECTRIC },
    [ ITEM_OCCA_BERRY - FIRST_BERRY ] = { 80, TYPE_FIRE },
    [ ITEM_PASSHO_BERRY - FIRST_BERRY ] = { 80, TYPE_WATER },
    [ ITEM_WACAN_BERRY - FIRST_BERRY ] = { 80, TYPE_ELECTRIC },
    [ ITEM_RINDO_BERRY - FIRST_BERRY ] = { 80, TYPE_GRASS },
    [ ITEM_YACHE_BERRY - FIRST_BERRY ] = { 80, TYPE_ICE },
    [ ITEM_CHOPLE_BERRY - FIRST_BERRY ] = { 80, TYPE_FIGHTING },
    [ ITEM_KEBIA_BERRY - FIRST_BERRY ] = { 80, TYPE_POISON },
    [ ITEM_SHUCA_BERRY - FIRST_BERRY ] = { 80, TYPE_GROUND },
    [ ITEM_COBA_BERRY - FIRST_BERRY ] = { 80, TYPE_FLYING },
    [ ITEM_PAYAPA_BERRY - FIRST_BERRY ] = { 80, TYPE_PSYCHIC },
    [ ITEM_TANGA_BERRY - FIRST_BERRY ] = { 80, TYPE_BUG },
    [ ITEM_CHARTI_BERRY - FIRST_BERRY ] = { 80, TYPE_ROCK },
    [ ITEM_KASIB_BERRY - FIRST_BERRY ] = { 80, TYPE_GHOST },
    [ ITEM_HABAN_BERRY - FIRST_BERRY ] = { 80, TYPE_DRAGON },
    [ ITEM_COLBUR_BERRY - FIRST_BERRY ] = { 80, TYPE_DARK },
    [ ITEM_BABIRI_BERRY - FIRST_BERRY ] = { 80, TYPE_STEEL },
    [ ITEM_CHILAN_BERRY - FIRST_BERRY ] = { 80, TYPE_NORMAL },
    [ ITEM_ROSELI_BERRY - FIRST_BERRY ] = { 80, TYPE_FAIRY },
    [ ITEM_LIECHI_BERRY - FIRST_BERRY ] = { 100, TYPE_GRASS },
    [ ITEM_GANLON_BERRY - FIRST_BERRY ] = { 100, TYPE_ICE },
    [ ITEM_SALAC_BERRY - FIRST_BERRY ] = { 100, TYPE_FIGHTING },
    [ ITEM_PETAYA_BERRY - FIRST_BERRY ] = { 100, TYPE_POISON },
    [ ITEM_APICOT_BERRY - FIRST_BERRY ] = { 100, TYPE_GROUND },
    [ ITEM_LANSAT_BERRY - FIRST_BERRY ] = { 100, TYPE_FLYING },
    [ ITEM_STARF_BERRY - FIRST_BERRY ] = { 100, TYPE_PSYCHIC },
    [ ITEM_ENIGMA_BERRY - FIRST_BERRY ] = { 100, TYPE_BUG },
    [ ITEM_MICLE_BERRY - FIRST_BERRY ] = { 100, TYPE_ROCK },
    [ ITEM_CUSTAP_BERRY - FIRST_BERRY ] = { 100, TYPE_GHOST },
    [ ITEM_JABOCA_BERRY - FIRST_BERRY ] = { 100, TYPE_DRAGON },
    [ ITEM_ROWAP_BERRY - FIRST_BERRY ] = { 100, TYPE_DARK },
    [ ITEM_KEE_BERRY - FIRST_BERRY ] = { 100, TYPE_FAIRY },
    [ ITEM_MARANGA_BERRY - FIRST_BERRY ] = { 100, TYPE_DARK },
};

//extern needed variables
extern u8 gUnknown_02023A14_50;
extern u8 gCritMultiplier;
extern s32 gBattleMoveDamage;
extern u32 gStatuses3[MAX_BATTLERS_COUNT];
extern u16 gBattleTypeFlags;
extern const struct BaseStats gBaseStats[];
extern struct BattleEnigmaBerry gEnigmaBerries[MAX_BATTLERS_COUNT];
extern struct BattlePokemon gBattleMons[MAX_BATTLERS_COUNT];
extern u8 gActiveBattler;
extern u32 gBattleControllerExecFlags;
extern u8 gBattlersCount;
extern u16 gBattlerPartyIndexes[MAX_BATTLERS_COUNT];
extern u8 gBattlerByTurnOrder[MAX_BATTLERS_COUNT];
extern u8 gActionsByTurnOrder[MAX_BATTLERS_COUNT];
extern u16 gCurrentMove;
extern u8 gLastUsedAbility;
extern u16 gBattleWeather;
extern u8 gPotentialItemEffectBattler;
extern u8 gEffectBattler;
extern u8 gAbsentBattlerFlags;
extern u8 gMultiHitCounter;
extern u16 gLastMoves[4];
extern u16 gLockedMoves[4];
extern u16 gChosenMovesByBanks[4];
extern u16 gSideStatuses[2];
extern u16 gPauseCounterBattle;
extern u16 gPaydayMoney;
extern u16 gRandomTurnNumber;
extern u8 gBattleOutcome;
extern u8 gBattleTerrain;
extern u16 gTrainerBattleOpponent;
extern u8 gBattlerAttacker;
extern u8 gBattlerTarget;
extern const u8* gBattlescriptCurrInstr;
extern u8 gCurrMovePos;
extern u8 gCurrentActionFuncId;
extern u32 gHitMarker;
extern u8 gMoveResultFlags;
extern u8 gBattleCommunication[];
extern u16 gLastLandedMoves[4];
extern u16 gLastHitByType[4];
extern u8 gPotentialItemEffectBattler;
extern u16 gDynamicBasePower;
extern u8 gDynamicType;
extern const u8 gTypeEffectiveness[];
extern u16 gLastUsedItem;
extern u16 gBattleMovePower;
extern s32 gHpDealt;
extern s32 gTakenDmg[MAX_BATTLERS_COUNT];
extern u8 gTakenDmgByBattler[MAX_BATTLERS_COUNT];
extern const u16 gMissStringIds[];
extern u8 gSentPokesToOpponent[2];
extern u8 gBank1;
extern u16 gExpShareExp;
extern u8 gBattleTextBuff1[];
extern u8 gBattleTextBuff2[];
extern u8 gBattleTextBuff3[];
extern u8 gLeveledUpInBattle;
extern void (*gBattleMainFunc)(void);
extern struct Window gUnknown_03004210;
extern const u8 BattleText_YesNo[];
extern u8 gPlayerPartyCount;
extern u16 gMoveToLearn; //move to learn
extern const u8 gTrainerMoney[];
extern u16 gRandomMove;
extern u8* gBattleScriptsForMoveEffects[];
extern u16 gChosenMove; //last used move in battle
extern u8 gBankInMenu;
extern u8 gActionForBanks[4];
extern u16 gUnknown_02024C2C[4]; //last used moves 2, used by sketch
extern u16 gUnknown_02024C4C[4]; //last used moves by banks, another one
extern u8 gCurrentTurnActionNumber;
extern u16 gTrappingMoves[];

extern u8 BattleScript_MoveEffectSleep[];
extern u8 BattleScript_MoveEffectPoison[];
extern u8 BattleScript_MoveEffectBurn[];
extern u8 BattleScript_MoveEffectFreeze[];
extern u8 BattleScript_MoveEffectParalysis[];
extern u8 BattleScript_MoveEffectToxic[];
extern u8 BattleScript_MoveEffectConfusion[];
extern u8 BattleScript_MoveEffectUproar[];
extern u8 BattleScript_MoveEffectWrap[];
extern u8 BattleScript_MoveEffectPayDay[];
extern u8 BattleScript_MoveEffectRecoil33[];
extern u8 BattleScript_SturdyPreventsOHKO[];

//extern functions
u8 AtkCanceller_UnableToUseMove(void);
void PressurePPLose(u8 bank_atk, u8 bank_def, u16 move);
void CancelMultiTurnMoves(u8 bank);
void BattleScriptPush(const u8* BS_ptr);
void BattleScriptPushCursor(void);
void RecordAbilityBattle(u8 bank, u8 ability);
void RecordItemEffectBattle(u8 bank, u8 holdEffect);
static bool8 IsTwoTurnsMove(u16 move);
static void TrySetDestinyBondToHappen(void);
static void CheckWonderGuardAndLevitate(void);
u8 GetBattlerPosition(u8 bank);
u8 GetBattlerSide(u8 bank);
u8 GetBattlerForBattleScript(u8 bankValue);
s32 CalculateBaseDamage(struct BattlePokemon *attacker, struct BattlePokemon *defender, u32 move, u16 a4, u16 powerOverride, u8 typeOverride, u8 bank_atk, u8 bank_def);
static u8 AttacksThisTurn(u8 bank, u16 move); //Note: returns 1 if it's a charging turn, otherwise 2
void UndoEffectsAfterFainting(void);
void BattleStopLowHpSound(void);
void PlayBGM(u16 songID);
void MonGainEVs(struct Pokemon*, u16 defeatedSpecies);
extern u8 gBattleBufferB[4][0x200];
void HandleLowHpMusicChange(struct Pokemon*, u8 bank);
bool8 IsTradedMon(struct Pokemon*);
void BattleScriptPop(void);
void SwitchInClearSetData(void);
//u8* ConvertIntToDecimalStringN(u8*, s32, u8, u8);
s8 GetSetPokedexFlag(u16 nationalNum, u8 caseID);
u16 SpeciesToNationalPokedexNum(u16 species);
u8 sub_803FC34(u8 bank);
u16 sub_803FBFC(u8 a);
u8 GetBattlerAtPosition(u8 ID);
void sub_8012258(u8);
//MonTryLearningNewMove teach poke a move
u16 GiveMoveToBattleMon(struct BattlePokemon *mon, u16 move);
void IncrementGameStat(u8 index);
u8 GetScaledHPFraction(s16 hp, s16 maxhp, u8 scale);
u16 GetPokedexHeightWeight(u16 national_num, u8 heightweight);
u8 MenuCursor_Create814A5C0(u8 a1, u16 a2, u8 a3, u16 a4, u8 a5);
void DestroyMenuCursor(void);
void sub_802BC6C(void);
u8 sub_809FA30(void);
bool32 IsHMMove2(u16 move);
void sub_802BBD4(u8 r0, u8 r1, u8 r2, u8 r3, u8 sp0);
void nullsub_6(void);
void ReshowBattleScreenAfterMenu(void);
void BattleMainCB2(void);
void AddMoney(u32* moneySaveblock, u32 to_give);
u8 CountAliveMons(u8 caseID);
void PokemonUseItemEffects(struct Pokemon*, u16 item, u8 partyID, u8 r3, u8 sp);
u8 CanRunFromBattle(void);
u8 GetMoveTarget(u16 move, u8 targetbyte); //get target of move
u8 CastformDataTypeChange(u8 bank);
u8 Overworld_GetMapTypeOfSaveblockLocation(void);
u8 CalculatePlayerPartyCount(void);
u16 Sqrt(u32 num);
u8 PrepareDexDisplayForCaughtMon(u16 nationalNum, u32 TiD, u32 PiD); //task prepare poke dex display
void MenuCursor_SetPos814A880(u8 a1, u8 a2);
u8 CheckMoveLimitations(u8 bank, u8 unusable_moves, u8 flags);
bool8 IsLinkDoubleBattle(void);
void sub_8094B6C(u8 bank, u8 partyID, u8 r2);

//extern BattleScripts
extern u8 BattleScript_MoveEnd[];
extern u8 BattleScript_NoPPForMove[];
extern u8 BattleScript_MagicCoatBounce[];
extern u8 BattleScript_TookAttack[];
extern u8 BattleScript_SnatchedMove[];
extern u8 BattleScript_Pausex20[];
extern u8 BattleScript_SubstituteFade[];
extern u8 BattleScript_HangedOnMsg[];
extern u8 BattleScript_OneHitKOMsg[];
extern u8 BattleScript_EnduredMsg[];
extern u8 BattleScript_PSNPrevention[];
extern u8 BattleScript_BRNPrevention[];
extern u8 BattleScript_PRLZPrevention[];
extern u8 BattleScript_SLPPrevention[];
extern u8 BattleScript_FlinchPrevention[];
extern u8 BattleScript_StatUp[];
extern u8 BattleScript_StatDown[];
extern u8 BattleScript_NoItemSteal[];
extern u8 BattleScript_ItemSteal[];
extern u8 BattleScript_RapidSpinAway[];
extern u8 BattleScript_TargetPRLZHeal[];
extern u8 BattleScript_TargetSleepHeal[];
extern u8 BattleScript_TargetBurnHeal[];
extern u8 BattleScript_KnockedOff[];
extern u8 BattleScript_LevelUp[];
extern u8 BattleScript_WrapFree[];
extern u8 BattleScript_LeechSeedFree[];
extern u8 BattleScript_SpikesFree[];
extern u8 BattleScript_ToxicSpikesFree[];
extern u8 BattleScript_RocksFree[];
extern u8 BattleScript_SideStatusWoreOffReturn[];
extern u8 BattleScript_ButItFailed[];
extern u8 BattleScript_ButItFailedAtkStringPpReduce[];
extern u8 BattleScript_ButItFailedPpReduce[];
extern u8 BattleScript_HitEscapeEnd[];
extern u8 BattleScript_ObliviousPreventsAttraction[];
extern u8 BattleScript_DestinyKnotActivates[];
extern u8 BattleScript_MistProtected[];
extern u8 BattleScript_AbilityNoStatLoss[];
extern u8 BattleScript_AbilityNoSpecificStatLoss[];
extern u8 BattleScript_TrainerBallBlock[];
extern u8 BattleScript_WallyBallThrow[];
extern u8 BattleScript_SuccessBallThrow[];
extern u8 BattleScript_ShakeBallThrow[];
extern u8 BattleScript_AllStatsUp[];
extern u8 BattleScript_AtkDefDown[];
extern u8 BattleScript_DefSpDefDown[];
extern u8 BattleScript_SAtkDown2[];
extern u8 BattleScript_BerryCureChosenStatusRet[]; //berry cure any status return
extern u8 BattleScript_WhiteHerbFling[];
extern u8 BattleScript_MoveEffectRecoilWithStatus[];
extern u8 BattleScript_EffectWithChance[];
extern u8 BattleScript_CaptivateCheckAcc[];
extern u8 BattleScript_MoveEffectBugBite[];

extern const u8 gStatusConditionString_LoveJpn[];
extern const u8 BattleText_Taunt[];
extern const u8 BattleText_Encore[];
extern const u8 BattleText_Torment[];
extern const u8 BattleText_Disable[];
extern const u8 BattleText_Many[];

extern u8 BattleScript_SpikesOnTarget[]; //spikes1
extern u8 BattleScript_SpikesOnAttacker[]; //spikes2
extern u8 BattleScript_SpikesOngBank1[]; //spikes3
extern u8 BattleScript_ToxicSpikesAbsorbed[];
extern u8 BattleScript_ToxicSpikesPoisoned[];
extern u8 BattleScript_StealthRockOnTarget[]; //StealthRock1
extern u8 BattleScript_StealthRockOnAttacker[]; //StealthRock2
extern u8 BattleScript_StealthRockOngBank1[]; //StealthRock3
extern u8 BattleScript_HitFromCritCalc[]; //present dmg
extern u8 BattleScript_AlreadyAtFullHp[]; //present full hp
extern u8 BattleScript_PresentHealTarget[]; //present hp heal
extern u8 BattleScript_MoveMissedPause[];
extern u8 BattleScript_CastformChange[];
extern u8 BattleScript_DampStopsExplosion[];
extern u8 BattleScript_SuccessForceOut[]; //bs random switchout
extern u8 BattleScript_PrintPayDayMoneyString[]; //bs payday money give
extern u8 BattleScript_FaintAttacker[];
extern u8 BattleScript_FaintTarget[];
extern u8 BattleScript_DestinyBondTakesLife[];
extern u8 BattleScript_GrudgeTakesPp[];

// read via orr
#define BSScriptRead32(ptr) ((ptr)[0] | (ptr)[1] << 8 | (ptr)[2] << 16 | (ptr)[3] << 24)
#define BSScriptRead16(ptr) ((ptr)[0] | ((ptr)[1] << 8))
#define BSScriptReadPtr(ptr) ((void *)BSScriptRead32(ptr))

// read via add
#define BS2ScriptRead32(ptr) ((ptr)[0] + ((ptr)[1] << 8) + ((ptr)[2] << 16) + ((ptr)[3] << 24))
#define BS2ScriptRead16(ptr) ((ptr)[0] + ((ptr)[1] << 8))
#define BS2ScriptReadPtr(ptr) ((void *)BS2ScriptRead32(ptr))

#define DEFENDER_IS_PROTECTED ((gProtectStructs[gBattlerTarget].protected && gBattleMoves[gCurrentMove].flags & F_AFFECTED_BY_PROTECT))

//array entries for battle communication
#define MOVE_EFFECT_BYTE    0x3
#define MULTISTRING_CHOOSER 0x5
#define MSG_DISPLAY         0x7

#define TARGET_SELECTED         0x0
#define TARGET_DEPENDS          0x1
#define MOVE_TARGET_BOTH             0x8
#define MOVE_TARGET_FOES_AND_ALLY    0x20
#define MOVE_TARGET_OPPONENTS_FIELD  0x40

#define TYPE_FORESIGHT  0xFE
#define TYPE_ENDTABLE   0xFF

#define CMP_EQUAL               0x0
#define CMP_NOT_EQUAL           0x1
#define CMP_GREATER_THAN        0x2
#define CMP_LESS_THAN           0x3
#define CMP_COMMON_BITS         0x4
#define CMP_NO_COMMON_BITS      0x5

#define uBYTE0_16(value)(( (u8) (((u16)(value) & (0x000000FF)) >> 0x00)))
#define uBYTE1_16(value)(( (u8) (((u16)(value) & (0x0000FF00)) >> 0x08)))

#define uBYTE0_32(value)(( (u8) (((u32)(value) & (0x000000FF)) >> 0x00)))
#define uBYTE1_32(value)(( (u8) (((u32)(value) & (0x0000FF00)) >> 0x08)))
#define uBYTE2_32(value)(( (u8) (((u32)(value) & (0x00FF0000)) >> 0x10)))
#define uBYTE3_32(value)(( (u8) (((u32)(value) & (0xFF000000)) >> 0x18)))

#define sBYTE0_16(value)(( (u8) (((s16)(value) & (0x000000FF)) >> 0x00)))
#define sBYTE1_16(value)(( (u8) (((s16)(value) & (0x0000FF00)) >> 0x08)))

#define sBYTE0_32(value)(( (u8) (((s32)(value) & (0x000000FF)) >> 0x00)))
#define sBYTE1_32(value)(( (u8) (((s32)(value) & (0x0000FF00)) >> 0x08)))
#define sBYTE2_32(value)(( (u8) (((s32)(value) & (0x00FF0000)) >> 0x10)))
#define sBYTE3_32(value)(( (u8) (((s32)(value) & (0xFF000000)) >> 0x18)))

#define RecordAbilitySetField6(ability, fieldValue) \
(gLastUsedAbility = ability, gBattleCommunication[6] = fieldValue, RecordAbilityBattle(gBattlerTarget, ability))

#define HP_ON_SWITCHOUT (((u16*)(ewram_addr + 0x160BC)))

static void atk00_attackcanceler(void);
static void atk01_accuracycheck(void);
static void atk02_attackstring(void);
static void atk03_ppreduce(void);
static void atk04_critcalc(void);
static void atk05_damagecalc(void);
static void atk06_typecalc(void);
static void atk07_adjustnormaldamage(void);
static void atk08_adjustnormaldamage2(void);
static void atk09_attackanimation(void);
static void atk0A_waitanimation(void);
static void atk0B_healthbarupdate(void);
static void atk0C_datahpupdate(void);
static void atk0D_critmessage(void);
static void atk0E_effectivenesssound(void);
static void atk0F_resultmessage(void);
static void atk10_printstring(void);
static void atk11_printselectionstring(void);
static void atk12_waitmessage(void);
static void atk13_printfromtable(void);
static void atk14_printselectionstringfromtable(void);
static void atk15_seteffectwithchance(void);
static void atk16_seteffectprimary(void);
static void atk17_seteffectsecondary(void);
static void atk18_clearstatusfromeffect(void);
static void atk19_tryfaintmon(void);
static void atk1A_dofaintanimation(void);
static void atk1B_cleareffectsonfaint(void);
static void atk1C_jumpifstatus(void);
static void atk1D_jumpifstatus2(void);
static void atk1E_jumpifability(void);
static void atk1F_jumpifsideaffecting(void);
static void atk20_jumpifstat(void);
static void atk21_jumpifstatus3condition(void);
static void atk22_jumpiftype(void);
static void atk23_getexp(void);
static bool32 NoAliveMonsForOpponent(void);
static bool32 NoAliveMonsForPlayer(void);
bool32 NoAliveMonsForEitherParty(void);
static void atk24(void);
static void atk25_movevaluescleanup(void);
static void atk26_setmultihit(void);
static void atk27_decrementmultihit(void);
static void atk28_goto(void);
static void atk29_jumpifbyte(void);
static void atk2A_jumpifhalfword(void);
static void atk2B_jumpifword(void);
static void atk2C_jumpifarrayequal(void);
static void atk2D_jumpifarraynotequal(void);
static void atk2E_setbyte(void);
static void atk2F_addbyte(void);
static void atk30_subbyte(void);
static void atk31_copyarray(void);
static void atk32_copyarraywithindex(void);
static void atk33_orbyte(void);
static void atk34_orhalfword(void);
static void atk35_orword(void);
static void atk36_bicbyte(void);
static void atk37_bichalfword(void);
static void atk38_bicword(void);
static void atk39_pause(void);
static void atk3A_waitstate(void);
static void atk3B_healthbar_update(void);
static void atk3C_return(void);
static void atk3D_end(void);
static void atk3E_end2(void);
static void atk3F_end3(void);
static void atk40_jumpifaffectedbyprotect(void);
static void atk41_call(void);
static void atk42_jumpiftype2(void);
static void atk43_jumpifabilitypresent(void);
static void atk44_endselectionscript(void);
static void atk45_playanimation(void);
static void atk46_playanimation2(void);
static void atk47_setgraphicalstatchangevalues(void);
static void atk48_playstatchangeanimation(void);
void atk49_moveend(void);
static void atk4A_typecalc2(void);
static void atk4B_returnatktoball(void);
static void atk4C_getswitchedmondata(void);
static void atk4D_switchindataupdate(void);
static void atk4E_switchinanim(void);
static void atk4F_jumpifcantswitch(void);
static void atk50_openpartyscreen(void);
static void atk51_switchhandleorder(void);
static void atk52_switchineffects(void);
static void atk53_trainerslidein(void);
static void atk54_playse(void);
static void atk55_fanfare(void);
static void atk56_playfaintcry(void);
static void atk57(void);
static void atk58_returntoball(void);
void atk59_handlelearnnewmove(void);
static void atk5A_yesnoboxlearnmove(void);
static void atk5B_yesnoboxstoplearningmove(void);
static void atk5C_hitanimation(void);
static void atk5D_getmoneyreward(void);
static void atk5E(void);
static void atk5F_swapattackerwithtarget(void);
static void atk60_incrementgamestat(void);
static void atk61_drawpartystatussummary(void);
static void atk62_hidepartystatussummary(void);
static void atk63_jumptorandomattack(void);
static void atk64_statusanimation(void);
static void atk65_status2animation(void);
static void atk66_chosenstatusanimation(void);
static void atk67_yesnobox(void);
static void atk68_cancelallactions(void);
static void atk69_adjustsetdamage(void);
void atk6A_removeitem(void);
static void atk6B_atknameinbuff1(void);
static void atk6C_drawlvlupbox(void);
static void atk6D_resetsentmonsvalue(void);
static void atk6E_setatktoplayer0(void);
static void atk6F_makevisible(void);
static void atk70_recordlastability(void);
static void atk71_buffermovetolearn(void);
static void atk72_jumpifplayerran(void);
static void atk73_hpthresholds(void);
static void atk74_hpthresholds2(void);
static void atk75_useitemonopponent(void);
static u16 GetFlingBasePowerAndEffect(u16 item);
static bool32 ClearDefogHazards(u8 bank, bool32 clear);
static void atk76_various(void);
static void atk77_setprotectlike(void);
static void atk78_faintifabilitynotdamp(void);
static void atk79_setatkhptozero(void);
static void atk7A_jumpifnexttargetvalid(void);
static void atk7B_tryhealhalfhealth(void);
static void atk7C_trymirrormove(void);
static void atk7D_setrain(void);
static void atk7E_setreflect(void);
static void atk7F_setseeded(void);
static void atk80_manipulatedamage(void);
static void atk81_trysetrest(void);
static void atk82_jumpifnotfirstturn(void);
static void atk83_nop(void);
static void atk84_jumpifcantmakeasleep(void);
static void atk85_stockpile(void);
static void atk86_stockpiletobasedamage(void);
static void atk87_stockpiletohpheal(void);
static void atk88_negativedamage(void);
static u8 ChangeStatBuffs(s8 statValue, u8 statId, u8 flags, const u8 *BS_ptr);
static void atk89_statbuffchange(void);
static void atk8A_normalisebuffs(void);
static void atk8B_setbide(void);
static void atk8C_confuseifrepeatingattackends(void);
static void atk8D_setmultihitcounter(void);
static void atk8E_initmultihitstring(void);
static void atk8F_forcerandomswitch(void);
static void atk90_tryconversiontypechange(void);
static void atk91_givepaydaymoney(void);
static void atk92_setlightscreen(void);
static void atk93_tryKO(void);
static void atk94_damagetohalftargethp(void);
static void atk95_setsandstorm(void);
static void atk96_weatherdamage(void);
static void atk97_tryinfatuating(void);
static void atk98_updatestatusicon(void);
static void atk99_setmist(void);
static void atk9A_setfocusenergy(void);
static void atk9B_transformdataexecution(void);
static void atk9C_setsubstitute(void);
static void atk9D_mimicattackcopy(void);
static void atk9E_metronome(void);
static void atk9F_dmgtolevel(void);
static void atkA0_psywavedamageeffect(void);
static void atkA1_counterdamagecalculator(void);
static void atkA2_mirrorcoatdamagecalculator(void);
static void atkA3_disablelastusedattack(void);
static void atkA4_trysetencore(void);
static void atkA5_painsplitdmgcalc(void);
static void atkA6_settypetorandomresistance(void);
static void atkA7_setalwayshitflag(void);
static void atkA8_copymovepermanently(void);
static void atkA9_trychoosesleeptalkmove(void);
static void atkAA_setdestinybond(void);
static void atkAB_trysetdestinybondtohappen(void);
static void atkAC_remaininghptopower(void);
static void atkAD_tryspiteppreduce(void);
static void atkAE_healpartystatus(void);
static void atkAF_cursetarget(void);
static void atkB0_trysetspikes(void);
static void atkB1_setforesight(void);
static void atkB2_trysetperishsong(void);
static void atkB3_rolloutdamagecalculation(void);
static void atkB4_jumpifconfusedandstatmaxed(void);
static void atkB5_furycuttercalc(void);
static void atkB6_happinesstodamagecalculation(void);
static void atkB7_presentdamagecalculation(void);
static void atkB8_setsafeguard(void);
static void atkB9_magnitudedamagecalculation(void);
static void atkBA_jumpifnopursuitswitchdmg(void);
static void atkBB_setsunny(void);
static void atkBC_maxattackhalvehp(void);
static void atkBD_copyfoestats(void);
static void atkBE_rapidspinfree(void);
static void atkBF_setdefensecurlbit(void);
static void atkC0_recoverbasedonsunlight(void);
static void atkC1_hiddenpowercalc(void);
static void atkC2_selectfirstvalidtarget(void);
static void atkC3_trysetfutureattack(void);
static void atkC4_trydobeatup(void);
static void atkC5_setsemiinvulnerablebit(void);
static void atkC6_clearsemiinvulnerablebit(void);
static void atkC7_setminimize(void);
static void atkC8_sethail(void);
static void atkC9_jumpifattackandspecialattackcannotfall(void);
static void atkCA_setforcedtarget(void);
static void atkCB_setcharge(void);
static void atkCC_callterrainattack(void);
static void atkCD_cureifburnedparalysedorpoisoned(void);
static void atkCE_settorment(void);
static void atkCF_jumpifnodamage(void);
static void atkD0_settaunt(void);
static void atkD1_trysethelpinghand(void);
static void atkD2_tryswapitems(void);
static void atkD3_trycopyability(void);
static void atkD4_trywish(void);
static void atkD5_trysetroots(void);
static void atkD6_doubledamagedealtifdamaged(void);
static void atkD7_setyawn(void);
static void atkD8_setdamagetohealthdifference(void);
static void atkD9_scaledamagebyhealthratio(void);
static void atkDA_tryswapabilities(void);
static void atkDB_tryimprison(void);
static void atkDC_trysetgrudge(void);
static void atkDD_weightdamagecalculation(void);
static void atkDE_assistattackselect(void);
static void atkDF_trysetmagiccoat(void);
static void atkE0_trysetsnatch(void);
static void atkE1_trygetintimidatetarget(void);
static void atkE2_switchoutabilities(void);
static void atkE3_jumpifhasnohp(void);
static void atkE4_getsecretpowereffect(void);
static void atkE5_pickup(void);
static void atkE6_docastformchangeanimation(void);
static void atkE7_trycastformdatachange(void);
static void atkE8_settypebasedhalvers(void);
static void atkE9_setweatherballtype(void);
static void atkEA_tryrecycleitem(void);
static void atkEB_settypetoterrain(void);
static void atkEC_pursuitrelated(void);
static void atkEF_snatchsetbattlers(void);
static void atkEE_removelightscreenreflect(void);
void atkEF_handleballthrow(void);
static void atkF0_givecaughtmon(void);
static void atkF1_trysetcaughtmondexflags(void);
static void atkF2_displaydexinfo(void);
static void atkF3_trygivecaughtmonnick(void);
static void atkF4_subattackerhpbydmg(void);
static void atkF5_removeattackerstatus1(void);
static void atkF6_finishaction(void);
static void atkF7_finishturn(void);
static void atkF8_jumpifholdeffect(void);

EWRAM_DATA u16 gLastUsedMove;

void (* const gBattleScriptingCommandsTable[])(void) =
{
    atk00_attackcanceler,
    atk01_accuracycheck,
    atk02_attackstring,
    atk03_ppreduce,
    atk04_critcalc,
    atk05_damagecalc,
    atk06_typecalc,
    atk07_adjustnormaldamage,
    atk08_adjustnormaldamage2,
    atk09_attackanimation,
    atk0A_waitanimation,
    atk0B_healthbarupdate,
    atk0C_datahpupdate,
    atk0D_critmessage,
    atk0E_effectivenesssound,
    atk0F_resultmessage,
    atk10_printstring,
    atk11_printselectionstring,
    atk12_waitmessage,
    atk13_printfromtable,
    atk14_printselectionstringfromtable,
    atk15_seteffectwithchance,
    atk16_seteffectprimary,
    atk17_seteffectsecondary,
    atk18_clearstatusfromeffect,
    atk19_tryfaintmon,
    atk1A_dofaintanimation,
    atk1B_cleareffectsonfaint,
    atk1C_jumpifstatus,
    atk1D_jumpifstatus2,
    atk1E_jumpifability,
    atk1F_jumpifsideaffecting,
    atk20_jumpifstat,
    atk21_jumpifstatus3condition,
    atk22_jumpiftype,
    atk23_getexp,
    atk24,
    atk25_movevaluescleanup,
    atk26_setmultihit,
    atk27_decrementmultihit,
    atk28_goto,
    atk29_jumpifbyte,
    atk2A_jumpifhalfword,
    atk2B_jumpifword,
    atk2C_jumpifarrayequal,
    atk2D_jumpifarraynotequal,
    atk2E_setbyte,
    atk2F_addbyte,
    atk30_subbyte,
    atk31_copyarray,
    atk32_copyarraywithindex,
    atk33_orbyte,
    atk34_orhalfword,
    atk35_orword,
    atk36_bicbyte,
    atk37_bichalfword,
    atk38_bicword,
    atk39_pause,
    atk3A_waitstate,
    atk3B_healthbar_update,
    atk3C_return,
    atk3D_end,
    atk3E_end2,
    atk3F_end3,
    atk40_jumpifaffectedbyprotect,
    atk41_call,
    atk42_jumpiftype2,
    atk43_jumpifabilitypresent,
    atk44_endselectionscript,
    atk45_playanimation,
    atk46_playanimation2,
    atk47_setgraphicalstatchangevalues,
    atk48_playstatchangeanimation,
    atk49_moveend,
    atk4A_typecalc2,
    atk4B_returnatktoball,
    atk4C_getswitchedmondata,
    atk4D_switchindataupdate,
    atk4E_switchinanim,
    atk4F_jumpifcantswitch,
    atk50_openpartyscreen,
    atk51_switchhandleorder,
    atk52_switchineffects,
    atk53_trainerslidein,
    atk54_playse,
    atk55_fanfare,
    atk56_playfaintcry,
    atk57,
    atk58_returntoball,
    atk59_handlelearnnewmove,
    atk5A_yesnoboxlearnmove,
    atk5B_yesnoboxstoplearningmove,
    atk5C_hitanimation,
    atk5D_getmoneyreward,
    atk5E,
    atk5F_swapattackerwithtarget,
    atk60_incrementgamestat,
    atk61_drawpartystatussummary,
    atk62_hidepartystatussummary,
    atk63_jumptorandomattack,
    atk64_statusanimation,
    atk65_status2animation,
    atk66_chosenstatusanimation,
    atk67_yesnobox,
    atk68_cancelallactions,
    atk69_adjustsetdamage,
    atk6A_removeitem,
    atk6B_atknameinbuff1,
    atk6C_drawlvlupbox,
    atk6D_resetsentmonsvalue,
    atk6E_setatktoplayer0,
    atk6F_makevisible,
    atk70_recordlastability,
    atk71_buffermovetolearn,
    atk72_jumpifplayerran,
    atk73_hpthresholds,
    atk74_hpthresholds2,
    atk75_useitemonopponent,
    atk76_various,
    atk77_setprotectlike,
    atk78_faintifabilitynotdamp,
    atk79_setatkhptozero,
    atk7A_jumpifnexttargetvalid,
    atk7B_tryhealhalfhealth,
    atk7C_trymirrormove,
    atk7D_setrain,
    atk7E_setreflect,
    atk7F_setseeded,
    atk80_manipulatedamage,
    atk81_trysetrest,
    atk82_jumpifnotfirstturn,
    atk83_nop,
    atk84_jumpifcantmakeasleep,
    atk85_stockpile,
    atk86_stockpiletobasedamage,
    atk87_stockpiletohpheal,
    atk88_negativedamage,
    atk89_statbuffchange,
    atk8A_normalisebuffs,
    atk8B_setbide,
    atk8C_confuseifrepeatingattackends,
    atk8D_setmultihitcounter,
    atk8E_initmultihitstring,
    atk8F_forcerandomswitch,
    atk90_tryconversiontypechange,
    atk91_givepaydaymoney,
    atk92_setlightscreen,
    atk93_tryKO,
    atk94_damagetohalftargethp,
    atk95_setsandstorm,
    atk96_weatherdamage,
    atk97_tryinfatuating,
    atk98_updatestatusicon,
    atk99_setmist,
    atk9A_setfocusenergy,
    atk9B_transformdataexecution,
    atk9C_setsubstitute,
    atk9D_mimicattackcopy,
    atk9E_metronome,
    atk9F_dmgtolevel,
    atkA0_psywavedamageeffect,
    atkA1_counterdamagecalculator,
    atkA2_mirrorcoatdamagecalculator,
    atkA3_disablelastusedattack,
    atkA4_trysetencore,
    atkA5_painsplitdmgcalc,
    atkA6_settypetorandomresistance,
    atkA7_setalwayshitflag,
    atkA8_copymovepermanently,
    atkA9_trychoosesleeptalkmove,
    atkAA_setdestinybond,
    atkAB_trysetdestinybondtohappen,
    atkAC_remaininghptopower,
    atkAD_tryspiteppreduce,
    atkAE_healpartystatus,
    atkAF_cursetarget,
    atkB0_trysetspikes,
    atkB1_setforesight,
    atkB2_trysetperishsong,
    atkB3_rolloutdamagecalculation,
    atkB4_jumpifconfusedandstatmaxed,
    atkB5_furycuttercalc,
    atkB6_happinesstodamagecalculation,
    atkB7_presentdamagecalculation,
    atkB8_setsafeguard,
    atkB9_magnitudedamagecalculation,
    atkBA_jumpifnopursuitswitchdmg,
    atkBB_setsunny,
    atkBC_maxattackhalvehp,
    atkBD_copyfoestats,
    atkBE_rapidspinfree,
    atkBF_setdefensecurlbit,
    atkC0_recoverbasedonsunlight,
    atkC1_hiddenpowercalc,
    atkC2_selectfirstvalidtarget,
    atkC3_trysetfutureattack,
    atkC4_trydobeatup,
    atkC5_setsemiinvulnerablebit,
    atkC6_clearsemiinvulnerablebit,
    atkC7_setminimize,
    atkC8_sethail,
    atkC9_jumpifattackandspecialattackcannotfall,
    atkCA_setforcedtarget,
    atkCB_setcharge,
    atkCC_callterrainattack,
    atkCD_cureifburnedparalysedorpoisoned,
    atkCE_settorment,
    atkCF_jumpifnodamage,
    atkD0_settaunt,
    atkD1_trysethelpinghand,
    atkD2_tryswapitems,
    atkD3_trycopyability,
    atkD4_trywish,
    atkD5_trysetroots,
    atkD6_doubledamagedealtifdamaged,
    atkD7_setyawn,
    atkD8_setdamagetohealthdifference,
    atkD9_scaledamagebyhealthratio,
    atkDA_tryswapabilities,
    atkDB_tryimprison,
    atkDC_trysetgrudge,
    atkDD_weightdamagecalculation,
    atkDE_assistattackselect,
    atkDF_trysetmagiccoat,
    atkE0_trysetsnatch,
    atkE1_trygetintimidatetarget,
    atkE2_switchoutabilities,
    atkE3_jumpifhasnohp,
    atkE4_getsecretpowereffect,
    atkE5_pickup,
    atkE6_docastformchangeanimation,
    atkE7_trycastformdatachange,
    atkE8_settypebasedhalvers,
    atkE9_setweatherballtype,
    atkEA_tryrecycleitem,
    atkEB_settypetoterrain,
    atkEC_pursuitrelated,
    atkEF_snatchsetbattlers,
    atkEE_removelightscreenreflect,
    atkEF_handleballthrow,
    atkF0_givecaughtmon,
    atkF1_trysetcaughtmondexflags,
    atkF2_displaydexinfo,
    atkF3_trygivecaughtmonnick,
    atkF4_subattackerhpbydmg,
    atkF5_removeattackerstatus1,
    atkF6_finishaction,
    atkF7_finishturn,
    atkF8_jumpifholdeffect,
};

struct StatFractions
{
    u8 dividend;
    u8 divisor;
};

static const struct StatFractions gAccuracyStageRatios[] =
{
    { 33, 100}, // -6
    { 36, 100}, // -5
    { 43, 100}, // -4
    { 50, 100}, // -3
    { 60, 100}, // -2
    { 75, 100}, // -1
    {  1,   1}, //  0
    {133, 100}, // +1
    {166, 100}, // +2
    {  2,   1}, // +3
    {233, 100}, // +4
    {133,  50}, // +5
    {  3,   1}, // +6
};

// The chance is 1/N for each stage.
static const u16 sCriticalHitChance[] = {16, 8, 4, 3, 2, 1};

static const u32 sStatusFlagsForMoveEffects[] =
{
    0x00000000,
    STATUS1_SLEEP,
    STATUS1_POISON,
    STATUS1_BURN,
    STATUS1_FREEZE,
    STATUS1_PARALYSIS,
    STATUS1_TOXIC_POISON,
    STATUS2_CONFUSION,
    STATUS2_FLINCHED,
    0x00000000,
    STATUS2_UPROAR,
    0x00000000,
    STATUS2_MULTIPLETURNS,
    STATUS2_WRAPPED,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    STATUS2_RECHARGE,
    0x00000000,
    0x00000000,
    STATUS2_ESCAPE_PREVENTION,
    STATUS2_NIGHTMARE,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    STATUS2_LOCK_CONFUSE,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000
};

u8* const gMoveEffectBS_Ptrs[] =
{
    BattleScript_MoveEffectSleep,
    BattleScript_MoveEffectSleep,
    BattleScript_MoveEffectPoison,
    BattleScript_MoveEffectBurn,
    BattleScript_MoveEffectFreeze,
    BattleScript_MoveEffectParalysis,
    BattleScript_MoveEffectToxic,
    BattleScript_MoveEffectConfusion,
    BattleScript_MoveEffectSleep,
    BattleScript_MoveEffectSleep,
    BattleScript_MoveEffectUproar,
    BattleScript_MoveEffectPayDay,
    BattleScript_MoveEffectSleep,
    BattleScript_MoveEffectWrap,
    BattleScript_MoveEffectRecoil33,
    BattleScript_MoveEffectSleep,
    BattleScript_MoveEffectSleep,
    BattleScript_MoveEffectSleep,
    BattleScript_MoveEffectSleep,
    BattleScript_MoveEffectSleep,
    BattleScript_MoveEffectSleep,
    BattleScript_MoveEffectSleep,
    BattleScript_MoveEffectSleep,
    BattleScript_MoveEffectSleep,
    BattleScript_MoveEffectSleep,
    BattleScript_MoveEffectSleep,
    BattleScript_MoveEffectSleep,
    BattleScript_MoveEffectSleep,
    BattleScript_MoveEffectSleep,
    BattleScript_MoveEffectSleep,
    BattleScript_MoveEffectSleep,
    BattleScript_MoveEffectSleep,
    BattleScript_MoveEffectSleep,
    BattleScript_MoveEffectSleep,
    BattleScript_MoveEffectSleep,
    BattleScript_MoveEffectSleep,
    BattleScript_MoveEffectSleep,
    BattleScript_MoveEffectSleep,
    BattleScript_MoveEffectRecoil33
};

const u8 sUnreferencedBitMask1[] = {0, 1, 3, 7, 0xF, 0x1F, 0x3F};

const u8 gLevelUpStatBoxStats[] =
{
    MON_DATA_MAX_HP, MON_DATA_SPATK, MON_DATA_ATK,
    MON_DATA_SPDEF, MON_DATA_DEF, MON_DATA_SPEED
};

static const u16 sProtectSuccessRates[] = {0xFFFF, 0x7FFF, 0x3FFF, 0x1FFF};

#define MIMIC_FORBIDDEN_END             0xFFFE
#define METRONOME_FORBIDDEN_END         0xFFFF
#define ASSIST_FORBIDDEN_END            0xFFFF

static const u16 sMovesForbiddenToCopy[] =
{
    MOVE_METRONOME,
    MOVE_STRUGGLE,
    MOVE_SKETCH,
    MOVE_MIMIC,
    MIMIC_FORBIDDEN_END,
    MOVE_COUNTER,
    MOVE_MIRROR_COAT,
    MOVE_PROTECT,
    MOVE_DETECT,
    MOVE_ENDURE,
    MOVE_DESTINY_BOND,
    MOVE_SLEEP_TALK,
    MOVE_THIEF,
    MOVE_FOLLOW_ME,
    MOVE_SNATCH,
    MOVE_HELPING_HAND,
    MOVE_COVET,
    MOVE_TRICK,
    MOVE_FOCUS_PUNCH,
    METRONOME_FORBIDDEN_END
};

static const u8 sFlailHpScaleToPowerTable[] = //reversal+flail HP thresholds to power
{
    1, 200,
    4, 150,
    9, 100,
    16, 80,
    32, 40,
    48, 20
};

static const u16 sNaturePowerMoves[] =
{
    MOVE_STUN_SPORE,
    MOVE_RAZOR_LEAF,
    MOVE_EARTHQUAKE,
    MOVE_HYDRO_PUMP,
    MOVE_SURF,
    MOVE_BUBBLE_BEAM,
    MOVE_ROCK_SLIDE,
    MOVE_SHADOW_BALL,
    MOVE_SWIFT,
    MOVE_SWIFT
};

// weight-based damage table for Low Kick
// format: min. weight (hectograms), base power
static const u16 sWeightToDamageTable[] =
{
    100, 20,
    250, 40,
    500, 60,
    1000, 80,
    2000, 100,
    0xFFFF, 0xFFFF
};

static const u16 sPickupItems[10][22] =
{
    {ITEM_POTION,       30, ITEM_ANTIDOTE,     40, ITEM_SUPER_POTION, 50, ITEM_GREAT_BALL,   60, ITEM_REPEL,        70, ITEM_ESCAPE_ROPE,  80, ITEM_X_ATTACK,     90, ITEM_FULL_HEAL,    94, ITEM_ULTRA_BALL,   98, ITEM_HYPER_POTION, 99, ITEM_NUGGET,       1},
    {ITEM_ANTIDOTE,     30, ITEM_SUPER_POTION, 40, ITEM_GREAT_BALL,   50, ITEM_REPEL,        60, ITEM_ESCAPE_ROPE,  70, ITEM_X_ATTACK,     80, ITEM_FULL_HEAL,    90, ITEM_ULTRA_BALL,   94, ITEM_HYPER_POTION, 98, ITEM_NUGGET,       99, ITEM_KINGS_ROCK,   1},
    {ITEM_SUPER_POTION, 30, ITEM_GREAT_BALL,   40, ITEM_REPEL,        50, ITEM_ESCAPE_ROPE,  60, ITEM_X_ATTACK,     70, ITEM_FULL_HEAL,    80, ITEM_ULTRA_BALL,   90, ITEM_HYPER_POTION, 94, ITEM_RARE_CANDY,   98, ITEM_KINGS_ROCK,   99, ITEM_FULL_RESTORE, 1},
    {ITEM_GREAT_BALL,   30, ITEM_REPEL,        40, ITEM_ESCAPE_ROPE,  50, ITEM_X_ATTACK,     60, ITEM_FULL_HEAL,    70, ITEM_ULTRA_BALL,   80, ITEM_HYPER_POTION, 90, ITEM_RARE_CANDY,   94, ITEM_PROTEIN,      98, ITEM_FULL_RESTORE, 99, ITEM_ETHER,        1},
    {ITEM_REPEL,        30, ITEM_ESCAPE_ROPE,  40, ITEM_X_ATTACK,     50, ITEM_FULL_HEAL,    60, ITEM_ULTRA_BALL,   70, ITEM_HYPER_POTION, 80, ITEM_RARE_CANDY,   90, ITEM_PROTEIN,      94, ITEM_REVIVE,       98, ITEM_ETHER,        99, ITEM_WHITE_HERB,   1},
    {ITEM_ESCAPE_ROPE,  30, ITEM_X_ATTACK,     40, ITEM_FULL_HEAL,    50, ITEM_ULTRA_BALL,   60, ITEM_HYPER_POTION, 70, ITEM_RARE_CANDY,   80, ITEM_PROTEIN,      90, ITEM_REVIVE,       94, ITEM_HP_UP,        98, ITEM_WHITE_HERB,   99, ITEM_TM44,         1},
    {ITEM_X_ATTACK,     30, ITEM_FULL_HEAL,    40, ITEM_ULTRA_BALL,   50, ITEM_HYPER_POTION, 60, ITEM_RARE_CANDY,   70, ITEM_PROTEIN,      80, ITEM_REVIVE,       90, ITEM_HP_UP,        94, ITEM_FULL_RESTORE, 98, ITEM_TM44,         99, ITEM_ELIXIR,       1},
    {ITEM_FULL_HEAL,    30, ITEM_ULTRA_BALL,   40, ITEM_HYPER_POTION, 50, ITEM_RARE_CANDY,   60, ITEM_PROTEIN,      70, ITEM_REVIVE,       80, ITEM_HP_UP,        90, ITEM_FULL_RESTORE, 94, ITEM_MAX_REVIVE,   98, ITEM_ELIXIR,       99, ITEM_TM01,         1},
    {ITEM_ULTRA_BALL,   30, ITEM_HYPER_POTION, 40, ITEM_RARE_CANDY,   50, ITEM_PROTEIN,      60, ITEM_REVIVE,       70, ITEM_HP_UP,        80, ITEM_FULL_RESTORE, 90, ITEM_MAX_REVIVE,   94, ITEM_PP_UP,        98, ITEM_TM01,         99, ITEM_LEFTOVERS,    1},
    {ITEM_HYPER_POTION, 30, ITEM_RARE_CANDY,   40, ITEM_PROTEIN,      50, ITEM_REVIVE,       60, ITEM_HP_UP,        70, ITEM_FULL_RESTORE, 80, ITEM_MAX_REVIVE,   90, ITEM_PP_UP,        94, ITEM_MAX_ELIXIR,   98, ITEM_LEFTOVERS,    99, ITEM_TM26,         1},
};

static const u8 sTerrainToType[] =
{
    TYPE_GRASS, // tall grass
    TYPE_GRASS, // long grass
    TYPE_GROUND, // sand
    TYPE_WATER, // underwater
    TYPE_WATER, // water
    TYPE_WATER, // pond water
    TYPE_ROCK, // rock
    TYPE_ROCK, // cave
    TYPE_NORMAL, // building
    TYPE_NORMAL, // plain
};

static const u8 sBallCatchBonuses[] =
{
    20, 15, 10, 15 // Ultra, Great, Poke, Safari
};

static void atk00_attackcanceler(void)
{
    s32 i;

    if (gBattleOutcome != 0)
    {
        gCurrentActionFuncId = B_ACTION_FINISHED;
        return;
    }
    if (gBattleMons[gBattlerAttacker].hp == 0 && !(gHitMarker & HITMARKER_NO_ATTACKSTRING))
    {
        gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
        gBattlescriptCurrInstr = BattleScript_MoveEnd;
        return;
    }
    if (AtkCanceller_UnableToUseMove())
        return;
    if (AbilityBattleEffects(ABILITYEFFECT_MOVES_BLOCK, gBattlerTarget, 0, 0, 0))
        return;
    if (!gBattleMons[gBattlerAttacker].pp[gCurrMovePos] && gCurrentMove != MOVE_STRUGGLE && !(gHitMarker & (HITMARKER_x800000 | HITMARKER_NO_ATTACKSTRING))
     && !(gBattleMons[gBattlerAttacker].status2 & STATUS2_MULTIPLETURNS))
    {
        gBattlescriptCurrInstr = BattleScript_NoPPForMove;
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        return;
    }

    gHitMarker &= ~(HITMARKER_x800000);

    if (!(gHitMarker & HITMARKER_OBEYS) && !(gBattleMons[gBattlerAttacker].status2 & STATUS2_MULTIPLETURNS))
    {
        i = IsMonDisobedient(); // why use the 'i' variable...?
        switch (i)
        {
        case 0:
            break;
        case 2:
            gHitMarker |= HITMARKER_OBEYS;
            return;
        default:
            gMoveResultFlags |= MOVE_RESULT_MISSED;
            return;
        }
    }

    gHitMarker |= HITMARKER_OBEYS;

    if (gProtectStructs[gBattlerTarget].bounceMove && gBattleMoves[gCurrentMove].flags & F_AFFECTED_BY_MAGIC_COAT)
    {
        PressurePPLose(gBattlerAttacker, gBattlerTarget, MOVE_MAGIC_COAT);
        gProtectStructs[gBattlerTarget].bounceMove = 0;
        BattleScriptPushCursor();
        gBattlescriptCurrInstr = BattleScript_MagicCoatBounce;
        return;
    }

    for (i = 0; i < gBattlersCount; i++)
    {
        if ((gProtectStructs[gBattlerByTurnOrder[i]].stealMove) && gBattleMoves[gCurrentMove].flags & F_AFFECTED_BY_SNATCH)
        {
            PressurePPLose(gBattlerAttacker, gBattlerByTurnOrder[i], MOVE_SNATCH);
            gProtectStructs[gBattlerByTurnOrder[i]].stealMove = 0;
            gBattleStruct->scriptingActive = gBattlerByTurnOrder[i];
            BattleScriptPushCursor();
            gBattlescriptCurrInstr = BattleScript_SnatchedMove;
            return;
        }
    }

    if (gSpecialStatuses[gBattlerTarget].lightningRodRedirected)
    {
        gSpecialStatuses[gBattlerTarget].lightningRodRedirected = 0;
        gLastUsedAbility = ABILITY_LIGHTNING_ROD;
        BattleScriptPushCursor();
        gBattlescriptCurrInstr = BattleScript_TookAttack;
        RecordAbilityBattle(gBattlerTarget, gLastUsedAbility);
    }
    else if (DEFENDER_IS_PROTECTED
     && (gCurrentMove != MOVE_CURSE || IS_BATTLER_OF_TYPE(gBattlerAttacker, TYPE_GHOST))
     && ((!IsTwoTurnsMove(gCurrentMove) || (gBattleMons[gBattlerAttacker].status2 & STATUS2_MULTIPLETURNS))))
    {
        CancelMultiTurnMoves(gBattlerAttacker);
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gLastLandedMoves[gBattlerTarget] = 0;
        gLastHitByType[gBattlerTarget] = 0;
        gBattleCommunication[6] = 1;
        gBattlescriptCurrInstr++;
    }
    else
    {
        gBattlescriptCurrInstr++;
    }
}

static void JumpIfMoveFailed(u8 adder, u16 move)
{
    const u8 *BS_ptr = gBattlescriptCurrInstr + adder;
    if (gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
    {
        gLastLandedMoves[gBattlerTarget] = 0;
        gLastHitByType[gBattlerTarget] = 0;
        BS_ptr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
    else
    {
        TrySetDestinyBondToHappen();
        if (AbilityBattleEffects(ABILITYEFFECT_ABSORBING, gBattlerTarget, 0, 0, move))
            return;
    }
    gBattlescriptCurrInstr = BS_ptr;
}

static void atk40_jumpifaffectedbyprotect(void)
{
    if (DEFENDER_IS_PROTECTED)
    {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        JumpIfMoveFailed(5, 0);
        gBattleCommunication[6] = 1;
    }
    else
    {
        gBattlescriptCurrInstr += 5;
    }
}

static bool8 JumpIfMoveAffectedByProtect(u16 move)
{
    bool8 affected = FALSE;
    if (DEFENDER_IS_PROTECTED)
    {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        JumpIfMoveFailed(7, move);
        gBattleCommunication[6] = 1;
        affected = TRUE;
    }
    return affected;
}

static bool8 AccuracyCalcHelper(u16 move)
{
    if (gStatuses3[gBattlerTarget] & STATUS3_ALWAYS_HITS && gDisableStructs[gBattlerTarget].battlerWithSureHit == gBattlerAttacker)
    {
        JumpIfMoveFailed(7, move);
        return TRUE;
    }
    if (GetBattlerAbility(gBattlerAttacker) == ABILITY_NO_GUARD)
    {
        JumpIfMoveFailed(7, move);
        RecordAbilityBattle(gBattlerAttacker, ABILITY_NO_GUARD);
        return TRUE;
    }
    if (GetBattlerAbility(gBattlerTarget) == ABILITY_NO_GUARD)
    {
        JumpIfMoveFailed(7, move);
        RecordAbilityBattle(gBattlerTarget, ABILITY_NO_GUARD);
        return TRUE;
    }

    if (!(gHitMarker & HITMARKER_IGNORE_ON_AIR) && gStatuses3[gBattlerTarget] & STATUS3_ON_AIR)
    {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        JumpIfMoveFailed(7, move);
        return TRUE;
    }

    gHitMarker &= ~HITMARKER_IGNORE_ON_AIR;

    if (!(gHitMarker & HITMARKER_IGNORE_UNDERGROUND) && gStatuses3[gBattlerTarget] & STATUS3_UNDERGROUND)
    {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        JumpIfMoveFailed(7, move);
        return TRUE;
    }

    gHitMarker &= ~HITMARKER_IGNORE_UNDERGROUND;

    if (!(gHitMarker & HITMARKER_IGNORE_UNDERWATER) && gStatuses3[gBattlerTarget] & STATUS3_UNDERWATER)
    {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        JumpIfMoveFailed(7, move);
        return TRUE;
    }

    gHitMarker &= ~HITMARKER_IGNORE_UNDERWATER;

    if ((WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_RAIN_ANY) && gBattleMoves[move].effect == EFFECT_THUNDER)
     || (WEATHER_HAS_EFFECT && ((gBattleWeather & WEATHER_HAIL_ANY) || gBattleGlobalTimers.hailPermanent) && move == MOVE_BLIZZARD) // check for blizzard explicitly bc too lazy to add an effect
     || (gBattleMoves[move].effect == EFFECT_ALWAYS_HIT || gBattleMoves[move].effect == EFFECT_VITAL_THROW || gBattleMoves[move].effect == EFFECT_TRUMP_CARD)) // all these always hit
    {
        JumpIfMoveFailed(7, move);
        return TRUE;
    }

    return FALSE;
}

static void atk01_accuracycheck(void)
{
    u16 move = T2_READ_16(gBattlescriptCurrInstr + 5);

    if (move == NO_ACC_CALC || move == NO_ACC_CALC_CHECK_LOCK_ON)
    {
        if (gStatuses3[gBattlerTarget] & STATUS3_ALWAYS_HITS && move == NO_ACC_CALC_CHECK_LOCK_ON && gDisableStructs[gBattlerTarget].battlerWithSureHit == gBattlerAttacker)
            gBattlescriptCurrInstr += 7;
        else if (gStatuses3[gBattlerTarget] & (STATUS3_ON_AIR | STATUS3_UNDERGROUND | STATUS3_UNDERWATER))
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
        else if (!JumpIfMoveAffectedByProtect(0))
            gBattlescriptCurrInstr += 7;
    }
    else
    {
        int i;
        u8 type, moveAcc, atkHoldEffect, defHoldEffect, atkQuality, defQuality;
        s8 buff;
        u16 calc;

        if (move == ACC_CURR_MOVE)
            move = gCurrentMove;

        GET_MOVE_TYPE(move, type);

        if (JumpIfMoveAffectedByProtect(move))
            return;
        if (AccuracyCalcHelper(move))
            return;

        if (gBattleMons[gBattlerTarget].status2 & STATUS2_FORESIGHT || gStatuses3[gBattlerTarget] & STATUS3_MIRACLE_EYED)
        {
            u8 acc = gBattleMons[gBattlerAttacker].statStages[STAT_STAGE_ACC];
            buff = acc;
        }
        else
        {
            u8 acc = gBattleMons[gBattlerAttacker].statStages[STAT_STAGE_ACC];
            buff = acc + 6 - gBattleMons[gBattlerTarget].statStages[STAT_STAGE_EVASION];
        }

        if (buff < 0)
            buff = 0;
        if (buff > 0xC)
            buff = 0xC;

        moveAcc = gBattleMoves[move].accuracy;

        // check Thunder on sunny weather
        if (WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_SUN_ANY && gBattleMoves[move].effect == EFFECT_THUNDER)
            moveAcc = 50;

        calc = gAccuracyStageRatios[buff].dividend * moveAcc;
        calc /= gAccuracyStageRatios[buff].divisor;

        if (gBattleGlobalTimers.fog)
            calc = (calc * 60) / 100; // .6 fog multiplier
        if (gBattleGlobalTimers.gravityTimer)
            calc = (calc * 5) / 3; // 5/3 gravity multiplier

        if (GetBattlerAbility(gBattlerAttacker) == ABILITY_COMPOUND_EYES)
            calc = (calc * 130) / 100; // 1.3 compound eyes boost
        if (WEATHER_HAS_EFFECT && GetBattlerAbility(gBattlerTarget) == ABILITY_SAND_VEIL && gBattleWeather & WEATHER_SANDSTORM_ANY)
            calc = (calc * 80) / 100; // 1.2 sand veil loss;
        if (GetBattlerAbility(gBattlerAttacker) == ABILITY_HUSTLE && gBattleMoves[gCurrentMove].split == MOVE_PHYSICAL)
            calc = (calc * 80) / 100; // 1.2 hustle loss;
        if (WEATHER_HAS_EFFECT && GetBattlerAbility(gBattlerTarget) == ABILITY_SNOW_CLOAK && gBattleWeather & WEATHER_HAIL)
            calc = (calc * 80) / 100; // 1.2 snow cloak loss;
        if (GetBattlerAbility(gBattlerTarget) == ABILITY_TANGLED_FEET && gBattleMons[gBattlerTarget].status2 & STATUS2_CONFUSION)
            calc = (calc * 50) / 100; // halve it for tangled feet

        for (i = 0; i <= 5; i++)
        {
            if (GetAbilityBySpecies(GetMonData(&gPlayerParty[i], MON_DATA_SPECIES, NULL), GetMonData(&gPlayerParty[i], MON_DATA_ALT_ABILITY, NULL), GetMonData(&gPlayerParty[i], MON_DATA_HIDDEN_ABILITY, NULL)) == ABILITY_VICTORY_STAR)
            {
                calc = (calc * 110) / 100; // 1.1 victory star boost
                break;
            }
        }

        if (gBattleMons[gBattlerTarget].item == ITEM_ENIGMA_BERRY)
        {
            defHoldEffect = gEnigmaBerries[gBattlerTarget].holdEffect;
            defQuality = gEnigmaBerries[gBattlerTarget].holdEffectParam;
        }
        else
        {
            defHoldEffect = ItemId_GetHoldEffect(gBattleMons[gBattlerTarget].item);
            defQuality = ItemId_GetHoldEffectParam(gBattleMons[gBattlerTarget].item);
        }

        if (gBattleMons[gBattlerAttacker].item == ITEM_ENIGMA_BERRY)
        {
            atkHoldEffect = gEnigmaBerries[gBattlerAttacker].holdEffect;
            atkQuality = gEnigmaBerries[gBattlerAttacker].holdEffectParam;
        }
        else
        {
            atkHoldEffect = ItemId_GetHoldEffect(gBattleMons[gBattlerAttacker].item);
            atkQuality = ItemId_GetHoldEffectParam(gBattleMons[gBattlerAttacker].item);
        }

        gPotentialItemEffectBattler = gBattlerTarget;

        if (defHoldEffect == HOLD_EFFECT_EVASION_UP)
            calc = (calc * (100 - defQuality)) / 100;
        if (atkHoldEffect == HOLD_EFFECT_BOOST_ACCURACY)
            calc *= (110 / 100);
        if (atkHoldEffect == HOLD_EFFECT_ZOOM_LENS && GetBattlerTurnOrderNum(gBattlerAttacker) > GetBattlerTurnOrderNum(gBattlerTarget)) // the opponent mon went first
            calc *= (120 / 100); 

        // final calculation
        if ((Random() % 100 + 1) > calc)
        {
            gMoveResultFlags |= MOVE_RESULT_MISSED;
            if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE &&
                (gBattleMoves[move].target == 0x8 || gBattleMoves[move].target == 0x20))
                gBattleCommunication[6] = 2;
            else
                gBattleCommunication[6] = 0;
            CheckWonderGuardAndLevitate();
        }
        JumpIfMoveFailed(7, move);
    }
}

static void atk02_attackstring(void)
{
    if (gBattleControllerExecFlags)
         return;
    if (!(gHitMarker & (HITMARKER_NO_ATTACKSTRING | HITMARKER_ATTACKSTRING_PRINTED)))
    {
        PrepareStringBattle(4, gBattlerAttacker);
        gHitMarker |= HITMARKER_ATTACKSTRING_PRINTED;
    }
    gBattlescriptCurrInstr++;
    gBattleCommunication[MSG_DISPLAY] = 0;
}

static void atk03_ppreduce(void)
{
    s32 ppToDeduct = 1;

    if (gBattleControllerExecFlags)
        return;

    if (!gSpecialStatuses[gBattlerAttacker].ppNotAffectedByPressure)
    {
        switch (gBattleMoves[gCurrentMove].target)
        {
        case MOVE_TARGET_FOES_AND_ALLY:
            ppToDeduct += AbilityBattleEffects(ABILITYEFFECT_COUNT_ON_FIELD, gBattlerAttacker, ABILITY_PRESSURE, 0, 0);
            break;
        case MOVE_TARGET_BOTH:
        case MOVE_TARGET_OPPONENTS_FIELD:
            ppToDeduct += AbilityBattleEffects(ABILITYEFFECT_COUNT_OTHER_SIZE, gBattlerAttacker, ABILITY_PRESSURE, 0, 0);
            break;
        default:
            if (gBattlerAttacker != gBattlerTarget && GetBattlerAbility(gBattlerTarget) == ABILITY_PRESSURE)
                ppToDeduct++;
            break;
        }
    }

    if (!(gHitMarker & (HITMARKER_NO_PPDEDUCT | HITMARKER_NO_ATTACKSTRING)) && gBattleMons[gBattlerAttacker].pp[gCurrMovePos])
    {
        gProtectStructs[gBattlerAttacker].notFirstStrike = 1;

        if (gBattleMons[gBattlerAttacker].pp[gCurrMovePos] > ppToDeduct)
            gBattleMons[gBattlerAttacker].pp[gCurrMovePos] -= ppToDeduct;
        else
            gBattleMons[gBattlerAttacker].pp[gCurrMovePos] = 0;

        if (!(gBattleMons[gBattlerAttacker].status2 & STATUS2_TRANSFORMED)
            && !((gDisableStructs[gBattlerAttacker].mimickedMoves) & gBitTable[gCurrMovePos]))
        {
            gActiveBattler = gBattlerAttacker;
            BtlController_EmitSetMonData(0, REQUEST_PPMOVE1_BATTLE + gCurrMovePos, 0, 1, &gBattleMons[gBattlerAttacker].pp[gCurrMovePos]);
            MarkBattlerForControllerExec(gBattlerAttacker);
        }
    }

    gHitMarker &= ~(HITMARKER_NO_PPDEDUCT);
    gBattlescriptCurrInstr++;
}

static void atk04_critcalc(void)
{
    u8 holdEffect;
    u16 item, critChance;

    item = gBattleMons[gBattlerAttacker].item;

    if (item == ITEM_ENIGMA_BERRY)
        holdEffect = gEnigmaBerries[gBattlerAttacker].holdEffect;
    else
        holdEffect = ItemId_GetHoldEffect(item);

    gPotentialItemEffectBattler = gBattlerAttacker;

    critChance  = 2 * ((gBattleMons[gBattlerAttacker].status2 & STATUS2_FOCUS_ENERGY) != 0)
                + (GetBattlerAbility(gBattlerAttacker) == ABILITY_SUPER_LUCK)
                + ((gBattleMoves[gCurrentMove].flags & FLAG_HIGH_CRIT) == FLAG_HIGH_CRIT)
                + (gBattleMoves[gCurrentMove].effect == EFFECT_SKY_ATTACK)
                + (gBattleMoves[gCurrentMove].effect == EFFECT_BLAZE_KICK)
                + (gBattleMoves[gCurrentMove].effect == EFFECT_POISON_TAIL)
                + (holdEffect == HOLD_EFFECT_BOOST_CRITICAL)
                + 2 * (holdEffect == HOLD_EFFECT_LUCKY_PUNCH && gBattleMons[gBattlerAttacker].species == SPECIES_CHANSEY)
                + 2 * (holdEffect == HOLD_EFFECT_STICK && gBattleMons[gBattlerAttacker].species == SPECIES_FARFETCHD);

    if (critChance >= ARRAY_COUNT(sCriticalHitChance) - 1)
        critChance = ARRAY_COUNT(sCriticalHitChance) - 2;

    if (GetBattlerAbility(gBattlerAttacker) == ABILITY_MERCILESS && gBattleMons[gBattlerTarget].status1 & STATUS1_PSN_ANY)
        critChance = 5;

    if ((GetBattlerAbility(gBattlerTarget) != ABILITY_BATTLE_ARMOR && GetBattlerAbility(gBattlerTarget) != ABILITY_SHELL_ARMOR)
     && !(gStatuses3[gBattlerAttacker] & STATUS3_CANT_SCORE_A_CRIT)
     && !(gSideTimers[gBattlerTarget].luckyChantTimer)
     && !(gBattleTypeFlags & (BATTLE_TYPE_WALLY_TUTORIAL | BATTLE_TYPE_FIRST_BATTLE))
     && !(Random() % sCriticalHitChance[critChance]))
    {
        if (GetBattlerAbility(gBattlerAttacker) == ABILITY_SNIPER)
            gCritMultiplier = 3;
        else
            gCritMultiplier = 2;
    }
    else
        gCritMultiplier = 1;

    gBattlescriptCurrInstr++;
}

static void atk05_damagecalc(void)
{
    u16 sideStatus = gSideStatuses[GET_BATTLER_SIDE(gBattlerTarget)];
    gBattleMoveDamage = CalculateBaseDamage(&gBattleMons[gBattlerAttacker], &gBattleMons[gBattlerTarget], gCurrentMove,
                                            sideStatus, gDynamicBasePower,
                                            gBattleStruct->dynamicMoveType, gBattlerAttacker, gBattlerTarget);
    gBattleMoveDamage = gBattleMoveDamage * gCritMultiplier * gBattleStruct->dmgMultiplier;

    if (ItemId_GetHoldEffect(gBattleMons[gBattlerAttacker].item) == HOLD_EFFECT_METRONOME && gLastMoves[gBattlerAttacker] == gCurrentMove) {
        double multiplier;

        multiplier = 0.2 * gDisableStructs[gBattlerAttacker].metronomeCounter;

        if (gDisableStructs[gBattlerAttacker].metronomeCounter != 5)
            gDisableStructs[gBattlerAttacker].metronomeCounter++;
        
        gBattleMoveDamage *= (multiplier + 1);
    } else 
        gDisableStructs[gBattlerAttacker].metronomeCounter = 0;

    if (gStatuses3[gBattlerAttacker] & STATUS3_CHARGED_UP && gBattleMoves[gCurrentMove].type == TYPE_ELECTRIC)
        gBattleMoveDamage *= 2;
    if (gProtectStructs[gBattlerAttacker].helpingHand)
        gBattleMoveDamage = gBattleMoveDamage * 15 / 10;

    gBattlescriptCurrInstr++;
}

void AI_CalcDmg(u8 attacker, u8 defender)
{
    u16 sideStatus = gSideStatuses[GET_BATTLER_SIDE(defender)];
    gBattleMoveDamage = CalculateBaseDamage(&gBattleMons[attacker], &gBattleMons[defender], gCurrentMove,
                                            sideStatus, gDynamicBasePower,
                                            gBattleStruct->dynamicMoveType, attacker, defender);
    gDynamicBasePower = 0;
    gBattleMoveDamage = gBattleMoveDamage * gCritMultiplier * gBattleStruct->dmgMultiplier;

    if (ItemId_GetHoldEffect(gBattleMons[attacker].item) == HOLD_EFFECT_METRONOME && gLastMoves[attacker] == gCurrentMove) {
        double multiplier = 0.2 * gDisableStructs[attacker].metronomeCounter;
        
        gBattleMoveDamage *= (multiplier + 1);
    }

    if (gStatuses3[attacker] & STATUS3_CHARGED_UP && gBattleMoves[gCurrentMove].type == TYPE_ELECTRIC)
        gBattleMoveDamage *= 2;
    if (gProtectStructs[attacker].helpingHand)
        gBattleMoveDamage = gBattleMoveDamage * 15 / 10;
}

static void ModulateDmgByType(u8 multiplier)
{
    int i;

    switch (multiplier)
    {
    case TYPE_MUL_NO_EFFECT:
        gMoveResultFlags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
        gMoveResultFlags &= ~MOVE_RESULT_NOT_VERY_EFFECTIVE;
        gMoveResultFlags &= ~MOVE_RESULT_SUPER_EFFECTIVE;
        gDisableStructs[gBattlerTarget].wasLastMoveSuperEffective = FALSE;
        break;
    case TYPE_MUL_NOT_EFFECTIVE:
        if (gBattleMoves[gCurrentMove].power && !(gMoveResultFlags & MOVE_RESULT_NO_EFFECT))
        {
            if (gMoveResultFlags & MOVE_RESULT_SUPER_EFFECTIVE)
                gMoveResultFlags &= ~MOVE_RESULT_SUPER_EFFECTIVE;
            else
                gMoveResultFlags |= MOVE_RESULT_NOT_VERY_EFFECTIVE;
            gDisableStructs[gBattlerTarget].wasLastMoveSuperEffective = FALSE;
        }
        break;
    case TYPE_MUL_SUPER_EFFECTIVE:
        if (gBattleMoves[gCurrentMove].power && !(gMoveResultFlags & MOVE_RESULT_NO_EFFECT))
        {
            if (gMoveResultFlags & MOVE_RESULT_NOT_VERY_EFFECTIVE) {
                gMoveResultFlags &= ~MOVE_RESULT_NOT_VERY_EFFECTIVE;
                gDisableStructs[gBattlerTarget].wasLastMoveSuperEffective = FALSE;
            } else {
                gMoveResultFlags |= MOVE_RESULT_SUPER_EFFECTIVE;
                gDisableStructs[gBattlerTarget].wasLastMoveSuperEffective = TRUE;
            }
        }
        break;
    }

    for (i = 0; i < NUMBER_OF_MON_TYPES - 1; i++)
    {
        if (ItemId_GetHoldEffect(gBattleMons[gBattlerTarget].item) == HOLD_EFFECT_HALVE_BERRIES
            && multiplier > 10)
        {
            if (gBattleMons[gBattlerTarget].item == gBerryToType[i].berry
                && gBattleMoves[gCurrentMove].type == gBerryToType[i].type)
                multiplier /= 2;
        }
    }

    if (gBattleMons[gBattlerTarget].item == ITEM_CHILAN_BERRY
        && gBattleMoves[gCurrentMove].type == TYPE_NORMAL)
        multiplier /= 2;
    
    if (ItemId_GetHoldEffect(gBattleMons[gBattlerTarget].item) == HOLD_EFFECT_BOOST_EFFECTIVE && gDisableStructs[gBattlerTarget].wasLastMoveSuperEffective) {
        multiplier += 2;
    }

    gBattleMoveDamage = gBattleMoveDamage * multiplier / 10;
    if (gBattleMoveDamage == 0 && multiplier != 0)
        gBattleMoveDamage = 1;
}

static void atk06_typecalc(void)
{
    int i = 0;
    u8 moveType;
    if (gCurrentMove != MOVE_STRUGGLE)
    {
        if (gBattleStruct->dynamicMoveType)
            moveType = gBattleStruct->dynamicMoveType & 0x3F;
        else
            moveType = gBattleMoves[gCurrentMove].type;

        //check stab
        if (gBattleMons[gBattlerAttacker].type1 == moveType || gBattleMons[gBattlerAttacker].type2 == moveType)
        {
            if (gBattleMons[gBattlerAttacker].ability == ABILITY_ADAPTABILITY)
            {
                gBattleMoveDamage *= 2;
            }
            else
            {
                gBattleMoveDamage = gBattleMoveDamage * 15 / 10;
            }
        }

        if ((GetBattlerAbility(gBattlerTarget) == ABILITY_LEVITATE || gDisableStructs[gBattlerTarget].magnetRiseTimer)
         && moveType == TYPE_GROUND 
         && ItemId_GetHoldEffect(gBattleMons[gBattlerTarget].item) != HOLD_EFFECT_IRON_BALL 
         && !gBattleGlobalTimers.gravityTimer)
        {
            gLastUsedAbility = GetBattlerAbility(gBattlerTarget);
            gMoveResultFlags |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);
            gLastLandedMoves[gBattlerTarget] = 0;
            gLastHitByType[gBattlerTarget] = 0;
            gBattleCommunication[6] = moveType;
            RecordAbilityBattle(gBattlerTarget, gLastUsedAbility);
        }
        else
        {
            while (gTypeEffectiveness[i] != TYPE_ENDTABLE)
            {
                if (gTypeEffectiveness[i] == TYPE_FORESIGHT)
                {
                    if (gBattleMons[gBattlerTarget].status2 & STATUS2_FORESIGHT)
                        break;
                    i += 3;
                    continue;
                }
                else if ((gTypeEffectiveness[i + 1] == TYPE_FLYING && (gDisableStructs[gBattlerTarget].roost || gBattleGlobalTimers.gravityTimer)) // effectively eliminate flying type from the pool
                         || (gTypeEffectiveness[i] == TYPE_PSYCHIC && gTypeEffectiveness[i + 1] == TYPE_DARK && (gStatuses3[gBattlerTarget] & STATUS3_MIRACLE_EYED))) // moves ineffective against dark may be used
                {
                    i += 3;
                    continue;
                }
                else if (gTypeEffectiveness[i] == moveType)
                {
                    //check type1
                    if (gTypeEffectiveness[i + 1] == gBattleMons[gBattlerTarget].type1)
                        ModulateDmgByType(gTypeEffectiveness[i + 2]);
                    //check type2
                    if (gTypeEffectiveness[i + 1] == gBattleMons[gBattlerTarget].type2 &&
                        gBattleMons[gBattlerTarget].type1 != gBattleMons[gBattlerTarget].type2)
                        ModulateDmgByType(gTypeEffectiveness[i + 2]);
                }
                i += 3;
            }
        }

        if (GetBattlerAbility(gBattlerTarget) == ABILITY_WONDER_GUARD && AttacksThisTurn(gBattlerAttacker, gCurrentMove) == 2
         && (!(gMoveResultFlags & MOVE_RESULT_SUPER_EFFECTIVE) || ((gMoveResultFlags & (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)) == (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)))
         && gBattleMoves[gCurrentMove].power)
        {
            gLastUsedAbility = ABILITY_WONDER_GUARD;
            gMoveResultFlags |= MOVE_RESULT_MISSED;
            gLastLandedMoves[gBattlerTarget] = 0;
            gLastHitByType[gBattlerTarget] = 0;
            gBattleCommunication[6] = 3;
            RecordAbilityBattle(gBattlerTarget, gLastUsedAbility);
        }
        if (gMoveResultFlags & MOVE_RESULT_DOESNT_AFFECT_FOE)
            gProtectStructs[gBattlerAttacker].targetNotAffected = 1;
    }
    gBattlescriptCurrInstr++;
}
static void CheckWonderGuardAndLevitate(void)
{
    u8 flags = 0;
    s32 i = 0;
    u8 moveType;

    if (gCurrentMove == MOVE_STRUGGLE || !gBattleMoves[gCurrentMove].power)
        return;

    GET_MOVE_TYPE(gCurrentMove, moveType);

    if (GetBattlerAbility(gBattlerTarget) == ABILITY_LEVITATE // magnet rise here as well? 
     && moveType == TYPE_GROUND 
     && ItemId_GetHoldEffect(gBattleMons[gBattlerTarget].item) != HOLD_EFFECT_IRON_BALL 
     && !gBattleGlobalTimers.gravityTimer)
    {
        RecordAbilitySetField6(ABILITY_LEVITATE, moveType);
        return;
    }

    while (gTypeEffectiveness[i] != TYPE_ENDTABLE)
    {
        if (gTypeEffectiveness[i] == TYPE_FORESIGHT)
        {
            if (gBattleMons[gBattlerTarget].status2 & STATUS2_FORESIGHT)
                break;
            i += 3;
            continue;
        }

        if (gTypeEffectiveness[i] == moveType)
        {
            // check no effect
            if (gTypeEffectiveness[i + 1] == gBattleMons[gBattlerTarget].type1 && gTypeEffectiveness[i + 2] == 0)
            {
                gMoveResultFlags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
                gProtectStructs[gBattlerAttacker].targetNotAffected = 1;
            }
            if (gTypeEffectiveness[i + 1] == gBattleMons[gBattlerTarget].type2 &&
                gBattleMons[gBattlerTarget].type1 != gBattleMons[gBattlerTarget].type2 &&
                gTypeEffectiveness[i + 2] == 0)
            {
                gMoveResultFlags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
                gProtectStructs[gBattlerAttacker].targetNotAffected = 1;
            }

            // check super effective
            if (gTypeEffectiveness[i + 1] == gBattleMons[gBattlerTarget].type1 && gTypeEffectiveness[i + 2] == 20)
                flags |= 1;
            if (gTypeEffectiveness[i + 1] == gBattleMons[gBattlerTarget].type2
             && gBattleMons[gBattlerTarget].type1 != gBattleMons[gBattlerTarget].type2
             && gTypeEffectiveness[i + 2] == 20)
                flags |= 1;

            // check not very effective
            if (gTypeEffectiveness[i + 1] == gBattleMons[gBattlerTarget].type1 && gTypeEffectiveness[i + 2] == 5)
                flags |= 2;
            if (gTypeEffectiveness[i + 1] == gBattleMons[gBattlerTarget].type2
             && gBattleMons[gBattlerTarget].type1 != gBattleMons[gBattlerTarget].type2
             && gTypeEffectiveness[i + 2] == 5)
                flags |= 2;
        }
        i += 3;
    }

    if (GetBattlerAbility(gBattlerTarget) == ABILITY_WONDER_GUARD && AttacksThisTurn(gBattlerAttacker, gCurrentMove) == 2)
    {
        if (((flags & 2) || !(flags & 1)) && gBattleMoves[gCurrentMove].power)
        {
            RecordAbilitySetField6(ABILITY_WONDER_GUARD, 3);
        }
    }
}

static void ModulateDmgByType2(u8 multiplier, u16 move, u8* flags) // same as ModulateDmgByType except different arguments
{
    int i;

    switch (multiplier)
    {
    case TYPE_MUL_NO_EFFECT:
        *flags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
        *flags &= ~MOVE_RESULT_NOT_VERY_EFFECTIVE;
        *flags &= ~MOVE_RESULT_SUPER_EFFECTIVE;
        gDisableStructs[gBattlerTarget].wasLastMoveSuperEffective = FALSE;
        break;
    case TYPE_MUL_NOT_EFFECTIVE:
        if (gBattleMoves[move].power && !(*flags & MOVE_RESULT_NO_EFFECT))
        {
            if (*flags & MOVE_RESULT_SUPER_EFFECTIVE)
                *flags &= ~MOVE_RESULT_SUPER_EFFECTIVE;
            else
                *flags |= MOVE_RESULT_NOT_VERY_EFFECTIVE;
            gDisableStructs[gBattlerTarget].wasLastMoveSuperEffective = FALSE;
        }
        break;
    case TYPE_MUL_SUPER_EFFECTIVE:
        if (gBattleMoves[move].power && !(*flags & MOVE_RESULT_NO_EFFECT))
        {
            if (*flags & MOVE_RESULT_NOT_VERY_EFFECTIVE) {
                *flags &= ~MOVE_RESULT_NOT_VERY_EFFECTIVE;
                gDisableStructs[gBattlerTarget].wasLastMoveSuperEffective = FALSE;
            } else {
                *flags |= MOVE_RESULT_SUPER_EFFECTIVE;
                gDisableStructs[gBattlerTarget].wasLastMoveSuperEffective = TRUE;
            }
        }
        break;
    }

    for (i = 0; i < NUMBER_OF_MON_TYPES - 1; i++)
    {
        if (ItemId_GetHoldEffect(gBattleMons[gBattlerTarget].item) == HOLD_EFFECT_HALVE_BERRIES
            && multiplier == 20)
        {
            if (gBattleMons[gBattlerTarget].item == gBerryToType[i].berry
                && gBattleMoves[gCurrentMove].type == gBerryToType[i].type)
                multiplier = 10;
        }
    }
    
    gBattleMoveDamage = gBattleMoveDamage * multiplier / 10;
    if (gBattleMoveDamage == 0 && multiplier != 0)
        gBattleMoveDamage = 1;
}

u8 TypeCalc(u16 move, u8 attacker, u8 defender)
{
    s32 i = 0;
    u8 flags = 0;
    u8 moveType;

    if (move == MOVE_STRUGGLE)
        return 0;

    moveType = gBattleMoves[move].type;

    // check stab
    if (IS_BATTLER_OF_TYPE(attacker, moveType))
    {
        gBattleMoveDamage = gBattleMoveDamage * 15;
        gBattleMoveDamage = gBattleMoveDamage / 10;
    }

    if ((GetBattlerAbility(defender) == ABILITY_LEVITATE || gDisableStructs[defender].magnetRiseTimer) 
     && moveType == TYPE_GROUND 
     && ItemId_GetHoldEffect(gBattleMons[gBattlerTarget].item) != HOLD_EFFECT_IRON_BALL 
     && !gBattleGlobalTimers.gravityTimer)
    {
        flags |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);
    }
    else
    {
        while (gTypeEffectiveness[i]!= TYPE_ENDTABLE)
        {
            if (gTypeEffectiveness[i] == TYPE_FORESIGHT)
            {
                if (gBattleMons[defender].status2 & STATUS2_FORESIGHT)
                    break;
                i += 3;
                continue;
            }

            if (moveType == TYPE_GROUND
                && (gBattleMons[defender].type1 == TYPE_FLYING || gBattleMons[defender].type2 == TYPE_FLYING)
                && (ItemId_GetHoldEffect(gBattleMons[defender].item) == HOLD_EFFECT_IRON_BALL
                 || gBattleGlobalTimers.gravityTimer))
            { // if attacking with ground and the other pokemon is flying but grounded by the iron ball
                i += 3;
                continue;
            }
            else if (gTypeEffectiveness[i] == moveType)
            {
                // check type1
                if (gTypeEffectiveness[i + 1] == gBattleMons[defender].type1)
                    ModulateDmgByType2(gTypeEffectiveness[i + 2], move, &flags);
                // check type2
                if (gTypeEffectiveness[i + 1] == gBattleMons[defender].type2 &&
                    gBattleMons[defender].type1 != gBattleMons[defender].type2)
                    ModulateDmgByType2(gTypeEffectiveness[i + 2], move, &flags);
            }
            i += 3;
        }
    }

    if (GetBattlerAbility(defender) == ABILITY_WONDER_GUARD && !(flags & MOVE_RESULT_MISSED) &&
        AttacksThisTurn(attacker, move) == 2 &&
        (!(flags & MOVE_RESULT_SUPER_EFFECTIVE) || ((flags & (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)) == (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE))) &&
        gBattleMoves[move].power)
    {
        flags |= MOVE_RESULT_MISSED;
    }
    return flags;
}

u8 AI_TypeCalc(u16 move, u16 targetSpecies, u8 targetAbility)
{
    int i = 0;
    u8 flags = 0;
    u8 type1 = gBaseStats[targetSpecies].type1, type2 = gBaseStats[targetSpecies].type2, moveType;

    if (move == MOVE_STRUGGLE)
        return 0;

    moveType = gBattleMoves[move].type;

    if (targetAbility == ABILITY_LEVITATE // how do we do magnet rise here without banks?  will have to look into how ai_typecalc receives its arguments
     && moveType == TYPE_GROUND 
     && ItemId_GetHoldEffect(gBattleMons[gBattlerTarget].item) != HOLD_EFFECT_IRON_BALL 
     && !gBattleGlobalTimers.gravityTimer) {
        flags = MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE;
    }
    else
    {
        while (TYPE_EFFECT_ATK_TYPE(i) != TYPE_ENDTABLE)
        {
            if (TYPE_EFFECT_ATK_TYPE(i) == TYPE_FORESIGHT)
            {
                i += 3;
                continue;
            }
            if (TYPE_EFFECT_ATK_TYPE(i) == moveType)
            {
                // check type1
                if (TYPE_EFFECT_DEF_TYPE(i) == type1)
                    ModulateDmgByType2(TYPE_EFFECT_MULTIPLIER(i), move, &flags);
                // check type2
                if (TYPE_EFFECT_DEF_TYPE(i) == type2 && type1 != type2)
                    ModulateDmgByType2(TYPE_EFFECT_MULTIPLIER(i), move, &flags);
            }
            i += 3;
        }
    }
    if (targetAbility == ABILITY_WONDER_GUARD
     && (!(flags & MOVE_RESULT_SUPER_EFFECTIVE) || ((flags & (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)) == (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)))
     && gBattleMoves[move].power)
        flags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
    return flags;
}

// Multiplies the damage by a random factor between 85% to 100% inclusive
static inline void ApplyRandomDmgMultiplier(void)
{
    u16 rand = Random();
    u16 randPercent = 100 - (rand % 16);

    if (gBattleMoveDamage != 0)
    {
        gBattleMoveDamage *= randPercent;
        gBattleMoveDamage /= 100;
        if (gBattleMoveDamage == 0)
            gBattleMoveDamage = 1;
    }
}

void Unused_ApplyRandomDmgMultiplier(void)
{
    ApplyRandomDmgMultiplier();
}

static void atk07_adjustnormaldamage(void)
{
    u8 holdEffect, param;

    ApplyRandomDmgMultiplier();

    if (gBattleMons[gBattlerTarget].item == ITEM_ENIGMA_BERRY)
    {
        holdEffect = gEnigmaBerries[gBattlerTarget].holdEffect;
        param = gEnigmaBerries[gBattlerTarget].holdEffectParam;
    }
    else
    {
        holdEffect = ItemId_GetHoldEffect(gBattleMons[gBattlerTarget].item);
        param = ItemId_GetHoldEffectParam(gBattleMons[gBattlerTarget].item);
    }

    gPotentialItemEffectBattler = gBattlerTarget;

    if ((holdEffect == HOLD_EFFECT_FOCUS_BAND && (Random() % 100) < param)
        || (holdEffect == HOLD_EFFECT_FOCUS_SASH && gBattleMons[gBattlerTarget].hp == gBattleMons[gBattlerTarget].maxHP && !gSpecialStatuses[gBattlerTarget].focusSashed))
    {
        RecordItemEffectBattle(gBattlerTarget, holdEffect);
        gSpecialStatuses[gBattlerTarget].focusBanded = 1;
    }

    if (gBattleMons[gBattlerTarget].status2 & STATUS2_SUBSTITUTE // TODO:  hmmmm?
        || gSpecialStatuses[gBattlerTarget].focusSashed)
        goto END;

    if (holdEffect == HOLD_EFFECT_FOCUS_SASH && gBattleMons[gBattlerTarget].hp == gBattleMons[gBattlerTarget].maxHP && !gSpecialStatuses[gBattlerTarget].focusSashed)
        gSpecialStatuses[gBattlerTarget].focusSashed = 1;

    if (gBattleMons[gBattlerTarget].status2 & STATUS2_SUBSTITUTE)
        goto END;

    if (gBattleMoves[gCurrentMove].effect != EFFECT_FALSE_SWIPE && !gProtectStructs[gBattlerTarget].endured
     && !gSpecialStatuses[gBattlerTarget].focusBanded)
        goto END;

    if (gBattleMons[gBattlerTarget].hp > gBattleMoveDamage)
        goto END;

    gBattleMoveDamage = gBattleMons[gBattlerTarget].hp - 1;

    if (gProtectStructs[gBattlerTarget].endured)
    {
        gMoveResultFlags |= MOVE_RESULT_FOE_ENDURED;
    }
    else if (gSpecialStatuses[gBattlerTarget].focusBanded)
    {
        gMoveResultFlags |= MOVE_RESULT_FOE_HUNG_ON;
        gLastUsedItem = gBattleMons[gBattlerTarget].item;
    }

    END:
        gBattlescriptCurrInstr++;
}

static void atk08_adjustnormaldamage2(void) // The same as 0x7 except it doesn't check for false swipe move effect.
{
    u8 holdEffect, param;

    ApplyRandomDmgMultiplier();

    if (gBattleMons[gBattlerTarget].item == ITEM_ENIGMA_BERRY)
    {
        holdEffect = gEnigmaBerries[gBattlerTarget].holdEffect;
        param = gEnigmaBerries[gBattlerTarget].holdEffectParam;
    }
    else
    {
        holdEffect = ItemId_GetHoldEffect(gBattleMons[gBattlerTarget].item);
        param = ItemId_GetHoldEffectParam(gBattleMons[gBattlerTarget].item);
    }

    gPotentialItemEffectBattler = gBattlerTarget;

    if ((holdEffect == HOLD_EFFECT_FOCUS_BAND && (Random() % 100) < param)
        || (holdEffect == HOLD_EFFECT_FOCUS_SASH && gBattleMons[gBattlerTarget].hp == gBattleMons[gBattlerTarget].maxHP && !gSpecialStatuses[gBattlerTarget].focusSashed))
    {
        RecordItemEffectBattle(gBattlerTarget, holdEffect);
        gSpecialStatuses[gBattlerTarget].focusBanded = 1;
    }

    if (gBattleMons[gBattlerTarget].status2 & STATUS2_SUBSTITUTE
        || gSpecialStatuses[gBattlerTarget].focusSashed)
        goto END;

    if (holdEffect == HOLD_EFFECT_FOCUS_SASH && gBattleMons[gBattlerTarget].hp == gBattleMons[gBattlerTarget].maxHP && !gSpecialStatuses[gBattlerTarget].focusSashed)
        gSpecialStatuses[gBattlerTarget].focusSashed = 1;

    if (gBattleMons[gBattlerTarget].status2 & STATUS2_SUBSTITUTE)
        goto END;

    if (!gProtectStructs[gBattlerTarget].endured
     && !gSpecialStatuses[gBattlerTarget].focusBanded)
        goto END;
    if (gBattleMons[gBattlerTarget].hp > gBattleMoveDamage)
        goto END;

    gBattleMoveDamage = gBattleMons[gBattlerTarget].hp - 1;

    if (gProtectStructs[gBattlerTarget].endured)
    {
        gMoveResultFlags |= MOVE_RESULT_FOE_ENDURED;
    }
    else if (gSpecialStatuses[gBattlerTarget].focusBanded)
    {
        gMoveResultFlags |= MOVE_RESULT_FOE_HUNG_ON;
        gLastUsedItem = gBattleMons[gBattlerTarget].item;
    }

    END:
        gBattlescriptCurrInstr++;
}

static void atk09_attackanimation(void)
{
    if (gBattleControllerExecFlags)
        return;

    if ((gHitMarker & HITMARKER_NO_ANIMATIONS) && (gCurrentMove != MOVE_TRANSFORM && gCurrentMove != MOVE_SUBSTITUTE))
    {
        BattleScriptPush(gBattlescriptCurrInstr + 1);
        gBattlescriptCurrInstr = BattleScript_Pausex20;
        gBattleStruct->animTurn++;
        gBattleStruct->animTargetsHit++;
    }
    else
    {
        if ((gBattleMoves[gCurrentMove].target & MOVE_TARGET_BOTH || gBattleMoves[gCurrentMove].target & MOVE_TARGET_FOES_AND_ALLY || gBattleMoves[gCurrentMove].target & TARGET_DEPENDS) && gBattleStruct->animTargetsHit)
        {
            gBattlescriptCurrInstr++;
            return;
        }
        if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT))
        {
            gActiveBattler = gBattlerAttacker;

            //ItemBattleEffects(5, 0, FALSE);

            BtlController_EmitMoveAnimation(0, gCurrentMove, gBattleStruct->animTurn, gBattleMovePower, gBattleMoveDamage, gBattleMons[gBattlerAttacker].friendship, &gDisableStructs[gBattlerAttacker]);
            gBattleStruct->animTurn += 1;
            gBattleStruct->animTargetsHit += 1;
            MarkBattlerForControllerExec(gBattlerAttacker);
            gBattlescriptCurrInstr++;
        }
        else
        {
            BattleScriptPush(gBattlescriptCurrInstr + 1);
            gBattlescriptCurrInstr = BattleScript_Pausex20;
        }
    }
}

static void atk0A_waitanimation(void)
{
    if (gBattleControllerExecFlags == 0)
        gBattlescriptCurrInstr++;
}

static void atk0B_healthbarupdate(void)
{
    if (gBattleControllerExecFlags)
        return;

    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT))
    {
        gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);

        if (gBattleMons[gActiveBattler].status2 & STATUS2_SUBSTITUTE && gDisableStructs[gActiveBattler].substituteHP && !(gHitMarker & HITMARKER_IGNORE_SUBSTITUTE))
        {
            PrepareStringBattle(0x80, gActiveBattler);
        }
        else
        {
            // Emerald
            /*
            s16 healthValue;

            s32 currDmg = gBattleMoveDamage;
            s32 maxPossibleDmgValue = 10000; // not present in R/S, ensures that huge damage values don't change sign

            if (currDmg <= maxPossibleDmgValue)
                healthValue = currDmg;
            else
                healthValue = maxPossibleDmgValue;

            BtlController_EmitHealthBarUpdate(0, healthValue);
            */

            BtlController_EmitHealthBarUpdate(0, gBattleMoveDamage);
            MarkBattlerForControllerExec(gActiveBattler);

            if (GetBattlerSide(gActiveBattler) == B_SIDE_PLAYER && gBattleMoveDamage > 0)
                gBattleResults.playerMonWasDamaged = TRUE;
        }
    }

    gBattlescriptCurrInstr += 2;
}

static void atk0C_datahpupdate(void)
{
    u32 moveType;

    if (gBattleControllerExecFlags)
        return;

    if (gBattleStruct->dynamicMoveType == 0)
        moveType = gBattleMoves[gCurrentMove].type;
    else if (!(gBattleStruct->dynamicMoveType & 0x40))
        moveType = gBattleStruct->dynamicMoveType & 0x3F;
    else
        moveType = gBattleMoves[gCurrentMove].type;

    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT))
    {
        gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
        if (gBattleMons[gActiveBattler].status2 & STATUS2_SUBSTITUTE && gDisableStructs[gActiveBattler].substituteHP && !(gHitMarker & HITMARKER_IGNORE_SUBSTITUTE))
        {
            if (gDisableStructs[gActiveBattler].substituteHP >= gBattleMoveDamage)
            {
                if (gSpecialStatuses[gActiveBattler].dmg == 0)
                    gSpecialStatuses[gActiveBattler].dmg = gBattleMoveDamage;
                gDisableStructs[gActiveBattler].substituteHP -= gBattleMoveDamage;
                gHpDealt = gBattleMoveDamage;
            }
            else
            {
                if (gSpecialStatuses[gActiveBattler].dmg == 0)
                    gSpecialStatuses[gActiveBattler].dmg = gDisableStructs[gActiveBattler].substituteHP;
                gHpDealt = gDisableStructs[gActiveBattler].substituteHP;
                gDisableStructs[gActiveBattler].substituteHP = 0;
            }
            // check substitute fading
            if (gDisableStructs[gActiveBattler].substituteHP == 0)
            {
                gBattlescriptCurrInstr += 2;
                BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_SubstituteFade;
                return;
            }
        }
        else
        {
            gHitMarker &= ~(HITMARKER_IGNORE_SUBSTITUTE);
            if (gBattleMoveDamage < 0) // hp goes up
            {
                gBattleMons[gActiveBattler].hp -= gBattleMoveDamage;
                if (gBattleMons[gActiveBattler].hp > gBattleMons[gActiveBattler].maxHP)
                    gBattleMons[gActiveBattler].hp = gBattleMons[gActiveBattler].maxHP;

            }
            else // hp goes down
            {
                if (gHitMarker & HITMARKER_x20)
                {
                    gHitMarker &= ~(HITMARKER_x20);
                }
                else
                {
                    gTakenDmg[gActiveBattler] += gBattleMoveDamage;
                    if (gBattlescriptCurrInstr[1] == BS_TARGET)
                        gTakenDmgByBattler[gActiveBattler] = gBattlerAttacker;
                    else
                        gTakenDmgByBattler[gActiveBattler] = gBattlerTarget;
                }

                if (gBattleMons[gActiveBattler].hp > gBattleMoveDamage)
                {
                    gBattleMons[gActiveBattler].hp -= gBattleMoveDamage;
                    gHpDealt = gBattleMoveDamage;
                }
                else
                {
                    gHpDealt = gBattleMons[gActiveBattler].hp;
                    gBattleMons[gActiveBattler].hp = 0;
                }

                if (!gSpecialStatuses[gActiveBattler].dmg && !(gHitMarker & HITMARKER_x100000))
                    gSpecialStatuses[gActiveBattler].dmg = gHpDealt;

                if (gBattleMoves[gCurrentMove].split == MOVE_PHYSICAL && !(gHitMarker & HITMARKER_x100000) && gCurrentMove != MOVE_PAIN_SPLIT)
                {
                    gProtectStructs[gActiveBattler].physicalDmg = gHpDealt;
                    gSpecialStatuses[gActiveBattler].physicalDmg = gHpDealt;
                    if (gBattlescriptCurrInstr[1] == BS_TARGET)
                    {
                        gProtectStructs[gActiveBattler].physicalBattlerId = gBattlerAttacker;
                        gSpecialStatuses[gActiveBattler].physicalBattlerId = gBattlerAttacker;
                    }
                    else
                    {
                        gProtectStructs[gActiveBattler].physicalBattlerId = gBattlerTarget;
                        gSpecialStatuses[gActiveBattler].physicalBattlerId = gBattlerTarget;
                    }
                }
                else if (gBattleMoves[gCurrentMove].split != MOVE_PHYSICAL && !(gHitMarker & HITMARKER_x100000))
                {
                    gProtectStructs[gActiveBattler].specialDmg = gHpDealt;
                    gSpecialStatuses[gActiveBattler].specialDmg = gHpDealt;
                    if (gBattlescriptCurrInstr[1] == BS_TARGET)
                    {
                        gProtectStructs[gActiveBattler].specialBattlerId = gBattlerAttacker;
                        gSpecialStatuses[gActiveBattler].specialBattlerId = gBattlerAttacker;
                    }
                    else
                    {
                        gProtectStructs[gActiveBattler].specialBattlerId = gBattlerTarget;
                        gSpecialStatuses[gActiveBattler].specialBattlerId = gBattlerTarget;
                    }
                }
            }
            gHitMarker &= ~(HITMARKER_x100000);
            BtlController_EmitSetMonData(0, REQUEST_HP_BATTLE, 0, 2, &gBattleMons[gActiveBattler].hp);
            MarkBattlerForControllerExec(gActiveBattler);
        }
    }
    else
    {
        gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
        if (gSpecialStatuses[gActiveBattler].dmg == 0)
            gSpecialStatuses[gActiveBattler].dmg = 0xFFFF;
    }
    gBattlescriptCurrInstr += 2;
}

static void atk0D_critmessage(void)
{
    if (gBattleControllerExecFlags == 0)
    {
        if (gCritMultiplier >= 2 && !(gMoveResultFlags & MOVE_RESULT_NO_EFFECT))
        {
            PrepareStringBattle(STRINGID_CRITICALHIT, gBattlerAttacker);
            gBattleCommunication[MSG_DISPLAY] = 1;
        }
        gBattlescriptCurrInstr++;
    }
}

static void atk0E_effectivenesssound(void)
{
    if (gBattleControllerExecFlags)
        return;

    gActiveBattler = gBattlerTarget;
    if (!(gMoveResultFlags & MOVE_RESULT_MISSED))
    {
        switch (gMoveResultFlags & (u8)(~(MOVE_RESULT_MISSED)))
        {
        case MOVE_RESULT_SUPER_EFFECTIVE:
            BtlController_EmitPlaySE(0, SE_KOUKA_H);
            MarkBattlerForControllerExec(gActiveBattler);
            break;
        case MOVE_RESULT_NOT_VERY_EFFECTIVE:
            BtlController_EmitPlaySE(0, SE_KOUKA_L);
            MarkBattlerForControllerExec(gActiveBattler);
            break;
        case MOVE_RESULT_DOESNT_AFFECT_FOE:
        case MOVE_RESULT_FAILED:
            // no sound
            break;
        case MOVE_RESULT_FOE_ENDURED:
        case MOVE_RESULT_ONE_HIT_KO:
        case MOVE_RESULT_FOE_HUNG_ON:
        default:
            if (gMoveResultFlags & MOVE_RESULT_SUPER_EFFECTIVE)
            {
                BtlController_EmitPlaySE(0, SE_KOUKA_H);
                MarkBattlerForControllerExec(gActiveBattler);
            }
            else if (gMoveResultFlags & MOVE_RESULT_NOT_VERY_EFFECTIVE)
            {
                BtlController_EmitPlaySE(0, SE_KOUKA_L);
                MarkBattlerForControllerExec(gActiveBattler);
            }
            else if (!(gMoveResultFlags & (MOVE_RESULT_DOESNT_AFFECT_FOE | MOVE_RESULT_FAILED)))
            {
                BtlController_EmitPlaySE(0, SE_KOUKA_M);
                MarkBattlerForControllerExec(gActiveBattler);
            }
            break;
        }
    }
    gBattlescriptCurrInstr++;
}

static void atk0F_resultmessage(void)
{
    u32 stringId = 0;

    if (gBattleControllerExecFlags)
        return;

    if (gMoveResultFlags & MOVE_RESULT_MISSED && (!(gMoveResultFlags & MOVE_RESULT_DOESNT_AFFECT_FOE) || gBattleCommunication[6] > 2))
    {
        stringId = gMissStringIds[gBattleCommunication[6]];
        gBattleCommunication[MSG_DISPLAY] = 1;
    }
    else
    {
        gBattleCommunication[MSG_DISPLAY] = 1;
        switch (gMoveResultFlags & (u8)(~(MOVE_RESULT_MISSED)))
        {
        case MOVE_RESULT_SUPER_EFFECTIVE:
            stringId = STRINGID_SUPEREFFECTIVE;
            break;
        case MOVE_RESULT_NOT_VERY_EFFECTIVE:
            stringId = STRINGID_NOTVERYEFFECTIVE;
            break;
        case MOVE_RESULT_ONE_HIT_KO:
            stringId = STRINGID_ONEHITKO;
            break;
        case MOVE_RESULT_FOE_ENDURED:
            stringId = STRINGID_PKMNENDUREDHIT;
            break;
        case MOVE_RESULT_FAILED:
            stringId = STRINGID_BUTITFAILED;
            break;
        case MOVE_RESULT_DOESNT_AFFECT_FOE:
            stringId = STRINGID_ITDOESNTAFFECT;
            break;
        case MOVE_RESULT_FOE_HUNG_ON:
            gLastUsedItem = gBattleMons[gBattlerTarget].item;
            gPotentialItemEffectBattler = gBattlerTarget;
            gMoveResultFlags &= ~(MOVE_RESULT_FOE_ENDURED | MOVE_RESULT_FOE_HUNG_ON);
            BattleScriptPushCursor();
            gBattlescriptCurrInstr = BattleScript_HangedOnMsg;
            return;
        default:
            if (gMoveResultFlags & MOVE_RESULT_DOESNT_AFFECT_FOE)
            {
                stringId = STRINGID_ITDOESNTAFFECT;
            }
            else if (gMoveResultFlags & MOVE_RESULT_ONE_HIT_KO)
            {
                gMoveResultFlags &= ~(MOVE_RESULT_ONE_HIT_KO);
                gMoveResultFlags &= ~(MOVE_RESULT_SUPER_EFFECTIVE);
                gMoveResultFlags &= ~(MOVE_RESULT_NOT_VERY_EFFECTIVE);
                BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_OneHitKOMsg;
                return;
            }
            else if (gMoveResultFlags & MOVE_RESULT_FOE_ENDURED)
            {
                gMoveResultFlags &= ~(MOVE_RESULT_FOE_ENDURED | MOVE_RESULT_FOE_HUNG_ON);
                BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_EnduredMsg;
                return;
            }
            else if (gMoveResultFlags & MOVE_RESULT_FOE_HUNG_ON)
            {
                gLastUsedItem = gBattleMons[gBattlerTarget].item;
                gPotentialItemEffectBattler = gBattlerTarget;
                gMoveResultFlags &= ~(MOVE_RESULT_FOE_ENDURED | MOVE_RESULT_FOE_HUNG_ON);
                BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_HangedOnMsg;
                return;
            }
            else if (gMoveResultFlags & MOVE_RESULT_FAILED)
            {
                stringId = STRINGID_BUTITFAILED;
            }
            else
            {
                gBattleCommunication[MSG_DISPLAY] = 0;
            }
        }
    }

    if (stringId)
        PrepareStringBattle(stringId, gBattlerAttacker);

    gBattlescriptCurrInstr++;
}

static void atk10_printstring(void)
{
    if (gBattleControllerExecFlags == 0)
    {
        u16 var = T2_READ_16(gBattlescriptCurrInstr + 1);
        PrepareStringBattle(var, gBattlerAttacker);
        gBattlescriptCurrInstr += 3;
        gBattleCommunication[MSG_DISPLAY] = 1;
    }
}

static void atk11_printselectionstring(void)
{
    gActiveBattler = gBattlerAttacker;

    BtlController_EmitPrintSelectionString(0, T2_READ_16(gBattlescriptCurrInstr + 1));
    MarkBattlerForControllerExec(gActiveBattler);

    gBattlescriptCurrInstr += 3;
    gBattleCommunication[MSG_DISPLAY] = 1;
}

static void atk12_waitmessage(void)
{
    if (gBattleControllerExecFlags == 0)
    {
        if (!gBattleCommunication[MSG_DISPLAY])
        {
            gBattlescriptCurrInstr += 3;
        }
        else
        {
            u16 toWait = T2_READ_16(gBattlescriptCurrInstr + 1);
            if (++gPauseCounterBattle >= toWait)
            {
                gPauseCounterBattle = 0;
                gBattlescriptCurrInstr += 3;
                gBattleCommunication[MSG_DISPLAY] = 0;
            }
        }
    }
}

static void atk13_printfromtable(void)
{
    if (gBattleControllerExecFlags == 0)
    {
        u16 *ptr = (u16 *)T1_READ_PTR(gBattlescriptCurrInstr + 1);
        ptr += gBattleCommunication[MULTISTRING_CHOOSER];
        PrepareStringBattle(*(u16*)ptr, gBattlerAttacker);
        gBattlescriptCurrInstr += 5;
        gBattleCommunication[MSG_DISPLAY] = 1;
    }
}

static void atk14_printselectionstringfromtable(void)
{
    if (gBattleControllerExecFlags == 0)
    {
        u16 *ptr = (u16 *)T1_READ_PTR(gBattlescriptCurrInstr + 1); // FIXME
        ptr += gBattleCommunication[MULTISTRING_CHOOSER];
        gActiveBattler = gBattlerAttacker;
        BtlController_EmitPrintSelectionString(0, *(u16*)ptr);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattlescriptCurrInstr += 5;
        gBattleCommunication[MSG_DISPLAY] = 1;
    }
}

u8 GetBattlerTurnOrderNum(u8 battlerId)
{
    int i;
    for (i = 0; i < gBattlersCount; i++)
    {
        if (gBattlerByTurnOrder[i] == battlerId)
            break;
    }
    return i;
}

#define INCREMENT_RESET_RETURN                  \
{                                               \
    gBattlescriptCurrInstr++;                   \
    gBattleCommunication[MOVE_EFFECT_BYTE] = 0; \
    return;                                     \
}

#define RESET_RETURN                            \
{                                               \
    gBattleCommunication[MOVE_EFFECT_BYTE] = 0; \
    return;                                     \
}

void SetMoveEffect(bool8 primary, u8 certain)
{
    bool32 statusChanged = FALSE;
    u8 affectsUser;
    bool32 noSunCanFreeze = 1;

    if (gBattleCommunication[MOVE_EFFECT_BYTE] & MOVE_EFFECT_AFFECTS_USER)
    {
        gEffectBattler = gBattlerAttacker; //bank that effects get applied on
        gBattleCommunication[MOVE_EFFECT_BYTE] &= ~(MOVE_EFFECT_AFFECTS_USER);
        affectsUser = MOVE_EFFECT_AFFECTS_USER;
        gBattleStruct->scriptingActive = gBattlerTarget; //theoretically the attacker
    }
    else
    {
        gEffectBattler = gBattlerTarget;
        gBattleStruct->scriptingActive = gBattlerAttacker;
    }

    if (GetBattlerAbility(gEffectBattler) == ABILITY_SHIELD_DUST && !(gHitMarker & HITMARKER_IGNORE_SAFEGUARD) &&
        !primary && gBattleCommunication[MOVE_EFFECT_BYTE] <= 9)
        INCREMENT_RESET_RETURN

    if (gSideStatuses[GetBattlerPosition(gEffectBattler) & 1] & SIDE_STATUS_SAFEGUARD && !(gHitMarker & HITMARKER_IGNORE_SAFEGUARD) &&
        !primary && gBattleCommunication[MOVE_EFFECT_BYTE] <= 7)
        INCREMENT_RESET_RETURN

    //make sure at least ONE HP except payday and thief
    if (gBattleMons[gEffectBattler].hp == 0 && gBattleCommunication[MOVE_EFFECT_BYTE] != 0xB && gBattleCommunication[MOVE_EFFECT_BYTE] != 0x1F)
        INCREMENT_RESET_RETURN

    if (gBattleMons[gEffectBattler].status2 & STATUS2_SUBSTITUTE && affectsUser != MOVE_EFFECT_AFFECTS_USER)
        INCREMENT_RESET_RETURN

    if (gBattleCommunication[MOVE_EFFECT_BYTE] <= 6) //status change
    {
        switch (sStatusFlagsForMoveEffects[gBattleCommunication[MOVE_EFFECT_BYTE]])
        {
        case STATUS1_SLEEP:
            //check active uproar
            if (GetBattlerAbility(gEffectBattler) != ABILITY_SOUNDPROOF)
            {
                for (gActiveBattler = 0; 
                    gActiveBattler < gBattlersCount && !(gBattleMons[gActiveBattler].status2 & STATUS2_UPROAR); 
                    gActiveBattler++) 
                {}
            }
            else
                gActiveBattler = gBattlersCount;

            if (DoesLeafGuardProtect(gEffectBattler)
                && 
                (primary == TRUE || certain == MOVE_EFFECT_CERTAIN))
            {
                gLastUsedAbility = GetBattlerAbility(gEffectBattler);
                RecordAbilityBattle(gEffectBattler, GetBattlerAbility(gEffectBattler));

                BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = BattleScript_SLPPrevention;

                if (gHitMarker & HITMARKER_IGNORE_SAFEGUARD)
                {
                    gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                    gHitMarker &= ~(HITMARKER_IGNORE_SAFEGUARD);
                }
                else
                {
                    gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                }
                RESET_RETURN
            }

            if (gBattleMons[gEffectBattler].status1
             || gActiveBattler != gBattlersCount
             || GetBattlerAbility(gEffectBattler) == ABILITY_VITAL_SPIRIT
             || GetBattlerAbility(gEffectBattler) == ABILITY_INSOMNIA
             || DoesLeafGuardProtect(gEffectBattler))
                break;

            CancelMultiTurnMoves(gEffectBattler);
            statusChanged = TRUE;
            break;
        case STATUS1_POISON:
            if ((GetBattlerAbility(gEffectBattler) == ABILITY_IMMUNITY 
              || DoesLeafGuardProtect(gEffectBattler))
                && 
                (primary == TRUE || certain == MOVE_EFFECT_CERTAIN))
            {
                gLastUsedAbility = GetBattlerAbility(gEffectBattler);
                RecordAbilityBattle(gEffectBattler, GetBattlerAbility(gEffectBattler));

                BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = BattleScript_PSNPrevention;

                if (gHitMarker & HITMARKER_IGNORE_SAFEGUARD)
                {
                    gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                    gHitMarker &= ~(HITMARKER_IGNORE_SAFEGUARD);
                }
                else
                {
                    gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                }
                RESET_RETURN
            }
            if ((IS_BATTLER_OF_TYPE(gEffectBattler, TYPE_POISON) || IS_BATTLER_OF_TYPE(gEffectBattler, TYPE_STEEL))
                && (gHitMarker & HITMARKER_IGNORE_SAFEGUARD) 
                && (primary == TRUE || certain == MOVE_EFFECT_CERTAIN))
            {
                BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = BattleScript_PSNPrevention;

                gBattleCommunication[MULTISTRING_CHOOSER] = 2;
                RESET_RETURN
            }
            if (IS_BATTLER_OF_TYPE(gEffectBattler, TYPE_POISON)
             || IS_BATTLER_OF_TYPE(gEffectBattler, TYPE_STEEL)
             || gBattleMons[gEffectBattler].status1
             || GetBattlerAbility(gEffectBattler) == ABILITY_IMMUNITY
             || DoesLeafGuardProtect(gEffectBattler))
                break;

            statusChanged = TRUE;
            break;
        case STATUS1_BURN:
            if ((GetBattlerAbility(gEffectBattler) == ABILITY_WATER_VEIL 
              || DoesLeafGuardProtect(gEffectBattler))
                && (primary == TRUE || certain == MOVE_EFFECT_CERTAIN))
            {
                gLastUsedAbility = GetBattlerAbility(gEffectBattler);
                RecordAbilityBattle(gEffectBattler, GetBattlerAbility(gEffectBattler));

                BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = BattleScript_BRNPrevention;
                if (gHitMarker & HITMARKER_IGNORE_SAFEGUARD)
                {
                    gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                    gHitMarker &= ~(HITMARKER_IGNORE_SAFEGUARD);
                }
                else
                {
                    gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                }
                RESET_RETURN
            }
            if (IS_BATTLER_OF_TYPE(gEffectBattler, TYPE_FIRE)
                && (gHitMarker & HITMARKER_IGNORE_SAFEGUARD) 
                && (primary == TRUE || certain == MOVE_EFFECT_CERTAIN))
            {
                BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = BattleScript_BRNPrevention;

                gBattleCommunication[MULTISTRING_CHOOSER] = 2;
                return;
            }
            if (IS_BATTLER_OF_TYPE(gEffectBattler, TYPE_FIRE)
             || GetBattlerAbility(gEffectBattler) == ABILITY_WATER_VEIL
             || DoesLeafGuardProtect(gEffectBattler)
             || gBattleMons[gEffectBattler].status1)
                break;

            statusChanged = TRUE;
            break;
        case STATUS1_FREEZE:
            if (WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_SUN_ANY)
                noSunCanFreeze = FALSE;

            if (IS_BATTLER_OF_TYPE(gEffectBattler, TYPE_ICE)
             || gBattleMons[gEffectBattler].status1
             || noSunCanFreeze == 0
             || DoesLeafGuardProtect(gEffectBattler)
             || GetBattlerAbility(gEffectBattler) == ABILITY_MAGMA_ARMOR)
                break;

            CancelMultiTurnMoves(gEffectBattler);
            statusChanged = TRUE;
            break;
        case STATUS1_PARALYSIS:
            if (GetBattlerAbility(gEffectBattler) == ABILITY_LIMBER
             || DoesLeafGuardProtect(gEffectBattler))
            {
                if ((primary == TRUE || certain == MOVE_EFFECT_CERTAIN))
                {
                    gLastUsedAbility = GetBattlerAbility(gEffectBattler);
                    RecordAbilityBattle(gEffectBattler, GetBattlerAbility(gEffectBattler));

                    BattleScriptPush(gBattlescriptCurrInstr + 1);
                    gBattlescriptCurrInstr = BattleScript_PRLZPrevention;

                    if (gHitMarker & HITMARKER_IGNORE_SAFEGUARD)
                    {
                        gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                        gHitMarker &= ~(HITMARKER_IGNORE_SAFEGUARD);
                    }
                    else
                    {
                        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                    }
                    RESET_RETURN
                }
                else
                    break;
            }
            if (gBattleMons[gEffectBattler].status1)
                break;

            statusChanged = TRUE;
            break;
        case STATUS1_TOXIC_POISON:
            if ((GetBattlerAbility(gEffectBattler) == ABILITY_IMMUNITY || DoesLeafGuardProtect(gEffectBattler)) && (primary == TRUE || certain == MOVE_EFFECT_CERTAIN))
            {
                gLastUsedAbility = GetBattlerAbility(gEffectBattler);
                RecordAbilityBattle(gEffectBattler, GetBattlerAbility(gEffectBattler));

                BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = BattleScript_PSNPrevention;

                if (gHitMarker & HITMARKER_IGNORE_SAFEGUARD)
                {
                    gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                    gHitMarker &= ~(HITMARKER_IGNORE_SAFEGUARD);
                }
                else
                {
                    gBattleCommunication[MULTISTRING_CHOOSER] = 0; 
                }
                RESET_RETURN
            }
            if ((IS_BATTLER_OF_TYPE(gEffectBattler, TYPE_POISON) || IS_BATTLER_OF_TYPE(gEffectBattler, TYPE_STEEL))
                && (gHitMarker & HITMARKER_IGNORE_SAFEGUARD) 
                && (primary == TRUE || certain == MOVE_EFFECT_CERTAIN))
            {
                BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = BattleScript_PSNPrevention;

                gBattleCommunication[MULTISTRING_CHOOSER] = 2;
                RESET_RETURN
            }
            if (gBattleMons[gEffectBattler].status1)
                break;
            if (!IS_BATTLER_OF_TYPE(gEffectBattler, TYPE_POISON) && !IS_BATTLER_OF_TYPE(gEffectBattler, TYPE_STEEL))
            {
                if (GetBattlerAbility(gEffectBattler) == ABILITY_IMMUNITY)
                    break;

                statusChanged = TRUE;
                break;
            }
            else
            {
                gMoveResultFlags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
            }
            break;
        }
        if (statusChanged == TRUE)
        {
            BattleScriptPush(gBattlescriptCurrInstr + 1);

            if (sStatusFlagsForMoveEffects[gBattleCommunication[MOVE_EFFECT_BYTE]] == STATUS1_SLEEP)
                gBattleMons[gEffectBattler].status1 |= ((Random() & 3) + 2);
            else
                gBattleMons[gEffectBattler].status1 |= sStatusFlagsForMoveEffects[gBattleCommunication[MOVE_EFFECT_BYTE]];
            
            gBattlescriptCurrInstr = gMoveEffectBS_Ptrs[gBattleCommunication[MOVE_EFFECT_BYTE]];
            
            gActiveBattler = gEffectBattler;
            BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gEffectBattler].status1);
            MarkBattlerForControllerExec(gActiveBattler);

            if (gHitMarker & HITMARKER_IGNORE_SAFEGUARD)
            {
                gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                gHitMarker &= ~(HITMARKER_IGNORE_SAFEGUARD);
            }
            else
            {
                gBattleCommunication[MULTISTRING_CHOOSER] = 0;
            }

            if (gBattleCommunication[MOVE_EFFECT_BYTE] == MOVE_EFFECT_POISON 
             || gBattleCommunication[MOVE_EFFECT_BYTE] == MOVE_EFFECT_TOXIC
             || gBattleCommunication[MOVE_EFFECT_BYTE] == MOVE_EFFECT_PARALYSIS
             || gBattleCommunication[MOVE_EFFECT_BYTE] == MOVE_EFFECT_BURN)
            {
                gBattleStruct->synchroniseEffect = gBattleCommunication[MOVE_EFFECT_BYTE];
                gHitMarker |= HITMARKER_SYNCHRONISE_EFFECT;
            }
            return;
        }
        else if (statusChanged == FALSE)
        {
            gBattleCommunication[MOVE_EFFECT_BYTE] = 0;
            gBattlescriptCurrInstr++; 
            return;
        }
        return;
    }
    else
    {
        if (gBattleMons[gEffectBattler].status2 & sStatusFlagsForMoveEffects[gBattleCommunication[MOVE_EFFECT_BYTE]]) // if it already has the status trying to inflict
        {
            gBattlescriptCurrInstr++;
        }
        else
        {
            switch (gBattleCommunication[MOVE_EFFECT_BYTE])
            {
            case MOVE_EFFECT_CONFUSION:
                if (GetBattlerAbility(gEffectBattler) == ABILITY_OWN_TEMPO 
                    || gBattleMons[gEffectBattler].status2 & STATUS2_CONFUSION)
                {
                    gBattlescriptCurrInstr++;
                }
                else
                {
                    gBattleMons[gEffectBattler].status2 |= (((Random()) % 0x4)) + 2;

                    BattleScriptPush(gBattlescriptCurrInstr + 1);
                    gBattlescriptCurrInstr = gMoveEffectBS_Ptrs[gBattleCommunication[MOVE_EFFECT_BYTE]];
                }
                break;
            case MOVE_EFFECT_FLINCH:
                if (GetBattlerAbility(gEffectBattler) == ABILITY_INNER_FOCUS)
                {
                    if (primary == TRUE || certain == MOVE_EFFECT_CERTAIN)
                    {
                        gLastUsedAbility = ABILITY_INNER_FOCUS;
                        RecordAbilityBattle(gEffectBattler, ABILITY_INNER_FOCUS);
                        gBattlescriptCurrInstr = BattleScript_FlinchPrevention;
                        return;
                    }
                    else
                    {
                        gBattlescriptCurrInstr++;
                    }
                }
                else
                {
                    if (GetBattlerTurnOrderNum(gEffectBattler) > gCurrentTurnActionNumber)
                        gBattleMons[gEffectBattler].status2 |= sStatusFlagsForMoveEffects[gBattleCommunication[MOVE_EFFECT_BYTE]];
                    gBattlescriptCurrInstr++;
                }
                break;
            case MOVE_EFFECT_UPROAR:
                if (!(gBattleMons[gEffectBattler].status2 & STATUS2_UPROAR))
                {
                    gBattleMons[gEffectBattler].status2 |= STATUS2_MULTIPLETURNS;
                    gLockedMoves[gEffectBattler] = gCurrentMove;
                    gBattleMons[gEffectBattler].status2 |= ((Random() & 3) + 2) << 4;

                    BattleScriptPush(gBattlescriptCurrInstr + 1);
                    gBattlescriptCurrInstr = gMoveEffectBS_Ptrs[gBattleCommunication[MOVE_EFFECT_BYTE]];
                }
                else
                {
                    gBattlescriptCurrInstr++;
                }
                break;
            case MOVE_EFFECT_PAYDAY:
                if (!(GetBattlerPosition(gBattlerAttacker) & 1))
                {
                    u16 PayDay = gPaydayMoney;
                    gPaydayMoney += (gBattleMons[gBattlerAttacker].level * 5);
                    if (PayDay > gPaydayMoney)
                        gPaydayMoney = UINT16_MAX;
                }
                BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = gMoveEffectBS_Ptrs[gBattleCommunication[MOVE_EFFECT_BYTE]];
                break;
            case MOVE_EFFECT_TRI_ATTACK:
                if (gBattleMons[gEffectBattler].status1)
                {
                    gBattlescriptCurrInstr++;
                }
                else
                {
                    gBattleCommunication[MOVE_EFFECT_BYTE] = Random() % 3 + 3;
                    SetMoveEffect(FALSE, 0);
                }
                break;
            case MOVE_EFFECT_CHARGING:
                gBattleMons[gEffectBattler].status2 |= STATUS2_MULTIPLETURNS;
                gLockedMoves[gEffectBattler] = gCurrentMove;
                gProtectStructs[gEffectBattler].chargingTurn = 1;
                gBattlescriptCurrInstr++;
                break;
            case MOVE_EFFECT_WRAP:
                if (gBattleMons[gEffectBattler].status2 & STATUS2_WRAPPED)
                {
                    gBattlescriptCurrInstr++;
                }
                else
                {
                    if (ItemId_GetHoldEffect(gBattleMons[gBattlerAttacker].item) == HOLD_EFFECT_EXTEND_MULTITURN)
                        gBattleMons[gEffectBattler].status2 |= (7 << 0xD);
                    else
                        gBattleMons[gEffectBattler].status2 |= ((Random() & 3) + 2) << 0xD;

                    gBattleStruct->wrappedMove[gEffectBattler*2] = (u8)gCurrentMove;
                    (1 + gBattleStruct->wrappedMove)[gEffectBattler*2] = gCurrentMove >> 8; //don't ask.
                    gBattleStruct->wrappedBy[gEffectBattler] = gBattlerAttacker;

                    BattleScriptPush(gBattlescriptCurrInstr + 1);
                    gBattlescriptCurrInstr = gMoveEffectBS_Ptrs[gBattleCommunication[MOVE_EFFECT_BYTE]];
                    
                    for (gBattleCommunication[MULTISTRING_CHOOSER] = 0; ; gBattleCommunication[MULTISTRING_CHOOSER]++)
                    {
                        if (gBattleCommunication[MULTISTRING_CHOOSER] > 4)
                            break;
                        if (gTrappingMoves[gBattleCommunication[MULTISTRING_CHOOSER]] == gCurrentMove)
                            break;
                    }
                }
                break;
            case MOVE_EFFECT_RECOIL_25:
                gBattleMoveDamage = (gHpDealt) / 4;
                if (gBattleMoveDamage == 0)
                    gBattleMoveDamage = 1;

                BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = gMoveEffectBS_Ptrs[gBattleCommunication[MOVE_EFFECT_BYTE]];
                break;
            case MOVE_EFFECT_ATK_PLUS_1 ... MOVE_EFFECT_EVS_PLUS_1: //stat + 1
                if (ChangeStatBuffs(SET_STAT_BUFF_VALUE(1), 
                                    gBattleCommunication[MOVE_EFFECT_BYTE] - MOVE_EFFECT_ATK_PLUS_1 + 1, 
                                    affectsUser, 0)) 
                {
                    gBattlescriptCurrInstr++;
                }
                else
                {
                    gBattleStruct->animArg1 = gBattleCommunication[MOVE_EFFECT_BYTE] & ~(MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN);
                    gBattleStruct->animArg2 = 0;
                    BattleScriptPush(gBattlescriptCurrInstr + 1);
                    gBattlescriptCurrInstr = BattleScript_StatUp;
                }
                break;
            case MOVE_EFFECT_ATK_MINUS_1 ... MOVE_EFFECT_EVS_MINUS_1: //stat - 1
                if (ChangeStatBuffs(SET_STAT_BUFF_VALUE(1) | STAT_BUFF_NEGATIVE, 
                                    gBattleCommunication[MOVE_EFFECT_BYTE] - MOVE_EFFECT_ATK_MINUS_1 + 1, 
                                    affectsUser, 0)) 
                {
                    gBattlescriptCurrInstr++;
                }
                else
                {
                    gBattleStruct->animArg1 = gBattleCommunication[MOVE_EFFECT_BYTE] & ~(MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN);
                    gBattleStruct->animArg2 = 0;
                    BattleScriptPush(gBattlescriptCurrInstr + 1);
                    gBattlescriptCurrInstr = BattleScript_StatDown;
                }
                break;
            case MOVE_EFFECT_ATK_PLUS_2 ... MOVE_EFFECT_EVS_PLUS_2: //stat + 2
                if (ChangeStatBuffs(SET_STAT_BUFF_VALUE(2),
                                    gBattleCommunication[MOVE_EFFECT_BYTE] - MOVE_EFFECT_ATK_PLUS_2 + 1, 
                                    affectsUser, 0)) 
                {
                    gBattlescriptCurrInstr++;
                }
                else
                {
                    gBattleStruct->animArg1 = gBattleCommunication[MOVE_EFFECT_BYTE] & ~(MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN);
                    gBattleStruct->animArg2 = 0;
                    BattleScriptPush(gBattlescriptCurrInstr + 1);
                    gBattlescriptCurrInstr = BattleScript_StatUp;
                }
                break;
            case MOVE_EFFECT_ATK_MINUS_2 ... MOVE_EFFECT_EVS_MINUS_2: //stat - 2
                if (ChangeStatBuffs(SET_STAT_BUFF_VALUE(2) | STAT_BUFF_NEGATIVE,
                                    gBattleCommunication[MOVE_EFFECT_BYTE] - MOVE_EFFECT_ATK_MINUS_2 + 1,
                                    affectsUser, 0)) 
                {
                    gBattlescriptCurrInstr++;
                }
                else
                {
                    gBattleStruct->animArg1 = gBattleCommunication[MOVE_EFFECT_BYTE] & ~(MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN);
                    gBattleStruct->animArg2 = 0;
                    BattleScriptPush(gBattlescriptCurrInstr + 1);
                    gBattlescriptCurrInstr = BattleScript_StatDown;
                }
                break;
            case MOVE_EFFECT_RECHARGE:
                gBattleMons[gEffectBattler].status2 |= STATUS2_RECHARGE;
                gDisableStructs[gEffectBattler].rechargeCounter = 2;
                gLockedMoves[gEffectBattler] = gCurrentMove;
                gBattlescriptCurrInstr++;
                break;
            case MOVE_EFFECT_RAGE:
                gBattleMons[gBattlerAttacker].status2 |= STATUS2_RAGE;
                gBattlescriptCurrInstr++;
                break;
            case MOVE_EFFECT_STEAL_ITEM:
                {
                    u8 side = GetBattlerSide(gBattlerAttacker);
                    if (GetBattlerSide(gBattlerAttacker) == B_SIDE_OPPONENT
                        && !(gBattleTypeFlags & 
                             (BATTLE_TYPE_EREADER_TRAINER 
                              | BATTLE_TYPE_BATTLE_TOWER 
                              | BATTLE_TYPE_LINK)) 
                        && gTrainerBattleOpponent != 0x400)
                    {
                        gBattlescriptCurrInstr++;
                    }
                    else if (!(gBattleTypeFlags & 
                               (BATTLE_TYPE_EREADER_TRAINER 
                              | BATTLE_TYPE_BATTLE_TOWER 
                              | BATTLE_TYPE_LINK)) 
                             && gTrainerBattleOpponent != 0x400 
                             && (gWishFutureKnock.knockedOffPokes[side] & gBitTable[gBattlerPartyIndexes[gBattlerAttacker]]))
                    {
                        gBattlescriptCurrInstr++;
                    }
                    else if (gBattleMons[gBattlerTarget].item 
                        && GetBattlerAbility(gBattlerTarget) == ABILITY_STICKY_HOLD)
                    {
                        BattleScriptPushCursor();
                        gBattlescriptCurrInstr = BattleScript_NoItemSteal;

                        gLastUsedAbility = GetBattlerAbility(gBattlerTarget);
                        RecordAbilityBattle(gBattlerTarget, gLastUsedAbility);
                    }
                    else if (gBattleMons[gBattlerAttacker].item
                        || gBattleMons[gBattlerTarget].item == ITEM_ENIGMA_BERRY
                        || IS_ITEM_MAIL(gBattleMons[gBattlerTarget].item)
                        || gBattleMons[gBattlerTarget].item == 0)
                    {
                        gBattlescriptCurrInstr++;
                    }
                    else
                    {
                        gLastUsedItem = gBattleMons[gBattlerTarget].item;
                        *USED_HELD_ITEM(gBattlerAttacker) = gLastUsedItem;
                        gBattleMons[gBattlerTarget].item = 0;

                        gActiveBattler = gBattlerAttacker;
                        BtlController_EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gLastUsedItem);
                        MarkBattlerForControllerExec(gBattlerAttacker);

                        gActiveBattler = gBattlerTarget;
                        BtlController_EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gBattlerTarget].item);
                        MarkBattlerForControllerExec(gBattlerTarget);

                        BattleScriptPush(gBattlescriptCurrInstr + 1);
                        gBattlescriptCurrInstr = BattleScript_ItemSteal;

                        *CHOICED_MOVE(gBattlerTarget) = 0;
                    }
                }
                break;
            case MOVE_EFFECT_PREVENT_ESCAPE:
                gBattleMons[gBattlerTarget].status2 |= STATUS2_ESCAPE_PREVENTION;
                gDisableStructs[gBattlerTarget].bankPreventingEscape = gBattlerAttacker;
                gBattlescriptCurrInstr++;
                break;
            case MOVE_EFFECT_NIGHTMARE:
                gBattleMons[gBattlerTarget].status2 |= STATUS2_NIGHTMARE;
                gBattlescriptCurrInstr++;
                break;
            case MOVE_EFFECT_ALL_STATS_UP:
                BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = BattleScript_AllStatsUp;
                return;
            case MOVE_EFFECT_RAPIDSPIN:
                BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = BattleScript_RapidSpinAway;
                return;
            case MOVE_EFFECT_REMOVE_STATUS:
                if (gBattleMons[gBattlerTarget].status1 & gBattleMoves[gCurrentMove].argument)
                {
                    gBattleMons[gBattlerTarget].status1 &= ~(gBattleMoves[gCurrentMove].argument);

                    gActiveBattler = gBattlerTarget;
                    BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gBattlerTarget].status1);
                    MarkBattlerForControllerExec(gActiveBattler);

                    BattleScriptPush(gBattlescriptCurrInstr + 1);

                    if (gBattleMoves[gCurrentMove].argument == STATUS1_BURN)
                        gBattlescriptCurrInstr = BattleScript_TargetBurnHeal;
                    else if (gBattleMoves[gCurrentMove].argument == STATUS1_SLEEP)
                        gBattlescriptCurrInstr = BattleScript_TargetSleepHeal;
                    else if (gBattleMoves[gCurrentMove].argument == STATUS1_PARALYSIS)
                        gBattlescriptCurrInstr = BattleScript_TargetPRLZHeal;
                }
                else
                {
                    gBattlescriptCurrInstr++;
                }
                break;
            case MOVE_EFFECT_ATK_DEF_DOWN: //superpower
                BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = BattleScript_AtkDefDown;
                return;
            case MOVE_EFFECT_RECOIL_33_PARALYSIS:
                gBattleMoveDamage = gHpDealt / 3;
                if (gBattleMoveDamage == 0)
                    gBattleMoveDamage = 1;

                BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = gMoveEffectBS_Ptrs[gBattleCommunication[MOVE_EFFECT_BYTE]];
                break;
            case MOVE_EFFECT_THRASH: //thrash
                if (!(gBattleMons[gEffectBattler].status2 & STATUS2_LOCK_CONFUSE))
                {
                    gBattleMons[gEffectBattler].status2 |= STATUS2_MULTIPLETURNS;
                    gLockedMoves[gEffectBattler] = gCurrentMove;
                    gBattleMons[gEffectBattler].status2 |= (((Random() & 1) + 2) << 0xA);
                }
                else
                {
                    gBattlescriptCurrInstr++;
                }
                break;
            case MOVE_EFFECT_KNOCK_OFF:
                if (GetBattlerAbility(gEffectBattler) == ABILITY_STICKY_HOLD)
                {
                    if (gBattleMons[gEffectBattler].item == 0)
                    {
                        gBattlescriptCurrInstr++;
                    }
                    else
                    {
                        gLastUsedAbility = ABILITY_STICKY_HOLD;
                        gBattlescriptCurrInstr = BattleScript_NoItemSteal;
                        RecordAbilityBattle(gEffectBattler, ABILITY_STICKY_HOLD);
                    }
                    break;
                }
                if (gBattleMons[gEffectBattler].item)
                {
                    u8 side = GetBattlerSide(gEffectBattler);

                    gLastUsedItem = gBattleMons[gEffectBattler].item;
                    gBattleMons[gEffectBattler].item = 0;
                    gWishFutureKnock.knockedOffPokes[side] |= gBitTable[gBattlerPartyIndexes[gEffectBattler]];
                    
                    BattleScriptPush(gBattlescriptCurrInstr + 1);
                    gBattlescriptCurrInstr = BattleScript_KnockedOff;

                    *CHOICED_MOVE(gEffectBattler) = 0;
                }
                else
                {
                    gBattlescriptCurrInstr++;
                }
                break;
            case MOVE_EFFECT_HEAL_VARIOUS: // throwing a mental herb, will probably only display the last one applicable but hey
                gBattleStruct->scriptingActive = gEffectBattler;
                affectsUser = 0; // used here as the "effect" variable from ItemBattleEffects

                if (gBattleMons[gEffectBattler].status2 & STATUS2_INFATUATION)
                {
                    gBattleMons[gEffectBattler].status2 &= ~(STATUS2_INFATUATION);
                    StringCopy(gBattleTextBuff1, gStatusConditionString_LoveJpn);
                    affectsUser++;
                }
                if (gDisableStructs[gEffectBattler].tauntTimer1)
                {
                    gDisableStructs[gEffectBattler].tauntTimer1 = 0;
                    gDisableStructs[gEffectBattler].tauntTimer2 = 0;
                    StringCopy(gBattleTextBuff1, BattleText_Taunt);
                    affectsUser++;
                }
                if (gDisableStructs[gEffectBattler].encoreTimer1)
                {
                    gDisableStructs[gEffectBattler].encoredMove = 0;
                    gDisableStructs[gEffectBattler].encoreTimer1 = 0;
                    StringCopy(gBattleTextBuff1, BattleText_Encore);
                    affectsUser++;
                }
                if (gBattleMons[gEffectBattler].status2 & STATUS2_TORMENT)
                {
                    gBattleMons[gEffectBattler].status2 &= ~(STATUS2_TORMENT);
                    StringCopy(gBattleTextBuff1, BattleText_Torment);
                    affectsUser++;
                }
                if (gDisableStructs[gEffectBattler].disableTimer1)
                {
                    gDisableStructs[gEffectBattler].disabledMove = 0;
                    gDisableStructs[gEffectBattler].disableTimer1 = 0;
                    StringCopy(gBattleTextBuff1, BattleText_Disable);
                    affectsUser++;
                }
                
                if (affectsUser)
                {
                    if (affectsUser > 1)
                       StringCopy(gBattleTextBuff1, BattleText_Many);
                    BattleScriptPush(gBattlescriptCurrInstr + 1);
                    gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                    gBattlescriptCurrInstr = BattleScript_BerryCureChosenStatusRet;
                }

                affectsUser = 0;
                break;
            case MOVE_EFFECT_RESTORE_STATS:
                gBattleStruct->scriptingActive = gEffectBattler;
                statusChanged = FALSE;

                for (affectsUser = 0; affectsUser < NUM_BATTLE_STATS; affectsUser++) // using affectsUser as an i okay?
                {
                    if (gBattleMons[gEffectBattler].statStages[affectsUser] < 6)
                    {
                        gBattleMons[gEffectBattler].statStages[affectsUser] = 6;
                        statusChanged = TRUE; // used here to determine if the item needs used or not
                    }
                }

                if (statusChanged)
                {
                    BattleScriptPush(gBattlescriptCurrInstr + 1);
                    gBattlescriptCurrInstr = BattleScript_WhiteHerbFling;
                }

                statusChanged = FALSE;
                affectsUser = 0;
                break;
            case MOVE_EFFECT_RECOIL_33_STATUS:
                gBattleMoveDamage = gHpDealt / 3;
                if (gBattleMoveDamage == 0)
                    gBattleMoveDamage = 1;

                BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = BattleScript_MoveEffectRecoilWithStatus;
                break;
            case MOVE_EFFECT_RECOIL_50:
                gBattleMoveDamage = gHpDealt / 2;
                if (gBattleMoveDamage == 0)
                    gBattleMoveDamage = 1;

                BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = BattleScript_MoveEffectRecoil33;
                break;
            case MOVE_EFFECT_SP_ATK_TWO_DOWN: //overheat
                BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = BattleScript_SAtkDown2;
                break;
            case MOVE_EFFECT_BUG_BITE:
                if ((ITEM_IS_BERRY(gBattleMons[gEffectBattler].item))
                    && GetBattlerAbility(gEffectBattler) != ABILITY_STICKY_HOLD)
                {
                    gLastUsedItem = gBattleMons[gEffectBattler].item;
                    gDisableStructs[gBattlerAttacker].heldItemForBugBite = gBattleMons[gBattlerAttacker].item;
                    gBattleMons[gBattlerAttacker].item = gBattleMons[gEffectBattler].item;
                    gBattleMons[gEffectBattler].item = 0;
                    gActiveBattler = gEffectBattler;
                    gBattleStruct->scriptingActive = gEffectBattler;
                    BtlController_EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gEffectBattler].item);
                    MarkBattlerForControllerExec(gActiveBattler);
                    BattleScriptPush(gBattlescriptCurrInstr + 1);
                    gBattlescriptCurrInstr = BattleScript_MoveEffectBugBite;
                }
                break;
            }
        }
    }

    gBattleCommunication[MOVE_EFFECT_BYTE] = 0;
}

static void atk15_seteffectwithchance(void)
{
    u32 percentChance;

    if (GetBattlerAbility(gBattlerAttacker) == ABILITY_SERENE_GRACE)
        percentChance = gBattleMoves[gCurrentMove].secondaryEffectChance * 2;
    else
        percentChance = gBattleMoves[gCurrentMove].secondaryEffectChance;

    if (DEBUG && (gUnknown_02023A14_50 & 4)
        && !(gBattleCommunication[MOVE_EFFECT_BYTE] & MOVE_EFFECT_CERTAIN)
        && !(gMoveResultFlags & MOVE_RESULT_NO_EFFECT))
    {
        SetMoveEffect(0, 0);
    }
    else if ((gBattleCommunication[MOVE_EFFECT_BYTE] & MOVE_EFFECT_CERTAIN)
        && !(gMoveResultFlags & MOVE_RESULT_NO_EFFECT))
    {
        gBattleCommunication[MOVE_EFFECT_BYTE] &= ~(MOVE_EFFECT_CERTAIN);
        SetMoveEffect(0, MOVE_EFFECT_CERTAIN);
    }
    else if (Random() % 100 <= percentChance  // Random() % 100 < percentChance in Emerald
             && gBattleCommunication[MOVE_EFFECT_BYTE]
             && !(gMoveResultFlags & MOVE_RESULT_NO_EFFECT))
    {
        if (percentChance >= 100)
            SetMoveEffect(0, MOVE_EFFECT_CERTAIN);
        else
            SetMoveEffect(0, 0);
    }
    else
    {
        gBattlescriptCurrInstr++;
    }

    gBattleCommunication[MOVE_EFFECT_BYTE] = 0;
    gBattleStruct->unk16112 = 0;
}

static void atk16_seteffectprimary(void)
{
    SetMoveEffect(1, 0);
}

static void atk17_seteffectsecondary(void)
{
    SetMoveEffect(0, 0);
}

static void atk18_clearstatusfromeffect(void)
{
    gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);

    if (gBattleCommunication[MOVE_EFFECT_BYTE] <= MOVE_EFFECT_TOXIC)
        gBattleMons[gActiveBattler].status1 &= (~sStatusFlagsForMoveEffects[gBattleCommunication[MOVE_EFFECT_BYTE]]);
    else
        gBattleMons[gActiveBattler].status2 &= (~sStatusFlagsForMoveEffects[gBattleCommunication[MOVE_EFFECT_BYTE]]);

    gBattleCommunication[MOVE_EFFECT_BYTE] = 0;
    gBattlescriptCurrInstr += 2;
    gBattleStruct->unk16112 = 0;
}

static void atk19_tryfaintmon(void)
{
    const u8 *BS_ptr;

    if (gBattlescriptCurrInstr[2] != 0)
    {
        gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
        if (gHitMarker & HITMARKER_FAINTED(gActiveBattler))
        {
            BS_ptr = T1_READ_PTR(gBattlescriptCurrInstr + 3);

            BattleScriptPop();
            gBattlescriptCurrInstr = BS_ptr;
            gSideStatuses[GetBattlerSide(gActiveBattler)] &= ~(SIDE_STATUS_SPIKES_DAMAGED | SIDE_STATUS_TOXIC_SPIKES_DAMAGED | SIDE_STATUS_STEALTH_ROCK_DAMAGED);
        }
        else
        {
            gBattlescriptCurrInstr += 7;
        }
    }
    else
    {
        u8 battlerId;

        if (gBattlescriptCurrInstr[1] == 1)
        {
            gActiveBattler = gBattlerAttacker;
            battlerId = gBattlerTarget;
            BS_ptr = BattleScript_FaintAttacker;
        }
        else
        {
            gActiveBattler = gBattlerTarget;
            battlerId = gBattlerAttacker;
            BS_ptr = BattleScript_FaintTarget;
        }
        if (!(gAbsentBattlerFlags & gBitTable[gActiveBattler])
         && gBattleMons[gActiveBattler].hp == 0)
        {
            ewram160ACarr2(0, battlerId) = 0;
            ewram160ACarr2(1, battlerId) = 0;
            ewram16100arr2(0, battlerId) = 0;
            ewram16100arr2(1, battlerId) = 0;
            ewram16100arr2(2, battlerId) = 0;
            ewram16100arr2(3, battlerId) = 0;

            gHitMarker |= HITMARKER_FAINTED(gActiveBattler);
            BattleScriptPush(gBattlescriptCurrInstr + 7);
            gBattlescriptCurrInstr = BS_ptr;
            if (GetBattlerSide(gActiveBattler) == B_SIDE_PLAYER)
            {
                gHitMarker |= HITMARKER_x400000;
                if (gBattleResults.playerFaintCounter < 0xFF)
                    gBattleResults.playerFaintCounter++;
                if (gBattleMons[battlerId].level > gBattleMons[gActiveBattler].level)
                {
                    if (gBattleMons[battlerId].level - gBattleMons[gActiveBattler].level > 29)
                        AdjustFriendship(&gPlayerParty[gBattlerPartyIndexes[gActiveBattler]], FRIENDSHIP_EVENT_FAINT_LARGE);
                    else
                        AdjustFriendship(&gPlayerParty[gBattlerPartyIndexes[gActiveBattler]], FRIENDSHIP_EVENT_FAINT_SMALL);
                }
            }
            else
            {
                if (gBattleResults.opponentFaintCounter < 0xFF)
                    gBattleResults.opponentFaintCounter++;
                gBattleResults.lastOpponentSpecies = gBattleMons[gActiveBattler].species;
            }
            if ((gHitMarker & HITMARKER_DESTINYBOND) && gBattleMons[gBattlerAttacker].hp != 0)
            {
                BattleScriptPush(gBattlescriptCurrInstr);
                gBattleMoveDamage = gBattleMons[battlerId].hp;
                gBattlescriptCurrInstr = BattleScript_DestinyBondTakesLife;
            }
            if ((gStatuses3[gBattlerTarget] & STATUS3_GRUDGE)
             && !(gHitMarker & HITMARKER_GRUDGE)
             && GetBattlerSide(gBattlerAttacker) != GetBattlerSide(gBattlerTarget)
             && gBattleMons[gBattlerAttacker].hp != 0
             && gCurrentMove != MOVE_STRUGGLE)
            {
                u8 moveIndex = ewramChosenMoveIndex(gBattlerAttacker);

                gBattleMons[gBattlerAttacker].pp[moveIndex] = 0;
                BattleScriptPush(gBattlescriptCurrInstr);
                gBattlescriptCurrInstr = BattleScript_GrudgeTakesPp;
                gActiveBattler = gBattlerAttacker;
                BtlController_EmitSetMonData(0, moveIndex + 9, 0, 1, &gBattleMons[gActiveBattler].pp[moveIndex]);
                MarkBattlerForControllerExec(gActiveBattler);

                gBattleTextBuff1[0] = 0xFD;
                gBattleTextBuff1[1] = 2;
                gBattleTextBuff1[2] = gBattleMons[gBattlerAttacker].moves[moveIndex];
                gBattleTextBuff1[3] = gBattleMons[gBattlerAttacker].moves[moveIndex] >> 8;
                gBattleTextBuff1[4] = EOS;
            }
        }
        else
        {
            gBattlescriptCurrInstr += 7;
        }
    }
}

static void atk1A_dofaintanimation(void)
{
    if (gBattleControllerExecFlags == 0)
    {
        gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
        BtlController_EmitFaintAnimation(0);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattlescriptCurrInstr += 2;
    }
}

static void atk1B_cleareffectsonfaint(void)
{
    //Clears things like attraction or trapping to other banks
    if (gBattleControllerExecFlags == 0)
    {
        gActiveBattler = GetBattlerForBattleScript(T2_READ_8(gBattlescriptCurrInstr + 1));
        gBattleMons[gActiveBattler].status1 = 0;
        BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 0x4, &gBattleMons[gActiveBattler].status1);
        MarkBattlerForControllerExec(gActiveBattler);
        UndoEffectsAfterFainting();
        gBattlescriptCurrInstr += 2;
    }
}

static void atk1C_jumpifstatus(void)
{
    u8 bank = GetBattlerForBattleScript(T2_READ_8(gBattlescriptCurrInstr + 1));
    u32 flags = T2_READ_32(gBattlescriptCurrInstr + 2);
    void* jump_loc = T2_READ_PTR(gBattlescriptCurrInstr + 6);
    if (gBattleMons[bank].status1 & flags && gBattleMons[bank].hp)
        gBattlescriptCurrInstr = jump_loc;
    else
        gBattlescriptCurrInstr += 10;
}

static void atk1D_jumpifstatus2(void)
{
    u8 battlerId = GetBattlerForBattleScript(T2_READ_8(gBattlescriptCurrInstr + 1));
    u32 flags = T2_READ_32(gBattlescriptCurrInstr + 2);
    void* jump_loc = T2_READ_PTR(gBattlescriptCurrInstr + 6);
    if (gBattleMons[battlerId].status2 & flags && gBattleMons[battlerId].hp)
        gBattlescriptCurrInstr = jump_loc;
    else
        gBattlescriptCurrInstr += 10;
}

static void atk1E_jumpifability(void)
{
    u8 battlerId;
    u8 ability = gBattlescriptCurrInstr[2];
    void* jumpPtr = T2_READ_PTR(gBattlescriptCurrInstr + 3);
    if (T2_READ_8(gBattlescriptCurrInstr + 1) == 8)
    {
        battlerId = AbilityBattleEffects(ABILITYEFFECT_CHECK_BANK_SIDE, gBattlerAttacker, ability, 0, 0);
        if (battlerId)
        {
            gLastUsedAbility = ability;
            gBattlescriptCurrInstr = jumpPtr;
            RecordAbilityBattle(battlerId -1, gLastUsedAbility);
            ewram160F8 = battlerId - 1;
        }
        else
            gBattlescriptCurrInstr += 7;
    }
    else if (T2_READ_8(gBattlescriptCurrInstr + 1) == 9)
    {
        battlerId = AbilityBattleEffects(ABILITYEFFECT_CHECK_OTHER_SIDE, gBattlerAttacker, ability, 0, 0);
        if (battlerId)
        {
            gLastUsedAbility = ability;
            gBattlescriptCurrInstr = jumpPtr;
            RecordAbilityBattle(battlerId - 1, gLastUsedAbility);
            ewram160F8 = battlerId - 1;
        }
        else
            gBattlescriptCurrInstr += 7;
    }
    else
    {
        battlerId = GetBattlerForBattleScript(T2_READ_8(gBattlescriptCurrInstr + 1));
        if (GetBattlerAbility(battlerId) == ability)
        {
            gLastUsedAbility = ability;
            gBattlescriptCurrInstr = jumpPtr;
            RecordAbilityBattle(battlerId, gLastUsedAbility);
            ewram160F8 = battlerId;
        }
        else
            gBattlescriptCurrInstr += 7;
    }
}

static void atk1F_jumpifsideaffecting(void)
{
    u8 side;
    u16 flags;
    void* jump_loc;
    if (T2_READ_8(gBattlescriptCurrInstr + 1) == 1)
        side = GetBattlerPosition(gBattlerAttacker) & 1;
    else
        side = GetBattlerPosition(gBattlerTarget) & 1;

    flags = T2_READ_16(gBattlescriptCurrInstr + 2);
    jump_loc = T2_READ_PTR(gBattlescriptCurrInstr + 4);

    if (gSideStatuses[side] & flags)
        gBattlescriptCurrInstr = jump_loc;
    else
        gBattlescriptCurrInstr += 8;
}

static void atk20_jumpifstat(void)
{
    u8 ret = 0;
    u8 battlerId = GetBattlerForBattleScript(T2_READ_8(gBattlescriptCurrInstr + 1));
    u8 value = gBattleMons[battlerId].statStages[T2_READ_8(gBattlescriptCurrInstr + 3)];
    switch (T2_READ_8(gBattlescriptCurrInstr + 2))
    {
    case CMP_EQUAL:
        if (value == T2_READ_8(gBattlescriptCurrInstr + 4))
            ret++;
        break;
    case CMP_NOT_EQUAL:
        if (value != T2_READ_8(gBattlescriptCurrInstr + 4))
            ret++;
        break;
    case CMP_GREATER_THAN:
        if (value > T2_READ_8(gBattlescriptCurrInstr + 4))
            ret++;
        break;
    case CMP_LESS_THAN:
        if (value < T2_READ_8(gBattlescriptCurrInstr + 4))
            ret++;
        break;
    case CMP_COMMON_BITS:
        if (value & T2_READ_8(gBattlescriptCurrInstr + 4))
            ret++;
        break;
    case CMP_NO_COMMON_BITS:
        if (!(value & T2_READ_8(gBattlescriptCurrInstr + 4)))
            ret++;
        break;
    }
    if (ret)
        gBattlescriptCurrInstr = T2_READ_PTR(gBattlescriptCurrInstr + 5);
    else
        gBattlescriptCurrInstr += 9;
}

static void atk21_jumpifstatus3condition(void)
{
    u32 flags;
    void* jump_loc;

    gActiveBattler = GetBattlerForBattleScript(T2_READ_8(gBattlescriptCurrInstr + 1));
    flags = T2_READ_32(gBattlescriptCurrInstr + 2);
    jump_loc = T2_READ_PTR(gBattlescriptCurrInstr + 7);
    if (T2_READ_8(gBattlescriptCurrInstr + 6))
    {
        if ((gStatuses3[gActiveBattler] & flags) != 0)
            gBattlescriptCurrInstr += 11;
        else
            gBattlescriptCurrInstr = jump_loc;
    }
    else
    {
        if ((gStatuses3[gActiveBattler] & flags) != 0)
            gBattlescriptCurrInstr = jump_loc;
        else
            gBattlescriptCurrInstr += 11;
    }
}

static void atk22_jumpiftype(void) //u8 battlerId, u8 type, *ptr
{
    u8 battlerId = GetBattlerForBattleScript(T2_READ_8(gBattlescriptCurrInstr + 1));
    u8 type = T2_READ_8(gBattlescriptCurrInstr + 2);
    void* jump_loc = T2_READ_PTR(gBattlescriptCurrInstr + 3);

    if (gBattleMons[battlerId].type1 == type || gBattleMons[battlerId].type2 == type)
        gBattlescriptCurrInstr = jump_loc;
    else
        gBattlescriptCurrInstr += 7;
}

static void atk23_getexp(void)
{
    u16 item;
    s32 i; // also used as stringId
    u8 holdEffect;
    s32 sentIn;

    s32 viaExpShare = 0;
    u16* exp = &gBattleStruct->exp;

    gBank1 = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
    sentIn = gSentPokesToOpponent[(gBank1 & 2) >> 1];

    switch (gBattleStruct->getexpStateTracker)
    {
    case 0: // check if should receive exp at all
        if (GetBattlerSide(gBank1) != B_SIDE_OPPONENT || (gBattleTypeFlags &
             (BATTLE_TYPE_LINK
              | BATTLE_TYPE_SAFARI
              | BATTLE_TYPE_BATTLE_TOWER
              | BATTLE_TYPE_EREADER_TRAINER)))
        {
            gBattleStruct->getexpStateTracker = 6; // goto last case
        }
        else
        {
            gBattleStruct->getexpStateTracker++;
            gBattleStruct->unk16113 |= gBitTable[gBattlerPartyIndexes[gBank1]];
        }
        break;
    case 1: // calculate experience points to redistribute
        {
            u16 calculatedExp;
            s32 viaSentIn;
            

            for (viaSentIn = 0, i = 0; i < 6; i++)
            {
                if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES) == SPECIES_NONE || GetMonData(&gPlayerParty[i], MON_DATA_HP) == 0)
                    continue;
                if (gBitTable[i] & sentIn)
                    viaSentIn++;

                item = GetMonData(&gPlayerParty[i], MON_DATA_HELD_ITEM);

                if (item == ITEM_ENIGMA_BERRY)
                    holdEffect = gSaveBlock1.enigmaBerry.holdEffect;
                else
                    holdEffect = ItemId_GetHoldEffect(item);

//                if (holdEffect == HOLD_EFFECT_EXP_SHARE)
//                    viaExpShare++;
            }

            calculatedExp = gBaseStats[gBattleMons[gBank1].species].expYield * gBattleMons[gBank1].level / 7;

            if (gSaveBlock2.expShare) // exp share is turned on
            {
                *exp = calculatedExp / 2 / viaSentIn;
                if (*exp == 0)
                    *exp = 1;

                viaExpShare = gSaveBlock1.playerPartyCount;
                gExpShareExp = calculatedExp / 2;
                if (gExpShareExp == 0)
                    gExpShareExp = 1;
            }
            else
            {
                *exp = calculatedExp / viaSentIn;
                if (*exp == 0)
                    *exp = 1;
                gExpShareExp = 0;
            }

            gBattleStruct->getexpStateTracker++;
            gBattleStruct->expGetterMonId = 0;
            gBattleStruct->sentInPokes = sentIn;
        }
        // fall through
    case 2: // set exp value to the poke in expgetter_id and print message
        if (gBattleControllerExecFlags == 0)
        {
            item = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_HELD_ITEM);

            if (item == ITEM_ENIGMA_BERRY)
                holdEffect = gSaveBlock1.enigmaBerry.holdEffect;
            else
                holdEffect = ItemId_GetHoldEffect(item);

            if (!gSaveBlock2.expShare && !(gBattleStruct->sentInPokes & 1))
            {
                gBattleStruct->sentInPokes >>= 1;
                gBattleStruct->getexpStateTracker = 5;
                gBattleMoveDamage = 0; // used for exp
            }
            else if (GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_LEVEL) == 100)
            {
                gBattleStruct->sentInPokes >>= 1;
                gBattleStruct->getexpStateTracker = 5;
                gBattleMoveDamage = 0; // used for exp
            }
            else
            {
                // music change in wild battle after fainting a poke
                if (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER) && gBattleMons[0].hp && !gBattleStruct->wildVictorySong)
                {
                    BattleStopLowHpSound();
                    PlayBGM(MUS_KACHI2);
                    gBattleStruct->wildVictorySong++;
                }

                if (GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_HP) && !GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_IS_EGG))
                {
                    if (gBattleStruct->sentInPokes & 1)
                        gBattleMoveDamage = *exp;
                    else
                        gBattleMoveDamage = 0;

                    if (gSaveBlock2.expShare)
                        gBattleMoveDamage += gExpShareExp;
                    if (holdEffect == HOLD_EFFECT_LUCKY_EGG)
                        gBattleMoveDamage = (gBattleMoveDamage * 150) / 100;
                    if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
                        gBattleMoveDamage = (gBattleMoveDamage * 150) / 100;

                    if (IsTradedMon(&gPlayerParty[gBattleStruct->expGetterMonId]))
                    {
                        gBattleMoveDamage = (gBattleMoveDamage * 150) / 100;
                        i = STRINGID_EMPTYSTRING4;
                    }
                    else
                    {
                        i = STRINGID_ABOOSTED;
                    }

                    // get exp getter bank
                    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
                    {
                        if (!(gBattlerPartyIndexes[2] != gBattleStruct->expGetterMonId) && !(gAbsentBattlerFlags & gBitTable[2]))
                            gBattleStruct->expGetterBattlerId = 2;
                        else
                        {
                            if (!(gAbsentBattlerFlags & gBitTable[0]))
                                gBattleStruct->expGetterBattlerId = 0;
                            else
                                gBattleStruct->expGetterBattlerId = 2;
                        }
                    }
                    else
                        gBattleStruct->expGetterBattlerId = 0;

                    PREPARE_MON_NICK_WITH_PREFIX_BUFFER(gBattleTextBuff1, gBattleStruct->expGetterBattlerId, gBattleStruct->expGetterMonId)

                    // buffer 'gained' or 'gained a boosted'
                    PREPARE_STRING_BUFFER(gBattleTextBuff2, i)

                    PREPARE_WORD_NUMBER_BUFFER(gBattleTextBuff3, 5, gBattleMoveDamage)

                    PrepareStringBattle(STRINGID_PKMNGAINEDEXP, gBattleStruct->expGetterBattlerId);
                    MonGainEVs(&gPlayerParty[gBattleStruct->expGetterMonId], gBattleMons[gBank1].species);
                }
                gBattleStruct->sentInPokes >>= 1;
                gBattleStruct->getexpStateTracker++;
            }
        }
        break;
    case 3: // Set stats and give exp
        if (gBattleControllerExecFlags == 0)
        {
            gBattleBufferB[gBattleStruct->expGetterBattlerId][0] = 0;
            if (GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_HP) && GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_LEVEL) != 100)
            {
                gBattleResources_statsBeforeLvlUp->hp = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_MAX_HP);
                gBattleResources_statsBeforeLvlUp->atk = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_ATK);
                gBattleResources_statsBeforeLvlUp->def = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_DEF);
                gBattleResources_statsBeforeLvlUp->spd = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_SPEED);
                gBattleResources_statsBeforeLvlUp->spAtk = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_SPATK);
                gBattleResources_statsBeforeLvlUp->spDef = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_SPDEF);

                gActiveBattler = gBattleStruct->expGetterBattlerId;

                BtlController_EmitExpUpdate(0, gBattleStruct->expGetterMonId, gBattleMoveDamage);
                MarkBattlerForControllerExec(gActiveBattler);
            }
            gBattleStruct->getexpStateTracker++;
        }
        break;
    case 4: // lvl up if necessary
        if (gBattleControllerExecFlags == 0)
        {
            gActiveBattler = gBattleStruct->expGetterBattlerId;
            if (gBattleBufferB[gActiveBattler][0] == CONTROLLER_TWORETURNVALUES
             && gBattleBufferB[gActiveBattler][1] == RET_VALUE_LEVELLED_UP)
            {
                u16 bank = 0xFF, temp;
                if (gBattleTypeFlags & BATTLE_TYPE_TRAINER && gBattlerPartyIndexes[gActiveBattler] == gBattleStruct->expGetterMonId)
                    HandleLowHpMusicChange(&gPlayerParty[gBattlerPartyIndexes[gActiveBattler]], gActiveBattler);

                PREPARE_MON_NICK_WITH_PREFIX_BUFFER(gBattleTextBuff1, gActiveBattler, gBattleStruct->expGetterMonId)

                PREPARE_BYTE_NUMBER_BUFFER(gBattleTextBuff2, 3, GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_LEVEL))

                BattleScriptPushCursor();
                gLeveledUpInBattle |= gBitTable[gBattleStruct->expGetterMonId];
                gBattlescriptCurrInstr = BattleScript_LevelUp;
                gBattleMoveDamage = (gBattleBufferB[gActiveBattler][2] | (gBattleBufferB[gActiveBattler][3] << 8));
                AdjustFriendship(&gPlayerParty[gBattleStruct->expGetterMonId], FRIENDSHIP_EVENT_GROW_LEVEL);

                if (gBattlerPartyIndexes[0] == gBattleStruct->expGetterMonId && gBattleMons[0].hp)
                    bank = 0;
                else if (gBattlerPartyIndexes[2] == gBattleStruct->expGetterMonId && gBattleMons[2].hp && (gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
                    bank = 2;

                // update battle mon structure after level up
                if (bank != 0xFF)
                {
                    gBattleMons[bank].level = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_LEVEL);
                    gBattleMons[bank].hp = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_HP);
                    gBattleMons[bank].maxHP = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_MAX_HP);
                    gBattleMons[bank].attack = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_ATK);
                    gBattleMons[bank].defense = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_DEF);
                    gBattleMons[bank].speed = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_SPEED);
                    gBattleMons[bank].spAttack = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_SPATK);
                    gBattleMons[bank].spDefense = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_SPDEF);
                
                    if (gStatuses3[bank] & STATUS3_POWER_TRICK)
                    {
                        temp = gBattleMons[bank].attack;
                        gBattleMons[bank].attack = gBattleMons[bank].defense;
                        gBattleMons[bank].defense = temp;
                    }
                }
            }
            else
            {
                gBattleMoveDamage = 0;
            }
            gBattleStruct->getexpStateTracker = 5;
        }
        break;
    case 5: // looper increment
        if (gBattleMoveDamage) // there is exp to give, goto case 3 that gives exp
            gBattleStruct->getexpStateTracker = 3;
        else
        {
            gBattleStruct->expGetterMonId++;
            if (gBattleStruct->expGetterMonId <= 5)
                gBattleStruct->getexpStateTracker = 2; // loop again
            else
                gBattleStruct->getexpStateTracker = 6; // we're done
        }
        break;
    case 6: // increment instruction
        if (gBattleControllerExecFlags == 0)
        {
            // not sure why gf clears the item and ability here
            gBattleMons[gBank1].item = 0;
            gBattleMons[gBank1].ability = 0;
            gBattlescriptCurrInstr += 2;
        }
        break;
    }
}

static bool32 NoAliveMonsForPlayer(void)
{
    u32 i;
    u32 HP_count = 0;

    if (gBattleTypeFlags & BATTLE_TYPE_MULTI // && (gPartnerTrainerId == TRAINER_STEVEN_PARTNER || gPartnerTrainerId >= TRAINER_CUSTOM_PARTNER)
        ) // we will figure out the whole multi battle stuff in a hot minute
    {
        for (i = 0; i < 3; i++)
        {
            if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES) && !GetMonData(&gPlayerParty[i], MON_DATA_IS_EGG))
                HP_count += GetMonData(&gPlayerParty[i], MON_DATA_HP);
        }
    }
    else
    {
        for (i = 0; i < PARTY_SIZE; i++)
        {
            if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES) && !GetMonData(&gPlayerParty[i], MON_DATA_IS_EGG))
            {
                HP_count += GetMonData(&gPlayerParty[i], MON_DATA_HP);
            }
        }
    }

    return (HP_count == 0);
}

static bool32 NoAliveMonsForOpponent(void)
{
    u32 i;
    u32 HP_count = 0;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (GetMonData(&gEnemyParty[i], MON_DATA_SPECIES) && !GetMonData(&gEnemyParty[i], MON_DATA_IS_EGG))
        {
            HP_count += GetMonData(&gEnemyParty[i], MON_DATA_HP);
        }
    }

    return (HP_count == 0);
}

bool32 NoAliveMonsForEitherParty(void)
{
    return (NoAliveMonsForPlayer() || NoAliveMonsForOpponent());
}

#ifdef NONMATCHING
static void atk24(void)
{
    u16 HP_count = 0;
    int i;
    if (gBattleControllerExecFlags) {return;}

    for (i = 0; i < 6; i++)
    {
        if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES) && !GetMonData(&gPlayerParty[i], MON_DATA_IS_EGG))
            HP_count += GetMonData(&gPlayerParty[i], MON_DATA_HP);
    }

    if (HP_count == 0)
        gBattleOutcome |= B_OUTCOME_LOST;

    for (HP_count = 0, i = 0; i < 6; i++)
    {
        if (GetMonData(&gEnemyParty[i], MON_DATA_SPECIES) && !GetMonData(&gEnemyParty[i], MON_DATA_IS_EGG))
            HP_count += GetMonData(&gEnemyParty[i], MON_DATA_HP);
    }

    if (!HP_count)
        gBattleOutcome |= B_OUTCOME_WON;

    if (!gBattleOutcome && (gBattleTypeFlags & BATTLE_TYPE_LINK))
    {
        register int found1 asm("r2");
        register int found2 asm("r4");

        //I can't for the love of god decompile that part

        for (found1 = 0, i = 0; i < gBattlersCount; i += 2)
        {
            if ((gHitMarker & HITMARKER_UNK(i)) && !gSpecialStatuses[i].flag40)
                found1++;
        }

        for (found2 = 0, i = 1; i < gBattlersCount; i += 2)
        {
            if ((gHitMarker & HITMARKER_UNK(i)) && !gSpecialStatuses[i].flag40)
                found2++;
        }

        if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
        {
            if (found2 + found1 > 1)
                gBattlescriptCurrInstr = T2_READ_PTR(gBattlescriptCurrInstr + 1);
            else
                gBattlescriptCurrInstr += 5;
        }
        else
        {
            if (found2 != 0 && found1 != 0)
                gBattlescriptCurrInstr = T2_READ_PTR(gBattlescriptCurrInstr + 1);
            else
                gBattlescriptCurrInstr += 5;
        }
    }
    else
        gBattlescriptCurrInstr += 5;

}
#else
NAKED
static void atk24(void)
{
    asm(".syntax unified\n\
    push {r4-r7,lr}\n\
    mov r7, r8\n\
    push {r7}\n\
    movs r6, 0\n\
    ldr r0, _08020AF0 @ =gBattleControllerExecFlags\n\
    ldr r0, [r0]\n\
    cmp r0, 0\n\
    beq _080209C6\n\
    b _08020B46\n\
_080209C6:\n\
    movs r5, 0\n\
_080209C8:\n\
    movs r0, 0x64\n\
    adds r1, r5, 0\n\
    muls r1, r0\n\
    ldr r0, _08020AF4 @ =gPlayerParty\n\
    adds r4, r1, r0\n\
    adds r0, r4, 0\n\
    movs r1, 0xB\n\
    bl GetMonData\n\
    cmp r0, 0\n\
    beq _080209F8\n\
    adds r0, r4, 0\n\
    movs r1, 0x2D\n\
    bl GetMonData\n\
    cmp r0, 0\n\
    bne _080209F8\n\
    adds r0, r4, 0\n\
    movs r1, 0x39\n\
    bl GetMonData\n\
    adds r0, r6, r0\n\
    lsls r0, 16\n\
    lsrs r6, r0, 16\n\
_080209F8:\n\
    adds r5, 0x1\n\
    cmp r5, 0x5\n\
    ble _080209C8\n\
    cmp r6, 0\n\
    bne _08020A0C\n\
    ldr r0, _08020AF8 @ =gBattleOutcome\n\
    ldrb r1, [r0]\n\
    movs r2, 0x2\n\
    orrs r1, r2\n\
    strb r1, [r0]\n\
_08020A0C:\n\
    movs r6, 0\n\
    movs r5, 0\n\
_08020A10:\n\
    movs r0, 0x64\n\
    adds r1, r5, 0\n\
    muls r1, r0\n\
    ldr r0, _08020AFC @ =gEnemyParty\n\
    adds r4, r1, r0\n\
    adds r0, r4, 0\n\
    movs r1, 0xB\n\
    bl GetMonData\n\
    cmp r0, 0\n\
    beq _08020A40\n\
    adds r0, r4, 0\n\
    movs r1, 0x2D\n\
    bl GetMonData\n\
    cmp r0, 0\n\
    bne _08020A40\n\
    adds r0, r4, 0\n\
    movs r1, 0x39\n\
    bl GetMonData\n\
    adds r0, r6, r0\n\
    lsls r0, 16\n\
    lsrs r6, r0, 16\n\
_08020A40:\n\
    adds r5, 0x1\n\
    cmp r5, 0x5\n\
    ble _08020A10\n\
    ldr r2, _08020AF8 @ =gBattleOutcome\n\
    cmp r6, 0\n\
    bne _08020A54\n\
    ldrb r0, [r2]\n\
    movs r1, 0x1\n\
    orrs r0, r1\n\
    strb r0, [r2]\n\
_08020A54:\n\
    ldrb r0, [r2]\n\
    cmp r0, 0\n\
    bne _08020B3E\n\
    ldr r2, _08020B00 @ =gBattleTypeFlags\n\
    ldrh r1, [r2]\n\
    movs r0, 0x2\n\
    ands r0, r1\n\
    mov r8, r2\n\
    cmp r0, 0\n\
    beq _08020B3E\n\
    movs r2, 0\n\
    movs r5, 0\n\
    ldr r0, _08020B04 @ =gBattlersCount\n\
    ldrb r3, [r0]\n\
    mov r12, r0\n\
    ldr r7, _08020B08 @ =gBattlescriptCurrInstr\n\
    cmp r2, r3\n\
    bge _08020AA0\n\
    ldr r0, _08020B0C @ =gHitMarker\n\
    movs r1, 0x80\n\
    lsls r1, 21\n\
    ldr r6, [r0]\n\
    adds r4, r3, 0\n\
    ldr r3, _08020B10 @ =gSpecialStatuses\n\
_08020A84:\n\
    adds r0, r1, 0\n\
    lsls r0, r5\n\
    ands r0, r6\n\
    cmp r0, 0\n\
    beq _08020A98\n\
    ldrb r0, [r3]\n\
    lsls r0, 25\n\
    cmp r0, 0\n\
    blt _08020A98\n\
    adds r2, 0x1\n\
_08020A98:\n\
    adds r3, 0x28\n\
    adds r5, 0x2\n\
    cmp r5, r4\n\
    blt _08020A84\n\
_08020AA0:\n\
    movs r4, 0\n\
    movs r5, 0x1\n\
    mov r0, r12\n\
    ldrb r3, [r0]\n\
    cmp r5, r3\n\
    bge _08020ADA\n\
    ldr r0, _08020B0C @ =gHitMarker\n\
    movs r1, 0x80\n\
    lsls r1, 21\n\
    mov r12, r1\n\
    ldr r1, [r0]\n\
    ldr r0, _08020B10 @ =gSpecialStatuses\n\
    adds r6, r3, 0\n\
    adds r3, r0, 0\n\
    adds r3, 0x14\n\
_08020ABE:\n\
    mov r0, r12\n\
    lsls r0, r5\n\
    ands r0, r1\n\
    cmp r0, 0\n\
    beq _08020AD2\n\
    ldrb r0, [r3]\n\
    lsls r0, 25\n\
    cmp r0, 0\n\
    blt _08020AD2\n\
    adds r4, 0x1\n\
_08020AD2:\n\
    adds r3, 0x28\n\
    adds r5, 0x2\n\
    cmp r5, r6\n\
    blt _08020ABE\n\
_08020ADA:\n\
    mov r0, r8\n\
    ldrh r1, [r0]\n\
    movs r0, 0x40\n\
    ands r0, r1\n\
    cmp r0, 0\n\
    beq _08020B14\n\
    adds r0, r4, r2\n\
    cmp r0, 0x1\n\
    bgt _08020B1C\n\
    b _08020B36\n\
    .align 2, 0\n\
_08020AF0: .4byte gBattleControllerExecFlags\n\
_08020AF4: .4byte gPlayerParty\n\
_08020AF8: .4byte gBattleOutcome\n\
_08020AFC: .4byte gEnemyParty\n\
_08020B00: .4byte gBattleTypeFlags\n\
_08020B04: .4byte gBattlersCount\n\
_08020B08: .4byte gBattlescriptCurrInstr\n\
_08020B0C: .4byte gHitMarker\n\
_08020B10: .4byte gSpecialStatuses\n\
_08020B14:\n\
    cmp r4, 0\n\
    beq _08020B36\n\
    cmp r2, 0\n\
    beq _08020B36\n\
_08020B1C:\n\
    ldr r2, [r7]\n\
    ldrb r1, [r2, 0x1]\n\
    ldrb r0, [r2, 0x2]\n\
    lsls r0, 8\n\
    adds r1, r0\n\
    ldrb r0, [r2, 0x3]\n\
    lsls r0, 16\n\
    adds r1, r0\n\
    ldrb r0, [r2, 0x4]\n\
    lsls r0, 24\n\
    adds r1, r0\n\
    str r1, [r7]\n\
    b _08020B46\n\
_08020B36:\n\
    ldr r0, [r7]\n\
    adds r0, 0x5\n\
    str r0, [r7]\n\
    b _08020B46\n\
_08020B3E:\n\
    ldr r1, _08020B50 @ =gBattlescriptCurrInstr\n\
    ldr r0, [r1]\n\
    adds r0, 0x5\n\
    str r0, [r1]\n\
_08020B46:\n\
    pop {r3}\n\
    mov r8, r3\n\
    pop {r4-r7}\n\
    pop {r0}\n\
    bx r0\n\
    .align 2, 0\n\
_08020B50: .4byte gBattlescriptCurrInstr\n\
        .syntax divided\n");
}
#endif // NONMATCHING

static void MoveValuesCleanUp(void)
{
    gMoveResultFlags = 0;
    gBattleStruct->dmgMultiplier = 1;
    gCritMultiplier = 1;
    gBattleCommunication[MOVE_EFFECT_BYTE] = 0;
    gBattleCommunication[6] = 0;
    gHitMarker &= ~(HITMARKER_DESTINYBOND);
    gHitMarker &= ~(HITMARKER_SYNCHRONISE_EFFECT);
}

static void atk25_movevaluescleanup(void)
{
    MoveValuesCleanUp();
    gBattlescriptCurrInstr += 1;
}

static void atk26_setmultihit(void)
{
    gMultiHitCounter = T2_READ_8(gBattlescriptCurrInstr + 1);
    gBattlescriptCurrInstr += 2;
}

static void atk27_decrementmultihit(void)
{
    if (--gMultiHitCounter == 0)
        gBattlescriptCurrInstr += 5;
    else
        gBattlescriptCurrInstr = T2_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atk28_goto(void)
{
    gBattlescriptCurrInstr = T2_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atk29_jumpifbyte(void)
{
    u8 caseID = T2_READ_8(gBattlescriptCurrInstr + 1);
    u8* ptr = T2_READ_PTR(gBattlescriptCurrInstr + 2);
    u8 value = T2_READ_8(gBattlescriptCurrInstr + 6);
    u8* jump_loc = T2_READ_PTR(gBattlescriptCurrInstr + 7);
    gBattlescriptCurrInstr += 11;
    switch (caseID)
    {
    case CMP_EQUAL:
        if (*ptr == value)
            gBattlescriptCurrInstr = jump_loc;
        break;
    case CMP_NOT_EQUAL:
        if (*ptr != value)
            gBattlescriptCurrInstr = jump_loc;
        break;
    case CMP_GREATER_THAN:
        if (*ptr > value)
            gBattlescriptCurrInstr = jump_loc;
        break;
    case CMP_LESS_THAN:
        if (*ptr < value)
            gBattlescriptCurrInstr = jump_loc;
        break;
    case CMP_COMMON_BITS:
        if (*ptr & value)
            gBattlescriptCurrInstr = jump_loc;
        break;
    case CMP_NO_COMMON_BITS:
        if (!(*ptr & value))
            gBattlescriptCurrInstr = jump_loc;
        break;
    }
}

static void atk2A_jumpifhalfword(void)
{
    u8 caseID = T2_READ_8(gBattlescriptCurrInstr + 1);
    u16* ptr = T2_READ_PTR(gBattlescriptCurrInstr + 2);
    u16 value = T2_READ_16(gBattlescriptCurrInstr + 6);
    u8* jump_loc = T2_READ_PTR(gBattlescriptCurrInstr + 8);
    gBattlescriptCurrInstr += 12;
    switch (caseID)
    {
    case CMP_EQUAL:
        if (*ptr == value)
            gBattlescriptCurrInstr = jump_loc;
        break;
    case CMP_NOT_EQUAL:
        if (*ptr != value)
            gBattlescriptCurrInstr = jump_loc;
        break;
    case CMP_GREATER_THAN:
        if (*ptr > value)
            gBattlescriptCurrInstr = jump_loc;
        break;
    case CMP_LESS_THAN:
        if (*ptr < value)
            gBattlescriptCurrInstr = jump_loc;
        break;
    case CMP_COMMON_BITS:
        if (*ptr & value)
            gBattlescriptCurrInstr = jump_loc;
        break;
    case CMP_NO_COMMON_BITS:
        if (!(*ptr & value))
            gBattlescriptCurrInstr = jump_loc;
        break;
    }
}

// Strange that there's an instance of T1_READ_32 in what seems to be a T2 function. see global.h for the distinction.
static void atk2B_jumpifword(void)
{
    u8 caseID = T2_READ_8(gBattlescriptCurrInstr + 1);
    u32* ptr = T2_READ_PTR(gBattlescriptCurrInstr + 2);
    u32 value = T1_READ_32(gBattlescriptCurrInstr + 6);
    u8* jump_loc = T2_READ_PTR(gBattlescriptCurrInstr + 10);
    gBattlescriptCurrInstr += 14;
    switch (caseID)
    {
    case CMP_EQUAL:
        if (*ptr == value)
            gBattlescriptCurrInstr = jump_loc;
        break;
    case CMP_NOT_EQUAL:
        if (*ptr != value)
            gBattlescriptCurrInstr = jump_loc;
        break;
    case CMP_GREATER_THAN:
        if (*ptr > value)
            gBattlescriptCurrInstr = jump_loc;
        break;
    case CMP_LESS_THAN:
        if (*ptr < value)
            gBattlescriptCurrInstr = jump_loc;
        break;
    case CMP_COMMON_BITS:
        if (*ptr & value)
            gBattlescriptCurrInstr = jump_loc;
        break;
    case CMP_NO_COMMON_BITS:
        if (!(*ptr & value))
            gBattlescriptCurrInstr = jump_loc;
        break;
    }
}

static void atk2C_jumpifarrayequal(void)
{
    //Mem1, Mem2, Size, Jump Loc
    u8* mem1 = T2_READ_PTR(gBattlescriptCurrInstr + 1);
    u8* mem2 = T2_READ_PTR(gBattlescriptCurrInstr + 5);
    u32 size = T2_READ_8(gBattlescriptCurrInstr + 9);
    u8* jump_loc = T2_READ_PTR(gBattlescriptCurrInstr + 10);

    u8 i;
    for (i = 0; i < size; i++)
    {
        if (*mem1 != *mem2)
        {
            gBattlescriptCurrInstr += 14;
            break;
        }
        mem1++, mem2++;
    }

    if (i == size)
        gBattlescriptCurrInstr = jump_loc;
}

static void atk2D_jumpifarraynotequal(void)
{
    //Mem1, Mem2, Size, Jump Loc
    u8 equal_bytes = 0;
    u8* mem1 = T2_READ_PTR(gBattlescriptCurrInstr + 1);
    u8* mem2 = T2_READ_PTR(gBattlescriptCurrInstr + 5);
    u32 size = T2_READ_8(gBattlescriptCurrInstr + 9);
    u8* jump_loc = T2_READ_PTR(gBattlescriptCurrInstr + 10);

    u8 i;
    for (i = 0; i < size; i++)
    {
        if (*mem1 == *mem2)
        {
            equal_bytes++;
        }
        mem1++, mem2++;
    }

    if (equal_bytes != size)
        gBattlescriptCurrInstr = jump_loc;
    else
        gBattlescriptCurrInstr += 14;
}

static void atk2E_setbyte(void)
{
    u8* mem = T2_READ_PTR(gBattlescriptCurrInstr + 1);
    *mem = T2_READ_8(gBattlescriptCurrInstr + 5);

    gBattlescriptCurrInstr += 6;
}

static void atk2F_addbyte(void)
{
    u8* mem = T2_READ_PTR(gBattlescriptCurrInstr + 1);
    *mem += T2_READ_8(gBattlescriptCurrInstr + 5);
    gBattlescriptCurrInstr += 6;
}

static void atk30_subbyte(void)
{
    u8* mem = T2_READ_PTR(gBattlescriptCurrInstr + 1);
    *mem -= T2_READ_8(gBattlescriptCurrInstr + 5);
    gBattlescriptCurrInstr += 6;
}

static void atk31_copyarray(void)
{
    u8* mem1 = T2_READ_PTR(gBattlescriptCurrInstr + 1);
    u8* mem2 = T2_READ_PTR(gBattlescriptCurrInstr + 5);
    s32 size = T2_READ_8(gBattlescriptCurrInstr + 9);

    s32 i;
    for (i = 0; i < size; i++)
    {
        mem1[i] = mem2[i];
    }

    gBattlescriptCurrInstr += 10;
}

static void atk32_copyarraywithindex(void)
{
    u8* mem1 = T2_READ_PTR(gBattlescriptCurrInstr + 1);
    u8* mem2 = T2_READ_PTR(gBattlescriptCurrInstr + 5);
    u8* index = T2_READ_PTR(gBattlescriptCurrInstr + 9);
    s32 size = T2_READ_8(gBattlescriptCurrInstr + 13);

    s32 i;
    for (i = 0; i < size; i++)
    {
        mem1[i] = mem2[i + *index];
    }

    gBattlescriptCurrInstr += 14;
}

static void atk33_orbyte(void)
{
    u8* mem = T2_READ_PTR(gBattlescriptCurrInstr + 1);
    *mem |= T2_READ_8(gBattlescriptCurrInstr + 5);
    gBattlescriptCurrInstr += 6;
}

static void atk34_orhalfword(void)
{
    u16* mem = T2_READ_PTR(gBattlescriptCurrInstr + 1);
    u16 val = T2_READ_16(gBattlescriptCurrInstr + 5);

    *mem |= val;
    gBattlescriptCurrInstr += 7;
}

static void atk35_orword(void)
{
    u32* mem = T2_READ_PTR(gBattlescriptCurrInstr + 1);
    u32 val = T2_READ_32(gBattlescriptCurrInstr + 5);

    *mem |= val;
    gBattlescriptCurrInstr += 9;
}

static void atk36_bicbyte(void)
{
    u8* mem = T2_READ_PTR(gBattlescriptCurrInstr + 1);
    *mem &= ~(T2_READ_8(gBattlescriptCurrInstr + 5));
    gBattlescriptCurrInstr += 6;
}

static void atk37_bichalfword(void)
{
    u16* mem = T2_READ_PTR(gBattlescriptCurrInstr + 1);
    u16 val = T2_READ_16(gBattlescriptCurrInstr + 5);

    *mem &= ~val;
    gBattlescriptCurrInstr += 7;
}

static void atk38_bicword(void)
{
    u32* mem = T2_READ_PTR(gBattlescriptCurrInstr + 1);
    u32 val = T2_READ_32(gBattlescriptCurrInstr + 5);

    *mem &= ~val;
    gBattlescriptCurrInstr += 9;
}

static void atk39_pause(void)
{
    if (gBattleControllerExecFlags == 0)
    {
        u16 value = T2_READ_16(gBattlescriptCurrInstr + 1);
        if (++gPauseCounterBattle >= value)
        {
            gPauseCounterBattle = 0;
            gBattlescriptCurrInstr += 3;
        }
    }
}

static void atk3A_waitstate(void)
{
    if (gBattleControllerExecFlags == 0)
        gBattlescriptCurrInstr++;
}

static void atk3B_healthbar_update(void)
{
    if (!T2_READ_8(gBattlescriptCurrInstr + 1))
        gActiveBattler = gBattlerTarget;
    else
        gActiveBattler = gBattlerAttacker;

    BtlController_EmitHealthBarUpdate(0, gBattleMoveDamage);
    MarkBattlerForControllerExec(gActiveBattler);
    gBattlescriptCurrInstr += 2;
}

static void atk3C_return(void)
{
    BattleScriptPop();
}

static void atk3D_end(void)
{
    gMoveResultFlags = 0;
    gActiveBattler = 0;
    gCurrentActionFuncId = 0xB;
}

static void atk3E_end2(void)
{
    //not much difference between this and 3D. It's more apparent in Emerald
    gActiveBattler = 0;
    gCurrentActionFuncId = 0xB;
}

static void atk3F_end3(void) //pops the main function stack
{
    BattleScriptPop();
    if (B_FUNCTION_STACK->size)
        B_FUNCTION_STACK->size--;
    gBattleMainFunc = B_FUNCTION_STACK->ptr[B_FUNCTION_STACK->size];
}

static void atk41_call(void)
{
    BattleScriptPush(gBattlescriptCurrInstr + 5);
    gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atk42_jumpiftype2(void) //u8 bank, u8 type, *ptr
{
    u8 bank = GetBattlerForBattleScript(T1_READ_8(gBattlescriptCurrInstr + 1));

    if (T1_READ_8(gBattlescriptCurrInstr + 2) == gBattleMons[bank].type1 || T1_READ_8(gBattlescriptCurrInstr + 2) == gBattleMons[bank].type2)
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 3);
    else
        gBattlescriptCurrInstr += 7;
}

static void atk43_jumpifabilitypresent(void)
{
    if (AbilityBattleEffects(ABILITYEFFECT_CHECK_ON_FIELD, 0, T2_READ_8(gBattlescriptCurrInstr + 1), 0, 0))
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
    else
        gBattlescriptCurrInstr += 6;
}

static void atk44_endselectionscript(void)
{
    ewram16060(gBattlerAttacker) = 1;
}

static void atk45_playanimation(void)
{
    const u16* argumentPtr;

    gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
    argumentPtr = BS2ScriptReadPtr(gBattlescriptCurrInstr + 3);

    if (gBattlescriptCurrInstr[2] == B_ANIM_STATS_CHANGE
        || gBattlescriptCurrInstr[2] == B_ANIM_SNATCH_MOVE
        || gBattlescriptCurrInstr[2] == B_ANIM_SUBSTITUTE_FADE)
    {
        BtlController_EmitBattleAnimation(0, gBattlescriptCurrInstr[2], *argumentPtr);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattlescriptCurrInstr += 7;
    }
    else if (gHitMarker & HITMARKER_NO_ANIMATIONS)
    {
        BattleScriptPush(gBattlescriptCurrInstr + 7);
        gBattlescriptCurrInstr = BattleScript_Pausex20;
    }
    else if (gBattlescriptCurrInstr[2] == B_ANIM_RAIN_CONTINUES
             || gBattlescriptCurrInstr[2] == B_ANIM_SUN_CONTINUES
             || gBattlescriptCurrInstr[2] == B_ANIM_SANDSTORM_CONTINUES
             || gBattlescriptCurrInstr[2] == B_ANIM_HAIL_CONTINUES
             || gBattlescriptCurrInstr[2] == B_ANIM_FOG_IS_DEEP)
    {
        BtlController_EmitBattleAnimation(0, gBattlescriptCurrInstr[2], *argumentPtr);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattlescriptCurrInstr += 7;
    }
    else if (gStatuses3[gActiveBattler] & STATUS3_SEMI_INVULNERABLE)
    {
        gBattlescriptCurrInstr += 7;
    }
    else
    {
        BtlController_EmitBattleAnimation(0, gBattlescriptCurrInstr[2], *argumentPtr);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattlescriptCurrInstr += 7;
    }
}

static void atk46_playanimation2(void) // animation Id is stored in the first pointer
{
    const u16* argumentPtr;
    const u8* animationIdPtr;

    gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
    animationIdPtr = BS2ScriptReadPtr(gBattlescriptCurrInstr + 2);
    argumentPtr = BS2ScriptReadPtr(gBattlescriptCurrInstr + 6);

    if (*animationIdPtr == B_ANIM_STATS_CHANGE
        || *animationIdPtr == B_ANIM_SNATCH_MOVE
        || *animationIdPtr == B_ANIM_SUBSTITUTE_FADE)
    {
        BtlController_EmitBattleAnimation(0, *animationIdPtr, *argumentPtr);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattlescriptCurrInstr += 10;
    }
    else if (gHitMarker & HITMARKER_NO_ANIMATIONS)
    {
        gBattlescriptCurrInstr += 10;
    }
    else if (*animationIdPtr == B_ANIM_RAIN_CONTINUES
             || *animationIdPtr == B_ANIM_SUN_CONTINUES
             || *animationIdPtr == B_ANIM_SANDSTORM_CONTINUES
             || *animationIdPtr == B_ANIM_HAIL_CONTINUES
             || *animationIdPtr == B_ANIM_FOG_IS_DEEP)
    {
        BtlController_EmitBattleAnimation(0, *animationIdPtr, *argumentPtr);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattlescriptCurrInstr += 10;
    }
    else if (gStatuses3[gActiveBattler] & STATUS3_SEMI_INVULNERABLE)
    {
        gBattlescriptCurrInstr += 10;
    }
    else
    {
        BtlController_EmitBattleAnimation(0, *animationIdPtr, *argumentPtr);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattlescriptCurrInstr += 10;
    }
}

static void atk47_setgraphicalstatchangevalues(void)
{
    u8 to_add = 0;
    switch (gBattleStruct->statChanger & 0xF0)
    {
    case 0x10: //+1
        to_add = 0xF;
        break;
    case 0x20: //+2
        to_add = 0x27;
        break;
    case 0x90: //-1
        to_add = 0x16;
        break;
    case 0xA0: //-2
        to_add = 0x2E;
        break;
    }
    gBattleStruct->animArg1 = (gBattleStruct->statChanger & 0xF) + to_add - 1;
    gBattleStruct->animArg2 = 0;
    gBattlescriptCurrInstr++;
}

#define STAT_CHANGE_NEGATIVE         0x1
#define STAT_CHANGE_BY_TWO           0x2
#define STAT_CHANGE_ONLY_MULTIPLE    0x4
#define STAT_CHANGE_CANT_PREVENT 	 0x8

static void atk48_playstatchangeanimation(void)
{
    int curr_stat = 0;
    u16 stat_animID = 0;
    int changeable_stats = 0;
    u16 ability;
    u32 stats_to_check;
    u8 flags;

    gActiveBattler = GetBattlerForBattleScript(T2_READ_8(gBattlescriptCurrInstr + 1));
    ability = GetBattlerAbility(gActiveBattler);
    stats_to_check = T2_READ_8(gBattlescriptCurrInstr + 2);
    flags = T2_READ_8(gBattlescriptCurrInstr + 3);

    if (ability == ABILITY_CONTRARY)
        flags ^= STAT_CHANGE_NEGATIVE;
    else if (ability == ABILITY_SIMPLE)
        flags |= STAT_CHANGE_BY_TWO;

    if (flags & STAT_CHANGE_NEGATIVE)
    {
        u16 r1 = 0x15;
        if (flags & STAT_CHANGE_BY_TWO)
            r1 = 0x2D;
        while (stats_to_check != 0)
        {
            if (!(stats_to_check & 1))
            {
                if (flags & STAT_CHANGE_CANT_PREVENT)
                {
                    if (gBattleMons[gActiveBattler].statStages[curr_stat] > 0)
                    {
                        stat_animID = r1 + curr_stat;
                        changeable_stats++;
                    }
                }
                else if (!gSideTimers[GET_BATTLER_SIDE(gActiveBattler)].mistTimer
                      && ability != ABILITY_CLEAR_BODY
                      && ability != ABILITY_WHITE_SMOKE
                      && !(ability == ABILITY_KEEN_EYE && curr_stat == STAT_ACC)
                      && !(ability == ABILITY_HYPER_CUTTER && curr_stat == STAT_ATK)
                      && !(ability == ABILITY_BIG_PECKS && curr_stat == STAT_DEF))
                {
                    if (gBattleMons[gActiveBattler].statStages[curr_stat] > 0)
                    {
                        stat_animID = r1 + curr_stat;
                        changeable_stats++;
                    }
                }
            }

            stats_to_check >>= 1;
            curr_stat++;
        }
        if (changeable_stats > 1)
            stat_animID = 0x39;
        else
            stat_animID = 0x3A;
    }
    else
    {
        u16 r1 = 0x0E;
        if (flags & STAT_CHANGE_BY_TWO)
            r1 = 0x26;

        while (stats_to_check != 0)
        {
            if (stats_to_check & 1 && gBattleMons[gActiveBattler].statStages[curr_stat] < 0xC)
            {
                stat_animID = r1 + curr_stat;
                changeable_stats++;
            }

            stats_to_check >>= 1;
            curr_stat++;
        }
        if (changeable_stats > 1 && flags & 2)
            stat_animID = 0x37;
        else
            stat_animID = 0x38;
    }

    if (flags & STAT_CHANGE_ONLY_MULTIPLE && changeable_stats <= 1)
        gBattlescriptCurrInstr += 4;
    else if (changeable_stats != 0 && !gBattleStruct->unk160DC)
    {
        BtlController_EmitBattleAnimation(0, B_ANIM_STATS_CHANGE, stat_animID);
        MarkBattlerForControllerExec(gActiveBattler);
        if ((flags & STAT_CHANGE_ONLY_MULTIPLE) && changeable_stats > 1)
            gBattleStruct->unk160DC = 1;
        gBattlescriptCurrInstr += 4;
    }
    else
    {
        gBattlescriptCurrInstr += 4;
    }
}

#define BattleScript_FlushMessageBox gUnknown_081D9B2D

extern u8 BattleScript_RageIsBuilding[];
extern u8 BattleScript_DefrostedViaFireMove[];
extern u8 BattleScript_GravityGrounded[];
extern u8 BattleScript_FlushMessageBox[];

// atk49, moveend cases
enum
{
    MOVEEND_EAT_BERRY,
    MOVEEND_RAGE,
    MOVEEND_DEFROST,
    MOVEEND_GRAVITY,
    MOVEEND_SYNCHRONIZE_TARGET,
    MOVEEND_ABILITIES,
    MOVEEND_STATUS_ABILITIES,
    MOVEEND_SYNCHRONIZE_ATTACKER,
    MOVEEND_CHOICE_MOVE,
    MOVEEND_CHANGED_ITEMS,
    MOVEEND_ITEM_EFFECTS,
    MOVEEND_KINGSROCK_SHELLBELL,
    MOVEEND_RESTORE_ITEM,
    MOVEEND_ATTACKER_INVISIBLE,
    MOVEEND_ATTACKER_VISIBLE,
    MOVEEND_TARGET_VISIBLE,
    MOVEEND_SUBSTITUTE,
    MOVEEND_UPDATE_LAST_MOVES,
    MOVEEND_MIRROR_MOVE,
    MOVEEND_NEXT_TARGET,
    MOVEEND_COUNT,
};

void atk49_moveend(void)
{
    int i;
    bool32 effect = FALSE;
    u16 moveType;
    u8 holdEffectAtk;
    u16 *choicedMoveAtk;
    u8 arg1, arg2;

    arg1 = T2_READ_8(gBattlescriptCurrInstr + 1);
    arg2 = T2_READ_8(gBattlescriptCurrInstr + 2);

    if (gBattleMons[gBattlerAttacker].item == ITEM_ENIGMA_BERRY)
        holdEffectAtk = gEnigmaBerries[gBattlerAttacker].holdEffect;
    else
        holdEffectAtk = ItemId_GetHoldEffect(gBattleMons[gBattlerAttacker].item);

    choicedMoveAtk = &((u16*)(gSharedMem + 0x160E8))[gBattlerAttacker];
    GET_MOVE_TYPE(gCurrentMove, moveType);

    do
    {
        switch (gBattleStruct->cmd49StateTracker)
        {
        case MOVEEND_EAT_BERRY: // for bug bite
            if (gBattleMoves[gCurrentMove].effect == EFFECT_EAT_BERRY)
            {
                if (ItemBattleEffects(ITEMEFFECT_BUGBITE_BERRIES, gBattlerAttacker, FALSE))
                    effect = TRUE;
            }
            gBattleStruct->cmd49StateTracker++;
            break;
        case MOVEEND_RAGE: // rage check
            if (gBattleMons[gBattlerTarget].status2 & STATUS2_RAGE
                && gBattleMons[gBattlerTarget].hp != 0 && gBattlerAttacker != gBattlerTarget
                && GetBattlerSide(gBattlerAttacker) != GetBattlerSide(gBattlerTarget)
                && !(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && (gProtectStructs[gBattlerTarget].physicalDmg != 0 || gProtectStructs[gBattlerTarget].specialDmg != 0)
                && gBattleMoves[gCurrentMove].power && gBattleMons[gBattlerTarget].statStages[STAT_STAGE_ATK] <= 0xB)
            {
                gBattleMons[gBattlerTarget].statStages[STAT_STAGE_ATK]++;
                BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_RageIsBuilding;
                effect = TRUE;
            }
            gBattleStruct->cmd49StateTracker++;
            break;
        case MOVEEND_DEFROST: // defrosting check
            if (gBattleMons[gBattlerTarget].status1 & STATUS1_FREEZE
                && gBattleMons[gBattlerTarget].hp && gBattlerAttacker != gBattlerTarget
                && gSpecialStatuses[gBattlerTarget].specialDmg
                && !(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && moveType == TYPE_FIRE)
            {
                gBattleMons[gBattlerTarget].status1 &= ~(STATUS1_FREEZE);
                gActiveBattler = gBattlerTarget;
                BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gBattlerTarget].status1);
                MarkBattlerForControllerExec(gActiveBattler);
                BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_DefrostedViaFireMove;
                effect = TRUE;
            }
            gBattleStruct->cmd49StateTracker++;
            break;
        case MOVEEND_GRAVITY: // make the mons fall if gravity is active
            if (gBattleGlobalTimers.gravityTimer)
            {
                for (i = 0; i < gBattlersCount; i++)
                {
                    if (gStatuses3[i] & STATUS3_ON_AIR/* | STATUS3_TELEKINESIS*/ 
                     || gDisableStructs[i].magnetRiseTimer)
                    {
                        if (gStatuses3[i] & STATUS3_ON_AIR)
                            CancelMultiTurnMoves(i);
                        gStatuses3[i] &= ~(/*STATUS3_TELEKINESIS | */STATUS3_ON_AIR);
                        gDisableStructs[i].magnetRiseTimer = 0;
                        gActiveBattler = i;
                        BtlController_EmitSpriteInvisibility(0, 0);
                        MarkBattlerForControllerExec(i);
                        BattleScriptPushCursor();
                        gBattleStruct->scriptingActive = i;
                        gBattlescriptCurrInstr = BattleScript_GravityGrounded;
                        effect = TRUE;
                    }
                }
            }
            gBattleStruct->cmd49StateTracker++;
            break;
        case MOVEEND_SYNCHRONIZE_TARGET: // target synchronize
            if (AbilityBattleEffects(ABILITYEFFECT_SYNCHRONIZE, gBattlerTarget, 0, 0, 0))
                effect = TRUE;
            gBattleStruct->cmd49StateTracker++;
            break;
        case MOVEEND_ABILITIES: // Such as abilities activating on contact(Poison Spore, Rough Skin, etc.).
            if (AbilityBattleEffects(ABILITYEFFECT_MOVE_END, gBattlerTarget, 0, 0, 0))
                effect = TRUE;
            gBattleStruct->cmd49StateTracker++;
            break;
        case MOVEEND_STATUS_ABILITIES: //status immunities
            if (AbilityBattleEffects(ABILITYEFFECT_IMMUNITY, 0, 0, 0, 0))
                effect = TRUE; //it loops through 4 banks, so we increment after its done with all banks
            else
                gBattleStruct->cmd49StateTracker++;
            break;
        case MOVEEND_SYNCHRONIZE_ATTACKER: //attacker synchronize
            if (AbilityBattleEffects(ABILITYEFFECT_ATK_SYNCHRONIZE, gBattlerAttacker, 0, 0, 0))
                effect = TRUE;
            gBattleStruct->cmd49StateTracker++;
            break;
        case MOVEEND_CHOICE_MOVE: //update choice band move
            if (!(gHitMarker & HITMARKER_OBEYS) || holdEffectAtk != HOLD_EFFECT_CHOICE_ITEM
                || gChosenMove == MOVE_STRUGGLE || (*choicedMoveAtk != 0 && *choicedMoveAtk != 0xFFFF))
                    goto LOOP;
            if (gChosenMove == MOVE_BATON_PASS && !(gMoveResultFlags & MOVE_RESULT_FAILED))
            {
                gBattleStruct->cmd49StateTracker++;
                break;
            }
            *choicedMoveAtk = gChosenMove;
            LOOP:
            {
                for (i = 0; i < 4; i++)
                {
                    if (gBattleMons[gBattlerAttacker].moves[i] == *choicedMoveAtk)
                        break;
                }
                if (i == 4)
                    *choicedMoveAtk = 0;
                gBattleStruct->cmd49StateTracker++;
            }
            break;
        case MOVEEND_CHANGED_ITEMS: // changed held items
            for (i = 0; i < gBattlersCount; i++)
            {
                #define CHANGED_ITEM ((u16 *)(gSharedMem + 0x160F0))
                if (CHANGED_ITEM[i] != 0)
                {
                    gBattleMons[i].item = CHANGED_ITEM[i];
                    CHANGED_ITEM[i] = 0;
                }
                #undef CHANGED_ITEM
            }
            gBattleStruct->cmd49StateTracker++;
            break;
        case MOVEEND_ITEM_EFFECTS: // item effects for all battlers
            if (ItemBattleEffects(ITEMEFFECT_MOVE_END, 0, FALSE))
                effect = TRUE;
            else
                gBattleStruct->cmd49StateTracker++;
            break;
        case MOVEEND_KINGSROCK_SHELLBELL: // king's rock and shell bell
            if (ItemBattleEffects(ITEMEFFECT_KINGSROCK_SHELLBELL, 0, FALSE))
                effect = TRUE;
            gBattleStruct->cmd49StateTracker++;
            break;
        case MOVEEND_RESTORE_ITEM: // restore original held item after eating the new one
            if (gDisableStructs[gBattlerAttacker].heldItemForBugBite != 0)
            {
                gBattleMons[gBattlerAttacker].item = gDisableStructs[gBattlerAttacker].heldItemForBugBite;
                if (gBattleMons[gBattlerAttacker].item == gDisableStructs[gBattlerAttacker].heldItemForBugBite)
                    gDisableStructs[gBattlerAttacker].heldItemForBugBite = 0;
                gActiveBattler = gBattlerAttacker;
                BtlController_EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gActiveBattler].item);
                MarkBattlerForControllerExec(gActiveBattler);
                effect = TRUE;
            }
            else 
                gBattleStruct->cmd49StateTracker++;
            break;
        case MOVEEND_ATTACKER_INVISIBLE: // make attacker sprite invisible
            if ((gStatuses3[gBattlerAttacker] & STATUS3_SEMI_INVULNERABLE) && (gHitMarker & HITMARKER_NO_ANIMATIONS))
            {
                gActiveBattler = gBattlerAttacker;
                BtlController_EmitSpriteInvisibility(0, 1);
                MarkBattlerForControllerExec(gActiveBattler);
            }
            gBattleStruct->cmd49StateTracker++;
            break;
        case MOVEEND_ATTACKER_VISIBLE: // make attacker sprite visible
            if (gMoveResultFlags & MOVE_RESULT_NO_EFFECT
                || !(gStatuses3[gBattlerAttacker] & (STATUS3_SEMI_INVULNERABLE))
                || WasUnableToUseMove(gBattlerAttacker))
            {
                gActiveBattler = gBattlerAttacker;
                BtlController_EmitSpriteInvisibility(0, FALSE);
                MarkBattlerForControllerExec(gActiveBattler);
                gStatuses3[gBattlerAttacker] &= ~(STATUS3_SEMI_INVULNERABLE);
                gSpecialStatuses[gBattlerAttacker].restoredBattlerSprite = 1;
            }
            gBattleStruct->cmd49StateTracker++;
            break;
        case MOVEEND_TARGET_VISIBLE: // make target sprite visible
            if (!gSpecialStatuses[gBattlerTarget].restoredBattlerSprite && gBattlerTarget < gBattlersCount
                && !(gStatuses3[gBattlerTarget] & STATUS3_SEMI_INVULNERABLE))
            {
                gActiveBattler = gBattlerTarget;
                BtlController_EmitSpriteInvisibility(0, FALSE);
                MarkBattlerForControllerExec(gActiveBattler);
                gStatuses3[gBattlerTarget] &= ~(STATUS3_SEMI_INVULNERABLE);
            }
            gBattleStruct->cmd49StateTracker++;
            break;
        case MOVEEND_SUBSTITUTE: // update substitute
            for (i = 0; i < gBattlersCount; i++)
            {
                if (gDisableStructs[i].substituteHP == 0)
                    gBattleMons[i].status2 &= ~(STATUS2_SUBSTITUTE);
            }
            gBattleStruct->cmd49StateTracker++;
            break;
        case MOVEEND_UPDATE_LAST_MOVES:
            if (gHitMarker & HITMARKER_SWAP_ATTACKER_TARGET)
            {
                gActiveBattler = gBattlerAttacker;
                gBattlerAttacker = gBattlerTarget;
                gBattlerTarget = gActiveBattler;
                gHitMarker &= ~(HITMARKER_SWAP_ATTACKER_TARGET);
            }

            //if (!gSpecialStatuses[gBattlerAttacker].dancerUsedMove)
            {
                gDisableStructs[gBattlerAttacker].usedMoves |= gBitTable[gCurrMovePos];
                //gBattleStruct->lastMoveTarget[gBattlerAttacker] = gBattlerTarget;
                if (gHitMarker & HITMARKER_ATTACKSTRING_PRINTED)
                {
                    gLastUsedMove = gCurrentMove;
                }
            }

            if ((gBattleMoves[gChosenMove].effect != EFFECT_BATON_PASS && gBattleMoves[gChosenMove].effect != EFFECT_HEALING_WISH) || (gMoveResultFlags & 0x29))
            {
                gUnknown_02024C2C[gBattlerAttacker] = gChosenMove;
            }

            if (!(gAbsentBattlerFlags & gBitTable[gBattlerAttacker])
                && !(gSharedMem[0x160A6] & gBitTable[gBattlerAttacker])
                && gBattleMoves[gChosenMove].effect != EFFECT_BATON_PASS
                && gBattleMoves[gChosenMove].effect != EFFECT_HEALING_WISH)
            {
                if (gHitMarker & HITMARKER_OBEYS)
                {
                    gLastMoves[gBattlerAttacker] = gChosenMove;
                    gUnknown_02024C4C[gBattlerAttacker] = gCurrentMove;
                }
                else
                {
                    gLastMoves[gBattlerAttacker] = 0xFFFF;
                    gUnknown_02024C4C[gBattlerAttacker] = 0xFFFF;
                }

                if (!(gHitMarker & HITMARKER_FAINTED(gBattlerTarget)))
                    gLastHitBy[gBattlerTarget] = gBattlerAttacker;

                if (gHitMarker & HITMARKER_OBEYS && !(gMoveResultFlags & MOVE_RESULT_NO_EFFECT))
                {
                    if (gChosenMove == 0xFFFF)
                    {
                        gLastLandedMoves[gBattlerTarget] = gChosenMove;
                    }
                    else
                    {
                        gLastLandedMoves[gBattlerTarget] = gCurrentMove;
                        GET_MOVE_TYPE(gCurrentMove, gLastHitByType[gBattlerTarget]);
                    }
                }
                else
                {
                    gLastLandedMoves[gBattlerTarget] = 0xFFFF;
                }
            }

            gDisableStructs[gBattlerAttacker].abilityHasBoosted = FALSE;
            gDisableStructs[gBattlerTarget].abilityHasBoosted = FALSE;

            gBattleStruct->cmd49StateTracker++;
            break;
        case MOVEEND_MIRROR_MOVE:
            if (!(gAbsentBattlerFlags & gBitTable[gBattlerAttacker]) && !(gSharedMem[0x160A6] & gBitTable[gBattlerAttacker]))
            {
                if (gBattleMoves[gChosenMove].flags & 0x10 && gHitMarker & HITMARKER_OBEYS
                 && gBattlerAttacker != gBattlerTarget && !(gHitMarker & HITMARKER_FAINTED(gBattlerTarget))
                 && !(gMoveResultFlags & MOVE_RESULT_NO_EFFECT))
                {
                    gSharedMem[0x160AC + gBattlerTarget * 2 + 0] = gChosenMove;
                    gSharedMem[0x160AC + gBattlerTarget * 2 + 1] = gChosenMove >> 8;

                    gSharedMem[0x16100 + gBattlerTarget * 4 + 0 + (gBattlerAttacker / 2) * 2] = gChosenMove;
                    gSharedMem[0x16100 + gBattlerTarget * 4 + 1 + (gBattlerAttacker / 2) * 2] = gChosenMove >> 8;
                }
                else if (gBattlerAttacker != gBattlerTarget)
                {
                    gSharedMem[0x160AC + gBattlerTarget * 2 + 0] = 0;
                    gSharedMem[0x160AC + gBattlerTarget * 2 + 1] = 0;

                    gSharedMem[0x16100 + gBattlerTarget * 4 + 0 + (gBattlerAttacker / 2) * 2] = 0;
                    gSharedMem[0x16100 + gBattlerTarget * 4 + 1 + (gBattlerAttacker / 2) * 2] = 0;
                }
            }
            gBattleStruct->cmd49StateTracker++;
            break;
        case MOVEEND_NEXT_TARGET:
            if (!(gHitMarker & HITMARKER_UNABLE_TO_USE_MOVE) && gBattleTypeFlags & BATTLE_TYPE_DOUBLE
                && !gProtectStructs[gBattlerAttacker].chargingTurn && gBattleMoves[gCurrentMove].target == MOVE_TARGET_BOTH
                && !(gHitMarker & HITMARKER_NO_ATTACKSTRING))
            {
                u8 battlerId = GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(gBattlerTarget)));
                if (gBattleMons[battlerId].hp != 0)
                {
                    gBattlerTarget = battlerId;
                    gHitMarker |= HITMARKER_NO_ATTACKSTRING;
                    gBattleStruct->cmd49StateTracker = 0;
                    MoveValuesCleanUp();
                    BattleScriptPush(gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect]);
                    gBattlescriptCurrInstr = BattleScript_FlushMessageBox;
                    return;
                }
                else
                {
                    gHitMarker |= HITMARKER_NO_ATTACKSTRING;
                }
            }
            gBattleStruct->cmd49StateTracker++;
            break;
        case MOVEEND_COUNT:
            break;
        }

        if (arg1 == 1 && effect == FALSE)
            gBattleStruct->cmd49StateTracker = MOVEEND_COUNT;
        if (arg1 == 2 && arg2 == gBattleStruct->cmd49StateTracker)
            gBattleStruct->cmd49StateTracker = MOVEEND_COUNT;

    } while (gBattleStruct->cmd49StateTracker != MOVEEND_COUNT && effect == FALSE);

    if (gBattleStruct->cmd49StateTracker == MOVEEND_COUNT && effect == FALSE)
        gBattlescriptCurrInstr += 3;
}

static void atk4A_typecalc2(void)
{
    u8 flags = 0;
    int i = 0;
    u8 moveType = gBattleMoves[gCurrentMove].type;

    if (GetBattlerAbility(gBattlerTarget) == ABILITY_NORMALIZE)
    {
        moveType = TYPE_NORMAL;
    }

    if ((GetBattlerAbility(gBattlerTarget) == ABILITY_LEVITATE || gDisableStructs[gBattlerTarget].magnetRiseTimer)
     && moveType == TYPE_GROUND 
     && ItemId_GetHoldEffect(gBattleMons[gBattlerTarget].item) != HOLD_EFFECT_IRON_BALL 
     && !gBattleGlobalTimers.gravityTimer )
    {
        gLastUsedAbility = GetBattlerAbility(gBattlerTarget);
        gMoveResultFlags |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);
        gLastLandedMoves[gBattlerTarget] = 0;
        gBattleCommunication[6] = moveType;
        RecordAbilityBattle(gBattlerTarget, gLastUsedAbility);
    }
    else
    {
        while (gTypeEffectiveness[i] != TYPE_ENDTABLE)
        {
            if (gTypeEffectiveness[i] == TYPE_FORESIGHT)
            {
                if (gBattleMons[gBattlerTarget].status2 & STATUS2_FORESIGHT) {break;}
                else {i += 3; continue;}
            }

            if (gTypeEffectiveness[i] == moveType)
            {
                //check type1
                if (gTypeEffectiveness[i + 1] == gBattleMons[gBattlerTarget].type1)
                {
                    if (gTypeEffectiveness[i + 2] == 0)
                    {
                        gMoveResultFlags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
                        break;
                    }
                    if (gTypeEffectiveness[i + 2] == 5)
                        flags |= MOVE_RESULT_NOT_VERY_EFFECTIVE;
                    if (gTypeEffectiveness[i + 2] == 20)
                        flags |= MOVE_RESULT_SUPER_EFFECTIVE;
                }
                //check type2
                if (gTypeEffectiveness[i + 1] == gBattleMons[gBattlerTarget].type2)
                {
                    if (gBattleMons[gBattlerTarget].type1 != gBattleMons[gBattlerTarget].type2
                        && gTypeEffectiveness[i + 2] == 0)
                    {
                        gMoveResultFlags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
                        break;
                    }
                    if (gTypeEffectiveness[i + 1] == gBattleMons[gBattlerTarget].type2 && gBattleMons[gBattlerTarget].type1 != gBattleMons[gBattlerTarget].type2 && gTypeEffectiveness[i + 2] == 5)
                        flags |= MOVE_RESULT_NOT_VERY_EFFECTIVE;
                    if (gTypeEffectiveness[i + 1] == gBattleMons[gBattlerTarget].type2
                        && gBattleMons[gBattlerTarget].type1 != gBattleMons[gBattlerTarget].type2 && gTypeEffectiveness[i + 2] == 20)
                            flags |= MOVE_RESULT_SUPER_EFFECTIVE;
                }
            }
            i += 3;
        }
    }

    if (GetBattlerAbility(gBattlerTarget) == ABILITY_WONDER_GUARD && !(flags & MOVE_RESULT_NO_EFFECT) && AttacksThisTurn(gBattlerAttacker, gCurrentMove) == 2 &&
        (!(flags & MOVE_RESULT_SUPER_EFFECTIVE) || ((flags & (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)) == (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE))) &&
        gBattleMoves[gCurrentMove].power)
    {
        gLastUsedAbility = ABILITY_WONDER_GUARD;
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gLastLandedMoves[gBattlerTarget] = 0;
        gBattleCommunication[6] = 3;
        RecordAbilityBattle(gBattlerTarget, gLastUsedAbility);
    }
    if (gMoveResultFlags & MOVE_RESULT_DOESNT_AFFECT_FOE)
        gProtectStructs[gBattlerAttacker].targetNotAffected = 1;

    gBattlescriptCurrInstr++;
}

static void atk4B_returnatktoball(void)
{
    gActiveBattler = gBattlerAttacker;
    if (!(gHitMarker & HITMARKER_FAINTED(gActiveBattler)))
    {
        BtlController_EmitReturnMonToBall(0, 0);
        MarkBattlerForControllerExec(gActiveBattler);
    }
    gBattlescriptCurrInstr++;
}

static void atk4C_getswitchedmondata(void)
{
    if (gBattleControllerExecFlags)
        return;

    gActiveBattler = GetBattlerForBattleScript(T2_READ_8(gBattlescriptCurrInstr + 1));

    gBattlerPartyIndexes[gActiveBattler] = ewram16068arr(gActiveBattler);

    BtlController_EmitGetMonData(0, 0, gBitTable[gBattlerPartyIndexes[gActiveBattler]]);
    MarkBattlerForControllerExec(gActiveBattler);
    gBattlescriptCurrInstr += 2;
}

static void atk4D_switchindataupdate(void)
{
    struct BattlePokemon oldData;
    s32 i;
    u8 *monData;

    if (gBattleControllerExecFlags)
        return;

    gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
    oldData = gBattleMons[gActiveBattler];
    monData = (u8*)(&gBattleMons[gActiveBattler]);

    for (i = 0; i < sizeof(struct BattlePokemon); i++)
    {
        monData[i] = gBattleBufferB[gActiveBattler][4 + i];
    }

    gBattleMons[gActiveBattler].type1 = gBaseStats[gBattleMons[gActiveBattler].species].type1;
    gBattleMons[gActiveBattler].type2 = gBaseStats[gBattleMons[gActiveBattler].species].type2;
    gBattleMons[gActiveBattler].ability = GetAbilityBySpecies(gBattleMons[gActiveBattler].species, gBattleMons[gActiveBattler].altAbility, gBattleMons[gActiveBattler].hiddenAbility);

    // check knocked off item
    i = GetBattlerSide(gActiveBattler);
    if (gWishFutureKnock.knockedOffPokes[i] & gBitTable[gBattlerPartyIndexes[gActiveBattler]])
    {
        gBattleMons[gActiveBattler].item = 0;
    }

    if (gBattleMoves[gCurrentMove].effect == EFFECT_BATON_PASS)
    {
        for (i = 0; i < 8; i++)
        {
            gBattleMons[gActiveBattler].statStages[i] = oldData.statStages[i];
        }
        gBattleMons[gActiveBattler].status2 = oldData.status2;
    }

    SwitchInClearSetData();

    gBattleStruct->scriptingActive = gActiveBattler;

    PREPARE_MON_NICK_BUFFER(gBattleTextBuff1, gActiveBattler, gBattlerPartyIndexes[gActiveBattler]);

    gBattlescriptCurrInstr += 2;
}

static void atk4E_switchinanim(void)
{
    if (gBattleControllerExecFlags)
        return;

    gActiveBattler = GetBattlerForBattleScript(T2_READ_8(gBattlescriptCurrInstr + 1));
    if (GetBattlerSide(gActiveBattler) == 1 && !(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_EREADER_TRAINER | BATTLE_TYPE_BATTLE_TOWER)))
    {
        GetSetPokedexFlag(SpeciesToNationalPokedexNum(gBattleMons[gActiveBattler].species), 2);
    }
    gAbsentBattlerFlags &= ~(gBitTable[gActiveBattler]);
    BtlController_EmitSwitchInAnim(0, gBattlerPartyIndexes[gActiveBattler], T2_READ_8(gBattlescriptCurrInstr + 2));
    MarkBattlerForControllerExec(gActiveBattler);
    gBattlescriptCurrInstr += 3;
}

static void atk4F_jumpifcantswitch(void)
{
    int val, to_cmp;
    register struct Pokemon *party;
    u8 r7;
    //0x80 byte is used as a way of telling the function whether to not check status2/status3
    gActiveBattler = GetBattlerForBattleScript(T2_READ_8(gBattlescriptCurrInstr + 1) & 0x7F);
    if (!(T2_READ_8(gBattlescriptCurrInstr + 1) & 0x80)
        && ((gBattleMons[gActiveBattler].status2 & (STATUS2_WRAPPED | STATUS2_ESCAPE_PREVENTION))
            || (gStatuses3[gActiveBattler] & STATUS3_ROOTED)))
    {
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
        return;
    }
    if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
    {
        if (GetBattlerSide(gActiveBattler) == 1)
            party = gEnemyParty;
        else
            party = gPlayerParty;
        val = 0;
        if (sub_803FBFC(sub_803FC34(gActiveBattler)) == 1)
            val = 3;
        for (to_cmp = val + 3; val < to_cmp; val++)
        {
            if (GetMonData(&party[val], MON_DATA_SPECIES) != SPECIES_NONE
             && !GetMonData(&party[val], MON_DATA_IS_EGG)
             && GetMonData(&party[val], MON_DATA_HP) != 0
             && gBattlerPartyIndexes[gActiveBattler] != val)
                break;
        }
        if (val == to_cmp)
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
        else
            gBattlescriptCurrInstr += 6;
    }
    else
    {
        if (GetBattlerSide(gActiveBattler) == 1)
        {
            r7 = GetBattlerAtPosition(1);
            if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
                to_cmp = GetBattlerAtPosition(3);
            else
                to_cmp = r7;
            party = gEnemyParty;
        }
        else
        {
            r7 = GetBattlerAtPosition(0);
            if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
                to_cmp = GetBattlerAtPosition(2);
            else
                to_cmp = r7;
            party = gPlayerParty;
        }
        for (val = 0; val < 6; val++)
        {
            if (GetMonData(&party[val], MON_DATA_HP) != 0
             && GetMonData(&party[val], MON_DATA_SPECIES) != SPECIES_NONE
             && !GetMonData(&party[val], MON_DATA_IS_EGG)
             && val != gBattlerPartyIndexes[r7] && val != gBattlerPartyIndexes[to_cmp])
                break;
        }
        if (val == 6)
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
        else
            gBattlescriptCurrInstr += 6;
    }
}

void sub_8022A3C(u8 unkown)
{
    BATTLE_PARTY_ID(gActiveBattler) = gBattlerPartyIndexes[gActiveBattler];
    BtlController_EmitChoosePokemon(0, 1, unkown, 0, gBattleStruct->unk1606C[gActiveBattler]);
    MarkBattlerForControllerExec(gActiveBattler);
}

/*
static void atk50_openpartyscreen(void)
{
    int i = 0;
    int r9 = 0;
    u8* fail_loc = T1_READ_PTR(gBattlescriptCurrInstr + 2);

    if (T2_READ_8(gBattlescriptCurrInstr + 1) == 5)
    {
        if ((gBattleTypeFlags & (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_MULTI)) != BATTLE_TYPE_DOUBLE)
        {
            for (gActiveBattler = i; gActiveBattler < gBattlersCount; gActiveBattler++)
            {
                if (!(gHitMarker & HITMARKER_FAINTED(gActiveBattler)))
                {
                    BtlController_EmitLinkStandbyMsg(0, 2);
                    MarkBattlerForControllerExec(gActiveBattler);
                }
                else if (sub_8018018(gActiveBattler, 6, 6) == 0
                         && !gSpecialStatuses[gActiveBattler].flag40)
                {
                    sub_8022A3C(6);
                    gSpecialStatuses[gActiveBattler].flag40 = 1;
                }
                else
                {
                    gAbsentBattlerFlags |= gBitTable[gActiveBattler];
                    gHitMarker &= (~HITMARKER_FAINTED(gActiveBattler));
                    BtlController_EmitLinkStandbyMsg(0, 2);
                    MarkBattlerForControllerExec(gActiveBattler);
                }
            }
        }
        else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
        {
            if (!(gHitMarker >> 0x1C & gBitTable[0]))
            {

            }
            else if (sub_8018018(gActiveBattler, 6, 6) == 0
                     && !gSpecialStatuses[gActiveBattler].flag40)
            {

            }
            else
            {

            }
        }
    }
}
*/

NAKED
static void atk50_openpartyscreen(void)
{
    asm(".syntax unified\n\
    push {r4-r7,lr}\n\
    mov r7, r9\n\
    mov r6, r8\n\
    push {r6,r7}\n\
    sub sp, 0x4\n\
    movs r7, 0\n\
    movs r0, 0\n\
    mov r9, r0\n\
    ldr r6, _08022B44 @ =gBattlescriptCurrInstr\n\
    ldr r1, [r6]\n\
    ldrb r2, [r1, 0x2]\n\
    ldrb r0, [r1, 0x3]\n\
    lsls r0, 8\n\
    orrs r2, r0\n\
    ldrb r0, [r1, 0x4]\n\
    lsls r0, 16\n\
    orrs r2, r0\n\
    ldrb r0, [r1, 0x5]\n\
    lsls r0, 24\n\
    orrs r2, r0\n\
    mov r8, r2\n\
    ldrb r2, [r1, 0x1]\n\
    adds r0, r2, 0\n\
    mov r12, r6\n\
    cmp r0, 0x5\n\
    beq _08022ACE\n\
    b _08022F74\n\
_08022ACE:\n\
    ldr r0, _08022B48 @ =gBattleTypeFlags\n\
    ldrh r0, [r0]\n\
    movs r1, 0x41\n\
    ands r1, r0\n\
    cmp r1, 0x1\n\
    beq _08022BBC\n\
    ldr r1, _08022B4C @ =gActiveBattler\n\
    strb r7, [r1]\n\
    ldr r0, _08022B50 @ =gBattlersCount\n\
    ldrb r0, [r0]\n\
    cmp r7, r0\n\
    bcc _08022AE8\n\
    b _08022F62\n\
_08022AE8:\n\
    ldr r7, _08022B54 @ =gHitMarker\n\
    ldr r6, _08022B58 @ =gBitTable\n\
    adds r4, r1, 0\n\
    ldr r0, _08022B5C @ =gAbsentBattlerFlags\n\
    mov r8, r0\n\
_08022AF2:\n\
    ldrb r2, [r4]\n\
    lsls r0, r2, 2\n\
    adds r0, r6\n\
    ldr r1, [r0]\n\
    lsls r1, 28\n\
    ldr r0, [r7]\n\
    ands r0, r1\n\
    cmp r0, 0\n\
    beq _08022B94\n\
    adds r0, r2, 0\n\
    movs r1, 0x6\n\
    movs r2, 0x6\n\
    bl sub_8018018\n\
    lsls r0, 24\n\
    cmp r0, 0\n\
    beq _08022B60\n\
    ldrb r0, [r4]\n\
    lsls r0, 2\n\
    adds r0, r6\n\
    ldr r0, [r0]\n\
    mov r2, r8\n\
    ldrb r1, [r2]\n\
    orrs r0, r1\n\
    strb r0, [r2]\n\
    ldrb r0, [r4]\n\
    lsls r0, 2\n\
    adds r0, r6\n\
    ldr r1, [r0]\n\
    lsls r1, 28\n\
    ldr r0, [r7]\n\
    bics r0, r1\n\
    str r0, [r7]\n\
    movs r0, 0\n\
    movs r1, 0x2\n\
    bl BtlController_EmitLinkStandbyMsg\n\
    ldrb r0, [r4]\n\
    bl MarkBattlerForControllerExec\n\
    b _08022BA2\n\
    .align 2, 0\n\
_08022B44: .4byte gBattlescriptCurrInstr\n\
_08022B48: .4byte gBattleTypeFlags\n\
_08022B4C: .4byte gActiveBattler\n\
_08022B50: .4byte gBattlersCount\n\
_08022B54: .4byte gHitMarker\n\
_08022B58: .4byte gBitTable\n\
_08022B5C: .4byte gAbsentBattlerFlags\n\
_08022B60:\n\
    ldr r5, _08022B90 @ =gSpecialStatuses\n\
    ldrb r0, [r4]\n\
    lsls r1, r0, 2\n\
    adds r1, r0\n\
    lsls r1, 2\n\
    adds r1, r5\n\
    ldrb r0, [r1]\n\
    lsls r0, 25\n\
    cmp r0, 0\n\
    blt _08022BA2\n\
    movs r0, 0x6\n\
    bl sub_8022A3C\n\
    ldrb r1, [r4]\n\
    lsls r0, r1, 2\n\
    adds r0, r1\n\
    lsls r0, 2\n\
    adds r0, r5\n\
    ldrb r1, [r0]\n\
    movs r2, 0x40\n\
    orrs r1, r2\n\
    strb r1, [r0]\n\
    b _08022BA2\n\
    .align 2, 0\n\
_08022B90: .4byte gSpecialStatuses\n\
_08022B94:\n\
    movs r0, 0\n\
    movs r1, 0x2\n\
    bl BtlController_EmitLinkStandbyMsg\n\
    ldrb r0, [r4]\n\
    bl MarkBattlerForControllerExec\n\
_08022BA2:\n\
    ldrb r0, [r4]\n\
    adds r0, 0x1\n\
    strb r0, [r4]\n\
    ldr r1, _08022BB8 @ =gBattlersCount\n\
    lsls r0, 24\n\
    lsrs r0, 24\n\
    ldrb r1, [r1]\n\
    cmp r0, r1\n\
    bcc _08022AF2\n\
    b _08022F62\n\
    .align 2, 0\n\
_08022BB8: .4byte gBattlersCount\n\
_08022BBC:\n\
    ands r1, r0\n\
    cmp r1, 0\n\
    bne _08022BC4\n\
    b _08022F62\n\
_08022BC4:\n\
    ldr r0, _08022C1C @ =gHitMarker\n\
    mov r8, r0\n\
    ldr r0, [r0]\n\
    lsrs r5, r0, 28\n\
    ldr r6, _08022C20 @ =gBitTable\n\
    ldr r0, [r6]\n\
    ands r0, r5\n\
    cmp r0, 0\n\
    beq _08022C7E\n\
    ldr r4, _08022C24 @ =gActiveBattler\n\
    strb r7, [r4]\n\
    movs r0, 0\n\
    movs r1, 0x6\n\
    movs r2, 0x6\n\
    bl sub_8018018\n\
    lsls r0, 24\n\
    cmp r0, 0\n\
    beq _08022C2C\n\
    ldr r2, _08022C28 @ =gAbsentBattlerFlags\n\
    ldrb r0, [r4]\n\
    lsls r0, 2\n\
    adds r0, r6\n\
    ldr r0, [r0]\n\
    ldrb r1, [r2]\n\
    orrs r0, r1\n\
    strb r0, [r2]\n\
    ldrb r0, [r4]\n\
    lsls r0, 2\n\
    adds r0, r6\n\
    ldr r1, [r0]\n\
    lsls r1, 28\n\
    mov r2, r8\n\
    ldr r0, [r2]\n\
    bics r0, r1\n\
    str r0, [r2]\n\
    movs r0, 0\n\
    bl BtlController_EmitCmd42\n\
    ldrb r0, [r4]\n\
    bl MarkBattlerForControllerExec\n\
    b _08022C7E\n\
    .align 2, 0\n\
_08022C1C: .4byte gHitMarker\n\
_08022C20: .4byte gBitTable\n\
_08022C24: .4byte gActiveBattler\n\
_08022C28: .4byte gAbsentBattlerFlags\n\
_08022C2C:\n\
    ldr r6, _08022C60 @ =gSpecialStatuses\n\
    ldrb r0, [r4]\n\
    lsls r1, r0, 2\n\
    adds r1, r0\n\
    lsls r1, 2\n\
    adds r1, r6\n\
    ldrb r0, [r1]\n\
    lsls r0, 25\n\
    cmp r0, 0\n\
    blt _08022C6C\n\
    ldr r0, _08022C64 @ =gSharedMem\n\
    ldr r1, _08022C68 @ =0x0001606a\n\
    adds r0, r1\n\
    ldrb r0, [r0]\n\
    bl sub_8022A3C\n\
    ldrb r1, [r4]\n\
    lsls r0, r1, 2\n\
    adds r0, r1\n\
    lsls r0, 2\n\
    adds r0, r6\n\
    ldrb r1, [r0]\n\
    movs r2, 0x40\n\
    orrs r1, r2\n\
    strb r1, [r0]\n\
    b _08022C7E\n\
    .align 2, 0\n\
_08022C60: .4byte gSpecialStatuses\n\
_08022C64: .4byte gSharedMem\n\
_08022C68: .4byte 0x0001606a\n\
_08022C6C:\n\
    movs r0, 0\n\
    movs r1, 0x2\n\
    bl BtlController_EmitLinkStandbyMsg\n\
    ldrb r0, [r4]\n\
    bl MarkBattlerForControllerExec\n\
    movs r2, 0x1\n\
    mov r9, r2\n\
_08022C7E:\n\
    ldr r6, _08022CD8 @ =gBitTable\n\
    ldr r0, [r6, 0x8]\n\
    ands r0, r5\n\
    cmp r0, 0\n\
    beq _08022D40\n\
    ldr r0, [r6]\n\
    ands r0, r5\n\
    cmp r0, 0\n\
    bne _08022D40\n\
    ldr r4, _08022CDC @ =gActiveBattler\n\
    movs r0, 0x2\n\
    strb r0, [r4]\n\
    movs r0, 0x2\n\
    movs r1, 0x6\n\
    movs r2, 0x6\n\
    bl sub_8018018\n\
    lsls r0, 24\n\
    cmp r0, 0\n\
    beq _08022CE8\n\
    ldr r2, _08022CE0 @ =gAbsentBattlerFlags\n\
    ldrb r0, [r4]\n\
    lsls r0, 2\n\
    adds r0, r6\n\
    ldr r0, [r0]\n\
    ldrb r1, [r2]\n\
    orrs r0, r1\n\
    strb r0, [r2]\n\
    ldr r2, _08022CE4 @ =gHitMarker\n\
    ldrb r0, [r4]\n\
    lsls r0, 2\n\
    adds r0, r6\n\
    ldr r1, [r0]\n\
    lsls r1, 28\n\
    ldr r0, [r2]\n\
    bics r0, r1\n\
    str r0, [r2]\n\
    movs r0, 0\n\
    bl BtlController_EmitCmd42\n\
    ldrb r0, [r4]\n\
    bl MarkBattlerForControllerExec\n\
    b _08022D40\n\
    .align 2, 0\n\
_08022CD8: .4byte gBitTable\n\
_08022CDC: .4byte gActiveBattler\n\
_08022CE0: .4byte gAbsentBattlerFlags\n\
_08022CE4: .4byte gHitMarker\n\
_08022CE8:\n\
    ldr r6, _08022D1C @ =gSpecialStatuses\n\
    ldrb r0, [r4]\n\
    lsls r1, r0, 2\n\
    adds r1, r0\n\
    lsls r1, 2\n\
    adds r1, r6\n\
    ldrb r0, [r1]\n\
    lsls r0, 25\n\
    cmp r0, 0\n\
    blt _08022D28\n\
    ldr r0, _08022D20 @ =gSharedMem\n\
    ldr r1, _08022D24 @ =0x00016068\n\
    adds r0, r1\n\
    ldrb r0, [r0]\n\
    bl sub_8022A3C\n\
    ldrb r1, [r4]\n\
    lsls r0, r1, 2\n\
    adds r0, r1\n\
    lsls r0, 2\n\
    adds r0, r6\n\
    ldrb r1, [r0]\n\
    movs r2, 0x40\n\
    orrs r1, r2\n\
    strb r1, [r0]\n\
    b _08022D40\n\
    .align 2, 0\n\
_08022D1C: .4byte gSpecialStatuses\n\
_08022D20: .4byte gSharedMem\n\
_08022D24: .4byte 0x00016068\n\
_08022D28:\n\
    movs r0, 0x1\n\
    mov r2, r9\n\
    ands r0, r2\n\
    cmp r0, 0\n\
    bne _08022D40\n\
    movs r0, 0\n\
    movs r1, 0x2\n\
    bl BtlController_EmitLinkStandbyMsg\n\
    ldrb r0, [r4]\n\
    bl MarkBattlerForControllerExec\n\
_08022D40:\n\
    ldr r6, _08022D90 @ =gBitTable\n\
    ldr r0, [r6, 0x4]\n\
    ands r0, r5\n\
    cmp r0, 0\n\
    beq _08022DF6\n\
    ldr r4, _08022D94 @ =gActiveBattler\n\
    movs r0, 0x1\n\
    strb r0, [r4]\n\
    movs r0, 0x1\n\
    movs r1, 0x6\n\
    movs r2, 0x6\n\
    bl sub_8018018\n\
    lsls r0, 24\n\
    cmp r0, 0\n\
    beq _08022DA0\n\
    ldr r2, _08022D98 @ =gAbsentBattlerFlags\n\
    ldrb r0, [r4]\n\
    lsls r0, 2\n\
    adds r0, r6\n\
    ldr r0, [r0]\n\
    ldrb r1, [r2]\n\
    orrs r0, r1\n\
    strb r0, [r2]\n\
    ldr r2, _08022D9C @ =gHitMarker\n\
    ldrb r0, [r4]\n\
    lsls r0, 2\n\
    adds r0, r6\n\
    ldr r1, [r0]\n\
    lsls r1, 28\n\
    ldr r0, [r2]\n\
    bics r0, r1\n\
    str r0, [r2]\n\
    movs r0, 0\n\
    bl BtlController_EmitCmd42\n\
    ldrb r0, [r4]\n\
    bl MarkBattlerForControllerExec\n\
    b _08022DF6\n\
    .align 2, 0\n\
_08022D90: .4byte gBitTable\n\
_08022D94: .4byte gActiveBattler\n\
_08022D98: .4byte gAbsentBattlerFlags\n\
_08022D9C: .4byte gHitMarker\n\
_08022DA0:\n\
    ldr r6, _08022DD4 @ =gSpecialStatuses\n\
    ldrb r0, [r4]\n\
    lsls r1, r0, 2\n\
    adds r1, r0\n\
    lsls r1, 2\n\
    adds r1, r6\n\
    ldrb r0, [r1]\n\
    lsls r0, 25\n\
    cmp r0, 0\n\
    blt _08022DE0\n\
    ldr r0, _08022DD8 @ =gSharedMem\n\
    ldr r1, _08022DDC @ =0x0001606b\n\
    adds r0, r1\n\
    ldrb r0, [r0]\n\
    bl sub_8022A3C\n\
    ldrb r1, [r4]\n\
    lsls r0, r1, 2\n\
    adds r0, r1\n\
    lsls r0, 2\n\
    adds r0, r6\n\
    ldrb r1, [r0]\n\
    movs r2, 0x40\n\
    orrs r1, r2\n\
    strb r1, [r0]\n\
    b _08022DF6\n\
    .align 2, 0\n\
_08022DD4: .4byte gSpecialStatuses\n\
_08022DD8: .4byte gSharedMem\n\
_08022DDC: .4byte 0x0001606b\n\
_08022DE0:\n\
    movs r0, 0\n\
    movs r1, 0x2\n\
    bl BtlController_EmitLinkStandbyMsg\n\
    ldrb r0, [r4]\n\
    bl MarkBattlerForControllerExec\n\
    movs r0, 0x2\n\
    mov r2, r9\n\
    orrs r2, r0\n\
    mov r9, r2\n\
_08022DF6:\n\
    ldr r6, _08022E50 @ =gBitTable\n\
    ldr r0, [r6, 0xC]\n\
    ands r0, r5\n\
    cmp r0, 0\n\
    beq _08022EB8\n\
    ldr r0, [r6, 0x4]\n\
    ands r0, r5\n\
    cmp r0, 0\n\
    bne _08022EB8\n\
    ldr r4, _08022E54 @ =gActiveBattler\n\
    movs r0, 0x3\n\
    strb r0, [r4]\n\
    movs r0, 0x3\n\
    movs r1, 0x6\n\
    movs r2, 0x6\n\
    bl sub_8018018\n\
    lsls r0, 24\n\
    cmp r0, 0\n\
    beq _08022E60\n\
    ldr r2, _08022E58 @ =gAbsentBattlerFlags\n\
    ldrb r0, [r4]\n\
    lsls r0, 2\n\
    adds r0, r6\n\
    ldr r0, [r0]\n\
    ldrb r1, [r2]\n\
    orrs r0, r1\n\
    strb r0, [r2]\n\
    ldr r2, _08022E5C @ =gHitMarker\n\
    ldrb r0, [r4]\n\
    lsls r0, 2\n\
    adds r0, r6\n\
    ldr r1, [r0]\n\
    lsls r1, 28\n\
    ldr r0, [r2]\n\
    bics r0, r1\n\
    str r0, [r2]\n\
    movs r0, 0\n\
    bl BtlController_EmitCmd42\n\
    ldrb r0, [r4]\n\
    bl MarkBattlerForControllerExec\n\
    b _08022EB8\n\
    .align 2, 0\n\
_08022E50: .4byte gBitTable\n\
_08022E54: .4byte gActiveBattler\n\
_08022E58: .4byte gAbsentBattlerFlags\n\
_08022E5C: .4byte gHitMarker\n\
_08022E60:\n\
    ldr r6, _08022E94 @ =gSpecialStatuses\n\
    ldrb r0, [r4]\n\
    lsls r1, r0, 2\n\
    adds r1, r0\n\
    lsls r1, 2\n\
    adds r1, r6\n\
    ldrb r0, [r1]\n\
    lsls r0, 25\n\
    cmp r0, 0\n\
    blt _08022EA0\n\
    ldr r0, _08022E98 @ =gSharedMem\n\
    ldr r1, _08022E9C @ =0x00016069\n\
    adds r0, r1\n\
    ldrb r0, [r0]\n\
    bl sub_8022A3C\n\
    ldrb r1, [r4]\n\
    lsls r0, r1, 2\n\
    adds r0, r1\n\
    lsls r0, 2\n\
    adds r0, r6\n\
    ldrb r1, [r0]\n\
    movs r2, 0x40\n\
    orrs r1, r2\n\
    strb r1, [r0]\n\
    b _08022EB8\n\
    .align 2, 0\n\
_08022E94: .4byte gSpecialStatuses\n\
_08022E98: .4byte gSharedMem\n\
_08022E9C: .4byte 0x00016069\n\
_08022EA0:\n\
    movs r0, 0x2\n\
    mov r2, r9\n\
    ands r2, r0\n\
    cmp r2, 0\n\
    bne _08022EB8\n\
    movs r0, 0\n\
    movs r1, 0x2\n\
    bl BtlController_EmitLinkStandbyMsg\n\
    ldrb r0, [r4]\n\
    bl MarkBattlerForControllerExec\n\
_08022EB8:\n\
    ldr r1, _08022EE8 @ =gSpecialStatuses\n\
    ldrb r0, [r1]\n\
    lsls r0, 25\n\
    cmp r0, 0\n\
    blt _08022F0C\n\
    adds r0, r1, 0\n\
    adds r0, 0x28\n\
    ldrb r0, [r0]\n\
    lsls r0, 25\n\
    cmp r0, 0\n\
    blt _08022F0C\n\
    cmp r5, 0\n\
    beq _08022F0C\n\
    ldr r0, _08022EEC @ =gAbsentBattlerFlags\n\
    ldrb r1, [r0]\n\
    ldr r0, _08022EF0 @ =gBitTable\n\
    ldr r0, [r0]\n\
    ands r1, r0\n\
    cmp r1, 0\n\
    beq _08022EF8\n\
    ldr r1, _08022EF4 @ =gActiveBattler\n\
    movs r0, 0x2\n\
    strb r0, [r1]\n\
    b _08022EFC\n\
    .align 2, 0\n\
_08022EE8: .4byte gSpecialStatuses\n\
_08022EEC: .4byte gAbsentBattlerFlags\n\
_08022EF0: .4byte gBitTable\n\
_08022EF4: .4byte gActiveBattler\n\
_08022EF8:\n\
    ldr r0, _08022F3C @ =gActiveBattler\n\
    strb r1, [r0]\n\
_08022EFC:\n\
    movs r0, 0\n\
    movs r1, 0x2\n\
    bl BtlController_EmitLinkStandbyMsg\n\
    ldr r0, _08022F3C @ =gActiveBattler\n\
    ldrb r0, [r0]\n\
    bl MarkBattlerForControllerExec\n\
_08022F0C:\n\
    ldr r1, _08022F40 @ =gSpecialStatuses\n\
    ldrb r0, [r1, 0x14]\n\
    lsls r0, 25\n\
    cmp r0, 0\n\
    blt _08022F62\n\
    adds r0, r1, 0\n\
    adds r0, 0x3C\n\
    ldrb r0, [r0]\n\
    lsls r0, 25\n\
    cmp r0, 0\n\
    blt _08022F62\n\
    cmp r5, 0\n\
    beq _08022F62\n\
    ldr r0, _08022F44 @ =gAbsentBattlerFlags\n\
    ldrb r0, [r0]\n\
    ldr r1, _08022F48 @ =gBitTable\n\
    ldr r1, [r1, 0x4]\n\
    ands r0, r1\n\
    cmp r0, 0\n\
    beq _08022F4C\n\
    ldr r1, _08022F3C @ =gActiveBattler\n\
    movs r0, 0x3\n\
    b _08022F50\n\
    .align 2, 0\n\
_08022F3C: .4byte gActiveBattler\n\
_08022F40: .4byte gSpecialStatuses\n\
_08022F44: .4byte gAbsentBattlerFlags\n\
_08022F48: .4byte gBitTable\n\
_08022F4C:\n\
    ldr r1, _08022F6C @ =gActiveBattler\n\
    movs r0, 0x1\n\
_08022F50:\n\
    strb r0, [r1]\n\
    movs r0, 0\n\
    movs r1, 0x2\n\
    bl BtlController_EmitLinkStandbyMsg\n\
    ldr r0, _08022F6C @ =gActiveBattler\n\
    ldrb r0, [r0]\n\
    bl MarkBattlerForControllerExec\n\
_08022F62:\n\
    ldr r1, _08022F70 @ =gBattlescriptCurrInstr\n\
    ldr r0, [r1]\n\
    adds r0, 0x6\n\
    str r0, [r1]\n\
    b _08023302\n\
    .align 2, 0\n\
_08022F6C: .4byte gActiveBattler\n\
_08022F70: .4byte gBattlescriptCurrInstr\n\
_08022F74:\n\
    cmp r0, 0x6\n\
    beq _08022F7A\n\
    b _08023170\n\
_08022F7A:\n\
    ldr r0, _08022FF0 @ =gBattleTypeFlags\n\
    ldrh r2, [r0]\n\
    movs r0, 0x40\n\
    ands r0, r2\n\
    cmp r0, 0\n\
    beq _08022F88\n\
    b _0802310C\n\
_08022F88:\n\
    movs r0, 0x1\n\
    ands r0, r2\n\
    cmp r0, 0\n\
    bne _08022F92\n\
    b _0802310C\n\
_08022F92:\n\
    ldr r7, _08022FF4 @ =gHitMarker\n\
    ldr r0, [r7]\n\
    lsrs r5, r0, 28\n\
    ldr r4, _08022FF8 @ =gBitTable\n\
    ldr r0, [r4, 0x8]\n\
    ands r0, r5\n\
    cmp r0, 0\n\
    beq _0802303A\n\
    ldr r0, [r4]\n\
    ands r0, r5\n\
    cmp r0, 0\n\
    beq _0802303A\n\
    ldr r6, _08022FFC @ =gActiveBattler\n\
    movs r0, 0x2\n\
    strb r0, [r6]\n\
    ldr r0, _08023000 @ =gBattleBufferB\n\
    ldrb r1, [r0, 0x1]\n\
    movs r0, 0x2\n\
    movs r2, 0x6\n\
    bl sub_8018018\n\
    lsls r0, 24\n\
    cmp r0, 0\n\
    beq _08023008\n\
    ldr r2, _08023004 @ =gAbsentBattlerFlags\n\
    ldrb r0, [r6]\n\
    lsls r0, 2\n\
    adds r0, r4\n\
    ldr r0, [r0]\n\
    ldrb r1, [r2]\n\
    orrs r0, r1\n\
    strb r0, [r2]\n\
    ldrb r0, [r6]\n\
    lsls r0, 2\n\
    adds r0, r4\n\
    ldr r1, [r0]\n\
    lsls r1, 28\n\
    ldr r0, [r7]\n\
    bics r0, r1\n\
    str r0, [r7]\n\
    movs r0, 0\n\
    bl BtlController_EmitCmd42\n\
    ldrb r0, [r6]\n\
    bl MarkBattlerForControllerExec\n\
    b _0802303A\n\
    .align 2, 0\n\
_08022FF0: .4byte gBattleTypeFlags\n\
_08022FF4: .4byte gHitMarker\n\
_08022FF8: .4byte gBitTable\n\
_08022FFC: .4byte gActiveBattler\n\
_08023000: .4byte gBattleBufferB\n\
_08023004: .4byte gAbsentBattlerFlags\n\
_08023008:\n\
    ldr r4, _08023098 @ =gSpecialStatuses\n\
    ldrb r0, [r6]\n\
    lsls r1, r0, 2\n\
    adds r1, r0\n\
    lsls r1, 2\n\
    adds r1, r4\n\
    ldrb r0, [r1]\n\
    lsls r0, 25\n\
    cmp r0, 0\n\
    blt _0802303A\n\
    ldr r0, _0802309C @ =gSharedMem\n\
    ldr r1, _080230A0 @ =0x00016068\n\
    adds r0, r1\n\
    ldrb r0, [r0]\n\
    bl sub_8022A3C\n\
    ldrb r1, [r6]\n\
    lsls r0, r1, 2\n\
    adds r0, r1\n\
    lsls r0, 2\n\
    adds r0, r4\n\
    ldrb r1, [r0]\n\
    movs r2, 0x40\n\
    orrs r1, r2\n\
    strb r1, [r0]\n\
_0802303A:\n\
    ldr r4, _080230A4 @ =gBitTable\n\
    ldr r0, [r4, 0xC]\n\
    ands r0, r5\n\
    cmp r0, 0\n\
    beq _080230EE\n\
    ldr r0, [r4, 0x4]\n\
    ands r5, r0\n\
    cmp r5, 0\n\
    beq _080230EE\n\
    ldr r5, _080230A8 @ =gActiveBattler\n\
    movs r0, 0x3\n\
    strb r0, [r5]\n\
    ldr r0, _080230AC @ =gBattleBufferB\n\
    ldr r2, _080230B0 @ =0x00000201\n\
    adds r0, r2\n\
    ldrb r1, [r0]\n\
    movs r0, 0x3\n\
    movs r2, 0x6\n\
    bl sub_8018018\n\
    lsls r0, 24\n\
    cmp r0, 0\n\
    beq _080230BC\n\
    ldr r2, _080230B4 @ =gAbsentBattlerFlags\n\
    ldrb r0, [r5]\n\
    lsls r0, 2\n\
    adds r0, r4\n\
    ldr r0, [r0]\n\
    ldrb r1, [r2]\n\
    orrs r0, r1\n\
    strb r0, [r2]\n\
    ldr r2, _080230B8 @ =gHitMarker\n\
    ldrb r0, [r5]\n\
    lsls r0, 2\n\
    adds r0, r4\n\
    ldr r1, [r0]\n\
    lsls r1, 28\n\
    ldr r0, [r2]\n\
    bics r0, r1\n\
    str r0, [r2]\n\
    movs r0, 0\n\
    bl BtlController_EmitCmd42\n\
    ldrb r0, [r5]\n\
    bl MarkBattlerForControllerExec\n\
    b _080230EE\n\
    .align 2, 0\n\
_08023098: .4byte gSpecialStatuses\n\
_0802309C: .4byte gSharedMem\n\
_080230A0: .4byte 0x00016068\n\
_080230A4: .4byte gBitTable\n\
_080230A8: .4byte gActiveBattler\n\
_080230AC: .4byte gBattleBufferB\n\
_080230B0: .4byte 0x00000201\n\
_080230B4: .4byte gAbsentBattlerFlags\n\
_080230B8: .4byte gHitMarker\n\
_080230BC:\n\
    ldr r4, _080230FC @ =gSpecialStatuses\n\
    ldrb r0, [r5]\n\
    lsls r1, r0, 2\n\
    adds r1, r0\n\
    lsls r1, 2\n\
    adds r1, r4\n\
    ldrb r0, [r1]\n\
    lsls r0, 25\n\
    cmp r0, 0\n\
    blt _080230EE\n\
    ldr r0, _08023100 @ =gSharedMem\n\
    ldr r1, _08023104 @ =0x00016069\n\
    adds r0, r1\n\
    ldrb r0, [r0]\n\
    bl sub_8022A3C\n\
    ldrb r1, [r5]\n\
    lsls r0, r1, 2\n\
    adds r0, r1\n\
    lsls r0, 2\n\
    adds r0, r4\n\
    ldrb r1, [r0]\n\
    movs r2, 0x40\n\
    orrs r1, r2\n\
    strb r1, [r0]\n\
_080230EE:\n\
    ldr r1, _08023108 @ =gBattlescriptCurrInstr\n\
    ldr r0, [r1]\n\
    adds r0, 0x6\n\
    str r0, [r1]\n\
    mov r12, r1\n\
    b _08023110\n\
    .align 2, 0\n\
_080230FC: .4byte gSpecialStatuses\n\
_08023100: .4byte gSharedMem\n\
_08023104: .4byte 0x00016069\n\
_08023108: .4byte gBattlescriptCurrInstr\n\
_0802310C:\n\
    adds r0, r1, 0x6\n\
    str r0, [r6]\n\
_08023110:\n\
    ldr r0, _08023160 @ =gHitMarker\n\
    ldr r0, [r0]\n\
    lsrs r5, r0, 28\n\
    ldr r1, _08023164 @ =gBank1\n\
    movs r0, 0\n\
    strb r0, [r1]\n\
    ldr r4, _08023168 @ =gBitTable\n\
    ldr r2, [r4]\n\
    ands r2, r5\n\
    ldr r6, _0802316C @ =gBattlersCount\n\
    cmp r2, 0\n\
    bne _0802314C\n\
    adds r7, r6, 0\n\
    ldrb r0, [r6]\n\
    cmp r2, r0\n\
    bcs _0802314C\n\
    adds r3, r1, 0\n\
_08023132:\n\
    ldrb r0, [r3]\n\
    adds r0, 0x1\n\
    strb r0, [r3]\n\
    ldrb r2, [r3]\n\
    lsls r0, r2, 2\n\
    adds r0, r4\n\
    ldr r0, [r0]\n\
    ands r0, r5\n\
    cmp r0, 0\n\
    bne _0802314C\n\
    ldrb r0, [r7]\n\
    cmp r2, r0\n\
    bcc _08023132\n\
_0802314C:\n\
    ldrb r0, [r1]\n\
    ldrb r6, [r6]\n\
    cmp r0, r6\n\
    beq _08023156\n\
    b _08023302\n\
_08023156:\n\
    mov r1, r8\n\
    mov r2, r12\n\
    str r1, [r2]\n\
    b _08023302\n\
    .align 2, 0\n\
_08023160: .4byte gHitMarker\n\
_08023164: .4byte gBank1\n\
_08023168: .4byte gBitTable\n\
_0802316C: .4byte gBattlersCount\n\
_08023170:\n\
    movs r0, 0x80\n\
    ands r0, r2\n\
    movs r5, 0x1\n\
    cmp r0, 0\n\
    beq _0802317C\n\
    movs r5, 0\n\
_0802317C:\n\
    movs r0, 0x7F\n\
    ands r0, r2\n\
    bl GetBattlerForBattleScript\n\
    lsls r0, 24\n\
    lsrs r7, r0, 24\n\
    ldr r1, _080231A4 @ =gSpecialStatuses\n\
    lsls r0, r7, 2\n\
    adds r0, r7\n\
    lsls r0, 2\n\
    adds r0, r1\n\
    ldrb r0, [r0]\n\
    lsls r0, 25\n\
    cmp r0, 0\n\
    bge _080231A8\n\
    ldr r0, [r6]\n\
    adds r0, 0x6\n\
    str r0, [r6]\n\
    b _08023302\n\
    .align 2, 0\n\
_080231A4: .4byte gSpecialStatuses\n\
_080231A8:\n\
    adds r0, r7, 0\n\
    movs r1, 0x6\n\
    movs r2, 0x6\n\
    bl sub_8018018\n\
    lsls r0, 24\n\
    cmp r0, 0\n\
    beq _080231F8\n\
    ldr r2, _080231E8 @ =gActiveBattler\n\
    strb r7, [r2]\n\
    ldr r3, _080231EC @ =gAbsentBattlerFlags\n\
    ldr r4, _080231F0 @ =gBitTable\n\
    ldrb r0, [r2]\n\
    lsls r0, 2\n\
    adds r0, r4\n\
    ldr r0, [r0]\n\
    ldrb r1, [r3]\n\
    orrs r0, r1\n\
    strb r0, [r3]\n\
    ldr r3, _080231F4 @ =gHitMarker\n\
    ldrb r0, [r2]\n\
    lsls r0, 2\n\
    adds r0, r4\n\
    ldr r1, [r0]\n\
    lsls r1, 28\n\
    ldr r0, [r3]\n\
    bics r0, r1\n\
    str r0, [r3]\n\
    mov r0, r8\n\
    str r0, [r6]\n\
    b _08023302\n\
    .align 2, 0\n\
_080231E8: .4byte gActiveBattler\n\
_080231EC: .4byte gAbsentBattlerFlags\n\
_080231F0: .4byte gBitTable\n\
_080231F4: .4byte gHitMarker\n\
_080231F8:\n\
    ldr r4, _080232A0 @ =gActiveBattler\n\
    strb r7, [r4]\n\
    ldr r3, _080232A4 @ =gSharedMem\n\
    ldrb r0, [r4]\n\
    ldr r2, _080232A8 @ =0x00016064\n\
    adds r1, r0, r2\n\
    adds r1, r3\n\
    ldr r2, _080232AC @ =gBattlerPartyIndexes\n\
    lsls r0, 1\n\
    adds r0, r2\n\
    ldrh r0, [r0]\n\
    strb r0, [r1]\n\
    ldrb r1, [r4]\n\
    movs r0, 0x2\n\
    eors r0, r1\n\
    ldr r1, _080232B0 @ =0x00016068\n\
    adds r0, r1\n\
    adds r0, r3\n\
    ldrb r2, [r0]\n\
    ldrb r1, [r4]\n\
    lsls r0, r1, 1\n\
    adds r0, r1\n\
    ldr r1, _080232B4 @ =0x0001606c\n\
    adds r3, r1\n\
    adds r0, r3\n\
    str r0, [sp]\n\
    movs r0, 0\n\
    adds r1, r5, 0\n\
    movs r3, 0\n\
    bl BtlController_EmitChoosePokemon\n\
    ldrb r0, [r4]\n\
    bl MarkBattlerForControllerExec\n\
    ldr r0, [r6]\n\
    adds r0, 0x6\n\
    str r0, [r6]\n\
    ldrb r0, [r4]\n\
    bl GetBattlerPosition\n\
    lsls r0, 24\n\
    cmp r0, 0\n\
    bne _0802325A\n\
    ldr r1, _080232B8 @ =gBattleResults\n\
    ldrb r0, [r1, 0x2]\n\
    cmp r0, 0xFE\n\
    bhi _0802325A\n\
    adds r0, 0x1\n\
    strb r0, [r1, 0x2]\n\
_0802325A:\n\
    ldr r0, _080232BC @ =gBattleTypeFlags\n\
    ldrh r1, [r0]\n\
    movs r0, 0x40\n\
    ands r0, r1\n\
    cmp r0, 0\n\
    beq _080232C4\n\
    ldr r1, _080232A0 @ =gActiveBattler\n\
    movs r0, 0\n\
    strb r0, [r1]\n\
    ldr r0, _080232C0 @ =gBattlersCount\n\
    ldrb r0, [r0]\n\
    cmp r0, 0\n\
    beq _08023302\n\
    adds r4, r1, 0\n\
_08023276:\n\
    ldrb r0, [r4]\n\
    cmp r0, r7\n\
    beq _0802328A\n\
    movs r0, 0\n\
    movs r1, 0x2\n\
    bl BtlController_EmitLinkStandbyMsg\n\
    ldrb r0, [r4]\n\
    bl MarkBattlerForControllerExec\n\
_0802328A:\n\
    ldrb r0, [r4]\n\
    adds r0, 0x1\n\
    strb r0, [r4]\n\
    ldr r1, _080232C0 @ =gBattlersCount\n\
    lsls r0, 24\n\
    lsrs r0, 24\n\
    ldrb r1, [r1]\n\
    cmp r0, r1\n\
    bcc _08023276\n\
    b _08023302\n\
    .align 2, 0\n\
_080232A0: .4byte gActiveBattler\n\
_080232A4: .4byte gSharedMem\n\
_080232A8: .4byte 0x00016064\n\
_080232AC: .4byte gBattlerPartyIndexes\n\
_080232B0: .4byte 0x00016068\n\
_080232B4: .4byte 0x0001606c\n\
_080232B8: .4byte gBattleResults\n\
_080232BC: .4byte gBattleTypeFlags\n\
_080232C0: .4byte gBattlersCount\n\
_080232C4:\n\
    adds r0, r7, 0\n\
    bl GetBattlerPosition\n\
    movs r1, 0x1\n\
    eors r0, r1\n\
    lsls r0, 24\n\
    lsrs r0, 24\n\
    bl GetBattlerAtPosition\n\
    ldr r4, _08023310 @ =gActiveBattler\n\
    strb r0, [r4]\n\
    ldr r0, _08023314 @ =gAbsentBattlerFlags\n\
    ldrb r1, [r0]\n\
    ldr r2, _08023318 @ =gBitTable\n\
    ldrb r3, [r4]\n\
    lsls r0, r3, 2\n\
    adds r0, r2\n\
    ldr r0, [r0]\n\
    ands r1, r0\n\
    cmp r1, 0\n\
    beq _080232F4\n\
    movs r0, 0x2\n\
    eors r3, r0\n\
    strb r3, [r4]\n\
_080232F4:\n\
    movs r0, 0\n\
    movs r1, 0x2\n\
    bl BtlController_EmitLinkStandbyMsg\n\
    ldrb r0, [r4]\n\
    bl MarkBattlerForControllerExec\n\
_08023302:\n\
    add sp, 0x4\n\
    pop {r3,r4}\n\
    mov r8, r3\n\
    mov r9, r4\n\
    pop {r4-r7}\n\
    pop {r0}\n\
    bx r0\n\
    .align 2, 0\n\
_08023310: .4byte gActiveBattler\n\
_08023314: .4byte gAbsentBattlerFlags\n\
_08023318: .4byte gBitTable\n\
        .syntax divided");
}

static void atk51_switchhandleorder(void)
{
    int i;
    if (gBattleControllerExecFlags)
        return;

    gActiveBattler = GetBattlerForBattleScript(T2_READ_8(gBattlescriptCurrInstr + 1));
    switch (T2_READ_8(gBattlescriptCurrInstr + 2))
    {
    case 0:
        for (i = 0; i < gBattlersCount; i++)
        {
            if (gBattleBufferB[i][0] == 0x22)
                ewram16068arr(i) = gBattleBufferB[i][1];
        }
        break;
    case 1:
        if (!(gBattleTypeFlags & BATTLE_TYPE_MULTI))
            sub_8012258(gActiveBattler);
        break;
    case 2:
        gBattleCommunication[0] = gBattleBufferB[gActiveBattler][1];
        ewram16068arr(gActiveBattler) = gBattleBufferB[gActiveBattler][1];
        if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
        {
            ewram1606Carr(0, gActiveBattler) &= 0xF;
            ewram1606Carr(0, gActiveBattler) |= (gBattleBufferB[gActiveBattler][2] & 0xF0);
            ewram1606Carr(1, gActiveBattler) = gBattleBufferB[gActiveBattler][3];
            ewram1606Carr(0, (gActiveBattler ^ 2)) &= (0xF0);
            ewram1606Carr(0, (gActiveBattler ^ 2)) |= (gBattleBufferB[gActiveBattler][2] & 0xF0) >> 4;
            ewram1606Carr(2, (gActiveBattler ^ 2)) = gBattleBufferB[gActiveBattler][3];
        }
        else
            sub_8012258(gActiveBattler);

        gBattleTextBuff1[0] = 0xFD;
        gBattleTextBuff1[1] = 6;
        gBattleTextBuff1[2] = gBattleMons[gBattlerAttacker].species;
        gBattleTextBuff1[3] = gBattleMons[gBattlerAttacker].species >> 8;
        gBattleTextBuff1[4] = 0xFF;

        gBattleTextBuff2[0] = 0xFD;
        gBattleTextBuff2[1] = 7;
        gBattleTextBuff2[2] = gActiveBattler;
        gBattleTextBuff2[3] = gBattleBufferB[gActiveBattler][1];
        gBattleTextBuff2[4] = 0xFF;
        break;
    }
    gBattlescriptCurrInstr += 3;
}

static void atk52_switchineffects(void)
{
    int i;

    gActiveBattler = GetBattlerForBattleScript(T2_READ_8(gBattlescriptCurrInstr + 1));
    sub_80157C4(gActiveBattler);

    gHitMarker &= ~(HITMARKER_FAINTED(gActiveBattler));
    gSpecialStatuses[gActiveBattler].flag40 = 0;

    // first handle spikes
    if (!(gSideStatuses[GetBattlerSide(gActiveBattler)] & SIDE_STATUS_SPIKES_DAMAGED) 
     && (gSideStatuses[GetBattlerSide(gActiveBattler)] & SIDE_STATUS_SPIKES)
     && ((gBattleMons[gActiveBattler].type1 != TYPE_FLYING 
       && gBattleMons[gActiveBattler].type2 != TYPE_FLYING 
       && GetBattlerAbility(gActiveBattler) != ABILITY_LEVITATE
       && !gDisableStructs[gActiveBattler].magnetRiseTimer)
      || ItemId_GetHoldEffect(gBattleMons[gBattlerTarget].item) == HOLD_EFFECT_IRON_BALL 
      || gBattleGlobalTimers.gravityTimer))
    {
        u8 spikesDmg;

        gSideStatuses[GetBattlerSide(gActiveBattler)] |= SIDE_STATUS_SPIKES_DAMAGED;

        spikesDmg = (5 - gSideTimers[GetBattlerSide(gActiveBattler)].spikesAmount) * 2;
        gBattleMoveDamage = gBattleMons[gActiveBattler].maxHP / (spikesDmg);
        if (gBattleMoveDamage == 0)
            gBattleMoveDamage = 1;

        gBattleMons[gActiveBattler].status2 &= ~(STATUS2_DESTINY_BOND);
        gHitMarker &= ~(HITMARKER_DESTINYBOND);

        gBattleStruct->scriptingActive = gActiveBattler;
        BattleScriptPushCursor();

        if (T2_READ_8(gBattlescriptCurrInstr + 1) == 0)
            gBattlescriptCurrInstr = BattleScript_SpikesOnTarget;
        else if (T2_READ_8(gBattlescriptCurrInstr + 1) == 1)
            gBattlescriptCurrInstr = BattleScript_SpikesOnAttacker;
        else
            gBattlescriptCurrInstr = BattleScript_SpikesOngBank1;
    }
    // then handle stealth rock
    else if (!(gSideStatuses[GetBattlerSide(gActiveBattler)] & SIDE_STATUS_STEALTH_ROCK_DAMAGED)
          && (gSideStatuses[GetBattlerSide(gActiveBattler)] & SIDE_STATUS_STEALTH_ROCK))
    {
        u8 denominator, mod1 = ReturnTypeEffectiveness(gBattleMoves[MOVE_STEALTH_ROCK].type, gBattleMons[gActiveBattler].type1), mod2;
        
        if (gBattleMons[gActiveBattler].type1 == gBattleMons[gActiveBattler].type2)
            mod2 = 10;
        else
            mod2 = ReturnTypeEffectiveness(gBattleMoves[MOVE_STEALTH_ROCK].type, gBattleMons[gActiveBattler].type2);
        
        denominator = CalcStealthRockDenominator(mod1, mod2);;

        gSideStatuses[GetBattlerSide(gActiveBattler)] |= SIDE_STATUS_STEALTH_ROCK_DAMAGED;

        if (denominator)        
            gBattleMoveDamage = gBattleMons[gActiveBattler].maxHP / denominator;
        else
            gBattleMoveDamage = 0;

        if (gBattleMoveDamage)
        {
            gBattleMons[gActiveBattler].status2 &= ~(STATUS2_DESTINY_BOND);
            gHitMarker &= ~(HITMARKER_DESTINYBOND);

            gBattleStruct->scriptingActive = gActiveBattler;
            BattleScriptPushCursor();

            if (T2_READ_8(gBattlescriptCurrInstr + 1) == 0)
                gBattlescriptCurrInstr = BattleScript_StealthRockOnTarget;
            else if (T2_READ_8(gBattlescriptCurrInstr + 1) == 1)
                gBattlescriptCurrInstr = BattleScript_StealthRockOnAttacker;
            else
                gBattlescriptCurrInstr = BattleScript_StealthRockOngBank1;
        }
    }
    // then toxic spikes
    else if (!(gSideStatuses[GetBattlerSide(gActiveBattler)] & SIDE_STATUS_TOXIC_SPIKES_DAMAGED)
        && (gSideStatuses[GetBattlerSide(gActiveBattler)] & SIDE_STATUS_TOXIC_SPIKES)
        && ((gBattleMons[gActiveBattler].type1 != TYPE_FLYING 
          && gBattleMons[gActiveBattler].type2 != TYPE_FLYING 
          && GetBattlerAbility(gActiveBattler) != ABILITY_LEVITATE
          && !gDisableStructs[gActiveBattler].magnetRiseTimer)
        || ItemId_GetHoldEffect(gBattleMons[gBattlerTarget].item) == HOLD_EFFECT_IRON_BALL 
        || gBattleGlobalTimers.gravityTimer))
    {
        gSideStatuses[GetBattlerSide(gActiveBattler)] |= SIDE_STATUS_TOXIC_SPIKES_DAMAGED;
        if (IS_BATTLER_OF_TYPE(gActiveBattler, TYPE_POISON)) // Absorb the toxic spikes.
        {
            gSideStatuses[GetBattlerSide(gActiveBattler)] &= ~(SIDE_STATUS_TOXIC_SPIKES);
            gSideTimers[GetBattlerSide(gActiveBattler)].toxicSpikesAmount = 0;
            gBattleStruct->scriptingActive = gActiveBattler;
            BattleScriptPushCursor();
            gBattlescriptCurrInstr = BattleScript_ToxicSpikesAbsorbed;
        }
        else
        {
            if (!(gBattleMons[gActiveBattler].status1 & STATUS1_ANY)
                && !IS_BATTLER_OF_TYPE(gActiveBattler, TYPE_STEEL)
                && GetBattlerAbility(gActiveBattler) != ABILITY_IMMUNITY
                && !(gSideStatuses[GetBattlerSide(gActiveBattler)] & SIDE_STATUS_SAFEGUARD))
            {
                if (gSideTimers[GetBattlerSide(gActiveBattler)].toxicSpikesAmount >= 2)
                {
                    gBattleMons[gActiveBattler].status1 |= STATUS1_TOXIC_POISON;
                    gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                }
                else
                {
                    gBattleMons[gActiveBattler].status1 |= STATUS1_POISON;
                    gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                }

                BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
                MarkBattlerForControllerExec(gActiveBattler);
                gBattleStruct->scriptingActive = gActiveBattler;
                BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_ToxicSpikesPoisoned;
            }
        }
    }
    else
    {
        if (GetBattlerAbility(gActiveBattler) == ABILITY_TRUANT)
        {
            gDisableStructs[gActiveBattler].truantCounter = 1;
        }

        if (AbilityBattleEffects(0, gActiveBattler, 0, 0, 0) == 0 && ItemBattleEffects(0, gActiveBattler, 0) == 0)
        {
            gSideStatuses[GetBattlerSide(gActiveBattler)] &= ~(SIDE_STATUS_SPIKES_DAMAGED | SIDE_STATUS_TOXIC_SPIKES_DAMAGED | SIDE_STATUS_STEALTH_ROCK_DAMAGED);

            for (i = 0; i < gBattlersCount; i++)
            {
                if (gBattlerByTurnOrder[i] == gActiveBattler)
                    gActionsByTurnOrder[i] = 0xC;
            }

            for (i = 0; i < gBattlersCount; i++)
            {
                *(HP_ON_SWITCHOUT + GetBattlerSide(i)) = gBattleMons[i].hp;
            }

            if (T2_READ_8(gBattlescriptCurrInstr + 1) == 5)
            {
                u32 hitmark = gHitMarker >> 0x1C;
                gBank1++;
                while (1)
                {
                    if (hitmark & gBitTable[gBank1] && !(gAbsentBattlerFlags & gBitTable[gBank1]))
                        break;
                    if (gBank1 >= gBattlersCount)
                        break;
                    gBank1++;
                }
            }
            gBattlescriptCurrInstr += 2;
        }
    }
}

static void atk53_trainerslidein(void)
{
    if (!T2_READ_8(gBattlescriptCurrInstr + 1))
        gActiveBattler = GetBattlerAtPosition(0);
    else
        gActiveBattler = GetBattlerAtPosition(1);

    BtlController_EmitTrainerSlide(0);
    MarkBattlerForControllerExec(gActiveBattler);
    gBattlescriptCurrInstr += 2;
}

static void atk54_playse(void)
{
    gActiveBattler = gBattlerAttacker;
    BtlController_EmitPlaySE(0, T2_READ_16(gBattlescriptCurrInstr + 1));
    MarkBattlerForControllerExec(gActiveBattler);
    gBattlescriptCurrInstr += 3;
}

static void atk55_fanfare(void)
{
    gActiveBattler = gBattlerAttacker;
    BtlController_EmitPlayFanfareOrBGM(0, T2_READ_16(gBattlescriptCurrInstr + 1));
    MarkBattlerForControllerExec(gActiveBattler);
    gBattlescriptCurrInstr += 3;
}

static void atk56_playfaintcry(void)
{
    gActiveBattler = GetBattlerForBattleScript(T2_READ_8(gBattlescriptCurrInstr + 1));
    BtlController_EmitFaintingCry(0);
    MarkBattlerForControllerExec(gActiveBattler);
    gBattlescriptCurrInstr += 2;
}

static void atk57(void)
{
    gActiveBattler = GetBattlerAtPosition(0);
    BtlController_EmitCmd55(0, gBattleOutcome);
    MarkBattlerForControllerExec(gActiveBattler);
    gBattlescriptCurrInstr += 1;
}

static void atk58_returntoball(void)
{
    gActiveBattler = GetBattlerForBattleScript(T2_READ_8(gBattlescriptCurrInstr + 1));
    BtlController_EmitReturnMonToBall(0, 1);
    MarkBattlerForControllerExec(gActiveBattler);
    gBattlescriptCurrInstr += 2;
}

void atk59_handlelearnnewmove(void)
{
    u8* loc1 = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    u8* loc2 = T1_READ_PTR(gBattlescriptCurrInstr + 5);

    u16 ret = MonTryLearningNewMove(&gPlayerParty[gBattleStruct->expGetterMonId], T2_READ_8(gBattlescriptCurrInstr + 9));
    while (ret == 0xFFFE)
        ret = MonTryLearningNewMove(&gPlayerParty[gBattleStruct->expGetterMonId], 0);

    if (ret == 0)
    {
        gBattlescriptCurrInstr = loc2;
    }
    else if (ret == 0xFFFF)
    {
        gBattlescriptCurrInstr += 10;
    }
    else
    {
        gActiveBattler = GetBattlerAtPosition(0);
        if (gBattlerPartyIndexes[gActiveBattler] == gBattleStruct->expGetterMonId && !(gBattleMons[gActiveBattler].status2 & STATUS2_TRANSFORMED))
            GiveMoveToBattleMon(&gBattleMons[gActiveBattler], ret);
        if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE) //what is else if
        {
            gActiveBattler = GetBattlerAtPosition(2);
            if (gBattlerPartyIndexes[gActiveBattler] == gBattleStruct->expGetterMonId && !(gBattleMons[gActiveBattler].status2 & STATUS2_TRANSFORMED))
                GiveMoveToBattleMon(&gBattleMons[gActiveBattler], ret);
        }
        gBattlescriptCurrInstr = loc1;
    }
}

void sub_8023A80(void)
{
    sub_802BBD4(0x18, 8, 0x1D, 0xD, 0);
    Text_InitWindow(&gUnknown_03004210, BattleText_YesNo, 0x100, 0x19, 0x9);
    Text_PrintWindow8002F44(&gUnknown_03004210);
    MenuCursor_Create814A5C0(0, 0xFFFF, 0xC, 0x2D9F, 0x20);
}

void sub_8023AD8(void)
{
    sub_802BBD4(0x18, 8, 0x1D, 0xD, 1);
    DestroyMenuCursor();
}

static void atk5A_yesnoboxlearnmove(void)
{
    gActiveBattler = 0;
    switch (gBattleStruct->atk5A_StateTracker)
    {
    case 0:
        sub_8023A80();
        gBattleStruct->atk5A_StateTracker++;
        gBattleCommunication[1] = 0;
        sub_802BC6C();
        break;
    case 1:
        if (gMain.newKeys & DPAD_UP && gBattleCommunication[1] != 0)
        {
            PlaySE(SE_SELECT);
            nullsub_6();
            gBattleCommunication[1] = 0;
            sub_802BC6C();
        }
        if (gMain.newKeys & DPAD_DOWN && gBattleCommunication[1] == 0)
        {
            PlaySE(SE_SELECT);
            nullsub_6();
            gBattleCommunication[1] = 1;
            sub_802BC6C();
        }
        if (gMain.newKeys & A_BUTTON)
        {
            PlaySE(SE_SELECT);
            if (gBattleCommunication[1] == 0)
            {
                sub_8023AD8();
                BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB(0, 0, 0));
                gBattleStruct->atk5A_StateTracker++;
                return;
            }
            goto state_tracker_4;
        }
        else if (gMain.newKeys & B_BUTTON)
        {
            PlaySE(SE_SELECT);
          state_tracker_4:
            gBattleStruct->atk5A_StateTracker = 4;
        }
        break;
    case 2:
        if (!gPaletteFade.active)
        {
            ShowSelectMovePokemonSummaryScreen(gPlayerParty, gBattleStruct->expGetterMonId, gPlayerPartyCount - 1, ReshowBattleScreenAfterMenu, gMoveToLearn);
            gBattleStruct->atk5A_StateTracker++;
        }
        break;
    case 3:
        if (!gPaletteFade.active && gMain.callback2 == BattleMainCB2)
        {
            u8 move_pos = sub_809FA30();
            if (move_pos == 4)
            {
                gBattleStruct->atk5A_StateTracker = 4;
            }
            else
            {
                u16 move = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_MOVE1 + move_pos);
                if (IsHMMove2(move))
                {
                    PrepareStringBattle(0x13F, gActiveBattler);
                    gBattleStruct->atk5A_StateTracker = 5;
                }
                else
                {
                    u8 *ptr;

                    gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
                    ptr = gBattleTextBuff2;
                    {
                        ptr[0] = 0xFD;
                        ptr[1] = 2;
                        ptr[2] = move;
                        ptr[3] = ((move & 0xFF00) >> 8);
                        ptr += 4;
                    }
                    ptr[0] = 0xFF;
                    RemoveMonPPBonus(&gPlayerParty[gBattleStruct->expGetterMonId], move_pos);
                    SetMonMoveSlot(&gPlayerParty[gBattleStruct->expGetterMonId], gMoveToLearn, move_pos);
                    if (gBattlerPartyIndexes[0] == gBattleStruct->expGetterMonId && !(gBattleMons[0].status2 & STATUS2_TRANSFORMED)
                        && !(gDisableStructs[0].mimickedMoves & gBitTable[move_pos]))
                    {
                        RemoveBattleMonPPBonus(&gBattleMons[0], move_pos);
                        SetBattleMonMoveSlot(&gBattleMons[0], gMoveToLearn, move_pos);
                    }
                    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && gBattlerPartyIndexes[2] == gBattleStruct->expGetterMonId && !(gBattleMons[2].status2 & STATUS2_TRANSFORMED)
                        && !(gDisableStructs[2].mimickedMoves & gBitTable[move_pos]))
                    {
                        RemoveBattleMonPPBonus(&gBattleMons[2], move_pos);
                        SetBattleMonMoveSlot(&gBattleMons[2], gMoveToLearn, move_pos);
                    }
                }
            }
        }
        break;
    case 4:
        sub_8023AD8();
        gBattlescriptCurrInstr += 5;
        break;
    case 5:
        if (gBattleControllerExecFlags == 0)
        {
            gBattleStruct->atk5A_StateTracker = 2;
        }
        break;
    }
}

static void atk5B_yesnoboxstoplearningmove(void)
{
    switch (gBattleStruct->atk5A_StateTracker)
    {
    case 0:
        sub_8023A80();
        gBattleStruct->atk5A_StateTracker++;
        gBattleCommunication[1] = 0;
        sub_802BC6C();
        break;
    case 1:
        if (gMain.newKeys & DPAD_UP && gBattleCommunication[1] != 0)
        {
            PlaySE(SE_SELECT);
            nullsub_6();
            gBattleCommunication[1] = 0;
            sub_802BC6C();
        }
        if (gMain.newKeys & DPAD_DOWN && gBattleCommunication[1] == 0)
        {
            PlaySE(SE_SELECT);
            nullsub_6();
            gBattleCommunication[1] = 1;
            sub_802BC6C();
        }
        if (gMain.newKeys & A_BUTTON)
        {
            PlaySE(SE_SELECT);
            if (gBattleCommunication[1] != 0)
                gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
            else
                gBattlescriptCurrInstr += 5;
            sub_8023AD8();
        }
        else if (gMain.newKeys & B_BUTTON)
        {
            PlaySE(SE_SELECT);
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
            sub_8023AD8();
        }
    }
}

static void atk5C_hitanimation(void)
{
    gActiveBattler = GetBattlerForBattleScript(T2_READ_8(gBattlescriptCurrInstr + 1));
    if (gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
        gBattlescriptCurrInstr += 2;
    else if (!(gHitMarker & HITMARKER_IGNORE_SUBSTITUTE) || !(gBattleMons[gActiveBattler].status2 & STATUS2_SUBSTITUTE) || gDisableStructs[gActiveBattler].substituteHP == 0)
    {
        BtlController_EmitHitAnimation(0);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattlescriptCurrInstr += 2;
    }
    else
        gBattlescriptCurrInstr += 2;
}

#define MONEY_UNKNOWN ((*(u8*)(ewram_addr + 0x17000 + 0x94)))

#ifdef NONMATCHING
static void atk5D_getmoneyreward(void)
{
    int i = 0;
    u8 r5 = 0;
    u32 money_to_give;
    if (gTrainerBattleOpponent == SECRET_BASE_OPPONENT)
    {
        money_to_give = 2 * gBattleStruct->moneyMultiplier * MONEY_UNKNOWN;
    }
    else
    {
        switch(gTrainers[gTrainerBattleOpponent].partyFlags)
        {
        case 0:
            {
                const struct TrainerMonNoItemDefaultMoves *data = gTrainers[gTrainerBattleOpponent].party.NoItemDefaultMoves;
                r5 = data[gTrainers[gTrainerBattleOpponent].partySize - 1].level;
            }
            break;
        case 2:
            {
                const struct TrainerMonItemDefaultMoves *data = gTrainers[gTrainerBattleOpponent].party.ItemDefaultMoves;
                r5 = data[gTrainers[gTrainerBattleOpponent].partySize - 1].level;
            }
            break;
        case 1:
        case 3:
            {
                const struct TrainerMonItemCustomMoves *data = gTrainers[gTrainerBattleOpponent].party.ItemCustomMoves;
                r5 = data[gTrainers[gTrainerBattleOpponent].partySize - 1].level;
            }
            break;
        }
        for (; gTrainerMoney[i * 4] != 0xFF && gTrainerMoney[i * 4 + 1] != gTrainers[gTrainerBattleOpponent].trainerClass ; i++) {}

        money_to_give = (r5 << 2) * gBattleStruct->moneyMultiplier;
        if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
            money_to_give = 2 * gTrainerMoney[i * 4 + 1] * money_to_give;
        else
            money_to_give = 1 * gTrainerMoney[i * 4 + 1] * money_to_give;
    }

    AddMoney(&gSaveBlock1.money, money_to_give);
    gBattleTextBuff1[0] = 0xFD;
    gBattleTextBuff1[1] = 1;
    gBattleTextBuff1[2] = 4;
    gBattleTextBuff1[3] = 5;
    T2_WRITE_32(&gBattleTextBuff1[4], money_to_give);
    gBattleTextBuff1[8] = 0xFF;

    gBattlescriptCurrInstr += 1;
}
#else
NAKED
static void atk5D_getmoneyreward(void)
{
    asm(".syntax unified\n\
        push {r4-r7,lr}\n\
    mov r7, r8\n\
    push {r7}\n\
    movs r6, 0\n\
    movs r5, 0\n\
    ldr r0, _08024048 @ =gTrainerBattleOpponent\n\
    ldrh r2, [r0]\n\
    movs r1, 0x80\n\
    lsls r1, 3\n\
    cmp r2, r1\n\
    bne _08024058\n\
    ldr r0, _0802404C @ =gSharedMem + 0x17000\n\
    adds r1, r0, 0\n\
    adds r1, 0x94\n\
    ldrb r2, [r1]\n\
    ldr r1, _08024050 @ =0xfffff056\n\
    adds r0, r1\n\
    ldrb r1, [r0]\n\
    lsls r0, r1, 2\n\
    adds r0, r1\n\
    lsls r0, 2\n\
    adds r4, r2, 0\n\
    muls r4, r0\n\
    ldr r0, _08024054 @ =gSaveBlock1 + 0x490\n\
    mov r8, r0\n\
    b _08024140\n\
    .align 2, 0\n\
_08024048: .4byte gTrainerBattleOpponent\n\
_0802404C: .4byte gSharedMem + 0x17000\n\
_08024050: .4byte 0xfffff056\n\
_08024054: .4byte gSaveBlock1 + 0x490\n\
_08024058:\n\
    ldr r2, _08024074 @ =gTrainers\n\
    ldrh r1, [r0]\n\
    lsls r0, r1, 2\n\
    adds r0, r1\n\
    lsls r3, r0, 3\n\
    adds r4, r3, r2\n\
    ldrb r1, [r4]\n\
    cmp r1, 0x1\n\
    beq _080240AE\n\
    cmp r1, 0x1\n\
    bgt _08024078\n\
    cmp r1, 0\n\
    beq _08024082\n\
    b _080240C4\n\
    .align 2, 0\n\
_08024074: .4byte gTrainers\n\
_08024078:\n\
    cmp r1, 0x2\n\
    beq _08024098\n\
    cmp r1, 0x3\n\
    beq _080240AE\n\
    b _080240C4\n\
_08024082:\n\
    adds r0, r2, 0\n\
    adds r0, 0x24\n\
    adds r0, r3, r0\n\
    ldr r1, [r0]\n\
    adds r0, r4, 0\n\
    adds r0, 0x20\n\
    ldrb r0, [r0]\n\
    lsls r0, 3\n\
    adds r0, r1\n\
    subs r0, 0x8\n\
    b _080240C2\n\
_08024098:\n\
    adds r0, r2, 0\n\
    adds r0, 0x24\n\
    adds r0, r3, r0\n\
    ldr r1, [r0]\n\
    adds r0, r4, 0\n\
    adds r0, 0x20\n\
    ldrb r0, [r0]\n\
    lsls r0, 3\n\
    adds r0, r1\n\
    subs r0, 0x8\n\
    b _080240C2\n\
_080240AE:\n\
    adds r0, r2, 0\n\
    adds r0, 0x24\n\
    adds r0, r3, r0\n\
    ldr r1, [r0]\n\
    adds r0, r4, 0\n\
    adds r0, 0x20\n\
    ldrb r0, [r0]\n\
    lsls r0, 4\n\
    adds r0, r1\n\
    subs r0, 0x10\n\
_080240C2:\n\
    ldrb r5, [r0, 0x2]\n\
_080240C4:\n\
    ldr r0, _08024120 @ =gTrainerMoney\n\
    lsls r1, r6, 2\n\
    adds r3, r1, r0\n\
    ldrb r1, [r3]\n\
    mov r12, r0\n\
    lsls r4, r5, 2\n\
    ldr r5, _08024124 @ =gSharedMem\n\
    ldr r7, _08024128 @ =gBattleTypeFlags\n\
    ldr r0, _0802412C @ =gSaveBlock1 + 0x490\n\
    mov r8, r0\n\
    cmp r1, 0xFF\n\
    beq _080240FE\n\
    ldr r2, _08024130 @ =gTrainers\n\
    ldr r0, _08024134 @ =gTrainerBattleOpponent\n\
    ldrh r1, [r0]\n\
    lsls r0, r1, 2\n\
    adds r0, r1\n\
    lsls r0, 3\n\
    adds r0, r2\n\
    ldrb r2, [r0, 0x1]\n\
    adds r1, r3, 0\n\
_080240EE:\n\
    ldrb r0, [r1]\n\
    cmp r0, r2\n\
    beq _080240FE\n\
    adds r1, 0x4\n\
    adds r6, 0x1\n\
    ldrb r0, [r1]\n\
    cmp r0, 0xFF\n\
    bne _080240EE\n\
_080240FE:\n\
    ldr r1, _08024138 @ =0x00016056\n\
    adds r0, r5, r1\n\
    ldrb r0, [r0]\n\
    adds r3, r4, 0\n\
    muls r3, r0\n\
    lsls r0, r6, 2\n\
    add r0, r12\n\
    ldrb r2, [r0, 0x1]\n\
    ldrh r1, [r7]\n\
    movs r0, 0x1\n\
    ands r0, r1\n\
    cmp r0, 0\n\
    beq _0802413C\n\
    lsls r0, r2, 1\n\
    adds r4, r3, 0\n\
    muls r4, r0\n\
    b _08024140\n\
    .align 2, 0\n\
_08024120: .4byte gTrainerMoney\n\
_08024124: .4byte gSharedMem\n\
_08024128: .4byte gBattleTypeFlags\n\
_0802412C: .4byte gSaveBlock1 + 0x490\n\
_08024130: .4byte gTrainers\n\
_08024134: .4byte gTrainerBattleOpponent\n\
_08024138: .4byte 0x00016056\n\
_0802413C:\n\
    adds r4, r3, 0\n\
    muls r4, r2\n\
_08024140:\n\
    mov r0, r8\n\
    adds r1, r4, 0\n\
    bl AddMoney\n\
    ldr r1, _0802418C @ =gBattleTextBuff1\n\
    movs r0, 0xFD\n\
    strb r0, [r1]\n\
    movs r0, 0x1\n\
    strb r0, [r1, 0x1]\n\
    movs r0, 0x4\n\
    strb r0, [r1, 0x2]\n\
    movs r0, 0x5\n\
    strb r0, [r1, 0x3]\n\
    strb r4, [r1, 0x4]\n\
    movs r0, 0xFF\n\
    lsls r0, 8\n\
    ands r0, r4\n\
    lsrs r0, 8\n\
    strb r0, [r1, 0x5]\n\
    movs r0, 0xFF\n\
    lsls r0, 16\n\
    ands r0, r4\n\
    lsrs r0, 16\n\
    strb r0, [r1, 0x6]\n\
    lsrs r0, r4, 24\n\
    strb r0, [r1, 0x7]\n\
    movs r0, 0xFF\n\
    strb r0, [r1, 0x8]\n\
    ldr r1, _08024190 @ =gBattlescriptCurrInstr\n\
    ldr r0, [r1]\n\
    adds r0, 0x1\n\
    str r0, [r1]\n\
    pop {r3}\n\
    mov r8, r3\n\
    pop {r4-r7}\n\
    pop {r0}\n\
    bx r0\n\
    .align 2, 0\n\
_0802418C: .4byte gBattleTextBuff1\n\
_08024190: .4byte gBattlescriptCurrInstr\n\
        .syntax divided");
}
#endif //NONMATCHING

/*
static u32 GetTrainerMoneyToGive(u16 trainerId)
{
    u32 i = 0;
    u32 lastMonLevel = 0;
    u32 moneyReward = 0;

    if (trainerId == SECRET_BASE_OPPONENT)
    {
        moneyReward = 20 * eSecretBaseRecord->partyLevels[0] * gBattleStruct->moneyMultiplier;
    }
    else
    {
        switch (gTrainers[trainerId].partyFlags)
        {
        case 0:
            {
                const struct TrainerMonNoItemDefaultMoves *party = gTrainers[trainerId].party.NoItemDefaultMoves;
                lastMonLevel = party[gTrainers[trainerId].partySize - 1].lvl;
            }
            break;
        case F_TRAINER_PARTY_CUSTOM_MOVESET:
            {
                const struct TrainerMonNoItemCustomMoves *party = gTrainers[trainerId].party.NoItemCustomMoves;
                lastMonLevel = party[gTrainers[trainerId].partySize - 1].lvl;
            }
            break;
        case F_TRAINER_PARTY_HELD_ITEM:
            {
                const struct TrainerMonItemDefaultMoves *party = gTrainers[trainerId].party.ItemDefaultMoves;
                lastMonLevel = party[gTrainers[trainerId].partySize - 1].lvl;
            }
            break;
        case F_TRAINER_PARTY_CUSTOM_MOVESET | F_TRAINER_PARTY_HELD_ITEM:
            {
                const struct TrainerMonItemCustomMoves *party = gTrainers[trainerId].party.ItemCustomMoves;
                lastMonLevel = party[gTrainers[trainerId].partySize - 1].lvl;
            }
            break;
        }

        for (; gTrainerMoneyTable[i].classId != 0xFF; i++)
        {
            if (gTrainerMoneyTable[i].classId == gTrainers[trainerId].trainerClass)
                break;
        }

        if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
            moneyReward = 4 * lastMonLevel * gBattleStruct->moneyMultiplier * 2 * gTrainerMoneyTable[i].value;
        else
            moneyReward = 4 * lastMonLevel * gBattleStruct->moneyMultiplier * gTrainerMoneyTable[i].value;
    }

    return moneyReward;
}

static void atk5D_getmoneyreward(void)
{
    u32 moneyReward = GetTrainerMoneyToGive(gTrainerBattleOpponent_A);
    if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
        moneyReward += GetTrainerMoneyToGive(gTrainerBattleOpponent_B);

    AddMoney(&gSaveBlock1Ptr->money, moneyReward);

    PREPARE_WORD_NUMBER_BUFFER(gBattleTextBuff1, 5, moneyReward)

    gBattlescriptCurrInstr++;
}
*/

static void atk5E(void)
{
    gActiveBattler = GetBattlerForBattleScript(T2_READ_8(gBattlescriptCurrInstr + 1));
    switch (gBattleCommunication[0])
    {
    case 0:
        BtlController_EmitGetMonData(0, REQUEST_ALL_BATTLE, 0);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattleCommunication[0]++;
        break;
    case 1:
         if (gBattleControllerExecFlags == 0)
         {
            int i;
            struct BattlePokemon* bufferPoke = (struct BattlePokemon*) &gBattleBufferB[gActiveBattler][4];
            for (i = 0; i < 4; i++)
            {
                gBattleMons[gActiveBattler].moves[i] = bufferPoke->moves[i];
                gBattleMons[gActiveBattler].pp[i] = bufferPoke->pp[i];
            }
            gBattlescriptCurrInstr += 2;
         }
         break;
    }
}

static void atk5F_swapattackerwithtarget(void)
{
    gActiveBattler = gBattlerAttacker;
    gBattlerAttacker = gBattlerTarget;
    gBattlerTarget = gActiveBattler;
    //what is xor...
    if (gHitMarker & HITMARKER_SWAP_ATTACKER_TARGET)
        gHitMarker &= ~(HITMARKER_SWAP_ATTACKER_TARGET);
    else
        gHitMarker |= HITMARKER_SWAP_ATTACKER_TARGET;
    gBattlescriptCurrInstr++;
}

static void atk60_incrementgamestat(void)
{
    if (GetBattlerSide(gBattlerAttacker) == 0)
    {
        IncrementGameStat(T2_READ_8(gBattlescriptCurrInstr + 1));
    }
    gBattlescriptCurrInstr += 2;
}

static void atk61_drawpartystatussummary(void)
{
    int i;
    struct Pokemon* party;
    struct HpAndStatus hpStatus[6];
    if (gBattleControllerExecFlags)
        return;

    gActiveBattler = GetBattlerForBattleScript(T2_READ_8(gBattlescriptCurrInstr + 1));
    if (GetBattlerSide(gActiveBattler) == 0)
        party = gPlayerParty;
    else
        party = gEnemyParty;

    for (i = 0; i < 6; i++)
    {
        if (GetMonData(&party[i], MON_DATA_SPECIES2) == 0 || GetMonData(&party[i], MON_DATA_SPECIES2) == SPECIES_EGG)
        {
            hpStatus[i].hp = 0xFFFF;
            hpStatus[i].status = 0;
        }
        else
        {
            hpStatus[i].hp = GetMonData(&party[i], MON_DATA_HP);
            hpStatus[i].status = GetMonData(&party[i], MON_DATA_STATUS);
        }
    }
    BtlController_EmitDrawPartyStatusSummary(0, hpStatus, 1);
    MarkBattlerForControllerExec(gActiveBattler);
    gBattlescriptCurrInstr += 2;
}

static void atk62_hidepartystatussummary(void)
{
    gActiveBattler = GetBattlerForBattleScript(T2_READ_8(gBattlescriptCurrInstr + 1));
    BtlController_EmitHidePartyStatusSummary(0);
    MarkBattlerForControllerExec(gActiveBattler);
    gBattlescriptCurrInstr += 2;
}

static void atk63_jumptorandomattack(void)
{
    if (T2_READ_8(gBattlescriptCurrInstr + 1))
        gCurrentMove = gRandomMove;
    else
        gChosenMove = gCurrentMove = gRandomMove;
    gBattlescriptCurrInstr = gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect];
}

static void atk64_statusanimation(void)
{
    if (gBattleControllerExecFlags == 0)
    {
        gActiveBattler = GetBattlerForBattleScript(T2_READ_8(gBattlescriptCurrInstr + 1));
        if (!(gStatuses3[gActiveBattler] & STATUS3_SEMI_INVULNERABLE) && gDisableStructs[gActiveBattler].substituteHP == 0 && !(gHitMarker & HITMARKER_NO_ANIMATIONS))
        {
            BtlController_EmitStatusAnimation(0, 0, gBattleMons[gActiveBattler].status1);
            MarkBattlerForControllerExec(gActiveBattler);
        }
        gBattlescriptCurrInstr += 2;
    }
}

static void atk65_status2animation(void)
{
    if (gBattleControllerExecFlags == 0)
    {
        u32 possible_to_anim;
        gActiveBattler = GetBattlerForBattleScript(T1_READ_8(gBattlescriptCurrInstr + 1));
        possible_to_anim = T1_READ_32(gBattlescriptCurrInstr + 2);
        if (!(gStatuses3[gActiveBattler] & STATUS3_SEMI_INVULNERABLE) && gDisableStructs[gActiveBattler].substituteHP == 0 && !(gHitMarker & HITMARKER_NO_ANIMATIONS))
        {
            BtlController_EmitStatusAnimation(0, 1, gBattleMons[gActiveBattler].status2 & possible_to_anim);
            MarkBattlerForControllerExec(gActiveBattler);
        }
        gBattlescriptCurrInstr += 6;
    }
}

static void atk66_chosenstatusanimation(void)
{
    if (gBattleControllerExecFlags == 0)
    {
        u32 status;
        gActiveBattler = GetBattlerForBattleScript(T1_READ_8(gBattlescriptCurrInstr + 1));
        status = T1_READ_32(gBattlescriptCurrInstr + 3);
        if (!(gStatuses3[gActiveBattler] & STATUS3_SEMI_INVULNERABLE) && gDisableStructs[gActiveBattler].substituteHP == 0 && !(gHitMarker & HITMARKER_NO_ANIMATIONS))
        {
            BtlController_EmitStatusAnimation(0, T1_READ_8(gBattlescriptCurrInstr + 2), status);
            MarkBattlerForControllerExec(gActiveBattler);
        }
        gBattlescriptCurrInstr += 7;
    }
}

static void atk67_yesnobox(void)
{
    switch (gBattleCommunication[0])
    {
    case 0:
        sub_8023A80();
        gBattleCommunication[0]++;
        gBattleCommunication[1] = 0;
        sub_802BC6C();
        break;
    case 1:
        if (gMain.newKeys & DPAD_UP && gBattleCommunication[1] != 0)
        {
            PlaySE(SE_SELECT);
            nullsub_6();
            gBattleCommunication[1] = 0;
            sub_802BC6C();
        }
        if (gMain.newKeys & DPAD_DOWN && gBattleCommunication[1] == 0)
        {
            PlaySE(SE_SELECT);
            nullsub_6();
            gBattleCommunication[1] = 1;
            sub_802BC6C();
        }
        if (gMain.newKeys & B_BUTTON)
        {
            gBattleCommunication[1] = 1;
            PlaySE(SE_SELECT);
            sub_8023AD8();
            gBattlescriptCurrInstr++;
        }
        else if (gMain.newKeys & A_BUTTON)
        {
            PlaySE(SE_SELECT);
            sub_8023AD8();
            gBattlescriptCurrInstr++;
        }
        break;
    }
}

static void atk68_cancelallactions(void)
{
    int i;
    for (i = 0; i < gBattlersCount; i++)
    {
        gActionsByTurnOrder[i] = 0xC;
    }
    gBattlescriptCurrInstr++;
}

static void atk69_adjustsetdamage(void) //literally a copy of atk07 except theres no rand dmg modifier...
{
    u8 holdEffect, param;
    if (gBattleMons[gBattlerTarget].item == ITEM_ENIGMA_BERRY)
        holdEffect = gEnigmaBerries[gBattlerTarget].holdEffect, param = gEnigmaBerries[gBattlerTarget].holdEffectParam;
    else
    {
        holdEffect = ItemId_GetHoldEffect(gBattleMons[gBattlerTarget].item);
        param = ItemId_GetHoldEffectParam(gBattleMons[gBattlerTarget].item);
    }

    gPotentialItemEffectBattler = gBattlerTarget;

    if ((holdEffect == HOLD_EFFECT_FOCUS_BAND && (Random() % 100) < param)
        || (holdEffect == HOLD_EFFECT_FOCUS_SASH && gBattleMons[gBattlerTarget].hp == gBattleMons[gBattlerTarget].maxHP && !gSpecialStatuses[gBattlerTarget].focusSashed))
    {
        RecordItemEffectBattle(gBattlerTarget, holdEffect);
        gSpecialStatuses[gBattlerTarget].focusBanded = 1;
    }
    if (gBattleMons[gBattlerTarget].status2 & STATUS2_SUBSTITUTE
        || gSpecialStatuses[gBattlerTarget].focusSashed)
        goto END;

    if (holdEffect == HOLD_EFFECT_FOCUS_SASH && gBattleMons[gBattlerTarget].hp == gBattleMons[gBattlerTarget].maxHP && !gSpecialStatuses[gBattlerTarget].focusSashed)
        gSpecialStatuses[gBattlerTarget].focusSashed = 1;

    if (gBattleMoves[gCurrentMove].effect != EFFECT_FALSE_SWIPE && !gProtectStructs[gBattlerTarget].endured
     && !gSpecialStatuses[gBattlerTarget].focusBanded)
        goto END;

    if (gBattleMons[gBattlerTarget].hp > gBattleMoveDamage)
        goto END;

    gBattleMoveDamage = gBattleMons[gBattlerTarget].hp - 1;

    if (gProtectStructs[gBattlerTarget].endured)
    {
        gMoveResultFlags |= MOVE_RESULT_FOE_ENDURED;
        goto END;
    }

    if (gSpecialStatuses[gBattlerTarget].focusBanded)
    {
        gMoveResultFlags |= MOVE_RESULT_FOE_HUNG_ON;
        gLastUsedItem = gBattleMons[gBattlerTarget].item;
    }

    END:
        gBattlescriptCurrInstr++;
}

void atk6A_removeitem(void)
{
    gActiveBattler = GetBattlerForBattleScript(T2_READ_8(gBattlescriptCurrInstr + 1));

    //if (ItemId_GetHoldEffect(gBattleMons[gActiveBattler].item) == HOLD_EFFECT_AIR_BALLOON)
        USED_HELD_ITEMS(gActiveBattler) = gBattleMons[gActiveBattler].item;

    if (gBattleMons[gActiveBattler].item)
        gDisableStructs[gActiveBattler].unburden = TRUE;

    gBattleMons[gActiveBattler].item = 0;
    BtlController_EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gActiveBattler].item);
    MarkBattlerForControllerExec(gActiveBattler);
    gBattlescriptCurrInstr += 2;
}

static void atk6B_atknameinbuff1(void)
{
    gBattleTextBuff1[0] = 0xFD;
    gBattleTextBuff1[1] = 7;
    gBattleTextBuff1[2] = gBattlerAttacker;
    gBattleTextBuff1[3] = gBattlerPartyIndexes[gBattlerAttacker];
    gBattleTextBuff1[4] = 0xFF;
    gBattlescriptCurrInstr++;
}

#ifdef NONMATCHING
extern const u8 BattleText_Format2[];

// TODO: finish
static void atk6C_drawlvlupbox(void)
{
    UNUSED u8 r1 = 0;
    UNUSED u8 r7 = 0;
    switch (gBattleStruct->atk6C_statetracker)
    {
    case 0:
        sub_802BBD4(0xB, 0, 0x1D, 0x7, r1);
        StringCopy(gStringVar4, BattleText_Format2);

    }
}

#else
NAKED
static void atk6C_drawlvlupbox(void)
{
    asm(".syntax unified\n\
    push {r4-r7,lr}\n\
    mov r7, r10\n\
    mov r6, r9\n\
    mov r5, r8\n\
    push {r5-r7}\n\
    sub sp, 0x4\n\
    movs r1, 0\n\
    movs r7, 0\n\
    ldr r0, _08024928 @ =gSharedMem\n\
    mov r10, r0\n\
    ldr r4, _0802492C @ =0x0001609c\n\
    add r4, r10\n\
    ldrb r0, [r4]\n\
    cmp r0, 0x1\n\
    bne _0802491C\n\
    b _08024AF4\n\
_0802491C:\n\
    cmp r0, 0x1\n\
    bgt _08024930\n\
    cmp r0, 0\n\
    beq _0802493E\n\
    b _08024C38\n\
    .align 2, 0\n\
_08024928: .4byte gSharedMem\n\
_0802492C: .4byte 0x0001609c\n\
_08024930:\n\
    cmp r0, 0x2\n\
    bne _08024936\n\
    b _08024C04\n\
_08024936:\n\
    cmp r0, 0x3\n\
    bne _0802493C\n\
    b _08024C30\n\
_0802493C:\n\
    b _08024C38\n\
_0802493E:\n\
    str r1, [sp]\n\
    movs r0, 0xB\n\
    movs r1, 0\n\
    movs r2, 0x1D\n\
    movs r3, 0x7\n\
    bl sub_802BBD4\n\
    ldr r0, _0802499C @ =gStringVar4\n\
    ldr r1, _080249A0 @ =BattleText_Format2\n\
    bl StringCopy\n\
    adds r5, r0, 0\n\
    movs r1, 0\n\
    mov r8, r1\n\
_0802495A:\n\
    movs r2, 0\n\
    mov r9, r2\n\
    ldr r0, _080249A4 @ =gUnknown_0840165C\n\
    mov r1, r8\n\
    lsls r4, r1, 2\n\
    adds r0, r4, r0\n\
    ldr r1, [r0]\n\
    adds r0, r5, 0\n\
    bl StringAppend\n\
    adds r5, r0, 0\n\
    ldr r0, _080249A8 @ =gSharedMem\n\
    ldr r2, _080249AC @ =0x00016018\n\
    adds r0, r2\n\
    ldrb r1, [r0]\n\
    movs r0, 0x64\n\
    muls r0, r1\n\
    ldr r1, _080249B0 @ =gPlayerParty\n\
    adds r0, r1\n\
    ldr r1, _080249B4 @ =gLevelUpStatBoxStats\n\
    add r1, r8\n\
    ldrb r1, [r1]\n\
    bl GetMonData\n\
    lsls r0, 16\n\
    lsrs r1, r0, 16\n\
    mov r0, r8\n\
    cmp r0, 0x5\n\
    bhi _08024A1A\n\
    ldr r0, _080249B8 @ =_080249BC\n\
    adds r0, r4, r0\n\
    ldr r0, [r0]\n\
    mov pc, r0\n\
    .align 2, 0\n\
_0802499C: .4byte gStringVar4\n\
_080249A0: .4byte BattleText_Format2\n\
_080249A4: .4byte gUnknown_0840165C\n\
_080249A8: .4byte gSharedMem\n\
_080249AC: .4byte 0x00016018\n\
_080249B0: .4byte gPlayerParty\n\
_080249B4: .4byte gLevelUpStatBoxStats\n\
_080249B8: .4byte _080249BC\n\
    .align 2, 0\n\
_080249BC:\n\
    .4byte _080249D4\n\
    .4byte _080249E0\n\
    .4byte _080249EC\n\
    .4byte _080249F8\n\
    .4byte _08024A04\n\
    .4byte _08024A10\n\
_080249D4:\n\
    ldr r0, _080249DC @ =gSharedMem + 0x17180\n\
    ldrh r0, [r0]\n\
    b _08024A14\n\
    .align 2, 0\n\
_080249DC: .4byte gSharedMem + 0x17180\n\
_080249E0:\n\
    ldr r0, _080249E8 @ =gSharedMem + 0x17180\n\
    ldrh r0, [r0, 0x8]\n\
    b _08024A14\n\
    .align 2, 0\n\
_080249E8: .4byte gSharedMem + 0x17180\n\
_080249EC:\n\
    ldr r0, _080249F4 @ =gSharedMem + 0x17180\n\
    ldrh r0, [r0, 0x2]\n\
    b _08024A14\n\
    .align 2, 0\n\
_080249F4: .4byte gSharedMem + 0x17180\n\
_080249F8:\n\
    ldr r0, _08024A00 @ =gSharedMem + 0x17180\n\
    ldrh r0, [r0, 0xA]\n\
    b _08024A14\n\
    .align 2, 0\n\
_08024A00: .4byte gSharedMem + 0x17180\n\
_08024A04:\n\
    ldr r0, _08024A0C @ =gSharedMem + 0x17180\n\
    ldrh r0, [r0, 0x4]\n\
    b _08024A14\n\
    .align 2, 0\n\
_08024A0C: .4byte gSharedMem + 0x17180\n\
_08024A10:\n\
    ldr r0, _08024A54 @ =gSharedMem + 0x17180\n\
    ldrh r0, [r0, 0x6]\n\
_08024A14:\n\
    subs r0, r1, r0\n\
    lsls r0, 16\n\
    lsrs r7, r0, 16\n\
_08024A1A:\n\
    lsls r0, r7, 16\n\
    asrs r0, 16\n\
    cmp r0, 0\n\
    bge _08024A2C\n\
    negs r0, r0\n\
    lsls r0, 16\n\
    lsrs r7, r0, 16\n\
    movs r1, 0x1\n\
    add r9, r1\n\
_08024A2C:\n\
    movs r0, 0xFC\n\
    strb r0, [r5]\n\
    movs r0, 0x13\n\
    strb r0, [r5, 0x1]\n\
    movs r1, 0x1\n\
    mov r2, r8\n\
    ands r1, r2\n\
    lsls r0, r1, 3\n\
    adds r0, r1\n\
    adds r0, 0x5\n\
    lsls r0, 3\n\
    adds r0, 0x6\n\
    strb r0, [r5, 0x2]\n\
    adds r5, 0x3\n\
    mov r0, r9\n\
    cmp r0, 0\n\
    beq _08024A5C\n\
    ldr r1, _08024A58 @ =BattleText_Dash\n\
    b _08024A5E\n\
    .align 2, 0\n\
_08024A54: .4byte gSharedMem + 0x17180\n\
_08024A58: .4byte BattleText_Dash\n\
_08024A5C:\n\
    ldr r1, _08024AA4 @ =BattleText_Plus\n\
_08024A5E:\n\
    adds r0, r5, 0\n\
    bl StringCopy\n\
    adds r5, r0, 0\n\
    movs r6, 0xFC\n\
    strb r6, [r5]\n\
    movs r4, 0x14\n\
    strb r4, [r5, 0x1]\n\
    movs r0, 0x6\n\
    strb r0, [r5, 0x2]\n\
    adds r5, 0x3\n\
    lsls r1, r7, 16\n\
    asrs r1, 16\n\
    adds r0, r5, 0\n\
    movs r2, 0x1\n\
    movs r3, 0x2\n\
    bl ConvertIntToDecimalStringN\n\
    adds r5, r0, 0\n\
    strb r6, [r5]\n\
    strb r4, [r5, 0x1]\n\
    movs r0, 0\n\
    strb r0, [r5, 0x2]\n\
    adds r5, 0x3\n\
    movs r0, 0x1\n\
    mov r1, r8\n\
    ands r0, r1\n\
    cmp r0, 0\n\
    beq _08024AA8\n\
    movs r0, 0xFE\n\
    strb r0, [r5]\n\
    movs r0, 0xFF\n\
    strb r0, [r5, 0x1]\n\
    adds r5, 0x1\n\
    b _08024AB8\n\
    .align 2, 0\n\
_08024AA4: .4byte BattleText_Plus\n\
_08024AA8:\n\
    strb r6, [r5]\n\
    movs r0, 0x11\n\
    strb r0, [r5, 0x1]\n\
    movs r0, 0x8\n\
    strb r0, [r5, 0x2]\n\
    adds r5, 0x3\n\
    movs r0, 0xFF\n\
    strb r0, [r5]\n\
_08024AB8:\n\
    movs r2, 0x1\n\
    add r8, r2\n\
    mov r0, r8\n\
    cmp r0, 0x5\n\
    bgt _08024AC4\n\
    b _0802495A\n\
_08024AC4:\n\
    ldr r4, _08024AE4 @ =gUnknown_03004210\n\
    ldr r1, _08024AE8 @ =gStringVar4\n\
    adds r2, 0xFF\n\
    movs r0, 0x1\n\
    str r0, [sp]\n\
    adds r0, r4, 0\n\
    movs r3, 0xC\n\
    bl Text_InitWindow\n\
    adds r0, r4, 0\n\
    bl Text_PrintWindow8002F44\n\
    ldr r1, _08024AEC @ =gSharedMem\n\
    ldr r2, _08024AF0 @ =0x0001609c\n\
    adds r1, r2\n\
    b _08024BEA\n\
    .align 2, 0\n\
_08024AE4: .4byte gUnknown_03004210\n\
_08024AE8: .4byte gStringVar4\n\
_08024AEC: .4byte gSharedMem\n\
_08024AF0: .4byte 0x0001609c\n\
_08024AF4:\n\
    ldr r0, _08024B94 @ =gMain\n\
    ldrh r0, [r0, 0x2E]\n\
    cmp r0, 0\n\
    bne _08024AFE\n\
    b _08024C38\n\
_08024AFE:\n\
    movs r0, 0x5\n\
    bl PlaySE\n\
    ldr r0, _08024B98 @ =gStringVar4\n\
    ldr r1, _08024B9C @ =BattleText_Format2\n\
    bl StringCopy\n\
    adds r5, r0, 0\n\
    movs r0, 0\n\
    mov r8, r0\n\
    mov r9, r0\n\
    movs r6, 0xFC\n\
    movs r7, 0x14\n\
    ldr r1, _08024BA0 @ =0x00016018\n\
    add r10, r1\n\
_08024B1C:\n\
    ldr r1, _08024BA4 @ =gUnknown_0840165C\n\
    mov r2, r8\n\
    lsls r0, r2, 2\n\
    adds r0, r1\n\
    ldr r1, [r0]\n\
    adds r0, r5, 0\n\
    bl StringAppend\n\
    adds r5, r0, 0\n\
    mov r0, r10\n\
    ldrb r1, [r0]\n\
    movs r0, 0x64\n\
    muls r0, r1\n\
    ldr r1, _08024BA8 @ =gPlayerParty\n\
    adds r0, r1\n\
    ldr r1, _08024BAC @ =gLevelUpStatBoxStats\n\
    add r1, r8\n\
    ldrb r1, [r1]\n\
    bl GetMonData\n\
    adds r1, r0, 0\n\
    strb r6, [r5]\n\
    movs r0, 0x13\n\
    strb r0, [r5, 0x1]\n\
    movs r4, 0x1\n\
    mov r2, r8\n\
    ands r4, r2\n\
    lsls r0, r4, 3\n\
    adds r0, r4\n\
    adds r0, 0x5\n\
    lsls r0, 3\n\
    adds r0, 0x6\n\
    strb r0, [r5, 0x2]\n\
    adds r5, 0x3\n\
    strb r6, [r5]\n\
    strb r7, [r5, 0x1]\n\
    movs r0, 0x6\n\
    strb r0, [r5, 0x2]\n\
    adds r5, 0x3\n\
    lsls r1, 16\n\
    asrs r1, 16\n\
    adds r0, r5, 0\n\
    movs r2, 0x1\n\
    movs r3, 0x3\n\
    bl ConvertIntToDecimalStringN\n\
    adds r5, r0, 0\n\
    strb r6, [r5]\n\
    strb r7, [r5, 0x1]\n\
    mov r0, r9\n\
    strb r0, [r5, 0x2]\n\
    adds r5, 0x3\n\
    cmp r4, 0\n\
    beq _08024BB0\n\
    movs r0, 0xFE\n\
    strb r0, [r5]\n\
    movs r0, 0xFF\n\
    strb r0, [r5, 0x1]\n\
    adds r5, 0x1\n\
    b _08024BC0\n\
    .align 2, 0\n\
_08024B94: .4byte gMain\n\
_08024B98: .4byte gStringVar4\n\
_08024B9C: .4byte BattleText_Format2\n\
_08024BA0: .4byte 0x00016018\n\
_08024BA4: .4byte gUnknown_0840165C\n\
_08024BA8: .4byte gPlayerParty\n\
_08024BAC: .4byte gLevelUpStatBoxStats\n\
_08024BB0:\n\
    strb r6, [r5]\n\
    movs r0, 0x11\n\
    strb r0, [r5, 0x1]\n\
    movs r0, 0x8\n\
    strb r0, [r5, 0x2]\n\
    adds r5, 0x3\n\
    movs r0, 0xFF\n\
    strb r0, [r5]\n\
_08024BC0:\n\
    movs r1, 0x1\n\
    add r8, r1\n\
    mov r2, r8\n\
    cmp r2, 0x5\n\
    ble _08024B1C\n\
    ldr r4, _08024BF4 @ =gUnknown_03004210\n\
    ldr r1, _08024BF8 @ =gStringVar4\n\
    movs r2, 0x80\n\
    lsls r2, 1\n\
    movs r0, 0x1\n\
    str r0, [sp]\n\
    adds r0, r4, 0\n\
    movs r3, 0xC\n\
    bl Text_InitWindow\n\
    adds r0, r4, 0\n\
    bl Text_PrintWindow8002F44\n\
    ldr r1, _08024BFC @ =gSharedMem\n\
    ldr r0, _08024C00 @ =0x0001609c\n\
    adds r1, r0\n\
_08024BEA:\n\
    ldrb r0, [r1]\n\
    adds r0, 0x1\n\
    strb r0, [r1]\n\
    b _08024C38\n\
    .align 2, 0\n\
_08024BF4: .4byte gUnknown_03004210\n\
_08024BF8: .4byte gStringVar4\n\
_08024BFC: .4byte gSharedMem\n\
_08024C00: .4byte 0x0001609c\n\
_08024C04:\n\
    ldr r0, _08024C2C @ =gMain\n\
    ldrh r0, [r0, 0x2E]\n\
    cmp r0, 0\n\
    beq _08024C38\n\
    movs r0, 0x5\n\
    bl PlaySE\n\
    movs r0, 0x1\n\
    str r0, [sp]\n\
    movs r0, 0xB\n\
    movs r1, 0\n\
    movs r2, 0x1D\n\
    movs r3, 0x7\n\
    bl sub_802BBD4\n\
    ldrb r0, [r4]\n\
    adds r0, 0x1\n\
    strb r0, [r4]\n\
    b _08024C38\n\
    .align 2, 0\n\
_08024C2C: .4byte gMain\n\
_08024C30:\n\
    ldr r1, _08024C48 @ =gBattlescriptCurrInstr\n\
    ldr r0, [r1]\n\
    adds r0, 0x1\n\
    str r0, [r1]\n\
_08024C38:\n\
    add sp, 0x4\n\
    pop {r3-r5}\n\
    mov r8, r3\n\
    mov r9, r4\n\
    mov r10, r5\n\
    pop {r4-r7}\n\
    pop {r0}\n\
    bx r0\n\
    .align 2, 0\n\
_08024C48: .4byte gBattlescriptCurrInstr\n\
        .syntax divided");
}

#endif // NONMATCHING

static void atk6D_resetsentmonsvalue(void)
{
    ResetSentPokesToOpponentValue();
    gBattlescriptCurrInstr++;
}

static void atk6E_setatktoplayer0(void)
{
    gBattlerAttacker = GetBattlerAtPosition(0);
    gBattlescriptCurrInstr++;
}

static void atk6F_makevisible(void)
{
    gActiveBattler = GetBattlerForBattleScript(T2_READ_8(gBattlescriptCurrInstr + 1));
    BtlController_EmitSpriteInvisibility(0, 0);
    MarkBattlerForControllerExec(gActiveBattler);
    gBattlescriptCurrInstr += 2;
}

static void atk70_recordlastability(void)
{
    gActiveBattler = GetBattlerForBattleScript(T2_READ_8(gBattlescriptCurrInstr + 1));
    RecordAbilityBattle(gActiveBattler, gLastUsedAbility);
    gBattlescriptCurrInstr += 1; //buggy, should be += 2, one byte for command, one byte for argument...
}

void sub_8024CEC(void)
{
    gBattleTextBuff2[0] = 0xFD;
    gBattleTextBuff2[1] = 2;
    gBattleTextBuff2[2] = (gMoveToLearn);
    gBattleTextBuff2[3] = uBYTE1_16(gMoveToLearn);
    gBattleTextBuff2[4] = 0xFF;
}

static void atk71_buffermovetolearn(void)
{
    sub_8024CEC();
    gBattlescriptCurrInstr++;
}

static void atk72_jumpifplayerran(void)
{
    if (TryRunFromBattle(gBank1))
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    else
        gBattlescriptCurrInstr += 5;
}

static void atk73_hpthresholds(void)
{
    u8 opposing_bank;
    s32 result;
    if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
    {
        gActiveBattler = GetBattlerForBattleScript(T2_READ_8(gBattlescriptCurrInstr + 1));
        opposing_bank = gActiveBattler ^ 1;

        result = gBattleMons[opposing_bank].hp * 100 / gBattleMons[opposing_bank].maxHP;
        if (result == 0)
            result = 1;

        if (result >= 70 || !gBattleMons[opposing_bank].hp)
            gBattleStruct->hpScale = 0;
        else if (result >= 40)
            gBattleStruct->hpScale = 1;
        else if (result >= 10)
            gBattleStruct->hpScale = 2;
        else
            gBattleStruct->hpScale = 3;
    }

    gBattlescriptCurrInstr += 2;
}

static void atk74_hpthresholds2(void)
{
    u8 opposing_bank;
    u8 hp_switchout;
    s32 result;
    if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
    {
        gActiveBattler = GetBattlerForBattleScript(T2_READ_8(gBattlescriptCurrInstr + 1));
        opposing_bank = gActiveBattler ^ 1;
        hp_switchout = ewram160BCarr(GetBattlerSide(opposing_bank)); //gBattleStruct->HP_OnSwitchout[GetBattlerSide(opposing_bank)];
        result = (hp_switchout - gBattleMons[opposing_bank].hp) * 100 / hp_switchout;

        if (gBattleMons[opposing_bank].hp >= hp_switchout)
            gBattleStruct->hpScale = 0;
        else if (result <= 29)
            gBattleStruct->hpScale = 1;
        else if (result <= 69)
            gBattleStruct->hpScale = 2;
        else
            gBattleStruct->hpScale = 3;
    }

    gBattlescriptCurrInstr += 2;
}

static void atk75_useitemonopponent(void)
{
    gBankInMenu = gBattlerAttacker;
    PokemonUseItemEffects(&gEnemyParty[gBattlerPartyIndexes[gBattlerAttacker]], gLastUsedItem, gBattlerPartyIndexes[gBattlerAttacker], 0, 1);
    gBattlescriptCurrInstr += 1;
}

static u16 GetFlingBasePowerAndEffect(u16 item)
{
    u16 effectandpower = 0; // most significant 8 bits are effect, bottom 8 bits are power

    switch (item)
    {
    case ITEM_CHERI_BERRY ... ITEM_MARANGA_BERRY:
    case ITEM_SEA_INCENSE ... ITEM_LAX_INCENSE:
    case ITEM_ODD_INCENSE ... ITEM_PURE_INCENSE:
    case ITEM_RED_SCARF ... ITEM_YELLOW_SCARF:
    case ITEM_AIR_BALLOON:
    case ITEM_BIG_ROOT:
    case ITEM_BRIGHT_POWDER:
    case ITEM_CHOICE_BAND:
    case ITEM_CHOICE_SCARF:
    case ITEM_CHOICE_SPECS:
    case ITEM_DESTINY_KNOT:
    case ITEM_DISCOUNT_COUPON:
    case ITEM_ELECTRIC_SEED:
    case ITEM_EXPERT_BELT:
    case ITEM_FOCUS_BAND:
    case ITEM_FOCUS_SASH:
    case ITEM_GRASSY_SEED:
    case ITEM_LAGGING_TAIL:
    case ITEM_LEFTOVERS:
    case ITEM_METAL_POWDER:
    case ITEM_MISTY_SEED:
    case ITEM_MUSCLE_BAND:
    case ITEM_PINK_NECTAR:
    case ITEM_POWER_HERB:
    case ITEM_PSYCHIC_SEED:
    case ITEM_PURPLE_NECTAR:
    case ITEM_QUICK_POWDER:
    case ITEM_REAPER_CLOTH:
    case ITEM_RED_CARD:
    case ITEM_RED_NECTAR:
    case ITEM_RING_TARGET:
    case ITEM_SHED_SHELL:
    case ITEM_SILK_SCARF:
    case ITEM_SILVER_POWDER:
    case ITEM_SMOOTH_ROCK:
    case ITEM_SOFT_SAND:
    case ITEM_SOOTHE_BELL:
    case ITEM_WIDE_LENS:
    case ITEM_WISE_GLASSES:
    case ITEM_YELLOW_NECTAR:
    case ITEM_ZOOM_LENS:
        effectandpower = 10;
        break;
    case ITEM_PRETTY_WING:
    case ITEM_HEALTH_WING ... ITEM_SWIFT_WING:
        effectandpower = 20;
        break;
    case ITEM_POTION ... ITEM_PP_MAX:
    case ITEM_RED_SHARD ... ITEM_GREEN_SHARD:
    case ITEM_GROWTH_MULCH ... ITEM_AMAZE_MULCH:
    case ITEM_GUARD_SPEC ... ITEM_X_SP_DEF:
    case ITEM_DIRE_HIT_2 ... ITEM_RESET_URGE:
    case ITEM_BLUE_FLUTE ... ITEM_WHITE_FLUTE:
    case ITEM_ABSORB_BULB:
    case ITEM_ADRENALINE_ORB:
    case ITEM_AMULET_COIN:
    case ITEM_BALM_MUSHROOM:
    case ITEM_BIG_MALASADA:
    case ITEM_BIG_MUSHROOM:
    case ITEM_BIG_NUGGET:
    case ITEM_BIG_PEARL:
    case ITEM_BINDING_BAND:
    case ITEM_BLACK_BELT:
    case ITEM_BLACK_GLASSES:
    case ITEM_BLACK_SLUDGE:
    case ITEM_BOTTLE_CAP:
    case ITEM_CASTELIACONE:
    case ITEM_CELL_BATTERY:
    case ITEM_CHARCOAL:
    case ITEM_CLEANSE_TAG:
    case ITEM_COMET_SHARD:
    case ITEM_DEEP_SEA_SCALE:
    case ITEM_DRAGON_SCALE:
    case ITEM_EJECT_BUTTON:
    case ITEM_ESCAPE_ROPE:
    case ITEM_EVERSTONE:
    case ITEM_EXP_SHARE: // even though it's not really applicable in this specific base
    case ITEM_FIRE_STONE:
    case ITEM_FLOAT_STONE:
    case ITEM_FLUFFY_TAIL:
    case ITEM_GOLD_BOTTLE_CAP:
    case ITEM_HEART_SCALE:
    case ITEM_HONEY:
    case ITEM_ICE_STONE:
    case ITEM_LEAF_STONE:
    case ITEM_LIFE_ORB:
    case ITEM_LIGHT_CLAY:
    case ITEM_LUCKY_EGG:
    case ITEM_LUMINOUS_MOSS:
    case ITEM_LUMIOSE_GALETTE:
    case ITEM_MAGNET:
    case ITEM_METAL_COAT:
    case ITEM_METRONOME:
    case ITEM_MIRACLE_SEED:
    case ITEM_MOON_STONE:
    case ITEM_MYSTIC_WATER:
    case ITEM_NEVER_MELT_ICE:
    case ITEM_NUGGET:
    case ITEM_OLD_GATEAU:
    case ITEM_PASS_ORB:
    case ITEM_PEARL_STRING:
    case ITEM_PEARL:
    case ITEM_POKE_DOLL:
    case ITEM_POKE_TOY:
    case ITEM_PRISM_SCALE:
    case ITEM_PROTECTIVE_PADS:
    case ITEM_RELIC_BAND:
    case ITEM_RELIC_COPPER:
    case ITEM_RELIC_CROWN:
    case ITEM_RELIC_GOLD:
    case ITEM_RELIC_SILVER:
    case ITEM_RELIC_STATUE:
    case ITEM_RELIC_VASE:
    case ITEM_SCOPE_LENS:
    case ITEM_SHALOUR_SABLE:
    case ITEM_SHELL_BELL:
    case ITEM_SHOAL_SALT:
    case ITEM_SHOAL_SHELL:
    case ITEM_SMOKE_BALL:
    case ITEM_SNOWBALL:
    case ITEM_SOUL_DEW:
    case ITEM_SPELL_TAG:
    case ITEM_STAR_PIECE:
    case ITEM_STRANGE_SOUVENIR:
    case ITEM_STARDUST:
    case ITEM_SUN_STONE:
    case ITEM_SWEET_HEART:
    case ITEM_THUNDER_STONE:
    case ITEM_TINY_MUSHROOM:
    case ITEM_TWISTED_SPOON:
    case ITEM_UP_GRADE:
    case ITEM_WATER_STONE:
        effectandpower = 30;
        break;
    case ITEM_EVIOLITE:
    case ITEM_ICY_ROCK:
    case ITEM_LUCKY_PUNCH:
        effectandpower = 40;
        break;
    case ITEM_FIGHTING_MEMORY ... ITEM_FAIRY_MEMORY:
    case ITEM_DUBIOUS_DISC:
    case ITEM_SHARP_BEAK:
        effectandpower = 50;
        break;
    case ITEM_ADAMANT_ORB:
    case ITEM_DAMP_ROCK:
    case ITEM_GRISEOUS_ORB:
    case ITEM_HEAT_ROCK:
    case ITEM_STICK:
    case ITEM_LUSTROUS_ORB:
    case ITEM_MACHO_BRACE:
    case ITEM_ROCKY_HELMET:
    case ITEM_TERRAIN_EXTENDER:
        effectandpower = 60;
        break;
    case ITEM_DOUSE_DRIVE ... ITEM_SHOCK_DRIVE:
    case ITEM_DRAGON_FANG:
    case ITEM_POWER_BRACER ... ITEM_POWER_WEIGHT:
        effectandpower = 70;
        break;
    case ITEM_GENGARITE ... ITEM_BEEDRILLITE:
    case ITEM_ASSAULT_VEST:
    case ITEM_DAWN_STONE:
    case ITEM_DUSK_STONE:
    case ITEM_ELECTIRIZER:
    case ITEM_MAGMARIZER:
    case ITEM_ODD_KEYSTONE:
    case ITEM_OVAL_STONE:
    case ITEM_PROTECTOR:
    case ITEM_QUICK_CLAW:
    case ITEM_RAZOR_CLAW:
    case ITEM_SACHET:
    case ITEM_SAFETY_GOGGLES:
    case ITEM_SHINY_STONE:
    case ITEM_STICKY_BARB:
    case ITEM_WEAKNESS_POLICY:
    case ITEM_WHIPPED_DREAM:
        effectandpower = 80;
        break;
    case ITEM_FLAME_PLATE ... ITEM_PIXIE_PLATE:
    case ITEM_DEEP_SEA_TOOTH:
    case ITEM_GRIP_CLAW:
    case ITEM_THICK_CLUB:
        effectandpower = 90;
        break;
    case ITEM_ROOT_FOSSIL ... ITEM_SAIL_FOSSIL:
    case ITEM_HARD_STONE:
    case ITEM_RARE_BONE:
        effectandpower = 100;
        break;
    case ITEM_IRON_BALL:
        effectandpower = 130;
        break;
    case ITEM_FLAME_ORB:
        effectandpower = MOVE_EFFECT_BURN << 8 | 30;
        break;
    case ITEM_KINGS_ROCK:
        effectandpower = MOVE_EFFECT_FLINCH << 8 | 30;
        break;
    case ITEM_LIGHT_BALL:
    case ITEM_SHOCK_ORB:
        effectandpower = MOVE_EFFECT_PARALYSIS << 8 | 30;
        break;
    case ITEM_MENTAL_HERB:
        effectandpower = MOVE_EFFECT_HEAL_VARIOUS << 8 | 10;
        break;
    case ITEM_POISON_BARB:
        effectandpower = MOVE_EFFECT_POISON << 8 | 70;
        break;
    case ITEM_RAZOR_FANG:
        effectandpower = MOVE_EFFECT_FLINCH << 8 | 30;
        break;
    case ITEM_TOXIC_ORB:
        effectandpower = MOVE_EFFECT_TOXIC << 8 | 30;
        break;
    case ITEM_WHITE_HERB:
        effectandpower = MOVE_EFFECT_RESTORE_STATS << 8 | 10; // todo
        break;
    }

    return effectandpower;
}

#define DEFOG_CLEAR(status, structField, battlescript, move)\
{                                                           \
    if (*sideStatuses & status)                             \
    {                                                       \
        if (clear)                                          \
        {                                                   \
            if (move)                                       \
                PREPARE_MOVE_BUFFER(gBattleTextBuff1, move);\
            *sideStatuses &= ~(status);                     \
            sideTimer->structField = 0;                     \
            BattleScriptPushCursor();                       \
            gBattlescriptCurrInstr = battlescript;          \
        }                                                   \
        return TRUE;                                        \
    }                                                       \
}

static bool32 ClearDefogHazards(u8 bank, bool32 clear)
{
    s32 i;

    if (clear)
    {
        gBattleGlobalTimers.fog = 0;
    }

    for (i = 0; i < 2; i++)
    {
        struct SideTimer *sideTimer = &gSideTimers[i];
        u16 *sideStatuses = &gSideStatuses[i];

        //gBattlerAttacker = i;
        if (GetBattlerSide(bank) != i)
        {
            DEFOG_CLEAR(SIDE_STATUS_REFLECT, reflectTimer, BattleScript_SideStatusWoreOffReturn, MOVE_REFLECT);
            DEFOG_CLEAR(SIDE_STATUS_LIGHTSCREEN, lightscreenTimer, BattleScript_SideStatusWoreOffReturn, MOVE_LIGHT_SCREEN);
            DEFOG_CLEAR(SIDE_STATUS_MIST, mistTimer, BattleScript_SideStatusWoreOffReturn, MOVE_MIST);
            //DEFOG_CLEAR(SIDE_STATUS_AURORA_VEIL, auroraVeilTimer, BattleScript_SideStatusWoreOffReturn, MOVE_AURORA_VEIL);
            DEFOG_CLEAR(SIDE_STATUS_SAFEGUARD, safeguardTimer, BattleScript_SideStatusWoreOffReturn, MOVE_SAFEGUARD);
        }
        DEFOG_CLEAR(SIDE_STATUS_SPIKES, spikesAmount, BattleScript_SpikesFree, 0);
        DEFOG_CLEAR(SIDE_STATUS_STEALTH_ROCK, field4, BattleScript_RocksFree, 0); // because field4 is never really used
        DEFOG_CLEAR(SIDE_STATUS_TOXIC_SPIKES, toxicSpikesAmount, BattleScript_ToxicSpikesFree, 0);
        //DEFOG_CLEAR(SIDE_STATUS_STICKY_WEB, stickyWebAmount, BattleScript_StickyWebFree, 0);
    }

    return FALSE;
}

#define VARIOUS_CANCEL_MULTI_TURN_MOVES 0
#define VARIOUS_SET_MAGIC_COAT_TARGET 1
#define VARIOUS_IS_RUNNING_IMPOSSIBLE 2
#define VARIOUS_GET_MOVE_TARGET 3
#define VARIOUS_RESET_INTIMIDATE_TRACE_BITS 5
#define VARIOUS_UPDATE_CHOICE_MOVE_ON_LVL_UP 6
#define VARIOUS_RESTORE_PP 7
#define VARIOUS_RESTORE_ALL_HEALTH 8
#define VARIOUS_SET_ROOST 9
#define VARIOUS_SET_GRAVITY 10
#define VARIOUS_SET_MIRACLE_EYE 11
#define VARIOUS_CALC_BRINE 27
#define VARIOUS_JUMP_TO_FAIL_IF_NOT_BERRY 28
#define VARIOUS_SET_NATURAL_GIFT 29
#define VARIOUS_REMOVE_PROTECT 30
#define VARIOUS_SET_TAILWIND 31
#define VARIOUS_TRY_ACUPRESSURE 32
#define VARIOUS_CALC_METAL_BURST 33
#define VARIOUS_JUMP_TO_END_IF_BATTLE_OVER 34
#define VARIOUS_DO_CLOSE_COMBAT_STAT_DROPS 35
#define VARIOUS_SET_EMBARGO 36
#define VARIOUS_SET_FLING 37
#define VARIOUS_TRY_PSYCHO_SHIFT 38
#define VARIOUS_CURE_STATUS1 39
#define VARIOUS_SET_HEAL_BLOCK 40
#define VARIOUS_SET_POWER_TRICK 41
#define VARIOUS_SET_GASTRO_ACID 42
#define VARIOUS_SET_LUCKY_CHANT 43
#define VARIOUS_TRY_ME_FIRST 44
#define VARIOUS_TRY_COPYCAT 45
#define VARIOUS_SWAP_STAT_STAGES 46
#define VARIOUS_FAIL_IF_CANT_USE_LAST_RESORT 47
#define VARIOUS_TRY_WORRY_SEED 48
#define VARIOUS_TRY_SUCKER_PUNCH 49
#define VARIOUS_SET_TOXIC_SPIKES 50
#define VARIOUS_SET_STEALTH_ROCK 51
#define VARIOUS_SET_AQUA_RING 52
#define VARIOUS_ARGUMENT_STATUS_EFFECT 53
#define VARIOUS_SET_MAGNET_RISE 54
#define VARIOUS_DEFOG 55
#define VARIOUS_SET_ROOM 56
#define VARIOUS_PASS_IF_OPPOSITE_GENDERS 57

static void atk76_various(void)
{
    u8 data[10];
    int i;
    u32 bits;
    u8 sideAttacker = 0, sideTarget = 0;

    gActiveBattler = GetBattlerForBattleScript(T2_READ_8(gBattlescriptCurrInstr + 1));
    
    switch (T2_READ_8(gBattlescriptCurrInstr + 2))
    {
    case VARIOUS_CANCEL_MULTI_TURN_MOVES:
        CancelMultiTurnMoves(gActiveBattler);
        break;
    case VARIOUS_SET_MAGIC_COAT_TARGET:
        {
            u8 side;
            gBattlerAttacker = gBattlerTarget;
            side = GetBattlerSide(gBattlerAttacker) ^ 1;
            if (gSideTimers[side].followmeTimer && gBattleMons[gSideTimers[side].followmeTarget].hp)
                gBattlerTarget = gSideTimers[side].followmeTarget;
            else
                gBattlerTarget = gActiveBattler;
        }
        break;
    case VARIOUS_IS_RUNNING_IMPOSSIBLE:
        gBattleCommunication[0] = CanRunFromBattle();
        break;
    case VARIOUS_GET_MOVE_TARGET:
        gBattlerTarget = GetMoveTarget(gCurrentMove, 0);
        break;
    case 4:
        if (gHitMarker & HITMARKER_FAINTED(gActiveBattler))
            gBattleCommunication[0] = 1;
        else
            gBattleCommunication[0] = 0;
        break;
    case VARIOUS_RESET_INTIMIDATE_TRACE_BITS:
        gSpecialStatuses[gActiveBattler].intimidatedMon = 0;
        gSpecialStatuses[gActiveBattler].traced = 0;
        break;
    case VARIOUS_UPDATE_CHOICE_MOVE_ON_LVL_UP:
        {
            u16* choiced_move;
            
            if (gBattlerPartyIndexes[0] == gBattleStruct->expGetterMonId)
                goto ACTIVE_0;
            if (gBattlerPartyIndexes[2] != gBattleStruct->expGetterMonId)
                break;
            if (gBattlerPartyIndexes[0] == gBattlerPartyIndexes[2])
            {
                ACTIVE_0:
                    gActiveBattler = 0;
            }
            else
                gActiveBattler = 2;

            choiced_move = CHOICED_MOVE(gActiveBattler);
            for (i = 0; i < 4; i++)
            {
                if (gBattleMons[gActiveBattler].moves[i] == *choiced_move)
                    break;
            }
            if (i == 4)
                *choiced_move = 0;
        }
        break;
    case VARIOUS_RESTORE_PP:
        for (i = 0; i < 4; i++)
        {
            gBattleMons[gActiveBattler].pp[i] = CalculatePPWithBonus(gBattleMons[gActiveBattler].moves[i], gBattleMons[gActiveBattler].ppBonuses, i);
            data[i] = gBattleMons[gActiveBattler].pp[i];
        }
        data[i] = gBattleMons[gActiveBattler].ppBonuses;
        BtlController_EmitSetMonData(0, REQUEST_PP_DATA_BATTLE, 0, 5, data);
        MarkBattlerForControllerExec(gActiveBattler);
        break;
    case VARIOUS_RESTORE_ALL_HEALTH:
        gBattleMoveDamage = gBattleMons[gActiveBattler].maxHP;

        if (gBattleMoveDamage == 0)
            gBattleMoveDamage = 1;
        gBattleMoveDamage *= -1;

        if (gBattleMons[gActiveBattler].maxHP == gBattleMons[gActiveBattler].hp)
            BattleScriptExecute(BattleScript_AlreadyAtFullHp);
        break;
    case VARIOUS_SET_ROOST:
        gDisableStructs[gActiveBattler].roost = TRUE;
        break;
    case VARIOUS_SET_GRAVITY:
        if (gBattleGlobalTimers.gravityTimer)
            gBattlescriptCurrInstr = BattleScript_ButItFailed - 3;
        else
            gBattleGlobalTimers.gravityTimer = 5;
        break;
    case VARIOUS_SET_MIRACLE_EYE:
        if (!(gStatuses3[gActiveBattler] & STATUS3_MIRACLE_EYED))
            gStatuses3[gActiveBattler] |= STATUS3_MIRACLE_EYED;
        else
            gBattlescriptCurrInstr = BattleScript_ButItFailed - 3;
        break;
    case VARIOUS_CALC_BRINE:
        if (gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
        {
            gBattlescriptCurrInstr = BattleScript_MoveMissedPause - 3;
        }
        else
        {
            gDynamicBasePower = gBattleMoves[gCurrentMove].power;

            if (gBattleMons[gActiveBattler].hp < gBattleMons[gActiveBattler].maxHP / 2)
                gDynamicBasePower *= 2;
        }
        break;
    case VARIOUS_JUMP_TO_FAIL_IF_NOT_BERRY:
        if (!ITEM_IS_BERRY(gBattleMons[gActiveBattler].item))
            gBattlescriptCurrInstr = BattleScript_ButItFailed - 3;
        break;
    case VARIOUS_SET_NATURAL_GIFT: // the assumption here is that the above various succeeded and no jump happened, therefore the mon is holding a berry
                                    // note that this is separate from the above for moves like incinerate that may eventually be implemented
        gDynamicBasePower = gNaturalGiftBerries[gBattleMons[gActiveBattler].item - FIRST_BERRY].power;
        gBattleStruct->dynamicMoveType = gNaturalGiftBerries[gBattleMons[gActiveBattler].item - FIRST_BERRY].type;
        break;
    case VARIOUS_REMOVE_PROTECT:
        if (gProtectStructs[gActiveBattler].protected
            /*|| gSideStatuses[GetBattlerSide(gActiveBattler)] & SIDE_STATUS_WIDE_GUARD
            || gSideStatuses[GetBattlerSide(gActiveBattler)] & SIDE_STATUS_QUICK_GUARD
            || gSideStatuses[GetBattlerSide(gActiveBattler)] & SIDE_STATUS_CRAFTY_SHIELD
            || gSideStatuses[GetBattlerSide(gActiveBattler)] & SIDE_STATUS_MAT_BLOCK
            || gProtectStructs[gActiveBattler].spikyShielded
            || gProtectStructs[gActiveBattler].kingsShielded
            || gProtectStructs[gActiveBattler].banefulBunkered*/
            )
        {
            gProtectStructs[gActiveBattler].protected = 0;
            /*gSideStatuses[GetBattlerSide(gActiveBattler)] &= ~(SIDE_STATUS_WIDE_GUARD);
            gSideStatuses[GetBattlerSide(gActiveBattler)] &= ~(SIDE_STATUS_QUICK_GUARD);
            gSideStatuses[GetBattlerSide(gActiveBattler)] &= ~(SIDE_STATUS_CRAFTY_SHIELD);
            gSideStatuses[GetBattlerSide(gActiveBattler)] &= ~(SIDE_STATUS_MAT_BLOCK);
            gProtectStructs[gActiveBattler].spikyShielded = 0;
            gProtectStructs[gActiveBattler].kingsShielded = 0;
            gProtectStructs[gActiveBattler].banefulBunkered = 0;*/
        }
        break;
    case VARIOUS_SET_TAILWIND:
        if (gSideTimers[GetBattlerSide(gActiveBattler)].tailwindTimer
         || gSideTimers[!GetBattlerSide(gActiveBattler)].tailwindTimer)
            gBattlescriptCurrInstr = BattleScript_ButItFailed - 3;
        else
            gSideTimers[GetBattlerSide(gActiveBattler)].tailwindTimer = 4; // lasts for 4 turns
        break;
    case VARIOUS_TRY_ACUPRESSURE:
        for (i = STAT_ATK; i < NUM_BATTLE_STATS; i++)
        {
            if (gBattleMons[gActiveBattler].statStages[i] != 12)
                bits |= gBitTable[i];
        }
        if (bits)
        {
            u32 statId;
            do
            {
                statId = (Random() % NUM_BATTLE_STATS) + 1;
            } while (!(bits & gBitTable[statId]));

            if (gBattleMons[gActiveBattler].statStages[statId] >= 11)
                SET_STATCHANGER(statId, 1, FALSE);
            else
                SET_STATCHANGER(statId, 2, FALSE);
        }
        else
        {
            gBattlescriptCurrInstr = BattleScript_ButItFailed - 3;
        }
        break;
    case VARIOUS_CALC_METAL_BURST:
        sideAttacker = GetBattlerSide(gActiveBattler);
        sideTarget = GetBattlerSide(gProtectStructs[gActiveBattler].physicalBattlerId);

        if (gProtectStructs[gActiveBattler].physicalDmg
         && sideAttacker != sideTarget
         && gBattleMons[gProtectStructs[gActiveBattler].physicalBattlerId].hp)
        {
            gBattleMoveDamage = gProtectStructs[gActiveBattler].physicalDmg * 150 / 100;

            if (gSideTimers[sideTarget].followmeTimer && gBattleMons[gSideTimers[sideTarget].followmeTarget].hp)
                gBattlerTarget = gSideTimers[sideTarget].followmeTarget;
            else
                gBattlerTarget = gProtectStructs[gActiveBattler].physicalBattlerId;
        }
        else if (gProtectStructs[gActiveBattler].specialDmg
              && sideAttacker != sideTarget
              && gBattleMons[gProtectStructs[gActiveBattler].specialBattlerId].hp)
        {
            gBattleMoveDamage = gProtectStructs[gActiveBattler].specialDmg * 150 / 100;

            if (gSideTimers[sideTarget].followmeTimer && gBattleMons[gSideTimers[sideTarget].followmeTarget].hp)
                gBattlerTarget = gSideTimers[sideTarget].followmeTarget;
            else
                gBattlerTarget = gProtectStructs[gActiveBattler].specialBattlerId;
        }
        else
        {
            gSpecialStatuses[gActiveBattler].ppNotAffectedByPressure = 1;
            gBattlescriptCurrInstr = BattleScript_ButItFailedAtkStringPpReduce - 3;
        }
        break;
    case VARIOUS_JUMP_TO_END_IF_BATTLE_OVER:
        if (NoAliveMonsForEitherParty())
            gBattlescriptCurrInstr = BattleScript_HitEscapeEnd - 3;
        break;
    case VARIOUS_DO_CLOSE_COMBAT_STAT_DROPS:
        BattleScriptPush(gBattlescriptCurrInstr + 3);
        gBattlescriptCurrInstr = BattleScript_AtkDefDown - 3;
        break;
    case VARIOUS_SET_EMBARGO:
        if (gStatuses3[gActiveBattler] & STATUS3_EMBARGO)
            gBattlescriptCurrInstr = BattleScript_ButItFailed - 3;
        else
        {
            gStatuses3[gActiveBattler] |= STATUS3_EMBARGO;
            gDisableStructs[gActiveBattler].embargoTimer = 5;
        }
        break;
    case VARIOUS_SET_FLING:
        bits = GetFlingBasePowerAndEffect(gBattleMons[gActiveBattler].item); // bits here is used as a variable

        if (bits != 0)
        {
            gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_CERTAIN | (bits & 0xFF00) >> 8;
            gDynamicBasePower = bits & 0xFF;
        }
        else
        {
            gBattlescriptCurrInstr = BattleScript_ButItFailed - 3;
        }
        break;
    case VARIOUS_TRY_PSYCHO_SHIFT:
        i = TRUE;
        if (gBattleMons[gBattlerAttacker].status1 & STATUS1_PARALYSIS)
        {
            if (GetBattlerAbility(gBattlerTarget) == ABILITY_LIMBER)
            {
                //gBattlerAbility = gBattlerTarget;
                BattleScriptPush(BattleScript_MoveEnd);
                gBattlescriptCurrInstr = BattleScript_PRLZPrevention;
                i = FALSE;
            }
            else if (IS_BATTLER_OF_TYPE(gBattlerTarget, TYPE_ELECTRIC))
            {
                BattleScriptPush(BattleScript_MoveEnd);
                gBattlescriptCurrInstr = BattleScript_PRLZPrevention;
                i = FALSE;
            }
            else
            {
                gBattleCommunication[MULTISTRING_CHOOSER] = 3;
            }
        }
        else if (gBattleMons[gBattlerAttacker].status1 & STATUS1_PSN_ANY)
        {
            if (GetBattlerAbility(gBattlerTarget) == ABILITY_IMMUNITY)
            {
                //gBattlerAbility = gBattlerTarget;
                BattleScriptPush(BattleScript_MoveEnd);
                gBattlescriptCurrInstr = BattleScript_PSNPrevention;
                i = FALSE;
            }
            else if (IS_BATTLER_OF_TYPE(gBattlerTarget, TYPE_POISON) || IS_BATTLER_OF_TYPE(gBattlerTarget, TYPE_STEEL))
            {
                BattleScriptPush(BattleScript_MoveEnd);
                gBattlescriptCurrInstr = BattleScript_PSNPrevention;
                i = FALSE;
            }
            else
            {
                if (gBattleMons[gBattlerAttacker].status1 & STATUS1_POISON)
                    gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                else
                    gBattleCommunication[MULTISTRING_CHOOSER] = 1;
            }
        }
        else if (gBattleMons[gBattlerAttacker].status1 & STATUS1_BURN)
        {
            if (GetBattlerAbility(gBattlerTarget) == ABILITY_WATER_VEIL)
            {
                //gBattlerAbility = gBattlerTarget;
                BattleScriptPush(BattleScript_MoveEnd);
                gBattlescriptCurrInstr = BattleScript_BRNPrevention;
                i = FALSE;
            }
            else if (IS_BATTLER_OF_TYPE(gBattlerTarget, TYPE_FIRE))
            {
                BattleScriptPush(BattleScript_MoveEnd);
                gBattlescriptCurrInstr = BattleScript_BRNPrevention;
                i = FALSE;
            }
            else
            {
                gBattleCommunication[MULTISTRING_CHOOSER] = 2;
            }
        }
        else if (gBattleMons[gBattlerAttacker].status1 & STATUS1_SLEEP)
        {
            if (GetBattlerAbility(gBattlerTarget) == ABILITY_INSOMNIA || GetBattlerAbility(gBattlerTarget) == ABILITY_VITAL_SPIRIT)
            {
                gLastUsedAbility = gBattlerTarget;
                // BattleScriptPush(BattleScript_MoveEnd);
                // gBattlescriptCurrInstr = BattleScript_MoveEnd;
                i = FALSE;
            }
            else
            {
                gBattleCommunication[MULTISTRING_CHOOSER] = 4;
            }
        }

        if (i == TRUE)
        {
            gBattleMons[gBattlerTarget].status1 = gBattleMons[gBattlerAttacker].status1 & STATUS1_ANY;
            gActiveBattler = gBattlerTarget;
            BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
            MarkBattlerForControllerExec(gActiveBattler);
        }
        break;
    case VARIOUS_CURE_STATUS1:
        if (gBattleMons[gActiveBattler].status1 & STATUS1_ANY)
        {
            gBattleMons[gActiveBattler].status1 = 0;
            if (!GetBattlerSide(gActiveBattler)) // if is on player's team
            {
                BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
                MarkBattlerForControllerExec(gActiveBattler);
            }
        }
        break;
    case VARIOUS_SET_HEAL_BLOCK:
        if (gStatuses3[gActiveBattler] & STATUS3_HEAL_BLOCK)
            gBattlescriptCurrInstr = BattleScript_ButItFailed - 3;
        else
        {
            gStatuses3[gActiveBattler] |= STATUS3_HEAL_BLOCK;
            gDisableStructs[gActiveBattler].healBlockTimer = 5;
        }
        break;
    case VARIOUS_SET_POWER_TRICK:
        gStatuses3[gActiveBattler] |= STATUS3_POWER_TRICK;
        bits = gBattleMons[gActiveBattler].attack;
        gBattleMons[gActiveBattler].attack = gBattleMons[gActiveBattler].defense;
        gBattleMons[gActiveBattler].defense = bits;
        break;
    case VARIOUS_SET_GASTRO_ACID:
        switch (gBattleMons[gActiveBattler].ability)
        {
            case ABILITY_MULTITYPE:
            case ABILITY_STANCE_CHANGE:
            case ABILITY_SCHOOLING:
            case ABILITY_COMATOSE:
            case ABILITY_SHIELDS_DOWN:
            case ABILITY_DISGUISE:
            case ABILITY_RKS_SYSTEM:
            case ABILITY_BATTLE_BOND:
            case ABILITY_POWER_CONSTRUCT:
                gBattlescriptCurrInstr = BattleScript_ButItFailed - 3;
                break;
            default:
                gStatuses3[gBattlerTarget] |= STATUS3_GASTRO_ACID;
                break;
        }
        break;
    case VARIOUS_SET_LUCKY_CHANT:
        sideTarget = GetBattlerSide(gActiveBattler);
        if (gSideTimers[sideTarget].luckyChantTimer)
            gBattlescriptCurrInstr = BattleScript_ButItFailed - 3;
        else
            gSideTimers[sideTarget].luckyChantTimer = 5;
        break;
    case VARIOUS_TRY_ME_FIRST:
        if (GetBattlerTurnOrderNum(gBattlerAttacker) > GetBattlerTurnOrderNum(gBattlerTarget))
            gBattlescriptCurrInstr = BattleScript_ButItFailedPpReduce - 3;
        else if (gBattleMoves[gBattleMons[gBattlerTarget].moves[gBattleStruct->chosenMoveIndices[gBattlerTarget]]].power == 0)
            gBattlescriptCurrInstr = BattleScript_ButItFailedPpReduce - 3;
        else
        {
            u16 move = gBattleMons[gBattlerTarget].moves[gBattleStruct->chosenMoveIndices[gBattlerTarget]];
            switch (move)
            {
            case MOVE_STRUGGLE:
            case MOVE_CHATTER:
            case MOVE_FOCUS_PUNCH:
            case MOVE_THIEF:
            case MOVE_COVET:
            case MOVE_COUNTER:
            case MOVE_MIRROR_COAT:
            case MOVE_METAL_BURST:
            case MOVE_ME_FIRST:
                gBattlescriptCurrInstr = BattleScript_ButItFailedPpReduce - 3;
                break;
            default:
                gRandomMove = move;
                gStatuses3[gBattlerAttacker] |= STATUS3_ME_FIRST | STATUS3_ALWAYS_HITS; // used to multiply the move by 1.5
                gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);
                gBattlerTarget = GetMoveTarget(gRandomMove, 0);
                break;
            }
        }
        break;
    case VARIOUS_TRY_COPYCAT:
        for (i = 0; sMovesForbiddenToCopy[i] != 0xFFFF; i++)
        {
            if (sMovesForbiddenToCopy[i] == gLastUsedMove)
                break;
        }
        if (gLastUsedMove == 0 || gLastUsedMove == 0xFFFF || sMovesForbiddenToCopy[i] != 0xFFFF)
        {
            gBattlescriptCurrInstr = BattleScript_ButItFailedPpReduce - 3;
        }
        else
        {
            gRandomMove = gLastUsedMove;
            gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);
            gBattlerTarget = GetMoveTarget(gRandomMove, 0);
        }
        break;
    case VARIOUS_SWAP_STAT_STAGES:
        i = T2_READ_8(gBattlescriptCurrInstr + 1);
        bits = gBattleMons[gBattlerAttacker].statStages[i];
        gBattleMons[gBattlerAttacker].statStages[i] = gBattleMons[gBattlerTarget].statStages[i];
        gBattleMons[gBattlerTarget].statStages[i] = bits;
        break;
    case VARIOUS_FAIL_IF_CANT_USE_LAST_RESORT:
        if (!CanUseLastResort(gActiveBattler))
            gBattlescriptCurrInstr = BattleScript_ButItFailed - 3;
        break;
    case VARIOUS_TRY_WORRY_SEED:
        switch (gBattleMons[gActiveBattler].ability)
        {
        case ABILITY_INSOMNIA:
        case ABILITY_MULTITYPE:
        case ABILITY_TRUANT:
        case ABILITY_STANCE_CHANGE:
            gBattlescriptCurrInstr = BattleScript_ButItFailed - 3;
            break;
        default:
            gBattleMons[gActiveBattler].ability = ABILITY_INSOMNIA;
            break;
        }
        break;
    case VARIOUS_TRY_SUCKER_PUNCH:
        if (GetBattlerTurnOrderNum(gBattlerAttacker) > GetBattlerTurnOrderNum(gBattlerTarget)
         || gBattleMoves[gBattleMons[gBattlerTarget].moves[gBattleStruct->chosenMoveIndices[gBattlerTarget]]].power == 0)
            gBattlescriptCurrInstr = BattleScript_ButItFailedAtkStringPpReduce - 3;
        break;
    case VARIOUS_SET_TOXIC_SPIKES:
        sideTarget = GetBattlerSide(gActiveBattler);
        if (gSideTimers[sideTarget].toxicSpikesAmount >= 2)
        {
            gBattlescriptCurrInstr = BattleScript_ButItFailed - 3;
        }
        else
        {
            gSideTimers[sideTarget].toxicSpikesAmount++;
            gSideStatuses[sideTarget] |= SIDE_STATUS_TOXIC_SPIKES;
        }
        break;
    case VARIOUS_SET_STEALTH_ROCK:
        sideTarget = GetBattlerSide(gActiveBattler);
        if (gSideStatuses[sideTarget] & SIDE_STATUS_STEALTH_ROCK)
        {
            gBattlescriptCurrInstr = BattleScript_ButItFailed - 3;
        }
        else
        {
            gSideStatuses[sideTarget] |= SIDE_STATUS_STEALTH_ROCK;
        }
        break;
    case VARIOUS_SET_AQUA_RING:
        if (gStatuses3[gActiveBattler] & STATUS3_AQUA_RING)
        {
            gBattlescriptCurrInstr = BattleScript_ButItFailed - 3;
        }
        else
        {
            gStatuses3[gActiveBattler] |= STATUS3_AQUA_RING;
        }
        break;
    case VARIOUS_ARGUMENT_STATUS_EFFECT:
        switch (gBattleMoves[gCurrentMove].argument)
        {
        case STATUS1_BURN:
            gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_BURN;
            break;
        case STATUS1_FREEZE:
            gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_FREEZE;
            break;
        case STATUS1_PARALYSIS:
            gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_PARALYSIS;
            break;
        case STATUS1_POISON:
            gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_POISON;
            break;
        case STATUS1_TOXIC_POISON:
            gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_TOXIC;
            break;
        default:
            gBattleCommunication[MOVE_EFFECT_BYTE] = 0;
            break;
        }
        if (gBattleCommunication[MOVE_EFFECT_BYTE] != 0)
        {
            BattleScriptPush(gBattlescriptCurrInstr + 3);
            gBattlescriptCurrInstr = BattleScript_EffectWithChance;
            return; // not gonna mess with incrementing here
        }
        break;
    case VARIOUS_SET_MAGNET_RISE:
        if (gDisableStructs[gActiveBattler].magnetRiseTimer)
        {
            gBattlescriptCurrInstr = BattleScript_ButItFailed - 3;
        }
        else
        {
            gDisableStructs[gActiveBattler].magnetRiseTimer = 5;
        }
        break;
    case VARIOUS_DEFOG: // decided to break my convention here w importing dizzy's things, all script augmentation occurs in this block
        if (T1_READ_8(gBattlescriptCurrInstr + 3)) // Clear
        {
            if (ClearDefogHazards(gActiveBattler, TRUE))
                return;
            else
                gBattlescriptCurrInstr += 8;
        }
        else
        {
            if (ClearDefogHazards(gActiveBattler, FALSE))
                gBattlescriptCurrInstr += 8;
            else
                gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 4);
        }
        return;
    case VARIOUS_SET_ROOM:
        switch (gBattleMoves[gCurrentMove].effect)
        {
        case EFFECT_TRICK_ROOM:
            if (gBattleGlobalTimers.trickRoomTimer)
            {
                gBattleGlobalTimers.trickRoomTimer = 0;
                gBattleCommunication[MULTISTRING_CHOOSER] = 1;
            }
            else
            {
                gBattleGlobalTimers.trickRoomTimer = 5;
                gBattleCommunication[MULTISTRING_CHOOSER] = 0;
            }
            break;
        case EFFECT_WONDER_ROOM:
            if (gBattleGlobalTimers.wonderRoomTimer)
            {
                gBattleGlobalTimers.wonderRoomTimer = 0;
                gBattleCommunication[MULTISTRING_CHOOSER] = 3;
            }
            else
            {
                gBattleGlobalTimers.wonderRoomTimer = 5;
                gBattleCommunication[MULTISTRING_CHOOSER] = 2;
            }
            break;
        case EFFECT_MAGIC_ROOM:
            if (gBattleGlobalTimers.magicRoomTimer)
            {
                gBattleGlobalTimers.magicRoomTimer = 0;
                gBattleCommunication[MULTISTRING_CHOOSER] = 5;
            }
            else
            {
                gBattleGlobalTimers.magicRoomTimer = 5;
                gBattleCommunication[MULTISTRING_CHOOSER] = 4;
            }
            break;
        default:
            gBattleCommunication[MULTISTRING_CHOOSER] = 6;
            break;
        }
        break;
    case VARIOUS_PASS_IF_OPPOSITE_GENDERS: // hi i reuse variables nice to meet you
        sideAttacker = GetGenderFromSpeciesAndPersonality(gBattleMons[gBattlerAttacker].species, gBattleMons[gBattlerAttacker].personality);
        sideTarget = GetGenderFromSpeciesAndPersonality(gBattleMons[gBattlerTarget].species, gBattleMons[gBattlerTarget].personality);

        if ((sideAttacker == MON_MALE && sideTarget == MON_FEMALE) || (sideAttacker == MON_FEMALE && sideTarget == MON_MALE))
            gBattlescriptCurrInstr = BattleScript_CaptivateCheckAcc - 3;
        break;
    }

    gBattlescriptCurrInstr += 3;
}

static void atk77_setprotectlike(void) //protect and endure
{
    bool8 not_last_turn = 1;
    u16 last_move = gUnknown_02024C4C[gBattlerAttacker];

    if (last_move != MOVE_PROTECT && last_move != MOVE_DETECT && last_move != MOVE_ENDURE)
        gDisableStructs[gBattlerAttacker].protectUses = 0;
    if (gCurrentTurnActionNumber == (gBattlersCount - 1))
        not_last_turn = 0;

    if (sProtectSuccessRates[gDisableStructs[gBattlerAttacker].protectUses] > Random() && not_last_turn)
    {
        if (gBattleMoves[gCurrentMove].effect == EFFECT_PROTECT)
        {
            gProtectStructs[gBattlerAttacker].protected = 1;
            gBattleCommunication[MULTISTRING_CHOOSER] = 0;
        }
        if (gBattleMoves[gCurrentMove].effect == EFFECT_ENDURE) //what is else if
        {
            gProtectStructs[gBattlerAttacker].endured = 1;
            gBattleCommunication[MULTISTRING_CHOOSER] = 1;
        }
        gDisableStructs[gBattlerAttacker].protectUses++;
    }
    else
    {
        gDisableStructs[gBattlerAttacker].protectUses = 0;
        gBattleCommunication[MULTISTRING_CHOOSER] = 2;
        gMoveResultFlags |= MOVE_RESULT_MISSED;
    }

    gBattlescriptCurrInstr++;
}

static void atk78_faintifabilitynotdamp(void)
{
    if (gBattleControllerExecFlags)
        return;

    for (gBattlerTarget = 0; gBattlerTarget < gBattlersCount; gBattlerTarget++)
    {
        if (GetBattlerAbility(gBattlerTarget) == ABILITY_DAMP)
            break;
    }

    if (gBattlerTarget == gBattlersCount)
    {
        gActiveBattler = gBattlerAttacker;
        gBattleMoveDamage = gBattleMons[gActiveBattler].hp;
        BtlController_EmitHealthBarUpdate(0, 0x7FFF);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattlescriptCurrInstr++;

        for (gBattlerTarget = 0; gBattlerTarget < gBattlersCount; gBattlerTarget++)
        {
            if (gBattlerTarget == gBattlerAttacker)
                continue;
            if (!(gAbsentBattlerFlags & gBitTable[gBattlerTarget]))
                break;
        }
    }
    else
    {
        gLastUsedAbility = ABILITY_DAMP;
        RecordAbilityBattle(gBattlerTarget, gLastUsedAbility);
        gBattlescriptCurrInstr = BattleScript_DampStopsExplosion;
    }
}

static void atk79_setatkhptozero(void)
{
    if (gBattleControllerExecFlags)
        return;

    gActiveBattler = gBattlerAttacker;
    gBattleMons[gActiveBattler].hp = 0;
    BtlController_EmitSetMonData(0, REQUEST_HP_BATTLE, 0, 2, &gBattleMons[gActiveBattler].hp);
    MarkBattlerForControllerExec(gActiveBattler);

    gBattlescriptCurrInstr++;
}

static void atk7A_jumpifnexttargetvalid(void) //used by intimidate to loop through all targets
{
    u8* jump_loc = T1_READ_PTR(gBattlescriptCurrInstr + 1);

    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
    {
        for (gBattlerTarget++; ; gBattlerTarget++)
        {
            if (gBattlerTarget == gBattlerAttacker)
                continue;
            if (!(gAbsentBattlerFlags & gBitTable[gBattlerTarget]))
                break;
        }

        if (gBattlerTarget >= gBattlersCount)
            gBattlescriptCurrInstr += 5;
        else
            gBattlescriptCurrInstr = jump_loc;
    }
    else
        gBattlescriptCurrInstr += 5;
}

static void atk7B_tryhealhalfhealth(void)
{
    u8* fail_loc = T1_READ_PTR(gBattlescriptCurrInstr + 1);

    if (T2_READ_8(gBattlescriptCurrInstr + 5) == 1)
        gBattlerTarget = gBattlerAttacker;

    gBattleMoveDamage = gBattleMons[gBattlerTarget].maxHP / 2;

    if (gBattleMoveDamage == 0)
        gBattleMoveDamage = 1;
    gBattleMoveDamage *= -1;

    if (gBattleMons[gBattlerTarget].hp == gBattleMons[gBattlerTarget].maxHP)
        gBattlescriptCurrInstr = fail_loc;
    else
        gBattlescriptCurrInstr += 6;
}

static void atk7C_trymirrormove(void)
{
    u16 r7 = ewram160ACarr2(0, gBattlerAttacker) | (ewram160ACarr2(1, gBattlerAttacker) << 8);
    u16 r6 = ewram16100arr2(0, gBattlerAttacker) | (ewram16100arr2(1, gBattlerAttacker) << 8);
    u16 r5 = ewram16100arr2(2, gBattlerAttacker) | (ewram16100arr2(3, gBattlerAttacker) << 8);

    if (r7 != 0 && r7 != 0xFFFF)
    {
        gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);
        gCurrentMove = r7;
        gBattlerTarget = GetMoveTarget(gCurrentMove, 0);
        gBattlescriptCurrInstr = gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect];
    }
    else if (r6 != 0 && r5 != 0 && r6 != 0xFFFF && r5 != 0xFFFF)
    {
        gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);
        if (Random() & 1)
            gCurrentMove = r6;
        else
            gCurrentMove = r5;
        gBattlerTarget = GetMoveTarget(gCurrentMove, 0);
        gBattlescriptCurrInstr = gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect];
    }
    else if (r6 != 0 && r6 != 0xFFFF)
    {
        gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);
        gCurrentMove = r6;
        gBattlerTarget = GetMoveTarget(gCurrentMove, 0);
        gBattlescriptCurrInstr = gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect];
    }
    else if (r5 != 0 && r5 != 0xFFFF)
    {
        gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);
        gCurrentMove = r5;
        gBattlerTarget = GetMoveTarget(gCurrentMove, 0);
        gBattlescriptCurrInstr = gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect];
    }
    else
    {
        gSpecialStatuses[gBattlerAttacker].ppNotAffectedByPressure = 1;
        gBattlescriptCurrInstr++;
    }
}

static void atk7D_setrain(void)
{
    if (gBattleWeather & WEATHER_RAIN_ANY)
    {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 2;
    }
    else
    {
        gBattleGlobalTimers.fog = 0;
        gBattleWeather = WEATHER_RAIN_TEMPORARY;
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
        if (ItemId_GetHoldEffect(gBattleMons[gBattlerAttacker].item) == HOLD_EFFECT_EXTEND_RAIN)
            gWishFutureKnock.weatherDuration = 8;
        else
            gWishFutureKnock.weatherDuration = 5;
    }
    gBattlescriptCurrInstr++;
}

static void atk7E_setreflect(void)
{
    if (gSideStatuses[GetBattlerPosition(gBattlerAttacker) & 1] & SIDE_STATUS_REFLECT)
    {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
    }
    else
    {
        gSideStatuses[GetBattlerPosition(gBattlerAttacker) & 1] |= SIDE_STATUS_REFLECT;
        if (ItemId_GetHoldEffect(gBattleMons[gBattlerAttacker].item) == HOLD_EFFECT_EXTEND_SCREENS) 
            gSideTimers[GetBattlerPosition(gBattlerAttacker) & 1].reflectTimer = 8;
        else
            gSideTimers[GetBattlerPosition(gBattlerAttacker) & 1].reflectTimer = 5;
        if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && CountAliveMons(1) == 2)
            gBattleCommunication[MULTISTRING_CHOOSER] = 2;
        else
            gBattleCommunication[MULTISTRING_CHOOSER] = 1;
    }
    gBattlescriptCurrInstr++;
}

static void atk7F_setseeded(void)
{
    if (gMoveResultFlags & MOVE_RESULT_NO_EFFECT || gStatuses3[gBattlerTarget] & STATUS3_LEECHSEED)
    {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 1;
    }
    else if (gBattleMons[gBattlerTarget].type1 == TYPE_GRASS || gBattleMons[gBattlerTarget].type2 == TYPE_GRASS)
    {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 2;
    }
    else
    {
        gStatuses3[gBattlerTarget] |= gBattlerAttacker;
        gStatuses3[gBattlerTarget] |= STATUS3_LEECHSEED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
    }

    gBattlescriptCurrInstr++;
}

static void atk80_manipulatedamage(void)
{
    switch (T2_READ_8(gBattlescriptCurrInstr + 1))
    {
    case 0:
        gBattleMoveDamage *= -1;
        break;
    case 1:
        gBattleMoveDamage /= 2;
        if (gBattleMoveDamage == 0)
            gBattleMoveDamage = 1;
        if ((gBattleMons[gBattlerTarget].maxHP / 2) < gBattleMoveDamage)
            gBattleMoveDamage = gBattleMons[gBattlerTarget].maxHP / 2;
        break;
    case 2:
        gBattleMoveDamage *= 2;
        break;
    case 3:
        gBattleMoveDamage = gBattleMons[gBattlerAttacker].maxHP;
        break;
    }

    gBattlescriptCurrInstr += 2;
}

static void atk81_trysetrest(void)
{
    u8* fail_loc = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    gActiveBattler = gBattlerTarget = gBattlerAttacker;
    gBattleMoveDamage = gBattleMons[gBattlerTarget].maxHP * (-1);
    if (gBattleMons[gBattlerTarget].hp == gBattleMons[gBattlerTarget].maxHP)
        gBattlescriptCurrInstr = fail_loc;
    else
    {
        if (gBattleMons[gBattlerTarget].status1 & ((u8)(~STATUS1_SLEEP)))
            gBattleCommunication[MULTISTRING_CHOOSER] = 1;
        else
            gBattleCommunication[MULTISTRING_CHOOSER] = 0;

        gBattleMons[gBattlerTarget].status1 = 3;
        BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattlescriptCurrInstr += 5;
    }
}

static void atk82_jumpifnotfirstturn(void)
{
    u8* jump_loc = T1_READ_PTR(gBattlescriptCurrInstr + 1);

    if (gDisableStructs[gBattlerAttacker].isFirstTurn)
        gBattlescriptCurrInstr += 5;
    else
        gBattlescriptCurrInstr = jump_loc;
}

static void atk83_nop(void)
{
    gBattlescriptCurrInstr++;
}

bool8 UproarWakeUpCheck(u8 bank)
{
    int i;
    for (i = 0; i < gBattlersCount; i++)
    {
        if (!(gBattleMons[i].status2 & STATUS2_UPROAR) || GetBattlerAbility(bank) == ABILITY_SOUNDPROOF) //wtf gamefreak, you should check this only once, not every time in a loop...
            continue;
        gBattleStruct->scriptingActive = i;
        if (gBattlerTarget == 0xFF)
            gBattlerTarget = i;
        else if (gBattlerTarget == i)
            gBattleCommunication[MULTISTRING_CHOOSER] = 0;
        else
            gBattleCommunication[MULTISTRING_CHOOSER] = 1;
        break;
    }
    if (i == gBattlersCount)
        return 0;
    else
        return 1;
}

static void atk84_jumpifcantmakeasleep(void)
{
    u8* jump_loc = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    if (UproarWakeUpCheck(gBattlerTarget))
        gBattlescriptCurrInstr = jump_loc;
    else if (GetBattlerAbility(gBattlerTarget) == ABILITY_INSOMNIA || GetBattlerAbility(gBattlerTarget) == ABILITY_VITAL_SPIRIT)
    {
        gLastUsedAbility = GetBattlerAbility(gBattlerTarget);
        gBattleCommunication[MULTISTRING_CHOOSER] = 2;
        gBattlescriptCurrInstr = jump_loc;
        RecordAbilityBattle(gBattlerTarget, gLastUsedAbility);
    }
    else
    {
        gBattlescriptCurrInstr += 5;
    }
}

static void atk85_stockpile(void)
{
    if (gDisableStructs[gBattlerAttacker].stockpileCounter == 3)
    {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 1;
    }
    else
    {
        gDisableStructs[gBattlerAttacker].stockpileCounter++;
        gBattleTextBuff1[0] = 0xFD;
        gBattleTextBuff1[1] = 1;
        gBattleTextBuff1[2] = 1;
        gBattleTextBuff1[3] = 1;
        gBattleTextBuff1[4] = gDisableStructs[gBattlerAttacker].stockpileCounter;
        gBattleTextBuff1[5] = 0xFF;
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
    }
    gBattlescriptCurrInstr++;
}

static void atk86_stockpiletobasedamage(void)
{
    u8* jump_loc = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    if (gDisableStructs[gBattlerAttacker].stockpileCounter == 0)
    {
        gBattlescriptCurrInstr = jump_loc;
    }
    else
    {
        if (gBattleCommunication[6] != 1)
        {
            gBattleMoveDamage = CalculateBaseDamage(&gBattleMons[gBattlerAttacker], &gBattleMons[gBattlerTarget], gCurrentMove,
                                                    gSideStatuses[GetBattlerPosition(gBattlerTarget) & 1], 0,
                                                    0, gBattlerAttacker, gBattlerTarget)
                                * gDisableStructs[gBattlerAttacker].stockpileCounter;
            gBattleStruct->animTurn = gDisableStructs[gBattlerAttacker].stockpileCounter;

            if (gProtectStructs[gBattlerAttacker].helpingHand)
                gBattleMoveDamage = gBattleMoveDamage * 15 / 10;
        }
        gDisableStructs[gBattlerAttacker].stockpileCounter = 0;
        gBattlescriptCurrInstr += 5;
    }
}

static void atk87_stockpiletohpheal(void)
{
    u8* jump_loc = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    if (gDisableStructs[gBattlerAttacker].stockpileCounter == 0)
    {
        gBattlescriptCurrInstr = jump_loc;
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
    }
    else if (gBattleMons[gBattlerAttacker].maxHP == gBattleMons[gBattlerAttacker].hp)
    {
        gDisableStructs[gBattlerAttacker].stockpileCounter = 0;
        gBattlescriptCurrInstr = jump_loc;
        gBattlerTarget = gBattlerAttacker;
        gBattleCommunication[MULTISTRING_CHOOSER] = 1;
    }
    else
    {
        gBattleMoveDamage = gBattleMons[gBattlerAttacker].maxHP / (1 << (3 - gDisableStructs[gBattlerAttacker].stockpileCounter));
        if (gBattleMoveDamage == 0)
            gBattleMoveDamage = 1;
        gBattleMoveDamage *= -1;
        gBattleStruct->animTurn = gDisableStructs[gBattlerAttacker].stockpileCounter;
        gDisableStructs[gBattlerAttacker].stockpileCounter = 0;
        gBattlescriptCurrInstr += 5;
        gBattlerTarget = gBattlerAttacker;
    }
}

static void atk88_negativedamage(void)
{
    gBattleMoveDamage = -(gHpDealt / 2);
    if (gBattleMoveDamage == 0)
        gBattleMoveDamage = -1;
    if (ItemId_GetHoldEffect(gBattleMons[gBattlerAttacker].item) == HOLD_EFFECT_BOOST_DRAIN_MOVES)
        gBattleMoveDamage *= (130 / 100);
    gBattlescriptCurrInstr++;
}

static u8 ChangeStatBuffs(s8 statValue, u8 statId, u8 flags, const u8 *BS_ptr)
{
    bool8 certain = FALSE;
    bool8 notProtectAffected = FALSE;
    u32 index;

    if (flags & MOVE_EFFECT_AFFECTS_USER)
        gActiveBattler = gBattlerAttacker;
    else
        gActiveBattler = gBattlerTarget;

    flags &= ~(MOVE_EFFECT_AFFECTS_USER);

    if (flags & MOVE_EFFECT_CERTAIN)
        certain++;
    flags &= ~(MOVE_EFFECT_CERTAIN);

    if (flags & STAT_CHANGE_NOT_PROTECT_AFFECTED)
        notProtectAffected++;
    flags &= ~(STAT_CHANGE_NOT_PROTECT_AFFECTED);

    if (GetBattlerAbility(gActiveBattler) == ABILITY_CONTRARY)
    {
        statValue ^= STAT_BUFF_NEGATIVE;
        gBattleStruct->statChanger ^= STAT_BUFF_NEGATIVE;
    }
    else if (GetBattlerAbility(gActiveBattler) == ABILITY_SIMPLE)
    {
        statValue = (SET_STAT_BUFF_VALUE(GET_STAT_BUFF_VALUE(statValue) * 2)) | ((statValue <= -1) ? STAT_BUFF_NEGATIVE : 0);
    }

    PREPARE_STAT_BUFFER(gBattleTextBuff1, statId)

    if ((statValue << 0x18) < 0) // stat decrease
    {
        if (gSideTimers[GET_BATTLER_SIDE(gActiveBattler)].mistTimer
            && !certain && gCurrentMove != MOVE_CURSE)
        {
            if (flags == STAT_CHANGE_BS_PTR)
            {
                if (gSpecialStatuses[gActiveBattler].statLowered)
                {
                    gBattlescriptCurrInstr = BS_ptr;
                }
                else
                {
                    BattleScriptPush(BS_ptr);
                    gBattleStruct->scriptingActive = gActiveBattler;
                    gBattlescriptCurrInstr = BattleScript_MistProtected;
                    gSpecialStatuses[gActiveBattler].statLowered = 1;
                }
            }
            return STAT_CHANGE_DIDNT_WORK;
        }
        else if (gCurrentMove != MOVE_CURSE
                 && notProtectAffected != TRUE && JumpIfMoveAffectedByProtect(0))
        {
            gBattlescriptCurrInstr = BattleScript_ButItFailed;
            return STAT_CHANGE_DIDNT_WORK;
        }
        else if ((GetBattlerAbility(gActiveBattler) == ABILITY_CLEAR_BODY
                  || GetBattlerAbility(gActiveBattler) == ABILITY_WHITE_SMOKE)
                 && !certain && gCurrentMove != MOVE_CURSE)
        {
            if (flags == STAT_CHANGE_BS_PTR)
            {
                if (gSpecialStatuses[gActiveBattler].statLowered)
                {
                    gBattlescriptCurrInstr = BS_ptr;
                }
                else
                {
                    BattleScriptPush(BS_ptr);
                    gBattleStruct->scriptingActive = gActiveBattler;
                    gBattlescriptCurrInstr = BattleScript_AbilityNoStatLoss;
                    gLastUsedAbility = GetBattlerAbility(gActiveBattler);
                    RecordAbilityBattle(gActiveBattler, gLastUsedAbility);
                    gSpecialStatuses[gActiveBattler].statLowered = 1;
                }
            }
            return STAT_CHANGE_DIDNT_WORK;
        }
        else if (GetBattlerAbility(gActiveBattler) == ABILITY_KEEN_EYE
                 && !certain && statId == STAT_STAGE_ACC)
        {
            if (flags == STAT_CHANGE_BS_PTR)
            {
                BattleScriptPush(BS_ptr);
                gBattleStruct->scriptingActive = gActiveBattler;
                gBattlescriptCurrInstr = BattleScript_AbilityNoSpecificStatLoss;
                gLastUsedAbility = GetBattlerAbility(gActiveBattler);
                RecordAbilityBattle(gActiveBattler, gLastUsedAbility);
            }
            return STAT_CHANGE_DIDNT_WORK;
        }
        else if (GetBattlerAbility(gActiveBattler) == ABILITY_HYPER_CUTTER
                 && !certain && statId == STAT_STAGE_ATK)
        {
            if (flags == STAT_CHANGE_BS_PTR)
            {
                BattleScriptPush(BS_ptr);
                gBattleStruct->scriptingActive = gActiveBattler;
                gBattlescriptCurrInstr = BattleScript_AbilityNoSpecificStatLoss;
                gLastUsedAbility = GetBattlerAbility(gActiveBattler);
                RecordAbilityBattle(gActiveBattler, gLastUsedAbility);
            }
            return STAT_CHANGE_DIDNT_WORK;
        }
        else if (GetBattlerAbility(gActiveBattler) == ABILITY_SHIELD_DUST && flags == 0)
        {
            return STAT_CHANGE_DIDNT_WORK;
        }
        else // try to decrease
        {
            statValue = -GET_STAT_BUFF_VALUE(statValue);
            gBattleTextBuff2[0] = B_BUFF_PLACEHOLDER_BEGIN;
            index = 1;
            if (statValue == -2)
            {
                gBattleTextBuff2[1] = B_BUFF_STRING;
                gBattleTextBuff2[2] = STRINGID_STATHARSHLY;
                gBattleTextBuff2[3] = STRINGID_STATHARSHLY >> 8;
                index = 4;
            }
            else if (statValue <= -3)
            {
                gBattleTextBuff2[1] = B_BUFF_STRING;
                gBattleTextBuff2[2] = (u8) STRINGID_STATSEVERELY;
                gBattleTextBuff2[3] = STRINGID_STATSEVERELY >> 8;
                index = 4;
            }
            gBattleTextBuff2[index] = B_BUFF_STRING;
            index++;
            gBattleTextBuff2[index] = STRINGID_STATFELL;
            index++;
            gBattleTextBuff2[index] = STRINGID_STATFELL >> 8;
            index++;
            gBattleTextBuff2[index] = B_BUFF_EOS;

            if (gBattleMons[gActiveBattler].statStages[statId] == 0)
                gBattleCommunication[MULTISTRING_CHOOSER] = 2;
            else
                gBattleCommunication[MULTISTRING_CHOOSER] = (gBattlerTarget == gActiveBattler);

        }
    }
    else // stat increase
    {
        statValue = GET_STAT_BUFF_VALUE(statValue);
        gBattleTextBuff2[0] = B_BUFF_PLACEHOLDER_BEGIN;
        index = 1;
        if (statValue == 2)
        {
            gBattleTextBuff2[1] = B_BUFF_STRING;
            gBattleTextBuff2[2] = STRINGID_STATSHARPLY;
            gBattleTextBuff2[3] = STRINGID_STATSHARPLY >> 8;
            index = 4;
        }
        else if (statValue >= 3)
        {
            gBattleTextBuff2[1] = B_BUFF_STRING;
            gBattleTextBuff2[2] = (u8) STRINGID_STATDRASTICALLY;
            gBattleTextBuff2[3] = STRINGID_STATDRASTICALLY >> 8;
            index = 4;
        }
        gBattleTextBuff2[index] = B_BUFF_STRING;
        index++;
        gBattleTextBuff2[index] = STRINGID_STATROSE;
        index++;
        gBattleTextBuff2[index] = STRINGID_STATROSE >> 8;
        index++;
        gBattleTextBuff2[index] = B_BUFF_EOS;

        if (gBattleMons[gActiveBattler].statStages[statId] == 0xC)
            gBattleCommunication[MULTISTRING_CHOOSER] = 2;
        else
            gBattleCommunication[MULTISTRING_CHOOSER] = (gBattlerTarget == gActiveBattler);
    }

    gBattleMons[gActiveBattler].statStages[statId] += statValue;
    if (gBattleMons[gActiveBattler].statStages[statId] < 0)
        gBattleMons[gActiveBattler].statStages[statId] = 0;
    if (gBattleMons[gActiveBattler].statStages[statId] > 0xC)
        gBattleMons[gActiveBattler].statStages[statId] = 0xC;

    if (gBattleCommunication[MULTISTRING_CHOOSER] == 2 && flags & STAT_CHANGE_BS_PTR)
        gMoveResultFlags |= MOVE_RESULT_MISSED;

    if (gBattleCommunication[MULTISTRING_CHOOSER] == 2 && !(flags & STAT_CHANGE_BS_PTR))
        return STAT_CHANGE_DIDNT_WORK;

    return STAT_CHANGE_WORKED;
}

static void atk89_statbuffchange(void)
{
    u8* jump_loc = T1_READ_PTR(gBattlescriptCurrInstr + 2);
    if (ChangeStatBuffs(gBattleStruct->statChanger & 0xF0, gBattleStruct->statChanger & 0xF, T2_READ_8(gBattlescriptCurrInstr + 1), jump_loc) == 0)
        gBattlescriptCurrInstr += 6;
}

static void atk8A_normalisebuffs(void) //haze
{
    int i, j;
    for (i = 0; i < gBattlersCount; i++)
    {
        for (j = 0; j < 8; j++)
        {
            gBattleMons[i].statStages[j] = 6;
        }
    }
    gBattlescriptCurrInstr++;
}

static void atk8B_setbide(void)
{
    gBattleMons[gBattlerAttacker].status2 |= STATUS2_MULTIPLETURNS;
    gLockedMoves[gBattlerAttacker] = gCurrentMove;
    gTakenDmg[gBattlerAttacker] = 0;
    gBattleMons[gBattlerAttacker].status2 |= (STATUS2_BIDE - 0x100); //2 turns
    gBattlescriptCurrInstr++;
}

static void atk8C_confuseifrepeatingattackends(void)
{
    if (!(gBattleMons[gBattlerAttacker].status2 & STATUS2_LOCK_CONFUSE))
        gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_THRASH | MOVE_EFFECT_AFFECTS_USER;
    gBattlescriptCurrInstr++;
}

static void atk8D_setmultihitcounter(void)
{
    if (T2_READ_8(gBattlescriptCurrInstr + 1))
        gMultiHitCounter = T2_READ_8(gBattlescriptCurrInstr + 1);
    else
    {
        if (GetBattlerAbility(gBattlerAttacker) == ABILITY_SKILL_LINK)
            gMultiHitCounter = 5;
        else
        {
            gMultiHitCounter = Random() & 3;
            if (gMultiHitCounter > 1)
                gMultiHitCounter = (Random() & 3) + 2;
            else
                gMultiHitCounter += 2;
        }
    }
    gBattlescriptCurrInstr += 2;
}

static void atk8E_initmultihitstring(void)
{
    ewram160E0(0) = 0xFD;
    ewram160E0(1) = 1;
    ewram160E0(2) = 1;
    ewram160E0(3) = 1;
    ewram160E0(4) = 0;
    ewram160E0(5) = 0xFF;
    gBattlescriptCurrInstr++;
}

static bool8 sub_80264C0(void)
{
    if (gBattleMons[gBattlerAttacker].level >= gBattleMons[gBattlerTarget].level)
    {
        ewram16064arr(gBattlerTarget) = gBattlerPartyIndexes[gBattlerTarget];
    }
    else
    {
        u16 random = Random() & 0xFF;
        if ((u32)((random * (gBattleMons[gBattlerAttacker].level + gBattleMons[gBattlerTarget].level) >> 8) + 1) <= (gBattleMons[gBattlerTarget].level / 4))
        {
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
            return 0;
        }
        ewram16064arr(gBattlerTarget) = gBattlerPartyIndexes[gBattlerTarget];
    }
    gBattlescriptCurrInstr = BattleScript_SuccessForceOut;
    return 1;
}

static void atk8F_forcerandomswitch(void)
{
    if ((gBattleTypeFlags & BATTLE_TYPE_TRAINER))
    {
        u8 i;
        struct Pokemon* party;
        u8 valid;
        u8 val;
        if (!GetBattlerSide(gBattlerTarget))
            party = gPlayerParty;
        else
            party = gEnemyParty;

        if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
        {
            valid = 0;
            val = 0;
            if (sub_803FBFC(sub_803FC34(gBattlerTarget)) == 1)
                val = 3;
            for (i = val; i < val + 3; i++)
            {
                if (GetMonData(&party[i], MON_DATA_SPECIES) != SPECIES_NONE
                 && !GetMonData(&party[i], MON_DATA_IS_EGG)
                 && GetMonData(&party[i], MON_DATA_HP) != 0)
                    valid++;
            }
        }
        else
        {
            valid = 0;
            for (i = 0; i < 6; i++)
            {
                if (GetMonData(&party[i], MON_DATA_SPECIES) != SPECIES_NONE
                 && !GetMonData(&party[i], MON_DATA_IS_EGG)
                 && GetMonData(&party[i], MON_DATA_HP) != 0)
                    valid++;
            }
        }

        if ((valid < 2 && (gBattleTypeFlags & (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_MULTI)) != BATTLE_TYPE_DOUBLE)
         || (valid < 3 && (gBattleTypeFlags & BATTLE_TYPE_DOUBLE) && !(gBattleTypeFlags & BATTLE_TYPE_MULTI)))
        {
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
        }
        else if (sub_80264C0())
        {
#define MON_CAN_BATTLE(mon) (((GetMonData(mon, MON_DATA_SPECIES) && GetMonData(mon, MON_DATA_IS_EGG) != 1 && GetMonData(mon, MON_DATA_HP))))
            if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
            {
                do
                {
                    val = Random() % 3;
                    if (sub_803FBFC(sub_803FC34(gBattlerTarget)) == 1)
                        i = val + 3;
                    else
                        i = val;
                } while (i == gBattlerPartyIndexes[gBattlerTarget] || i == gBattlerPartyIndexes[gBattlerTarget ^ 2] || !MON_CAN_BATTLE(&party[i]));
            }
            else
            {
                if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
                {
                    do
                    {
                        i = Random() % 6;
                    } while (i == gBattlerPartyIndexes[gBattlerTarget] || i == gBattlerPartyIndexes[gBattlerTarget ^ 2] || !MON_CAN_BATTLE(&party[i]));
                }
                else
                {
                    do
                    {
                        i = Random() % 6;
                    } while (i == gBattlerPartyIndexes[gBattlerTarget] || !MON_CAN_BATTLE(&party[i]));
                }
            }
            ewram16068arr(gBattlerTarget) = i;
            if (!IsLinkDoubleBattle())
                sub_8012258(gBattlerTarget);
            sub_8094B6C(gBattlerTarget, i, 0);
            sub_8094B6C(gBattlerTarget ^ 2, i, 1);
#undef MON_CAN_BATTLE
        }
    }
    else
    {
        sub_80264C0();
    }
}

static void atk90_tryconversiontypechange(void)
{
    //randomly changes user's type to one of its moves' type
    u8 valid_moves = 0;
    u8 checked_move;
    u8 moveType;
    while (valid_moves < 4)
    {
        if (gBattleMons[gBattlerAttacker].moves[valid_moves] == 0)
            break;
        valid_moves++;
    }

    for (checked_move = 0; checked_move < valid_moves; checked_move++)
    {
        moveType = gBattleMoves[gBattleMons[gBattlerAttacker].moves[checked_move]].type;
        if (moveType == TYPE_MYSTERY)
        {
            if (gBattleMons[gBattlerAttacker].type1 == TYPE_GHOST || gBattleMons[gBattlerAttacker].type2 == TYPE_GHOST)
                moveType = TYPE_GHOST;
            else
                moveType = TYPE_NORMAL;
        }
        if (moveType != gBattleMons[gBattlerAttacker].type1 && moveType != gBattleMons[gBattlerAttacker].type2)
            break;
    }

    if (checked_move == valid_moves)
    {
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
    else
    {
        do
        {

            while ((checked_move = Random() & 3) >= valid_moves);

            moveType = gBattleMoves[gBattleMons[gBattlerAttacker].moves[checked_move]].type;
            if (moveType == TYPE_MYSTERY)
            {
                if (gBattleMons[gBattlerAttacker].type1 == TYPE_GHOST || gBattleMons[gBattlerAttacker].type2 == TYPE_GHOST)
                    moveType = TYPE_GHOST;
                else
                    moveType = TYPE_NORMAL;
            }
        } while (moveType == gBattleMons[gBattlerAttacker].type1 || moveType == gBattleMons[gBattlerAttacker].type2);

        gBattleMons[gBattlerAttacker].type1 = moveType;
        gBattleMons[gBattlerAttacker].type2 = moveType;

        gBattleTextBuff1[0] = 0xFD;
        gBattleTextBuff1[1] = 3;
        gBattleTextBuff1[2] = moveType;
        gBattleTextBuff1[3] = 0xFF;

        gBattlescriptCurrInstr += 5;
    }
}

static void atk91_givepaydaymoney(void)
{
    if (!(gBattleTypeFlags & BATTLE_TYPE_LINK) && gPaydayMoney)
    {
        AddMoney(&gSaveBlock1.money, gPaydayMoney * gBattleStruct->moneyMultiplier);
        gBattleTextBuff1[0] = 0xFD;
        gBattleTextBuff1[1] = 1;
        gBattleTextBuff1[2] = 2;
        gBattleTextBuff1[3] = 5;
        gBattleTextBuff1[4] = gPaydayMoney;
        gBattleTextBuff1[5] = uBYTE1_16(gPaydayMoney);
        gBattleTextBuff1[6] = 0xFF;
        BattleScriptPush(gBattlescriptCurrInstr + 1);
        gBattlescriptCurrInstr = BattleScript_PrintPayDayMoneyString;
    }
    else
        gBattlescriptCurrInstr++;
}

static void atk92_setlightscreen(void)
{
    if (gSideStatuses[GetBattlerPosition(gBattlerAttacker) & 1] & SIDE_STATUS_LIGHTSCREEN)
    {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
    }
    else
    {
        gSideStatuses[GetBattlerPosition(gBattlerAttacker) & 1] |= SIDE_STATUS_LIGHTSCREEN;
        if (ItemId_GetHoldEffect(gBattleMons[gBattlerAttacker].item) == HOLD_EFFECT_EXTEND_SCREENS) 
            gSideTimers[GetBattlerPosition(gBattlerAttacker) & 1].lightscreenTimer = 8;
        else
            gSideTimers[GetBattlerPosition(gBattlerAttacker) & 1].lightscreenTimer = 5;
        if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && CountAliveMons(1) == 2)
            gBattleCommunication[MULTISTRING_CHOOSER] = 4;
        else
            gBattleCommunication[MULTISTRING_CHOOSER] = 3;
    }
    gBattlescriptCurrInstr++;
}

static void atk93_tryKO(void)
{
    bool32 lands = FALSE;
    u32 holdEffect = ItemId_GetHoldEffect(gBattleMons[gBattlerTarget].item);

    gPotentialItemEffectBattler = gBattlerTarget;
    if (holdEffect == HOLD_EFFECT_FOCUS_BAND
        && (Random() % 100) < ItemId_GetHoldEffectParam(gBattleMons[gBattlerTarget].item))
    {
        gSpecialStatuses[gBattlerTarget].focusBanded = 1;
        RecordItemEffectBattle(gBattlerTarget, holdEffect);
    }
    else if (holdEffect == HOLD_EFFECT_FOCUS_SASH && gBattleMons[gBattlerTarget].maxHP)
    {
        gSpecialStatuses[gBattlerTarget].focusSashed = 1;
        RecordItemEffectBattle(gBattlerTarget, holdEffect);
    }

    if (GetBattlerAbility(gBattlerTarget) == ABILITY_STURDY)
    {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gLastUsedAbility = ABILITY_STURDY;
        gBattlescriptCurrInstr = BattleScript_SturdyPreventsOHKO;
    }
    else
    {
        if ((((gStatuses3[gBattlerTarget] & STATUS3_ALWAYS_HITS)
                && gDisableStructs[gBattlerTarget].battlerWithSureHit == gBattlerAttacker)
            || GetBattlerAbility(gBattlerAttacker) == ABILITY_NO_GUARD
            || GetBattlerAbility(gBattlerTarget) == ABILITY_NO_GUARD)
            && gBattleMons[gBattlerAttacker].level >= gBattleMons[gBattlerTarget].level)
        {
            lands = TRUE;
        }
        else
        {
            u16 odds = gBattleMoves[gCurrentMove].accuracy + (gBattleMons[gBattlerAttacker].level - gBattleMons[gBattlerTarget].level);
            if (Random() % 100 + 1 < odds && gBattleMons[gBattlerAttacker].level >= gBattleMons[gBattlerTarget].level)
                lands = TRUE;
        }

        if (lands)
        {
            if (gProtectStructs[gBattlerTarget].endured)
            {
                gBattleMoveDamage = gBattleMons[gBattlerTarget].hp - 1;
                gMoveResultFlags |= MOVE_RESULT_FOE_ENDURED;
            }
            else if (gSpecialStatuses[gBattlerTarget].focusBanded || gSpecialStatuses[gBattlerTarget].focusSashed)
            {
                gBattleMoveDamage = gBattleMons[gBattlerTarget].hp - 1;
                gMoveResultFlags |= MOVE_RESULT_FOE_HUNG_ON;
                gLastUsedItem = gBattleMons[gBattlerTarget].item;
            }
            else
            {
                gBattleMoveDamage = gBattleMons[gBattlerTarget].hp;
                gMoveResultFlags |= MOVE_RESULT_ONE_HIT_KO;
            }
            gBattlescriptCurrInstr += 5;
        }
        else
        {
            gMoveResultFlags |= MOVE_RESULT_MISSED;
            if (gBattleMons[gBattlerAttacker].level >= gBattleMons[gBattlerTarget].level)
                gBattleCommunication[MULTISTRING_CHOOSER] = 0;
            else
                gBattleCommunication[MULTISTRING_CHOOSER] = 1;
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
        }
    }
}

static void atk94_damagetohalftargethp(void) //super fang
{
    gBattleMoveDamage = gBattleMons[gBattlerTarget].hp / 2;
    if (gBattleMoveDamage == 0)
        gBattleMoveDamage = 1;
    gBattlescriptCurrInstr++;
}

static void atk95_setsandstorm(void)
{
    if (gBattleWeather & WEATHER_SANDSTORM_ANY)
    {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 2;
    }
    else
    {
        gBattleWeather = WEATHER_SANDSTORM_TEMPORARY;
        gBattleCommunication[MULTISTRING_CHOOSER] = 3;
        if (ItemId_GetHoldEffect(gBattleMons[gBattlerAttacker].item) == HOLD_EFFECT_EXTEND_SANDSTORM)
            gWishFutureKnock.weatherDuration = 8;
        else
            gWishFutureKnock.weatherDuration = 5;
    }
    gBattlescriptCurrInstr++;
}

static void atk96_weatherdamage(void)
{
    if (WEATHER_HAS_EFFECT)
    {
        if (gBattleWeather & WEATHER_SANDSTORM_ANY)
        {
            if (gBattleMons[gBattlerAttacker].type1 != TYPE_ROCK && gBattleMons[gBattlerAttacker].type1 != TYPE_STEEL && gBattleMons[gBattlerAttacker].type1 != TYPE_GROUND
             && gBattleMons[gBattlerAttacker].type2 != TYPE_ROCK && gBattleMons[gBattlerAttacker].type2 != TYPE_STEEL && gBattleMons[gBattlerAttacker].type2 != TYPE_GROUND
             && GetBattlerAbility(gBattlerAttacker) != ABILITY_MAGIC_GUARD && GetBattlerAbility(gBattlerAttacker) != ABILITY_SAND_VEIL && !(gStatuses3[gBattlerAttacker] & STATUS3_UNDERGROUND) && !(gStatuses3[gBattlerAttacker] & STATUS3_UNDERWATER))
            {
                gBattleMoveDamage = gBattleMons[gBattlerAttacker].maxHP / 16;
                if (gBattleMoveDamage == 0)
                    gBattleMoveDamage = 1;
            }
            else
            {
                gBattleMoveDamage = 0;
            }
        }
        if (gBattleWeather & WEATHER_HAIL)
        {
            if (gBattleMons[gBattlerAttacker].type1 != TYPE_ICE && gBattleMons[gBattlerAttacker].type2 != TYPE_ICE
             && GetBattlerAbility(gBattlerAttacker) != ABILITY_MAGIC_GUARD && !(gStatuses3[gBattlerAttacker] & STATUS3_UNDERGROUND) && !(gStatuses3[gBattlerAttacker] & STATUS3_UNDERWATER))
            {
                gBattleMoveDamage = gBattleMons[gBattlerAttacker].maxHP / 16;
                if (gBattleMoveDamage == 0)
                    gBattleMoveDamage = 1;
            }
            else
            {
                gBattleMoveDamage = 0;
            }
        }
    }
    else
        gBattleMoveDamage = 0;

    if (gAbsentBattlerFlags & gBitTable[gBattlerAttacker])
        gBattleMoveDamage = 0;

    gBattlescriptCurrInstr++;
}

static void atk97_tryinfatuating(void)
{
    struct Pokemon *attacker, *target;
    u16 atk_species, def_species;
    u32 atk_pid, def_pid;
    if (!GetBattlerSide(gBattlerAttacker))
        attacker = &gPlayerParty[gBattlerPartyIndexes[gBattlerAttacker]];
    else
        attacker = &gEnemyParty[gBattlerPartyIndexes[gBattlerAttacker]];

    if (!GetBattlerSide(gBattlerTarget))
        target = &gPlayerParty[gBattlerPartyIndexes[gBattlerTarget]];
    else
        target = &gEnemyParty[gBattlerPartyIndexes[gBattlerTarget]];

    atk_species = GetMonData(attacker, MON_DATA_SPECIES);
    atk_pid = GetMonData(attacker, MON_DATA_PERSONALITY);

    def_species = GetMonData(target, MON_DATA_SPECIES);
    def_pid = GetMonData(target, MON_DATA_PERSONALITY);

    if (GetBattlerAbility(gBattlerTarget) == ABILITY_OBLIVIOUS)
    {
        gBattlescriptCurrInstr = BattleScript_ObliviousPreventsAttraction;
        gLastUsedAbility = ABILITY_OBLIVIOUS;
        RecordAbilityBattle(gBattlerTarget, ABILITY_OBLIVIOUS);
    }
    else
    {
        if (GetGenderFromSpeciesAndPersonality(atk_species, atk_pid) == GetGenderFromSpeciesAndPersonality(def_species, def_pid)
            || gStatuses3[gBattlerTarget] & STATUS3_SEMI_INVULNERABLE || gBattleMons[gBattlerTarget].status2 & STATUS2_INFATUATION || GetGenderFromSpeciesAndPersonality(atk_species, atk_pid) == 0xFF
            || GetGenderFromSpeciesAndPersonality(def_species, def_pid) == 0xFF)
            {
                gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
            }
        else
        {
            gBattleMons[gBattlerTarget].status2 |= (gBitTable[gBattlerAttacker] << 16);
            if (ItemId_GetHoldEffect(gBattleMons[gBattlerTarget].item) == HOLD_EFFECT_DESTINY_KNOT && GetBattlerAbility(gBattlerAttacker) != ABILITY_OBLIVIOUS && !(gBattleMons[gBattlerAttacker].status2 & STATUS2_INFATUATION))
                gBattleMons[gBattlerAttacker].status2 |= (gBitTable[gBattlerTarget] << 16);
            gBattlescriptCurrInstr += 5;
        }
    }
}

static void atk98_updatestatusicon(void)
{
    if (gBattleControllerExecFlags)
        return;

    if (T2_READ_8(gBattlescriptCurrInstr + 1) != 4)
    {
        gActiveBattler = GetBattlerForBattleScript(T2_READ_8(gBattlescriptCurrInstr + 1));
        BtlController_EmitStatusIconUpdate(0, gBattleMons[gActiveBattler].status1, gBattleMons[gActiveBattler].status2);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattlescriptCurrInstr += 2;
    }
    else
    {
        gActiveBattler = gBattlerAttacker;
        if (!(gAbsentBattlerFlags & gBitTable[gActiveBattler]))
        {
            BtlController_EmitStatusIconUpdate(0, gBattleMons[gActiveBattler].status1, gBattleMons[gActiveBattler].status2);
            MarkBattlerForControllerExec(gActiveBattler);
        }
        if ((gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
        {
            gActiveBattler = GetBattlerAtPosition(GetBattlerPosition(gBattlerAttacker) ^ 2);
            if (!(gAbsentBattlerFlags & gBitTable[gActiveBattler]))
            {
                BtlController_EmitStatusIconUpdate(0, gBattleMons[gActiveBattler].status1, gBattleMons[gActiveBattler].status2);
                MarkBattlerForControllerExec(gActiveBattler);
            }
        }
        gBattlescriptCurrInstr += 2;
    }
}

static void atk99_setmist(void)
{
    if (gSideTimers[GetBattlerPosition(gBattlerAttacker) & 1].mistTimer)
    {
        gMoveResultFlags |= MOVE_RESULT_FAILED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 1;
    }
    else
    {
        gSideTimers[GetBattlerPosition(gBattlerAttacker) & 1].mistTimer = 5;
        gSideStatuses[GetBattlerPosition(gBattlerAttacker) & 1] |= SIDE_STATUS_MIST;
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
    }
    gBattlescriptCurrInstr++;
}

static void atk9A_setfocusenergy(void)
{
    if (gBattleMons[gBattlerAttacker].status2 & STATUS2_FOCUS_ENERGY)
    {
        gMoveResultFlags |= MOVE_RESULT_FAILED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 1;
    }
    else
    {
        gBattleMons[gBattlerAttacker].status2 |= STATUS2_FOCUS_ENERGY;
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
    }
    gBattlescriptCurrInstr++;
}

static void atk9B_transformdataexecution(void)
{
    gChosenMove = 0xFFFF;
    gBattlescriptCurrInstr++;
    if (gBattleMons[gBattlerTarget].status2 & STATUS2_TRANSFORMED || gStatuses3[gBattlerTarget] & STATUS3_SEMI_INVULNERABLE)
    {
        gMoveResultFlags |= MOVE_RESULT_FAILED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 1;
    }
    else
    {
        u32 i; s32 j;
        u8 *atk_data, *def_data;
        gBattleMons[gBattlerAttacker].status2 |= STATUS2_TRANSFORMED;
        gDisableStructs[gBattlerAttacker].disabledMove = 0;
        gDisableStructs[gBattlerAttacker].disableTimer1 = 0;
        gDisableStructs[gBattlerAttacker].transformedMonPersonality = gBattleMons[gBattlerTarget].personality;
        gDisableStructs[gBattlerAttacker].mimickedMoves = 0;
        gDisableStructs[gBattlerAttacker].usedMoves = 0;

        gBattleTextBuff1[0] = 0xFD;
        gBattleTextBuff1[1] = 6;
        gBattleTextBuff1[2] = (gBattleMons[gBattlerTarget].species);
        gBattleTextBuff1[3] = uBYTE1_16(gBattleMons[gBattlerTarget].species);
        gBattleTextBuff1[4] = 0xFF;

        atk_data = (u8*)(&gBattleMons[gBattlerAttacker]);
        def_data = (u8*)(&gBattleMons[gBattlerTarget]);

        for (i = 0; i < 0x24; i++)
            atk_data[i] = def_data[i];

        for (j = 0; j < 4; j++)
        {
            if (gBattleMoves[gBattleMons[gBattlerAttacker].moves[j]].pp < 5)
                gBattleMons[gBattlerAttacker].pp[j] = gBattleMoves[gBattleMons[gBattlerAttacker].moves[j]].pp;
            else
                gBattleMons[gBattlerAttacker].pp[j] = 5;
        }

        gActiveBattler = gBattlerAttacker;
        BtlController_EmitResetActionMoveSelection(0, 2);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
    }
}

static void atk9C_setsubstitute(void)
{
    u32 hp = gBattleMons[gBattlerAttacker].maxHP / 4;
    if (gBattleMons[gBattlerAttacker].maxHP / 4 == 0)
        hp = 1;
    if (gBattleMons[gBattlerAttacker].hp <= hp)
    {
        gBattleMoveDamage = 0;
        gBattleCommunication[MULTISTRING_CHOOSER] = 1;
    }
    else
    {
        gBattleMoveDamage = gBattleMons[gBattlerAttacker].maxHP / 4;
        if (gBattleMoveDamage == 0)
            gBattleMoveDamage = 1;
        gBattleMons[gBattlerAttacker].status2 |= STATUS2_SUBSTITUTE;
        gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_WRAPPED);
        gDisableStructs[gBattlerAttacker].substituteHP = gBattleMoveDamage;
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
        gHitMarker |= HITMARKER_IGNORE_SUBSTITUTE;
    }
    gBattlescriptCurrInstr++;
}

static bool8 IsMoveUncopyable(u16 move)
{
    int i;
    for (i = 0; sMovesForbiddenToCopy[i] != 0xFFFE && sMovesForbiddenToCopy[i] != move; i++) {}
    return (sMovesForbiddenToCopy[i] != 0xFFFE);
}

static void atk9D_mimicattackcopy(void)
{
    gChosenMove = 0xFFFF;
    if (IsMoveUncopyable(gLastMoves[gBattlerTarget]) || gBattleMons[gBattlerAttacker].status2 & STATUS2_TRANSFORMED
        || gLastMoves[gBattlerTarget] == 0 || gLastMoves[gBattlerTarget] == 0xFFFF)
    {
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
    else
    {
        int i;
        for (i = 0; i < 4; i++) // already has move
        {
            if (gBattleMons[gBattlerAttacker].moves[i] == gLastMoves[gBattlerTarget])
                break;
        }
        if (i == 4)
        {
            gBattleMons[gBattlerAttacker].moves[gCurrMovePos] = gLastMoves[gBattlerTarget];
            if (gBattleMoves[gLastMoves[gBattlerTarget]].pp < 5)
                gBattleMons[gBattlerAttacker].pp[gCurrMovePos] = gBattleMoves[gLastMoves[gBattlerTarget]].pp;
            else
                gBattleMons[gBattlerAttacker].pp[gCurrMovePos] = 5;

            gBattleTextBuff1[0] = 0xFD;
            gBattleTextBuff1[1] = 2;
            gBattleTextBuff1[2] = gLastMoves[gBattlerTarget];
            gBattleTextBuff1[3] = uBYTE1_16(gLastMoves[gBattlerTarget]);
            gBattleTextBuff1[4] = 0xFF;

            gDisableStructs[gBattlerAttacker].mimickedMoves |= gBitTable[gCurrMovePos];
            gBattlescriptCurrInstr += 5;
        }
        else
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
}

#if DEBUG
NAKED
static void atk9E_metronome(void)
{
    asm("\
    push    {r4, r5, r6, r7, lr}\n\
    mov r7, r8\n\
    push    {r7}\n\
    ldr r6, ._3076      @ gBattlerAttacker\n\
    ldrb    r2, [r6]\n\
    lsl r1, r2, #0x1\n\
    ldr r0, ._3076 + 4  @ \n\
    add r3, r1, r0\n\
    ldr r5, ._3076 + 8  @ \n\
    mov r4, #0x58\n\
    add r0, r2, #0\n\
    mul r0, r0, r4\n\
    add r1, r0, r5\n\
    ldrh    r0, [r1, #0xe]\n\
    cmp r0, #0\n\
    bne ._3071  @cond_branch\n\
    ldrh    r2, [r1, #0x10]\n\
    cmp r2, #0\n\
    beq ._3071  @cond_branch\n\
    ldrh    r0, [r1, #0x12]\n\
    cmp r0, #0\n\
    beq ._3071  @cond_branch\n\
    ldrh    r0, [r3]\n\
    cmp r0, #0\n\
    bne ._3072  @cond_branch\n\
    strh    r2, [r3]\n\
._3072:\n\
    ldr r1, ._3076 + 12 @ \n\
    ldrh    r0, [r3]\n\
    strh    r0, [r1]\n\
    ldrb    r0, [r6]\n\
    mul r0, r0, r4\n\
    add r0, r0, r5\n\
    ldrh    r5, [r0, #0x10]\n\
    ldrh    r2, [r0, #0x12]\n\
    add r7, r1, #0\n\
    cmp r5, r2\n\
    bcs ._3073  @cond_branch\n\
    ldrh    r0, [r3]\n\
    cmp r0, r2\n\
    beq ._3074  @cond_branch\n\
    add r0, r0, #0x1\n\
    b   ._3079\n\
._3077:\n\
    .align  2, 0\n\
._3076:\n\
    .word   gBattlerAttacker\n\
    .word   +0x20160b4\n\
    .word   gBattleMons\n\
    .word   gCurrentMove\n\
._3073:\n\
    ldrh    r4, [r3]\n\
    add r1, r4, #0\n\
    mov r0, #0xb1\n\
    lsl r0, r0, #0x1\n\
    cmp r1, r0\n\
    bne ._3078  @cond_branch\n\
    mov r0, #0x1\n\
    b   ._3079\n\
._3078:\n\
    cmp r1, r2\n\
    bne ._3080  @cond_branch\n\
._3074:\n\
    strh    r5, [r3]\n\
    b   ._3081\n\
._3080:\n\
    add r0, r4, #1\n\
._3079:\n\
    strh    r0, [r3]\n\
._3081:\n\
    ldr r4, ._3083      @ gHitMarker\n\
    ldr r2, [r4]\n\
    ldr r0, ._3083 + 4  @ 0xfffffbff\n\
    and r2, r2, r0\n\
    str r2, [r4]\n\
    ldr r6, ._3083 + 8  @ gBattlescriptCurrInstr\n\
    ldr r5, ._3083 + 12 @ gBattleScriptsForMoveEffects\n\
    ldr r3, ._3083 + 16 @ gBattleMoves\n\
    ldrh    r1, [r7]\n\
    lsl r0, r1, #0x1\n\
    add r0, r0, r1\n\
    lsl r0, r0, #0x2\n\
    add r0, r0, r3\n\
    ldrb    r0, [r0]\n\
    lsl r0, r0, #0x2\n\
    add r0, r0, r5\n\
    ldr r0, [r0]\n\
    str r0, [r6]\n\
    mov r0, #0x80\n\
    lsl r0, r0, #0x4\n\
    orr r2, r2, r0\n\
    str r2, [r4]\n\
    ldrh    r0, [r7]\n\
    b   ._3082\n\
._3084:\n\
    .align  2, 0\n\
._3083:\n\
    .word   gHitMarker\n\
    .word   0xfffffbff\n\
    .word   gBattlescriptCurrInstr\n\
    .word   gBattleScriptsForMoveEffects\n\
    .word   gBattleMoves\n\
._3071:\n\
    ldr r7, ._3090      @ gCurrentMove\n\
    mov r6, #0xb1\n\
    lsl r6, r6, #0x1\n\
    ldr r5, ._3090 + 4  @ sMovesForbiddenToCopy\n\
    ldr r0, ._3090 + 8  @ gBattlescriptCurrInstr\n\
    mov r8, r0\n\
._3089:\n\
    bl  Random\n\
    ldr r2, ._3090 + 12 @ 0x1ff\n\
    add r1, r2, #0\n\
    and r0, r0, r1\n\
    add r0, r0, #0x1\n\
    strh    r0, [r7]\n\
    cmp r0, r6\n\
    bhi ._3089  @cond_branch\n\
    mov r0, #0x3\n\
._3086:\n\
    sub r0, r0, #0x1\n\
    cmp r0, #0\n\
    bge ._3086  @cond_branch\n\
    ldr r4, ._3090      @ gCurrentMove\n\
    ldrh    r2, [r4]\n\
    ldr r3, ._3090 + 16 @ 0xffff\n\
    sub r0, r5, #2\n\
._3088:\n\
    add r0, r0, #0x2\n\
    ldrh    r1, [r0]\n\
    cmp r1, r2\n\
    beq ._3087  @cond_branch\n\
    cmp r1, r3\n\
    bne ._3088  @cond_branch\n\
._3087:\n\
    ldr r0, ._3090 + 16 @ 0xffff\n\
    cmp r1, r0\n\
    bne ._3089  @cond_branch\n\
    ldr r2, ._3090 + 20 @ gHitMarker\n\
    ldr r0, [r2]\n\
    ldr r1, ._3090 + 24 @ 0xfffffbff\n\
    and r0, r0, r1\n\
    str r0, [r2]\n\
    ldr r3, ._3090 + 28 @ gBattleScriptsForMoveEffects\n\
    ldr r2, ._3090 + 32 @ gBattleMoves\n\
    ldrh    r1, [r4]\n\
    lsl r0, r1, #0x1\n\
    add r0, r0, r1\n\
    lsl r0, r0, #0x2\n\
    add r0, r0, r2\n\
    ldrb    r0, [r0]\n\
    lsl r0, r0, #0x2\n\
    add r0, r0, r3\n\
    ldr r0, [r0]\n\
    mov r1, r8\n\
    str r0, [r1]\n\
    ldrh    r0, [r4]\n\
._3082:\n\
    mov r1, #0x0\n\
    bl  GetMoveTarget\n\
    ldr r1, ._3090 + 36 @ gBattlerTarget\n\
    strb    r0, [r1]\n\
    pop {r3}\n\
    mov r8, r3\n\
    pop {r4, r5, r6, r7}\n\
    pop {r0}\n\
    bx  r0\n\
._3091:\n\
    .align  2, 0\n\
._3090:\n\
    .word   gCurrentMove\n\
    .word   sMovesForbiddenToCopy\n\
    .word   gBattlescriptCurrInstr\n\
    .word   0x1ff\n\
    .word   0xffff\n\
    .word   gHitMarker\n\
    .word   0xfffffbff\n\
    .word   gBattleScriptsForMoveEffects\n\
    .word   gBattleMoves\n\
    .word   gBattlerTarget");
}
#else
#ifdef NONMATCHING
static void atk9E_metronome(void)
{
    // sMovesForbiddenToCopy
    int i;
    do
    {
        while ((gCurrentMove = (Random() & 0x1FF) + 1) > 0x162);
        for (i = 0; sMovesForbiddenToCopy[i] != gCurrentMove && sMovesForbiddenToCopy[i] != 0xFFFF; i++);
    } while (sMovesForbiddenToCopy[i] != 0xFFFF);

    gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);
    gBattlescriptCurrInstr = gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect];
    gBattlerTarget = GetMoveTarget(gCurrentMove, 0);
}

#else
NAKED
static void atk9E_metronome(void)
{
    asm(".syntax unified\n\
    push {r4-r7,lr}\n\
    mov r7, r8\n\
    push {r7}\n\
    ldr r7, _08027938 @ =gCurrentMove\n\
    movs r6, 0xB1\n\
    lsls r6, 1\n\
    ldr r5, _0802793C @ =sMovesForbiddenToCopy\n\
    ldr r0, _08027940 @ =gBattlescriptCurrInstr\n\
    mov r8, r0\n\
_080278CA:\n\
    bl Random\n\
    ldr r2, _08027944 @ =0x000001ff\n\
    adds r1, r2, 0\n\
    ands r0, r1\n\
    adds r0, 0x1\n\
    strh r0, [r7]\n\
    cmp r0, r6\n\
    bhi _080278CA\n\
    movs r0, 0x3\n\
_080278DE:\n\
    subs r0, 0x1\n\
    cmp r0, 0\n\
    bge _080278DE\n\
    ldr r4, _08027938 @ =gCurrentMove\n\
    ldrh r2, [r4]\n\
    ldr r3, _08027948 @ =0x0000ffff\n\
    subs r0, r5, 0x2\n\
_080278EC:\n\
    adds r0, 0x2\n\
    ldrh r1, [r0]\n\
    cmp r1, r2\n\
    beq _080278F8\n\
    cmp r1, r3\n\
    bne _080278EC\n\
_080278F8:\n\
    ldr r0, _08027948 @ =0x0000ffff\n\
    cmp r1, r0\n\
    bne _080278CA\n\
    ldr r2, _0802794C @ =gHitMarker\n\
    ldr r0, [r2]\n\
    ldr r1, _08027950 @ =0xfffffbff\n\
    ands r0, r1\n\
    str r0, [r2]\n\
    ldr r3, _08027954 @ =gBattleScriptsForMoveEffects\n\
    ldr r2, _08027958 @ =gBattleMoves\n\
    ldrh r1, [r4]\n\
    lsls r0, r1, 1\n\
    adds r0, r1\n\
    lsls r0, 2\n\
    adds r0, r2\n\
    ldrb r0, [r0]\n\
    lsls r0, 2\n\
    adds r0, r3\n\
    ldr r0, [r0]\n\
    mov r1, r8\n\
    str r0, [r1]\n\
    ldrh r0, [r4]\n\
    movs r1, 0\n\
    bl GetMoveTarget\n\
    ldr r1, _0802795C @ =gBattlerTarget\n\
    strb r0, [r1]\n\
    pop {r3}\n\
    mov r8, r3\n\
    pop {r4-r7}\n\
    pop {r0}\n\
    bx r0\n\
    .align 2, 0\n\
_08027938: .4byte gCurrentMove\n\
_0802793C: .4byte sMovesForbiddenToCopy\n\
_08027940: .4byte gBattlescriptCurrInstr\n\
_08027944: .4byte 0x000001ff\n\
_08027948: .4byte 0x0000ffff\n\
_0802794C: .4byte gHitMarker\n\
_08027950: .4byte 0xfffffbff\n\
_08027954: .4byte gBattleScriptsForMoveEffects\n\
_08027958: .4byte gBattleMoves\n\
_0802795C: .4byte gBattlerTarget\n\
        .syntax divided");
}
#endif // NONMATCHING
#endif

static void atk9F_dmgtolevel(void)
{
    gBattleMoveDamage = gBattleMons[gBattlerAttacker].level;
    gBattlescriptCurrInstr++;
}

static void atkA0_psywavedamageeffect(void)
{
    s32 rand_dmg;
    while ((rand_dmg = (Random() & 0xF)) > 0xA);
    rand_dmg *= 10;
    gBattleMoveDamage = gBattleMons[gBattlerAttacker].level * (rand_dmg + 50) / 100;
    gBattlescriptCurrInstr++;
}

static void atkA1_counterdamagecalculator(void)
{
    u8 atk_side = GetBattlerSide(gBattlerAttacker);
    u8 def_side = GetBattlerSide(gProtectStructs[gBattlerAttacker].physicalBattlerId);
    if (gProtectStructs[gBattlerAttacker].physicalDmg && atk_side != def_side && gBattleMons[gProtectStructs[gBattlerAttacker].physicalBattlerId].hp)
    {
        gBattleMoveDamage = gProtectStructs[gBattlerAttacker].physicalDmg * 2;
        if (gSideTimers[def_side].followmeTimer && gBattleMons[gSideTimers[def_side].followmeTarget].hp)
            gBattlerTarget = gSideTimers[def_side].followmeTarget;
        else
            gBattlerTarget = gProtectStructs[gBattlerAttacker].physicalBattlerId;
        gBattlescriptCurrInstr += 5;
    }
    else
    {
        gSpecialStatuses[gBattlerAttacker].ppNotAffectedByPressure = 1;
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
}

static void atkA2_mirrorcoatdamagecalculator(void) //a copy of atkA1 with the physical -> special field changes
{
    u8 atk_side = GetBattlerSide(gBattlerAttacker);
    u8 def_side = GetBattlerSide(gProtectStructs[gBattlerAttacker].specialBattlerId);
    if (gProtectStructs[gBattlerAttacker].specialDmg && atk_side != def_side && gBattleMons[gProtectStructs[gBattlerAttacker].specialBattlerId].hp)
    {
        gBattleMoveDamage = gProtectStructs[gBattlerAttacker].specialDmg * 2;
        if (gSideTimers[def_side].followmeTimer && gBattleMons[gSideTimers[def_side].followmeTarget].hp)
            gBattlerTarget = gSideTimers[def_side].followmeTarget;
        else
            gBattlerTarget = gProtectStructs[gBattlerAttacker].specialBattlerId;
        gBattlescriptCurrInstr += 5;
    }
    else
    {
        gSpecialStatuses[gBattlerAttacker].ppNotAffectedByPressure = 1;
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
}

static void atkA3_disablelastusedattack(void)
{
    int i;
    for (i = 0; i < 4; i++)
    {
        if (gBattleMons[gBattlerTarget].moves[i] == gLastMoves[gBattlerTarget])
            break;
    }
    if (gDisableStructs[gBattlerTarget].disabledMove == 0 && i != 4 && gBattleMons[gBattlerTarget].pp[i] != 0)
    {
        gBattleTextBuff1[0] = 0xFD;
        gBattleTextBuff1[1] = 2;
        gBattleTextBuff1[2] = gBattleMons[gBattlerTarget].moves[i];
        gBattleTextBuff1[3] = uBYTE1_16(gBattleMons[gBattlerTarget].moves[i]);
        gBattleTextBuff1[4] = 0xFF;

        gDisableStructs[gBattlerTarget].disabledMove = gBattleMons[gBattlerTarget].moves[i];
        gDisableStructs[gBattlerTarget].disableTimer1 = (Random() & 3) + 2;
        gDisableStructs[gBattlerTarget].disableTimer2 = gDisableStructs[gBattlerTarget].disableTimer1; //that's interesting
        gBattlescriptCurrInstr += 5;
    }
    else
    {
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
}

static void atkA4_trysetencore(void)
{
    int i;
    for (i = 0; i < 4; i++)
    {
        if (gBattleMons[gBattlerTarget].moves[i] == gLastMoves[gBattlerTarget])
            break;
    }
    if (gLastMoves[gBattlerTarget] == MOVE_STRUGGLE || gLastMoves[gBattlerTarget] == MOVE_ENCORE || gLastMoves[gBattlerTarget] == MOVE_MIRROR_MOVE)
        i = 4;
    if (gDisableStructs[gBattlerTarget].encoredMove == 0 && i != 4 && gBattleMons[gBattlerTarget].pp[i] != 0)
    {
        gDisableStructs[gBattlerTarget].encoredMove = gBattleMons[gBattlerTarget].moves[i];
        gDisableStructs[gBattlerTarget].encoredMovePos = i;
        gDisableStructs[gBattlerTarget].encoreTimer1 = (Random() & 3) + 3;
        gDisableStructs[gBattlerTarget].encoreTimer2 = gDisableStructs[gBattlerTarget].encoreTimer1;
        gBattlescriptCurrInstr += 5;
    }
    else
    {
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
}

static void atkA5_painsplitdmgcalc(void)
{
    if (!(gBattleMons[gBattlerTarget].status2 & STATUS2_SUBSTITUTE))
    {
        s32 hp_diff = (gBattleMons[gBattlerAttacker].hp + gBattleMons[gBattlerTarget].hp) / 2;
        s32 to_store = gBattleMoveDamage = gBattleMons[gBattlerTarget].hp - hp_diff;
        gBattleStruct->unk16014 = sBYTE0_32(to_store);
        gBattleStruct->unk16015 = sBYTE1_32(to_store);
        gBattleStruct->unk16016 = sBYTE2_32(to_store);
        gBattleStruct->unk16017 = sBYTE3_32(to_store);

        gBattleMoveDamage = gBattleMons[gBattlerAttacker].hp - hp_diff;
        gSpecialStatuses[gBattlerTarget].dmg = 0xFFFF;

        gBattlescriptCurrInstr += 5;
    }
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

#ifdef NONMATCHING
static void atkA6_settypetorandomresistance(void)
{
    if (gLastLandedMoves[gBattlerAttacker] == 0 || gLastLandedMoves[gBattlerAttacker] == 0xFFFF || (IsTwoTurnsMove(gLastLandedMoves[gBattlerAttacker]) && !gProtectStructs[gBattlerAttacker].physicalDmg && !gProtectStructs[gBattlerAttacker].specialDmg))
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    else
    {
        int type = 0, rands = 0;
        do
        {
            while (((type = (Random() & 0x7F)) > 0x70));
            type *= 3;
            if (gTypeEffectiveness[type] == gLastHitByType[gBattlerAttacker] && gTypeEffectiveness[type + 2] <= 5 && gBattleMons[gBattlerAttacker].type1 != gTypeEffectiveness[type + 1] && gBattleMons[gBattlerAttacker].type2 != gTypeEffectiveness[type + 1])
            {
                gBattleMons[gBattlerAttacker].type1 = type;
                gBattleMons[gBattlerAttacker].type2 = type;
                gBattleTextBuff1[0] = 0xFD;
                gBattleTextBuff1[1] = 3;
                gBattleTextBuff1[2] = type;
                gBattleTextBuff1[3] = 0xFF;
                gBattlescriptCurrInstr += 5;
                return;
            }
            rands++;
        } while (rands <= 999);

        type = 0, rands = 0;
        do
        {
            if (gTypeEffectiveness[type] == 0xFE || gTypeEffectiveness[type] != 0xFF)
            {
                if (gTypeEffectiveness[type] == gLastHitByType[gBattlerAttacker] && gTypeEffectiveness[type + 2] <= 5 && gBattleMons[gBattlerAttacker].type1 != gTypeEffectiveness[type + 1] && gBattleMons[gBattlerAttacker].type2 != gTypeEffectiveness[type + 1])
                {
                    gBattleMons[gBattlerAttacker].type1 = gTypeEffectiveness[rands + 1];
                    gBattleMons[gBattlerAttacker].type2 = gTypeEffectiveness[rands + 1];
                    gBattleTextBuff1[0] = 0xFD;
                    gBattleTextBuff1[1] = 3;
                    gBattleTextBuff1[2] = gTypeEffectiveness[rands + 1];
                    gBattleTextBuff1[3] = 0xFF;
                    gBattlescriptCurrInstr += 5;
                    return;
                }
            }
            type += 3, rands += 3;
        } while (rands < 336);

        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
}

#else
NAKED
static void atkA6_settypetorandomresistance(void)
{
    asm(".syntax unified\n\
    push {r4-r7,lr}\n\
    mov r7, r10\n\
    mov r6, r9\n\
    mov r5, r8\n\
    push {r5-r7}\n\
    ldr r1, _08027FA8 @ =gLastLandedMoves\n\
    ldr r4, _08027FAC @ =gBattlerAttacker\n\
    ldrb r0, [r4]\n\
    lsls r0, 1\n\
    adds r2, r0, r1\n\
    ldrh r1, [r2]\n\
    cmp r1, 0\n\
    beq _08027F8C\n\
    ldr r0, _08027FB0 @ =0x0000ffff\n\
    cmp r1, r0\n\
    beq _08027F8C\n\
    ldrh r0, [r2]\n\
    bl IsTwoTurnsMove\n\
    lsls r0, 24\n\
    cmp r0, 0\n\
    beq _08028024\n\
    ldr r2, _08027FB4 @ =gProtectStructs\n\
    ldrb r0, [r4]\n\
    lsls r1, r0, 4\n\
    adds r0, r2, 0x4\n\
    adds r0, r1, r0\n\
    ldr r0, [r0]\n\
    cmp r0, 0\n\
    bne _08028024\n\
    adds r0, r2, 0\n\
    adds r0, 0x8\n\
    adds r0, r1, r0\n\
    ldr r0, [r0]\n\
    cmp r0, 0\n\
    bne _08028024\n\
_08027F8C:\n\
    ldr r3, _08027FB8 @ =gBattlescriptCurrInstr\n\
    ldr r2, [r3]\n\
    ldrb r1, [r2, 0x1]\n\
    ldrb r0, [r2, 0x2]\n\
    lsls r0, 8\n\
    orrs r1, r0\n\
    ldrb r0, [r2, 0x3]\n\
    lsls r0, 16\n\
    orrs r1, r0\n\
    ldrb r0, [r2, 0x4]\n\
    lsls r0, 24\n\
    orrs r1, r0\n\
    str r1, [r3]\n\
    b _08028110\n\
    .align 2, 0\n\
_08027FA8: .4byte gLastLandedMoves\n\
_08027FAC: .4byte gBattlerAttacker\n\
_08027FB0: .4byte 0x0000ffff\n\
_08027FB4: .4byte gProtectStructs\n\
_08027FB8: .4byte gBattlescriptCurrInstr\n\
_08027FBC:\n\
    mov r0, r12\n\
    strb r5, [r0]\n\
    mov r1, r10\n\
    ldrb r0, [r1]\n\
    muls r0, r2\n\
    adds r0, r7\n\
    adds r0, 0x22\n\
    strb r5, [r0]\n\
    ldr r1, _08027FE0 @ =gBattleTextBuff1\n\
    movs r0, 0xFD\n\
    strb r0, [r1]\n\
    movs r0, 0x3\n\
    strb r0, [r1, 0x1]\n\
    strb r5, [r1, 0x2]\n\
    movs r0, 0xFF\n\
    strb r0, [r1, 0x3]\n\
    ldr r1, _08027FE4 @ =gBattlescriptCurrInstr\n\
    b _08028012\n\
    .align 2, 0\n\
_08027FE0: .4byte gBattleTextBuff1\n\
_08027FE4: .4byte gBattlescriptCurrInstr\n\
_08027FE8:\n\
    mov r0, r8\n\
    adds r0, 0x1\n\
    adds r0, r3\n\
    ldrb r2, [r0]\n\
    strb r2, [r4]\n\
    mov r4, r10\n\
    ldrb r0, [r4]\n\
    muls r0, r6\n\
    ldr r7, _0802801C @ =gBattleMons\n\
    adds r0, r7\n\
    adds r0, 0x22\n\
    strb r2, [r0]\n\
    ldr r1, _08028020 @ =gBattleTextBuff1\n\
    movs r0, 0xFD\n\
    strb r0, [r1]\n\
    movs r0, 0x3\n\
    strb r0, [r1, 0x1]\n\
    strb r2, [r1, 0x2]\n\
    movs r0, 0xFF\n\
    strb r0, [r1, 0x3]\n\
    mov r1, r12\n\
_08028012:\n\
    ldr r0, [r1]\n\
    adds r0, 0x5\n\
    str r0, [r1]\n\
    b _08028110\n\
    .align 2, 0\n\
_0802801C: .4byte gBattleMons\n\
_08028020: .4byte gBattleTextBuff1\n\
_08028024:\n\
    movs r4, 0\n\
    mov r8, r4\n\
    movs r7, 0x7F\n\
    mov r9, r7\n\
_0802802C:\n\
    bl Random\n\
    mov r4, r9\n\
    ands r4, r0\n\
    cmp r4, 0x70\n\
    bhi _0802802C\n\
    lsls r0, r4, 1\n\
    adds r4, r0, r4\n\
    ldr r6, _08028120 @ =gTypeEffectiveness\n\
    adds r3, r4, r6\n\
    ldr r1, _08028124 @ =gLastHitByType\n\
    ldr r2, _08028128 @ =gBattlerAttacker\n\
    ldrb r5, [r2]\n\
    lsls r0, r5, 1\n\
    adds r0, r1\n\
    ldrb r1, [r3]\n\
    mov r10, r2\n\
    ldrh r0, [r0]\n\
    cmp r1, r0\n\
    bne _08028088\n\
    adds r0, r4, 0x2\n\
    adds r0, r6\n\
    ldrb r0, [r0]\n\
    cmp r0, 0x5\n\
    bhi _08028088\n\
    ldr r7, _0802812C @ =gBattleMons\n\
    movs r2, 0x58\n\
    adds r0, r5, 0\n\
    muls r0, r2\n\
    adds r3, r0, r7\n\
    movs r0, 0x21\n\
    adds r0, r3\n\
    mov r12, r0\n\
    adds r0, r4, 0x1\n\
    adds r0, r6\n\
    ldrb r5, [r0]\n\
    mov r1, r12\n\
    ldrb r0, [r1]\n\
    adds r1, r5, 0\n\
    cmp r0, r1\n\
    beq _08028088\n\
    adds r0, r3, 0\n\
    adds r0, 0x22\n\
    ldrb r0, [r0]\n\
    cmp r0, r1\n\
    bne _08027FBC\n\
_08028088:\n\
    movs r7, 0x1\n\
    add r8, r7\n\
    ldr r0, _08028130 @ =0x000003e7\n\
    cmp r8, r0\n\
    ble _0802802C\n\
    movs r0, 0\n\
    mov r8, r0\n\
    ldr r1, _08028134 @ =gBattlescriptCurrInstr\n\
    mov r12, r1\n\
    ldr r3, _08028120 @ =gTypeEffectiveness\n\
    adds r0, r4, 0x1\n\
    adds r0, r3\n\
    mov r9, r0\n\
    adds r5, r3, 0\n\
_080280A4:\n\
    ldrb r1, [r5]\n\
    cmp r1, 0xFF\n\
    bgt _080280AE\n\
    cmp r1, 0xFE\n\
    bge _080280E8\n\
_080280AE:\n\
    mov r4, r10\n\
    ldrb r2, [r4]\n\
    lsls r0, r2, 1\n\
    ldr r7, _08028124 @ =gLastHitByType\n\
    adds r0, r7\n\
    ldrh r0, [r0]\n\
    cmp r1, r0\n\
    bne _080280E8\n\
    ldrb r0, [r5, 0x2]\n\
    cmp r0, 0x5\n\
    bhi _080280E8\n\
    movs r6, 0x58\n\
    adds r0, r2, 0\n\
    muls r0, r6\n\
    ldr r1, _0802812C @ =gBattleMons\n\
    adds r2, r0, r1\n\
    adds r4, r2, 0\n\
    adds r4, 0x21\n\
    ldrb r0, [r4]\n\
    mov r7, r9\n\
    ldrb r1, [r7]\n\
    cmp r0, r1\n\
    beq _080280E8\n\
    adds r0, r2, 0\n\
    adds r0, 0x22\n\
    ldrb r0, [r0]\n\
    cmp r0, r1\n\
    beq _080280E8\n\
    b _08027FE8\n\
_080280E8:\n\
    adds r5, 0x3\n\
    movs r0, 0x3\n\
    add r8, r0\n\
    ldr r0, _08028138 @ =0x0000014f\n\
    cmp r8, r0\n\
    bls _080280A4\n\
    mov r1, r12\n\
    ldr r2, [r1]\n\
    ldrb r1, [r2, 0x1]\n\
    ldrb r0, [r2, 0x2]\n\
    lsls r0, 8\n\
    orrs r1, r0\n\
    ldrb r0, [r2, 0x3]\n\
    lsls r0, 16\n\
    orrs r1, r0\n\
    ldrb r0, [r2, 0x4]\n\
    lsls r0, 24\n\
    orrs r1, r0\n\
    mov r4, r12\n\
    str r1, [r4]\n\
_08028110:\n\
    pop {r3-r5}\n\
    mov r8, r3\n\
    mov r9, r4\n\
    mov r10, r5\n\
    pop {r4-r7}\n\
    pop {r0}\n\
    bx r0\n\
    .align 2, 0\n\
_08028120: .4byte gTypeEffectiveness\n\
_08028124: .4byte gLastHitByType\n\
_08028128: .4byte gBattlerAttacker\n\
_0802812C: .4byte gBattleMons\n\
_08028130: .4byte 0x000003e7\n\
_08028134: .4byte gBattlescriptCurrInstr\n\
_08028138: .4byte 0x0000014f\n\
        .syntax divided");
}
#endif // NONMATCHING

static void atkA7_setalwayshitflag(void)
{
    gStatuses3[gBattlerTarget] &= ~(STATUS3_ALWAYS_HITS);
    gStatuses3[gBattlerTarget] |= 0x10;
    gDisableStructs[gBattlerTarget].battlerWithSureHit = gBattlerAttacker;
    gBattlescriptCurrInstr++;
}

struct move_pp
{
    u16 move[4];
    u8 pp[4];
    u8 ppBonuses;
};

static void atkA8_copymovepermanently(void)
{
    gChosenMove = 0xFFFF;
    if (!(gBattleMons[gBattlerAttacker].status2 & STATUS2_TRANSFORMED) && gUnknown_02024C2C[gBattlerTarget] != MOVE_STRUGGLE && gUnknown_02024C2C[gBattlerTarget] != 0 && gUnknown_02024C2C[gBattlerTarget] != 0xFFFF && gUnknown_02024C2C[gBattlerTarget] != MOVE_SKETCH)
    {
        int i;
        for (i = 0; i < 4; i++)
        {
            if (gBattleMons[gBattlerAttacker].moves[i] == MOVE_SKETCH)
                continue;
            if (gBattleMons[gBattlerAttacker].moves[i] == gUnknown_02024C2C[gBattlerTarget])
                break;
        }
        if (i != 4) //sketch fail
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
        else //sketch worked
        {
            struct move_pp moves_data;
            gBattleMons[gBattlerAttacker].moves[gCurrMovePos] = gUnknown_02024C2C[gBattlerTarget];
            gBattleMons[gBattlerAttacker].pp[gCurrMovePos] = gBattleMoves[gUnknown_02024C2C[gBattlerTarget]].pp;
            gActiveBattler = gBattlerAttacker;
            for (i = 0; i < 4; i++)
            {
                moves_data.move[i] = gBattleMons[gBattlerAttacker].moves[i];
                moves_data.pp[i] = gBattleMons[gBattlerAttacker].pp[i];
            }
            moves_data.ppBonuses = gBattleMons[gBattlerAttacker].ppBonuses;
            BtlController_EmitSetMonData(0, REQUEST_MOVES_PP_BATTLE, 0, sizeof(struct move_pp), &moves_data);
            MarkBattlerForControllerExec(gActiveBattler);
            gBattleTextBuff1[0] = 0xFD;
            gBattleTextBuff1[1] = 2;
            gBattleTextBuff1[2] = gUnknown_02024C2C[gBattlerTarget];
            gBattleTextBuff1[3] = gUnknown_02024C2C[gBattlerTarget] >> 8;
            gBattleTextBuff1[4] = 0xFF;
            gBattlescriptCurrInstr += 5;
        }
    }
    else //sketch fail
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

static bool8 IsTwoTurnsMove(u16 move)
{
    u8 effect = gBattleMoves[move].effect;
    if (effect == EFFECT_SKULL_BASH || effect == EFFECT_RAZOR_WIND || effect == EFFECT_SKY_ATTACK || effect == EFFECT_SOLARBEAM || effect == EFFECT_FLY || effect == EFFECT_BIDE)
        return 1;
    else
        return 0;
}

static bool8 IsMoveUnchoosable(u16 move)
{
    if (move == 0 || move == MOVE_SLEEP_TALK || move == MOVE_ASSIST || move == MOVE_MIRROR_MOVE || move == MOVE_METRONOME)
        return 1;
    else
        return 0;
}

static u8 AttacksThisTurn(u8 bank, u16 move) //Note: returns 1 if it's a charging turn, otherwise 2
{
    //first argument is unused
    u8 effect;
    if (gBattleMoves[move].effect == EFFECT_SOLARBEAM && (gBattleWeather & WEATHER_SUN_ANY))
        return 2;
    effect = gBattleMoves[move].effect;
    if (effect == EFFECT_SKULL_BASH || effect == EFFECT_RAZOR_WIND || effect == EFFECT_SKY_ATTACK || effect == EFFECT_SOLARBEAM || effect == EFFECT_FLY || effect == EFFECT_BIDE)
    {
        if ((gHitMarker & HITMARKER_CHARGING))
            return 1;
    }
    return 2;
}

static void atkA9_trychoosesleeptalkmove(void)
{
    u8 unusable_moves = 0;
    int i;

    for (i = 0; i < 4; i++)
    {
        if (IsMoveUnchoosable(gBattleMons[gBattlerAttacker].moves[i]) || gBattleMons[gBattlerAttacker].moves[i] == MOVE_FOCUS_PUNCH
            || gBattleMons[gBattlerAttacker].moves[i] == MOVE_UPROAR || IsTwoTurnsMove(gBattleMons[gBattlerAttacker].moves[i]))
                unusable_moves |= gBitTable[i];
    }
    unusable_moves = CheckMoveLimitations(gBattlerAttacker, unusable_moves, 0xFD);
    if (unusable_moves == 0xF) //all 4 moves cannot be chosen
        gBattlescriptCurrInstr += 5;
    else //at least one move can be chosen
    {
        u32 random_pos;
        do
        {
            random_pos = Random() & 3;
        } while ((gBitTable[random_pos] & unusable_moves));

        gRandomMove = gBattleMons[gBattlerAttacker].moves[random_pos];
        gCurrMovePos = random_pos;
        gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);
        gBattlerTarget = GetMoveTarget(gRandomMove, 0);
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
}

static void atkAA_setdestinybond(void)
{
    gBattleMons[gBattlerAttacker].status2 |= STATUS2_DESTINY_BOND;
    gBattlescriptCurrInstr++;
}

static void TrySetDestinyBondToHappen(void)
{
    u8 atk_side = GetBattlerSide(gBattlerAttacker);
    u8 def_side = GetBattlerSide(gBattlerTarget);
    if (gBattleMons[gBattlerTarget].status2 & STATUS2_DESTINY_BOND && atk_side != def_side && !(gHitMarker & HITMARKER_GRUDGE))
        gHitMarker |= HITMARKER_DESTINYBOND;
}

static void atkAB_trysetdestinybondtohappen(void)
{
    TrySetDestinyBondToHappen();
    gBattlescriptCurrInstr++;
}

static void atkAC_remaininghptopower(void)
{
    s32 hp_fraction = GetScaledHPFraction(gBattleMons[gBattlerAttacker].hp, gBattleMons[gBattlerAttacker].maxHP, 48);
    int i;
    for (i = 0; i < 12; i += 2)
    {
        if (hp_fraction <= sFlailHpScaleToPowerTable[i])
            break;
    }
    gDynamicBasePower = sFlailHpScaleToPowerTable[i + 1];
    gBattlescriptCurrInstr++;
}

static void atkAD_tryspiteppreduce(void)
{
    if (gLastMoves[gBattlerTarget] != 0 && gLastMoves[gBattlerTarget] != 0xFFFF && !(gStatuses3[gBattlerTarget] & STATUS3_SEMI_INVULNERABLE))
    {
        int i;
        for (i = 0; i < 4; i++)
        {
            if (gLastMoves[gBattlerTarget] == gBattleMons[gBattlerTarget].moves[i])
                break;
        }
        if (i != 4 && gBattleMons[gBattlerTarget].pp[i] > 1)
        {
            s32 lost_pp = (Random() & 3) + 2;
            if (gBattleMons[gBattlerTarget].pp[i] < lost_pp)
                lost_pp = gBattleMons[gBattlerTarget].pp[i];

            gBattleTextBuff1[0] = 0xFD;
            gBattleTextBuff1[1] = 2;
            gBattleTextBuff1[2] = gLastMoves[gBattlerTarget];
            gBattleTextBuff1[3] = gLastMoves[gBattlerTarget] >> 8;
            gBattleTextBuff1[4] = 0xFF;
            ConvertIntToDecimalStringN(gBattleTextBuff2, lost_pp, 0, 1);
            gBattleTextBuff2[0] = 0xFD;
            gBattleTextBuff2[1] = 1;
            gBattleTextBuff2[2] = 1;
            gBattleTextBuff2[3] = 1;
            gBattleTextBuff2[4] = lost_pp;
            gBattleTextBuff2[5] = 0xFF;

            gBattleMons[gBattlerTarget].pp[i] -= lost_pp;
            gActiveBattler = gBattlerTarget;
            if (!(gDisableStructs[gActiveBattler].mimickedMoves & gBitTable[i])
                && !(gBattleMons[gActiveBattler].status2 & STATUS2_TRANSFORMED))
            {
                BtlController_EmitSetMonData(0, REQUEST_PPMOVE1_BATTLE + i, 0, 1, &gBattleMons[gActiveBattler].pp[i]);
                MarkBattlerForControllerExec(gActiveBattler);
            }
            gBattlescriptCurrInstr += 5;
            if (gBattleMons[gBattlerTarget].pp[i] == 0)
                CancelMultiTurnMoves(gBattlerTarget);
            return;
        }
    }
    gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atkAE_healpartystatus(void)
{
    register u32 zero2 asm("r4") = 0;
    u32 zero = zero2;
    u8 to_heal = 0;
    if (gCurrentMove == MOVE_HEAL_BELL)
    {
        struct Pokemon* party;
        int i;

        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
        if (GetBattlerSide(gBattlerAttacker) == 0)
            party = gPlayerParty;
        else
            party = gEnemyParty;

        if (GetBattlerAbility(gBattlerAttacker) != ABILITY_SOUNDPROOF)
        {
            gBattleMons[gBattlerAttacker].status1 = 0;
        }
        else
        {
            RecordAbilityBattle(gBattlerAttacker, GetBattlerAbility(gBattlerAttacker));
            gBattleCommunication[MULTISTRING_CHOOSER] |= 1;
        }

        gActiveBattler = gBattleStruct->scriptingActive = GetBattlerAtPosition(GetBattlerPosition(gBattlerAttacker) ^ 2);
        if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && !(gAbsentBattlerFlags & gBitTable[gActiveBattler]))
        {
            if (GetBattlerAbility(gActiveBattler) != ABILITY_SOUNDPROOF)
            {
                gBattleMons[gActiveBattler].status1 = 0;
            }
            else
            {
                RecordAbilityBattle(gActiveBattler, GetBattlerAbility(gActiveBattler));
                gBattleCommunication[MULTISTRING_CHOOSER] |= 2;
            }
        }

        for (i = 0; i < 6; i++)
        {
            u16 species = GetMonData(&party[i], MON_DATA_SPECIES2);
            u8 abilityBit = GetMonData(&party[i], MON_DATA_ALT_ABILITY);
            u8 hiddenAbilityBit = GetMonData(&party[i], MON_DATA_HIDDEN_ABILITY);
            if (species != 0 && species != SPECIES_EGG)
            {
                u8 ability;
                if (gBattlerPartyIndexes[gBattlerAttacker] == i)
                    ability = GetBattlerAbility(gBattlerAttacker);
                else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && gBattlerPartyIndexes[gActiveBattler] == i && !(gAbsentBattlerFlags & gBitTable[gActiveBattler]))
                    ability = GetBattlerAbility(gActiveBattler);
                else
                    ability = GetAbilityBySpecies(species, abilityBit, hiddenAbilityBit);
                if (ability != ABILITY_SOUNDPROOF)
                    to_heal |= (1 << i);
            }
        }
    }
    else //Aromatherapy
    {
        gBattleCommunication[MULTISTRING_CHOOSER] = 4;
        to_heal = 0x3F;
        gBattleMons[gBattlerAttacker].status1 = zero2;

        gActiveBattler = GetBattlerAtPosition(GetBattlerPosition(gBattlerAttacker) ^ 2);
        if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && !(gAbsentBattlerFlags & gBitTable[gActiveBattler]))
            gBattleMons[gActiveBattler].status1 = 0;

    }
    //missing check?
    gActiveBattler = gBattlerAttacker;
    BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, to_heal, 4, &zero);
    MarkBattlerForControllerExec(gActiveBattler);

    gBattlescriptCurrInstr++;
}

static void atkAF_cursetarget(void)
{
    if (gBattleMons[gBattlerTarget].status2 & STATUS2_CURSED || gStatuses3[gBattlerTarget] & STATUS3_SEMI_INVULNERABLE)
    {
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
    else
    {
        gBattleMons[gBattlerTarget].status2 |= STATUS2_CURSED;
        gBattleMoveDamage = gBattleMons[gBattlerAttacker].maxHP / 2;
        if (gBattleMoveDamage == 0)
            gBattleMoveDamage = 1;
        gBattlescriptCurrInstr += 5;
    }
}

static void atkB0_trysetspikes(void)
{
    u8 side = GetBattlerSide(gBattlerAttacker) ^ 1;
    if (gSideTimers[side].spikesAmount == 3)
    {
        gSpecialStatuses[gBattlerAttacker].ppNotAffectedByPressure = 1;
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
    else
    {
        gSideStatuses[side] |= SIDE_STATUS_SPIKES;
        gSideTimers[side].spikesAmount++;
        gBattlescriptCurrInstr += 5;
    }
}

static void atkB1_setforesight(void)
{
    gBattleMons[gBattlerTarget].status2 |= STATUS2_FORESIGHT;
    gBattlescriptCurrInstr++;
}

static void atkB2_trysetperishsong(void)
{
    int not_affected_pokes = 0, i;

    for (i = 0; i < gBattlersCount; i++)
    {
        if (gStatuses3[i] & STATUS3_PERISH_SONG || GetBattlerAbility(i) == ABILITY_SOUNDPROOF)
            not_affected_pokes++;
        else
        {
            gStatuses3[i] |= STATUS3_PERISH_SONG;
            gDisableStructs[i].perishSongTimer1 = 3;
            gDisableStructs[i].perishSongTimer2 = 3;
        }
    }

    PressurePPLoseOnUsingPerishSong(gBattlerAttacker);
    if (not_affected_pokes == gBattlersCount)
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    else
        gBattlescriptCurrInstr += 5;
}

static void atkB3_rolloutdamagecalculation(void)
{
    if (gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
    {
        CancelMultiTurnMoves(gBattlerAttacker);
        gBattlescriptCurrInstr = BattleScript_MoveMissedPause;
    }
    else
    {
        int i;
        if (!(gBattleMons[gBattlerAttacker].status2 & STATUS2_MULTIPLETURNS)) //first hit
        {
            gDisableStructs[gBattlerAttacker].rolloutTimer1 = 5;
            gDisableStructs[gBattlerAttacker].rolloutTimer2 = 5;
            gBattleMons[gBattlerAttacker].status2 |= STATUS2_MULTIPLETURNS;
            gLockedMoves[gBattlerAttacker] = gCurrentMove;
        }
        if (--gDisableStructs[gBattlerAttacker].rolloutTimer1 == 0)
            gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_MULTIPLETURNS);

        gDynamicBasePower = gBattleMoves[gCurrentMove].power;
        for (i = 1; i < (5 - gDisableStructs[gBattlerAttacker].rolloutTimer1); i++)
            gDynamicBasePower *= 2;

        if (gBattleMons[gBattlerAttacker].status2 & STATUS2_DEFENSE_CURL)
            gDynamicBasePower *= 2;

        gBattlescriptCurrInstr++;
    }
}

static void atkB4_jumpifconfusedandstatmaxed(void)
{
    if (gBattleMons[gBattlerTarget].status2 & STATUS2_CONFUSION && gBattleMons[gBattlerTarget].statStages[T2_READ_8(gBattlescriptCurrInstr + 1)] == 0xC)
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
    else
        gBattlescriptCurrInstr += 6;
}

static void atkB5_furycuttercalc(void)
{
    if (gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
    {
        gDisableStructs[gBattlerAttacker].furyCutterCounter = 0;
        gBattlescriptCurrInstr = BattleScript_MoveMissedPause;
    }
    else
    {
        int i;

        if (gDisableStructs[gBattlerAttacker].furyCutterCounter != 5)
            gDisableStructs[gBattlerAttacker].furyCutterCounter++;

        gDynamicBasePower = gBattleMoves[gCurrentMove].power;

        for (i = 1; i < gDisableStructs[gBattlerAttacker].furyCutterCounter; i++)
            gDynamicBasePower *= 2;

        gBattlescriptCurrInstr++;
    }
}

static void atkB6_happinesstodamagecalculation(void)
{
    if (gBattleMoves[gCurrentMove].effect == EFFECT_RETURN)
        gDynamicBasePower = 10 * (gBattleMons[gBattlerAttacker].friendship) / 25;
    else //EFFECT_FRUSTRATION
        gDynamicBasePower = 10 * (255 - gBattleMons[gBattlerAttacker].friendship) / 25;
    gBattlescriptCurrInstr++;
}

static void atkB7_presentdamagecalculation(void)
{
    s32 rand = Random() & 0xFF;
    if (rand < 102)
        gDynamicBasePower = 40;
    else if (rand < 178)
        gDynamicBasePower = 80;
    else if (rand < 204)
        gDynamicBasePower = 120;
    else
    {
        gBattleMoveDamage = gBattleMons[gBattlerTarget].maxHP / 4;
        if (gBattleMoveDamage == 0)
            gBattleMoveDamage = 1;
        gBattleMoveDamage *= -1;
    }
    if (rand < 204)
        gBattlescriptCurrInstr = BattleScript_HitFromCritCalc;
    else if (gBattleMons[gBattlerTarget].maxHP == gBattleMons[gBattlerTarget].hp)
        gBattlescriptCurrInstr = BattleScript_AlreadyAtFullHp;
    else
    {
        //gMoveResultFlags &= ~(MOVE_RESULT_DOESNT_AFFECT_FOE); only in Emerald
        gBattlescriptCurrInstr = BattleScript_PresentHealTarget;
    }
}

static void atkB8_setsafeguard(void)
{
    if (gSideStatuses[GetBattlerPosition(gBattlerAttacker) & 1] & SIDE_STATUS_SAFEGUARD)
    {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
    }
    else
    {
        gSideStatuses[GetBattlerPosition(gBattlerAttacker) & 1] |= SIDE_STATUS_SAFEGUARD;
        gSideTimers[GetBattlerPosition(gBattlerAttacker) & 1].safeguardTimer = 5;
        gBattleCommunication[MULTISTRING_CHOOSER] = 5;
    }
    gBattlescriptCurrInstr++;
}

static void atkB9_magnitudedamagecalculation(void)
{
    s32 magnitude = Random() % 100;
    if (magnitude < 5)
    {
        gDynamicBasePower = 10;
        magnitude = 4;
    }
    else if (magnitude < 15)
    {
        gDynamicBasePower = 30;
        magnitude = 5;
    }
    else if (magnitude < 35)
    {
        gDynamicBasePower = 50;
        magnitude = 6;
    }
    else if (magnitude < 65)
    {
        gDynamicBasePower = 70;
        magnitude = 7;
    }
    else if (magnitude < 85)
    {
        gDynamicBasePower = 90;
        magnitude = 8;
    }
    else if (magnitude < 95)
    {
        gDynamicBasePower = 110;
        magnitude = 9;
    }
    else
    {
        gDynamicBasePower = 150;
        magnitude = 10;
    }

    gBattleTextBuff1[0] = 0xFD;
    gBattleTextBuff1[1] = 1;
    gBattleTextBuff1[2] = 1;
    gBattleTextBuff1[3] = 2;
    gBattleTextBuff1[4] = magnitude;
    gBattleTextBuff1[5] = 0xFF;

    for (gBattlerTarget = 0; gBattlerTarget < gBattlersCount; gBattlerTarget++)
    {
        if (gBattlerTarget == gBattlerAttacker)
            continue;
        if (!(gAbsentBattlerFlags & gBitTable[gBattlerTarget])) //a valid target was found
            break;
    }
    gBattlescriptCurrInstr++;
}

static void atkBA_jumpifnopursuitswitchdmg(void)
{
    if (gMultiHitCounter == 1)
    {
        if (GetBattlerSide(gBattlerAttacker) == 0)
            gBattlerTarget = GetBattlerAtPosition(1);
        else
            gBattlerTarget = GetBattlerAtPosition(0);
    }
    else
    {
        if (GetBattlerSide(gBattlerAttacker) == 0)
            gBattlerTarget = GetBattlerAtPosition(3);
        else
            gBattlerTarget = GetBattlerAtPosition(2);
    }

    if (gActionForBanks[gBattlerTarget] == 0 && gBattlerAttacker == ewram16010arr(gBattlerTarget) && !(gBattleMons[gBattlerTarget].status1 & (STATUS1_SLEEP | STATUS1_FREEZE))
        && gBattleMons[gBattlerAttacker].hp && !gDisableStructs[gBattlerTarget].truantCounter && gChosenMovesByBanks[gBattlerTarget] == MOVE_PURSUIT)
    {
        int i;
        for (i = 0; i < gBattlersCount; i++)
        {
            if (gBattlerByTurnOrder[i] == gBattlerTarget)
                gActionsByTurnOrder[i] = 11;
        }
        gCurrentMove = MOVE_PURSUIT;
        gBattlescriptCurrInstr += 5;
        gBattleStruct->animTurn = 1;
        gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);
    }
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atkBB_setsunny(void)
{
    if (gBattleWeather & WEATHER_SUN_ANY)
    {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 2;
    }
    else
    {
        gBattleGlobalTimers.fog = 0;
        gBattleWeather = WEATHER_SUN_TEMPORARY;
        gBattleCommunication[MULTISTRING_CHOOSER] = 4;
        if (ItemId_GetHoldEffect(gBattleMons[gBattlerAttacker].item) == HOLD_EFFECT_EXTEND_SUN)
            gWishFutureKnock.weatherDuration = 8;
        else
            gWishFutureKnock.weatherDuration = 5;
    }
    gBattlescriptCurrInstr++;
}

static void atkBC_maxattackhalvehp(void) //belly drum
{
    u32 half_hp = gBattleMons[gBattlerAttacker].maxHP / 2;
    if (!(gBattleMons[gBattlerAttacker].maxHP / 2))
        half_hp = 1;

    if (gBattleMons[gBattlerAttacker].statStages[STAT_STAGE_ATK] < 12 && gBattleMons[gBattlerAttacker].hp > half_hp)
    {
        gBattleMons[gBattlerAttacker].statStages[STAT_STAGE_ATK] = 12;
        gBattleMoveDamage = gBattleMons[gBattlerAttacker].maxHP / 2;
        if (gBattleMoveDamage == 0)
            gBattleMoveDamage = 1;
        gBattlescriptCurrInstr += 5;
    }
    else
    {
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
}

static void atkBD_copyfoestats(void) //psych up
{
    int i;
    for (i = 0; i < 8; i++)
    {
        gBattleMons[gBattlerAttacker].statStages[i] = gBattleMons[gBattlerTarget].statStages[i];
    }
    gBattlescriptCurrInstr += 5; //why not 1? possible unused fail possibility?
}

static void atkBE_rapidspinfree(void) //rapid spin
{
    if (gBattleMons[gBattlerAttacker].status2 & STATUS2_WRAPPED)
    {
        gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_WRAPPED);
        gBattlerTarget = ewram16020arr(gBattlerAttacker);
        gBattleTextBuff1[0] = 0xFD;
        gBattleTextBuff1[1] = 2;
        gBattleTextBuff1[2] = ewram16004arr(0, gBattlerAttacker);
        gBattleTextBuff1[3] = ewram16004arr(1, gBattlerAttacker);
        gBattleTextBuff1[4] = 0xFF;
        BattleScriptPushCursor();
        gBattlescriptCurrInstr = BattleScript_WrapFree;
    }
    else if (gStatuses3[gBattlerAttacker] & STATUS3_LEECHSEED)
    {
        gStatuses3[gBattlerAttacker] &= ~(STATUS3_LEECHSEED);
        gStatuses3[gBattlerAttacker] &= ~(STATUS3_LEECHSEED_BATTLER);
        BattleScriptPushCursor();
        gBattlescriptCurrInstr = BattleScript_LeechSeedFree;
    }
    else if (gSideStatuses[GetBattlerSide(gBattlerAttacker)] & SIDE_STATUS_SPIKES)
    {
        gSideStatuses[GetBattlerSide(gBattlerAttacker)] &= ~(SIDE_STATUS_SPIKES);
        gSideTimers[GetBattlerSide(gBattlerAttacker)].spikesAmount = 0;
        BattleScriptPushCursor();
        gBattlescriptCurrInstr = BattleScript_SpikesFree;
    }
    else if (gSideStatuses[GetBattlerSide(gBattlerAttacker)] & SIDE_STATUS_TOXIC_SPIKES)
    {
        gSideStatuses[GetBattlerSide(gBattlerAttacker)] &= ~(SIDE_STATUS_TOXIC_SPIKES);
        gSideTimers[GetBattlerSide(gBattlerAttacker)].toxicSpikesAmount = 0;
        BattleScriptPushCursor();
        gBattlescriptCurrInstr = BattleScript_ToxicSpikesFree;
    }
    else if (gSideStatuses[GetBattlerSide(gBattlerAttacker)] & SIDE_STATUS_STEALTH_ROCK)
    {
        gSideStatuses[GetBattlerSide(gBattlerAttacker)] &= ~(SIDE_STATUS_STEALTH_ROCK);
        BattleScriptPushCursor();
        gBattlescriptCurrInstr = BattleScript_RocksFree;
    }
    else
        gBattlescriptCurrInstr++;
}

static void atkBF_setdefensecurlbit(void)
{
    gBattleMons[gBattlerAttacker].status2 |= STATUS2_DEFENSE_CURL;
    gBattlescriptCurrInstr++;
}

static void atkC0_recoverbasedonsunlight(void)
{
    gBattlerTarget = gBattlerAttacker;
    if (gBattleMons[gBattlerAttacker].hp != gBattleMons[gBattlerAttacker].maxHP)
    {
        if (!gBattleWeather || !WEATHER_HAS_EFFECT)
            gBattleMoveDamage = gBattleMons[gBattlerAttacker].maxHP / 2;
        else if (gBattleWeather & WEATHER_SUN_ANY)
            gBattleMoveDamage = 20 * gBattleMons[gBattlerAttacker].maxHP / 30;
        else //not sunny weather
            gBattleMoveDamage = gBattleMons[gBattlerAttacker].maxHP / 4;
        if (gBattleMoveDamage == 0)
            gBattleMoveDamage = 1;
        gBattleMoveDamage *= -1;
        gBattlescriptCurrInstr += 5;
    }
    else
    {
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
}

static void atkC1_hiddenpowercalc(void)
{
    u8 power = ((gBattleMons[gBattlerAttacker].hpIV & 2) >> 1) |
               ((gBattleMons[gBattlerAttacker].attackIV & 2)) |
               ((gBattleMons[gBattlerAttacker].defenseIV & 2) << 1) |
               ((gBattleMons[gBattlerAttacker].speedIV & 2) << 2) |
               ((gBattleMons[gBattlerAttacker].spAttackIV & 2) << 3) |
               ((gBattleMons[gBattlerAttacker].spDefenseIV & 2) << 4);
    u8 type = ((gBattleMons[gBattlerAttacker].hpIV & 1)) |
              ((gBattleMons[gBattlerAttacker].attackIV & 1) << 1) |
              ((gBattleMons[gBattlerAttacker].defenseIV & 1) << 2) |
              ((gBattleMons[gBattlerAttacker].speedIV & 1) << 3) |
              ((gBattleMons[gBattlerAttacker].spAttackIV & 1) << 4) |
              ((gBattleMons[gBattlerAttacker].spDefenseIV & 1) << 5);

    gDynamicBasePower = 30 + (power * 40 / 63);

    gBattleStruct->dynamicMoveType = ((type * 15) / 63) + 1;
    if (gBattleStruct->dynamicMoveType >= TYPE_MYSTERY)
        gBattleStruct->dynamicMoveType++;

    gBattleStruct->dynamicMoveType |= 0xC0;

    gBattlescriptCurrInstr++;
}

static void atkC2_selectfirstvalidtarget(void)
{
    for (gBattlerTarget = 0; gBattlerTarget < gBattlersCount; gBattlerTarget++)
    {
        if (gBattlerTarget == gBattlerAttacker)
            continue;
        if (!(gAbsentBattlerFlags & gBitTable[gBattlerTarget]))
            break;
    }
    gBattlescriptCurrInstr++;
}

static void atkC3_trysetfutureattack(void)
{
    if (gWishFutureKnock.futureSightCounter[gBattlerTarget] != 0)
    {
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
    else
    {
        gWishFutureKnock.futureSightMove[gBattlerTarget] = gCurrentMove;
        gWishFutureKnock.futureSightAttacker[gBattlerTarget] = gBattlerAttacker;
        gWishFutureKnock.futureSightCounter[gBattlerTarget] = 3;
        gWishFutureKnock.futureSightDmg[gBattlerTarget] = CalculateBaseDamage(&gBattleMons[gBattlerAttacker], &gBattleMons[gBattlerTarget], gCurrentMove,
                                                    gSideStatuses[GetBattlerPosition(gBattlerTarget) & 1], 0,
                                                    0, gBattlerAttacker, gBattlerTarget);

        if (gProtectStructs[gBattlerAttacker].helpingHand)
            gWishFutureKnock.futureSightDmg[gBattlerTarget] = gWishFutureKnock.futureSightDmg[gBattlerTarget] * 15 / 10;

        if (gCurrentMove == MOVE_DOOM_DESIRE)
            gBattleCommunication[MULTISTRING_CHOOSER] = 1;
        else
            gBattleCommunication[MULTISTRING_CHOOSER] = 0;

        gBattlescriptCurrInstr += 5;
    }
}

#ifdef NONMATCHING
static void atkC4_trydobeatup(void)
{
    register struct Pokemon* party asm("r7");
    if (GetBattlerSide(gBattlerAttacker) == 0)
        party = gPlayerParty;
    else
        party = gEnemyParty;

    if (gBattleMons[gBattlerTarget].hp == 0)
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    else
    {
        while (gBattleCommunication[0] < 6)
        {
            if (GetMonData(&party[gBattleCommunication[0]], MON_DATA_HP) && GetMonData(&party[gBattleCommunication[0]], MON_DATA_SPECIES2)
                && GetMonData(&party[gBattleCommunication[0]], MON_DATA_SPECIES2) != SPECIES_EGG && !GetMonData(&party[gBattleCommunication[0]], MON_DATA_STATUS))
                    break;
            gBattleCommunication[0]++;
        }
        if (gBattleCommunication[0] < 6)
        {
            gBattleTextBuff1[0] = 0xFD;
            gBattleTextBuff1[1] = 4;
            gBattleTextBuff1[2] = gBattlerAttacker;
            gBattleTextBuff1[3] = gBattleCommunication[0];
            gBattleTextBuff1[4] = 0xFF;
            gBattlescriptCurrInstr += 9;

            gBattleMoveDamage = gBaseStats[GetMonData(&party[gBattleCommunication[0]], MON_DATA_SPECIES)].baseAttack;
            gBattleMoveDamage *= gBattleMoves[gCurrentMove].power;
            gBattleMoveDamage *= (GetMonData(&party[gBattleCommunication[0]], MON_DATA_LEVEL) * 2 / 5 + 2);
            gBattleMoveDamage /= gBaseStats[gBattleMons[gBattlerTarget].species].baseDefense;
            gBattleMoveDamage = (gBattleMoveDamage / 50) + 2;
            if (gProtectStructs[gBattlerAttacker].helpingHand)
                gBattleMoveDamage = gBattleMoveDamage * 15 / 10;

            gBattleCommunication[0]++;
        }
        else if (gBattleCommunication[0] != 0)
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
        else
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 5);
    }
}
#else
NAKED
static void atkC4_trydobeatup(void)
{
    asm(".syntax unified\n\
    push {r4-r7,lr}\n\
    mov r7, r9\n\
    mov r6, r8\n\
    push {r6,r7}\n\
    ldr r0, _08029A8C @ =gBattlerAttacker\n\
    ldrb r0, [r0]\n\
    bl GetBattlerSide\n\
    lsls r0, 24\n\
    ldr r7, _08029A90 @ =gEnemyParty\n\
    cmp r0, 0\n\
    bne _08029A62\n\
    ldr r7, _08029A94 @ =gPlayerParty\n\
_08029A62:\n\
    ldr r2, _08029A98 @ =gBattleMons\n\
    ldr r0, _08029A9C @ =gBattlerTarget\n\
    ldrb r1, [r0]\n\
    movs r0, 0x58\n\
    muls r0, r1\n\
    adds r0, r2\n\
    ldrh r0, [r0, 0x28]\n\
    cmp r0, 0\n\
    bne _08029AA4\n\
    ldr r3, _08029AA0 @ =gBattlescriptCurrInstr\n\
    ldr r2, [r3]\n\
    ldrb r1, [r2, 0x1]\n\
    ldrb r0, [r2, 0x2]\n\
    lsls r0, 8\n\
    orrs r1, r0\n\
    ldrb r0, [r2, 0x3]\n\
    lsls r0, 16\n\
    orrs r1, r0\n\
    ldrb r0, [r2, 0x4]\n\
    b _08029C40\n\
    .align 2, 0\n\
_08029A8C: .4byte gBattlerAttacker\n\
_08029A90: .4byte gEnemyParty\n\
_08029A94: .4byte gPlayerParty\n\
_08029A98: .4byte gBattleMons\n\
_08029A9C: .4byte gBattlerTarget\n\
_08029AA0: .4byte gBattlescriptCurrInstr\n\
_08029AA4:\n\
    ldr r6, _08029BE0 @ =gBattleCommunication\n\
    ldrb r0, [r6]\n\
    mov r8, r0\n\
    cmp r0, 0x5\n\
    bls _08029AB0\n\
    b _08029C0C\n\
_08029AB0:\n\
    adds r4, r6, 0\n\
    movs r5, 0x64\n\
_08029AB4:\n\
    ldrb r0, [r4]\n\
    muls r0, r5\n\
    adds r0, r7, r0\n\
    movs r1, 0x39\n\
    bl GetMonData\n\
    cmp r0, 0\n\
    beq _08029AF8\n\
    ldrb r0, [r6]\n\
    muls r0, r5\n\
    adds r0, r7, r0\n\
    movs r1, 0x41\n\
    bl GetMonData\n\
    cmp r0, 0\n\
    beq _08029AF8\n\
    ldrb r0, [r4]\n\
    muls r0, r5\n\
    adds r0, r7, r0\n\
    movs r1, 0x41\n\
    bl GetMonData\n\
    movs r1, 0xCE\n\
    lsls r1, 1\n\
    cmp r0, r1\n\
    beq _08029AF8\n\
    ldrb r0, [r4]\n\
    muls r0, r5\n\
    adds r0, r7, r0\n\
    movs r1, 0x37\n\
    bl GetMonData\n\
    cmp r0, 0\n\
    beq _08029B08\n\
_08029AF8:\n\
    ldrb r0, [r4]\n\
    adds r0, 0x1\n\
    strb r0, [r4]\n\
    adds r6, r4, 0\n\
    lsls r0, 24\n\
    lsrs r0, 24\n\
    cmp r0, 0x5\n\
    bls _08029AB4\n\
_08029B08:\n\
    ldr r1, _08029BE0 @ =gBattleCommunication\n\
    mov r9, r1\n\
    ldrb r2, [r1]\n\
    cmp r2, 0x5\n\
    bhi _08029C0C\n\
    ldr r1, _08029BE4 @ =gBattleTextBuff1\n\
    movs r0, 0xFD\n\
    strb r0, [r1]\n\
    movs r0, 0x4\n\
    strb r0, [r1, 0x1]\n\
    ldr r6, _08029BE8 @ =gBattlerAttacker\n\
    ldrb r0, [r6]\n\
    strb r0, [r1, 0x2]\n\
    strb r2, [r1, 0x3]\n\
    movs r0, 0xFF\n\
    strb r0, [r1, 0x4]\n\
    ldr r1, _08029BEC @ =gBattlescriptCurrInstr\n\
    ldr r0, [r1]\n\
    adds r0, 0x9\n\
    str r0, [r1]\n\
    ldr r2, _08029BF0 @ =gBattleMoveDamage\n\
    mov r8, r2\n\
    ldr r5, _08029BF4 @ =gBaseStats\n\
    mov r1, r9\n\
    ldrb r0, [r1]\n\
    movs r4, 0x64\n\
    muls r0, r4\n\
    adds r0, r7, r0\n\
    movs r1, 0xB\n\
    bl GetMonData\n\
    lsls r1, r0, 3\n\
    subs r1, r0\n\
    lsls r1, 2\n\
    adds r1, r5\n\
    ldrb r3, [r1, 0x1]\n\
    mov r2, r8\n\
    str r3, [r2]\n\
    ldr r2, _08029BF8 @ =gBattleMoves\n\
    ldr r0, _08029BFC @ =gCurrentMove\n\
    ldrh r1, [r0]\n\
    lsls r0, r1, 1\n\
    adds r0, r1\n\
    lsls r0, 2\n\
    adds r0, r2\n\
    ldrb r0, [r0, 0x1]\n\
    muls r0, r3\n\
    mov r1, r8\n\
    str r0, [r1]\n\
    mov r2, r9\n\
    ldrb r0, [r2]\n\
    muls r0, r4\n\
    adds r0, r7, r0\n\
    movs r1, 0x38\n\
    bl GetMonData\n\
    lsls r0, 1\n\
    movs r1, 0x5\n\
    bl __udivsi3\n\
    adds r0, 0x2\n\
    mov r2, r8\n\
    ldr r1, [r2]\n\
    muls r0, r1\n\
    str r0, [r2]\n\
    ldr r3, _08029C00 @ =gBattleMons\n\
    ldr r1, _08029C04 @ =gBattlerTarget\n\
    ldrb r2, [r1]\n\
    movs r1, 0x58\n\
    muls r1, r2\n\
    adds r1, r3\n\
    ldrh r2, [r1]\n\
    lsls r1, r2, 3\n\
    subs r1, r2\n\
    lsls r1, 2\n\
    adds r1, r5\n\
    ldrb r1, [r1, 0x2]\n\
    bl __divsi3\n\
    mov r1, r8\n\
    str r0, [r1]\n\
    movs r1, 0x32\n\
    bl __divsi3\n\
    adds r2, r0, 0x2\n\
    mov r0, r8\n\
    str r2, [r0]\n\
    ldr r1, _08029C08 @ =gProtectStructs\n\
    ldrb r0, [r6]\n\
    lsls r0, 4\n\
    adds r0, r1\n\
    ldrb r0, [r0]\n\
    lsls r0, 28\n\
    cmp r0, 0\n\
    bge _08029BD4\n\
    lsls r0, r2, 4\n\
    subs r0, r2\n\
    movs r1, 0xA\n\
    bl __divsi3\n\
    mov r1, r8\n\
    str r0, [r1]\n\
_08029BD4:\n\
    mov r2, r9\n\
    ldrb r0, [r2]\n\
    adds r0, 0x1\n\
    strb r0, [r2]\n\
    b _08029C46\n\
    .align 2, 0\n\
_08029BE0: .4byte gBattleCommunication\n\
_08029BE4: .4byte gBattleTextBuff1\n\
_08029BE8: .4byte gBattlerAttacker\n\
_08029BEC: .4byte gBattlescriptCurrInstr\n\
_08029BF0: .4byte gBattleMoveDamage\n\
_08029BF4: .4byte gBaseStats\n\
_08029BF8: .4byte gBattleMoves\n\
_08029BFC: .4byte gCurrentMove\n\
_08029C00: .4byte gBattleMons\n\
_08029C04: .4byte gBattlerTarget\n\
_08029C08: .4byte gProtectStructs\n\
_08029C0C:\n\
    mov r0, r8\n\
    cmp r0, 0\n\
    beq _08029C2C\n\
    ldr r3, _08029C28 @ =gBattlescriptCurrInstr\n\
    ldr r2, [r3]\n\
    ldrb r1, [r2, 0x1]\n\
    ldrb r0, [r2, 0x2]\n\
    lsls r0, 8\n\
    orrs r1, r0\n\
    ldrb r0, [r2, 0x3]\n\
    lsls r0, 16\n\
    orrs r1, r0\n\
    ldrb r0, [r2, 0x4]\n\
    b _08029C40\n\
    .align 2, 0\n\
_08029C28: .4byte gBattlescriptCurrInstr\n\
_08029C2C:\n\
    ldr r3, _08029C54 @ =gBattlescriptCurrInstr\n\
    ldr r2, [r3]\n\
    ldrb r1, [r2, 0x5]\n\
    ldrb r0, [r2, 0x6]\n\
    lsls r0, 8\n\
    orrs r1, r0\n\
    ldrb r0, [r2, 0x7]\n\
    lsls r0, 16\n\
    orrs r1, r0\n\
    ldrb r0, [r2, 0x8]\n\
_08029C40:\n\
    lsls r0, 24\n\
    orrs r1, r0\n\
    str r1, [r3]\n\
_08029C46:\n\
    pop {r3,r4}\n\
    mov r8, r3\n\
    mov r9, r4\n\
    pop {r4-r7}\n\
    pop {r0}\n\
    bx r0\n\
    .align 2, 0\n\
_08029C54: .4byte gBattlescriptCurrInstr\n\
        .syntax divided");
}
#endif // NONMATCHING

static void atkC5_setsemiinvulnerablebit(void)
{
    switch (gCurrentMove)
    {
    case MOVE_FLY:
    case MOVE_BOUNCE:
        gStatuses3[gBattlerAttacker] |= STATUS3_ON_AIR;
        break;
    case MOVE_DIG:
        gStatuses3[gBattlerAttacker] |= STATUS3_UNDERGROUND;
        break;
    case MOVE_DIVE:
        gStatuses3[gBattlerAttacker] |= STATUS3_UNDERWATER;
        break;
    }
    gBattlescriptCurrInstr++;
}

static void atkC6_clearsemiinvulnerablebit(void)
{
    switch (gCurrentMove)
    {
    case MOVE_FLY:
    case MOVE_BOUNCE:
        gStatuses3[gBattlerAttacker] &= ~STATUS3_ON_AIR;
        break;
    case MOVE_DIG:
        gStatuses3[gBattlerAttacker] &= ~STATUS3_UNDERGROUND;
        break;
    case MOVE_DIVE:
        gStatuses3[gBattlerAttacker] &= ~STATUS3_UNDERWATER;
        break;
    }
    gBattlescriptCurrInstr++;
}

static void atkC7_setminimize(void)
{
    if (gHitMarker & HITMARKER_OBEYS)
        gStatuses3[gBattlerAttacker] |= STATUS3_MINIMIZED;
    gBattlescriptCurrInstr++;
}

static void atkC8_sethail(void)
{
    if (gBattleWeather & WEATHER_HAIL)
    {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 2;
    }
    else
    {
        gBattleGlobalTimers.fog = 0;
        gBattleWeather = WEATHER_HAIL;
        gBattleCommunication[MULTISTRING_CHOOSER] = 5;
        if (ItemId_GetHoldEffect(gBattleMons[gBattlerAttacker].item) == HOLD_EFFECT_EXTEND_HAIL)
            gWishFutureKnock.weatherDuration = 8;
        else
            gWishFutureKnock.weatherDuration = 5;
    }
    gBattlescriptCurrInstr++;
}

static void atkC9_jumpifattackandspecialattackcannotfall(void) //memento
{
    if (gBattleMons[gBattlerTarget].statStages[STAT_STAGE_ATK] == 0
        && gBattleMons[gBattlerTarget].statStages[STAT_STAGE_SPATK] == 0
        && gBattleCommunication[6] != 1)
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    else
    {
        gActiveBattler = gBattlerAttacker;
        gBattleMoveDamage = gBattleMons[gActiveBattler].hp;
        BtlController_EmitHealthBarUpdate(0, 0x7FFF);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattlescriptCurrInstr += 5;
    }
}

static void atkCA_setforcedtarget(void) //follow me
{
    gSideTimers[GetBattlerSide(gBattlerAttacker)].followmeTimer = 1;
    gSideTimers[GetBattlerSide(gBattlerAttacker)].followmeTarget = gBattlerAttacker;
    gBattlescriptCurrInstr++;
}

static void atkCB_setcharge(void)
{
    gStatuses3[gBattlerAttacker] |= STATUS3_CHARGED_UP;
    gDisableStructs[gBattlerAttacker].chargeTimer1 = 2;
    gDisableStructs[gBattlerAttacker].chargeTimer2 = 2;
    gBattlescriptCurrInstr++;
}

static void atkCC_callterrainattack(void) //nature power
{
    gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);
    gCurrentMove = sNaturePowerMoves[gBattleTerrain];
    gBattlerTarget = GetMoveTarget(gCurrentMove, 0);
    BattleScriptPush(gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect]);
    gBattlescriptCurrInstr++;
}

static void atkCD_cureifburnedparalysedorpoisoned(void) //refresh
{
    if (gBattleMons[gBattlerAttacker].status1 & (STATUS1_POISON | STATUS1_BURN | STATUS1_PARALYSIS | STATUS1_TOXIC_POISON))
    {
        gBattleMons[gBattlerAttacker].status1 = 0;
        gBattlescriptCurrInstr += 5;
        gActiveBattler = gBattlerAttacker;
        BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
        MarkBattlerForControllerExec(gActiveBattler);
    }
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atkCE_settorment(void)
{
    if (gBattleMons[gBattlerTarget].status2 & STATUS2_TORMENT)
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    else
    {
        gBattleMons[gBattlerTarget].status2 |= STATUS2_TORMENT;
        gBattlescriptCurrInstr += 5;
    }
}

static void atkCF_jumpifnodamage(void)
{
    if (gProtectStructs[gBattlerAttacker].physicalDmg || gProtectStructs[gBattlerAttacker].specialDmg)
        gBattlescriptCurrInstr += 5;
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atkD0_settaunt(void)
{
    if (gDisableStructs[gBattlerTarget].tauntTimer1 == 0)
    {
        gDisableStructs[gBattlerTarget].tauntTimer1 = 2;
        gDisableStructs[gBattlerTarget].tauntTimer2 = 2;
        gBattlescriptCurrInstr += 5;
    }
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atkD1_trysethelpinghand(void)
{
    gBattlerTarget = GetBattlerAtPosition(GetBattlerPosition(gBattlerAttacker) ^ 2);
    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && !(gAbsentBattlerFlags & gBitTable[gBattlerTarget])
        && !gProtectStructs[gBattlerAttacker].helpingHand && !gProtectStructs[gBattlerTarget].helpingHand)
    {
        gProtectStructs[gBattlerTarget].helpingHand = 1;
        gBattlescriptCurrInstr += 5;
    }
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atkD2_tryswapitems(void)
{
    // opponent can't swap items with player in regular battles
    if ((GetBattlerSide(gBattlerAttacker) == B_SIDE_OPPONENT
         && !(gBattleTypeFlags & (BATTLE_TYPE_LINK
                                  | BATTLE_TYPE_EREADER_TRAINER
                                  | BATTLE_TYPE_BATTLE_TOWER))
         && gTrainerBattleOpponent != SECRET_BASE_OPPONENT)) {
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
    else {
        u8 sideAttacker = GetBattlerSide(gBattlerAttacker);

        // you can't swap items if they were knocked off in regular battles
        if (!(gBattleTypeFlags & (BATTLE_TYPE_LINK
                                  | BATTLE_TYPE_EREADER_TRAINER
                                  | BATTLE_TYPE_BATTLE_TOWER))
            && gTrainerBattleOpponent != SECRET_BASE_OPPONENT
            && (gWishFutureKnock.knockedOffPokes[sideAttacker] & gBitTable[gBattlerPartyIndexes[gBattlerAttacker]]
            )) {
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
        }
            // can't swap if two pokemon don't have an item
            // or if either of them is an enigma berry or a mail
        else if ((gBattleMons[gBattlerAttacker].item == 0 && gBattleMons[gBattlerTarget].item == 0)
                 || gBattleMons[gBattlerAttacker].item == ITEM_ENIGMA_BERRY
                 || gBattleMons[gBattlerTarget].item == ITEM_ENIGMA_BERRY
                 || IS_ITEM_MAIL(gBattleMons[gBattlerAttacker].item)
                 || IS_ITEM_MAIL(gBattleMons[gBattlerTarget].item)) {
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
        }
            // check if ability prevents swapping
        else if (GetBattlerAbility(gBattlerTarget) == ABILITY_STICKY_HOLD) {
            gBattlescriptCurrInstr = BattleScript_NoItemSteal;
            gLastUsedAbility = GetBattlerAbility(gBattlerTarget);
            RecordAbilityBattle(gBattlerTarget, gLastUsedAbility);
        }
            // took a while, but all checks passed and items can be safely swapped
        else {
            u16 oldItemAtk, *newItemAtk;

            newItemAtk = (u16 *)(ewram160F0 + 2 * gBattlerAttacker);
            oldItemAtk = gBattleMons[gBattlerAttacker].item;
            *newItemAtk = gBattleMons[gBattlerTarget].item;

            gBattleMons[gBattlerAttacker].item = 0;
            gBattleMons[gBattlerTarget].item = oldItemAtk;

            gActiveBattler = gBattlerAttacker;
            BtlController_EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, newItemAtk);
            MarkBattlerForControllerExec(gBattlerAttacker);

            gActiveBattler = gBattlerTarget;
            BtlController_EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gBattlerTarget].item);
            MarkBattlerForControllerExec(gBattlerTarget);

            ewram160E8arr2(0, gBattlerTarget) = 0;
            ewram160E8arr2(1, gBattlerTarget) = 0;
            ewram160E8arr2(0, gBattlerAttacker) = 0;
            ewram160E8arr2(1, gBattlerAttacker) = 0;

            gBattlescriptCurrInstr += 5;

            PREPARE_ITEM_BUFFER(gBattleTextBuff1, *newItemAtk)
            PREPARE_ITEM_BUFFER(gBattleTextBuff2, oldItemAtk)

            if (oldItemAtk != 0 && *newItemAtk != 0)
                gBattleCommunication[MULTISTRING_CHOOSER] = 2; // attacker's item -> <- target's item
            else if (oldItemAtk == 0 && *newItemAtk != 0)
                gBattleCommunication[MULTISTRING_CHOOSER] = 0; // nothing -> <- target's item
            else
                gBattleCommunication[MULTISTRING_CHOOSER] = 1; // attacker's item -> <- nothing
        }
    }
}

static void atkD3_trycopyability(void) //role play
{
    if (GetBattlerAbility(gBattlerTarget) != 0 && GetBattlerAbility(gBattlerTarget) != ABILITY_WONDER_GUARD)
    {
        gBattleMons[gBattlerAttacker].ability = GetBattlerAbility(gBattlerTarget);
        gLastUsedAbility = GetBattlerAbility(gBattlerTarget);
        gBattlescriptCurrInstr += 5;
    }
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atkD4_trywish(void)
{
    switch (T2_READ_8(gBattlescriptCurrInstr + 1))
    {
    case 0: //use wish
        if (gWishFutureKnock.wishCounter[gBattlerAttacker] == 0)
        {
            gWishFutureKnock.wishCounter[gBattlerAttacker] = 2;
            gWishFutureKnock.wishMonId[gBattlerAttacker] = gBattlerPartyIndexes[gBattlerAttacker];
            gBattlescriptCurrInstr += 6;
        }
        else
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
        break;
    case 1: //heal effect
        gBattleTextBuff1[0] = 0xFD;
        gBattleTextBuff1[1] = 4;
        gBattleTextBuff1[2] = gBattlerTarget;
        gBattleTextBuff1[3] = gWishFutureKnock.wishMonId[gBattlerTarget];
        gBattleTextBuff1[4] = 0xFF;
        gBattleMoveDamage = gBattleMons[gBattlerTarget].maxHP / 2;
        if (gBattleMoveDamage == 0)
            gBattleMoveDamage = 1;
        gBattleMoveDamage *= -1;
        if (gBattleMons[gBattlerTarget].hp == gBattleMons[gBattlerTarget].maxHP)
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
        else
            gBattlescriptCurrInstr += 6;
        break;
    }
}

static void atkD5_trysetroots(void) //ingrain
{
    if (gStatuses3[gBattlerAttacker] & STATUS3_ROOTED)
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    else
    {
        gStatuses3[gBattlerAttacker] |= STATUS3_ROOTED;
        gBattlescriptCurrInstr += 5;
    }
}

static void atkD6_doubledamagedealtifdamaged(void)
{
    if ((gProtectStructs[gBattlerAttacker].physicalDmg && gProtectStructs[gBattlerAttacker].physicalBattlerId == gBattlerTarget)
        || (gProtectStructs[gBattlerAttacker].specialDmg && gProtectStructs[gBattlerAttacker].specialBattlerId == gBattlerTarget))
            gBattleStruct->dmgMultiplier = 2;
    gBattlescriptCurrInstr++;
}

static void atkD7_setyawn(void)
{
    if (gStatuses3[gBattlerTarget] & STATUS3_YAWN || (u8) gBattleMons[gBattlerTarget].status1)
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    else
    {
        gStatuses3[gBattlerTarget] |= 0x1000;
        gBattlescriptCurrInstr += 5;
    }
}

static void atkD8_setdamagetohealthdifference(void)
{
    if (gBattleMons[gBattlerTarget].hp <= gBattleMons[gBattlerAttacker].hp)
    {
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
    else
    {
        gBattleMoveDamage = gBattleMons[gBattlerTarget].hp - gBattleMons[gBattlerAttacker].hp;
        gBattlescriptCurrInstr += 5;
    }
}

static void atkD9_scaledamagebyhealthratio(void)
{
    if (gDynamicBasePower == 0)
    {
        u8 power = gBattleMoves[gCurrentMove].power;
        gDynamicBasePower = gBattleMons[gBattlerAttacker].hp * power / gBattleMons[gBattlerAttacker].maxHP;
        if (gDynamicBasePower == 0)
            gDynamicBasePower = 1;
    }
    gBattlescriptCurrInstr++;
}

static void atkDA_tryswapabilities(void)
{
    if ((GetBattlerAbility(gBattlerAttacker) == 0 && GetBattlerAbility(gBattlerTarget) == 0)
     || GetBattlerAbility(gBattlerAttacker) == ABILITY_WONDER_GUARD || GetBattlerAbility(gBattlerTarget) == ABILITY_WONDER_GUARD
     || gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    else
    {
        u8 atk_ability = GetBattlerAbility(gBattlerAttacker);
        gBattleMons[gBattlerAttacker].ability = GetBattlerAbility(gBattlerTarget);
        gBattleMons[gBattlerTarget].ability = atk_ability;
        gBattlescriptCurrInstr += 5;
    }
}

static void atkDB_tryimprison(void)
{
    u8 r8 = 0;
    if ((gStatuses3[gBattlerAttacker] & STATUS3_IMPRISONED_OTHERS))
    {
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
    else
    {
        u8 bank;
        PressurePPLoseOnUsingImprision(gBattlerAttacker);
        for (bank = 0; bank < gBattlersCount; bank++)
        {
            if (r8 != GetBattlerSide(bank))
            {
                int j;
                for (j = 0; j < 4; j++)
                {
                    int k;
                    for (k = 0; k < 4; k++)
                    {
                        if (gBattleMons[gBattlerAttacker].moves[j] == gBattleMons[bank].moves[k] && gBattleMons[gBattlerAttacker].moves[j])
                            break;
                    }
                    if (k != 4)
                        break;
                }
                if (j != 4)
                {
                    gStatuses3[gBattlerAttacker] |= STATUS3_IMPRISONED_OTHERS;
                    gBattlescriptCurrInstr += 5;
                    break;
                }
            }
        }
        if (bank == gBattlersCount)
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
}

static void atkDC_trysetgrudge(void)
{
    if (gStatuses3[gBattlerAttacker] & STATUS3_GRUDGE)
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    else
    {
        gStatuses3[gBattlerAttacker] |= STATUS3_GRUDGE;
        gBattlescriptCurrInstr += 5;
    }
}

static void atkDD_weightdamagecalculation(void)
{
    int i;
    for (i = 0; sWeightToDamageTable[i] != 0xFFFF; i += 2)
    {
        if (sWeightToDamageTable[i] > GetPokedexHeightWeight(SpeciesToNationalPokedexNum(gBattleMons[gBattlerTarget].species), 1))
            break;
    }
    if (sWeightToDamageTable[i] != 0xFFFF)
        gDynamicBasePower = sWeightToDamageTable[i + 1];
    else
        gDynamicBasePower = 120;
    gBattlescriptCurrInstr++;
}

#ifdef NONMATCHING
static void atkDE_assistattackselect(void)
{
    u32 chooseable_moves_no = 0;
    struct Pokemon* party;
    int i, j;
    u16* chooseable_moves;
    if (GetBattlerPosition(gBattlerAttacker) & 1)
        party = gEnemyParty;
    else
        party = gPlayerParty;

    for (i = 0; i < 6; i++)
    {
        if (i == gBattlerPartyIndexes[gBattlerAttacker])
            break;
        if (!GetMonData(&party[i], MON_DATA_SPECIES2) || GetMonData(&party[i], MON_DATA_SPECIES2) == SPECIES_EGG)
            break;
        chooseable_moves = &gBattleStruct->assistMove[chooseable_moves_no];
        for (j = 0; j < 4; j++)
        {
            int k;
            u16 move = GetMonData(&party[i], MON_DATA_MOVE1 + i);
            if (IsMoveUnchoosable(move))
                break;
            //sMovesForbiddenToCopy[k]
            for (k = 0; ;k++)
            {
                if (sMovesForbiddenToCopy[k] == 0xFFFF)
                {
                    if (move)
                    {
                        *chooseable_moves = move;
                        chooseable_moves++;
                        chooseable_moves_no++;
                    }
                    break;
                }
                if (sMovesForbiddenToCopy[k] == move)
                    break;
            }
        }
    }
    if (chooseable_moves_no)
    {
        gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);
        gRandomMove = gBattleStruct->assistMove[Random() % chooseable_moves_no];
        gBattlerTarget = GetMoveTarget(gRandomMove, 0);
        gBattlescriptCurrInstr += 5;
    }
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

#else
NAKED
static void atkDE_assistattackselect(void)
{
    asm(".syntax unified\n\
    push {r4-r7,lr}\n\
    mov r7, r10\n\
    mov r6, r9\n\
    mov r5, r8\n\
    push {r5-r7}\n\
    sub sp, 0x8\n\
    movs r0, 0\n\
    mov r10, r0\n\
    ldr r0, _0802AB9C @ =gBattlerAttacker\n\
    ldrb r0, [r0]\n\
    bl GetBattlerPosition\n\
    movs r1, 0x1\n\
    ands r1, r0\n\
    ldr r0, _0802ABA0 @ =gPlayerParty\n\
    str r0, [sp]\n\
    cmp r1, 0\n\
    beq _0802AAAC\n\
    ldr r1, _0802ABA4 @ =gEnemyParty\n\
    str r1, [sp]\n\
_0802AAAC:\n\
    movs r2, 0\n\
_0802AAAE:\n\
    ldr r1, _0802ABA8 @ =gBattlerPartyIndexes\n\
    ldr r0, _0802AB9C @ =gBattlerAttacker\n\
    ldrb r0, [r0]\n\
    lsls r0, 1\n\
    adds r0, r1\n\
    adds r1, r2, 0x1\n\
    str r1, [sp, 0x4]\n\
    ldrh r0, [r0]\n\
    cmp r2, r0\n\
    beq _0802AB54\n\
    movs r0, 0x64\n\
    adds r6, r2, 0\n\
    muls r6, r0\n\
    ldr r0, [sp]\n\
    adds r4, r0, r6\n\
    adds r0, r4, 0\n\
    movs r1, 0x41\n\
    bl GetMonData\n\
    cmp r0, 0\n\
    beq _0802AB54\n\
    adds r0, r4, 0\n\
    movs r1, 0x41\n\
    bl GetMonData\n\
    movs r1, 0xCE\n\
    lsls r1, 1\n\
    cmp r0, r1\n\
    beq _0802AB54\n\
    movs r5, 0\n\
    ldr r1, _0802ABAC @ =0x0000ffff\n\
    mov r8, r1\n\
    mov r9, r6\n\
    mov r1, r10\n\
    lsls r0, r1, 1\n\
    ldr r1, _0802ABB0 @ =gSharedMem + 0x16024\n\
    adds r6, r0, r1\n\
_0802AAF8:\n\
    movs r7, 0\n\
    adds r1, r5, 0\n\
    adds r1, 0xD\n\
    ldr r0, [sp]\n\
    add r0, r9\n\
    bl GetMonData\n\
    lsls r0, 16\n\
    lsrs r4, r0, 16\n\
    adds r0, r4, 0\n\
    bl IsMoveUnchoosable\n\
    lsls r0, 24\n\
    adds r1, r5, 0x1\n\
    cmp r0, 0\n\
    bne _0802AB4E\n\
    ldr r0, _0802ABB4 @ =sMovesForbiddenToCopy\n\
    ldrh r2, [r0]\n\
    adds r3, r0, 0\n\
    cmp r2, r8\n\
    beq _0802AB42\n\
    cmp r4, r2\n\
    beq _0802AB38\n\
    ldr r5, _0802ABAC @ =0x0000ffff\n\
    adds r2, r3, 0\n\
_0802AB2A:\n\
    adds r2, 0x2\n\
    adds r7, 0x1\n\
    ldrh r0, [r2]\n\
    cmp r0, r5\n\
    beq _0802AB42\n\
    cmp r4, r0\n\
    bne _0802AB2A\n\
_0802AB38:\n\
    lsls r0, r7, 1\n\
    adds r0, r3\n\
    ldrh r0, [r0]\n\
    cmp r0, r8\n\
    bne _0802AB4E\n\
_0802AB42:\n\
    cmp r4, 0\n\
    beq _0802AB4E\n\
    strh r4, [r6]\n\
    adds r6, 0x2\n\
    movs r0, 0x1\n\
    add r10, r0\n\
_0802AB4E:\n\
    adds r5, r1, 0\n\
    cmp r5, 0x3\n\
    ble _0802AAF8\n\
_0802AB54:\n\
    ldr r2, [sp, 0x4]\n\
    cmp r2, 0x5\n\
    ble _0802AAAE\n\
    mov r1, r10\n\
    cmp r1, 0\n\
    beq _0802ABCC\n\
    ldr r2, _0802ABB8 @ =gHitMarker\n\
    ldr r0, [r2]\n\
    ldr r1, _0802ABBC @ =0xfffffbff\n\
    ands r0, r1\n\
    str r0, [r2]\n\
    ldr r4, _0802ABC0 @ =gRandomMove\n\
    bl Random\n\
    movs r1, 0xFF\n\
    ands r1, r0\n\
    mov r0, r10\n\
    muls r0, r1\n\
    asrs r0, 8\n\
    lsls r0, 1\n\
    ldr r1, _0802ABB0 @ =gSharedMem + 0x16024\n\
    adds r0, r1\n\
    ldrh r0, [r0]\n\
    strh r0, [r4]\n\
    ldrh r0, [r4]\n\
    movs r1, 0\n\
    bl GetMoveTarget\n\
    ldr r1, _0802ABC4 @ =gBattlerTarget\n\
    strb r0, [r1]\n\
    ldr r1, _0802ABC8 @ =gBattlescriptCurrInstr\n\
    ldr r0, [r1]\n\
    adds r0, 0x5\n\
    str r0, [r1]\n\
    b _0802ABE6\n\
    .align 2, 0\n\
_0802AB9C: .4byte gBattlerAttacker\n\
_0802ABA0: .4byte gPlayerParty\n\
_0802ABA4: .4byte gEnemyParty\n\
_0802ABA8: .4byte gBattlerPartyIndexes\n\
_0802ABAC: .4byte 0x0000ffff\n\
_0802ABB0: .4byte gSharedMem + 0x16024\n\
_0802ABB4: .4byte sMovesForbiddenToCopy\n\
_0802ABB8: .4byte gHitMarker\n\
_0802ABBC: .4byte 0xfffffbff\n\
_0802ABC0: .4byte gRandomMove\n\
_0802ABC4: .4byte gBattlerTarget\n\
_0802ABC8: .4byte gBattlescriptCurrInstr\n\
_0802ABCC:\n\
    ldr r3, _0802ABF8 @ =gBattlescriptCurrInstr\n\
    ldr r2, [r3]\n\
    ldrb r1, [r2, 0x1]\n\
    ldrb r0, [r2, 0x2]\n\
    lsls r0, 8\n\
    orrs r1, r0\n\
    ldrb r0, [r2, 0x3]\n\
    lsls r0, 16\n\
    orrs r1, r0\n\
    ldrb r0, [r2, 0x4]\n\
    lsls r0, 24\n\
    orrs r1, r0\n\
    str r1, [r3]\n\
_0802ABE6:\n\
    add sp, 0x8\n\
    pop {r3-r5}\n\
    mov r8, r3\n\
    mov r9, r4\n\
    mov r10, r5\n\
    pop {r4-r7}\n\
    pop {r0}\n\
    bx r0\n\
    .align 2, 0\n\
_0802ABF8: .4byte gBattlescriptCurrInstr\n\
        .syntax divided");
}

#endif // NONMATCHING

static void atkDF_trysetmagiccoat(void)
{
    gBattlerTarget = gBattlerAttacker;
    gSpecialStatuses[gBattlerAttacker].ppNotAffectedByPressure = 1;
    if (gCurrentTurnActionNumber == gBattlersCount - 1) //last turn
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    else
    {
        gProtectStructs[gBattlerAttacker].bounceMove = 1;
        gBattlescriptCurrInstr += 5;
    }
}

static void atkE0_trysetsnatch(void)
{
    gSpecialStatuses[gBattlerAttacker].ppNotAffectedByPressure = 1;
    if (gCurrentTurnActionNumber == gBattlersCount - 1) //last turn
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    else
    {
        gProtectStructs[gBattlerAttacker].stealMove = 1;
        gBattlescriptCurrInstr += 5;
    }
}

static void atkE1_trygetintimidatetarget(void)
{
    u8 side;

    gBattleStruct->scriptingActive = ewram160DD;
    side = GetBattlerSide(gBattleStruct->scriptingActive);
    gBattleTextBuff1[0] = 0xFD;
    gBattleTextBuff1[1] = 9;
    gBattleTextBuff1[2] = GetBattlerAbility(gBattleStruct->scriptingActive);
    gBattleTextBuff1[3] = 0xFF;

    for (;gBattlerTarget < gBattlersCount; gBattlerTarget++)
    {
        if (GetBattlerSide(gBattlerTarget) == side)
            continue;
        if (!(gAbsentBattlerFlags & gBitTable[gBattlerTarget]))
            break;
    }

    if (gBattlerTarget >= gBattlersCount)
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    else
        gBattlescriptCurrInstr += 5;
}

static void atkE2_switchoutabilities(void)
{
    gActiveBattler = GetBattlerForBattleScript(T2_READ_8(gBattlescriptCurrInstr + 1));
    switch (GetBattlerAbility(gActiveBattler))
    {
    case ABILITY_NATURAL_CURE:
        gBattleMons[gActiveBattler].status1 = 0;
        BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, gBitTable[ewram16064arr(gActiveBattler)], 4, &gBattleMons[gActiveBattler].status1);
        MarkBattlerForControllerExec(gActiveBattler);
        break;
    }
    gBattlescriptCurrInstr += 2;
}

static void atkE3_jumpifhasnohp(void)
{
    gActiveBattler = GetBattlerForBattleScript(T2_READ_8(gBattlescriptCurrInstr + 1));
    if (gBattleMons[gActiveBattler].hp == 0)
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
    else
        gBattlescriptCurrInstr += 6;
}

static void atkE4_getsecretpowereffect(void)
{
    switch (gBattleTerrain)
    {
    case BATTLE_TERRAIN_GRASS:
        gBattleCommunication[MOVE_EFFECT_BYTE] = 2;
        break;
    case BATTLE_TERRAIN_LONG_GRASS:
        gBattleCommunication[MOVE_EFFECT_BYTE] = 1;
        break;
    case BATTLE_TERRAIN_SAND:
        gBattleCommunication[MOVE_EFFECT_BYTE] = 27;
        break;
    case BATTLE_TERRAIN_UNDERWATER:
        gBattleCommunication[MOVE_EFFECT_BYTE] = 23;
        break;
    case BATTLE_TERRAIN_WATER:
        gBattleCommunication[MOVE_EFFECT_BYTE] = 22;
        break;
    case BATTLE_TERRAIN_POND:
        gBattleCommunication[MOVE_EFFECT_BYTE] = 24;
        break;
    case BATTLE_TERRAIN_MOUNTAIN:
        gBattleCommunication[MOVE_EFFECT_BYTE] = 7;
        break;
    case BATTLE_TERRAIN_CAVE:
        gBattleCommunication[MOVE_EFFECT_BYTE] = 8;
        break;
    default:
        gBattleCommunication[MOVE_EFFECT_BYTE] = 5;
        break;
    }
    gBattlescriptCurrInstr++;
}

static void atkE5_pickup(void)
{
    int i;
    for (i = 0; i < 6; i++)
    {
        u16 species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2);
        u16 held_item = GetMonData(&gPlayerParty[i], MON_DATA_HELD_ITEM);
        u8 level = GetMonData(&gPlayerParty[i], MON_DATA_LEVEL);
        u8 ability;
        if (GetMonData(&gPlayerParty[i], MON_DATA_HIDDEN_ABILITY))
            ability = gBaseStats[species].hiddenAbility;
        else if (GetMonData(&gPlayerParty[i], MON_DATA_ALT_ABILITY))
            ability = gBaseStats[species].ability2;
        else
            ability = gBaseStats[species].ability1;

        level = (int)(level / 10);

        if (ability == ABILITY_PICKUP && species != 0 && species != SPECIES_EGG && held_item == 0 && (Random() % 10) == 0)
        {
            s32 chance = Random() % 100;
            s32 j;

            for (j = 0; j < 22; j += 2)
            {
                if (sPickupItems[level][j + 1] > chance)
                    break;
            }
            SetMonData(&gPlayerParty[i], MON_DATA_HELD_ITEM, (const void*) &sPickupItems[level][j]);
        }

        if (ability == ABILITY_HONEY_GATHER && species != 0 && species != SPECIES_EGG && held_item == 0 && ((Random() % (20 / level)) < 1.0))
        {
            held_item = ITEM_HONEY;
            SetMonData(&gPlayerParty[i], MON_DATA_HELD_ITEM, &held_item);
        }

    }
    gBattlescriptCurrInstr++;
}

static void atkE6_docastformchangeanimation(void)
{
    gActiveBattler = gBattleStruct->scriptingActive;
    if (gBattleMons[gActiveBattler].status2 & STATUS2_SUBSTITUTE)
        gBattleStruct->castformToChangeInto |= 0x80;
    BtlController_EmitBattleAnimation(0, B_ANIM_CASTFORM_CHANGE, gBattleStruct->castformToChangeInto);
    MarkBattlerForControllerExec(gActiveBattler);
    gBattlescriptCurrInstr++;
}

static void atkE7_trycastformdatachange(void)
{
    u8 form;
    gBattlescriptCurrInstr++;
    form = CastformDataTypeChange(gBattleStruct->scriptingActive);
    if (form)
    {
        BattleScriptPushCursorAndCallback(BattleScript_CastformChange);
        gBattleStruct->castformToChangeInto = form - 1;
    }
}

static void atkE8_settypebasedhalvers(void) //water/mud sport
{
    bool8 worked = FALSE;
    if (gBattleMoves[gCurrentMove].effect == EFFECT_MUD_SPORT)
    {
        if (!(gStatuses3[gBattlerAttacker] & STATUS3_MUDSPORT))
        {
            gStatuses3[gBattlerAttacker] |= STATUS3_MUDSPORT;
            gBattleCommunication[MULTISTRING_CHOOSER] = 0;
            worked = TRUE;
        }
    }
    else //water sport
    {
        if (!(gStatuses3[gBattlerAttacker] & STATUS3_WATERSPORT))
        {
            gStatuses3[gBattlerAttacker] |= STATUS3_WATERSPORT;
            gBattleCommunication[MULTISTRING_CHOOSER] = 1;
            worked = TRUE;
        }
    }
    if (worked)
        gBattlescriptCurrInstr += 5;
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atkE9_setweatherballtype(void)
{
    if (WEATHER_HAS_EFFECT)
    {
        if ((u8)(gBattleWeather))
            gBattleStruct->dmgMultiplier = 2;
        if (gBattleWeather & WEATHER_RAIN_ANY)
            gBattleStruct->dynamicMoveType = TYPE_WATER | 0x80;
        else if (gBattleWeather & WEATHER_SANDSTORM_ANY)
            gBattleStruct->dynamicMoveType = TYPE_ROCK | 0x80;
        else if (gBattleWeather & WEATHER_SUN_ANY)
            gBattleStruct->dynamicMoveType = TYPE_FIRE | 0x80;
        else if (gBattleWeather & WEATHER_HAIL)
            gBattleStruct->dynamicMoveType = TYPE_ICE | 0x80;
        else
            gBattleStruct->dynamicMoveType = TYPE_NORMAL | 0x80;
    }
    gBattlescriptCurrInstr++;
}

static void atkEA_tryrecycleitem(void)
{
    u16* used_item;
    gActiveBattler = gBattlerAttacker;
    used_item = USED_HELD_ITEM(gActiveBattler);
    if (*used_item && gBattleMons[gActiveBattler].item == 0)
    {
        gLastUsedItem = *used_item;
        *used_item = 0;
        gBattleMons[gActiveBattler].item = gLastUsedItem;
        BtlController_EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gActiveBattler].item);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattlescriptCurrInstr += 5;
    }
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atkEB_settypetoterrain(void)
{
    if (gBattleMons[gBattlerAttacker].type1 != sTerrainToType[gBattleTerrain] && gBattleMons[gBattlerAttacker].type2 != sTerrainToType[gBattleTerrain])
    {
        gBattleMons[gBattlerAttacker].type1 = sTerrainToType[gBattleTerrain];
        gBattleMons[gBattlerAttacker].type2 = sTerrainToType[gBattleTerrain];
        gBattleTextBuff1[0] = 0xFD;
        gBattleTextBuff1[1] = 3;
        gBattleTextBuff1[2] = sTerrainToType[gBattleTerrain];
        gBattleTextBuff1[3] = 0xFF;
        gBattlescriptCurrInstr += 5;
    }
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atkEC_pursuitrelated(void)
{
    gActiveBattler = GetBattlerAtPosition(GetBattlerPosition(gBattlerAttacker) ^ 2);
    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && !(gAbsentBattlerFlags & gBitTable[gActiveBattler]) && gActionForBanks[gActiveBattler] == 0 && gChosenMovesByBanks[gActiveBattler] == MOVE_PURSUIT)
    {
        gActionsByTurnOrder[gActiveBattler] = 11;
        gCurrentMove = MOVE_PURSUIT;
        gBattlescriptCurrInstr += 5;
        gBattleStruct->animTurn = 1;
        gBattleStruct->unk160A7 = gBattlerAttacker;
        gBattlerAttacker = gActiveBattler;
    }
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atkEF_snatchsetbattlers(void)
{
    gEffectBattler = gBattlerAttacker;
    if (gBattlerAttacker == gBattlerTarget)
        gBattlerAttacker = gBattlerTarget = gBattleStruct->scriptingActive;
    else
        gBattlerTarget = gBattleStruct->scriptingActive;
    gBattleStruct->scriptingActive = gEffectBattler;
    gBattlescriptCurrInstr++;
}

static void atkEE_removelightscreenreflect(void) //brick break
{
    u8 side = GetBattlerSide(gBattlerAttacker) ^ 1;
    if (gSideTimers[side].reflectTimer || gSideTimers[side].lightscreenTimer)
    {
        gSideStatuses[side] &= ~(SIDE_STATUS_REFLECT);
        gSideStatuses[side] &= ~(SIDE_STATUS_LIGHTSCREEN);
        gSideTimers[side].reflectTimer = 0;
        gSideTimers[side].lightscreenTimer = 0;
        gBattleStruct->animTurn = 1;
        gBattleStruct->animTargetsHit = 1;
    }
    else
    {
        gBattleStruct->animTurn = 0;
        gBattleStruct->animTargetsHit = 0;
    }
    gBattlescriptCurrInstr++;
}

void atkEF_handleballthrow(void)
{
    u8 ball_multiplier = 0;
    if (gBattleControllerExecFlags)
        return;

    gActiveBattler = gBattlerAttacker;
    gBattlerTarget = gBattlerAttacker ^ 1;
    if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
    {
        BtlController_EmitBallThrowAnim(0, 5);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattlescriptCurrInstr = BattleScript_TrainerBallBlock;
    }
    else if (gBattleTypeFlags & BATTLE_TYPE_WALLY_TUTORIAL)
    {
        BtlController_EmitBallThrowAnim(0, 4);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattlescriptCurrInstr = BattleScript_WallyBallThrow;
    }
    else
    {
        u32 odds;
        u8 catch_rate;
        u32 status = 0;
        u16 weight;
        u16 species;
        
        
        if (gLastUsedItem == ITEM_SAFARI_BALL || gLastUsedItem == ITEM_SPORT_BALL)
            catch_rate = gBattleStruct->unk16089 * 1275 / 100; //correct the name to safariFleeRate
        else
            catch_rate = gBaseStats[gBattleMons[gBattlerTarget].species].catchRate;
        if (gLastUsedItem > 5)
        {
            switch (gLastUsedItem)
            {
            case ITEM_NET_BALL:
                if (gBattleMons[gBattlerTarget].type1 == TYPE_WATER || gBattleMons[gBattlerTarget].type2 == TYPE_WATER || gBattleMons[gBattlerTarget].type1 == TYPE_BUG || gBattleMons[gBattlerTarget].type2 == TYPE_BUG)
                    ball_multiplier = 30;
                else
                    ball_multiplier = 10;
                break;
            case ITEM_DIVE_BALL:
                if (Overworld_GetMapTypeOfSaveblockLocation() == 5)
                    ball_multiplier = 35;
                else
                    ball_multiplier = 10;
                break;
            case ITEM_NEST_BALL:
                if (gBattleMons[gBattlerTarget].level <= 39)
                {
                    ball_multiplier = 40 - gBattleMons[gBattlerTarget].level;
                    if (ball_multiplier <= 9)
                        ball_multiplier = 10;
                }
                else
                    ball_multiplier = 10;
                break;
            case ITEM_REPEAT_BALL:
                if (GetSetPokedexFlag(SpeciesToNationalPokedexNum(gBattleMons[gBattlerTarget].species), 1))
                    ball_multiplier = 30;
                else
                    ball_multiplier = 10;
                break;
            case ITEM_TIMER_BALL:
                ball_multiplier = gBattleResults.battleTurnCounter + 10;
                if (ball_multiplier > 40)
                    ball_multiplier = 40;
                break;
            case ITEM_LUXURY_BALL:
            case ITEM_PREMIER_BALL:
            case ITEM_HEAL_BALL:
            case ITEM_CHERISH_BALL:
            case ITEM_FRIEND_BALL:
                ball_multiplier = 10;
                break;
            case ITEM_SPORT_BALL:
            case ITEM_PARK_BALL:
            case ITEM_DREAM_BALL:
                ball_multiplier = 15;
                break;
            case ITEM_LEVEL_BALL:
                if (gBattleMons[gBattlerAttacker].level <= gBattleMons[gBattlerTarget].level)
                    ball_multiplier = 10;
                else if (gBattleMons[gBattlerAttacker].level <= (2 * gBattleMons[gBattlerTarget].level))
                    ball_multiplier = 20;
                else if (gBattleMons[gBattlerAttacker].level <= (4 * gBattleMons[gBattlerTarget].level))
                    ball_multiplier = 40;
                else
                    ball_multiplier = 80;
                break;
            case ITEM_LURE_BALL: // not exactly how it works but whatever
                if (gBattleTerrain == BATTLE_TERRAIN_UNDERWATER || gBattleTerrain == BATTLE_TERRAIN_WATER || gBattleTerrain == BATTLE_TERRAIN_POND)
                    ball_multiplier = 50;
                else
                    ball_multiplier = 10;
                break;
            case ITEM_MOON_BALL:
                if (gBattleMons[gBattlerTarget].species == SPECIES_NIDORAN_F 
                    || gBattleMons[gBattlerTarget].species == SPECIES_NIDORINA
                    || gBattleMons[gBattlerTarget].species == SPECIES_NIDOQUEEN
                    || gBattleMons[gBattlerTarget].species == SPECIES_NIDORAN_M
                    || gBattleMons[gBattlerTarget].species == SPECIES_NIDORINO
                    || gBattleMons[gBattlerTarget].species == SPECIES_NIDOKING
                    || gBattleMons[gBattlerTarget].species == SPECIES_CLEFFA
                    || gBattleMons[gBattlerTarget].species == SPECIES_CLEFAIRY
                    || gBattleMons[gBattlerTarget].species == SPECIES_CLEFABLE
                    || gBattleMons[gBattlerTarget].species == SPECIES_IGGLYBUFF
                    || gBattleMons[gBattlerTarget].species == SPECIES_JIGGLYPUFF
                    || gBattleMons[gBattlerTarget].species == SPECIES_WIGGLYTUFF
                    || gBattleMons[gBattlerTarget].species == SPECIES_SKITTY
                    || gBattleMons[gBattlerTarget].species == SPECIES_DELCATTY
                    //|| gBattleMons[gBattlerTarget].species == SPECIES_MUNNA
                    //|| gBattleMons[gBattlerTarget].species == SPECIES_MUSHARNA
                    )
                    ball_multiplier = 40;
                else
                    ball_multiplier = 10;
                break;
            case ITEM_LOVE_BALL:
                if (gBattleMons[gBattlerTarget].species == gBattleMons[gBattlerAttacker].species
                    && GetGenderFromSpeciesAndPersonality(gBattleMons[gBattlerTarget].species, gBattleMons[gBattlerTarget].personality) != GetGenderFromSpeciesAndPersonality(gBattleMons[gBattlerAttacker].species, gBattleMons[gBattlerAttacker].personality)
                    && GetGenderFromSpeciesAndPersonality(gBattleMons[gBattlerTarget].species, gBattleMons[gBattlerTarget].personality) != MON_GENDERLESS
                    && GetGenderFromSpeciesAndPersonality(gBattleMons[gBattlerAttacker].species, gBattleMons[gBattlerAttacker].personality) != MON_GENDERLESS)
                    ball_multiplier = 80;
                else
                    ball_multiplier = 10;
                break;
            case ITEM_HEAVY_BALL: // all values in hectograms, current as of s/m
                weight = GetPokedexHeightWeight(SpeciesToNationalPokedexNum(gBattleMons[gBattlerTarget].species), 1);
                
                if (weight <= 1000) //220.46 lbs
                {
                    if (catch_rate > 20)
                        catch_rate -= 20;
                    else 
                        catch_rate = 0;
                }
                else if (weight <= 3000 && weight > 2000) //440.92-661.38 lbs
                {
                    catch_rate += 20;
                }
                else
                {
                    catch_rate += 30;
                }
                ball_multiplier = 10;
                break;
            case ITEM_FAST_BALL:
                species = gBattleMons[gBattlerTarget].species;
                ball_multiplier = 10;
                if (gBaseStats[species].baseSpeed > 100)
                    ball_multiplier = 40;
                break;
            case ITEM_DUSK_BALL:    // figure this one out later
                /*uXX time;
                if (time == night || Overworld_GetMapTypeOfSaveblockLocation() == cavePlaces)
                    ball_multiplier = 30;
                else*/
                    ball_multiplier = 10;
                break;
            case ITEM_QUICK_BALL:
                if (gBattleResults.battleTurnCounter == 0)
                    ball_multiplier = 50;
                else
                    ball_multiplier = 10;
                break;
            case ITEM_BEAST_BALL:
                if (gBattleMons[gBattlerTarget].species == SPECIES_NIHILEGO
                    || gBattleMons[gBattlerTarget].species == SPECIES_BUZZWOLE
                    || gBattleMons[gBattlerTarget].species == SPECIES_PHEROMOSA
                    || gBattleMons[gBattlerTarget].species == SPECIES_XURKITREE
                    || gBattleMons[gBattlerTarget].species == SPECIES_CELESTEELA
                    || gBattleMons[gBattlerTarget].species == SPECIES_KARTANA
                    || gBattleMons[gBattlerTarget].species == SPECIES_GUZZLORD
                    || gBattleMons[gBattlerTarget].species == SPECIES_POIPOLE
                    || gBattleMons[gBattlerTarget].species == SPECIES_NAGANADEL
                    || gBattleMons[gBattlerTarget].species == SPECIES_STAKATAKA
                    || gBattleMons[gBattlerTarget].species == SPECIES_BLACEPHALON)
                    ball_multiplier = 50;
                else
                    ball_multiplier = 1;
                break;
            }
        }
        else
            ball_multiplier = sBallCatchBonuses[gLastUsedItem - 2];

        odds = (catch_rate * ball_multiplier / 10) * (gBattleMons[gBattlerTarget].maxHP * 3 - gBattleMons[gBattlerTarget].hp * 2) / (3 * gBattleMons[gBattlerTarget].maxHP);
        if (gBattleMons[gBattlerTarget].status1 & (STATUS1_SLEEP | STATUS1_FREEZE))
            odds *= 2;
        if (gBattleMons[gBattlerTarget].status1 & (STATUS1_POISON | STATUS1_BURN | STATUS1_PARALYSIS | STATUS1_TOXIC_POISON))
            odds = (odds * 15) / 10;

        if (gLastUsedItem != ITEM_SAFARI_BALL && gLastUsedItem != ITEM_SPORT_BALL)
        {
            if (gLastUsedItem == ITEM_MASTER_BALL || gLastUsedItem != ITEM_PARK_BALL || gLastUsedItem != ITEM_DREAM_BALL)
            {
                gBattleResults.unk5_1 = 1;
            }
            else
            {
                if (gBattleResults.usedBalls[gLastUsedItem - ITEM_ULTRA_BALL] < 0xFF)
                    gBattleResults.usedBalls[gLastUsedItem - ITEM_ULTRA_BALL]++;
            }
        }
        if (odds > 254) //poke caught
        {
            BtlController_EmitBallThrowAnim(0, 4);
            MarkBattlerForControllerExec(gActiveBattler);
            gBattlescriptCurrInstr = BattleScript_SuccessBallThrow;
            SetMonData(&gEnemyParty[gBattlerPartyIndexes[gBattlerTarget]], MON_DATA_POKEBALL, (const void*) &gLastUsedItem);
            if (gLastUsedItem == ITEM_HEAL_BALL) // kachow, gotta add that heal ball
            {
                SetMonData(&gEnemyParty[gBattlerPartyIndexes[gBattlerTarget]], MON_DATA_HP, &gEnemyParty[gBattlerPartyIndexes[gBattlerTarget]].maxHP);
                SetMonData(&gEnemyParty[gBattlerPartyIndexes[gBattlerTarget]], MON_DATA_STATUS, &status);
            }
            if (CalculatePlayerPartyCount() == 6)
                gBattleCommunication[MULTISTRING_CHOOSER] = 0;
            else
                gBattleCommunication[MULTISTRING_CHOOSER] = 1;
        }
        else //poke may be caught, calculate shakes
        {
            u8 shakes;
            odds = Sqrt(Sqrt(16711680 / odds));
            odds = 1048560 / odds;
            for (shakes = 0; shakes < 4 && Random() < odds; shakes++) {}
            if (gLastUsedItem == ITEM_MASTER_BALL)
                shakes = 4; //why calculate the shakes before that check?
            BtlController_EmitBallThrowAnim(0, shakes);
            MarkBattlerForControllerExec(gActiveBattler);
            if (shakes == 4) //poke caught, copy of the code above
            {
                gBattlescriptCurrInstr = BattleScript_SuccessBallThrow;
                SetMonData(&gEnemyParty[gBattlerPartyIndexes[gBattlerTarget]], MON_DATA_POKEBALL, (const void*) &gLastUsedItem);
                if (gLastUsedItem == ITEM_HEAL_BALL) // heal ball copy
                {
                    SetMonData(&gEnemyParty[gBattlerPartyIndexes[gBattlerTarget]], MON_DATA_HP, &gEnemyParty[gBattlerPartyIndexes[gBattlerTarget]].maxHP);
                    SetMonData(&gEnemyParty[gBattlerPartyIndexes[gBattlerTarget]], MON_DATA_STATUS, &status);
                }
                if (CalculatePlayerPartyCount() == 6)
                    gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                else
                    gBattleCommunication[MULTISTRING_CHOOSER] = 1;
            }
            else //rip
            {
                gBattleCommunication[MULTISTRING_CHOOSER] = shakes;
                gBattlescriptCurrInstr = BattleScript_ShakeBallThrow;
            }
        }
    }
}

static void atkF0_givecaughtmon(void)
{
    GiveMonToPlayer(&gEnemyParty[gBattlerPartyIndexes[gBattlerAttacker ^ 1]]);
    gBattleResults.caughtPoke = gBattleMons[gBattlerAttacker ^ 1].species;
    GetMonData(&gEnemyParty[gBattlerPartyIndexes[gBattlerAttacker ^ 1]], MON_DATA_NICKNAME, gBattleResults.caughtNick);
    gBattlescriptCurrInstr++;
}

static void atkF1_trysetcaughtmondexflags(void)
{
    if (GetSetPokedexFlag(SpeciesToNationalPokedexNum(gBattleMons[gBattlerTarget].species), 1))
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    else
    {
        GetSetPokedexFlag(SpeciesToNationalPokedexNum(gBattleMons[gBattlerTarget].species), 3);
        if (gBattleMons[gBattlerTarget].species == SPECIES_UNOWN)
            gSaveBlock2.pokedex.unownPersonality = gBattleMons[gBattlerTarget].personality;
        if (gBattleMons[gBattlerTarget].species == SPECIES_SPINDA) //else if
            gSaveBlock2.pokedex.spindaPersonality = gBattleMons[gBattlerTarget].personality;
        gBattlescriptCurrInstr += 5;
    }
}

extern const u32 gBattleTerrainTiles_Building[];
extern const u32 gBattleTerrainTilemap_Building[];
extern const u32 gBattleTerrainPalette_BattleTower[];

static void atkF2_displaydexinfo(void)
{
    switch (gBattleCommunication[0])
    {
    case 0:
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB(0, 0, 0));
        gBattleCommunication[0]++;
        break;
    case 1:
        if (!gPaletteFade.active)
        {
            gBattleCommunication[1] = PrepareDexDisplayForCaughtMon(SpeciesToNationalPokedexNum(gBattleMons[gBattlerTarget].species), gBattleMons[gBattlerTarget].otId, gBattleMons[gBattlerTarget].personality);
            gBattleCommunication[0]++;
        }
        break;
    case 2:
        if (!gPaletteFade.active && gMain.callback2 == BattleMainCB2 && !gTasks[gBattleCommunication[1]].isActive)
        {
            LZDecompressVram(gBattleTerrainTiles_Building, (void*)(VRAM + 0x8000));
            LZDecompressVram(gBattleTerrainTilemap_Building, (void*)(VRAM + 0xD000));
            LoadCompressedPalette(gBattleTerrainPalette_BattleTower, 0x20, 0x60);
            REG_BG3CNT = 0x5a0b;
            gBattle_BG3_X = 0x100;
            BeginNormalPaletteFade(0xFFFC, 0, 16, 0, RGB(0, 0, 0));
            gBattleCommunication[0]++;
        }
        break;
    case 3:
        if (!gPaletteFade.active)
            gBattlescriptCurrInstr++;
        break;
    }
}

NAKED
void sub_802BBD4(u8 r0, u8 r1, u8 r2, u8 r3, u8 sp0)
{
    asm(".syntax unified\n\
    push {r4-r7,lr}\n\
    mov r7, r10\n\
    mov r6, r9\n\
    mov r5, r8\n\
    push {r5-r7}\n\
    ldr r4, [sp, 0x20]\n\
    lsls r0, 24\n\
    lsrs r6, r0, 24\n\
    lsls r1, 24\n\
    lsrs r1, 24\n\
    mov r12, r1\n\
    lsls r2, 24\n\
    lsrs r5, r2, 24\n\
    lsls r3, 24\n\
    lsrs r7, r3, 24\n\
    lsls r4, 24\n\
    lsrs r4, 24\n\
    mov r8, r4\n\
    mov r2, r12\n\
    cmp r2, r7\n\
    bgt _0802BC5A\n\
    lsls r1, r6, 1\n\
    ldr r0, _0802BC20 @ =0x0600c000\n\
    adds r1, r0\n\
    mov r9, r1\n\
_0802BC06:\n\
    adds r1, r6, 0\n\
    adds r0, r2, 0x1\n\
    mov r10, r0\n\
    cmp r1, r5\n\
    bgt _0802BC54\n\
    lsls r0, r2, 6\n\
    mov r4, r9\n\
    adds r3, r4, r0\n\
_0802BC16:\n\
    cmp r2, r12\n\
    bne _0802BC28\n\
    ldr r0, _0802BC24 @ =0x00001022\n\
    b _0802BC36\n\
    .align 2, 0\n\
_0802BC20: .4byte 0x0600c000\n\
_0802BC24: .4byte 0x00001022\n\
_0802BC28:\n\
    cmp r2, r7\n\
    bne _0802BC34\n\
    ldr r0, _0802BC30 @ =0x00001028\n\
    b _0802BC36\n\
    .align 2, 0\n\
_0802BC30: .4byte 0x00001028\n\
_0802BC34:\n\
    ldr r0, _0802BC68 @ =0x00001025\n\
_0802BC36:\n\
    cmp r1, r6\n\
    beq _0802BC42\n\
    adds r0, 0x1\n\
    cmp r1, r5\n\
    bne _0802BC42\n\
    adds r0, 0x1\n\
_0802BC42:\n\
    mov r4, r8\n\
    cmp r4, 0\n\
    beq _0802BC4A\n\
    movs r0, 0\n\
_0802BC4A:\n\
    strh r0, [r3]\n\
    adds r3, 0x2\n\
    adds r1, 0x1\n\
    cmp r1, r5\n\
    ble _0802BC16\n\
_0802BC54:\n\
    mov r2, r10\n\
    cmp r2, r7\n\
    ble _0802BC06\n\
_0802BC5A:\n\
    pop {r3-r5}\n\
    mov r8, r3\n\
    mov r9, r4\n\
    mov r10, r5\n\
    pop {r4-r7}\n\
    pop {r0}\n\
    bx r0\n\
    .align 2, 0\n\
_0802BC68: .4byte 0x00001025\n\
        .syntax divided");
}

void sub_802BC6C(void)
{
    MenuCursor_SetPos814A880(0xC8, ((gBattleCommunication[1] << 28) + 1207959552) >> 24); //what could that be?
}

void nullsub_6(void)
{
    return;
}

static void atkF3_trygivecaughtmonnick(void)
{
    switch (gBattleCommunication[0])
    {
    case 0:
        sub_8023A80();
        gBattleCommunication[0]++;
        gBattleCommunication[1] = 0;
        sub_802BC6C();
        break;
    case 1:
        if (gMain.newKeys & DPAD_UP && gBattleCommunication[1] != 0)
        {
            PlaySE(SE_SELECT);
            nullsub_6();
            gBattleCommunication[1] = 0;
            sub_802BC6C();
        }
        if (gMain.newKeys & DPAD_DOWN && gBattleCommunication[1] == 0)
        {
            PlaySE(SE_SELECT);
            nullsub_6();
            gBattleCommunication[1] = 1;
            sub_802BC6C();
        }
        if (gMain.newKeys & A_BUTTON)
        {
            PlaySE(SE_SELECT);
            if (gBattleCommunication[1] == 0)
            {
                gBattleCommunication[0]++;
                BeginFastPaletteFade(3);
            }
            else
                gBattleCommunication[0] = 4;
        }
        else if (gMain.newKeys & B_BUTTON)
        {
            PlaySE(SE_SELECT);
            gBattleCommunication[0] = 4;
        }
        break;
    case 2:
        if (!gPaletteFade.active)
        {
            GetMonData(&gEnemyParty[gBattlerPartyIndexes[gBattlerAttacker ^ 1]], MON_DATA_NICKNAME, gBattleStruct->caughtNick);
            DoNamingScreen(2, gBattleStruct->caughtNick, GetMonData(&gEnemyParty[gBattlerPartyIndexes[gBattlerAttacker ^ 1]], MON_DATA_SPECIES), GetMonGender(&gEnemyParty[gBattlerPartyIndexes[gBattlerAttacker ^ 1]]), GetMonData(&gEnemyParty[gBattlerPartyIndexes[gBattlerAttacker ^ 1]], MON_DATA_PERSONALITY, 0), BattleMainCB2);
            gBattleCommunication[0]++;
        }
        break;
    case 3:
        if (gMain.callback2 == BattleMainCB2 && !gPaletteFade.active )
        {
            SetMonData(&gEnemyParty[gBattlerPartyIndexes[gBattlerAttacker ^ 1]], MON_DATA_NICKNAME, gBattleStruct->caughtNick);
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
        }
        break;
    case 4:
        if (CalculatePlayerPartyCount() == 6)
            gBattlescriptCurrInstr += 5;
        else
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
        break;
    }
}

static void atkF4_subattackerhpbydmg(void)
{
    gBattleMons[gBattlerAttacker].hp -= gBattleMoveDamage;
    gBattlescriptCurrInstr++;
}

static void atkF5_removeattackerstatus1(void)
{
    gBattleMons[gBattlerAttacker].status1 = 0;
    gBattlescriptCurrInstr++;
}

static void atkF6_finishaction(void)
{
    gCurrentActionFuncId = 0xC;
}

static void atkF7_finishturn(void)
{
    gCurrentActionFuncId = 0xC;
    gCurrentTurnActionNumber = gBattlersCount;
}

static void atkF8_jumpifholdeffect(void)
{
    u8 holdEffect = T2_READ_8(gBattlescriptCurrInstr + 2);
    gActiveBattler = GetBattlerForBattleScript(T2_READ_8(gBattlescriptCurrInstr + 1));

    if (ItemId_GetHoldEffect(gBattleMons[gActiveBattler].item) == holdEffect) {
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 3);
        gLastUsedItem = gBattleMons[gActiveBattler].item;
    }
    else
        gBattlescriptCurrInstr += 7;
}

bool32 DoesLeafGuardProtect(u32 bank)
{
    if (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_SUN_ANY))
        return GetBattlerAbility(bank) == ABILITY_LEAF_GUARD;
    else
        return 0;
}
