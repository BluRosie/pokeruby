#include "global.h"
#include "constants/battle_constants.h"
#include "constants/hold_effects.h"
#include "constants/items.h"
#include "constants/species.h"
#include "battle.h"
#include "evolution_scene.h"
#include "ewram.h"
#include "item.h"
#include "main.h"
#include "overworld.h"
#include "pokemon.h"
#include "pokemon_item_effect.h"
#include "rom_8077ABC.h"
#include "rom_8094928.h"
#include "util.h"

extern s32 gBattleMoveDamage;
extern u8 gAbsentBattlerFlags;
extern u8 gBankInMenu;
extern u8 gBattlersCount;
extern u16 gBattlerPartyIndexes[];
extern u8 gActiveBattler;
extern u8 gStringBank;
extern struct BattlePokemon gBattleMons[];
extern struct BattleEnigmaBerry gEnigmaBerries[];

static const u8 sGetMonDataEVConstants[] =
{
    MON_DATA_HP_EV,
    MON_DATA_ATK_EV,
    MON_DATA_DEF_EV,
    MON_DATA_SPEED_EV,
    MON_DATA_SPATK_EV,
    MON_DATA_SPDEF_EV
};

extern u8 gPPUpReadMasks[];
extern u8 gPPUpWriteMasks[];
extern u8 gPPUpValues[];

bool8 PokemonUseItemEffects(struct Pokemon *pkmn, u16 item, u8 partyIndex, u8 moveIndex, u8 e);

bool8 ExecuteTableBasedItemEffect_(struct Pokemon *pkmn, u16 item, u8 partyIndex, u8 moveIndex)
{
    return PokemonUseItemEffects(pkmn, item, partyIndex, moveIndex, 0);
}

bool8 PokemonUseItemEffects(struct Pokemon *pkmn, u16 item, u8 partyIndex, u8 moveIndex, u8 e)
{
    u32 data;
    s32 friendship;
    s32 cmdIndex;
    bool8 retVal = TRUE;
    const u8 *itemEffect;
    u8 sp24 = 6;
    u32 sp28;
    s8 sp2C = 0;
    u8 holdEffect;
    u8 sp34 = 4;
    u16 heldItem;
    u8 r10;
    u32 r4;
    u32 evCount = GetMonEVCount(pkmn);
    u8 i = 0;

    heldItem = GetMonData(pkmn, MON_DATA_HELD_ITEM, NULL);
    if (heldItem == ITEM_ENIGMA_BERRY)
    {
        if (gMain.inBattle)
            holdEffect = gEnigmaBerries[gBankInMenu].holdEffect;
        else
            holdEffect = gSaveBlock1.enigmaBerry.holdEffect;
    }
    else
    {
        holdEffect = ItemId_GetHoldEffect(heldItem);
    }

    gStringBank = gBankInMenu;
    if (gMain.inBattle)
    {
        gActiveBattler = gBankInMenu;
        cmdIndex = (GetBattlerSide(gActiveBattler) != 0);
        while (cmdIndex < gBattlersCount)
        {
            if (gBattlerPartyIndexes[cmdIndex] == partyIndex)
            {
                sp34 = cmdIndex;
                break;
            }
            cmdIndex += 2;
        }
    }
    else
    {
        gActiveBattler = 0;
        sp34 = 4;
    }

    if (!IS_POKEMON_ITEM(item))
        return TRUE;
    if (ItemId_GetEffect(item) == NULL && item != ITEM_ENIGMA_BERRY)
        return TRUE;

    if (item == ITEM_ENIGMA_BERRY)
    {
        if (gMain.inBattle)
            itemEffect = gEnigmaBerries[gActiveBattler].itemEffect;
        else
            itemEffect = gSaveBlock1.enigmaBerry.itemEffect;
    }
    else
    {
        itemEffect = ItemId_GetEffect(item);
    }

    for (cmdIndex = 0; cmdIndex < 6; cmdIndex++)
    {
        switch (cmdIndex)
        {
        case 0: // currently just sacred ash right here, handled in GetItemEffectType in party_menu.c
            break;
        case X_ITEMS:
            if ((itemEffect[cmdIndex] & RAISE_ATTACK)
             && gBattleMons[gActiveBattler].statStages[STAT_STAGE_ATK] < 12)
            {
                gBattleMons[gActiveBattler].statStages[STAT_STAGE_ATK] += itemEffect[8];
                if (gBattleMons[gActiveBattler].statStages[STAT_STAGE_ATK] > 12)
                    gBattleMons[gActiveBattler].statStages[STAT_STAGE_ATK] = 12;
                return FALSE;
            }
            if ((itemEffect[cmdIndex] & RAISE_DEFENSE)
             && gBattleMons[gActiveBattler].statStages[STAT_STAGE_DEF] < 12)
            {
                gBattleMons[gActiveBattler].statStages[STAT_STAGE_DEF] += itemEffect[8];
                if (gBattleMons[gActiveBattler].statStages[STAT_STAGE_DEF] > 12)
                    gBattleMons[gActiveBattler].statStages[STAT_STAGE_DEF] = 12;
                return FALSE;
            }
            if ((itemEffect[cmdIndex] & RAISE_SPEED)
             && gBattleMons[gActiveBattler].statStages[STAT_STAGE_SPEED] < 12)
            {
                gBattleMons[gActiveBattler].statStages[STAT_STAGE_SPEED] += itemEffect[8];
                if (gBattleMons[gActiveBattler].statStages[STAT_STAGE_SPEED] > 12)
                    gBattleMons[gActiveBattler].statStages[STAT_STAGE_SPEED] = 12;
                return FALSE;
            }
            if ((itemEffect[cmdIndex] & RAISE_SP_ATK)
             && gBattleMons[gActiveBattler].statStages[STAT_STAGE_SPATK] < 12)
            {
                gBattleMons[gActiveBattler].statStages[STAT_STAGE_SPATK] += itemEffect[8];
                if (gBattleMons[gActiveBattler].statStages[STAT_STAGE_SPATK] > 12)
                    gBattleMons[gActiveBattler].statStages[STAT_STAGE_SPATK] = 12;
                return FALSE;
            }
            if ((itemEffect[cmdIndex] & RAISE_SP_DEF)
             && gBattleMons[gActiveBattler].statStages[STAT_STAGE_SPDEF] < 12)
            {
                gBattleMons[gActiveBattler].statStages[STAT_STAGE_SPDEF] += itemEffect[8];
                if (gBattleMons[gActiveBattler].statStages[STAT_STAGE_SPDEF] > 12)
                    gBattleMons[gActiveBattler].statStages[STAT_STAGE_SPDEF] = 12;
                return FALSE;
            }
            if ((itemEffect[cmdIndex] & RAISE_ACCURACY)
             && gBattleMons[gActiveBattler].statStages[STAT_STAGE_ACC] < 12)
            {
                gBattleMons[gActiveBattler].statStages[STAT_STAGE_ACC] += itemEffect[8];
                if (gBattleMons[gActiveBattler].statStages[STAT_STAGE_ACC] > 12)
                    gBattleMons[gActiveBattler].statStages[STAT_STAGE_ACC] = 12;
                return FALSE;
            }
            if ((itemEffect[cmdIndex] & RAISE_CRITICAL)
             && !(gBattleMons[gActiveBattler].status2 & STATUS2_FOCUS_ENERGY))
            {
                gBattleMons[gActiveBattler].status2 |= STATUS2_FOCUS_ENERGY;
                return FALSE;
            }
            if ((itemEffect[cmdIndex] & PREVENT_STAT_LOSS)
             && gSideTimers[GetBattlerSide(gActiveBattler)].mistTimer == 0)
            {
                gSideTimers[GetBattlerSide(gActiveBattler)].mistTimer = 5;
                return FALSE;
            }
            break;
        case VITAMINS:
            if (evCount >= 510)
                return TRUE;

            while (!(itemEffect[VITAMINS] << i & EV_HP)) { // runs through the items 
                i++;
                if (i >= 6) {
                    break;
                }
            }

            data = GetMonData(pkmn, sGetMonDataEVConstants[i], NULL);
            if (i < 6)
            {
                if ((data >= 100) && IS_ITEM_VITAMIN(item)) { // only the vitamins are limited to 100
                    retVal = TRUE;
                    break;
                }
                else if ((data + itemEffect[PARAMETER] >= 100) && IS_ITEM_VITAMIN(item)) {
                    r4 = itemEffect[PARAMETER];
                    data = 100 - r4;
                }
                else
                    r4 = itemEffect[PARAMETER];
                if (evCount + r4 > 510)
                    r4 += 510 - (evCount + r4);
                data += r4;
                SetMonData(pkmn, sGetMonDataEVConstants[i], &data);
                CalculateMonStats(pkmn);
                retVal = FALSE;
            }
            else
            {
                retVal = TRUE;
            }
            break;
        case STATUS_HEALING: // these work too
            if ((itemEffect[cmdIndex] & RAISE_LEVEL)  // raise level
             && GetMonData(pkmn, MON_DATA_LEVEL, NULL) != 100)
            {
                data = gExperienceTables[gBaseStats[GetMonData(pkmn, MON_DATA_SPECIES, NULL)].growthRate][GetMonData(pkmn, MON_DATA_LEVEL, NULL) + 1];
                SetMonData(pkmn, MON_DATA_EXP, &data);
                CalculateMonStats(pkmn);
                retVal = FALSE;
            }
            if ((itemEffect[cmdIndex] & CURE_INFATUATION)
             && gMain.inBattle && sp34 != 4 && (gBattleMons[sp34].status2 & STATUS2_INFATUATION))
            {
                gBattleMons[sp34].status2 &= ~STATUS2_INFATUATION;
                retVal = FALSE;
            }
            if ((itemEffect[cmdIndex] & CURE_SLEEP)
             && HealStatusConditions(pkmn, partyIndex, 7, sp34) == 0)
            {
                if (sp34 != 4)
                    gBattleMons[sp34].status2 &= ~STATUS2_NIGHTMARE;
                retVal = FALSE;
            }
            if ((itemEffect[cmdIndex] & CURE_POISON) && HealStatusConditions(pkmn, partyIndex, 0xF88, sp34) == 0)
                retVal = FALSE;
            if ((itemEffect[cmdIndex] & CURE_BURN) && HealStatusConditions(pkmn, partyIndex, 16, sp34) == 0)
                retVal = FALSE;
            if ((itemEffect[cmdIndex] & CURE_ICE) && HealStatusConditions(pkmn, partyIndex, 32, sp34) == 0)
                retVal = FALSE;
            if ((itemEffect[cmdIndex] & CURE_PARALYSIS) && HealStatusConditions(pkmn, partyIndex, 64, sp34) == 0)
                retVal = FALSE;
            if ((itemEffect[cmdIndex] & CURE_CONFUSION)
             && gMain.inBattle && sp34 != 4 && (gBattleMons[sp34].status2 & STATUS2_CONFUSION))
            {
                gBattleMons[sp34].status2 &= ~STATUS2_CONFUSION;
                retVal = FALSE;
            }
            break;
        case 4:
            r10 = itemEffect[cmdIndex];
            if (itemEffect[cmdIndex] & PP_UP)
            {
                r10 &= ~PP_UP;
                data = (GetMonData(pkmn, MON_DATA_PP_BONUSES, NULL) & gPPUpReadMasks[moveIndex]) >> (moveIndex * 2);
                sp28 = CalculatePPWithBonus(GetMonData(pkmn, MON_DATA_MOVE1 + moveIndex, NULL), GetMonData(pkmn, MON_DATA_PP_BONUSES, NULL), moveIndex);
                if (data < 3 && sp28 > 4)
                {
                    data = GetMonData(pkmn, MON_DATA_PP_BONUSES, NULL) + gPPUpValues[moveIndex];
                    SetMonData(pkmn, MON_DATA_PP_BONUSES, &data);

                    data = CalculatePPWithBonus(GetMonData(pkmn, MON_DATA_MOVE1 + moveIndex, NULL), data, moveIndex) - sp28;
                    data = GetMonData(pkmn, MON_DATA_PP1 + moveIndex, NULL) + data;
                    SetMonData(pkmn, MON_DATA_PP1 + moveIndex, &data);
                    retVal = FALSE;
                }
            }
            sp28 = 0;
            while (r10 != 0)
            {
                if (r10 & 1)
                {
                    s32 r5;
     
                    switch (sp28)
                    {
                    case 0:
                    case 1:
                        break;
                    // if trying to revive
                    case 2:
                        if (r10 & (REVIVES >> sp28))
                        {
                            if (GetMonData(pkmn, MON_DATA_HP, NULL) != 0)
                            {
                                sp24++;
                                break;
                            }
                            if (gMain.inBattle)
                            {
                                if (sp34 != 4)
                                {
                                    gAbsentBattlerFlags &= ~gBitTable[sp34];
                                    CopyPlayerPartyMonToBattleData(sp34, pokemon_order_func(gBattlerPartyIndexes[sp34]));
                                    if (GetBattlerSide(gActiveBattler) == 0 && gBattleResults.unk4 < 255)
                                        gBattleResults.unk4++;
                                }
                                else
                                {
                                    gAbsentBattlerFlags &= ~gBitTable[gActiveBattler ^ 2];
                                    if (GetBattlerSide(gActiveBattler) == 0 && gBattleResults.unk4 < 255)
                                        gBattleResults.unk4++;
                                }
                            }
                        }
                        else // if trying to heal
                        {
                            if (GetMonData(pkmn, MON_DATA_HP, NULL) == 0)
                            {
                                sp24++;
                                break;
                            }
                        }
                        data = itemEffect[sp24++];
                        switch (data)
                        {
                        case 0xFF: // fully heal
                            data = GetMonData(pkmn, MON_DATA_MAX_HP, NULL) - GetMonData(pkmn, MON_DATA_HP, NULL);
                            break;
                        case 0xFE: // halfway heal
                            data = GetMonData(pkmn, MON_DATA_MAX_HP, NULL) / 2;
                            if (data == 0)
                                data = 1;
                            break;
                        case 0xFD: // keep distance from max constant
                            data = eStatHp;
                            break;
                        case 0xFC: // quarter heal
                            data = GetMonData(pkmn, MON_DATA_MAX_HP, NULL) / 4;
                            if (data == 0)
                                data = 1;
                            break;
                        }
                        if (GetMonData(pkmn, MON_DATA_MAX_HP, NULL) != GetMonData(pkmn, MON_DATA_HP, NULL))
                        {
                            if (e == 0)
                            {
                                data = GetMonData(pkmn, MON_DATA_HP, NULL) + data;
                                if (data > GetMonData(pkmn, MON_DATA_MAX_HP, NULL))
                                    data = GetMonData(pkmn, MON_DATA_MAX_HP, NULL);
                                SetMonData(pkmn, MON_DATA_HP, &data);
                                if (gMain.inBattle && sp34 != 4)
                                {
                                    gBattleMons[sp34].hp = data;
                                    if (!(r10 & 0x10) && GetBattlerSide(gActiveBattler) == 0) // won't let the opponent use revives?
                                    {
                                        if (gBattleResults.unk3 < 255)
                                            gBattleResults.unk3++;
                                        // I have to re-use this variable to match.
                                        r5 = gActiveBattler;
                                        gActiveBattler = sp34;
                                        EmitGetAttributes(0, 0, 0);
                                        MarkBufferBankForExecution(gActiveBattler);
                                        gActiveBattler = r5;
                                    }
                                }
                            }
                            else
                            {
                                gBattleMoveDamage = -data;
                            }
                            retVal = FALSE;
                        }
                        r10 &= 0xEF;
                        break;
                    // pp things
                    case 3:
                        if (!(r10 & (LIMITED_PP_RESTORE_ITEM >> sp28))) // if healing all moves' pp
                        {
                            for (r5 = 0; r5 < 4; r5++)
                            {
                                u16 r4;

                                data = GetMonData(pkmn, MON_DATA_PP1 + r5, NULL);
                                r4 = GetMonData(pkmn, MON_DATA_MOVE1 + r5, NULL);
                                if (data != CalculatePPWithBonus(r4, GetMonData(pkmn, MON_DATA_PP_BONUSES, NULL), r5))
                                {
                                    data += itemEffect[sp24];
                                    r4 = GetMonData(pkmn, MON_DATA_MOVE1 + r5, NULL);
                                    if (data > CalculatePPWithBonus(r4, GetMonData(pkmn, MON_DATA_PP_BONUSES, NULL), r5))
                                    {
                                        r4 = GetMonData(pkmn, MON_DATA_MOVE1 + r5, NULL);
                                        data = CalculatePPWithBonus(r4, GetMonData(pkmn, MON_DATA_PP_BONUSES, NULL), r5);
                                    }
                                    SetMonData(pkmn, MON_DATA_PP1 + r5, &data);
                                    if (gMain.inBattle
                                     && sp34 != 4 && !(gBattleMons[sp34].status2 & 0x200000)
                                     && !(gDisableStructs[sp34].unk18_b & gBitTable[r5]))
                                        gBattleMons[sp34].pp[r5] = data;
                                    retVal = FALSE;
                                }
                            }
                            sp24++;
                        }
                        else // if not healing all the moves
                        {
                            u16 r4;

                            data = GetMonData(pkmn, MON_DATA_PP1 + moveIndex, NULL);
                            r4 = GetMonData(pkmn, MON_DATA_MOVE1 + moveIndex, NULL);
                            if (data != CalculatePPWithBonus(r4, GetMonData(pkmn, MON_DATA_PP_BONUSES, NULL), moveIndex))
                            {
                                data += itemEffect[sp24++];
                                r4 = GetMonData(pkmn, MON_DATA_MOVE1 + moveIndex, NULL);
                                if (data > CalculatePPWithBonus(r4, GetMonData(pkmn, MON_DATA_PP_BONUSES, NULL), moveIndex))
                                {
                                    r4 = GetMonData(pkmn, MON_DATA_MOVE1 + moveIndex, NULL);
                                    data = CalculatePPWithBonus(r4, GetMonData(pkmn, MON_DATA_PP_BONUSES, NULL), moveIndex);
                                }
                                SetMonData(pkmn, MON_DATA_PP1 + moveIndex, &data);
                                if (gMain.inBattle
                                 && sp34 != 4 && !(gBattleMons[sp34].status2 & 0x200000)
                                 && !(gDisableStructs[sp34].unk18_b & gBitTable[moveIndex]))
                                    gBattleMons[sp34].pp[moveIndex] = data;
                                retVal = FALSE;
                            }
                        }
                        break;
                    // evolution stones
                    case 7:
                        {
                            u16 targetSpecies = GetEvolutionTargetSpecies(pkmn, 2, item);

                            if (targetSpecies != SPECIES_NONE)
                            {
                                BeginEvolutionScene(pkmn, targetSpecies, 0, partyIndex);
                                return FALSE;
                            }
                        }
                        break;
                    }
                }
                sp28++;
                r10 >>= 1;
            }
            break;
        case 5:
            r10 = itemEffect[cmdIndex];
            sp28 = 0;
            while (r10 != 0)
            {
                if (r10 & 1)
                {
                    switch (sp28)
                    {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                        break;
                    case 4:
                        data = (GetMonData(pkmn, MON_DATA_PP_BONUSES, NULL) & gPPUpReadMasks[moveIndex]) >> (moveIndex * 2);
                        if (data < 3)
                        {
                            r4 = CalculatePPWithBonus(GetMonData(pkmn, MON_DATA_MOVE1 + moveIndex, NULL), GetMonData(pkmn, MON_DATA_PP_BONUSES, NULL), moveIndex);
                            data = GetMonData(pkmn, MON_DATA_PP_BONUSES, NULL);
                            data &= gPPUpWriteMasks[moveIndex];
                            data += gPPUpValues[moveIndex] * 3;

                            SetMonData(pkmn, MON_DATA_PP_BONUSES, &data);
                            data = CalculatePPWithBonus(GetMonData(pkmn, MON_DATA_MOVE1 + moveIndex, NULL), data, moveIndex) - r4;
                            data = GetMonData(pkmn, MON_DATA_PP1 + moveIndex, NULL) + data;
                            SetMonData(pkmn, MON_DATA_PP1 + moveIndex, &data);
                            retVal = FALSE;
                        }
                        break;
                    case 5: // bad friendship
                        if (GetMonData(pkmn, MON_DATA_FRIENDSHIP, NULL) < 100 && retVal == 0 && sp2C == 0)
                        {
                            sp2C = itemEffect[sp24];
                            friendship = GetMonData(pkmn, MON_DATA_FRIENDSHIP, NULL);
                            if (sp2C > 0 && holdEffect == HOLD_EFFECT_HAPPINESS_UP)
                                friendship += 150 * sp2C / 100;
                            else
                                friendship += sp2C;
                            if (sp2C > 0)
                            {
                                if (GetMonData(pkmn, MON_DATA_POKEBALL, NULL) == 11)
                                    friendship++;
                                if (GetMonData(pkmn, MON_DATA_MET_LOCATION, NULL) == sav1_map_get_name())
                                    friendship++;
                            }
                            if (friendship < 0)
                                friendship = 0;
                            if (friendship > 255)
                                friendship = 255;
                            SetMonData(pkmn, MON_DATA_FRIENDSHIP, &friendship);
                        }
                        sp24++;
                        break;
                    case 6: // good friendship
                        if (GetMonData(pkmn, MON_DATA_FRIENDSHIP, NULL) >= 100 && GetMonData(pkmn, MON_DATA_FRIENDSHIP, NULL) < 200
                         && retVal == 0 && sp2C == 0)
                        {
                            sp2C = itemEffect[sp24];
                            friendship = GetMonData(pkmn, MON_DATA_FRIENDSHIP, NULL);
                            if (sp2C > 0 && holdEffect == HOLD_EFFECT_HAPPINESS_UP)
                                friendship += 150 * sp2C / 100;
                            else
                                friendship += sp2C;
                            if (sp2C > 0)
                            {
                                if (GetMonData(pkmn, MON_DATA_POKEBALL, NULL) == 11)
                                    friendship++;
                                if (GetMonData(pkmn, MON_DATA_MET_LOCATION, NULL) == sav1_map_get_name())
                                    friendship++;
                            }
                            if (friendship < 0)
                                friendship = 0;
                            if (friendship > 255)
                                friendship = 255;
                            SetMonData(pkmn, MON_DATA_FRIENDSHIP, &friendship);
                        }
                        sp24++;
                        break;
                    case 7: // great friendship
                        if (GetMonData(pkmn, MON_DATA_FRIENDSHIP, NULL) >= 200 && retVal == 0 && sp2C == 0)
                        {
                            sp2C = itemEffect[sp24];
                            friendship = GetMonData(pkmn, MON_DATA_FRIENDSHIP, NULL);
                            if (sp2C > 0 && holdEffect == HOLD_EFFECT_HAPPINESS_UP)
                                friendship += 150 * sp2C / 100;
                            else
                                friendship += sp2C;
                            if (sp2C > 0)
                            {
                                if (GetMonData(pkmn, MON_DATA_POKEBALL, NULL) == 11)
                                    friendship++;
                                if (GetMonData(pkmn, MON_DATA_MET_LOCATION, NULL) == sav1_map_get_name())
                                    friendship++;
                            }
                            if (friendship < 0)
                                friendship = 0;
                            if (friendship > 255)
                                friendship = 255;
                            SetMonData(pkmn, MON_DATA_FRIENDSHIP, &friendship);
                        }
                        sp24++;
                        break;
                    }
                }
                sp28++;
                r10 >>= 1;
            }
            break;
        }
    }
    return retVal;
}
