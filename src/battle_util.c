#include "global.h"
#include "battle.h"
#include "battle_util.h"
#include "battle_controllers.h"
#include "battle_message.h"
#include "data2.h"
#include "event_data.h"
#include "ewram.h"
#include "field_weather.h"
#include "item.h"
#include "link.h"
#include "pokemon.h"
#include "random.h"
#include "rom_8077ABC.h"
#include "string_util.h"
#include "text.h"
#include "util.h"
#include "constants/abilities.h"
#include "constants/battle_move_effects.h"
#include "constants/flags.h"
#include "constants/hold_effects.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/species.h"
#include "constants/weather.h"

extern u8 gUnknown_02023A14_50;

extern const u8* gBattlescriptCurrInstr;
extern u8 gActiveBattler;
extern u8 gBattleBufferB[4][0x200];
extern u8* gSelectionBattleScripts[4]; //battlescript location when you try to choose a move you're not allowed to
extern u16 gLastMoves[4];
extern struct BattlePokemon gBattleMons[4];
extern struct BattleEnigmaBerry gEnigmaBerries[4];
extern u8 gPotentialItemEffectBattler;
extern u16 gLastUsedItem;
extern u16 gCurrentMove;
extern const u32 gBitTable[];
extern u16 gBattleTypeFlags;
extern u8 gBattlersCount;
extern u32 gStatuses3[4];
extern u8 gBattlerAttacker;
extern u8 gBattlerTarget;
extern u8 gBattlerByTurnOrder[4];
extern u8 gActionsByTurnOrder[4];
extern u8 gCurrentTurnActionNumber;
extern u16 gSideStatuses[2];
extern u16 gBattleWeather;
extern void (*gBattleMainFunc)(void);
extern u8 gAbsentBattlerFlags;
extern u8 gBattleCommunication[];
extern u32 gHitMarker;
extern u8 gEffectBattler;
extern u8 gBank1;
extern s32 gBattleMoveDamage;
extern u16 gBattlerPartyIndexes[4];
extern u16 gChosenMovesByBanks[4];
extern s32 gTakenDmg[4];
extern u8 gTakenDmgByBattler[4];
extern u8 gMoveResultFlags;
extern u8 gLastUsedAbility;
extern u8 gBattleTextBuff2[];
extern u8 gCurrentActionFuncId;
extern struct BattleEnigmaBerry gEnigmaBerries[4];
extern u8 gUnknown_02024BE5;
extern u8 gCurrMovePos;
extern u16 gRandomMove;
extern s32 gBattleMoveDamage;
extern u16 gDynamicBasePower;
extern u32 gBattleControllerExecFlags;
extern u8 gSentPokesToOpponent[2];
extern const u16 gSoundMovesTable[];
extern const u8 gStatusConditionString_PoisonJpn[];
extern const u8 gStatusConditionString_SleepJpn[];
extern const u8 gStatusConditionString_ParalysisJpn[];
extern const u8 gStatusConditionString_BurnJpn[];
extern const u8 gStatusConditionString_IceJpn[];
extern const u8 gStatusConditionString_ConfusionJpn[];
extern const u8 gStatusConditionString_LoveJpn[];
extern const u8 BattleText_Taunt[];
extern const u8 BattleText_Encore[];
extern const u8 BattleText_Torment[];
extern const u8 BattleText_Disable[];
extern const u8 BattleText_Many[];
extern const BattleCmdFunc gBattleScriptingCommandsTable[];

u8 IsImprisoned(u8 bank, u16 move);
u8 GetBattlerAtPosition(u8 ID);
u8 GetBattlerPosition(u8 bank);
u8 GetBattlerSide(u8 bank);
void SetMoveEffect(bool8 primary, u8 certainArg);
bool8 UproarWakeUpCheck(u8 bank);
bool8 sub_8018018(u8 bank, u8, u8);
s32 CalculateBaseDamage(struct BattlePokemon *attacker, struct BattlePokemon *defender, u32 move, u16 a4, u16 powerOverride, u8 typeOverride, u8 bank_atk, u8 bank_def);
u8 CountTrailingZeroBits(u32 a);
u8 GetMoveTarget(u16 move, u8 useMoveTarget);
u8 sub_803FC34(u8 bank);
u16 sub_803FBFC(u8 a);
void RecordAbilityBattle(u8 bank, u8 ability);
void RecordItemEffectBattle(u8 bank, u8 holdEffect);
s8 GetPokeFlavourRelation(u32 pid, u8 flavor);

extern u8 BattleScript_MoveSelectionDisabledMove[];
extern u8 BattleScript_MoveSelectionTormented[];
extern u8 BattleScript_MoveSelectionTaunted[];
extern u8 BattleScript_MoveSelectionImprisoned[];
extern u8 BattleScript_MoveSelectionHealBlock[];
extern u8 BattleScript_MoveSelectionGravity[];
extern u8 BattleScript_MoveSelectionChoiceBanded[];
extern u8 BattleScript_MoveSelectionAssaultVested[];
extern u8 BattleScript_MoveSelectionNoPP[];
extern u8 BattleScript_NoMovesLeft[];
extern u8 BattleScript_WishComesTrue[];
extern u8 BattleScript_IngrainTurnHeal[];
extern u8 BattleScript_AquaRingHeal[];
extern u8 BattleScript_LeechSeedTurnDrain[];
extern u8 BattleScript_PoisonTurnDmg[];
extern u8 BattleScript_PoisonTurnHeal[];
extern u8 BattleScript_BurnTurnDmg[];
extern u8 BattleScript_NightmareTurnDmg[];
extern u8 BattleScript_CurseTurnDmg[];
extern u8 BattleScript_WrapTurnDmg[];
extern u8 BattleScript_WrapEnds[];
extern u8 BattleScript_DisabledNoMore[];
extern u8 BattleScript_EncoredNoMore[];

extern u8 BattleScript_SideStatusWoreOff[];
extern u8 BattleScript_RainContinuesOrEnds[];
extern u8 BattleScript_SandStormHailEnds[];
extern u8 BattleScript_DamagingWeatherContinues[];
extern u8 BattleScript_FogIsDeep[];
extern u8 BattleScript_TrickRoomEnds[];
extern u8 BattleScript_SunlightFaded[];
extern u8 BattleScript_SunlightContinues[];
extern u8 BattleScript_SafeguardEnds[];
extern u8 BattleScript_MonWokeUpInUproar[]; //uproar wakeup BS
extern u8 BattleScript_PrintUproarOverTurns[]; //uproar BS
extern u8 BattleScript_ThrashConfuses[];
extern u8 BattleScript_YawnMakesAsleep[];
extern u8 BattleScript_MonTookFutureAttack[];
extern u8 BattleScript_PerishSongHits[];
extern u8 BattleScript_PerishSongTimerGoesDown[];
extern u8 BattleScript_GiveExp[];
extern u8 BattleScript_HandleFaintedMon[];
extern u8 BattleScript_TailwindPetered[];
extern u8 BattleScript_EmbargoEndTurn[];
extern u8 BattleScript_SlowStartEnds[];
extern u8 BattleScript_GravityEndTurn[];
extern u8 BattleScript_HealBlockEnds[];
extern u8 BattleScript_MagnetRiseEnds[];
extern u8 BattleScript_LuckyChantFades[];

extern u8 BattleScript_MoveUsedIsAsleep[];
extern u8 BattleScript_MoveUsedWokeUp[];
extern u8 BattleScript_MoveUsedIsFrozen[];
extern u8 BattleScript_MoveUsedUnfroze[];
extern u8 BattleScript_MoveUsedLoafingAround[];
extern u8 BattleScript_MoveUsedMustRecharge[];
extern u8 BattleScript_MoveUsedFlinched[];
extern u8 BattleScript_MoveUsedIsDisabled[];
extern u8 BattleScript_MoveUsedIsTaunted[];
extern u8 BattleScript_MoveUsedIsImprisoned[];
extern u8 BattleScript_MoveUsedIsConfused[];
extern u8 BattleScript_MoveUsedIsConfusedNoMore[];
extern u8 BattleScript_MoveUsedIsParalyzed[];
extern u8 BattleScript_MoveUsedIsParalyzedCantAttack[];
extern u8 BattleScript_MoveUsedIsInLove[];
extern u8 BattleScript_BideStoringEnergy[];
extern u8 BattleScript_BideAttack[];
extern u8 BattleScript_BideNoEnergyToAttack[];
extern u8 BattleScript_MoveUsedHealBlockPrevents[];
extern u8 BattleScript_MoveUsedGravityPrevents[];

extern u8 BattleScript_OverworldWeatherStarts[]; //load weather from overworld
extern u8 BattleScript_DrizzleActivates[];
extern u8 BattleScript_SandstreamActivates[];
extern u8 BattleScript_DroughtActivates[];
extern u8 BattleScript_SnowWarningActivates[];
extern u8 BattleScript_CastformChange[];
extern u8 BattleScript_SlowStartStarts[];
extern u8 BattleScript_SlowStartStartsEnd3[];
extern u8 BattleScript_MoveEnd[];
extern u8 BattleScript_RainDishActivates[];
extern u8 BattleScript_ShedSkinActivates[];
extern u8 BattleScript_SpeedBoostActivates[];
extern u8 BattleScript_SoundproofProtected[];
extern u8 BattleScript_MoveHPDrain[];
extern u8 BattleScript_MoveHPDrain_PPLoss[];
extern u8 BattleScript_FlashFireBoost[];
extern u8 BattleScript_FlashFireBoost_PPLoss[];
extern u8 BattleScript_MoveHPDrain_FullHP[];
extern u8 BattleScript_MoveHPDrain_FullHP_PPLoss[];
extern u8 BattleScript_ColorChangeActivates[];
extern u8 BattleScript_RoughSkinActivates[];
extern u8 BattleScript_ApplySecondaryEffect[];
extern u8 BattleScript_CuteCharmActivates[];
extern u8 BattleScript_AbilityCuredStatus[]; //ability status clear
extern u8 BattleScript_SynchronizeActivates[];
extern u8 gUnknown_081D978C[]; //intimidate1
extern u8 gUnknown_081D9795[]; //intimidate2
extern u8 BattleScript_TraceActivates[];

extern u8 BattleScript_WhiteHerbEnd2[];
extern u8 BattleScript_WhiteHerbRet[];
extern u8 BattleScript_ItemHealHP_RemoveItemRet[];
extern u8 BattleScript_ItemHealHP_RemoveItem[];
extern u8 BattleScript_BerryPPHealRet[];
extern u8 BattleScript_BerryPPHealEnd2[];
extern u8 BattleScript_ItemHealHP_End2[];
extern u8 BattleScript_ToxicOrb[];
extern u8 BattleScript_FlameOrb[];
extern u8 BattleScript_ShockOrb[];
extern u8 BattleScript_BerryConfuseHealRet[];
extern u8 BattleScript_BerryConfuseHealEnd2[];
extern u8 BattleScript_BerryStatRaiseRet[];
extern u8 BattleScript_BerryStatRaiseEnd2[];
extern u8 BattleScript_BerryFocusEnergyEnd2[];
extern u8 BattleScript_BerryFocusEnergyRet[];
extern u8 BattleScript_BerryCurePrlzEnd2[];
extern u8 BattleScript_BerryCurePsnEnd2[];
extern u8 BattleScript_BerryCureBrnEnd2[];
extern u8 BattleScript_BerryCureFrzEnd2[];
extern u8 BattleScript_BerryCureSlpEnd2[];
extern u8 BattleScript_BerryCureConfusionEnd2[];
extern u8 BattleScript_BerryCureChosenStatusEnd2[]; //berry cure any status end2
extern u8 BattleScript_BerryCureParRet[];
extern u8 BattleScript_BerryCurePsnRet[];
extern u8 BattleScript_BerryCureBrnRet[];
extern u8 BattleScript_BerryCureFrzRet[];
extern u8 BattleScript_BerryCureSlpRet[];
extern u8 BattleScript_BerryCureConfusionRet[];
extern u8 BattleScript_BerryCureChosenStatusRet[]; //berry cure any status return

extern u8 BattleScript_ItemHealHP_Ret[];
extern u8 BattleScript_BerryWeakenedDamage[];
extern u8 BattleScript_ItemAllowedFirstMove[];
extern u8 BattleScript_BerryAllowedFirstMove[];
extern u8 BattleScript_ItemCausedDamage[];
extern u8 BattleScript_BerryCausedDamage[];
extern u8 BattleScript_AbilityItems[];
extern u8 BattleScript_LifeOrb[];
extern u8 BattleScript_BlackSludge[];
extern u8 BattleScript_ItemStuck[];

extern u8 gUnknown_081D995F[]; //disobedient while asleep
extern u8 BattleScript_IgnoresAndUsesRandomMove[]; //disobedient, uses a random move
extern u8 BattleScript_IgnoresAndFallsAsleep[]; //disobedient, went to sleep
extern u8 gUnknown_081D99A0[]; //disobedient, hits itself

static const u8 sAbilitiesAffectedByMoldBreaker[] =
{
    [ABILITY_BATTLE_ARMOR] = 1,
    [ABILITY_CLEAR_BODY] = 1,
    [ABILITY_DAMP] = 1,
    [ABILITY_DRY_SKIN] = 1,
    [ABILITY_FILTER] = 1,
    [ABILITY_FLASH_FIRE] = 1,
    [ABILITY_FLOWER_GIFT] = 1,
    [ABILITY_HEATPROOF] = 1,
    [ABILITY_HYPER_CUTTER] = 1,
    [ABILITY_IMMUNITY] = 1,
    [ABILITY_INNER_FOCUS] = 1,
    [ABILITY_INSOMNIA] = 1,
    [ABILITY_KEEN_EYE] = 1,
    [ABILITY_LEAF_GUARD] = 1,
    [ABILITY_LEVITATE] = 1,
    [ABILITY_LIGHTNING_ROD] = 1,
    [ABILITY_LIMBER] = 1,
    [ABILITY_MAGMA_ARMOR] = 1,
    [ABILITY_MARVEL_SCALE] = 1,
    [ABILITY_MOTOR_DRIVE] = 1,
    [ABILITY_OBLIVIOUS] = 1,
    [ABILITY_OWN_TEMPO] = 1,
    [ABILITY_SAND_VEIL] = 1,
    [ABILITY_SHELL_ARMOR] = 1,
    [ABILITY_SHIELD_DUST] = 1,
    [ABILITY_SIMPLE] = 1,
    [ABILITY_SNOW_CLOAK] = 1,
    [ABILITY_SOLID_ROCK] = 1,
    [ABILITY_SOUNDPROOF] = 1,
    [ABILITY_STICKY_HOLD] = 1,
    [ABILITY_STORM_DRAIN] = 1,
    [ABILITY_STURDY] = 1,
    [ABILITY_SUCTION_CUPS] = 1,
    [ABILITY_TANGLED_FEET] = 1,
    [ABILITY_THICK_FAT] = 1,
    [ABILITY_UNAWARE] = 1,
    [ABILITY_VITAL_SPIRIT] = 1,
    [ABILITY_VOLT_ABSORB] = 1,
    [ABILITY_WATER_ABSORB] = 1,
    [ABILITY_WATER_VEIL] = 1,
    [ABILITY_WHITE_SMOKE] = 1,
    [ABILITY_WONDER_GUARD] = 1,
    [ABILITY_BIG_PECKS] = 1,
    [ABILITY_CONTRARY] = 1,
    [ABILITY_FRIEND_GUARD] = 1,
    [ABILITY_HEAVY_METAL] = 1,
    [ABILITY_LIGHT_METAL] = 1,
    [ABILITY_MAGIC_BOUNCE] = 1,
    [ABILITY_MULTISCALE] = 1,
    [ABILITY_SAP_SIPPER] = 1,
    [ABILITY_TELEPATHY] = 1,
    [ABILITY_WONDER_SKIN] = 1,
    [ABILITY_AROMA_VEIL] = 1,
    [ABILITY_BULLETPROOF] = 1,
    [ABILITY_FLOWER_VEIL] = 1,
    [ABILITY_FUR_COAT] = 1,
    [ABILITY_OVERCOAT] = 1,
    [ABILITY_SWEET_VEIL] = 1,
    [ABILITY_DAZZLING] = 1,
    [ABILITY_DISGUISE] = 1,
    [ABILITY_FLUFFY] = 1,
    [ABILITY_QUEENLY_MAJESTY] = 1,
    [ABILITY_WATER_BUBBLE] = 1,
};

//array entries for battle communication
#define MOVE_EFFECT_BYTE    0x3
#define MULTISTRING_CHOOSER 0x5
#define MSG_DISPLAY         0x7

u8 GetBattlerForBattleScript(u8 caseId)
{
    u8 ret = 0;
    switch (caseId)
    {
    case BS_TARGET:
        ret = gBattlerTarget;
        break;
    case BS_GET_ATTACKER:
        ret = gBattlerAttacker;
        break;
    case BS_GET_EFFECT_BANK:
        ret = gEffectBattler;
        break;
    case BS_GET_BANK_0:
        ret = 0;
        break;
    case BS_GET_SCRIPTING_BANK:
        ret = ewram16003;
        break;
    case BS_GET_gBank1:
        ret = gBank1;
        break;
    case 5:
        ret = gBank1;
        break;
    }
    return ret;
}

void PressurePPLose(u8 bankDef, u8 bankAtk, u16 move)
{
    s32 i;

    if (GetBattlerAbility(bankDef) != ABILITY_PRESSURE)
        return;

    for (i = 0; i < 4; i++)
    {
        if (gBattleMons[bankAtk].moves[i] == move)
            break;
    }

    if (i == 4) // mons don't share any moves
        return;

    if (gBattleMons[bankAtk].pp[i] != 0)
        gBattleMons[bankAtk].pp[i]--;

    if (!(gBattleMons[bankAtk].status2 & STATUS2_TRANSFORMED)
        && !(gDisableStructs[bankAtk].mimickedMoves & gBitTable[i]))
    {
        gActiveBattler = bankAtk;
        BtlController_EmitSetMonData(0, REQUEST_PPMOVE1_BATTLE + i, 0, 1, &gBattleMons[gActiveBattler].pp[i]);
        MarkBattlerForControllerExec(gActiveBattler);
    }
}

void PressurePPLoseOnUsingImprision(u8 bankAtk)
{
    s32 i, j;
    s32 imprisionPos = 4;
    u8 atkSide = GetBattlerSide(bankAtk);

    for (i = 0; i < gBattlersCount; i++)
    {
        if (atkSide != GetBattlerSide(i) && GetBattlerAbility(i) == ABILITY_PRESSURE)
        {
            for (j = 0; j < 4; j++)
            {
                if (gBattleMons[bankAtk].moves[j] == MOVE_IMPRISON)
                    break;
            }
            if (j != 4)
            {
                imprisionPos = j;
                if (gBattleMons[bankAtk].pp[j] != 0)
                    gBattleMons[bankAtk].pp[j]--;
            }
        }
    }

    if (imprisionPos != 4
        && !(gBattleMons[bankAtk].status2 & STATUS2_TRANSFORMED)
        && !(gDisableStructs[bankAtk].mimickedMoves & gBitTable[imprisionPos]))
    {
        gActiveBattler = bankAtk;
        BtlController_EmitSetMonData(0, REQUEST_PPMOVE1_BATTLE + imprisionPos, 0, 1, &gBattleMons[gActiveBattler].pp[imprisionPos]);
        MarkBattlerForControllerExec(gActiveBattler);
    }
}

void PressurePPLoseOnUsingPerishSong(u8 bankAtk)
{
    s32 i, j;
    s32 perishSongPos = 4;

    for (i = 0; i < gBattlersCount; i++)
    {
        if (GetBattlerAbility(i) == ABILITY_PRESSURE && i != bankAtk)
        {
            for (j = 0; j < 4; j++)
            {
                if (gBattleMons[bankAtk].moves[j] == MOVE_PERISH_SONG)
                    break;
            }
            if (j != 4)
            {
                perishSongPos = j;
                if (gBattleMons[bankAtk].pp[j] != 0)
                    gBattleMons[bankAtk].pp[j]--;
            }
        }
    }

    if (perishSongPos != 4
        && !(gBattleMons[bankAtk].status2 & STATUS2_TRANSFORMED)
        && !(gDisableStructs[bankAtk].mimickedMoves & gBitTable[perishSongPos]))
    {
        gActiveBattler = bankAtk;
        BtlController_EmitSetMonData(0, REQUEST_PPMOVE1_BATTLE + perishSongPos, 0, 1, &gBattleMons[gActiveBattler].pp[perishSongPos]);
        MarkBattlerForControllerExec(gActiveBattler);
    }
}


void MarkAllBattlersForControllerExec(void) // unused
{
    s32 i;

    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
    {
        for (i = 0; i < gBattlersCount; i++)
            gBattleControllerExecFlags |= gBitTable[i] << 0x1C;
    }
    else
    {
        for (i = 0; i < gBattlersCount; i++)
            gBattleControllerExecFlags |= gBitTable[i];
    }
}

void MarkBattlerForControllerExec(u8 bank)
{
    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
        gBattleControllerExecFlags |= gBitTable[bank] << 0x1C;
    else
        gBattleControllerExecFlags |= gBitTable[bank];
}

void sub_80155A4(u8 arg0)
{
    s32 i;

    for (i = 0; i < GetLinkPlayerCount(); i++)
        gBattleControllerExecFlags |= gBitTable[arg0] << (i << 2);

    gBattleControllerExecFlags &= ~(0x10000000 << arg0);
}

void CancelMultiTurnMoves(u8 bank)
{
    gBattleMons[bank].status2 &= ~(STATUS2_MULTIPLETURNS);
    gBattleMons[bank].status2 &= ~(STATUS2_LOCK_CONFUSE);
    gBattleMons[bank].status2 &= ~(STATUS2_UPROAR);
    gBattleMons[bank].status2 &= ~(STATUS2_BIDE);

    gStatuses3[bank] &= ~(STATUS3_SEMI_INVULNERABLE);

    gDisableStructs[bank].rolloutTimer1 = 0;
    gDisableStructs[bank].furyCutterCounter = 0;
}

bool8 WasUnableToUseMove(u8 bank)
{
    if (gProtectStructs[bank].prlzImmobility
        || gProtectStructs[bank].targetNotAffected
        || gProtectStructs[bank].usedImprisonedMove
        || gProtectStructs[bank].loveImmobility
        || gProtectStructs[bank].usedDisabledMove
        || gProtectStructs[bank].usedTauntedMove
        || gProtectStructs[bank].flag2Unknown
        || gProtectStructs[bank].flinchImmobility
        || gProtectStructs[bank].confusionSelfDmg)
        return TRUE;
    else
        return FALSE;
}

void PrepareStringBattle(u16 stringId, u8 bank)
{
    gActiveBattler = bank;
    BtlController_EmitPrintString(0, stringId);
    MarkBattlerForControllerExec(gActiveBattler);
}

void ResetSentPokesToOpponentValue(void)
{
    s32 i;
    u32 bits = 0;

    gSentPokesToOpponent[0] = 0;
    gSentPokesToOpponent[1] = 0;

    for (i = 0; i < gBattlersCount; i += 2)
        bits |= gBitTable[gBattlerPartyIndexes[i]];

    for (i = 1; i < gBattlersCount; i += 2)
        gSentPokesToOpponent[(i & BIT_FLANK) >> 1] = bits;
}

void sub_8015740(u8 bank)
{
    s32 i = 0;
    u32 bits = 0;

    if (GetBattlerSide(bank) == B_SIDE_OPPONENT)
    {
        u8 id = ((bank & BIT_FLANK) >> 1);
        gSentPokesToOpponent[id] = 0;

        for (i = 0; i < gBattlersCount; i += 2)
        {
            if (!(gAbsentBattlerFlags & gBitTable[i]))
                bits |= gBitTable[gBattlerPartyIndexes[i]];
        }

        gSentPokesToOpponent[id] = bits;
    }
}

void sub_80157C4(u8 bank)
{
    if (GetBattlerSide(bank) == B_SIDE_OPPONENT)
    {
        sub_8015740(bank);
    }
    else
    {
        s32 i;
        for (i = 1; i < gBattlersCount; i++)
            gSentPokesToOpponent[(i & BIT_FLANK) >> 1] |= gBitTable[gBattlerPartyIndexes[bank]];
    }
}

void BattleScriptPush(const u8* BS_ptr)
{
    B_BATTLESCRIPTS_STACK->ptr[B_BATTLESCRIPTS_STACK->size++] = BS_ptr;
}

void BattleScriptPushCursor(void)
{
    B_BATTLESCRIPTS_STACK->ptr[B_BATTLESCRIPTS_STACK->size++] = gBattlescriptCurrInstr;
}

void BattleScriptPop(void)
{
    gBattlescriptCurrInstr = B_BATTLESCRIPTS_STACK->ptr[--B_BATTLESCRIPTS_STACK->size];
}

u8 TrySetCantSelectMoveBattleScript(void) //msg can't select a move
{
    u8 limitations = 0;
    u16 move = gBattleMons[gActiveBattler].moves[gBattleBufferB[gActiveBattler][2]];
    u8 holdEffect;
    u16* choicedMove = CHOICED_MOVE(gActiveBattler);

    if (gDisableStructs[gActiveBattler].disabledMove == move && move)
    {
        gBattleStruct->scriptingActive = gActiveBattler;
        gCurrentMove = move;
        gSelectionBattleScripts[gActiveBattler] = BattleScript_MoveSelectionDisabledMove;
        limitations++;
    }
    if (move == gLastMoves[gActiveBattler] && move != MOVE_STRUGGLE && gBattleMons[gActiveBattler].status2 & STATUS2_TORMENT)
    {
        CancelMultiTurnMoves(gActiveBattler);
        gSelectionBattleScripts[gActiveBattler] = BattleScript_MoveSelectionTormented;
        limitations++;
    }
    if (gDisableStructs[gActiveBattler].tauntTimer1 && gBattleMoves[move].power == 0)
    {
        gCurrentMove = move;
        gSelectionBattleScripts[gActiveBattler] = BattleScript_MoveSelectionTaunted;
        limitations++;
    }
    if (IsImprisoned(gActiveBattler, move))
    {
        gCurrentMove = move;
        gSelectionBattleScripts[gActiveBattler] = BattleScript_MoveSelectionImprisoned;
        limitations++;
    }
    if (IsHealBlockPreventingMove(gActiveBattler, move))
    {
        gCurrentMove = move;
        gSelectionBattleScripts[gActiveBattler] = BattleScript_MoveSelectionHealBlock;
        limitations++;
    }
    if (IsGravityPreventingMove(move))
    {
        gCurrentMove = move;
        gSelectionBattleScripts[gActiveBattler] = BattleScript_MoveSelectionGravity;
        limitations++;
    }

    if (gBattleMons[gActiveBattler].item == ITEM_ENIGMA_BERRY)
        holdEffect = gEnigmaBerries[gActiveBattler].holdEffect;
    else
        holdEffect = ItemId_GetHoldEffect(gBattleMons[gActiveBattler].item);

    gPotentialItemEffectBattler = gActiveBattler;

    if (holdEffect == HOLD_EFFECT_CHOICE_ITEM && *choicedMove != 0 && *choicedMove != 0xFFFF && *choicedMove != move)
    {
        gCurrentMove = *choicedMove;
        gLastUsedItem = gBattleMons[gActiveBattler].item;
        gSelectionBattleScripts[gActiveBattler] = BattleScript_MoveSelectionChoiceBanded;
        limitations++;
    }
    if (holdEffect == HOLD_EFFECT_ASSAULT_VEST && gBattleMoves[move].split == MOVE_STATUS)
    {
        gCurrentMove = move;
        gLastUsedItem = gBattleMons[gActiveBattler].item;
        gSelectionBattleScripts[gActiveBattler] = BattleScript_MoveSelectionAssaultVested;
        limitations++;
    }
    if (gBattleMons[gActiveBattler].pp[gBattleBufferB[gActiveBattler][2]] == 0)
    {
        gSelectionBattleScripts[gActiveBattler] = BattleScript_MoveSelectionNoPP;
        limitations++;
    }
    return limitations;
}

u8 CheckMoveLimitations(u8 battlerId, u8 unusableMoves, u8 check)
{
    u8 holdEffect;
    u16* choicedMove = CHOICED_MOVE(battlerId);
    s32 i;

    if (gBattleMons[battlerId].item == ITEM_ENIGMA_BERRY)
        holdEffect = gEnigmaBerries[battlerId].holdEffect;
    else
        holdEffect = ItemId_GetHoldEffect(gBattleMons[battlerId].item);

    gPotentialItemEffectBattler = battlerId;

    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        if (gBattleMons[battlerId].moves[i] == 0 && check & MOVE_LIMITATION_ZEROMOVE)
            unusableMoves |= gBitTable[i];
        if (gBattleMons[battlerId].pp[i] == 0 && check & MOVE_LIMITATION_PP)
            unusableMoves |= gBitTable[i];
        if (gBattleMons[battlerId].moves[i] == gDisableStructs[battlerId].disabledMove && check & MOVE_LIMITATION_DISABLED)
            unusableMoves |= gBitTable[i];
        if (gBattleMons[battlerId].moves[i] == gLastMoves[battlerId] && check & MOVE_LIMITATION_TORMENTED && gBattleMons[battlerId].status2 & STATUS2_TORMENT)
            unusableMoves |= gBitTable[i];
        if (gDisableStructs[battlerId].tauntTimer1 && check & MOVE_LIMITATION_TAUNT && gBattleMoves[gBattleMons[battlerId].moves[i]].power == 0)
            unusableMoves |= gBitTable[i];
        if (IsImprisoned(battlerId, gBattleMons[battlerId].moves[i]) && check & MOVE_LIMITATION_IMPRISON)
            unusableMoves |= gBitTable[i];
        if (gDisableStructs[battlerId].encoreTimer1 && gDisableStructs[battlerId].encoredMove != gBattleMons[battlerId].moves[i])
            unusableMoves |= gBitTable[i];
        if (holdEffect == HOLD_EFFECT_CHOICE_ITEM && *choicedMove != 0 && *choicedMove != 0xFFFF && *choicedMove != gBattleMons[battlerId].moves[i])
            unusableMoves |= gBitTable[i];
    }
    return unusableMoves;
}

bool8 AreAllMovesUnusable(void)
{
    u8 unusable = CheckMoveLimitations(gActiveBattler, 0, 0xFF);

    if (unusable == 0xF) // All moves are unusable.
    {
        gProtectStructs[gActiveBattler].noValidMoves = 1;
        gSelectionBattleScripts[gActiveBattler] = BattleScript_NoMovesLeft;
        if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
            gBattleBufferB[gActiveBattler][3] = GetBattlerAtPosition((GetBattlerPosition(gActiveBattler) ^ 1) | (Random() & 2));
        else
            gBattleBufferB[gActiveBattler][3] = GetBattlerAtPosition(GetBattlerPosition(gActiveBattler) ^ 1);
    }
    else
    {
        gProtectStructs[gActiveBattler].noValidMoves = 0;
    }

    return (unusable == 0xF);
}

u8 GetBattlerAbility(u8 bank)
{
    if (gStatuses3[bank] & STATUS3_GASTRO_ACID)
        return ABILITY_NONE;
    else if (
             (
               (
                   gBattleMons[gBattlerAttacker].ability == ABILITY_MOLD_BREAKER
                || gBattleMons[gBattlerAttacker].ability == ABILITY_TERAVOLT
                || gBattleMons[gBattlerAttacker].ability == ABILITY_TURBOBLAZE
               )
               && !(gStatuses3[gBattlerAttacker] & STATUS3_GASTRO_ACID)
               && sAbilitiesAffectedByMoldBreaker[gBattleMons[bank].ability]
             )
             || 
             (
                gBattleMoves[gCurrentMove].flags & FLAG_ABILITY_IGNORED
             && gBattlerByTurnOrder[gCurrentTurnActionNumber] == gBattlerAttacker
             && gActionsByTurnOrder[gBattlerByTurnOrder[gBattlerAttacker]] == B_ACTION_USE_MOVE
             && gCurrentTurnActionNumber < gBattlersCount
             )
            )
        return ABILITY_NONE;

    return gBattleMons[bank].ability;
}

u8 GetBattleMonMoveSlot(struct BattlePokemon *battleMon, u16 move)
{
    u8 i;

    for (i = 0; i < 4; i++)
    {
        if (battleMon->moves[i] == move)
            break;
    }
    return i;
}

bool32 IsGravityPreventingMove(u16 move)
{
    if (!(gBattleGlobalTimers.gravityTimer))
        return FALSE;

    switch (move)
    {
    case MOVE_BOUNCE:
    case MOVE_FLY:
    case MOVE_FLYING_PRESS:
    case MOVE_HI_JUMP_KICK:
    case MOVE_JUMP_KICK:
    case MOVE_MAGNET_RISE:
    case MOVE_SKY_DROP:
    case MOVE_SPLASH:
    case MOVE_TELEKINESIS:
        return TRUE;
    default:
        return FALSE;
    }
}

bool32 IsHealBlockPreventingMove(u32 bank, u16 move)
{
    if (!(gStatuses3[bank] & STATUS3_HEAL_BLOCK))
        return FALSE;

    switch (gBattleMoves[move].effect)
    {
    case EFFECT_ABSORB:
    case EFFECT_MORNING_SUN:
    case EFFECT_MOONLIGHT:
    case EFFECT_RESTORE_HP:
    case EFFECT_REST:
    case EFFECT_ROOST:
    case EFFECT_HEALING_WISH:
    case EFFECT_WISH:
    case EFFECT_DREAM_EATER:
        return TRUE;
    default:
        return FALSE;
    }
}

bool32 CanUseLastResort(u8 bank)
{
    u32 i;
    u32 knownMovesCount = 0, usedMovesCount = 0;

    for (i = 0; i < 4; i++)
    {
        if (gBattleMons[bank].moves[i] != MOVE_NONE)
            knownMovesCount++;
        if (gDisableStructs[bank].usedMoves & gBitTable[i])
            usedMovesCount++;
    }

    if (knownMovesCount >= 2 && usedMovesCount >= knownMovesCount - 1)
        return TRUE;
    else
        return FALSE;
}

extern const u8 gTypeEffectiveness[];

u8 ReturnTypeEffectiveness(u8 atkType, u8 defType)
{
    int i;

    for (i = 0; gTypeEffectiveness[i] != TYPE_ENDTABLE; i += 3)
    {
        if (gTypeEffectiveness[i] == TYPE_FORESIGHT)
        {
            if (gBattleMons[gBattlerTarget].status2 & STATUS2_FORESIGHT)
                break;
            continue;
        }
        else if ((gTypeEffectiveness[i + 1] == TYPE_FLYING && (gDisableStructs[gBattlerTarget].roost || gBattleGlobalTimers.gravityTimer)) // effectively eliminate flying type from the pool
                 || (gTypeEffectiveness[i] == TYPE_PSYCHIC && gTypeEffectiveness[i + 1] == TYPE_DARK && (gStatuses3[gBattlerTarget] & STATUS3_MIRACLE_EYED))) // moves ineffective against dark may be used
        {
            continue;
        }
        else if (gTypeEffectiveness[i] == atkType && gTypeEffectiveness[i + 1] == defType)
        {
            return gTypeEffectiveness[i + 2];
        }
    }

    return 10;
}

u8 CalcStealthRockDenominator(u8 type1effectiveness, u8 type2effectiveness)
{
    switch (type1effectiveness)
    {
    case 0:
        return 0;
    case 5:
        switch (type2effectiveness)
        {
        case 0:
            return 0;
        case 5:
            return 32;
        case 10:
            return 16;
        case 20:
            return 8;
        }
        break;
    case 10:
        switch (type2effectiveness)
        {
        case 0:
            return 0;
        case 5:
            return 16;
        case 10:
            return 8;
        case 20:
            return 4;
        }
        break;
    case 20:
        switch (type2effectiveness)
        {
        case 0:
            return 0;
        case 5:
            return 8;
        case 10:
            return 4;
        case 20:
            return 2;
        }
        break;
    }
}

u8 IsImprisoned(u8 battlerId, u16 move)
{
    s32 i;
    u8 imprisonedMoves = 0;
    u8 battlerSide = GetBattlerSide(battlerId);

    for (i = 0; i < gBattlersCount; i++)
    {
        if (battlerSide != GetBattlerSide(i) && gStatuses3[i] & STATUS3_IMPRISONED_OTHERS)
        {
            s32 j;
            for (j = 0; j < MAX_MON_MOVES; j++)
            {
                if (move == gBattleMons[i].moves[j])
                    break;
            }
            if (j < MAX_MON_MOVES)
                imprisonedMoves++;
        }
    }

    return imprisonedMoves;
}

enum
{
    ENDTURN_ORDER,
    ENDTURN_REFLECT,
    ENDTURN_LIGHT_SCREEN,
    ENDTURN_MIST,
    ENDTURN_SAFEGUARD,
    ENDTURN_WISH,
    ENDTURN_RAIN,
    ENDTURN_SANDSTORM,
    ENDTURN_SUN,
    ENDTURN_HAIL,
    ENDTURN_FOG,
    ENDTURN_TRICK_ROOM,
    ENDTURN_TAILWIND,
    ENDTURN_EMBARGO,
    ENDTURN_SLOW_START,
    ENDTURN_GRAVITY,
    ENDTURN_MAGNET_RISE,
    ENDTURN_HEAL_BLOCK,
    ENDTURN_LUCKY_CHANT,
    ENDTURN_FIELD_COUNT,
};

u8 DoFieldEndTurnEffects(void)
{
    u8 effect = 0;
    bool8 canTailwindPeter[2];
    s32 i;

    for (gBattlerAttacker = 0; gBattlerAttacker < gBattlersCount && gAbsentBattlerFlags & gBitTable[gBattlerAttacker]; gBattlerAttacker++)
    {
    }
    for (gBattlerTarget = 0; gBattlerTarget < gBattlersCount && gAbsentBattlerFlags & gBitTable[gBattlerTarget]; gBattlerTarget++)
    {
    }

    do
    {
        u8 side;

        switch (gBattleStruct->turnCountersTracker)
        {
        case ENDTURN_ORDER:
            for (i = 0; i < gBattlersCount; i++)
            {
                gBattlerByTurnOrder[i] = i;
            }
            for (i = 0; i < gBattlersCount - 1; i++)
            {
                s32 j;
                for (j = i + 1; j < gBattlersCount; j++)
                {
                    if (GetWhoStrikesFirst(gBattlerByTurnOrder[i], gBattlerByTurnOrder[j], 0))
                        SwapTurnOrder(i, j);
                }
            }
            gBattleStruct->turnCountersTracker++;
            gBattleStruct->turnSideTracker = 0;
            // fall through
        case ENDTURN_REFLECT:
            while (gBattleStruct->turnSideTracker < 2)
            {
                gActiveBattler = gBattlerAttacker = side = gBattleStruct->turnSideTracker;

                if (gSideStatuses[side] & SIDE_STATUS_REFLECT)
                {
                    if (--gSideTimers[side].reflectTimer == 0)
                    {
                        gSideStatuses[side] &= ~SIDE_STATUS_REFLECT;
                        BattleScriptExecute(BattleScript_SideStatusWoreOff);
                        PREPARE_MOVE_BUFFER(gBattleTextBuff1, MOVE_REFLECT);
                        effect++;
                    }
                }
                gBattleStruct->turnSideTracker++;
                if (effect)
                    break;
            }
            if (!effect)
            {
                gBattleStruct->turnCountersTracker++;
                gBattleStruct->turnSideTracker = 0;
            }
            break;
        case ENDTURN_LIGHT_SCREEN:
            while (gBattleStruct->turnSideTracker < 2)
            {
                gActiveBattler = gBattlerAttacker = side = gBattleStruct->turnSideTracker;
                if (gSideStatuses[side] & SIDE_STATUS_LIGHTSCREEN)
                {
                    if (--gSideTimers[side].lightscreenTimer == 0)
                    {
                        gSideStatuses[side] &= ~SIDE_STATUS_LIGHTSCREEN;
                        BattleScriptExecute(BattleScript_SideStatusWoreOff);
                        gBattleCommunication[MULTISTRING_CHOOSER] = side;
                        PREPARE_MOVE_BUFFER(gBattleTextBuff1, MOVE_LIGHT_SCREEN);
                        effect++;
                    }
                }
                gBattleStruct->turnSideTracker++;
                if (effect)
                    break;
            }
            if (!effect)
            {
                gBattleStruct->turnCountersTracker++;
                gBattleStruct->turnSideTracker = 0;
            }
            break;
        case ENDTURN_MIST:
            while (gBattleStruct->turnSideTracker < 2)
            {
                gActiveBattler = gBattlerAttacker = side = gBattleStruct->turnSideTracker;
                if (gSideTimers[side].mistTimer && --gSideTimers[side].mistTimer == 0)
                {
                    gSideStatuses[side] &= ~SIDE_STATUS_MIST;
                    BattleScriptExecute(BattleScript_SideStatusWoreOff);
                    gBattleCommunication[MULTISTRING_CHOOSER] = side;
                    PREPARE_MOVE_BUFFER(gBattleTextBuff1, MOVE_MIST);
                    effect++;
                }
                gBattleStruct->turnSideTracker++;
                if (effect)
                    break;
            }
            if (!effect)
            {
                gBattleStruct->turnCountersTracker++;
                gBattleStruct->turnSideTracker = 0;
            }
            break;
        case ENDTURN_SAFEGUARD:
            while (gBattleStruct->turnSideTracker < 2)
            {
                gActiveBattler = gBattlerAttacker = side = gBattleStruct->turnSideTracker;
                if (gSideStatuses[side] & SIDE_STATUS_SAFEGUARD)
                {
                    if (--gSideTimers[side].safeguardTimer == 0)
                    {
                        gSideStatuses[side] &= ~SIDE_STATUS_SAFEGUARD;
                        BattleScriptExecute(BattleScript_SafeguardEnds);
                        effect++;
                    }
                }
                gBattleStruct->turnSideTracker++;
                if (effect)
                    break;
            }
            if (!effect)
            {
                gBattleStruct->turnCountersTracker++;
                gBattleStruct->turnSideTracker = 0;
            }
            break;
        case ENDTURN_WISH:
            while (gBattleStruct->turnSideTracker < gBattlersCount)
            {
                gActiveBattler = gBattlerByTurnOrder[gBattleStruct->turnSideTracker];
                if (gWishFutureKnock.wishCounter[gActiveBattler] && --gWishFutureKnock.wishCounter[gActiveBattler] == 0 && gBattleMons[gActiveBattler].hp)
                {
                    gBattlerTarget = gActiveBattler;
                    BattleScriptExecute(BattleScript_WishComesTrue);
                    effect++;
                }
                gBattleStruct->turnSideTracker++;
                if (effect)
                    break;
            }
            if (!effect)
            {
                gBattleStruct->turnCountersTracker++;
            }
            break;
        case ENDTURN_RAIN:
            if (gBattleWeather & WEATHER_RAIN_ANY)
            {
                if (!(gBattleWeather & WEATHER_RAIN_PERMANENT))
                {
                    if (--gWishFutureKnock.weatherDuration == 0)
                    {
                        gBattleWeather &= ~WEATHER_RAIN_TEMPORARY;
                        gBattleWeather &= ~WEATHER_RAIN_DOWNPOUR;
                        gBattleCommunication[MULTISTRING_CHOOSER] = 2;
                    }
                    else if (gBattleWeather & WEATHER_RAIN_DOWNPOUR)
                        gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                    else
                        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                }
                else if (gBattleWeather & WEATHER_RAIN_DOWNPOUR)
                {
                    gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                }
                else
                {
                    gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                }

                BattleScriptExecute(BattleScript_RainContinuesOrEnds);
                effect++;
            }
            gBattleStruct->turnCountersTracker++;
            break;
        case ENDTURN_SANDSTORM:
            if (gBattleWeather & WEATHER_SANDSTORM_ANY)
            {
                if (!(gBattleWeather & WEATHER_SANDSTORM_PERMANENT) && --gWishFutureKnock.weatherDuration == 0)
                {
                    gBattleWeather &= ~WEATHER_SANDSTORM_TEMPORARY;
                    gBattlescriptCurrInstr = BattleScript_SandStormHailEnds;
                }
                else
                {
                    gBattlescriptCurrInstr = BattleScript_DamagingWeatherContinues;
                }

                gBattleStruct->animArg1 = B_ANIM_SANDSTORM_CONTINUES;
                gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                BattleScriptExecute(gBattlescriptCurrInstr);
                effect++;
            }
            gBattleStruct->turnCountersTracker++;
            break;
        case ENDTURN_SUN:
            if (gBattleWeather & WEATHER_SUN_ANY)
            {
                if (!(gBattleWeather & WEATHER_SUN_PERMANENT) && --gWishFutureKnock.weatherDuration == 0)
                {
                    gBattleWeather &= ~WEATHER_SUN_TEMPORARY;
                    gBattlescriptCurrInstr = BattleScript_SunlightFaded;
                }
                else
                {
                    gBattlescriptCurrInstr = BattleScript_SunlightContinues;
                }

                BattleScriptExecute(gBattlescriptCurrInstr);
                effect++;
            }
            gBattleStruct->turnCountersTracker++;
            break;
        case ENDTURN_HAIL:
            if (gBattleWeather & WEATHER_HAIL)
            {
                if (!gBattleGlobalTimers.hailPermanent && --gWishFutureKnock.weatherDuration == 0)
                {
                    gBattleWeather &= ~WEATHER_HAIL;
                    gBattlescriptCurrInstr = BattleScript_SandStormHailEnds;
                }
                else
                {
                    gBattlescriptCurrInstr = BattleScript_DamagingWeatherContinues;
                }

                gBattleStruct->animArg1 = B_ANIM_HAIL_CONTINUES;
                gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                BattleScriptExecute(gBattlescriptCurrInstr);
                effect++;
            }
            gBattleStruct->turnCountersTracker++;
            break;
        case ENDTURN_FOG:
            if (gBattleGlobalTimers.fog)
            {
                gBattlescriptCurrInstr = BattleScript_FogIsDeep;
            /*else
                gBattlescriptCurrInstr = BattleScript_FogFaded*/
                BattleScriptExecute(gBattlescriptCurrInstr);
                effect++;
            }
            gBattleStruct->turnCountersTracker++;
            break;
        case ENDTURN_TRICK_ROOM:
            if (gBattleGlobalTimers.trickRoomTimer)
            {
                if (--gBattleGlobalTimers.trickRoomTimer == 0)
                {
                    BattleScriptExecute(BattleScript_TrickRoomEnds);
                    effect++;
                }
            }
            gBattleStruct->turnCountersTracker++;
            break;
        case ENDTURN_TAILWIND:
            for (i = 0; i < 2; i++)
            {
                if (gSideTimers[i].tailwindTimer)
                {
                    gSideTimers[i].tailwindTimer--;
                    canTailwindPeter[i] = TRUE;
                }
                else
                    canTailwindPeter[i] = FALSE;

                if (gSideTimers[i].tailwindTimer == 0 && canTailwindPeter[i])
                {
                    gBattlescriptCurrInstr = BattleScript_TailwindPetered;
                    BattleScriptExecute(gBattlescriptCurrInstr);
                    effect++;
                }
            }
            gBattleStruct->turnCountersTracker++;
            break;
        case ENDTURN_EMBARGO:
            for (i = 0; i < gBattlersCount; i++)
            {
                if (gStatuses3[i] & STATUS3_EMBARGO)
                {
                    if (gDisableStructs[i].embargoTimer == 0 || --gDisableStructs[i].embargoTimer == 0)
                    {
                        gStatuses3[i] &= ~(STATUS3_EMBARGO);
                        gBattlescriptCurrInstr = BattleScript_EmbargoEndTurn;
                        BattleScriptExecute(gBattlescriptCurrInstr);
                        effect++;
                    }
                }
            }
            gBattleStruct->turnCountersTracker++;
            break;
        case ENDTURN_SLOW_START:
            for (i = 0; i < gBattlersCount; i++)
            {
                if (gDisableStructs[i].slowStartTimer && GetBattlerAbility(i) == ABILITY_SLOW_START)
                {
                    if (--gDisableStructs[i].slowStartTimer == 0)
                    {
                        gBattlescriptCurrInstr = BattleScript_SlowStartEnds;
                        BattleScriptExecute(gBattlescriptCurrInstr);
                        effect++;
                    }
                }
            }
            gBattleStruct->turnCountersTracker++;
            break;
        case ENDTURN_GRAVITY:
            if (gBattleGlobalTimers.gravityTimer)
            {
                if (--gBattleGlobalTimers.gravityTimer == 0)
                {
                    gBattlescriptCurrInstr = BattleScript_GravityEndTurn;
                    BattleScriptExecute(gBattlescriptCurrInstr);
                    effect++;
                }
            }
            gBattleStruct->turnCountersTracker++;
            break;
        case ENDTURN_MAGNET_RISE:
            for (i = 0; i < gBattlersCount; i++)
            {
                if (gDisableStructs[i].magnetRiseTimer)
                {
                    if (--gDisableStructs[i].magnetRiseTimer == 0)
                    {
                        gBattleStruct->scriptingActive = i;
                        gBattlescriptCurrInstr = BattleScript_MagnetRiseEnds;
                        BattleScriptExecute(gBattlescriptCurrInstr);
                        effect++;
                    }
                }
            }
            gBattleStruct->turnCountersTracker++;
            break;
        case ENDTURN_HEAL_BLOCK:
            for (i = 0; i < gBattlersCount; i++)
            {
                if (gDisableStructs[i].healBlockTimer)
                {
                    if (--gDisableStructs[i].healBlockTimer == 0)
                    {
                        gStatuses3[i] &= ~(STATUS3_HEAL_BLOCK);
                        gBattleStruct->scriptingActive = i;
                        gBattlescriptCurrInstr = BattleScript_HealBlockEnds;
                        BattleScriptExecute(gBattlescriptCurrInstr);
                        effect++;
                    }
                }
            }
            gBattleStruct->turnCountersTracker++;
            break;
        case ENDTURN_LUCKY_CHANT:
            for (i = 0; i < 2; i++)
            {
                if (gSideTimers[i].luckyChantTimer)
                {
                    if (--gSideTimers[i].luckyChantTimer == 0)
                    {
                        gBattleStruct->scriptingActive = i;
                        gBattlescriptCurrInstr = BattleScript_LuckyChantFades;
                        BattleScriptExecute(gBattlescriptCurrInstr);
                        effect++;
                    }
                }
            }
            gBattleStruct->turnCountersTracker++;
            break;
        case ENDTURN_FIELD_COUNT:
            effect++;
            break;
        }
    } while (effect == 0);
    return (gBattleMainFunc != BattleTurnPassed);
}

#define TURNBASED_MAX_CASE 19

enum
{
    ENDTURN_INGRAIN,
    ENDTURN_AQUA_RING,
    ENDTURN_ABILITIES,
    ENDTURN_ITEMS1,
    ENDTURN_LEECH_SEED,
    ENDTURN_POISON,
    ENDTURN_BAD_POISON,
    ENDTURN_BURN,
    ENDTURN_NIGHTMARES,
    ENDTURN_CURSE,
    ENDTURN_WRAP,
    ENDTURN_UPROAR,
    ENDTURN_THRASH,
    ENDTURN_DISABLE,
    ENDTURN_ENCORE,
    ENDTURN_LOCK_ON,
    ENDTURN_CHARGE,
    ENDTURN_TAUNT,
    ENDTURN_YAWN,
    ENDTURN_ITEMS2,
    ENDTURN_BATTLER_COUNT
};

u8 TurnBasedEffects(void)
{
    u8 effect = 0;

    gHitMarker |= (HITMARKER_GRUDGE | HITMARKER_x20);
    while (gBattleStruct->turnEffectsBattlerId < gBattlersCount && gBattleStruct->turnEffectsTracker <= TURNBASED_MAX_CASE)
    {
        gActiveBattler = gBattlerAttacker = gBattlerByTurnOrder[gBattleStruct->turnEffectsBattlerId];
        if (gAbsentBattlerFlags & gBitTable[gActiveBattler])
        {
            gBattleStruct->turnEffectsBattlerId++;
        }
        else
        {
            switch (gBattleStruct->turnEffectsTracker)
            {
            case ENDTURN_INGRAIN:  // ingrain
                if ((gStatuses3[gActiveBattler] & STATUS3_ROOTED)
                 && gBattleMons[gActiveBattler].hp != gBattleMons[gActiveBattler].maxHP
                 && !(gStatuses3[gActiveBattler] & STATUS3_HEAL_BLOCK)
                 && gBattleMons[gActiveBattler].hp != 0)
                {
                    gBattleMoveDamage = gBattleMons[gActiveBattler].maxHP / 16;
                    if (gBattleMoveDamage == 0)
                        gBattleMoveDamage = 1;
                    gBattleMoveDamage *= -1;
                    BattleScriptExecute(BattleScript_IngrainTurnHeal);
                    effect++;
                }
                gBattleStruct->turnEffectsTracker++;
                break;
            case ENDTURN_AQUA_RING:  // aqua ring
                if ((gStatuses3[gActiveBattler] & STATUS3_AQUA_RING)
                && (gBattleMons[gActiveBattler].hp != gBattleMons[gActiveBattler].maxHP)
                && !(gStatuses3[gActiveBattler] & STATUS3_HEAL_BLOCK)
                && gBattleMons[gActiveBattler].hp != 0)
                {
                    gBattleMoveDamage = gBattleMons[gActiveBattler].maxHP / 16;
                    if (gBattleMoveDamage == 0)
                        gBattleMoveDamage = 1;
                    gBattleMoveDamage *= -1;
                    BattleScriptExecute(BattleScript_AquaRingHeal);
                    effect++;
                }
                gBattleStruct->turnEffectsTracker++;
                break;
            case ENDTURN_ABILITIES:  // end turn abilities
                if (AbilityBattleEffects(ABILITYEFFECT_ENDTURN, gActiveBattler, 0, 0, 0))
                    effect++;
                gBattleStruct->turnEffectsTracker++;
                break;
            case ENDTURN_ITEMS1:  // item effects
                if (ItemBattleEffects(1, gActiveBattler, FALSE))
                    effect++;
                gBattleStruct->turnEffectsTracker++;
                break;
            case ENDTURN_ITEMS2:  // item effects again
                if (ItemBattleEffects(1, gActiveBattler, TRUE))
                    effect++;
                gBattleStruct->turnEffectsTracker++;
                break;
            case ENDTURN_LEECH_SEED:  // leech seed
                if ((gStatuses3[gActiveBattler] & STATUS3_LEECHSEED)
                 && gBattleMons[gStatuses3[gActiveBattler] & STATUS3_LEECHSEED_BATTLER].hp != 0
                 && !(gStatuses3[gActiveBattler] & STATUS3_HEAL_BLOCK)
                 && gBattleMons[gActiveBattler].hp != 0)
                {
                    gBattlerTarget = gStatuses3[gActiveBattler] & STATUS3_LEECHSEED_BATTLER; // Notice gBattlerTarget is actually the HP receiver.
                    gBattleMoveDamage = gBattleMons[gActiveBattler].maxHP / 8;
                    if (gBattleMoveDamage == 0)
                        gBattleMoveDamage = 1;
                    gBattleStruct->animArg1 = gBattlerTarget;
                    gBattleStruct->animArg2 = gBattlerAttacker;
                    BattleScriptExecute(BattleScript_LeechSeedTurnDrain);
                    effect++;
                }
                gBattleStruct->turnEffectsTracker++;
                break;
            case ENDTURN_POISON:  // poison
                if ((gBattleMons[gActiveBattler].status1 & STATUS1_POISON) && gBattleMons[gActiveBattler].hp != 0)
                {
                    gBattleMoveDamage = gBattleMons[gActiveBattler].maxHP / 8;
                    if (gBattleMoveDamage == 0) {
                        gBattleMoveDamage = 1;
                    }
                    if (GetBattlerAbility(gActiveBattler) == ABILITY_POISON_HEAL) {
                        gBattleMoveDamage = 0;
                    } else {
                        BattleScriptExecute(BattleScript_PoisonTurnDmg);
                        effect++;
                    }
                }
                gBattleStruct->turnEffectsTracker++;
                break;
            case ENDTURN_BAD_POISON:  // toxic poison
                if ((gBattleMons[gActiveBattler].status1 & STATUS1_TOXIC_POISON) && gBattleMons[gActiveBattler].hp != 0)
                {
                    gBattleMoveDamage = gBattleMons[gActiveBattler].maxHP / 16;
                    if (gBattleMoveDamage == 0)
                        gBattleMoveDamage = 1;
                    if (GetBattlerAbility(gActiveBattler) == ABILITY_POISON_HEAL) { // essentially do nothing
                        gBattleMoveDamage = 0;
                    } else { // only if the ability isn't poison heal
                        if ((gBattleMons[gActiveBattler].status1 & 0xF00) != 0xF00) //not 16 turns
                            gBattleMons[gActiveBattler].status1 += 0x100;
                        gBattleMoveDamage *= (gBattleMons[gActiveBattler].status1 & 0xF00) >> 8;
                        BattleScriptExecute(BattleScript_PoisonTurnDmg);
                        effect++;
                    }
                }
                gBattleStruct->turnEffectsTracker++;
                break;
            case ENDTURN_BURN:  // burn
                if ((gBattleMons[gActiveBattler].status1 & STATUS1_BURN) && gBattleMons[gActiveBattler].hp != 0)
                {
                    gBattleMoveDamage = gBattleMons[gActiveBattler].maxHP / 8;
                    if (gBattleMoveDamage == 0)
                        gBattleMoveDamage = 1;
                    BattleScriptExecute(BattleScript_BurnTurnDmg);
                    effect++;
                }
                gBattleStruct->turnEffectsTracker++;
                break;
            case ENDTURN_NIGHTMARES:  // spooky nightmares
                if ((gBattleMons[gActiveBattler].status2 & STATUS2_NIGHTMARE) && gBattleMons[gActiveBattler].hp != 0)
                {
                    // missing sleep check
                    gBattleMoveDamage = gBattleMons[gActiveBattler].maxHP / 4;
                    if (gBattleMoveDamage == 0)
                        gBattleMoveDamage = 1;
                    BattleScriptExecute(BattleScript_NightmareTurnDmg);
                    effect++;
                }
                gBattleStruct->turnEffectsTracker++;
                break;
            case ENDTURN_CURSE:  // curse
                if ((gBattleMons[gActiveBattler].status2 & STATUS2_CURSED) && gBattleMons[gActiveBattler].hp != 0)
                {
                    gBattleMoveDamage = gBattleMons[gActiveBattler].maxHP / 4;
                    if (gBattleMoveDamage == 0)
                        gBattleMoveDamage = 1;
                    BattleScriptExecute(BattleScript_CurseTurnDmg);
                    effect++;
                }
                gBattleStruct->turnEffectsTracker++;
                break;
            case ENDTURN_WRAP:  // wrap
                if ((gBattleMons[gActiveBattler].status2 & STATUS2_WRAPPED) && gBattleMons[gActiveBattler].hp != 0)
                {
                    gBattleMons[gActiveBattler].status2 -= 0x2000;
                    if (gBattleMons[gActiveBattler].status2 & STATUS2_WRAPPED)  // damaged by wrap
                    {
                        gBattleStruct->animArg1 = ewram16004arr(0, gActiveBattler);
                        gBattleStruct->animArg2 = ewram16004arr(1, gActiveBattler);
                        gBattleTextBuff1[0] = B_BUFF_PLACEHOLDER_BEGIN;
                        gBattleTextBuff1[1] = B_BUFF_MOVE;
                        gBattleTextBuff1[2] = ewram16004arr(0, gActiveBattler);
                        gBattleTextBuff1[3] = ewram16004arr(1, gActiveBattler);
                        gBattleTextBuff1[4] = EOS;
                        gBattlescriptCurrInstr = BattleScript_WrapTurnDmg;
                        gBattleMoveDamage = gBattleMons[gActiveBattler].maxHP / 16;
                        if (gBattleMoveDamage == 0)
                            gBattleMoveDamage = 1;
                    }
                    else  // broke free
                    {
                        gBattleTextBuff1[0] = 0xFD;
                        gBattleTextBuff1[1] = 2;
                        gBattleTextBuff1[2] = ewram16004arr(0, gActiveBattler);
                        gBattleTextBuff1[3] = ewram16004arr(1, gActiveBattler);
                        gBattleTextBuff1[4] = EOS;
                        gBattlescriptCurrInstr = BattleScript_WrapEnds;
                    }
                    BattleScriptExecute(gBattlescriptCurrInstr);
                    effect++;
                }
                gBattleStruct->turnEffectsTracker++;
                break;
            case ENDTURN_UPROAR:  // uproar
                if (gBattleMons[gActiveBattler].status2 & STATUS2_UPROAR)
                {
                    for (gBattlerAttacker = 0; gBattlerAttacker < gBattlersCount; gBattlerAttacker++)
                    {
                        if ((gBattleMons[gBattlerAttacker].status1 & STATUS1_SLEEP)
                         && GetBattlerAbility(gBattlerAttacker) != ABILITY_SOUNDPROOF)
                        {
                            gBattleMons[gBattlerAttacker].status1 &= ~(STATUS1_SLEEP);
                            gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_NIGHTMARE);
                            gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                            BattleScriptExecute(BattleScript_MonWokeUpInUproar);
                            gActiveBattler = gBattlerAttacker;
                            BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
                            MarkBattlerForControllerExec(gActiveBattler);
                            break;
                        }
                    }
                    if (gBattlerAttacker != gBattlersCount)
                    {
                        effect = 2;  // a pokemon was awaken
                        break;
                    }
                    else
                    {
                        gBattlerAttacker = gActiveBattler;
                        gBattleMons[gActiveBattler].status2 -= 0x10;  // uproar timer goes down
                        if (WasUnableToUseMove(gActiveBattler))
                        {
                            CancelMultiTurnMoves(gActiveBattler);
                            gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                        }
                        else if (gBattleMons[gActiveBattler].status2 & STATUS2_UPROAR)
                        {
                            gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                            gBattleMons[gActiveBattler].status2 |= STATUS2_MULTIPLETURNS;
                        }
                        else
                        {
                            gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                            CancelMultiTurnMoves(gActiveBattler);
                        }
                        BattleScriptExecute(BattleScript_PrintUproarOverTurns);
                        effect = 1;
                    }
                }
                if (effect != 2)
                    gBattleStruct->turnEffectsTracker++;
                break;
            case ENDTURN_THRASH:  // thrash
                if (gBattleMons[gActiveBattler].status2 & STATUS2_LOCK_CONFUSE)
                {
                    gBattleMons[gActiveBattler].status2 -= 0x400;
                    if (WasUnableToUseMove(gActiveBattler))
                        CancelMultiTurnMoves(gActiveBattler);
                    else if (!(gBattleMons[gActiveBattler].status2 & STATUS2_LOCK_CONFUSE)
                     && (gBattleMons[gActiveBattler].status2 & STATUS2_MULTIPLETURNS))
                    {
                        gBattleMons[gActiveBattler].status2 &= ~(STATUS2_MULTIPLETURNS);
                        if (!(gBattleMons[gActiveBattler].status2 & STATUS2_CONFUSION))
                        {
                            gBattleCommunication[MOVE_EFFECT_BYTE] = 0x47;
                            SetMoveEffect(1, 0);
                            if (gBattleMons[gActiveBattler].status2 & STATUS2_CONFUSION)
                                BattleScriptExecute(BattleScript_ThrashConfuses);
                            effect++;
                        }
                    }
                }
                gBattleStruct->turnEffectsTracker++;
                break;
            case ENDTURN_DISABLE:  // disable
                if (gDisableStructs[gActiveBattler].disableTimer1 != 0)
                {
                    s32 i;
                    for (i = 0; i < MAX_MON_MOVES; i++)
                    {
                        if (gDisableStructs[gActiveBattler].disabledMove == gBattleMons[gActiveBattler].moves[i])
                            break;
                    }
                    if (i == 4)  // pokemon does not have the disabled move anymore
                    {
                        gDisableStructs[gActiveBattler].disabledMove = 0;
                        gDisableStructs[gActiveBattler].disableTimer1 = 0;
                    }
                    else if (--gDisableStructs[gActiveBattler].disableTimer1 == 0)  // disable ends
                    {
                        gDisableStructs[gActiveBattler].disabledMove = 0;
                        BattleScriptExecute(BattleScript_DisabledNoMore);
                        effect++;
                    }
                }
                gBattleStruct->turnEffectsTracker++;
                break;
            case ENDTURN_ENCORE:  // encore
                if (gDisableStructs[gActiveBattler].encoreTimer1 != 0)
                {
                    if (gBattleMons[gActiveBattler].moves[gDisableStructs[gActiveBattler].encoredMovePos] != gDisableStructs[gActiveBattler].encoredMove)  // pokemon does not have the encored move anymore
                    {
                        gDisableStructs[gActiveBattler].encoredMove = 0;
                        gDisableStructs[gActiveBattler].encoreTimer1 = 0;
                    }
                    else if (--gDisableStructs[gActiveBattler].encoreTimer1 == 0
                     || gBattleMons[gActiveBattler].pp[gDisableStructs[gActiveBattler].encoredMovePos] == 0)
                    {
                        gDisableStructs[gActiveBattler].encoredMove = 0;
                        gDisableStructs[gActiveBattler].encoreTimer1 = 0;
                        BattleScriptExecute(BattleScript_EncoredNoMore);
                        effect++;
                    }
                }
                gBattleStruct->turnEffectsTracker++;
                break;
            case ENDTURN_LOCK_ON:  // lock-on decrement
                if (gStatuses3[gActiveBattler] & STATUS3_ALWAYS_HITS)
                    gStatuses3[gActiveBattler] -= 0x8;
                gBattleStruct->turnEffectsTracker++;
                break;
            case ENDTURN_CHARGE:  // charge
                if (gDisableStructs[gActiveBattler].chargeTimer1 && --gDisableStructs[gActiveBattler].chargeTimer1 == 0)
                    gStatuses3[gActiveBattler] &= ~STATUS3_CHARGED_UP;
                gBattleStruct->turnEffectsTracker++;
                break;
            case ENDTURN_TAUNT:  // taunt
                if (gDisableStructs[gActiveBattler].tauntTimer1)
                    gDisableStructs[gActiveBattler].tauntTimer1--;
                gBattleStruct->turnEffectsTracker++;
                break;
            case ENDTURN_YAWN:  // yawn
                if (gStatuses3[gActiveBattler] & STATUS3_YAWN)
                {
                    gStatuses3[gActiveBattler] -= 0x800;
                    if (!(gStatuses3[gActiveBattler] & STATUS3_YAWN) && !(gBattleMons[gActiveBattler].status1 & STATUS1_ANY)
                     && GetBattlerAbility(gActiveBattler) != ABILITY_VITAL_SPIRIT
                     && GetBattlerAbility(gActiveBattler) != ABILITY_INSOMNIA && !UproarWakeUpCheck(gActiveBattler))
                    {
                        CancelMultiTurnMoves(gActiveBattler);
                        gBattleMons[gActiveBattler].status1 |= (Random() & 3) + 2;
                        BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
                        MarkBattlerForControllerExec(gActiveBattler);
                        gEffectBattler = gActiveBattler;
                        BattleScriptExecute(BattleScript_YawnMakesAsleep);
                        effect++;
                    }
                }
                gBattleStruct->turnEffectsTracker++;
                break;
            case ENDTURN_BATTLER_COUNT:  // done
                gBattleStruct->turnEffectsTracker = 0;
                gBattleStruct->turnEffectsBattlerId++;
                break;
            }
            if (effect != 0)
                return effect;
        }
    }
    gHitMarker &= ~(HITMARKER_GRUDGE | HITMARKER_x20);
    return 0;
}

bool8 HandleWishPerishSongOnTurnEnd(void)
{
    gHitMarker |= (HITMARKER_GRUDGE | HITMARKER_x20);

    switch (gBattleStruct->wishPerishSongState)
    {
    case 0: // future sight
        while (gBattleStruct->wishPerishSongBattlerId < gBattlersCount)
        {
            gActiveBattler = gBattleStruct->wishPerishSongBattlerId;
            if (gAbsentBattlerFlags & gBitTable[gActiveBattler])
            {
                gBattleStruct->wishPerishSongBattlerId++;
            }
            else
            {
                gBattleStruct->wishPerishSongBattlerId++;
                if (gWishFutureKnock.futureSightCounter[gActiveBattler] && --gWishFutureKnock.futureSightCounter[gActiveBattler] == 0 && gBattleMons[gActiveBattler].hp)
                {
                    if (gWishFutureKnock.futureSightMove[gActiveBattler] == MOVE_FUTURE_SIGHT)
                        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                    else //Doom Desire
                        gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                    gBattleTextBuff1[0] = 0xFD;
                    gBattleTextBuff1[1] = 2;
                    gBattleTextBuff1[2] = gWishFutureKnock.futureSightMove[gActiveBattler];
                    gBattleTextBuff1[3] = gWishFutureKnock.futureSightMove[gActiveBattler] >> 8;
                    gBattleTextBuff1[4] = 0xFF;
                    gBattlerTarget = gActiveBattler;
                    gBattlerAttacker = gWishFutureKnock.futureSightAttacker[gActiveBattler];
                    gBattleMoveDamage = gWishFutureKnock.futureSightDmg[gActiveBattler];
                    gSpecialStatuses[gBattlerTarget].dmg = 0xFFFF;
                    BattleScriptExecute(BattleScript_MonTookFutureAttack);
                    return 1;
                }
            }
        }
        gBattleStruct->wishPerishSongState = 1;
        gBattleStruct->wishPerishSongBattlerId = 0;
    case 1: // perish song
        while (gBattleStruct->wishPerishSongBattlerId < gBattlersCount)
        {
            gActiveBattler = gBattlerAttacker = gBattlerByTurnOrder[gBattleStruct->wishPerishSongBattlerId];
            if (gAbsentBattlerFlags & gBitTable[gActiveBattler])
                gBattleStruct->wishPerishSongBattlerId++;
            else
            {
                gBattleStruct->wishPerishSongBattlerId++;
                if (gStatuses3[gActiveBattler] & STATUS3_PERISH_SONG)
                {
                    gBattleTextBuff1[0] = 0xFD;
                    gBattleTextBuff1[1] = 1;
                    gBattleTextBuff1[2] = 1;
                    gBattleTextBuff1[3] = 1;
                    gBattleTextBuff1[4] = gDisableStructs[gActiveBattler].perishSongTimer1;
                    gBattleTextBuff1[5] = 0xFF;
                    if (gDisableStructs[gActiveBattler].perishSongTimer1 == 0)
                    {
                        gStatuses3[gActiveBattler] &= ~(STATUS3_PERISH_SONG);
                        gBattleMoveDamage = gBattleMons[gActiveBattler].hp;
                        gBattlescriptCurrInstr = BattleScript_PerishSongHits;
                    }
                    else
                    {
                        gDisableStructs[gActiveBattler].perishSongTimer1--;
                        gBattlescriptCurrInstr = BattleScript_PerishSongTimerGoesDown;
                    }
                    BattleScriptExecute(gBattlescriptCurrInstr);
                    return 1;
                }
            }
        }
        break;
    }
    gHitMarker &= ~(HITMARKER_GRUDGE | HITMARKER_x20);
    return 0;
}

#define HandleFaintedMonActions_MAX_CASE 7

bool8 HandleFaintedMonActions(void)
{
    if (gBattleTypeFlags & BATTLE_TYPE_SAFARI)
        return 0;
    do
    {
        int i;
        switch (gBattleStruct->sub80173A4_Tracker)
        {
        case 0:
            gBattleStruct->unk1605A = 0;
            gBattleStruct->sub80173A4_Tracker++;
            for (i = 0; i < gBattlersCount; i++)
            {
                if (gAbsentBattlerFlags & gBitTable[i] && !sub_8018018(i, 6, 6))
                    gAbsentBattlerFlags &= ~(gBitTable[i]);
            }
        case 1:
            do
            {
                gBank1 = gBattlerTarget = gBattleStruct->unk1605A;
                if (gBattleMons[gBattleStruct->unk1605A].hp == 0 && !(gBattleStruct->unk16113 & gBitTable[gBattlerPartyIndexes[gBattleStruct->unk1605A]]) && !(gAbsentBattlerFlags & gBitTable[gBattleStruct->unk1605A]))
                {
                    BattleScriptExecute(BattleScript_GiveExp);
                    gBattleStruct->sub80173A4_Tracker = 2;
                    return 1;
                }
            } while (++gBattleStruct->unk1605A != gBattlersCount);
            gBattleStruct->sub80173A4_Tracker = 3;
            break;
        case 2:
            sub_8015740(gBank1);
            if (++gBattleStruct->unk1605A == gBattlersCount)
                gBattleStruct->sub80173A4_Tracker = 3;
            else
                gBattleStruct->sub80173A4_Tracker = 1;
            break;
        case 3:
            gBattleStruct->unk1605A = 0;
            gBattleStruct->sub80173A4_Tracker++;
        case 4:
            do
            {
                gBank1 = gBattlerTarget = gBattleStruct->unk1605A; //or should banks be switched?
                if (gBattleMons[gBattleStruct->unk1605A].hp == 0 && !(gAbsentBattlerFlags & gBitTable[gBattleStruct->unk1605A]))
                {
                    BattleScriptExecute(BattleScript_HandleFaintedMon);
                    gBattleStruct->sub80173A4_Tracker = 5;
                    return 1;
                }
            } while (++gBattleStruct->unk1605A != gBattlersCount);
            gBattleStruct->sub80173A4_Tracker = 6;
            break;
        case 5:
            if (++gBattleStruct->unk1605A == gBattlersCount)
                gBattleStruct->sub80173A4_Tracker = 6;
            else
                gBattleStruct->sub80173A4_Tracker = 4;
            break;
        case 6:
            if (AbilityBattleEffects(9, 0, 0, 0, 0) || AbilityBattleEffects(0xB, 0, 0, 0, 0) || ItemBattleEffects(1, 0, 1) || AbilityBattleEffects(6, 0, 0, 0, 0))
                return 1;
            gBattleStruct->sub80173A4_Tracker++;
            break;
        case 7:
            break;
        }
    } while (gBattleStruct->sub80173A4_Tracker != HandleFaintedMonActions_MAX_CASE);
    return 0;
}

void TryClearRageStatuses(void)
{
    int i;
    for (i = 0; i < gBattlersCount; i++)
    {
        if (gBattleMons[i].status2 & STATUS2_RAGE && gChosenMovesByBanks[i] != MOVE_RAGE)
            gBattleMons[i].status2 &= ~(STATUS2_RAGE);
    }
}

enum
{
    ATKCANCELLER_FLAG_CLEAR,
    ATKCANCELLER_SLEEP,
    ATKCANCELLER_FROZEN,
    ATKCANCELLER_TRUANT,
    ATKCANCELLER_RECHARGE,
    ATKCANCELLER_FLINCH,
    ATKCANCELLER_DISABLED,
    ATKCANCELLER_TAUNT,
    ATKCANCELLER_IMPRISON,
    ATKCANCELLER_HEAL_BLOCK,
    ATKCANCELLER_GRAVITY,
    ATKCANCELLER_CONFUSION,
    ATKCANCELLER_PARALYSIS,
    ATKCANCELLER_INFATUATION,
    ATKCANCELLER_BIDE,
    ATKCANCELLER_THAWING,
    ATKCANCELLER_MAX_CASE,
};

u8 AtkCanceller_UnableToUseMove(void)
{
    u8 effect = 0;
    s32* bideDmg = &gBattleStruct->bideDmg;
    do
    {
        switch (gBattleStruct->atkCancellerTracker)
        {
        case ATKCANCELLER_FLAG_CLEAR: // flags clear
            gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_DESTINY_BOND);
            gStatuses3[gBattlerAttacker] &= ~(STATUS3_GRUDGE);
            gBattleStruct->atkCancellerTracker++;
            break;
        case ATKCANCELLER_SLEEP: // check being asleep
            if (gBattleMons[gBattlerAttacker].status1 & STATUS1_SLEEP)
            {
                if (UproarWakeUpCheck(gBattlerAttacker))
                {
                    gBattleMons[gBattlerAttacker].status1 &= ~(STATUS1_SLEEP);
                    gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_NIGHTMARE);
                    BattleScriptPushCursor();
                    gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                    gBattlescriptCurrInstr = BattleScript_MoveUsedWokeUp;
                    effect = 2;
                }
                else
                {
                    u8 toSub;
                    if (GetBattlerAbility(gBattlerAttacker) == ABILITY_EARLY_BIRD)
                        toSub = 2;
                    else
                        toSub = 1;
                    if ((gBattleMons[gBattlerAttacker].status1 & STATUS1_SLEEP) < toSub)
                        gBattleMons[gBattlerAttacker].status1 &= ~(STATUS1_SLEEP);
                    else
                        gBattleMons[gBattlerAttacker].status1 -= toSub;
                    if (gBattleMons[gBattlerAttacker].status1 & STATUS1_SLEEP)
                    {
                        if (gCurrentMove != MOVE_SNORE && gCurrentMove != MOVE_SLEEP_TALK)
                        {
                            gBattlescriptCurrInstr = BattleScript_MoveUsedIsAsleep;
                            gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
                            effect = 2;
                        }
                    }
                    else
                    {
                        gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_NIGHTMARE);
                        BattleScriptPushCursor();
                        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                        gBattlescriptCurrInstr = BattleScript_MoveUsedWokeUp;
                        effect = 2;
                    }
                }
            }
            gBattleStruct->atkCancellerTracker++;
            break;
        case ATKCANCELLER_FROZEN: // check being frozen
            if (gBattleMons[gBattlerAttacker].status1 & STATUS1_FREEZE)
            {
                if (Random() % 5)
                {
                    if (gBattleMoves[gCurrentMove].effect != EFFECT_THAW_HIT) // unfreezing via a move effect happens in case 13
                    {
                        gBattlescriptCurrInstr = BattleScript_MoveUsedIsFrozen;
                        gHitMarker |= HITMARKER_NO_ATTACKSTRING;
                    }
                    else
                    {
                        gBattleStruct->atkCancellerTracker++;
                        break;
                    }
                }
                else // unfreeze
                {
                    gBattleMons[gBattlerAttacker].status1 &= ~(STATUS1_FREEZE);
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_MoveUsedUnfroze;
                    gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                }
                effect = 2;
            }
            gBattleStruct->atkCancellerTracker++;
            break;
        case ATKCANCELLER_TRUANT: // truant
            if (GetBattlerAbility(gBattlerAttacker) == ABILITY_TRUANT && gDisableStructs[gBattlerAttacker].truantCounter)
            {
                CancelMultiTurnMoves(gBattlerAttacker);
                gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
                gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                gBattlescriptCurrInstr = BattleScript_MoveUsedLoafingAround;
                gMoveResultFlags |= MOVE_RESULT_MISSED;
                effect = 1;
            }
            gBattleStruct->atkCancellerTracker++;
            break;
        case ATKCANCELLER_RECHARGE: // recharge
            if (gBattleMons[gBattlerAttacker].status2 & STATUS2_RECHARGE)
            {
                gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_RECHARGE);
                gDisableStructs[gBattlerAttacker].rechargeCounter = 0;
                CancelMultiTurnMoves(gBattlerAttacker);
                gBattlescriptCurrInstr = BattleScript_MoveUsedMustRecharge;
                gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
                effect = 1;
            }
            gBattleStruct->atkCancellerTracker++;
            break;
        case ATKCANCELLER_FLINCH: // flinch
            if (gBattleMons[gBattlerAttacker].status2 & STATUS2_FLINCHED)
            {
                gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_FLINCHED);
                gProtectStructs[gBattlerAttacker].flinchImmobility = 1;
                CancelMultiTurnMoves(gBattlerAttacker);
                gBattlescriptCurrInstr = BattleScript_MoveUsedFlinched;
                gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
                effect = 1;
            }
            gBattleStruct->atkCancellerTracker++;
            break;
        case ATKCANCELLER_DISABLED: // disabled move
            if (gDisableStructs[gBattlerAttacker].disabledMove == gCurrentMove && gDisableStructs[gBattlerAttacker].disabledMove != 0)
            {
                gProtectStructs[gBattlerAttacker].usedDisabledMove = 1;
                gBattleStruct->scriptingActive = gBattlerAttacker;
                CancelMultiTurnMoves(gBattlerAttacker);
                gBattlescriptCurrInstr = BattleScript_MoveUsedIsDisabled;
                gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
                effect = 1;
            }
            gBattleStruct->atkCancellerTracker++;
            break;
        case ATKCANCELLER_TAUNT: // taunt
            if (gDisableStructs[gBattlerAttacker].tauntTimer1 && gBattleMoves[gCurrentMove].power == 0)
            {
                gProtectStructs[gBattlerAttacker].usedTauntedMove = 1;
                CancelMultiTurnMoves(gBattlerAttacker);
                gBattlescriptCurrInstr = BattleScript_MoveUsedIsTaunted;
                gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
                effect = 1;
            }
            gBattleStruct->atkCancellerTracker++;
            break;
        case ATKCANCELLER_IMPRISON: // imprisoned
            if (IsImprisoned(gBattlerAttacker, gCurrentMove))
            {
                gProtectStructs[gBattlerAttacker].usedImprisonedMove = 1;
                CancelMultiTurnMoves(gBattlerAttacker);
                gBattlescriptCurrInstr = BattleScript_MoveUsedIsImprisoned;
                gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
                effect = 1;
            }
            gBattleStruct->atkCancellerTracker++;
            break;
        case ATKCANCELLER_HEAL_BLOCK:
            if (gStatuses3[gBattlerAttacker] & STATUS3_HEAL_BLOCK && IsHealBlockPreventingMove(gBattlerAttacker, gCurrentMove)) 
            {
                gProtectStructs[gBattlerAttacker].usedDisabledMove = 1;
                gBattleStruct->scriptingActive = gBattlerAttacker;
                CancelMultiTurnMoves(gBattlerAttacker);
                gBattlescriptCurrInstr = BattleScript_MoveUsedHealBlockPrevents;
                gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
                effect = 1;
            }
            gBattleStruct->atkCancellerTracker++;
            break;
        case ATKCANCELLER_GRAVITY:
            if (gBattleGlobalTimers.gravityTimer && IsGravityPreventingMove(gCurrentMove))
            {
                gProtectStructs[gBattlerAttacker].usedDisabledMove = 1;
                gBattleStruct->scriptingActive = gBattlerAttacker;
                CancelMultiTurnMoves(gBattlerAttacker);
                gBattlescriptCurrInstr = BattleScript_MoveUsedGravityPrevents;
                gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
                effect = 1;
            }
            gBattleStruct->atkCancellerTracker++;
            break;
        case ATKCANCELLER_CONFUSION: // confusion
            if (gBattleMons[gBattlerAttacker].status2 & STATUS2_CONFUSION)
            {
                gBattleMons[gBattlerAttacker].status2--;
                if (gBattleMons[gBattlerAttacker].status2 & STATUS2_CONFUSION)
                {
                    if (Random() & 1)
                    {
                        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                        BattleScriptPushCursor();
                    }
                    else // confusion dmg
                    {
                        gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                        gBattlerTarget = gBattlerAttacker;
                        gBattleMoveDamage = CalculateBaseDamage(&gBattleMons[gBattlerAttacker], &gBattleMons[gBattlerAttacker], MOVE_POUND, 0, 40, 0, gBattlerAttacker, gBattlerAttacker);
                        gProtectStructs[gBattlerAttacker].confusionSelfDmg = 1;
                        gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
                    }
                    gBattlescriptCurrInstr = BattleScript_MoveUsedIsConfused;
                }
                else // snapped out of confusion
                {
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_MoveUsedIsConfusedNoMore;
                }
                effect = 1;
            }
            gBattleStruct->atkCancellerTracker++;
            break;
        case ATKCANCELLER_PARALYSIS: // paralysis
            if (gBattleMons[gBattlerAttacker].status1 & STATUS1_PARALYSIS && (Random() % 4) == 0)
            {
                gProtectStructs[gBattlerAttacker].prlzImmobility = 1;
                CancelMultiTurnMoves(gBattlerAttacker);
                gBattlescriptCurrInstr = BattleScript_MoveUsedIsParalyzed;
                gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
                effect = 1;
            }
            gBattleStruct->atkCancellerTracker++;
            break;
        case ATKCANCELLER_INFATUATION: // infatuation
            if (gBattleMons[gBattlerAttacker].status2 & STATUS2_INFATUATION)
            {
                gBattleStruct->scriptingActive = CountTrailingZeroBits((gBattleMons[gBattlerAttacker].status2 & STATUS2_INFATUATION) >> 0x10);
                if (Random() & 1)
                    BattleScriptPushCursor();
                else
                {
                    BattleScriptPush(BattleScript_MoveUsedIsParalyzedCantAttack);
                    gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
                    gProtectStructs[gBattlerAttacker].loveImmobility = 1;
                    CancelMultiTurnMoves(gBattlerAttacker);
                }
                gBattlescriptCurrInstr = BattleScript_MoveUsedIsInLove;
                effect = 1;
            }
            gBattleStruct->atkCancellerTracker++;
            break;
        case ATKCANCELLER_BIDE: // bide
            if (gBattleMons[gBattlerAttacker].status2 & STATUS2_BIDE)
            {
                gBattleMons[gBattlerAttacker].status2 -= 0x100;
                if (gBattleMons[gBattlerAttacker].status2 & STATUS2_BIDE)
                    gBattlescriptCurrInstr = BattleScript_BideStoringEnergy;
                else
                {
                    gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_MULTIPLETURNS);
                    if (gTakenDmg[gBattlerAttacker])
                    {
                        gCurrentMove = MOVE_BIDE;
                        *bideDmg = gTakenDmg[gBattlerAttacker] * 2;
                        gBattlerTarget = gTakenDmgByBattler[gBattlerAttacker];
                        if (gAbsentBattlerFlags & gBitTable[gBattlerTarget])
                            gBattlerTarget = GetMoveTarget(MOVE_BIDE, 1);
                        gBattlescriptCurrInstr = BattleScript_BideAttack;
                    }
                    else
                        gBattlescriptCurrInstr = BattleScript_BideNoEnergyToAttack;
                }
                effect = 1;
            }
            gBattleStruct->atkCancellerTracker++;
            break;
        case ATKCANCELLER_THAWING: // move thawing
            if (gBattleMons[gBattlerAttacker].status1 & STATUS1_FREEZE)
            {
                if (gBattleMoves[gCurrentMove].effect == EFFECT_THAW_HIT)
                {
                    gBattleMons[gBattlerAttacker].status1 &= ~(STATUS1_FREEZE);
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_MoveUsedUnfroze;
                    gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                }
                effect = 2;
            }
            gBattleStruct->atkCancellerTracker++;
            break;
        case ATKCANCELLER_MAX_CASE: // last case
            break;
        }

    } while (gBattleStruct->atkCancellerTracker != ATKCANCELLER_MAX_CASE && effect == 0);

    if (effect == 2)
    {
        gActiveBattler = gBattlerAttacker;
        BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
        MarkBattlerForControllerExec(gActiveBattler);
    }
    return effect;
}

bool8 sub_8018018(u8 bank, u8 r1, u8 r2)
{
    struct Pokemon* party;
    u8 r7;
    u8 r6;
    s32 i;
    if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
        return 0;
    if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
    {
        r7 = sub_803FC34(bank);
        if (GetBattlerSide(bank) == 0)
            party = gPlayerParty;
        else
            party = gEnemyParty;
        r6 = sub_803FBFC(r7);
        for (i = r6 * 3; i < r6 * 3 + 3; i++)
        {
            if (GetMonData(&party[i], MON_DATA_HP) && GetMonData(&party[i], MON_DATA_SPECIES2) && GetMonData(&party[i], MON_DATA_SPECIES2) != SPECIES_EGG)
                break;
        }
        return (i == r6 * 3 + 3);
    }
    else
    {
        if (GetBattlerSide(bank) == 1)
        {
            r7 = GetBattlerAtPosition(1);
            r6 = GetBattlerAtPosition(3);
            party = gEnemyParty;
        }
        else
        {
            r7 = GetBattlerAtPosition(0);
            r6 = GetBattlerAtPosition(2);
            party = gPlayerParty;
        }
        if (r1 == 6)
            r1 = gBattlerPartyIndexes[r7];
        if (r2 == 6)
            r2 = gBattlerPartyIndexes[r6];
        for (i = 0; i < 6; i++)
        {
            if (GetMonData(&party[i], MON_DATA_HP) && GetMonData(&party[i], MON_DATA_SPECIES2) && GetMonData(&party[i], MON_DATA_SPECIES2) != SPECIES_EGG && i != r1 && i != r2 && i != ewram16068arr(r7) && i != ewram16068arr(r6))
                break;
        }
        return (i == 6);
    }
}

enum
{
    CASTFORM_NO_CHANGE, //0
    CASTFORM_TO_NORMAL, //1
    CASTFORM_TO_FIRE,   //2
    CASTFORM_TO_WATER,  //3
    CASTFORM_TO_ICE,    //4
};

u8 CastformDataTypeChange(u8 bank)
{
    u8 formChange = 0;
    if (gBattleMons[bank].species != SPECIES_CASTFORM || GetBattlerAbility(bank) != ABILITY_FORECAST || gBattleMons[bank].hp == 0)
        return CASTFORM_NO_CHANGE;
    if (!WEATHER_HAS_EFFECT && gBattleMons[bank].type1 != TYPE_NORMAL && gBattleMons[bank].type2 != TYPE_NORMAL)
    {
        gBattleMons[bank].type1 = TYPE_NORMAL;
        gBattleMons[bank].type2 = TYPE_NORMAL;
        return CASTFORM_TO_NORMAL;
    }
    if (!WEATHER_HAS_EFFECT)
        return CASTFORM_NO_CHANGE;
    if ((!(gBattleWeather & (WEATHER_RAIN_ANY | WEATHER_SUN_ANY | WEATHER_HAIL)) || gBattleGlobalTimers.fog) && gBattleMons[bank].type1 != TYPE_NORMAL && gBattleMons[bank].type2 != TYPE_NORMAL)
    {
        gBattleMons[bank].type1 = TYPE_NORMAL;
        gBattleMons[bank].type2 = TYPE_NORMAL;
        formChange = CASTFORM_TO_NORMAL;
    }
    if (gBattleWeather & WEATHER_SUN_ANY && gBattleMons[bank].type1 != TYPE_FIRE && gBattleMons[bank].type2 != TYPE_FIRE)
    {
        gBattleMons[bank].type1 = TYPE_FIRE;
        gBattleMons[bank].type2 = TYPE_FIRE;
        formChange = CASTFORM_TO_FIRE;
    }
    if (gBattleWeather & WEATHER_RAIN_ANY && gBattleMons[bank].type1 != TYPE_WATER && gBattleMons[bank].type2 != TYPE_WATER)
    {
        gBattleMons[bank].type1 = TYPE_WATER;
        gBattleMons[bank].type2 = TYPE_WATER;
        formChange = CASTFORM_TO_WATER;
    }
    if (gBattleWeather & WEATHER_HAIL && gBattleMons[bank].type1 != TYPE_ICE && gBattleMons[bank].type2 != TYPE_ICE)
    {
        gBattleMons[bank].type1 = TYPE_ICE;
        gBattleMons[bank].type2 = TYPE_ICE;
        formChange = CASTFORM_TO_ICE;
    }
    return formChange;
}

u8 AbilityBattleEffects(u8 caseID, u8 bank, u8 ability, u8 special, u16 moveArg)
{
    u8 effect = 0;
    struct Pokemon* pokeAtk;
    struct Pokemon* pokeDef;
    u16 speciesAtk;
    u16 speciesDef;
    u32 pidAtk;
    u32 pidDef;

    if (gBattlerAttacker >= gBattlersCount)
        gBattlerAttacker = bank;
    if (GetBattlerSide(gBattlerAttacker) == 0)
        pokeAtk = &gPlayerParty[gBattlerPartyIndexes[gBattlerAttacker]];
    else
        pokeAtk = &gEnemyParty[gBattlerPartyIndexes[gBattlerAttacker]];

    if (gBattlerTarget >= gBattlersCount)
        gBattlerTarget = bank;
    if (GetBattlerSide(gBattlerTarget) == 0)
        pokeDef = &gPlayerParty[gBattlerPartyIndexes[gBattlerTarget]];
    else
        pokeDef = &gEnemyParty[gBattlerPartyIndexes[gBattlerTarget]];

    speciesAtk = GetMonData(pokeAtk, MON_DATA_SPECIES);
    pidAtk = GetMonData(pokeAtk, MON_DATA_PERSONALITY);

    speciesDef = GetMonData(pokeDef, MON_DATA_SPECIES);
    pidDef = GetMonData(pokeDef, MON_DATA_PERSONALITY);

    if (!(gBattleTypeFlags & BATTLE_TYPE_SAFARI)) // why isn't that check done at the beginning?
    {
        u8 moveType;
        s32 i;
        u16 move;
        u8 side;
        u8 target1;
        u8 itemEffect;

        if (special)
            gLastUsedAbility = special;
        else
            gLastUsedAbility = GetBattlerAbility(bank);

        if (moveArg)
            move = moveArg;
        else
            move = gCurrentMove;

        if (gBattleStruct->dynamicMoveType)
            moveType = gBattleStruct->dynamicMoveType & 0x3F;
        else
            moveType = gBattleMoves[move].type;

        switch (caseID)
        {
        case ABILITYEFFECT_ON_SWITCHIN: // 0
            if (gBattlerAttacker >= gBattlersCount)
                gBattlerAttacker = bank;
            switch (gLastUsedAbility)
            {
            case 0xFF: //weather from overworld
                switch (GetCurrentWeather())
                {
                case WEATHER_RAIN_LIGHT:
                case WEATHER_RAIN_MED:
                case WEATHER_RAIN_HEAVY:
                    if (!(gBattleWeather & WEATHER_RAIN_ANY))
                    {
                        gBattleWeather = (WEATHER_RAIN_TEMPORARY | WEATHER_RAIN_PERMANENT);
                        gBattleStruct->animArg1 = B_ANIM_RAIN_CONTINUES;
                        gBattleStruct->scriptingActive = bank;
                        effect++;
                    }
                    break;
                case WEATHER_SANDSTORM:
                    if (!(gBattleWeather & WEATHER_SANDSTORM_ANY))
                    {
                        gBattleWeather = (WEATHER_SANDSTORM_PERMANENT | WEATHER_SANDSTORM_TEMPORARY);
                        gBattleStruct->animArg1 = B_ANIM_SANDSTORM_CONTINUES;
                        gBattleStruct->scriptingActive = bank;
                        effect++;
                    }
                    break;
                case WEATHER_DROUGHT:
                    if (!(gBattleWeather & WEATHER_SUN_ANY))
                    {
                        gBattleWeather = (WEATHER_SUN_PERMANENT | WEATHER_SUN_TEMPORARY);
                        gBattleStruct->animArg1 = B_ANIM_SUN_CONTINUES;
                        gBattleStruct->scriptingActive = bank;
                        effect++;
                    }
                    break;
                case WEATHER_SNOW:
                    if (!(gBattleWeather & WEATHER_HAIL))
                    {
                        gBattleWeather = WEATHER_HAIL;
                        gBattleGlobalTimers.hailPermanent = TRUE;
                        gBattleStruct->animArg1 = B_ANIM_HAIL_CONTINUES;
                        gBattleStruct->scriptingActive = bank;
                        effect++;
                    }
                    break;
                case WEATHER_FOG_3:
                    if (!gBattleGlobalTimers.fog)
                    {
                        gBattleGlobalTimers.fog = TRUE;
                        gBattleStruct->animArg1 = B_ANIM_FOG_IS_DEEP;
                        gBattleStruct->scriptingActive = bank;
                        effect++;
                    }
                    break;
                }
                if (effect)
                {
                    gBattleCommunication[MULTISTRING_CHOOSER] = GetCurrentWeather();
                    BattleScriptPushCursorAndCallback(BattleScript_OverworldWeatherStarts);
                }
                break;
            case ABILITY_DRIZZLE:
                if (!(gBattleWeather & WEATHER_RAIN_PERMANENT))
                {
                    gBattleWeather = (WEATHER_RAIN_PERMANENT | WEATHER_RAIN_TEMPORARY);
                    BattleScriptPushCursorAndCallback(BattleScript_DrizzleActivates);
                    gBattleStruct->scriptingActive = bank;
                    effect++;
                }
                break;
            case ABILITY_SAND_STREAM:
                if (!(gBattleWeather & WEATHER_SANDSTORM_PERMANENT))
                {
                    gBattleWeather = (WEATHER_SANDSTORM_PERMANENT | WEATHER_SANDSTORM_TEMPORARY);
                    BattleScriptPushCursorAndCallback(BattleScript_SandstreamActivates);
                    gBattleStruct->scriptingActive = bank;
                    effect++;
                }
                break;
            case ABILITY_DROUGHT:
                if (!(gBattleWeather & WEATHER_SUN_PERMANENT))
                {
                    gBattleWeather = (WEATHER_SUN_PERMANENT | WEATHER_SUN_TEMPORARY);
                    BattleScriptPushCursorAndCallback(BattleScript_DroughtActivates);
                    gBattleStruct->scriptingActive = bank;
                    effect++;
                }
                break;
            case ABILITY_SNOW_WARNING:
                if (!(gBattleWeather & WEATHER_HAIL))
                {
                    gBattleWeather = WEATHER_HAIL;
                    gBattleGlobalTimers.hailPermanent = TRUE;
                    BattleScriptPushCursorAndCallback(BattleScript_SnowWarningActivates);
                    gBattleStruct->scriptingActive = bank;
                    effect++;
                }
                break;
            case ABILITY_INTIMIDATE:
                if (!(gSpecialStatuses[bank].intimidatedMon))
                {
                    gStatuses3[bank] |= STATUS3_INTIMIDATE_POKES;
                    gSpecialStatuses[bank].intimidatedMon = 1;
                    gDisableStructs[bank].intimidate = TRUE; // used specifically for adrenaline orb
                    itemEffect++;
                }
                break;
            case ABILITY_FORECAST:
                effect = CastformDataTypeChange(bank);
                if (effect != 0)
                {
                    BattleScriptPushCursorAndCallback(BattleScript_CastformChange);
                    gBattleStruct->scriptingActive = bank;
                    gBattleStruct->castformToChangeInto = effect - 1;
                }
                break;
            case ABILITY_TRACE:
                if (!(gSpecialStatuses[bank].traced))
                {
                    gStatuses3[bank] |= STATUS3_TRACE;
                    gSpecialStatuses[bank].traced = 1;
                }
                break;
            case ABILITY_SLOW_START:
                gDisableStructs[bank].slowStartTimer = 5;
                if (gCurrentActionFuncId == B_ACTION_EXEC_SCRIPT) // if switching in, this handling is lowkey fucked but hey
                {
                    BattleScriptPush(BattleScript_MoveEnd);
                    BattleScriptExecute(BattleScript_SlowStartStarts);
                }
                else // if sending out at the beginning
                    BattleScriptPushCursorAndCallback(BattleScript_SlowStartStartsEnd3);
                gBattleStruct->scriptingActive = bank;
                effect++;
                break;
            case ABILITY_CLOUD_NINE:
            case ABILITY_AIR_LOCK:
                {
                    // that's a weird choice for a variable, why not use i or bank?
                    for (target1 = 0; target1 < gBattlersCount; target1++)
                    {
                        effect = CastformDataTypeChange(target1);
                        if (effect != 0)
                        {
                            BattleScriptPushCursorAndCallback(BattleScript_CastformChange);
                            gBattleStruct->scriptingActive = target1;
                            gBattleStruct->castformToChangeInto = effect - 1;
                            break;
                        }
                    }
                }
                break;
            }
            if (itemEffect)
                ItemBattleEffects(2, 0, FALSE);
            break;
        case ABILITYEFFECT_ENDTURN: // 1
            if (gBattleMons[bank].hp != 0)
            {
                gBattlerAttacker = bank;
                switch (gLastUsedAbility)
                {
                case ABILITY_RAIN_DISH:
                    if (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_RAIN_ANY)
                     && gBattleMons[bank].maxHP > gBattleMons[bank].hp)
                    {
                        gLastUsedAbility = ABILITY_RAIN_DISH; // why
                        BattleScriptPushCursorAndCallback(BattleScript_RainDishActivates);
                        gBattleMoveDamage = gBattleMons[bank].maxHP / 8;
                        if (gBattleMoveDamage == 0)
                            gBattleMoveDamage = 1;
                        gBattleMoveDamage *= -1;
                        effect++;
                    }
                    break;
                case ABILITY_SHED_SKIN:
                    if ((gBattleMons[bank].status1 & STATUS1_ANY) && (Random() % 3) == 0)
                    {
                        if (gBattleMons[bank].status1 & (STATUS1_POISON | STATUS1_TOXIC_POISON))
                            StringCopy(gBattleTextBuff1, gStatusConditionString_PoisonJpn);
                        if (gBattleMons[bank].status1 & STATUS1_SLEEP)
                            StringCopy(gBattleTextBuff1, gStatusConditionString_SleepJpn);
                        if (gBattleMons[bank].status1 & STATUS1_PARALYSIS)
                            StringCopy(gBattleTextBuff1, gStatusConditionString_ParalysisJpn);
                        if (gBattleMons[bank].status1 & STATUS1_BURN)
                            StringCopy(gBattleTextBuff1, gStatusConditionString_BurnJpn);
                        if (gBattleMons[bank].status1 & STATUS1_FREEZE)
                            StringCopy(gBattleTextBuff1, gStatusConditionString_IceJpn);
                        gBattleMons[bank].status1 = 0;
                        gBattleMons[bank].status2 &= ~(STATUS2_NIGHTMARE);
                        gBattleStruct->scriptingActive = gActiveBattler = bank;
                        BattleScriptPushCursorAndCallback(BattleScript_ShedSkinActivates);
                        BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[bank].status1);
                        MarkBattlerForControllerExec(gActiveBattler);
                        effect++;
                    }
                    break;
                case ABILITY_SPEED_BOOST:
                    if (gBattleMons[bank].statStages[STAT_STAGE_SPEED] < 0xC && gDisableStructs[bank].isFirstTurn != 2)
                    {
                        gBattleMons[bank].statStages[STAT_STAGE_SPEED]++;
                        gBattleStruct->animArg1 = 0x11;
                        gBattleStruct->animArg2 = 0;
                        BattleScriptPushCursorAndCallback(BattleScript_SpeedBoostActivates);
                        gBattleStruct->scriptingActive = bank;
                        effect++;
                    }
                    break;
                case ABILITY_TRUANT:
                    gDisableStructs[gBattlerAttacker].truantCounter ^= 1;
                    break;
				case ABILITY_POISON_HEAL:
                    if (GetBattlerAbility(bank) == ABILITY_POISON_HEAL
					 && ((gBattleMons[bank].status1 & STATUS1_POISON) || (gBattleMons[bank].status1 & STATUS1_TOXIC_POISON))
                     && gStatuses3[bank] & STATUS3_HEAL_BLOCK
                     && gBattleMons[bank].maxHP > gBattleMons[bank].hp)
                    {
                        BattleScriptPushCursorAndCallback(BattleScript_RainDishActivates);
                        gBattleMoveDamage = gBattleMons[bank].maxHP / 8;
                        if (gBattleMoveDamage == 0)
                            gBattleMoveDamage = 1;
                        gBattleMoveDamage *= -1;
                        effect++;
                    }
                }
            }
            break;
        case ABILITYEFFECT_MOVES_BLOCK: // 2
            if (gLastUsedAbility == ABILITY_SOUNDPROOF)
            {
                for (i = 0; gSoundMovesTable[i] != 0xFFFF; i++)
                {
                    if (gSoundMovesTable[i] == move)
                        break;
                }
                if (gSoundMovesTable[i] != 0xFFFF)
                {
                    if (gBattleMons[gBattlerAttacker].status2 & STATUS2_MULTIPLETURNS)
                        gHitMarker |= HITMARKER_NO_PPDEDUCT;
                    gBattlescriptCurrInstr = BattleScript_SoundproofProtected;
                    effect = 1;
                }
            }
            break;
        case ABILITYEFFECT_ABSORBING: // 3
            if (move)
            {
                switch (gLastUsedAbility)
                {
                case ABILITY_VOLT_ABSORB:
                    if (moveType == TYPE_ELECTRIC && gBattleMoves[move].power != 0)
                    {
                        if (gProtectStructs[gBattlerAttacker].notFirstStrike)
                            gBattlescriptCurrInstr = BattleScript_MoveHPDrain;
                        else
                            gBattlescriptCurrInstr = BattleScript_MoveHPDrain_PPLoss;
                        effect = 1;
                    }
                    break;
                case ABILITY_WATER_ABSORB:
                    if (moveType == TYPE_WATER && gBattleMoves[move].power != 0)
                    {
                        if (gProtectStructs[gBattlerAttacker].notFirstStrike)
                            gBattlescriptCurrInstr = BattleScript_MoveHPDrain;
                        else
                            gBattlescriptCurrInstr = BattleScript_MoveHPDrain_PPLoss;
                        effect = 1;
                    }
                    break;
                case ABILITY_FLASH_FIRE:
                    if (moveType == TYPE_FIRE && !(gBattleMons[bank].status1 & STATUS1_FREEZE))
                    {
                        if (!(eFlashFireArr.arr[bank] & 1))
                        {
                            gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                            if (gProtectStructs[gBattlerAttacker].notFirstStrike)
                                gBattlescriptCurrInstr = BattleScript_FlashFireBoost;
                            else
                                gBattlescriptCurrInstr = BattleScript_FlashFireBoost_PPLoss;
                            eFlashFireArr.arr[bank] |= 1;
                            effect = 2;
                        }
                        else
                        {
                            gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                            if (gProtectStructs[gBattlerAttacker].notFirstStrike)
                                gBattlescriptCurrInstr = BattleScript_FlashFireBoost;
                            else
                                gBattlescriptCurrInstr = BattleScript_FlashFireBoost_PPLoss;
                            effect = 2;
                        }
                    }
                    break;
                }
                if (effect == 1)
                {
                    if (gBattleMons[bank].maxHP == gBattleMons[bank].hp)
                    {
                        if ((gProtectStructs[gBattlerAttacker].notFirstStrike))
                            gBattlescriptCurrInstr = BattleScript_MoveHPDrain_FullHP;
                        else
                            gBattlescriptCurrInstr = BattleScript_MoveHPDrain_FullHP_PPLoss;
                    }
                    else
                    {
                        gBattleMoveDamage = gBattleMons[bank].maxHP / 4;
                        if (gBattleMoveDamage == 0)
                            gBattleMoveDamage = 1;
                        gBattleMoveDamage *= -1;
                    }
                }
            }
            break;
        case ABILITYEFFECT_CONTACT: // 4
            switch (gLastUsedAbility)
            {
            case ABILITY_COLOR_CHANGE:
                if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
                 && move != MOVE_STRUGGLE
                 && gBattleMoves[move].power != 0
                 && (gSpecialStatuses[gBattlerTarget].physicalDmg || gSpecialStatuses[gBattlerTarget].specialDmg)
                 && gBattleMons[bank].type1 != moveType
                 && gBattleMons[bank].type2 != moveType
                 && gBattleMons[bank].hp != 0)
                {
                    gBattleMons[bank].type1 = moveType;
                    gBattleMons[bank].type2 = moveType;
                    gBattleTextBuff1[0] = 0xFD;
                    gBattleTextBuff1[1] = 3;
                    gBattleTextBuff1[2] = moveType;
                    gBattleTextBuff1[3] = 0xFF;
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_ColorChangeActivates;
                    effect++;
                }
                break;
            case ABILITY_ROUGH_SKIN:
            case ABILITY_IRON_BARBS:
                if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
                 && gBattleMons[gBattlerAttacker].hp != 0
                 && !gProtectStructs[gBattlerAttacker].confusionSelfDmg
                 && (gSpecialStatuses[gBattlerTarget].physicalDmg || gSpecialStatuses[gBattlerTarget].specialDmg)
                 && (gBattleMoves[move].flags & F_MAKES_CONTACT))
                {
                    gBattleMoveDamage = gBattleMons[gBattlerAttacker].maxHP / 8;
                    if (gBattleMoveDamage == 0)
                        gBattleMoveDamage = 1;
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_RoughSkinActivates;
                    effect++;
                }
                break;
            case ABILITY_EFFECT_SPORE:
        if (DEBUG && (gUnknown_02023A14_50 & 4))
        {
            if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
            && gBattleMons[gBattlerAttacker].hp != 0
            && !gProtectStructs[gBattlerAttacker].confusionSelfDmg
            && (gSpecialStatuses[gBattlerTarget].physicalDmg || gSpecialStatuses[gBattlerTarget].specialDmg)
            && (gBattleMoves[move].flags & F_MAKES_CONTACT))
            {
            do
            {
                gBattleCommunication[MOVE_EFFECT_BYTE] = Random() & 3;
            } while (gBattleCommunication[MOVE_EFFECT_BYTE] == 0);
            if (gBattleCommunication[MOVE_EFFECT_BYTE] == 3)
                gBattleCommunication[MOVE_EFFECT_BYTE] += 2;
            gBattleCommunication[MOVE_EFFECT_BYTE] += 0x40;
            BattleScriptPushCursor();
            gBattlescriptCurrInstr = BattleScript_ApplySecondaryEffect;
            gHitMarker |= HITMARKER_IGNORE_SAFEGUARD;
            effect++;
            }
        }
        else
        {
            if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
            && gBattleMons[gBattlerAttacker].hp != 0
            && !gProtectStructs[gBattlerAttacker].confusionSelfDmg
            && (gSpecialStatuses[gBattlerTarget].physicalDmg || gSpecialStatuses[gBattlerTarget].specialDmg)
            && (gBattleMoves[move].flags & F_MAKES_CONTACT)
            && (Random() % 10) == 0)
            {
            do
            {
                gBattleCommunication[MOVE_EFFECT_BYTE] = Random() & 3;
            } while (gBattleCommunication[MOVE_EFFECT_BYTE] == 0);
            if (gBattleCommunication[MOVE_EFFECT_BYTE] == 3)
                gBattleCommunication[MOVE_EFFECT_BYTE] += 2;
            gBattleCommunication[MOVE_EFFECT_BYTE] += 0x40;
            BattleScriptPushCursor();
            gBattlescriptCurrInstr = BattleScript_ApplySecondaryEffect;
            gHitMarker |= HITMARKER_IGNORE_SAFEGUARD;
            effect++;
            }
        }
                break;
            case ABILITY_POISON_POINT:
        if (DEBUG && (gUnknown_02023A14_50 & 4))
        {
            if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
             && gBattleMons[gBattlerAttacker].hp != 0
             && !gProtectStructs[gBattlerAttacker].confusionSelfDmg
             && (gSpecialStatuses[gBattlerTarget].physicalDmg || gSpecialStatuses[gBattlerTarget].specialDmg)
             && (gBattleMoves[move].flags & F_MAKES_CONTACT))
            {
            gBattleCommunication[MOVE_EFFECT_BYTE] = 0x42;
            BattleScriptPushCursor();
            gBattlescriptCurrInstr = BattleScript_ApplySecondaryEffect;
            gHitMarker |= HITMARKER_IGNORE_SAFEGUARD;
            effect++;
            }
        }
        else
        {
            if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
             && gBattleMons[gBattlerAttacker].hp != 0
             && !gProtectStructs[gBattlerAttacker].confusionSelfDmg
             && (gSpecialStatuses[gBattlerTarget].physicalDmg || gSpecialStatuses[gBattlerTarget].specialDmg)
             && (gBattleMoves[move].flags & F_MAKES_CONTACT)
             && (Random() % 3) == 0)
            {
            gBattleCommunication[MOVE_EFFECT_BYTE] = 0x42;
            BattleScriptPushCursor();
            gBattlescriptCurrInstr = BattleScript_ApplySecondaryEffect;
            gHitMarker |= HITMARKER_IGNORE_SAFEGUARD;
            effect++;
            }
        }
                break;
            case ABILITY_STATIC:
        if (DEBUG && (gUnknown_02023A14_50 & 4))
        {
            if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
             && gBattleMons[gBattlerAttacker].hp != 0
             && !gProtectStructs[gBattlerAttacker].confusionSelfDmg
             && (gSpecialStatuses[gBattlerTarget].physicalDmg || gSpecialStatuses[gBattlerTarget].specialDmg)
             && (gBattleMoves[move].flags & F_MAKES_CONTACT))
            {
            gBattleCommunication[MOVE_EFFECT_BYTE] = 0x45;
            BattleScriptPushCursor();
            gBattlescriptCurrInstr = BattleScript_ApplySecondaryEffect;
            gHitMarker |= HITMARKER_IGNORE_SAFEGUARD;
            effect++;
            }
        }
        else
        {
            if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
             && gBattleMons[gBattlerAttacker].hp != 0
             && !gProtectStructs[gBattlerAttacker].confusionSelfDmg
             && (gSpecialStatuses[gBattlerTarget].physicalDmg || gSpecialStatuses[gBattlerTarget].specialDmg)
             && (gBattleMoves[move].flags & F_MAKES_CONTACT)
             && (Random() % 3) == 0)
            {
            gBattleCommunication[MOVE_EFFECT_BYTE] = 0x45;
            BattleScriptPushCursor();
            gBattlescriptCurrInstr = BattleScript_ApplySecondaryEffect;
            gHitMarker |= HITMARKER_IGNORE_SAFEGUARD;
            effect++;
            }
        }
                break;
            case ABILITY_FLAME_BODY:
        if (DEBUG && (gUnknown_02023A14_50 & 4))
        {
            if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
             && gBattleMons[gBattlerAttacker].hp != 0
             && !gProtectStructs[gBattlerAttacker].confusionSelfDmg
             && (gBattleMoves[move].flags & F_MAKES_CONTACT)
             && (gSpecialStatuses[gBattlerTarget].physicalDmg || gSpecialStatuses[gBattlerTarget].specialDmg))
            {
            gBattleCommunication[MOVE_EFFECT_BYTE] = 0x43;
            BattleScriptPushCursor();
            gBattlescriptCurrInstr = BattleScript_ApplySecondaryEffect;
            gHitMarker |= HITMARKER_IGNORE_SAFEGUARD;
            effect++;
            }
        }
        else
        {
            if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
             && gBattleMons[gBattlerAttacker].hp != 0
             && !gProtectStructs[gBattlerAttacker].confusionSelfDmg
             && (gBattleMoves[move].flags & F_MAKES_CONTACT)
             && (gSpecialStatuses[gBattlerTarget].physicalDmg || gSpecialStatuses[gBattlerTarget].specialDmg)
             && (Random() % 3) == 0)
            {
            gBattleCommunication[MOVE_EFFECT_BYTE] = 0x43;
            BattleScriptPushCursor();
            gBattlescriptCurrInstr = BattleScript_ApplySecondaryEffect;
            gHitMarker |= HITMARKER_IGNORE_SAFEGUARD;
            effect++;
            }
        }
                break;
            case ABILITY_CUTE_CHARM:
        if (DEBUG && (gUnknown_02023A14_50 & 4))
        {
            if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
             && gBattleMons[gBattlerAttacker].hp != 0
             && !gProtectStructs[gBattlerAttacker].confusionSelfDmg
             && (gBattleMoves[move].flags & F_MAKES_CONTACT)
             && (gSpecialStatuses[gBattlerTarget].physicalDmg || gSpecialStatuses[gBattlerTarget].specialDmg)
             && gBattleMons[gBattlerTarget].hp != 0
             && GetBattlerAbility(gBattlerAttacker) != ABILITY_OBLIVIOUS
             && GetGenderFromSpeciesAndPersonality(speciesAtk, pidAtk) != GetGenderFromSpeciesAndPersonality(speciesDef, pidDef)
             && !(gBattleMons[gBattlerAttacker].status2 & STATUS2_INFATUATION)
             && GetGenderFromSpeciesAndPersonality(speciesAtk, pidAtk) != 0xFF
             && GetGenderFromSpeciesAndPersonality(speciesDef, pidDef) != 0xFF)
            {
            gBattleMons[gBattlerAttacker].status2 |= (gBitTable[gBattlerTarget] << 0x10);
            BattleScriptPushCursor();
            gBattlescriptCurrInstr = BattleScript_CuteCharmActivates;
            effect++;
            }
        }
        else
        {
            if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
             && gBattleMons[gBattlerAttacker].hp != 0
             && !gProtectStructs[gBattlerAttacker].confusionSelfDmg
             && (gBattleMoves[move].flags & F_MAKES_CONTACT)
             && (gSpecialStatuses[gBattlerTarget].physicalDmg || gSpecialStatuses[gBattlerTarget].specialDmg)
             && gBattleMons[gBattlerTarget].hp != 0
             && (Random() % 3) == 0
             && GetBattlerAbility(gBattlerAttacker) != ABILITY_OBLIVIOUS
             && GetGenderFromSpeciesAndPersonality(speciesAtk, pidAtk) != GetGenderFromSpeciesAndPersonality(speciesDef, pidDef)
             && !(gBattleMons[gBattlerAttacker].status2 & STATUS2_INFATUATION)
             && GetGenderFromSpeciesAndPersonality(speciesAtk, pidAtk) != 0xFF
             && GetGenderFromSpeciesAndPersonality(speciesDef, pidDef) != 0xFF)
            {
            gBattleMons[gBattlerAttacker].status2 |= (gBitTable[gBattlerTarget] << 16);
            if (ItemId_GetHoldEffect(gBattleMons[gBattlerTarget].item) == HOLD_EFFECT_DESTINY_KNOT)
                gBattleMons[gBattlerTarget].status2 |= (gBitTable[gBattlerAttacker] << 16);
            BattleScriptPushCursor();
            gBattlescriptCurrInstr = BattleScript_CuteCharmActivates;
            effect++;
            }
        }
                break;
            }
            break;
        case ABILITYEFFECT_IMMUNITY: // 5
            {
                for (bank = 0; bank < gBattlersCount; bank++)
                {
                    switch (GetBattlerAbility(bank))
                    {
                    case ABILITY_IMMUNITY:
                        if (gBattleMons[bank].status1 & (STATUS1_POISON | STATUS1_TOXIC_POISON | 0xF00))  // TODO: what is 0xF00?
                        {
                            StringCopy(gBattleTextBuff1, gStatusConditionString_PoisonJpn);
                            effect = 1;
                        }
                        break;
                    case ABILITY_OWN_TEMPO:
                        if (gBattleMons[bank].status2 & STATUS2_CONFUSION)
                        {
                            StringCopy(gBattleTextBuff1, gStatusConditionString_ConfusionJpn);
                            effect = 2;
                        }
                        break;
                    case ABILITY_LIMBER:
                        if (gBattleMons[bank].status1 & STATUS1_PARALYSIS)
                        {
                            StringCopy(gBattleTextBuff1, gStatusConditionString_ParalysisJpn);
                            effect = 1;
                        }
                        break;
                    case ABILITY_INSOMNIA:
                    case ABILITY_VITAL_SPIRIT:
                        if (gBattleMons[bank].status1 & STATUS1_SLEEP)
                        {
                            gBattleMons[bank].status2 &= ~(STATUS2_NIGHTMARE);
                            StringCopy(gBattleTextBuff1, gStatusConditionString_SleepJpn);
                            effect = 1;
                        }
                        break;
                    case ABILITY_WATER_VEIL:
                        if (gBattleMons[bank].status1 & STATUS1_BURN)
                        {
                            StringCopy(gBattleTextBuff1, gStatusConditionString_BurnJpn);
                            effect = 1;
                        }
                        break;
                    case ABILITY_MAGMA_ARMOR:
                        if (gBattleMons[bank].status1 & STATUS1_FREEZE)
                        {
                            StringCopy(gBattleTextBuff1, gStatusConditionString_IceJpn);
                            effect = 1;
                        }
                        break;
                    case ABILITY_OBLIVIOUS:
                        if (gBattleMons[bank].status2 & STATUS2_INFATUATION)
                        {
                            StringCopy(gBattleTextBuff1, gStatusConditionString_LoveJpn);
                            effect = 3;
                        }
                        break;
                    }
                    if (effect)
                    {
                        switch (effect)
                        {
                        case 1: // status cleared
                            gBattleMons[bank].status1 = 0;
                            break;
                        case 2: // get rid of confusion
                            gBattleMons[bank].status2 &= ~(STATUS2_CONFUSION);
                            break;
                        case 3: // get rid of infatuation
                            gBattleMons[bank].status2 &= ~(STATUS2_INFATUATION);
                            break;
                        }
                        BattleScriptPushCursor();
                        gBattlescriptCurrInstr = BattleScript_AbilityCuredStatus;
                        gBattleStruct->scriptingActive = bank;
                        gActiveBattler = bank;
                        BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
                        MarkBattlerForControllerExec(gActiveBattler);
                        return effect;
                    }
                }
            }
            break;
        case ABILITYEFFECT_FORECAST: // 6
            {
                for (bank = 0; bank < gBattlersCount; bank++)
                {
                    if (GetBattlerAbility(bank) == ABILITY_FORECAST)
                    {
                        effect = CastformDataTypeChange(bank);
                        if (effect)
                        {
                            BattleScriptPushCursorAndCallback(BattleScript_CastformChange);
                            gBattleStruct->scriptingActive = bank;
                            gBattleStruct->castformToChangeInto = effect - 1;
                            return effect;
                        }
                    }
                }
            }
            break;
        case ABILITYEFFECT_SYNCHRONIZE: // 7
            if (gLastUsedAbility == ABILITY_SYNCHRONIZE && (gHitMarker & HITMARKER_SYNCHRONISE_EFFECT))
            {
                gHitMarker &= ~(HITMARKER_SYNCHRONISE_EFFECT);
                gBattleStruct->synchroniseEffect &= 0x3F;
                if (gBattleStruct->synchroniseEffect == MOVE_EFFECT_TOXIC) 
                    gBattleStruct->synchroniseEffect = MOVE_EFFECT_POISON;
                gBattleCommunication[MOVE_EFFECT_BYTE] = gBattleStruct->synchroniseEffect + 0x40;
                gBattleStruct->scriptingActive = gBattlerTarget;
                BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_SynchronizeActivates;
                gHitMarker |= HITMARKER_IGNORE_SAFEGUARD;
                effect++;
            }
            break;
        case ABILITYEFFECT_ATK_SYNCHRONIZE: // 8
            if (gLastUsedAbility == ABILITY_SYNCHRONIZE && (gHitMarker & HITMARKER_SYNCHRONISE_EFFECT))
            {
                gHitMarker &= ~(HITMARKER_SYNCHRONISE_EFFECT);
                gBattleStruct->synchroniseEffect &= 0x3F;
                if (gBattleStruct->synchroniseEffect == 6)
                    gBattleStruct->synchroniseEffect = 2;
                gBattleCommunication[MOVE_EFFECT_BYTE] = gBattleStruct->synchroniseEffect;
                gBattleStruct->scriptingActive = gBattlerAttacker;
                BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_SynchronizeActivates;
                gHitMarker |= HITMARKER_IGNORE_SAFEGUARD;
                effect++;
            }
            break;
        case ABILITYEFFECT_INTIMIDATE1: // 9
            for (i = 0; i < gBattlersCount; i++)
            {
                if (GetBattlerAbility(i) == ABILITY_INTIMIDATE && gStatuses3[i] & STATUS3_INTIMIDATE_POKES)
                {
                    gLastUsedAbility = ABILITY_INTIMIDATE;
                    gStatuses3[i] &= ~(STATUS3_INTIMIDATE_POKES);
                    BattleScriptPushCursorAndCallback(gUnknown_081D978C);
                    gBattleStruct->intimidateBank = i;
                    effect++;
                    break;
                }
            }
            break;
        case ABILITYEFFECT_TRACE: // 11
            for (i = 0; i < gBattlersCount; i++)
            {
                if (GetBattlerAbility(i) == ABILITY_TRACE && (gStatuses3[i] & STATUS3_TRACE))
                {
                    u8 target2;
                    side = (GetBattlerPosition(i) ^ 1) & 1;
                    target1 = GetBattlerAtPosition(side);
                    target2 = GetBattlerAtPosition(side + 2);
                    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
                    {
                        if (GetBattlerAbility(target1) != 0 && gBattleMons[target1].hp != 0
                         && GetBattlerAbility(target2) != 0 && gBattleMons[target2].hp != 0)
                        {
                            gActiveBattler = GetBattlerAtPosition(((Random() & 1) * 2) | side);
                            gBattleMons[i].ability = GetBattlerAbility(gActiveBattler); // does trace still trace w mold breaker?
                            gLastUsedAbility = GetBattlerAbility(gActiveBattler);
                            effect++;
                        }
                        else if (GetBattlerAbility(target1) != 0 && gBattleMons[target1].hp != 0)
                        {
                            gActiveBattler = target1;
                            gBattleMons[i].ability = GetBattlerAbility(gActiveBattler);
                            gLastUsedAbility = GetBattlerAbility(gActiveBattler);
                            effect++;
                        }
                        else if (GetBattlerAbility(target2) != 0 && gBattleMons[target2].hp != 0)
                        {
                            gActiveBattler = target2;
                            gBattleMons[i].ability = GetBattlerAbility(gActiveBattler);
                            gLastUsedAbility = GetBattlerAbility(gActiveBattler);
                            effect++;
                        }
                    }
                    else
                    {
                        gActiveBattler = target1;
                        if (GetBattlerAbility(target1) && gBattleMons[target1].hp)
                        {
                            gBattleMons[i].ability = GetBattlerAbility(target1);
                            gLastUsedAbility = GetBattlerAbility(target1);
                            effect++;
                        }
                    }
                    if (effect)
                    {
                        BattleScriptPushCursorAndCallback(BattleScript_TraceActivates);
                        gStatuses3[i] &= ~(STATUS3_TRACE);
                        gBattleStruct->scriptingActive = i;

                        gBattleTextBuff1[0] = 0xFD;
                        gBattleTextBuff1[1] = 4;
                        gBattleTextBuff1[2] = gActiveBattler;
                        gBattleTextBuff1[3] = gBattlerPartyIndexes[gActiveBattler];
                        gBattleTextBuff1[4] = EOS;

                        gBattleTextBuff2[0] = 0xFD;
                        gBattleTextBuff2[1] = 9;
                        gBattleTextBuff2[2] = gLastUsedAbility;
                        gBattleTextBuff2[3] = EOS;
                        break;
                    }
                }
            }
            break;
        case ABILITYEFFECT_INTIMIDATE2: // 10
            for (i = 0; i < gBattlersCount; i++)
            {
                if (GetBattlerAbility(i) == ABILITY_INTIMIDATE && (gStatuses3[i] & STATUS3_INTIMIDATE_POKES))
                {
                    gLastUsedAbility = ABILITY_INTIMIDATE;
                    gStatuses3[i] &= ~(STATUS3_INTIMIDATE_POKES);
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = gUnknown_081D9795;
                    gBattleStruct->intimidateBank = i;
                    effect++;
                    break;
                }
            }
            break;
        case ABILITYEFFECT_CHECK_OTHER_SIDE: // 12
            side = GetBattlerSide(bank);
            for (i = 0; i < gBattlersCount; i++)
            {
                if (GetBattlerSide(i) != side && GetBattlerAbility(i) == ability)
                {
                    gLastUsedAbility = ability;
                    effect = i + 1;
                }
            }
            break;
        case ABILITYEFFECT_CHECK_BANK_SIDE: // 13
            side = GetBattlerSide(bank);
            for (i = 0; i < gBattlersCount; i++)
            {
                if (GetBattlerSide(i) == side && GetBattlerAbility(i) == ability)
                {
                    gLastUsedAbility = ability;
                    effect = i + 1;
                }
            }
            break;
        case ABILITYEFFECT_FIELD_SPORT: // 14
            switch (gLastUsedAbility)
            {
            case 0xFD:
                for (i = 0; i < gBattlersCount; i++)
                {
                    if (gStatuses3[i] & STATUS3_MUDSPORT)
                        effect = i + 1;
                }
                break;
            case 0xFE:
                for (i = 0; i < gBattlersCount; i++)
                {
                    if (gStatuses3[i] & STATUS3_WATERSPORT)
                        effect = i + 1;
                }
                break;
            default:
                for (i = 0; i < gBattlersCount; i++)
                {
                    if (GetBattlerAbility(i) == ability)
                    {
                        gLastUsedAbility = ability;
                        effect = i + 1;
                    }
                }
                break;
            }
            break;
        case ABILITYEFFECT_CHECK_ON_FIELD: // 19
            for (i = 0; i < gBattlersCount; i++)
            {
                if (GetBattlerAbility(i) == ability && gBattleMons[i].hp != 0)
                {
                    gLastUsedAbility = ability;
                    effect = i + 1;
                }
            }
            break;
        case ABILITYEFFECT_CHECK_FIELD_EXCEPT_BANK: // 15
            for (i = 0; i < gBattlersCount; i++)
            {
                if (GetBattlerAbility(i) == ability && i != bank)
                {
                    gLastUsedAbility = ability;
                    effect = i + 1;
                }
            }
            break;
        case ABILITYEFFECT_COUNT_OTHER_SIZE: // 16
            side = GetBattlerSide(bank);
            for (i = 0; i < gBattlersCount; i++)
            {
                if (GetBattlerSide(i) != side && GetBattlerAbility(i) == ability)
                {
                    gLastUsedAbility = ability;
                    effect++;
                }
            }
            break;
        case ABILITYEFFECT_COUNT_BANK_SIDE: // 17
            side = GetBattlerSide(bank);
            for (i = 0; i < gBattlersCount; i++)
            {
                if (GetBattlerSide(i) == side && GetBattlerAbility(i) == ability)
                {
                    gLastUsedAbility = ability;
                    effect++;
                }
            }
            break;
        case ABILITYEFFECT_COUNT_ON_FIELD: // 18
            for (i = 0; i < gBattlersCount; i++)
            {
                if (GetBattlerAbility(i) == ability && i != bank)
                {
                    gLastUsedAbility = ability;
                    effect++;
                }
            }
            break;
        }
        if (effect && caseID < 0xC && gLastUsedAbility != 0xFF)
            RecordAbilityBattle(bank, gLastUsedAbility);
    }

    return effect;
}

void BattleScriptExecute(const u8* BS_ptr)
{
    gBattlescriptCurrInstr = BS_ptr;
    B_FUNCTION_STACK->ptr[B_FUNCTION_STACK->size++] = gBattleMainFunc;
    gBattleMainFunc = RunBattleScriptCommands_PopCallbacksStack;
    gCurrentActionFuncId = 0;
}

void BattleScriptPushCursorAndCallback(u8* BS_ptr)
{
    BattleScriptPushCursor();
    gBattlescriptCurrInstr = BS_ptr;
    B_FUNCTION_STACK->ptr[B_FUNCTION_STACK->size++] = gBattleMainFunc;
    gBattleMainFunc = RunBattleScriptCommands;
}

enum
{
    ITEM_NO_EFFECT, // 0
    ITEM_STATUS_CHANGE, // 1
    ITEM_EFFECT_OTHER, // 2
    ITEM_PP_CHANGE, // 3
    ITEM_HP_CHANGE, // 4
    ITEM_STATS_CHANGE, // 5
};

enum
{
    FLAVOR_SPICY, // 0
    FLAVOR_DRY, // 1
    FLAVOR_SWEET, // 2
    FLAVOR_BITTER, // 3
    FLAVOR_SOUR, // 4
};

u8 ItemBattleEffects(u8 caseID, u8 bank, bool8 moveTurn)
{
    int i = 0;
    u8 effect = ITEM_NO_EFFECT;
    u8 changedPP = 0;
    u8 bankHoldEffect, atkHoldEffect, defHoldEffect;
    u8 bankQuality, atkQuality, defQuality;
    u16 atkItem, defItem;

    if (gStatuses3[bank] & STATUS3_EMBARGO)
        return ITEM_NO_EFFECT;
    if (GetBattlerAbility(bank) == ABILITY_KLUTZ)
        return ITEM_NO_EFFECT;

    gLastUsedItem = gBattleMons[bank].item;
    if (gLastUsedItem == ITEM_ENIGMA_BERRY)
    {
        bankHoldEffect = gEnigmaBerries[bank].holdEffect;
        bankQuality = gEnigmaBerries[bank].holdEffectParam;
    }
    else
    {
        bankHoldEffect = ItemId_GetHoldEffect(gLastUsedItem);
        bankQuality = ItemId_GetHoldEffectParam(gLastUsedItem);
    }

    atkItem = gBattleMons[gBattlerAttacker].item;
    if (atkItem == ITEM_ENIGMA_BERRY)
    {
        atkHoldEffect = gEnigmaBerries[gBattlerAttacker].holdEffect;
        atkQuality = gEnigmaBerries[gBattlerAttacker].holdEffectParam;
    }
    else
    {
        atkHoldEffect = ItemId_GetHoldEffect(atkItem);
        atkQuality = ItemId_GetHoldEffectParam(atkItem);
    }

    defItem = gBattleMons[gBattlerTarget].item;
    if (defItem == ITEM_ENIGMA_BERRY)
    {
        defHoldEffect = gEnigmaBerries[gBattlerTarget].holdEffect;
        defQuality = gEnigmaBerries[gBattlerTarget].holdEffectParam;
    }
    else
    {
        defHoldEffect = ItemId_GetHoldEffect(defItem);
        defQuality = ItemId_GetHoldEffectParam(defItem);
    }

    switch (caseID)
    {
    case ITEMEFFECT_ON_SWITCH_IN: // whenever the white herb is activated
        switch (bankHoldEffect)
        {
        case HOLD_EFFECT_DOUBLE_PRIZE:
            gBattleStruct->moneyMultiplier = 2;
            break;
        case HOLD_EFFECT_RESTORE_STATS:
            for (i = 0; i < 8; i++)
            {
                if (gBattleMons[bank].statStages[i] < 6)
                {
                    gBattleMons[bank].statStages[i] = 6;
                    effect = ITEM_STATS_CHANGE;
                }
            }
            if (effect)
            {
                gBattleStruct->scriptingActive = bank;
                gPotentialItemEffectBattler = bank;
                gActiveBattler = gBattlerAttacker = bank;
                BattleScriptExecute(BattleScript_WhiteHerbEnd2);
            }
            break;
        }
        break;
    case 1: // after attacking, bankHoldEffect means it's whichever pokemon
        if (gBattleMons[bank].hp)
        {
            switch (bankHoldEffect)
            {
            case HOLD_EFFECT_RESTORE_HP:
                if (gBattleMons[bank].hp <= gBattleMons[bank].maxHP / 2 && !moveTurn)
                {
                    if (bankQuality < 10)
                        gBattleMoveDamage = gBattleMons[bank].maxHP / bankQuality;
                    else 
                        gBattleMoveDamage = bankQuality;
                    if (gBattleMons[bank].hp + gBattleMoveDamage > gBattleMons[bank].maxHP)
                        gBattleMoveDamage = gBattleMons[bank].maxHP - gBattleMons[bank].hp;
                    gBattleMoveDamage *= -1;
                    BattleScriptExecute(BattleScript_ItemHealHP_RemoveItem);
                    effect = 4;
                }
                break;
            case HOLD_EFFECT_RESTORE_PP:
                if (!moveTurn)
                {
                    struct Pokemon* poke;
                    u8 ppBonuses;
                    u16 move;

                    if (GetBattlerSide(bank) == 0)
                        poke = &gPlayerParty[gBattlerPartyIndexes[bank]];
                    else
                        poke = &gEnemyParty[gBattlerPartyIndexes[bank]];
                    for (i = 0; i < 4; i++)
                    {
                        move = GetMonData(poke, MON_DATA_MOVE1 + i);
                        changedPP = GetMonData(poke, MON_DATA_PP1 + i);
                        ppBonuses = GetMonData(poke, MON_DATA_PP_BONUSES);
                        if (move && changedPP == 0)
                            break;
                    }
                    if (i != 4)
                    {
                        u8 maxPP = CalculatePPWithBonus(move, ppBonuses, i);
                        if (changedPP + bankQuality > maxPP)
                            changedPP = maxPP;
                        else
                            changedPP = changedPP + bankQuality;
                        gBattleTextBuff1[0] = 0xFD;
                        gBattleTextBuff1[1] = 2;
                        gBattleTextBuff1[2] = move;
                        gBattleTextBuff1[3] = move >> 8;
                        gBattleTextBuff1[4] = 0xFF;
                        BattleScriptExecute(BattleScript_BerryPPHealEnd2);
                        BtlController_EmitSetMonData(0, i + REQUEST_PPMOVE1_BATTLE, 0, 1, &changedPP);
                        MarkBattlerForControllerExec(gActiveBattler);
                        effect = ITEM_PP_CHANGE;
                    }
                }
                break;
            case HOLD_EFFECT_RESTORE_STATS:
                for (i = 0; i < 8; i++)
                {
                    if (gBattleMons[bank].statStages[i] < 6)
                    {
                        gBattleMons[bank].statStages[i] = 6;
                        effect = ITEM_STATS_CHANGE;
                    }
                }
                if (effect)
                {
                    gBattleStruct->scriptingActive = bank;
                    gPotentialItemEffectBattler = bank;
                    gActiveBattler = gBattlerAttacker = bank;
                    BattleScriptExecute(BattleScript_WhiteHerbEnd2);
                }
                break;
            case HOLD_EFFECT_LEFTOVERS:
                if (gBattleMons[bank].hp < gBattleMons[bank].maxHP && !moveTurn && gStatuses3[bank] & STATUS3_HEAL_BLOCK)
                {
                    gBattleMoveDamage = gBattleMons[bank].maxHP / 16;
                    if (gBattleMoveDamage == 0)
                        gBattleMoveDamage = 1;
                    if (gBattleMons[bank].hp + gBattleMoveDamage > gBattleMons[bank].maxHP)
                        gBattleMoveDamage = gBattleMons[bank].maxHP - gBattleMons[bank].hp;
                    gBattleMoveDamage *= -1;
                    BattleScriptExecute(BattleScript_ItemHealHP_End2);
                    effect = ITEM_HP_CHANGE;
                    RecordItemEffectBattle(bank, bankHoldEffect);
                }
                break;
            case HOLD_EFFECT_CONFUSE_SPICY:
                ConfuseBerry(bank, bankQuality, FLAVOR_SPICY, moveTurn, FALSE, FALSE);
                break;
            case HOLD_EFFECT_CONFUSE_DRY:
                ConfuseBerry(bank, bankQuality, FLAVOR_DRY, moveTurn, FALSE, FALSE);
                break;
            case HOLD_EFFECT_CONFUSE_SWEET:
                ConfuseBerry(bank, bankQuality, FLAVOR_SWEET, moveTurn, FALSE, FALSE);
                break;
            case HOLD_EFFECT_CONFUSE_BITTER:
                ConfuseBerry(bank, bankQuality, FLAVOR_BITTER, moveTurn, FALSE, FALSE);
                break;
            case HOLD_EFFECT_CONFUSE_SOUR:
                ConfuseBerry(bank, bankQuality, FLAVOR_SOUR, moveTurn, FALSE, FALSE);
                break;
            case HOLD_EFFECT_ATTACK_UP:
                if (gLastUsedItem != ITEM_SNOWBALL)
                    effect = StatChangeBerry(bank, bankQuality, STAT_STAGE_ATK, moveTurn, effect, FALSE, FALSE);
                break;
            case HOLD_EFFECT_DEFENSE_UP:
                if (gLastUsedItem != ITEM_KEE_BERRY)
                    effect = StatChangeBerry(bank, bankQuality, STAT_STAGE_DEF, moveTurn, effect, FALSE, FALSE);
                break;
            case HOLD_EFFECT_SPEED_UP:
                if (gLastUsedItem != ITEM_ADRENALINE_ORB)
                    effect = StatChangeBerry(bank, bankQuality, STAT_STAGE_SPEED, moveTurn, effect, FALSE, FALSE);
                break;
            case HOLD_EFFECT_SP_ATTACK_UP:
                effect = StatChangeBerry(bank, bankQuality, STAT_STAGE_SPATK, moveTurn, effect, FALSE, FALSE);
                break;
            case HOLD_EFFECT_SP_DEFENSE_UP:
                if (gLastUsedItem != ITEM_MARANGA_BERRY && gLastUsedItem != ITEM_LUMINOUS_MOSS)
                    effect = StatChangeBerry(bank, bankQuality, STAT_STAGE_SPDEF, moveTurn, effect, FALSE, FALSE);
                break;
            case HOLD_EFFECT_CRITICAL_UP:
                if (GetBattlerAbility(bank) == ABILITY_GLUTTONY)
                    bankQuality /= 2;
                if (gBattleMons[bank].hp <= gBattleMons[bank].maxHP / bankQuality && !moveTurn && !(gBattleMons[bank].status2 & STATUS2_FOCUS_ENERGY))
                {
                    gBattleMons[bank].status2 |= STATUS2_FOCUS_ENERGY;
                    BattleScriptExecute(BattleScript_BerryFocusEnergyEnd2);
                    effect = ITEM_EFFECT_OTHER;
                }
                break;
            case HOLD_EFFECT_RANDOM_STAT_UP:
                if (GetBattlerAbility(bank) == ABILITY_GLUTTONY)
                    bankQuality /= 2;
                if (!moveTurn && gBattleMons[bank].hp <= gBattleMons[bank].maxHP / bankQuality)
                {
                    for (i = 0; i < 5; i++)
                    {
                        if (gBattleMons[bank].statStages[STAT_STAGE_ATK + i] < 0xC)
                            break;
                    }
                    if (i != 5)
                    {
                        do
                        {
                            i = Random() % 5;
                        } while (gBattleMons[bank].statStages[STAT_STAGE_ATK + i] == 0xC);

                        gBattleTextBuff1[0] = 0xFD;
                        gBattleTextBuff1[1] = 5;
                        gBattleTextBuff1[2] = i + 1;
                        gBattleTextBuff1[3] = EOS;

                        gBattleTextBuff2[0] = 0xFD;
                        gBattleTextBuff2[1] = 0;
                        gBattleTextBuff2[2] = 0xD1;
                        gBattleTextBuff2[3] = 0xD1 >> 8;
                        gBattleTextBuff2[4] = 0;
                        gBattleTextBuff2[5] = 0xD2;
                        gBattleTextBuff2[6] = 0xD2 >> 8;
                        gBattleTextBuff2[7] = EOS;

                        gEffectBattler = bank;
                        gBattleStruct->scriptingActive = bank;
                        gBattleStruct->statChanger = 0x21 + i;
                        gBattleStruct->animArg1 = 0x21 + i + 6;
                        gBattleStruct->animArg2 = 0;
                        BattleScriptExecute(BattleScript_BerryStatRaiseEnd2);
                        effect = ITEM_STATS_CHANGE;
                    }
                }
                break;
            case HOLD_EFFECT_CURE_PAR:
                if (gBattleMons[bank].status1 & STATUS1_PARALYSIS)
                {
                    gBattleMons[bank].status1 &= ~(STATUS1_PARALYSIS);
                    BattleScriptExecute(BattleScript_BerryCurePrlzEnd2);
                    effect = ITEM_STATUS_CHANGE;
                }
                break;
            case HOLD_EFFECT_CURE_PSN:
                if (gBattleMons[bank].status1 & STATUS1_PSN_ANY)
                {
                    gBattleMons[bank].status1 &= ~(STATUS1_PSN_ANY | STATUS1_TOXIC_COUNTER);
                    BattleScriptExecute(BattleScript_BerryCurePsnEnd2);
                    effect = ITEM_STATUS_CHANGE;
                }
                break;
            case HOLD_EFFECT_CURE_BRN:
                if (gBattleMons[bank].status1 & STATUS1_BURN)
                {
                    gBattleMons[bank].status1 &= ~(STATUS1_BURN);
                    BattleScriptExecute(BattleScript_BerryCureBrnEnd2);
                    effect = ITEM_STATUS_CHANGE;
                }
                break;
            case HOLD_EFFECT_CURE_FRZ:
                if (gBattleMons[bank].status1 & STATUS1_FREEZE)
                {
                    gBattleMons[bank].status1 &= ~(STATUS1_FREEZE);
                    BattleScriptExecute(BattleScript_BerryCureFrzEnd2);
                    effect = ITEM_STATUS_CHANGE;
                }
                break;
            case HOLD_EFFECT_CURE_SLP:
                if (gBattleMons[bank].status1 & STATUS1_SLEEP)
                {
                    gBattleMons[bank].status1 &= ~(STATUS1_SLEEP);
                    gBattleMons[bank].status2 &= ~(STATUS2_NIGHTMARE);
                    BattleScriptExecute(BattleScript_BerryCureSlpEnd2);
                    effect = ITEM_STATUS_CHANGE;
                }
                break;
            case HOLD_EFFECT_CURE_CONFUSION:
                if (gBattleMons[bank].status2 & STATUS2_CONFUSION)
                {
                    gBattleMons[bank].status2 &= ~(STATUS2_CONFUSION);
                    BattleScriptExecute(BattleScript_BerryCureConfusionEnd2);
                    effect = ITEM_EFFECT_OTHER;
                }
                break;
            case HOLD_EFFECT_CURE_STATUS:
                if (gBattleMons[bank].status1 & STATUS1_ANY || gBattleMons[bank].status2 & STATUS2_CONFUSION)
                {
                    i = 0;
                    if (gBattleMons[bank].status1 & STATUS1_PSN_ANY)
                    {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_PoisonJpn);
                        i++;
                    }
                    if (gBattleMons[bank].status1 & STATUS1_SLEEP)
                    {
                        gBattleMons[bank].status2 &= ~(STATUS2_NIGHTMARE);
                        StringCopy(gBattleTextBuff1, gStatusConditionString_SleepJpn);
                        i++;
                    }
                    if (gBattleMons[bank].status1 & STATUS1_PARALYSIS)
                    {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_ParalysisJpn);
                        i++;
                    }
                    if (gBattleMons[bank].status1 & STATUS1_BURN)
                    {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_BurnJpn);
                        i++;
                    }
                    if (gBattleMons[bank].status1 & STATUS1_FREEZE)
                    {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_IceJpn);
                        i++;
                    }
                    if (gBattleMons[bank].status2 & STATUS2_CONFUSION)
                    {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_ConfusionJpn);
                        i++;
                    }
                    if (!(i > 1))
                        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                    else
                        gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                    gBattleMons[bank].status1 = 0;
                    gBattleMons[bank].status2 &= ~(STATUS2_CONFUSION);
                    BattleScriptExecute(BattleScript_BerryCureChosenStatusEnd2);
                    effect = ITEM_STATUS_CHANGE;
                }
                break;
            case HOLD_EFFECT_MENTAL_HERB:
                // effect will always be zero at this point
                if (gBattleMons[bank].status2 & STATUS2_INFATUATION)
                {
                    gBattleMons[bank].status2 &= ~(STATUS2_INFATUATION);
                    StringCopy(gBattleTextBuff1, gStatusConditionString_LoveJpn);
                    effect++;
                }
                if (gDisableStructs[bank].tauntTimer1)
                {
                    gDisableStructs[bank].tauntTimer1 = 0;
                    gDisableStructs[bank].tauntTimer2 = 0;
                    StringCopy(gBattleTextBuff1, BattleText_Taunt);
                    effect++;
                }
                if (gDisableStructs[bank].encoreTimer1)
                {
                    gDisableStructs[gActiveBattler].encoredMove = 0;
                    gDisableStructs[gActiveBattler].encoreTimer1 = 0;
                    StringCopy(gBattleTextBuff1, BattleText_Encore);
                    effect++;
                }
                if (gBattleMons[bank].status2 & STATUS2_TORMENT)
                {
                    gBattleMons[bank].status2 &= ~(STATUS2_TORMENT);
                    StringCopy(gBattleTextBuff1, BattleText_Torment);
                    effect++;
                }
                if (gDisableStructs[bank].disableTimer1)
                {
                    gDisableStructs[bank].disabledMove = 0;
                    gDisableStructs[bank].disableTimer1 = 0;
                    StringCopy(gBattleTextBuff1, BattleText_Disable);
                    effect++;
                }

                if (effect)
                {
                    if (effect > 1) // if it's curing more than one status
                        StringCopy(gBattleTextBuff1, BattleText_Many);
                    gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                    BattleScriptExecute(BattleScript_BerryCureChosenStatusEnd2);
                    effect = ITEM_EFFECT_OTHER;
                }
                break;
            case HOLD_EFFECT_STICKY_BARB:
            case HOLD_EFFECT_BLACK_SLUDGE:
                if (!moveTurn)
                {
                    gBattleMoveDamage = gBattleMons[bank].maxHP / 16;
                    if (gBattleMoveDamage == 0)
                        gBattleMoveDamage = 1;

                    if ((gBattleMons[bank].type1 == TYPE_POISON || gBattleMons[bank].type2 == TYPE_POISON)
                        && bankHoldEffect == HOLD_EFFECT_BLACK_SLUDGE) {
                        if (gBattleMons[bank].hp + gBattleMoveDamage > gBattleMons[bank].maxHP)
                            gBattleMoveDamage = gBattleMons[bank].maxHP - gBattleMons[bank].hp;
                        gBattleMoveDamage *= -1;
                        BattleScriptExecute(BattleScript_ItemHealHP_End2);
                    } else {
                        gBattleMoveDamage *= 2;
                        if (gBattleMons[bank].hp < gBattleMoveDamage)
                            gBattleMoveDamage = gBattleMons[bank].hp;
                        BattleScriptExecute(BattleScript_BlackSludge);
                    }
                    effect = ITEM_HP_CHANGE;
                    RecordItemEffectBattle(bank, bankHoldEffect);
                }
                break;
            case HOLD_EFFECT_TOXIC_ORB:
                if (!(gBattleMons[bank].status1 & STATUS1_TOXIC_POISON) && !moveTurn)
                {
                    gBattleMons[bank].status1 &= STATUS1_TOXIC_POISON;
                    BattleScriptExecute(BattleScript_ToxicOrb);
                    effect = ITEM_STATUS_CHANGE;
                    RecordItemEffectBattle(bank, bankHoldEffect);
                }
                break;
            case HOLD_EFFECT_FLAME_ORB:
                if (!(gBattleMons[bank].status1 & STATUS1_BURN) && !moveTurn)
                {
                    gBattleMons[bank].status1 &= STATUS1_BURN;
                    BattleScriptExecute(BattleScript_FlameOrb);
                    effect = ITEM_STATUS_CHANGE;
                    RecordItemEffectBattle(bank, bankHoldEffect);
                }
                break;
            case HOLD_EFFECT_SHOCK_ORB:
                if (!(gBattleMons[bank].status1 & STATUS1_PARALYSIS) && !moveTurn)
                {
                    gBattleMons[bank].status1 &= STATUS1_PARALYSIS;
                    BattleScriptExecute(BattleScript_ShockOrb);
                    effect = ITEM_STATUS_CHANGE;
                    RecordItemEffectBattle(bank, bankHoldEffect);
                }
                break;
            }
            if (effect)
            {
                gBattleStruct->scriptingActive = bank;
                gPotentialItemEffectBattler = bank;
                gActiveBattler = gBattlerAttacker = bank;
                switch (effect)
                {
                case ITEM_STATUS_CHANGE:
                    BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[bank].status1);
                    MarkBattlerForControllerExec(gActiveBattler);
                    break;
                case ITEM_PP_CHANGE:
                    if (!(gBattleMons[bank].status2 & STATUS2_TRANSFORMED) && !(gDisableStructs[bank].mimickedMoves & gBitTable[i]))
                        gBattleMons[bank].pp[i] = changedPP;
                    break;
                }
            }
        }
        break;
    case 2: // activated by abilities
        switch (defHoldEffect)
        {
        case HOLD_EFFECT_SPEED_UP:
            if (gBattleMons[gBattlerTarget].hp != 0
                && (gDisableStructs[gBattlerTarget].intimidate && defItem == ITEM_ADRENALINE_ORB)
                && (gBattleMons[gBattlerTarget].statStages[STAT_STAGE_SPEED] < 0xC))
            {
                gBattleMons[gBattlerTarget].statStages[STAT_STAGE_SPEED]++;
                //gBattleStruct->animArg1 = 0x11;
                gBattleStruct->animArg2 = 0;
                BattleScriptPushCursorAndCallback(BattleScript_AbilityItems);
                gBattleStruct->scriptingActive = gBattlerTarget;
                effect++;
            }
            gDisableStructs[gBattlerTarget].intimidate = FALSE;
            break;
        }
        break;
    case ITEMEFFECT_MOVE_END: // end turn (look at everyone's items)
        for (bank = 0; bank < gBattlersCount; bank++)
        {
            gLastUsedItem = gBattleMons[bank].item;
            if (gBattleMons[bank].item == ITEM_ENIGMA_BERRY)
            {
                bankHoldEffect = gEnigmaBerries[bank].holdEffect;
                bankQuality = gEnigmaBerries[bank].holdEffectParam;
            }
            else
            {
                bankHoldEffect = ItemId_GetHoldEffect(gLastUsedItem);
                bankQuality = ItemId_GetHoldEffectParam(gLastUsedItem);
            }
            switch (bankHoldEffect)
            {
            case HOLD_EFFECT_CURE_PAR:
                if (gBattleMons[bank].status1 & STATUS1_PARALYSIS)
                {
                    gBattleMons[bank].status1 &= ~(STATUS1_PARALYSIS);
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_BerryCureParRet;
                    effect = ITEM_STATUS_CHANGE;
                }
                break;
            case HOLD_EFFECT_CURE_PSN:
                if (gBattleMons[bank].status1 & STATUS1_PSN_ANY)
                {
                    gBattleMons[bank].status1 &= ~(STATUS1_PSN_ANY | STATUS1_TOXIC_COUNTER);
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_BerryCurePsnRet;
                    effect = ITEM_STATUS_CHANGE;
                }
                break;
            case HOLD_EFFECT_CURE_BRN:
                if (gBattleMons[bank].status1 & STATUS1_BURN)
                {
                    gBattleMons[bank].status1 &= ~(STATUS1_BURN);
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_BerryCureBrnRet;
                    effect = ITEM_STATUS_CHANGE;
                }
                break;
            case HOLD_EFFECT_CURE_FRZ:
                if (gBattleMons[bank].status1 & STATUS1_FREEZE)
                {
                    gBattleMons[bank].status1 &= ~(STATUS1_FREEZE);
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_BerryCureFrzRet;
                    effect = ITEM_STATUS_CHANGE;
                }
                break;
            case HOLD_EFFECT_CURE_SLP:
                if (gBattleMons[bank].status1 & STATUS1_SLEEP)
                {
                    gBattleMons[bank].status1 &= ~(STATUS1_SLEEP);
                    gBattleMons[bank].status2 &= ~(STATUS2_NIGHTMARE);
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_BerryCureSlpRet;
                    effect = ITEM_STATUS_CHANGE;
                }
                break;
            case HOLD_EFFECT_CURE_CONFUSION:
                if (gBattleMons[bank].status2 & STATUS2_CONFUSION)
                {
                    gBattleMons[bank].status2 &= ~(STATUS2_CONFUSION);
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_BerryCureConfusionRet;
                    effect = ITEM_EFFECT_OTHER;
                }
                break;
            case HOLD_EFFECT_MENTAL_HERB:
                // effect will always be zero at this point
                if (gBattleMons[bank].status2 & STATUS2_INFATUATION)
                {
                    gBattleMons[bank].status2 &= ~(STATUS2_INFATUATION);
                    StringCopy(gBattleTextBuff1, gStatusConditionString_LoveJpn);
                    effect++;
                }
                if (gDisableStructs[bank].tauntTimer1)
                {
                    gDisableStructs[bank].tauntTimer1 = 0;
                    gDisableStructs[bank].tauntTimer2 = 0;
                    StringCopy(gBattleTextBuff1, BattleText_Taunt);
                    effect++;
                }
                if (gDisableStructs[bank].encoreTimer1)
                {
                    gDisableStructs[bank].encoredMove = 0;
                    gDisableStructs[bank].encoreTimer1 = 0;
                    StringCopy(gBattleTextBuff1, BattleText_Encore);
                    effect++;
                }
                if (gBattleMons[bank].status2 & STATUS2_TORMENT)
                {
                    gBattleMons[bank].status2 &= ~(STATUS2_TORMENT);
                    StringCopy(gBattleTextBuff1, BattleText_Torment);
                    effect++;
                }
                if (gDisableStructs[bank].disableTimer1)
                {
                    gDisableStructs[bank].disabledMove = 0;
                    gDisableStructs[bank].disableTimer1 = 0;
                    StringCopy(gBattleTextBuff1, BattleText_Disable);
                    effect++;
                }

                if (effect)
                {
                    if (effect > 1) // if it's curing more than one status
                        StringCopy(gBattleTextBuff1, BattleText_Many);
                    BattleScriptPushCursor();
                    gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                    gBattlescriptCurrInstr = BattleScript_BerryCureChosenStatusRet;
                    effect = ITEM_EFFECT_OTHER;
                }
                break;
            case HOLD_EFFECT_CURE_STATUS:
                if (gBattleMons[bank].status1 & STATUS1_ANY || gBattleMons[bank].status2 & STATUS2_CONFUSION)
                {
                    if (gBattleMons[bank].status1 & STATUS1_PSN_ANY)
                    {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_PoisonJpn);
                    }
                    if (gBattleMons[bank].status1 & STATUS1_SLEEP)
                    {
                        gBattleMons[bank].status2 &= ~(STATUS2_NIGHTMARE);
                        StringCopy(gBattleTextBuff1, gStatusConditionString_SleepJpn);
                    }
                    if (gBattleMons[bank].status1 & STATUS1_PARALYSIS)
                    {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_ParalysisJpn);
                    }
                    if (gBattleMons[bank].status1 & STATUS1_BURN)
                    {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_BurnJpn);
                    }
                    if (gBattleMons[bank].status1 & STATUS1_FREEZE)
                    {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_IceJpn);
                    }
                    if (gBattleMons[bank].status2 & STATUS2_CONFUSION)
                    {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_ConfusionJpn);
                    }
                    gBattleMons[bank].status1 = 0;
                    gBattleMons[bank].status2 &= ~(STATUS2_CONFUSION);
                    BattleScriptPushCursor();
                    gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                    gBattlescriptCurrInstr = BattleScript_BerryCureChosenStatusRet;
                    effect = ITEM_STATUS_CHANGE;
                }
                break;
            case HOLD_EFFECT_RESTORE_STATS:
                for (i = 0; i < NUM_BATTLE_STATS; i++)
                {
                    if (gBattleMons[bank].statStages[i] < 6)
                    {
                        gBattleMons[bank].statStages[i] = 6;
                        effect = ITEM_STATS_CHANGE;
                    }
                }
                if (effect)
                {
                    gBattleStruct->scriptingActive = bank;
                    gPotentialItemEffectBattler = bank;
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_WhiteHerbRet;
                }
                break;
            }
            if (effect)
            {
                gBattleStruct->scriptingActive = bank;
                gPotentialItemEffectBattler = bank;
                gActiveBattler = bank;
                BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
                MarkBattlerForControllerExec(gActiveBattler);
                break;
            }
        }
        break;
    case ITEMEFFECT_KINGSROCK_SHELLBELL: // upon attacking (attacking item hold effect)
        switch (atkHoldEffect)
        {
        case HOLD_EFFECT_FLINCH:
            if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
                && (gSpecialStatuses[gBattlerTarget].physicalDmg || gSpecialStatuses[gBattlerTarget].specialDmg)
                && (Random() % 100) < bankQuality
                && gBattleMoves[gCurrentMove].flags & F_AFFECTED_BY_KINGS_ROCK
                && gBattleMons[gBattlerTarget].hp
                && gBattleMoveDamage)
            {
                gBattleCommunication[MOVE_EFFECT_BYTE] = 8;
                BattleScriptPushCursor();
                SetMoveEffect(0, 0);
                BattleScriptPop();
            }
            break;
        case HOLD_EFFECT_SHELL_BELL:
            if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
                && gSpecialStatuses[gBattlerTarget].dmg != 0
                && gSpecialStatuses[gBattlerTarget].dmg != 0xFFFF
                && gBattlerAttacker != gBattlerTarget
                && gBattleMons[gBattlerAttacker].hp != gBattleMons[gBattlerAttacker].maxHP
                && gBattleMons[gBattlerAttacker].hp != 0
                && gBattleMoveDamage)
            {
                gLastUsedItem = atkItem;
                gPotentialItemEffectBattler = gBattlerAttacker;
                gBattleStruct->scriptingActive = gBattlerAttacker;
                gBattleMoveDamage = (gSpecialStatuses[gBattlerTarget].dmg / atkQuality) * -1;
                if (gBattleMoveDamage == 0)
                    gBattleMoveDamage = -1;
                gSpecialStatuses[gBattlerTarget].dmg = 0;
                BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_ItemHealHP_Ret;
                effect++;
            }
            break;
        case HOLD_EFFECT_CUSTAP_BERRY:
            gDisableStructs[gBattlerAttacker].berryActivates = TRUE;
        case HOLD_EFFECT_QUICK_CLAW:
            if (gBattleMons[gBattlerAttacker].hp && gDisableStructs[gBattlerAttacker].quickClaw)
            {
                gLastUsedItem = atkItem;
                gPotentialItemEffectBattler = gBattlerAttacker;
                gBattleStruct->scriptingActive = gBattlerAttacker;
                BattleScriptPushCursor();
                if (gDisableStructs[gBattlerAttacker].berryActivates)
                    gBattlescriptCurrInstr = BattleScript_BerryAllowedFirstMove;
                else
                    gBattlescriptCurrInstr = BattleScript_ItemAllowedFirstMove;
                effect++;
                gDisableStructs[gBattlerAttacker].quickClaw = FALSE;
            }
            gDisableStructs[gBattlerAttacker].berryActivates = FALSE;
            break;
        case HOLD_EFFECT_LIFE_ORB:
            if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
                && gSpecialStatuses[gBattlerTarget].dmg != 0
                && gSpecialStatuses[gBattlerTarget].dmg != 0xFFFF
                && gBattlerAttacker != gBattlerTarget
                && gBattleMons[gBattlerAttacker].hp != gBattleMons[gBattlerAttacker].maxHP
                && gBattleMons[gBattlerAttacker].hp != 0
                && gBattleMoveDamage)
            {
                gLastUsedItem = atkItem;
                gPotentialItemEffectBattler = gBattlerAttacker;
                gBattleStruct->scriptingActive = gBattlerAttacker;
                gBattleMoveDamage = (gBattleMons[gBattlerAttacker].maxHP / 10);
                if (gBattleMoveDamage == 0)
                    gBattleMoveDamage = 1;
                BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_LifeOrb;
                effect++;
            }
            break;
        }
        switch (defHoldEffect)
        {
        case HOLD_EFFECT_HALVE_BERRIES:
            if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
                && gSpecialStatuses[gBattlerTarget].dmg != 0
                && gSpecialStatuses[gBattlerTarget].dmg != 0xFFFF
                && gBattleMons[gBattlerAttacker].hp != 0
                && gBattleMoveDamage
                && gDisableStructs[gBattlerTarget].wasLastMoveSuperEffective)
            {
                gLastUsedItem = defItem;
                gPotentialItemEffectBattler = gBattlerTarget;
                gBattleStruct->scriptingActive = gBattlerTarget;
                BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_BerryWeakenedDamage;
                effect++;
                gDisableStructs[gBattlerTarget].wasLastMoveSuperEffective = FALSE;
            }
            break;
        case HOLD_EFFECT_HURT_BERRIES:
            gDisableStructs[gBattlerTarget].berryActivates = TRUE;
        case HOLD_EFFECT_HURT_IF_DAMAGED:
            if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
                && gSpecialStatuses[gBattlerTarget].dmg != 0
                && gSpecialStatuses[gBattlerTarget].dmg != 0xFFFF
                && gBattleMons[gBattlerAttacker].hp != 0
                && gBattleMoveDamage
                && ((gBattleMoves[gCurrentMove].split == MOVE_PHYSICAL && defItem == ITEM_JABOCA_BERRY)
                 || (gBattleMoves[gCurrentMove].split == MOVE_SPECIAL && defItem == ITEM_ROWAP_BERRY)
                 || (gBattleMoves[gCurrentMove].flags & F_MAKES_CONTACT && defItem == ITEM_ROCKY_HELMET)))
            {
                gLastUsedItem = defItem;
                gPotentialItemEffectBattler = gBattlerTarget;
                gBattleStruct->scriptingActive = gBattlerTarget;
                gBattleMoveDamage = gBattleMons[gBattlerAttacker].maxHP / 6;
                if (gBattleMoveDamage == 0)
                    gBattleMoveDamage = 1;
                BattleScriptPushCursor();
                if (gDisableStructs[gBattlerTarget].berryActivates)
                    gBattlescriptCurrInstr = BattleScript_BerryCausedDamage;
                else
                    gBattlescriptCurrInstr = BattleScript_ItemCausedDamage;
                effect++;
            }
            gDisableStructs[gBattlerTarget].berryActivates = FALSE;
            break;
        case HOLD_EFFECT_ATTACK_UP:
            if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
                && gSpecialStatuses[gBattlerTarget].dmg != 0
                && gSpecialStatuses[gBattlerTarget].dmg != 0xFFFF
                && gBattleMons[gBattlerTarget].hp != 0
                && gBattleMoveDamage
                && gBattleMons[bank].statStages[STAT_STAGE_ATK] < 0xC
                && (gBattleMoves[gCurrentMove].type == TYPE_NORMAL && defItem == ITEM_SNOWBALL))
            {
                StatChangeBerry(gBattlerTarget, defQuality, STAT_STAGE_ATK, FALSE, 0, TRUE, FALSE);
                effect++;
            }
            break;
        case HOLD_EFFECT_DEFENSE_UP:
            if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
                && gSpecialStatuses[gBattlerTarget].dmg != 0
                && gSpecialStatuses[gBattlerTarget].dmg != 0xFFFF
                && gBattleMons[gBattlerTarget].hp != 0
                && gBattleMoveDamage
                && gBattleMons[bank].statStages[STAT_STAGE_DEF] < 0xC
                && (gBattleMoves[gCurrentMove].split == MOVE_PHYSICAL && defItem == ITEM_KEE_BERRY))
            {
                StatChangeBerry(gBattlerTarget, defQuality, STAT_STAGE_DEF, FALSE, 0, TRUE, FALSE);
                effect++;
            }
            break;
        case HOLD_EFFECT_SP_DEFENSE_UP:
            if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
                && gSpecialStatuses[gBattlerTarget].dmg != 0
                && gSpecialStatuses[gBattlerTarget].dmg != 0xFFFF
                && gBattleMons[gBattlerTarget].hp != 0
                && gBattleMoveDamage
                && gBattleMons[gBattlerTarget].statStages[STAT_STAGE_SPDEF] < 0xC
                && ((gBattleMoves[gCurrentMove].split == MOVE_SPECIAL && defItem == ITEM_MARANGA_BERRY)
                 || (gBattleMoves[gCurrentMove].type == TYPE_WATER && defItem == ITEM_LUMINOUS_MOSS)))
            {
                StatChangeBerry(gBattlerTarget, defQuality, STAT_STAGE_SPDEF, FALSE, 0, TRUE, FALSE);
                effect++;
            }
            break;
        case HOLD_EFFECT_STICKY_BARB:
            if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
                && gSpecialStatuses[gBattlerTarget].dmg != 0
                && gSpecialStatuses[gBattlerTarget].dmg != 0xFFFF
                && gBattleMons[gBattlerTarget].hp != 0
                && gBattleMoveDamage
                && (gBattleMoves[gCurrentMove].flags & F_MAKES_CONTACT))
            {
                if (gBattleMons[gBattlerAttacker].item)
                    break;
                else
                {
                    gLastUsedItem = gBattleMons[gBattlerTarget].item;
                    *USED_HELD_ITEM(gBattlerAttacker) = gLastUsedItem;
                    gBattleMons[gBattlerTarget].item = 0;
                    gBattleMons[gBattlerAttacker].item = gLastUsedItem;
                    gActiveBattler = gBattlerAttacker;
                    BtlController_EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gLastUsedItem);
                    MarkBattlerForControllerExec(gBattlerAttacker);
                    gActiveBattler = gBattlerTarget;
                    BtlController_EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gBattlerTarget].item);
                    MarkBattlerForControllerExec(gBattlerTarget);

                    gBattlescriptCurrInstr = BattleScript_ItemStuck;
                    effect++;
                }
            }
            break;
        }
        break;
    case 5: // right before the attack animation?  would need to return
        switch (atkHoldEffect)
        {
        }
        break;
    case ITEMEFFECT_BUGBITE_BERRIES: // bug bite berries on move end, called by atk49_moveend
        if (gBattleMons[bank].hp) 
        // note that the only berries that need be handled here are the berries that have a sort of hp requirement so they activate immediately
        {
            switch (bankHoldEffect)
            {
            case HOLD_EFFECT_RESTORE_HP:
                if (!moveTurn)
                {
                    if (bankQuality < 10)
                        gBattleMoveDamage = gBattleMons[bank].maxHP / bankQuality;
                    else 
                        gBattleMoveDamage = bankQuality;
                    if (gBattleMons[bank].hp + gBattleMoveDamage > gBattleMons[bank].maxHP)
                        gBattleMoveDamage = gBattleMons[bank].maxHP - gBattleMons[bank].hp;
                    gBattleMoveDamage *= -1;
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_ItemHealHP_RemoveItemRet;
                    effect = 4;
                }
                break;
            case HOLD_EFFECT_RESTORE_PP:
                if (!moveTurn)
                {
                    struct Pokemon* poke;
                    u8 ppBonuses;
                    u16 move;

                    if (GetBattlerSide(bank) == 0)
                        poke = &gPlayerParty[gBattlerPartyIndexes[bank]];
                    else
                        poke = &gEnemyParty[gBattlerPartyIndexes[bank]];
                    for (i = 0; i < 4; i++)
                    {
                        move = GetMonData(poke, MON_DATA_MOVE1 + i);
                        changedPP = GetMonData(poke, MON_DATA_PP1 + i);
                        ppBonuses = GetMonData(poke, MON_DATA_PP_BONUSES);
                        if (move && changedPP == 0)
                            break;
                    }
                    if (i != 4)
                    {
                        u8 maxPP = CalculatePPWithBonus(move, ppBonuses, i);
                        if (changedPP + bankQuality > maxPP)
                            changedPP = maxPP;
                        else
                            changedPP = changedPP + bankQuality;
                        gBattleTextBuff1[0] = 0xFD;
                        gBattleTextBuff1[1] = 2;
                        gBattleTextBuff1[2] = move;
                        gBattleTextBuff1[3] = move >> 8;
                        gBattleTextBuff1[4] = 0xFF;
                        BattleScriptPushCursor();
                        gBattlescriptCurrInstr = BattleScript_BerryPPHealRet;
                        BtlController_EmitSetMonData(0, i + REQUEST_PPMOVE1_BATTLE, 0, 1, &changedPP);
                        MarkBattlerForControllerExec(gActiveBattler);
                        effect = ITEM_PP_CHANGE;
                    }
                }
                break;
            case HOLD_EFFECT_CONFUSE_SPICY:
                ConfuseBerry(bank, bankQuality, FLAVOR_SPICY, moveTurn, TRUE, TRUE);
                effect = TRUE;
                break;
            case HOLD_EFFECT_CONFUSE_DRY:
                ConfuseBerry(bank, bankQuality, FLAVOR_DRY, moveTurn, TRUE, TRUE);
                effect = TRUE;
                break;
            case HOLD_EFFECT_CONFUSE_SWEET:
                ConfuseBerry(bank, bankQuality, FLAVOR_SWEET, moveTurn, TRUE, TRUE);
                effect = TRUE;
                break;
            case HOLD_EFFECT_CONFUSE_BITTER:
                ConfuseBerry(bank, bankQuality, FLAVOR_BITTER, moveTurn, TRUE, TRUE);
                effect = TRUE;
                break;
            case HOLD_EFFECT_CONFUSE_SOUR:
                ConfuseBerry(bank, bankQuality, FLAVOR_SOUR, moveTurn, TRUE, TRUE);
                effect = TRUE;
                break;
            case HOLD_EFFECT_ATTACK_UP:
                if (gLastUsedItem != ITEM_SNOWBALL)
                    effect = StatChangeBerry(bank, bankQuality, STAT_STAGE_ATK, moveTurn, effect, TRUE, TRUE);
                break;
            case HOLD_EFFECT_DEFENSE_UP:
                if (gLastUsedItem != ITEM_KEE_BERRY)
                    effect = StatChangeBerry(bank, bankQuality, STAT_STAGE_DEF, moveTurn, effect, TRUE, TRUE);
                break;
            case HOLD_EFFECT_SPEED_UP:
                if (gLastUsedItem != ITEM_ADRENALINE_ORB)
                    effect = StatChangeBerry(bank, bankQuality, STAT_STAGE_SPEED, moveTurn, effect, TRUE, TRUE);
                break;
            case HOLD_EFFECT_SP_ATTACK_UP:
                effect = StatChangeBerry(bank, bankQuality, STAT_STAGE_SPATK, moveTurn, effect, TRUE, TRUE);
                break;
            case HOLD_EFFECT_SP_DEFENSE_UP:
                if (gLastUsedItem != ITEM_MARANGA_BERRY && gLastUsedItem != ITEM_LUMINOUS_MOSS)
                    effect = StatChangeBerry(bank, bankQuality, STAT_STAGE_SPDEF, moveTurn, effect, TRUE, TRUE);
                break;
            case HOLD_EFFECT_CRITICAL_UP:
                if (!moveTurn && !(gBattleMons[bank].status2 & STATUS2_FOCUS_ENERGY))
                {
                    gBattleMons[bank].status2 |= STATUS2_FOCUS_ENERGY;
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_BerryFocusEnergyRet;
                    effect = ITEM_EFFECT_OTHER;
                }
                break;
            case HOLD_EFFECT_RANDOM_STAT_UP:
                if (!moveTurn)
                {
                    for (i = 0; i < 5; i++)
                    {
                        if (gBattleMons[bank].statStages[STAT_STAGE_ATK + i] < 0xC)
                            break;
                    }
                    if (i != 5)
                    {
                        do
                        {
                            i = Random() % 5;
                        } while (gBattleMons[bank].statStages[STAT_STAGE_ATK + i] == 0xC);

                        gBattleTextBuff1[0] = 0xFD;
                        gBattleTextBuff1[1] = 5;
                        gBattleTextBuff1[2] = i + 1;
                        gBattleTextBuff1[3] = EOS;

                        gBattleTextBuff2[0] = 0xFD;
                        gBattleTextBuff2[1] = 0;
                        gBattleTextBuff2[2] = 0xD1;
                        gBattleTextBuff2[3] = 0xD1 >> 8;
                        gBattleTextBuff2[4] = 0;
                        gBattleTextBuff2[5] = 0xD2;
                        gBattleTextBuff2[6] = 0xD2 >> 8;
                        gBattleTextBuff2[7] = EOS;

                        gEffectBattler = bank;
                        gBattleStruct->scriptingActive = bank;
                        gBattleStruct->statChanger = 0x21 + i;
                        gBattleStruct->animArg1 = 0x21 + i + 6;
                        gBattleStruct->animArg2 = 0;
                        BattleScriptPushCursor();
                        gBattlescriptCurrInstr = BattleScript_BerryStatRaiseRet;
                        effect = ITEM_STATS_CHANGE;
                    }
                }
                break;
            // all the status berries do not need to be handled here because they activate anyway.
            }
            if (effect)
            {
                gBattleStruct->scriptingActive = bank;
                gPotentialItemEffectBattler = bank;
                gActiveBattler = gBattlerAttacker = bank;
                switch (effect)
                {
                case ITEM_STATUS_CHANGE:
                    BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[bank].status1);
                    MarkBattlerForControllerExec(gActiveBattler);
                    break;
                case ITEM_PP_CHANGE:
                    if (!(gBattleMons[bank].status2 & STATUS2_TRANSFORMED) && !(gDisableStructs[bank].mimickedMoves & gBitTable[i]))
                        gBattleMons[bank].pp[i] = changedPP;
                    break;
                }
            }
        }
        break;
    }

    return effect;
}

void ConfuseBerry(u8 bank, u8 bankQuality, u8 flavor, bool8 moveTurn, bool8 passChecks, bool8 shouldReturn) {
    if (((gBattleMons[bank].hp <= gBattleMons[bank].maxHP / 4
         || (gBattleMons[bank].hp <= gBattleMons[bank].maxHP / 2 && GetBattlerAbility(bank) == ABILITY_GLUTTONY))
         && !moveTurn)
        || passChecks)
    {
        gBattleTextBuff1[0] = 253;
        gBattleTextBuff1[1] = 8;
        gBattleTextBuff1[2] = flavor;
        gBattleTextBuff1[3] = EOS;
        gBattleMoveDamage = gBattleMons[bank].maxHP / bankQuality;
        if (gBattleMoveDamage == 0)
            gBattleMoveDamage = 1;
        if (gBattleMons[bank].hp + gBattleMoveDamage > gBattleMons[bank].maxHP)
            gBattleMoveDamage = gBattleMons[bank].maxHP - gBattleMons[bank].hp;
        gBattleMoveDamage *= -1;

        if (shouldReturn)
        {
            BattleScriptPushCursor();
            if (GetPokeFlavourRelation(gBattleMons[bank].personality, flavor) < 0)
            {
                gBattlescriptCurrInstr = BattleScript_BerryConfuseHealRet;
            }
            else
            {
                gBattlescriptCurrInstr = BattleScript_ItemHealHP_RemoveItemRet;
            }
        }
        else
        {
            if (GetPokeFlavourRelation(gBattleMons[bank].personality, flavor) < 0)
                BattleScriptExecute(BattleScript_BerryConfuseHealEnd2);
            else
                BattleScriptExecute(BattleScript_ItemHealHP_RemoveItem);
        }
    }
}

u8 StatChangeBerry(u8 bank, u8 bankQuality, u8 stat, bool8 moveTurn, u8 effect, bool8 passChecks, bool8 shouldReturn) {
    if (GetBattlerAbility(bank) == ABILITY_GLUTTONY)
        bankQuality /= 2;
    if ((gBattleMons[bank].hp <= gBattleMons[bank].maxHP / bankQuality && !moveTurn && gBattleMons[bank].statStages[stat] < 0xC)
        || passChecks)
    {
        gBattleTextBuff1[0] = 0xFD;
        gBattleTextBuff1[1] = 5;
        gBattleTextBuff1[2] = stat;
        gBattleTextBuff1[3] = EOS;

        gEffectBattler = bank;
        gBattleStruct->scriptingActive = bank;
        gBattleStruct->statChanger = 0x10 + stat;
        gBattleStruct->animArg1 = 0xE + stat;
        gBattleStruct->animArg2 = 0;
        if (shouldReturn)
        {
            BattleScriptPushCursor();
            gBattlescriptCurrInstr = BattleScript_BerryStatRaiseRet;
        }
        else
            BattleScriptExecute(BattleScript_BerryStatRaiseEnd2);
        return ITEM_STATS_CHANGE;
    } else 
        return effect;
}

struct CombinedMove
{
    u16 move1;
    u16 move2;
    u16 newMove;
};

static const struct CombinedMove sCombinedMoves[2] = // bro this is so sad
{
    {MOVE_EMBER, MOVE_GUST, MOVE_HEAT_WAVE},
    {0xFFFF, 0xFFFF, 0xFFFF}
};

void sub_801B594(void)
{
    if (gBattleControllerExecFlags == 0)
        gBattleScriptingCommandsTable[*gBattlescriptCurrInstr]();
}

u8 GetMoveTarget(u16 move, u8 useMoveTarget) //get move target
{
    u8 targetBank = 0;
    u8 moveTarget;
    u8 side;

    if (useMoveTarget)
        moveTarget = useMoveTarget - 1;
    else
        moveTarget = gBattleMoves[move].target;

    switch (moveTarget)
    {
    case 0:
        side = GetBattlerSide(gBattlerAttacker) ^ 1;
        if (gSideTimers[side].followmeTimer && gBattleMons[gSideTimers[side].followmeTarget].hp)
            targetBank = gSideTimers[side].followmeTarget;
        else
        {
            side = GetBattlerSide(gBattlerAttacker);
            do
            {
                targetBank = Random() % gBattlersCount;
            } while (targetBank == gBattlerAttacker || side == GetBattlerSide(targetBank) || gAbsentBattlerFlags & gBitTable[targetBank]);
            if (gBattleMoves[move].type == TYPE_ELECTRIC
                && AbilityBattleEffects(ABILITYEFFECT_COUNT_OTHER_SIZE, gBattlerAttacker, ABILITY_LIGHTNING_ROD, 0, 0)
                && GetBattlerAbility(targetBank) != ABILITY_LIGHTNING_ROD)
            {
                targetBank ^= 2;
                RecordAbilityBattle(targetBank, GetBattlerAbility(targetBank));
                gSpecialStatuses[targetBank].lightningRodRedirected = 1;
            }
            else if (gBattleMoves[move].type == TYPE_WATER
                && AbilityBattleEffects(ABILITYEFFECT_COUNT_OTHER_SIZE, gBattlerAttacker, ABILITY_STORM_DRAIN, 0, 0)
                && GetBattlerAbility(targetBank) != ABILITY_STORM_DRAIN)
            {
                targetBank ^= 2;
                RecordAbilityBattle(targetBank, GetBattlerAbility(targetBank));
                gSpecialStatuses[targetBank].stormDrainRedirected = 1;
            }
        }
        break;
    case 1:
    case 8:
    case 32:
    case 64:
        targetBank = GetBattlerAtPosition((GetBattlerPosition(gBattlerAttacker) & 1) ^ 1);
        if (gAbsentBattlerFlags & gBitTable[targetBank])
            targetBank ^= 2;
        break;
    case 4:
        side = GetBattlerSide(gBattlerAttacker) ^ 1;
        if (gSideTimers[side].followmeTimer && gBattleMons[gSideTimers[side].followmeTarget].hp)
            targetBank = gSideTimers[side].followmeTarget;
        else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && moveTarget & 4)
        {
            if (GetBattlerSide(gBattlerAttacker) == 0)
            {
                if (Random() & 1)
                    targetBank = GetBattlerAtPosition(1);
                else
                    targetBank = GetBattlerAtPosition(3);
            }
            else
            {
                if (Random() & 1)
                    targetBank = GetBattlerAtPosition(0);
                else
                    targetBank = GetBattlerAtPosition(2);
            }
            if (gAbsentBattlerFlags & gBitTable[targetBank])
                targetBank ^= 2;
        }
        else
            targetBank = GetBattlerAtPosition((GetBattlerPosition(gBattlerAttacker) & 1) ^ 1);
        break;
    case 2:
    case 16:
        targetBank = gBattlerAttacker;
        break;
    }
    ewram16010arr(gBattlerAttacker) = targetBank;
    return targetBank;
}

u8 IsMonDisobedient(void)
{
    u8 obedienceLevel;
    s32 rnd;
    s32 calc;

    if (gBattleTypeFlags & BATTLE_TYPE_LINK
     || GetBattlerSide(gBattlerAttacker) == 1
     || !IsOtherTrainer(gBattleMons[gBattlerAttacker].otId, gBattleMons[gBattlerAttacker].otName))
    return 0;

    if (DEBUG && (gUnknown_02023A14_50 & 0x40))
    {
    obedienceLevel = 10;
    }
    else
    {
    if (FlagGet(FLAG_BADGE08_GET))
        return 0;
    obedienceLevel = 10;
    if (FlagGet(FLAG_BADGE02_GET))
        obedienceLevel = 30;
    if (FlagGet(FLAG_BADGE04_GET))
        obedienceLevel = 50;
    if (FlagGet(FLAG_BADGE06_GET))
        obedienceLevel = 70;
    }

    if (gBattleMons[gBattlerAttacker].level <= obedienceLevel)
        return 0;
    rnd = (Random() & 255);
    calc = (gBattleMons[gBattlerAttacker].level + obedienceLevel) * rnd >> 8;
    if (calc < obedienceLevel)
        return 0;

    // is not obedient
    if (gCurrentMove == MOVE_RAGE)
        gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_RAGE);
    if (gBattleMons[gBattlerAttacker].status1 & STATUS1_SLEEP && (gCurrentMove == MOVE_SNORE || gCurrentMove == MOVE_SLEEP_TALK))
    {
        gBattlescriptCurrInstr = gUnknown_081D995F;
        return 1;
    }

    rnd = (Random() & 255);
    calc = (gBattleMons[gBattlerAttacker].level + obedienceLevel) * rnd >> 8;
    if (calc < obedienceLevel)
    {
        calc = CheckMoveLimitations(gBattlerAttacker, gBitTable[gCurrMovePos], 0xFF);
        if (calc == 0xF) // all moves cannot be used
        {
            gBattleCommunication[MULTISTRING_CHOOSER] = Random() & 3;
            gBattlescriptCurrInstr = BattleScript_MoveUsedLoafingAround;
            return 1;
        }
        else // use a random move
        {
            do
            {
                gCurrMovePos = gUnknown_02024BE5 = Random() & 3;
            } while (gBitTable[gCurrMovePos] & calc);
            gRandomMove = gBattleMons[gBattlerAttacker].moves[gCurrMovePos];
            gBattleCommunication[3] = 0;
            gDynamicBasePower = 0;
            gBattleStruct->dynamicMoveType = 0;
            gBattlescriptCurrInstr = BattleScript_IgnoresAndUsesRandomMove;
            gBattlerTarget = GetMoveTarget(gRandomMove, 0);
            gHitMarker |= HITMARKER_x200000;
            return 2;
        }
    }
    else
    {
        obedienceLevel = gBattleMons[gBattlerAttacker].level - obedienceLevel;

        calc = (Random() & 255);
        if (calc < obedienceLevel && !(gBattleMons[gBattlerAttacker].status1 & STATUS1_ANY) && GetBattlerAbility(gBattlerAttacker) != ABILITY_VITAL_SPIRIT && GetBattlerAbility(gBattlerAttacker) != ABILITY_INSOMNIA)
        {
            // try putting asleep
            int i;
            for (i = 0; i < gBattlersCount; i++)
            {
                if (gBattleMons[i].status2 & STATUS2_UPROAR)
                    break;
            }
            if (i == gBattlersCount)
            {
                gBattlescriptCurrInstr = BattleScript_IgnoresAndFallsAsleep;
                return 1;
            }
        }
        calc -= obedienceLevel;
        if (calc < obedienceLevel)
        {
            gBattleMoveDamage = CalculateBaseDamage(&gBattleMons[gBattlerAttacker], &gBattleMons[gBattlerAttacker], MOVE_POUND, 0, 40, 0, gBattlerAttacker, gBattlerAttacker);
            gBattlerTarget = gBattlerAttacker;
            gBattlescriptCurrInstr = gUnknown_081D99A0;
            gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
            return 2;
        }
        else
        {
            gBattleCommunication[MULTISTRING_CHOOSER] = Random() & 3;
            gBattlescriptCurrInstr = BattleScript_MoveUsedLoafingAround;
            return 1;
        }
    }
}
