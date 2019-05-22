#include "global.h"
#include "battle_tower.h"
#include "start_menu.h"
#include "event_data.h"
#include "field_player_avatar.h"
#include "field_weather.h"
#include "fieldmap.h"
#include "item_menu.h"
#include "load_save.h"
#include "m4a.h"
#include "main.h"
#include "event_obj_lock.h"
#include "menu.h"
#include "new_game.h"
#include "option_menu.h"
#include "palette.h"
#include "pokedex.h"
#include "pokemon_menu.h"
#include "pokenav.h"
#include "overworld.h"
#include "safari_zone.h"
#include "save.h"
#include "save_menu_util.h"
#include "script.h"
#include "constants/songs.h"
#include "sound.h"
#include "sprite.h"
#include "string_util.h"
#include "strings.h"
#include "strings2.h"
#include "task.h"
#include "trainer_card.h"
#include "scanline_effect.h"
#include "graphics.h"

//Menu actions
enum {
    MENU_ACTION_POKEDEX,
    MENU_ACTION_POKEMON,
    MENU_ACTION_BAG,
    MENU_ACTION_PLAYER,
    MENU_ACTION_SAVE,
    MENU_ACTION_OPTION,
    MENU_ACTION_EXIT,
    MENU_ACTION_RETIRE,
    MENU_ACTION_PLAYER_LINK
};

#if DEBUG
static u32 _debugStartMenu_0  __attribute__((unused));
static u32 _debugStartMenu_1 __attribute__((unused));
#endif

static u8 (*saveDialogCallback)(void);
static u8 saveDialogTimer;    //Number of frames to keep the window on screen after save was completed
static bool8 savingComplete;

extern bool8 gDifferentSaveFile;
extern u16 gSaveFileStatus;
extern u16 gSpecialVar_Result;

extern u8 gUnknown_03004860;

u8 (*gMenuCallback)(void);

EWRAM_DATA static u8 sStartMenuCursorPos = 0;
EWRAM_DATA static u8 sNumStartMenuActions = 0;
EWRAM_DATA static u8 sCurrentStartMenuActions[10] = {0};

//Text strings
extern u8 gSaveText_PlayerSavedTheGame[];
extern u8 gSaveText_DontTurnOff[];
extern u8 gSaveText_ThereIsAlreadyAFile[];
extern u8 gSaveText_ThereIsADifferentFile[];
extern u8 gSaveText_WouldYouLikeToSave[];

//Callbacks

static u8 StartMenu_PokedexCallback(void);
static u8 StartMenu_PokemonCallback(void);
static u8 StartMenu_BagCallback(void);
static u8 StartMenu_PlayerCallback(void);
static u8 StartMenu_SaveCallback(void);
static u8 StartMenu_OptionCallback(void);
static u8 StartMenu_ExitCallback(void);
static u8 StartMenu_RetireCallback(void);
static u8 StartMenu_PlayerLinkCallback(void);

static const struct MenuAction sStartMenuItems[] =
{
    { SystemText_Pokedex, StartMenu_PokedexCallback },
    { SystemText_Pokemon, StartMenu_PokemonCallback },
    { SystemText_BAG, StartMenu_BagCallback },
    { SystemText_Player, StartMenu_PlayerCallback },
    { SystemText_Save, StartMenu_SaveCallback },
    { SystemText_Option, StartMenu_OptionCallback },
    { SystemText_Exit, StartMenu_ExitCallback },
    { SystemText_Retire, StartMenu_RetireCallback },
    { SystemText_Player, StartMenu_PlayerLinkCallback },
};

//Private functions
static void BuildStartMenuActions(void);
static void AddStartMenuAction(u8 action);
static void BuildStartMenuActions_Normal(void);
static void BuildStartMenuActions_SafariZone(void);
static void BuildStartMenuActions_Link(void);
static void DisplaySafariBallsWindow(void);
static bool32 PrintStartMenuItemsMultistep(s16 *a, u32 b);
static bool32 InitStartMenuMultistep(s16 *a, s16 *b);
static void Task_StartMenu(u8 taskId);
static u8 StartMenu_InputProcessCallback(void);
static u8 SaveCallback1(void);
static u8 SaveCallback2(void);
static void sub_807160C(void);
static u8 RunSaveDialogCallback(void);
static void DisplaySaveMessageWithCallback(const u8 *ptr, u8 (*func)(void));
static void Task_SaveDialog(u8 taskId);
static void sub_8071700(void);
static void HideSaveDialog(void);
static void SaveDialogStartTimeout(void);
static u8 SaveDialogCheckForTimeoutOrKeypress(void);
static u8 SaveDialogCheckForTimeoutAndKeypress(void);
static u8 SaveDialogCheckForTimeoutAndKeypress(void);
static u8 SaveDialogCB_DisplayConfirmMessage(void);
static u8 SaveDialogCB_DisplayConfirmYesNoMenu(void);
static u8 SaveDialogCB_ProcessConfirmYesNoMenu(void);
static u8 SaveDialogCB_SaveFileExists(void);
static u8 SaveDialogCB_DisplayOverwriteYesNoMenu(void);
static u8 SaveDialogCB_ProcessOverwriteYesNoMenu(void);
static u8 SaveDialogCB_DisplaySavingMessage(void);
static u8 SaveDialogCB_DoSave(void);
static u8 SaveDialogCB_SaveSuccess(void);
static u8 SaveDialogCB_ReturnSuccess(void);
static u8 SaveDialogCB_SaveError(void);
static u8 SaveDialogCB_ReturnError(void);
static void sub_80719F0(void);
static bool32 sub_80719FC(u8 *ptr);
static void sub_8071B54(void);
static void Task_8071B64(u8 taskId);

static void ArrowCallback(struct Sprite *sprite);

static EWRAM_DATA u8 iconDex = 0;
static EWRAM_DATA u8 iconPoke = 0;
static EWRAM_DATA u8 iconBag = 0;
static EWRAM_DATA u8 iconTCard = 0;
static EWRAM_DATA u8 iconSave = 0;
static EWRAM_DATA u8 iconOptions = 0;
static EWRAM_DATA u8 iconReturn = 0;
static EWRAM_DATA u8 iconArrow = 0;
static EWRAM_DATA u8 iconCircle = 0;
static EWRAM_DATA u8 iconDexBW = 0;
static EWRAM_DATA u8 iconPokeBW = 0;
static EWRAM_DATA u8 iconBagBW = 0;
static EWRAM_DATA u8 iconTCardBW = 0;
static EWRAM_DATA u8 iconSaveBW = 0;
static EWRAM_DATA u8 iconOptionsBW = 0;
static EWRAM_DATA u8 iconReturnBW = 0;

extern const struct SpriteSheet spriteSheetDex;
extern const struct SpritePalette spritePaletteDex;
extern const struct SpriteTemplate spriteTemplateDex;
extern const struct SpriteSheet spriteSheetDexBW;
extern const struct SpritePalette spritePaletteDexBW;
extern const struct SpriteTemplate spriteTemplateDexBW;
extern const struct SpriteSheet spriteSheetPoke;
extern const struct SpritePalette spritePalettePoke;
extern const struct SpriteTemplate spriteTemplatePoke;
extern const struct SpriteSheet spriteSheetPokeBW;
extern const struct SpritePalette spritePalettePokeBW;
extern const struct SpriteTemplate spriteTemplatePokeBW;
extern const struct SpriteSheet spriteSheetBag;
extern const struct SpritePalette spritePaletteBag;
extern const struct SpriteTemplate spriteTemplateBag;
extern const struct SpriteSheet spriteSheetBagBW;
extern const struct SpritePalette spritePaletteBagBW;
extern const struct SpriteTemplate spriteTemplateBagBW;
extern const struct SpriteSheet spriteSheetTCard;
extern const struct SpritePalette spritePaletteTCard;
extern const struct SpriteTemplate spriteTemplateTCard;
extern const struct SpriteSheet spriteSheetTCardBW;
extern const struct SpritePalette spritePaletteTCardBW;
extern const struct SpriteTemplate spriteTemplateTCardBW;
extern const struct SpriteSheet spriteSheetSave;
extern const struct SpritePalette spritePaletteSave;
extern const struct SpriteTemplate spriteTemplateSave;
extern const struct SpriteSheet spriteSheetSaveBW;
extern const struct SpritePalette spritePaletteSaveBW;
extern const struct SpriteTemplate spriteTemplateSaveBW;
extern const struct SpriteSheet spriteSheetOptions;
extern const struct SpritePalette spritePaletteOptions;
extern const struct SpriteTemplate spriteTemplateOptions;
extern const struct SpriteSheet spriteSheetOptionsBW;
extern const struct SpritePalette spritePaletteOptionsBW;
extern const struct SpriteTemplate spriteTemplateOptionsBW;
extern const struct SpriteSheet spriteSheetReturn;
extern const struct SpritePalette spritePaletteReturn;
extern const struct SpriteTemplate spriteTemplateReturn;
extern const struct SpriteSheet spriteSheetReturnBW;
extern const struct SpritePalette spritePaletteReturnBW;
extern const struct SpriteTemplate spriteTemplateReturnBW;
extern const struct SpriteSheet spriteSheetCircle;
extern const struct SpritePalette spritePaletteCircle;
extern const struct SpriteTemplate spriteTemplateCircle;
extern const struct SpriteSheet spriteSheetArrow;
extern const struct SpritePalette spritePaletteArrow;
extern const struct SpriteTemplate spriteTemplateArrow;


#if DEBUG

void debug_sub_8075D9C(void);

u8 debug_sub_8075C30(void)
{
    CloseMenu();
    debug_sub_8075D9C();
    return 1;
}

extern const u8 gUnknown_Debug_839B6D8[];

void debug_sub_8075C40(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    u16 savedIme;
    s32 i;

    switch (data[0])
    {
    case 0:
        m4aSoundVSyncOff();
        data[0]++;
        break;
    case 1:
        savedIme = REG_IME;
        REG_IME = 0;
        REG_TM2CNT_L = 0;
        REG_TM2CNT = 0x830000;
        for (i = 0; i < 0xC350; i++)
        {
            DmaSet(
                0,
                gScanlineEffectRegBuffers,
                &REG_WIN0H,
                ((DMA_ENABLE | DMA_START_HBLANK | DMA_16BIT | DMA_REPEAT | DMA_SRC_INC | DMA_DEST_RELOAD) << 16) | 1);
            DmaStop(0);
        }
        REG_TM2CNT_H = 0;
        _debugStartMenu_0 = REG_TM2CNT_L;
        REG_TM2CNT_L = 0;
        REG_IME = savedIme;
        _debugStartMenu_1 = i;
        m4aSoundVSyncOn();
        data[0]++;
        break;
    case 2:
        PlaySE(0x15);
        ConvertIntToDecimalStringN(gStringVar1, _debugStartMenu_1, 1, 8);
        ConvertIntToDecimalStringN(gStringVar2, _debugStartMenu_0, 1, 8);
        Menu_DisplayDialogueFrame();
        Menu_PrintText(gUnknown_Debug_839B6D8, 2, 15);
        data[0]++;
        break;
    case 3:
        if (gMain.newKeys & A_BUTTON)
        {
            Menu_EraseScreen();
            ScriptContext2_Disable();
            DestroyTask(taskId);
        }
        break;
    }
}

void debug_sub_8075D9C(void)
{
    CreateTask(debug_sub_8075C40, 10);
    ScriptContext2_Enable();
}

void debug_sub_8075DB4(struct BattleTowerEReaderTrainer *ereaderTrainer, const u8 *b, u32 trainerId)
{
    s32 i;
    s32 r3;

    ereaderTrainer->trainerClass = trainerId % 77;
    write_word_to_mem(trainerId, ereaderTrainer->trainerId);
    StringCopy8(ereaderTrainer->name, b);
    r3 = 7;
    for (i = 0; i < 6; i++)
    {
        ereaderTrainer->greeting[i] = gSaveBlock1.easyChats.unk2B28[i];
        ereaderTrainer->farewellPlayerLost[i] = r3;
        ereaderTrainer->farewellPlayerWon[i] = r3 + 6;
        r3++;
    }
    for (i = 0; i < 3; i++)
        sub_803AF78(&gPlayerParty[i], &ereaderTrainer->party[i]);
    SetEReaderTrainerChecksum(ereaderTrainer);
}

void unref_sub_8070F90(void)
{
    FlagSet(FLAG_SYS_POKEDEX_GET);
    FlagSet(FLAG_SYS_POKEMON_GET);
    FlagSet(FLAG_SYS_POKENAV_GET);
}

#endif

static const struct OamData gIconOamData =
{
    .y = 0,
    .affineMode = 0,
    .objMode = 0, 
    .mosaic = 0,
    .bpp = 0,
    .shape = 0,
    .x = 0,
    .matrixNum = 0,
    .size = 2,
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0, 
};

static const struct OamData gIconOamData16 =
{
    .y = 0,
    .affineMode = 0,
    .objMode = 0, 
    .mosaic = 0,
    .bpp = 0,
    .shape = 0,
    .x = 0,
    .matrixNum = 0,
    .size = 1,
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0, 
};

static const struct OamData gIconRotateOamData =
{
    .y = 0,
    .affineMode = 1,
    .objMode = 0, 
    .mosaic = 0, 
    .bpp = 0,
    .shape = 0, 
    .x = 0, 
    .matrixNum = 0,
    .size = 2,
    .tileNum = 0,
    .priority = 0, 
    .paletteNum = 0, 
    .affineParam = 0, 
};

static const struct OamData gCircleOamData =
{
    .y = 0,
    .affineMode = 0,
    .objMode = 0, 
    .mosaic = 0,
    .bpp = 0,
    .shape = 0,
    .x = 0,
    .matrixNum = 0,
    .size = 2,
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0, 
};

static const union AnimCmd gIconAnimSeq0[] =
{
    ANIMCMD_FRAME(0, 5),
    ANIMCMD_END,
};

static const union AnimCmd *const gIconAnimTable[] =
{
    gIconAnimSeq0,
};

static const union AffineAnimCmd gRotateAnim[] =
{
    AFFINEANIMCMD_FRAME(0, 0, 4, 5),
    AFFINEANIMCMD_FRAME(0, 0, -4, 5),
    AFFINEANIMCMD_FRAME(0, 0, 4, 5),
    AFFINEANIMCMD_FRAME(0, 0, -4, 5),
    AFFINEANIMCMD_FRAME(0, 0, 4, 5),
    AFFINEANIMCMD_FRAME(0, 0, -4, 5),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const gRotateTable[] =
{
    gRotateAnim,
};

static const union AnimCmd gArrowAnimSeq0[] =
{
    ANIMCMD_FRAME(0, 6),
    ANIMCMD_FRAME(4, 6),
    ANIMCMD_FRAME(8, 6),
    ANIMCMD_FRAME(4, 6),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd gArrowAnimSeq1[] =
{
    ANIMCMD_FRAME(0, 6),
    ANIMCMD_END,
};

static const union AnimCmd *const gArrowAnimTable[] =
{
    gArrowAnimSeq0,
    gArrowAnimSeq1,
};

/*const struct SpriteSheet gIcons_SpriteSheet[] = {
    { gMenuIcons_Dex, 0x200, 0 },
    { gMenuIcons_DexBW, 0x200, 1 },
    { gMenuIcons_Pokemon, 0x200, 2 },
    { gMenuIcons_PokemonBW, 0x200, 3 },
    { gMenuIcons_Bag, 0x200, 4 },
    { gMenuIcons_BagBW, 0x200, 5 },
    { gMenuIcons_TCard, 0x200, 6 },
    { gMenuIcons_TCardBW, 0x200, 7 },
    { gMenuIcons_Save, 0x200, 8 },
    { gMenuIcons_SaveBW, 0x200, 9 },
    { gMenuIcons_Options, 0x200, 10 },
    { gMenuIcons_OptionsBW, 0x200, 11 },
    { gMenuIcons_Return, 0x200, 12 },
    { gMenuIcons_ReturnBW, 0x200, 13 },
    { gMenuIcons_Circle, 0x200, 14 },
    { gMenuIcons_Arrow, 0x180, 15 },
    {}
};*/


//INDIVIDUAL ICONS SHEETS

//DEX

    struct SpriteSheet spriteSheetDex =
    {
                .data = gMenuIcons_Dex, //GRÁFICO ----------
                .size = 512, //TAMAÑO DEL GRÁFICO
                .tag = 70, //LUGAR DONDE SE CARGA EL GRÁFICO ----------
    };
    struct SpritePalette spritePaletteDex =
    {
                .data = gMenuIcons_Palette,
                .tag = 70, //LUGAR DONDE SE CARGA LA PALETA ----------
    };
    struct SpriteTemplate spriteTemplateDex =
    {
                .tileTag = 70, //LUGAR DONDE SE CARGA EL GRÁFICO ----------
                .paletteTag = 70, //LUGAR DONDE SE CARGA LA PALETA ----------
                .oam = &gIconRotateOamData, //OAM DATA DEL ICONO ----------
                .anims = gIconAnimTable, //TABLA DE ANIMACIÓN DEL ICONO ---------- 
                .images = NULL,
                .affineAnims = gRotateTable, 
                .callback = SpriteCallbackDummy, //ANIMACIÓN DEL ICONO ----------
    };

//DEXBW

    struct SpriteSheet spriteSheetDexBW =
    {
                .data = gMenuIcons_DexBW, //GRÁFICO ----------
                .size = 512, //TAMAÑO DEL GRÁFICO
                .tag = 71, //LUGAR DONDE SE CARGA EL GRÁFICO ----------
    };
    struct SpritePalette spritePaletteDexBW =
    {
                .data = gMenuIcons_Palette,
                .tag = 71, //LUGAR DONDE SE CARGA LA PALETA ----------
    };
    struct SpriteTemplate spriteTemplateDexBW =
    {
                .tileTag = 71, //LUGAR DONDE SE CARGA EL GRÁFICO ----------
                .paletteTag = 71, //LUGAR DONDE SE CARGA LA PALETA ----------
                .oam = &gIconOamData, //OAM DATA DEL ICONO ----------
                .anims = gIconAnimTable, //TABLA DE ANIMACIÓN DEL ICONO ---------- 
                .images = NULL,
                .affineAnims = gDummySpriteAffineAnimTable, 
                .callback = SpriteCallbackDummy, //ANIMACIÓN DEL ICONO ----------
    };

//POKE

    struct SpriteSheet spriteSheetPoke =
    {
                .data = gMenuIcons_Pokemon, //GRÁFICO ----------
                .size = 512, //TAMAÑO DEL GRÁFICO
                .tag = 72, //LUGAR DONDE SE CARGA EL GRÁFICO ----------
    };
    struct SpritePalette spritePalettePoke =
    {
                .data = gMenuIcons_Palette,
                .tag = 72, //LUGAR DONDE SE CARGA LA PALETA ----------
    };
    struct SpriteTemplate spriteTemplatePoke =
    {
                .tileTag = 72, //LUGAR DONDE SE CARGA EL GRÁFICO ----------
                .paletteTag = 72, //LUGAR DONDE SE CARGA LA PALETA ----------
                .oam = &gIconRotateOamData, //OAM DATA DEL ICONO ----------
                .anims = gIconAnimTable, //TABLA DE ANIMACIÓN DEL ICONO ---------- 
                .images = NULL,
                .affineAnims = gRotateTable, 
                .callback = SpriteCallbackDummy, //ANIMACIÓN DEL ICONO ----------
    };

//POKEBW

    struct SpriteSheet spriteSheetPokeBW =
    {
                .data = gMenuIcons_PokemonBW, //GRÁFICO ----------
                .size = 512, //TAMAÑO DEL GRÁFICO
                .tag = 73, //LUGAR DONDE SE CARGA EL GRÁFICO ----------
    };
    struct SpritePalette spritePalettePokeBW =
    {
                .data = gMenuIcons_Palette,
                .tag = 73, //LUGAR DONDE SE CARGA LA PALETA ----------
    };
    struct SpriteTemplate spriteTemplatePokeBW =
    {
                .tileTag = 73, //LUGAR DONDE SE CARGA EL GRÁFICO ----------
                .paletteTag = 73, //LUGAR DONDE SE CARGA LA PALETA ----------
                .oam = &gIconOamData, //OAM DATA DEL ICONO ----------
                .anims = gIconAnimTable, //TABLA DE ANIMACIÓN DEL ICONO ---------- 
                .images = NULL,
                .affineAnims = gDummySpriteAffineAnimTable, 
                .callback = SpriteCallbackDummy, //ANIMACIÓN DEL ICONO ----------
    };

//BAG

    struct SpriteSheet spriteSheetBag =
    {
                .data = gMenuIcons_Bag, //GRÁFICO ----------
                .size = 512, //TAMAÑO DEL GRÁFICO
                .tag = 74, //LUGAR DONDE SE CARGA EL GRÁFICO ----------
    };
    struct SpritePalette spritePaletteBag =
    {
                .data = gMenuIcons_Palette,
                .tag = 74, //LUGAR DONDE SE CARGA LA PALETA ----------
    };
    struct SpriteTemplate spriteTemplateBag =
    {
                .tileTag = 74, //LUGAR DONDE SE CARGA EL GRÁFICO ----------
                .paletteTag = 74, //LUGAR DONDE SE CARGA LA PALETA ----------
                .oam = &gIconRotateOamData, //OAM DATA DEL ICONO ----------
                .anims = gIconAnimTable, //TABLA DE ANIMACIÓN DEL ICONO ---------- 
                .images = NULL,
                .affineAnims = gRotateTable, 
                .callback = SpriteCallbackDummy, //ANIMACIÓN DEL ICONO ----------
    };

//BAGBW

    struct SpriteSheet spriteSheetBagBW =
    {
                .data = gMenuIcons_BagBW, //GRÁFICO ----------
                .size = 512, //TAMAÑO DEL GRÁFICO
                .tag = 75, //LUGAR DONDE SE CARGA EL GRÁFICO ----------
    };
    struct SpritePalette spritePaletteBagBW =
    {
                .data = gMenuIcons_Palette,
                .tag = 75, //LUGAR DONDE SE CARGA LA PALETA ----------
    };
    struct SpriteTemplate spriteTemplateBagBW =
    {
                .tileTag = 75, //LUGAR DONDE SE CARGA EL GRÁFICO ----------
                .paletteTag = 75, //LUGAR DONDE SE CARGA LA PALETA ----------
                .oam = &gIconOamData, //OAM DATA DEL ICONO ----------
                .anims = gIconAnimTable, //TABLA DE ANIMACIÓN DEL ICONO ---------- 
                .images = NULL,
                .affineAnims = gDummySpriteAffineAnimTable, 
                .callback = SpriteCallbackDummy, //ANIMACIÓN DEL ICONO ----------
    };

//TCARD

    struct SpriteSheet spriteSheetTCard =
    {
                .data = gMenuIcons_TCard, //GRÁFICO ----------
                .size = 512, //TAMAÑO DEL GRÁFICO
                .tag = 76, //LUGAR DONDE SE CARGA EL GRÁFICO ----------
    };
    struct SpritePalette spritePaletteTCard =
    {
                .data = gMenuIcons_Palette,
                .tag = 76, //LUGAR DONDE SE CARGA LA PALETA ----------
    };
    struct SpriteTemplate spriteTemplateTCard =
    {
                .tileTag = 76, //LUGAR DONDE SE CARGA EL GRÁFICO ----------
                .paletteTag = 76, //LUGAR DONDE SE CARGA LA PALETA ----------
                .oam = &gIconRotateOamData, //OAM DATA DEL ICONO ----------
                .anims = gIconAnimTable, //TABLA DE ANIMACIÓN DEL ICONO ---------- 
                .images = NULL,
                .affineAnims = gRotateTable, 
                .callback = SpriteCallbackDummy, //ANIMACIÓN DEL ICONO ----------
    };

//TCARDBW

    struct SpriteSheet spriteSheetTCardBW =
    {
                .data = gMenuIcons_TCardBW, //GRÁFICO ----------
                .size = 512, //TAMAÑO DEL GRÁFICO
                .tag = 77, //LUGAR DONDE SE CARGA EL GRÁFICO ----------
    };
    struct SpritePalette spritePaletteTCardBW =
    {
                .data = gMenuIcons_Palette,
                .tag = 77, //LUGAR DONDE SE CARGA LA PALETA ----------
    };
    struct SpriteTemplate spriteTemplateTCardBW =
    {
                .tileTag = 77, //LUGAR DONDE SE CARGA EL GRÁFICO ----------
                .paletteTag = 77, //LUGAR DONDE SE CARGA LA PALETA ----------
                .oam = &gIconOamData, //OAM DATA DEL ICONO ----------
                .anims = gIconAnimTable, //TABLA DE ANIMACIÓN DEL ICONO ---------- 
                .images = NULL,
                .affineAnims = gDummySpriteAffineAnimTable, 
                .callback = SpriteCallbackDummy, //ANIMACIÓN DEL ICONO ----------
    };

//SAVE

    struct SpriteSheet spriteSheetSave =
    {
                .data = gMenuIcons_Save, //GRÁFICO ----------
                .size = 512, //TAMAÑO DEL GRÁFICO
                .tag = 78, //LUGAR DONDE SE CARGA EL GRÁFICO ----------
    };
    struct SpritePalette spritePaletteSave =
    {
                .data = gMenuIcons_Palette,
                .tag = 78, //LUGAR DONDE SE CARGA LA PALETA ----------
    };
    struct SpriteTemplate spriteTemplateSave =
    {
                .tileTag = 78, //LUGAR DONDE SE CARGA EL GRÁFICO ----------
                .paletteTag = 78, //LUGAR DONDE SE CARGA LA PALETA ----------
                .oam = &gIconRotateOamData, //OAM DATA DEL ICONO ----------
                .anims = gIconAnimTable, //TABLA DE ANIMACIÓN DEL ICONO ---------- 
                .images = NULL,
                .affineAnims = gRotateTable, 
                .callback = SpriteCallbackDummy, //ANIMACIÓN DEL ICONO ----------
    };

//SAVEBW

    struct SpriteSheet spriteSheetSaveBW =
    {
                .data = gMenuIcons_SaveBW, //GRÁFICO ----------
                .size = 512, //TAMAÑO DEL GRÁFICO
                .tag = 79, //LUGAR DONDE SE CARGA EL GRÁFICO ----------
    };
    struct SpritePalette spritePaletteSaveBW =
    {
                .data = gMenuIcons_Palette,
                .tag = 79, //LUGAR DONDE SE CARGA LA PALETA ----------
    };
    struct SpriteTemplate spriteTemplateSaveBW =
    {
                .tileTag = 79, //LUGAR DONDE SE CARGA EL GRÁFICO ----------
                .paletteTag = 79, //LUGAR DONDE SE CARGA LA PALETA ----------
                .oam = &gIconOamData, //OAM DATA DEL ICONO ----------
                .anims = gIconAnimTable, //TABLA DE ANIMACIÓN DEL ICONO ---------- 
                .images = NULL,
                .affineAnims = gDummySpriteAffineAnimTable, 
                .callback = SpriteCallbackDummy, //ANIMACIÓN DEL ICONO ----------
    };

//OPTIONS

    struct SpriteSheet spriteSheetOptions =
    {
                .data = gMenuIcons_Options, //GRÁFICO ----------
                .size = 512, //TAMAÑO DEL GRÁFICO
                .tag = 80, //LUGAR DONDE SE CARGA EL GRÁFICO ----------
    };
    struct SpritePalette spritePaletteOptions =
    {
                .data = gMenuIcons_Palette,
                .tag = 80, //LUGAR DONDE SE CARGA LA PALETA ----------
    };
    struct SpriteTemplate spriteTemplateOptions =
    {
                .tileTag = 80, //LUGAR DONDE SE CARGA EL GRÁFICO ----------
                .paletteTag = 80, //LUGAR DONDE SE CARGA LA PALETA ----------
                .oam = &gIconRotateOamData, //OAM DATA DEL ICONO ----------
                .anims = gIconAnimTable, //TABLA DE ANIMACIÓN DEL ICONO ---------- 
                .images = NULL,
                .affineAnims = gRotateTable, 
                .callback = SpriteCallbackDummy, //ANIMACIÓN DEL ICONO ----------
    };

//OPTIONSBW

    struct SpriteSheet spriteSheetOptionsBW =
    {
                .data = gMenuIcons_OptionsBW, //GRÁFICO ----------
                .size = 512, //TAMAÑO DEL GRÁFICO
                .tag = 81, //LUGAR DONDE SE CARGA EL GRÁFICO ----------
    };
    struct SpritePalette spritePaletteOptionsBW =
    {
                .data = gMenuIcons_Palette,
                .tag = 81, //LUGAR DONDE SE CARGA LA PALETA ----------
    };
    struct SpriteTemplate spriteTemplateOptionsBW =
    {
                .tileTag = 81, //LUGAR DONDE SE CARGA EL GRÁFICO ----------
                .paletteTag = 81, //LUGAR DONDE SE CARGA LA PALETA ----------
                .oam = &gIconOamData, //OAM DATA DEL ICONO ----------
                .anims = gIconAnimTable, //TABLA DE ANIMACIÓN DEL ICONO ---------- 
                .images = NULL,
                .affineAnims = gDummySpriteAffineAnimTable, 
                .callback = SpriteCallbackDummy, //ANIMACIÓN DEL ICONO ----------
    };

//RETURN

    struct SpriteSheet spriteSheetReturn =
    {
                .data = gMenuIcons_Return, //GRÁFICO ----------
                .size = 512, //TAMAÑO DEL GRÁFICO
                .tag = 82, //LUGAR DONDE SE CARGA EL GRÁFICO ----------
    };
    struct SpritePalette spritePaletteReturn =
    {
                .data = gMenuIcons_Palette,
                .tag = 82, //LUGAR DONDE SE CARGA LA PALETA ----------
    };
    struct SpriteTemplate spriteTemplateReturn =
    {
                .tileTag = 82, //LUGAR DONDE SE CARGA EL GRÁFICO ----------
                .paletteTag = 82, //LUGAR DONDE SE CARGA LA PALETA ----------
                .oam = &gIconRotateOamData, //OAM DATA DEL ICONO ----------
                .anims = gIconAnimTable, //TABLA DE ANIMACIÓN DEL ICONO ---------- 
                .images = NULL,
                .affineAnims = gRotateTable, 
                .callback = SpriteCallbackDummy, //ANIMACIÓN DEL ICONO ----------
    };

//RETURNBW

    struct SpriteSheet spriteSheetReturnBW =
    {
                .data = gMenuIcons_ReturnBW, //GRÁFICO ----------
                .size = 512, //TAMAÑO DEL GRÁFICO
                .tag = 83, //LUGAR DONDE SE CARGA EL GRÁFICO ----------
    };
    struct SpritePalette spritePaletteReturnBW =
    {
                .data = gMenuIcons_Palette,
                .tag = 83, //LUGAR DONDE SE CARGA LA PALETA ----------
    };
    struct SpriteTemplate spriteTemplateReturnBW =
    {
                .tileTag = 83, //LUGAR DONDE SE CARGA EL GRÁFICO ----------
                .paletteTag = 83, //LUGAR DONDE SE CARGA LA PALETA ----------
                .oam = &gIconOamData, //OAM DATA DEL ICONO ----------
                .anims = gIconAnimTable, //TABLA DE ANIMACIÓN DEL ICONO ---------- 
                .images = NULL,
                .affineAnims = gDummySpriteAffineAnimTable, 
                .callback = SpriteCallbackDummy, //ANIMACIÓN DEL ICONO ----------
    };

//CIRCLE

    struct SpriteSheet spriteSheetCircle =
    {
                .data = gMenuIcons_Circle, //GRÁFICO ----------
                .size = 512, //TAMAÑO DEL GRÁFICO
                .tag = 84, //LUGAR DONDE SE CARGA EL GRÁFICO ----------
    };
    struct SpritePalette spritePaletteCircle =
    {
                .data = gMenuIcons_Palette,
                .tag = 84, //LUGAR DONDE SE CARGA LA PALETA ----------
    };
    struct SpriteTemplate spriteTemplateCircle =
    {
                .tileTag = 84, //LUGAR DONDE SE CARGA EL GRÁFICO ----------
                .paletteTag = 84, //LUGAR DONDE SE CARGA LA PALETA ----------
                .oam = &gCircleOamData, //OAM DATA DEL ICONO ----------
                .anims = gIconAnimTable, //TABLA DE ANIMACIÓN DEL ICONO ---------- 
                .images = NULL,
                .affineAnims = gDummySpriteAffineAnimTable, 
                .callback = SpriteCallbackDummy, //ANIMACIÓN DEL ICONO ----------
    };

//ARROW

    struct SpriteSheet spriteSheetArrow =
    {
                .data = gMenuIcons_Arrow, //GRÁFICO ----------
                .size = 0x200, //TAMAÑO DEL GRÁFICO
                .tag = 85, //LUGAR DONDE SE CARGA EL GRÁFICO ----------
    };
    struct SpritePalette spritePaletteArrow =
    {
                .data = gMenuIcons_Palette,
                .tag = 85, //LUGAR DONDE SE CARGA LA PALETA ----------
    };
    struct SpriteTemplate spriteTemplateArrow =
    {
                .tileTag = 85, //LUGAR DONDE SE CARGA EL GRÁFICO ----------
                .paletteTag = 85, //LUGAR DONDE SE CARGA LA PALETA ----------
                .oam = &gIconOamData16, //OAM DATA DEL ICONO ----------
                .anims = gArrowAnimTable, //TABLA DE ANIMACIÓN DEL ICONO ---------- 
                .images = NULL,
                .affineAnims = gDummySpriteAffineAnimTable, 
                .callback = ArrowCallback, //ANIMACIÓN DEL ICONO ----------
    };




/*
const struct SpritePalette gIcons_SpritePalette[] = {
    { gMenuIcons_Palette, 0 },
    {}
};*/
/*
const struct SpriteTemplate spriteTemplateDex = {
    0, 0, &gIconRotateOamData, gIconAnimTable, NULL, gRotateTable, SpriteCallbackDummy
};
const struct SpriteTemplate spriteTemplateDexBW = {
    1, 0, &gIconOamData, gIconAnimTable, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
};
const struct SpriteTemplate spriteTemplatePoke = {
    2, 0, &gIconRotateOamData, gIconAnimTable, NULL, gRotateTable, SpriteCallbackDummy
};
const struct SpriteTemplate spriteTemplatePokeBW = {
    3, 0, &gIconOamData, gIconAnimTable, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
};
const struct SpriteTemplate spriteTemplateBag = {
    4, 0, &gIconRotateOamData, gIconAnimTable, NULL, gRotateTable, SpriteCallbackDummy
};
const struct SpriteTemplate spriteTemplateBagBW = {
    5, 0, &gIconOamData, gIconAnimTable, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
};
const struct SpriteTemplate spriteTemplateTCard = {
    6, 0, &gIconRotateOamData, gIconAnimTable, NULL, gRotateTable, SpriteCallbackDummy
};
const struct SpriteTemplate spriteTemplateTCardBW = {
    7, 0, &gIconOamData, gIconAnimTable, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
};
const struct SpriteTemplate spriteTemplateSave = {
    8, 0, &gIconRotateOamData, gIconAnimTable, NULL, gRotateTable, SpriteCallbackDummy
};
const struct SpriteTemplate spriteTemplateSaveBW = {
    9, 0, &gIconOamData, gIconAnimTable, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
};
const struct SpriteTemplate spriteTemplateOptions = {
    10, 0, &gIconRotateOamData, gIconAnimTable, NULL, gRotateTable, SpriteCallbackDummy
};
const struct SpriteTemplate spriteTemplateOptionsBW = {
    11, 0, &gIconOamData, gIconAnimTable, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
};
const struct SpriteTemplate spriteTemplateReturn = {
    12, 0, &gIconRotateOamData, gIconAnimTable, NULL, gRotateTable, SpriteCallbackDummy
};
const struct SpriteTemplate spriteTemplateReturnBW = {
    13, 0, &gIconOamData, gIconAnimTable, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
};
const struct SpriteTemplate spriteTemplateCircle = {
    14, 0, &gCircleOamData, gIconAnimTable, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
};
const struct SpriteTemplate spriteTemplateArrow = {
    15, 0, &gIconOamData16, gArrowAnimTable, NULL, gDummySpriteAffineAnimTable, ArrowCallback
};*/

void DrawIcon(u8 x, u8 y, u8 numberOfIcon)
{
    u8 taskId;
    u8 spriteId;

    //iconDex = CreateSprite(&spriteTemplate, x, y, 0); //X and Y being the x any y co-ordinates. The 0 is sub-sprite, which tbh, i wouldn't mess with.
    switch (numberOfIcon)
    {
        case 0:
            LoadSpriteSheet(&spriteSheetDex);
            LoadSpritePalette(&spritePaletteDex);
            iconDex = CreateSprite(&spriteTemplateDex, x, y, 0);
            break;
        case 1:
            LoadSpriteSheet(&spriteSheetPoke);
            LoadSpritePalette(&spritePalettePoke);
            iconPoke = CreateSprite(&spriteTemplatePoke, x, y, 0);
            break;
        case 2:
            LoadSpriteSheet(&spriteSheetBag);
            LoadSpritePalette(&spritePaletteBag);
            iconBag = CreateSprite(&spriteTemplateBag, x, y, 0);
            break;
        case 3:
            LoadSpriteSheet(&spriteSheetTCard);
            LoadSpritePalette(&spritePaletteTCard);
            iconTCard = CreateSprite(&spriteTemplateTCard, x, y, 0);
            break;
        case 4:
            LoadSpriteSheet(&spriteSheetSave);
            LoadSpritePalette(&spritePaletteSave);
            iconSave = CreateSprite(&spriteTemplateSave, x, y, 0);
            break;
        case 5:
            LoadSpriteSheet(&spriteSheetOptions);
            LoadSpritePalette(&spritePaletteOptions);
            iconOptions = CreateSprite(&spriteTemplateOptions, x, y, 0);
            break;
        case 6:
            LoadSpriteSheet(&spriteSheetReturn);
            LoadSpritePalette(&spritePaletteReturn);
            iconReturn = CreateSprite(&spriteTemplateReturn, x, y, 0);
            break;
        case 7:
            LoadSpriteSheet(&spriteSheetArrow);
            LoadSpritePalette(&spritePaletteArrow);
            spriteId = iconArrow = CreateSprite(&spriteTemplateArrow, x, y, 0);
            gSprites[spriteId].data[0] = taskId;
            gSprites[spriteId].data[1] = 0;
            break;
        case 8:
            LoadSpriteSheet(&spriteSheetCircle);
            LoadSpritePalette(&spritePaletteCircle);
            iconCircle = CreateSprite(&spriteTemplateCircle, x, y, 0);
            break;
        case 9:
            LoadSpriteSheet(&spriteSheetDexBW);
            LoadSpritePalette(&spritePaletteDexBW);
            iconDexBW = CreateSprite(&spriteTemplateDexBW, x, y, 0);
            break;
        case 10:
            LoadSpriteSheet(&spriteSheetPokeBW);
            LoadSpritePalette(&spritePalettePokeBW);
            iconPokeBW = CreateSprite(&spriteTemplatePokeBW, x, y, 0);
            break;
        case 11:
            LoadSpriteSheet(&spriteSheetBagBW);
            LoadSpritePalette(&spritePaletteBagBW);
            iconBagBW = CreateSprite(&spriteTemplateBagBW, x, y, 0);
            break;
        case 12:
            LoadSpriteSheet(&spriteSheetTCardBW);
            LoadSpritePalette(&spritePaletteTCardBW);
            iconTCardBW = CreateSprite(&spriteTemplateTCardBW, x, y, 0);
            break;
        case 13:
            LoadSpriteSheet(&spriteSheetSaveBW);
            LoadSpritePalette(&spritePaletteSaveBW);
            iconSaveBW = CreateSprite(&spriteTemplateSaveBW, x, y, 0);
            break;
        case 14:
            LoadSpriteSheet(&spriteSheetOptionsBW);
            LoadSpritePalette(&spritePaletteOptionsBW);
            iconOptionsBW = CreateSprite(&spriteTemplateOptionsBW, x, y, 0);
            break;
        case 15:
            LoadSpriteSheet(&spriteSheetReturnBW);
            LoadSpritePalette(&spritePaletteReturnBW);
            iconReturnBW = CreateSprite(&spriteTemplateReturnBW, x, y, 0);
            break;
        
    }
    //SetVBlankCallback(VblankCallback); //I dont know if this is needed or not.
}

void destroyIcons(void){
    if (iconDex != 0) DestroySpriteAndFreeResources(&gSprites[iconDex]);
    if (iconPoke != 0) DestroySpriteAndFreeResources(&gSprites[iconPoke]);
    if (iconBag != 0) DestroySpriteAndFreeResources(&gSprites[iconBag]);
    if (iconTCard != 0) DestroySpriteAndFreeResources(&gSprites[iconTCard]);
    if (iconSave != 0) DestroySpriteAndFreeResources(&gSprites[iconSave]);
    if (iconOptions != 0) DestroySpriteAndFreeResources(&gSprites[iconOptions]);
    if (iconReturn != 0) DestroySpriteAndFreeResources(&gSprites[iconReturn]);
    if (iconArrow != 0) DestroySpriteAndFreeResources(&gSprites[iconArrow]);
    if (iconCircle != 0) DestroySpriteAndFreeResources(&gSprites[iconCircle]);
    if (iconDexBW != 0) DestroySpriteAndFreeResources(&gSprites[iconDexBW]);
    if (iconPokeBW != 0) DestroySpriteAndFreeResources(&gSprites[iconPokeBW]);
    if (iconBagBW != 0) DestroySpriteAndFreeResources(&gSprites[iconBagBW]);
    if (iconTCardBW != 0) DestroySpriteAndFreeResources(&gSprites[iconTCardBW]);
    if (iconSaveBW != 0) DestroySpriteAndFreeResources(&gSprites[iconSaveBW]);
    if (iconOptionsBW != 0) DestroySpriteAndFreeResources(&gSprites[iconOptionsBW]);
    if (iconReturnBW != 0) DestroySpriteAndFreeResources(&gSprites[iconReturnBW]);
}

static u8 closeIconsAndMenu(void)
{
    REG_WIN0H = 240;
    REG_WIN0V = 160;
    REG_BLDY = 0x0;
    destroyIcons();
}

static void BuildStartMenuActions(void)
{
    sNumStartMenuActions = 0;
    if (is_c1_link_related_active() == TRUE)
        BuildStartMenuActions_Link();
    else
    {
        if (GetSafariZoneFlag() == TRUE)
            BuildStartMenuActions_SafariZone();
        else
            BuildStartMenuActions_Normal();
    }
}

#define GENERAL_Y 144
#define DEX_X 25
#define POKE_X 57
#define BAG_X 89
#define TCARD_X 121
#define SAVE_X 153
#define OPTIONS_X 185
#define RETURN_X 217

void drawMain(void)
{
    u8 startValue;
    
    startValue = sStartMenuCursorPos;

    if (FlagGet(FLAG_SYS_POKEDEX_GET) == TRUE){
        if (FlagGet(FLAG_SYS_POKEMON_GET) == TRUE){
            if(sStartMenuCursorPos == 0){
                DrawIcon(25, 144, sStartMenuCursorPos);
                sStartMenuCursorPos = 7;
                DrawIcon(25, 128, sStartMenuCursorPos);
                sStartMenuCursorPos = 8;
                DrawIcon(25, 144, sStartMenuCursorPos);
                sStartMenuCursorPos = 0;
            } else if (sStartMenuCursorPos == 1){
                DrawIcon(57, 144, sStartMenuCursorPos);
                sStartMenuCursorPos = 7;
                DrawIcon(57, 128, sStartMenuCursorPos);
                sStartMenuCursorPos = 8;
                DrawIcon(57, 144, sStartMenuCursorPos);
                sStartMenuCursorPos = 1;
            } else if (sStartMenuCursorPos == 2){
                DrawIcon(89, 144, sStartMenuCursorPos);
                sStartMenuCursorPos = 7;
                DrawIcon(89, 128, sStartMenuCursorPos);
                sStartMenuCursorPos = 8;
                DrawIcon(89, 144, sStartMenuCursorPos);
                sStartMenuCursorPos = 2;
            } else if (sStartMenuCursorPos == 3){
                DrawIcon(121, 144, sStartMenuCursorPos);
                sStartMenuCursorPos = 7;
                DrawIcon(121, 128, sStartMenuCursorPos);
                sStartMenuCursorPos = 8;
                DrawIcon(121, 144, sStartMenuCursorPos);
                sStartMenuCursorPos = 3;
            } else if (sStartMenuCursorPos == 4){
                DrawIcon(153, 144, sStartMenuCursorPos);
                sStartMenuCursorPos = 7;
                DrawIcon(153, 128, sStartMenuCursorPos);
                sStartMenuCursorPos = 8;
                DrawIcon(153, 144, sStartMenuCursorPos);
                sStartMenuCursorPos = 4;
            } else if (sStartMenuCursorPos == 5){
                DrawIcon(185, 144, sStartMenuCursorPos);
                sStartMenuCursorPos = 7;
                DrawIcon(185, 128, sStartMenuCursorPos);
                sStartMenuCursorPos = 8;
                DrawIcon(185, 144, sStartMenuCursorPos);
                sStartMenuCursorPos = 5;
            } else if (sStartMenuCursorPos == 6){
                DrawIcon(217, 144, sStartMenuCursorPos);
                sStartMenuCursorPos = 7;
                DrawIcon(217, 128, sStartMenuCursorPos);
                sStartMenuCursorPos = 8;
                DrawIcon(217, 144, sStartMenuCursorPos);
                sStartMenuCursorPos = 6;
            }       

            if (sStartMenuCursorPos != 0){
                startValue = sStartMenuCursorPos;
                sStartMenuCursorPos = 9;
                DrawIcon(25, 144, sStartMenuCursorPos);
                sStartMenuCursorPos = startValue;
            }   
            if (sStartMenuCursorPos != 1){
                startValue = sStartMenuCursorPos;
                sStartMenuCursorPos = 10;
                DrawIcon(57, 144, sStartMenuCursorPos);
                sStartMenuCursorPos = startValue;
            }
            if (sStartMenuCursorPos != 2){
                startValue = sStartMenuCursorPos;
                sStartMenuCursorPos = 11;
                DrawIcon(89, 144, sStartMenuCursorPos);
                sStartMenuCursorPos = startValue;
            }   
            if (sStartMenuCursorPos != 3){
                startValue = sStartMenuCursorPos;
                sStartMenuCursorPos = 12;
                DrawIcon(121, 144, sStartMenuCursorPos);
                sStartMenuCursorPos = startValue;
            }   
            if (sStartMenuCursorPos != 4){
                startValue = sStartMenuCursorPos;
                sStartMenuCursorPos = 13;
                DrawIcon(153, 144, sStartMenuCursorPos);
                sStartMenuCursorPos = startValue;
            }
            if (sStartMenuCursorPos != 5){
                startValue = sStartMenuCursorPos;
                sStartMenuCursorPos = 14;
                DrawIcon(185, 144, sStartMenuCursorPos);
                sStartMenuCursorPos = startValue;
            }
            if (sStartMenuCursorPos != 6){
                startValue = sStartMenuCursorPos;
                sStartMenuCursorPos = 15;
                DrawIcon(217, 144, sStartMenuCursorPos);
                sStartMenuCursorPos = startValue;
            }
        } else {
            if(sStartMenuCursorPos == 0){
                DrawIcon(25, 144, sStartMenuCursorPos);
                sStartMenuCursorPos = 7;
                DrawIcon(25, 128, sStartMenuCursorPos);
                sStartMenuCursorPos = 8;
                DrawIcon(25, 144, sStartMenuCursorPos);
                sStartMenuCursorPos = 0;
            } else if (sStartMenuCursorPos == 1){
                sStartMenuCursorPos = 2;
                DrawIcon(57, 144, sStartMenuCursorPos);
                sStartMenuCursorPos = 7;
                DrawIcon(57, 128, sStartMenuCursorPos);
                sStartMenuCursorPos = 8;
                DrawIcon(57, 144, sStartMenuCursorPos);
                sStartMenuCursorPos = 1;
            } else if (sStartMenuCursorPos == 2){
                sStartMenuCursorPos = 3;
                DrawIcon(89, 144, sStartMenuCursorPos);
                sStartMenuCursorPos = 7;
                DrawIcon(89, 128, sStartMenuCursorPos);
                sStartMenuCursorPos = 8;
                DrawIcon(89, 144, sStartMenuCursorPos);
                sStartMenuCursorPos = 2;
            } else if (sStartMenuCursorPos == 3){
                sStartMenuCursorPos = 4;
                DrawIcon(121, 144, sStartMenuCursorPos);
                sStartMenuCursorPos = 7;
                DrawIcon(121, 128, sStartMenuCursorPos);
                sStartMenuCursorPos = 8;
                DrawIcon(121, 144, sStartMenuCursorPos);
                sStartMenuCursorPos = 3;
            } else if (sStartMenuCursorPos == 4){
                sStartMenuCursorPos = 5;
                DrawIcon(153, 144, sStartMenuCursorPos);
                sStartMenuCursorPos = 7;
                DrawIcon(153, 128, sStartMenuCursorPos);
                sStartMenuCursorPos = 8;
                DrawIcon(153, 144, sStartMenuCursorPos);
                sStartMenuCursorPos = 4;
            } else if (sStartMenuCursorPos == 5){
                sStartMenuCursorPos = 6;
                DrawIcon(185, 144, sStartMenuCursorPos);
                sStartMenuCursorPos = 7;
                DrawIcon(185, 128, sStartMenuCursorPos);
                sStartMenuCursorPos = 8;
                DrawIcon(185, 144, sStartMenuCursorPos);
                sStartMenuCursorPos = 5;
            }       

            if (sStartMenuCursorPos != 0){
                startValue = sStartMenuCursorPos;
                sStartMenuCursorPos = 9;
                DrawIcon(25, 144, sStartMenuCursorPos);
                sStartMenuCursorPos = startValue;
            }
            if (sStartMenuCursorPos != 1){
                startValue = sStartMenuCursorPos;
                sStartMenuCursorPos = 11;
                DrawIcon(57, 144, sStartMenuCursorPos);
                sStartMenuCursorPos = startValue;
            }   
            if (sStartMenuCursorPos != 2){
                startValue = sStartMenuCursorPos;
                sStartMenuCursorPos = 12;
                DrawIcon(89, 144, sStartMenuCursorPos);
                sStartMenuCursorPos = startValue;
            }   
            if (sStartMenuCursorPos != 3){
                startValue = sStartMenuCursorPos;
                sStartMenuCursorPos = 13;
                DrawIcon(121, 144, sStartMenuCursorPos);
                sStartMenuCursorPos = startValue;
            }
            if (sStartMenuCursorPos != 4){
                startValue = sStartMenuCursorPos;
                sStartMenuCursorPos = 14;
                DrawIcon(153, 144, sStartMenuCursorPos);
                sStartMenuCursorPos = startValue;
            }
            if (sStartMenuCursorPos != 5){
                startValue = sStartMenuCursorPos;
                sStartMenuCursorPos = 15;
                DrawIcon(185, 144, sStartMenuCursorPos);
                sStartMenuCursorPos = startValue;
            }
        }
    } else if (FlagGet(FLAG_SYS_POKEMON_GET) == TRUE){
        if (sStartMenuCursorPos == 0){
            sStartMenuCursorPos = 1;
            DrawIcon(25, 144, sStartMenuCursorPos);
            sStartMenuCursorPos = 7;
            DrawIcon(25, 128, sStartMenuCursorPos);
            sStartMenuCursorPos = 8;
            DrawIcon(25, 144, sStartMenuCursorPos);
            sStartMenuCursorPos = 0;
        } else if (sStartMenuCursorPos == 1){
            sStartMenuCursorPos = 2;
            DrawIcon(57, 144, sStartMenuCursorPos);
            sStartMenuCursorPos = 7;
            DrawIcon(57, 128, sStartMenuCursorPos);
            sStartMenuCursorPos = 8;
            DrawIcon(57, 144, sStartMenuCursorPos);
            sStartMenuCursorPos = 1;
        } else if (sStartMenuCursorPos == 2){
            sStartMenuCursorPos = 3;
            DrawIcon(89, 144, sStartMenuCursorPos);
            sStartMenuCursorPos = 7;
            DrawIcon(89, 128, sStartMenuCursorPos);
            sStartMenuCursorPos = 8;
            DrawIcon(89, 144, sStartMenuCursorPos);
            sStartMenuCursorPos = 2;
        } else if (sStartMenuCursorPos == 3){
            sStartMenuCursorPos = 4;
            DrawIcon(121, 144, sStartMenuCursorPos);
            sStartMenuCursorPos = 7;
            DrawIcon(121, 128, sStartMenuCursorPos);
            sStartMenuCursorPos = 8;
            DrawIcon(121, 144, sStartMenuCursorPos);
            sStartMenuCursorPos = 3;
        } else if (sStartMenuCursorPos == 4){
            sStartMenuCursorPos = 5;
            DrawIcon(153, 144, sStartMenuCursorPos);
            sStartMenuCursorPos = 7;
            DrawIcon(153, 128, sStartMenuCursorPos);
            sStartMenuCursorPos = 8;
            DrawIcon(153, 144, sStartMenuCursorPos);
            sStartMenuCursorPos = 4;
        } else if (sStartMenuCursorPos == 5){
            sStartMenuCursorPos = 6;
            DrawIcon(185, 144, sStartMenuCursorPos);
            sStartMenuCursorPos = 7;
            DrawIcon(185, 128, sStartMenuCursorPos);
            sStartMenuCursorPos = 8;
            DrawIcon(185, 144, sStartMenuCursorPos);
            sStartMenuCursorPos = 5;
        }       

        if (sStartMenuCursorPos != 0){
            startValue = sStartMenuCursorPos;
            sStartMenuCursorPos = 10;
            DrawIcon(25, 144, sStartMenuCursorPos);
            sStartMenuCursorPos = startValue;
        }
        if (sStartMenuCursorPos != 1){
            startValue = sStartMenuCursorPos;
            sStartMenuCursorPos = 11;
            DrawIcon(57, 144, sStartMenuCursorPos);
            sStartMenuCursorPos = startValue;
        }   
        if (sStartMenuCursorPos != 2){
            startValue = sStartMenuCursorPos;
            sStartMenuCursorPos = 12;
            DrawIcon(89, 144, sStartMenuCursorPos);
            sStartMenuCursorPos = startValue;
        }   
        if (sStartMenuCursorPos != 3){
            startValue = sStartMenuCursorPos;
            sStartMenuCursorPos = 13;
            DrawIcon(121, 144, sStartMenuCursorPos);
            sStartMenuCursorPos = startValue;
        }
        if (sStartMenuCursorPos != 4){
            startValue = sStartMenuCursorPos;
            sStartMenuCursorPos = 14;
            DrawIcon(153, 144, sStartMenuCursorPos);
            sStartMenuCursorPos = startValue;
        }
        if (sStartMenuCursorPos != 5){
            startValue = sStartMenuCursorPos;
            sStartMenuCursorPos = 15;
            DrawIcon(185, 144, sStartMenuCursorPos);
            sStartMenuCursorPos = startValue;
        }
    } else {
        if (sStartMenuCursorPos == 0){
            sStartMenuCursorPos = 2;
            DrawIcon(25, 144, sStartMenuCursorPos);
            sStartMenuCursorPos = 7;
            DrawIcon(25, 128, sStartMenuCursorPos);
            sStartMenuCursorPos = 8;
            DrawIcon(25, 144, sStartMenuCursorPos);
            sStartMenuCursorPos = 0;
        } else if (sStartMenuCursorPos == 1){
            sStartMenuCursorPos = 3;
            DrawIcon(57, 144, sStartMenuCursorPos);
            sStartMenuCursorPos = 7;
            DrawIcon(57, 128, sStartMenuCursorPos);
            sStartMenuCursorPos = 8;
            DrawIcon(57, 144, sStartMenuCursorPos);
            sStartMenuCursorPos = 1;
        } else if (sStartMenuCursorPos == 2){
            sStartMenuCursorPos = 4;
            DrawIcon(89, 144, sStartMenuCursorPos);
            sStartMenuCursorPos = 7;
            DrawIcon(89, 128, sStartMenuCursorPos);
            sStartMenuCursorPos = 8;
            DrawIcon(89, 144, sStartMenuCursorPos);
            sStartMenuCursorPos = 2;
        } else if (sStartMenuCursorPos == 3){
            sStartMenuCursorPos = 5;
            DrawIcon(121, 144, sStartMenuCursorPos);
            sStartMenuCursorPos = 7;
            DrawIcon(121, 128, sStartMenuCursorPos);
            sStartMenuCursorPos = 8;
            DrawIcon(121, 144, sStartMenuCursorPos);
            sStartMenuCursorPos = 3;
        } else if (sStartMenuCursorPos == 4){
            sStartMenuCursorPos = 6;
            DrawIcon(153, 144, sStartMenuCursorPos);
            sStartMenuCursorPos = 7;
            DrawIcon(153, 128, sStartMenuCursorPos);
            sStartMenuCursorPos = 8;
            DrawIcon(153, 144, sStartMenuCursorPos);
            sStartMenuCursorPos = 4;
        }       

        if (sStartMenuCursorPos != 0){
            startValue = sStartMenuCursorPos;
            sStartMenuCursorPos = 11;
            DrawIcon(25, 144, sStartMenuCursorPos);
            sStartMenuCursorPos = startValue;
        }   
        if (sStartMenuCursorPos != 1){
            startValue = sStartMenuCursorPos;
            sStartMenuCursorPos = 12;
            DrawIcon(57, 144, sStartMenuCursorPos);
            sStartMenuCursorPos = startValue;
        }   
        if (sStartMenuCursorPos != 2){
            startValue = sStartMenuCursorPos;
            sStartMenuCursorPos = 13;
            DrawIcon(89, 144, sStartMenuCursorPos);
            sStartMenuCursorPos = startValue;
        }
        if (sStartMenuCursorPos != 3){
            startValue = sStartMenuCursorPos;
            sStartMenuCursorPos = 14;
            DrawIcon(121, 144, sStartMenuCursorPos);
            sStartMenuCursorPos = startValue;
        }
        if (sStartMenuCursorPos != 4){
            startValue = sStartMenuCursorPos;
            sStartMenuCursorPos = 15;
            DrawIcon(153, 144, sStartMenuCursorPos);
            sStartMenuCursorPos = startValue;
        }
    }
}

static u8 setIconPos(void)
{
    u8 startValue;
                            
    startValue = sStartMenuCursorPos;
    
    destroyIcons();
    drawMain();
    
    sStartMenuCursorPos = startValue;
}

static void AddStartMenuAction(u8 action)
{
    AppendToList(sCurrentStartMenuActions, &sNumStartMenuActions, action);
}

static void BuildStartMenuActions_Normal(void)
{
    u8 labelLeft;
    u8 labelRight;
    u8 labelTop;
    u8 labelBottom;
    
    labelLeft = 0;
    labelRight = 240;
    labelTop = 120;
    labelBottom = 0;

    drawMain();
    
    REG_WINOUT = 0xFF;
    REG_BLDCNT = 0xFE;
    REG_BLDALPHA = 0;
    REG_BLDY = 6;
    REG_WIN0H = WIN_RANGE(labelLeft, labelRight);
    REG_WIN0V = WIN_RANGE(labelTop, labelBottom);

    if (FlagGet(FLAG_SYS_POKEDEX_GET) == TRUE)
        AddStartMenuAction(MENU_ACTION_POKEDEX);
    if (FlagGet(FLAG_SYS_POKEMON_GET) == TRUE)
        AddStartMenuAction(MENU_ACTION_POKEMON);
    AddStartMenuAction(MENU_ACTION_BAG);
    AddStartMenuAction(MENU_ACTION_PLAYER);
    AddStartMenuAction(MENU_ACTION_SAVE);
    AddStartMenuAction(MENU_ACTION_OPTION);
    AddStartMenuAction(MENU_ACTION_EXIT);
}

static void BuildStartMenuActions_SafariZone(void)
{
    AddStartMenuAction(MENU_ACTION_RETIRE);
    AddStartMenuAction(MENU_ACTION_POKEDEX);
    AddStartMenuAction(MENU_ACTION_POKEMON);
    AddStartMenuAction(MENU_ACTION_BAG);
    AddStartMenuAction(MENU_ACTION_PLAYER);
    AddStartMenuAction(MENU_ACTION_OPTION);
    AddStartMenuAction(MENU_ACTION_EXIT);
}

static void BuildStartMenuActions_Link(void)
{
    AddStartMenuAction(MENU_ACTION_POKEMON);
    AddStartMenuAction(MENU_ACTION_BAG);
    AddStartMenuAction(MENU_ACTION_PLAYER_LINK);
    AddStartMenuAction(MENU_ACTION_OPTION);
    AddStartMenuAction(MENU_ACTION_EXIT);
}

//Show number of safari balls left
static void DisplaySafariBallsWindow(void)
{
    AlignInt2InMenuWindow(gStringVar1, gNumSafariBalls, 12, 1);
    Menu_DrawStdWindowFrame(0, 0, 10, 5);
    Menu_PrintText(gOtherText_SafariStock, 1, 1);
}

//Prints n menu items starting at *index
static bool32 PrintStartMenuItemsMultistep(s16 *index, u32 n)
{
    int _index = *index;

    do
    {
        //Menu_PrintText(sStartMenuItems[sCurrentStartMenuActions[_index]].text, 23, 2 + _index * 2);
        _index++;
        if (_index >= sNumStartMenuActions)
        {
            *index = _index;
            return TRUE;
        }
    }
    while (--n != 0);
    *index = _index;
    return FALSE;
}

static bool32 InitStartMenuMultistep(s16 *step, s16 *index)
{
    switch (*step)
    {
    case 1:
        BuildStartMenuActions();
        (*step)++;
        break;
    case 2:
        //Menu_DrawStdWindowFrame(22, 0, 29, sNumStartMenuActions * 2 + 3);
        *index = 0;
        (*step)++;
        break;
    case 3:
        if (GetSafariZoneFlag())
            DisplaySafariBallsWindow();
        (*step)++;
        break;
    case 4:
        if (PrintStartMenuItemsMultistep(index, 2))
            (*step)++;
        break;
    case 0:
        (*step)++;
        break;
    case 5:
        //sStartMenuCursorPos = InitMenu(0, 0x17, 2, sNumStartMenuActions, sStartMenuCursorPos, 6);
        return TRUE;
    }
    return FALSE;
}

static void InitStartMenu(void)
{
    s16 step = 0;
    s16 index = 0;

    while (InitStartMenuMultistep(&step, &index) == FALSE)
        ;
}

static void Task_StartMenu(u8 taskId)
{
    if (InitStartMenuMultistep(gTasks[taskId].data, gTasks[taskId].data + 1) == TRUE)
    {
        *gTasks[taskId].data = 0;
        SwitchTaskToFollowupFunc(taskId);
    }
}

void CreateStartMenuTask(void (*func)(u8))
{
    u8 taskId;

    InitMenuWindow(&gMenuTextWindowTemplate);
    taskId = CreateTask(Task_StartMenu, 0x50);
    SetTaskFuncWithFollowupFunc(taskId, Task_StartMenu, func);
}

void sub_80712B4(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    switch (task->data[0])
    {
    case 0:
        gMenuCallback = StartMenu_InputProcessCallback;
        task->data[0]++;
        break;
    case 1:
        if (gMenuCallback() == 1)
            DestroyTask(taskId);
        break;
    }
}

void sub_8071310(void)
{
    if (!is_c1_link_related_active())
    {
        FreezeEventObjects();
        sub_80594C0();
        sub_80597F4();
    }
    CreateStartMenuTask(sub_80712B4);
    ScriptContext2_Enable();
}

static u8 cursorPosCalcPlus(void){
    
    if (FlagGet(FLAG_SYS_POKEDEX_GET) == TRUE){
        if (FlagGet(FLAG_SYS_POKEMON_GET) == TRUE){
            if (sStartMenuCursorPos != 6){
                sStartMenuCursorPos++;
            } else {
                sStartMenuCursorPos = 0;
            }
        } else {
            if (sStartMenuCursorPos != 5){
                sStartMenuCursorPos++;
            } else {
                sStartMenuCursorPos = 0;
            }
        }
    } else if (FlagGet(FLAG_SYS_POKEMON_GET) == TRUE){
        if (sStartMenuCursorPos != 5){
            sStartMenuCursorPos++;
        } else {
            sStartMenuCursorPos = 0;
        }
    } else {
        if (sStartMenuCursorPos != 4){
            sStartMenuCursorPos++;
        } else {
            sStartMenuCursorPos = 0;
        }
        
    }
}

static u8 cursorPosCalcMinus(void){
    if (FlagGet(FLAG_SYS_POKEDEX_GET) == TRUE){
        if (FlagGet(FLAG_SYS_POKEMON_GET) == TRUE){
            if (sStartMenuCursorPos != 0){
                sStartMenuCursorPos--;
            } else {
                sStartMenuCursorPos = 6;
            }
        } else {
            if (sStartMenuCursorPos != 0){
                sStartMenuCursorPos--;
            } else {
                sStartMenuCursorPos = 5;
            }
        }
    } else if (FlagGet(FLAG_SYS_POKEMON_GET) == TRUE){
        if (sStartMenuCursorPos != 0){
            sStartMenuCursorPos--;
        } else {
            sStartMenuCursorPos = 5;
        }
    } else {
        if (sStartMenuCursorPos != 0){
            sStartMenuCursorPos--;
        } else {
            sStartMenuCursorPos = 4;
        }
        
    }
}

static u8 StartMenu_InputProcessCallback(void)
{
    if (gMain.newKeys & DPAD_LEFT)
    {
        PlaySE(SE_SELECT);
        //sStartMenuCursorPos = Menu_MoveCursor(-1);
        cursorPosCalcMinus();
        setIconPos();
    }
    if (gMain.newKeys & DPAD_RIGHT)
    {
        PlaySE(SE_SELECT);
        //sStartMenuCursorPos = Menu_MoveCursor(1);
        cursorPosCalcPlus();
        setIconPos();
    }
    if (gMain.newKeys & A_BUTTON)
    {
        PlaySE(SE_SELECT);
        if (sStartMenuItems[sCurrentStartMenuActions[sStartMenuCursorPos]].func == StartMenu_PokedexCallback)
        {
            if (GetNationalPokedexCount(0) == 0)
                return 0;
        }
        gMenuCallback = sStartMenuItems[sCurrentStartMenuActions[sStartMenuCursorPos]].func;
        if (gMenuCallback != StartMenu_SaveCallback &&
           gMenuCallback != StartMenu_ExitCallback &&
           gMenuCallback != StartMenu_RetireCallback)
            FadeScreen(1, 0);
        return 0;
    }
    if (gMain.newKeys & (START_BUTTON | B_BUTTON))
    {
        closeIconsAndMenu();
        CloseMenu();
        return 1;
    }
    return 0;
}

//When player selects POKEDEX
static u8 StartMenu_PokedexCallback(void)
{
    closeIconsAndMenu();
    if (!gPaletteFade.active)
    {
        IncrementGameStat(GAME_STAT_CHECKED_POKEDEX);
        PlayRainSoundEffect();
        SetMainCallback2(CB2_InitPokedex);
        return 1;
    }
    return 0;
}

//When player selects POKEMON
static u8 StartMenu_PokemonCallback(void)
{
    closeIconsAndMenu();
    if (!gPaletteFade.active)
    {
        PlayRainSoundEffect();
        SetMainCallback2(sub_8089A70);
        return 1;
    }
    return 0;
}

//When player selects BAG
static u8 StartMenu_BagCallback(void)
{
    closeIconsAndMenu();
    if (!gPaletteFade.active)
    {
        PlayRainSoundEffect();
        SetMainCallback2(sub_80A53F8);
        return 1;
    }
    return 0;
}

//When player selects his/her name
static u8 StartMenu_PlayerCallback(void)
{
    closeIconsAndMenu();
    if (!gPaletteFade.active)
    {
        PlayRainSoundEffect();
        TrainerCard_ShowPlayerCard(c2_exit_to_overworld_1_sub_8080DEC);
        return 1;
    }
    return 0;
}

//When player selects SAVE
static u8 StartMenu_SaveCallback(void)
{
    closeIconsAndMenu();
    Menu_DestroyCursor();
    gMenuCallback = SaveCallback1;
    return 0;
}

//When player selects OPTION
static u8 StartMenu_OptionCallback(void)
{
    closeIconsAndMenu();
    if (!gPaletteFade.active)
    {
        PlayRainSoundEffect();
        SetMainCallback2(CB2_InitOptionMenu);
        gMain.savedCallback = c2_exit_to_overworld_1_sub_8080DEC;
        return 1;
    }
    return 0;
}

//When player selects EXIT
static u8 StartMenu_ExitCallback(void)
{
    closeIconsAndMenu();
    CloseMenu();
    return 1;
}

//When player selects RETIRE
static u8 StartMenu_RetireCallback(void)
{
    closeIconsAndMenu();
    CloseMenu();
    SafariZoneRetirePrompt();
    return 1;
}

//When player selects their name in multiplayer mode
static u8 StartMenu_PlayerLinkCallback(void)
{
    closeIconsAndMenu();
    if (!gPaletteFade.active)
    {
        PlayRainSoundEffect();
        TrainerCard_ShowLinkCard(gUnknown_03004860, c2_exit_to_overworld_1_sub_8080DEC);
        return 1;
    }
    return 0;
}

//Save dialog status
enum
{
    SAVE_IN_PROGRESS,
    SAVE_SUCCESS,
    SAVE_CANCELED,
    SAVE_ERROR,
};

static u8 SaveCallback1(void)
{
    sub_807160C();
    gMenuCallback = SaveCallback2;
    return FALSE;
}

static u8 SaveCallback2(void)
{
    switch (RunSaveDialogCallback())
    {
    case SAVE_IN_PROGRESS:
        return FALSE;
    case SAVE_CANCELED:
        //Go back to start menu
        Menu_EraseScreen();
        InitStartMenu();
        gMenuCallback = StartMenu_InputProcessCallback;
        return FALSE;
    case SAVE_SUCCESS:
    case SAVE_ERROR:
        Menu_EraseScreen();
        ScriptUnfreezeEventObjects();
        ScriptContext2_Disable();
        return TRUE;
    }
    return FALSE;
}

static void sub_807160C(void)
{
    save_serialize_map();
    saveDialogCallback = SaveDialogCB_DisplayConfirmMessage;
    savingComplete = FALSE;
}

static u8 RunSaveDialogCallback(void)
{
    if (savingComplete)
    {
        if (!Menu_UpdateWindowText())
            return 0;
    }
    savingComplete = FALSE;
    return saveDialogCallback();
}

void ScrSpecial_DoSaveDialog(void)
{
    sub_807160C();
    CreateTask(Task_SaveDialog, 0x50);
}

static void DisplaySaveMessageWithCallback(const u8 *ptr, u8 (*func)(void))
{
    StringExpandPlaceholders(gStringVar4, ptr);
    Menu_DisplayDialogueFrame();
    MenuPrintMessageDefaultCoords(gStringVar4);
    savingComplete = TRUE;
    saveDialogCallback = func;
}

static void Task_SaveDialog(u8 taskId)
{
    u8 status = RunSaveDialogCallback();

    switch (status)
    {
    case SAVE_CANCELED:
    case SAVE_ERROR:
        gSpecialVar_Result = 0;
        break;
    case SAVE_SUCCESS:
        gSpecialVar_Result = status;
        break;
    case SAVE_IN_PROGRESS:
        return;
    }
    DestroyTask(taskId);
    EnableBothScriptContexts();
}

static void sub_8071700(void)
{
    HandleCloseSaveWindow(0, 0);
}

static void HideSaveDialog(void)
{
    Menu_EraseWindowRect(20, 8, 26, 13);
}

static void SaveDialogStartTimeout(void)
{
    saveDialogTimer = 60;
}

static bool8 SaveDialogCheckForTimeoutOrKeypress(void)
{
    saveDialogTimer--;
    if (gMain.heldKeys & A_BUTTON)
    {
        PlaySE(SE_SELECT);
        return TRUE;
    }
    else if (saveDialogTimer == 0)
        return TRUE;
    return FALSE;
}

static bool8 SaveDialogCheckForTimeoutAndKeypress(void)
{
    if (saveDialogTimer != 0)
        saveDialogTimer--;
    else if (gMain.heldKeys & A_BUTTON)
        return TRUE;
    return FALSE;
}

static u8 SaveDialogCB_DisplayConfirmMessage(void)
{
    Menu_EraseScreen();
    HandleDrawSaveWindowInfo(0, 0);
    DisplaySaveMessageWithCallback(gSaveText_WouldYouLikeToSave, SaveDialogCB_DisplayConfirmYesNoMenu);
    return SAVE_IN_PROGRESS;
}

static u8 SaveDialogCB_DisplayConfirmYesNoMenu(void)
{
    DisplayYesNoMenu(20, 8, 1);
    saveDialogCallback = SaveDialogCB_ProcessConfirmYesNoMenu;
    return SAVE_IN_PROGRESS;
}

static u8 SaveDialogCB_ProcessConfirmYesNoMenu(void)
{
    switch (Menu_ProcessInputNoWrap_())
    {
    case 0:     //YES
        HideSaveDialog();
        switch (gSaveFileStatus)
        {
        case 0:
        case 2:
            if (gDifferentSaveFile == FALSE)
            {
                saveDialogCallback = SaveDialogCB_SaveFileExists;
                return SAVE_IN_PROGRESS;
            }
            saveDialogCallback = SaveDialogCB_DisplaySavingMessage;
            return SAVE_IN_PROGRESS;
        default:
            saveDialogCallback = SaveDialogCB_SaveFileExists;
            return SAVE_IN_PROGRESS;
        }
        break;
    case -1:    //B button
    case 1:     //NO
        HideSaveDialog();
        sub_8071700();
        return SAVE_CANCELED;
    }
    return SAVE_IN_PROGRESS;
}

static u8 SaveDialogCB_SaveFileExists(void)
{
    DisplaySaveMessageWithCallback(
      gDifferentSaveFile == TRUE ? gSaveText_ThereIsADifferentFile : gSaveText_ThereIsAlreadyAFile,
      SaveDialogCB_DisplayOverwriteYesNoMenu);
    return SAVE_IN_PROGRESS;
}

static u8 SaveDialogCB_DisplayOverwriteYesNoMenu(void)
{
    DisplayYesNoMenu(20, 8, 1);
    saveDialogCallback = SaveDialogCB_ProcessOverwriteYesNoMenu;
    return SAVE_IN_PROGRESS;
}

static u8 SaveDialogCB_ProcessOverwriteYesNoMenu(void)
{
    switch (Menu_ProcessInputNoWrap_())
    {
    case 0:     //YES
        HideSaveDialog();
        saveDialogCallback = SaveDialogCB_DisplaySavingMessage;
        break;
    case -1:    //B button
    case 1:     //NO
        HideSaveDialog();
        sub_8071700();
        return SAVE_CANCELED;
    }
    return SAVE_IN_PROGRESS;
}

static u8 SaveDialogCB_DisplaySavingMessage(void)
{
    //"SAVING... DON'T TURN OFF THE POWER."
    DisplaySaveMessageWithCallback(gSaveText_DontTurnOff, SaveDialogCB_DoSave);
    return SAVE_IN_PROGRESS;
}

static u8 SaveDialogCB_DoSave(void)
{
    u8 saveStatus;

    IncrementGameStat(GAME_STAT_SAVED_GAME);
    if (gDifferentSaveFile == TRUE)
    {
        saveStatus = Save_WriteData(SAVE_OVERWRITE_DIFFERENT_FILE);
        gDifferentSaveFile = FALSE;
    }
    else
    {
        saveStatus = Save_WriteData(SAVE_NORMAL);
    }

    if (saveStatus == SAVE_STATUS_OK)
    {
        //"(Player) saved the game."
        DisplaySaveMessageWithCallback(gSaveText_PlayerSavedTheGame, SaveDialogCB_SaveSuccess);
    }
    else
    {
        //"Save error. Please exchange the backup memory."
        DisplaySaveMessageWithCallback(gSystemText_SaveErrorExchangeBackup, SaveDialogCB_SaveError);
    }

    SaveDialogStartTimeout();
    return SAVE_IN_PROGRESS;
}

static u8 SaveDialogCB_SaveSuccess(void)
{
    if (Menu_UpdateWindowText())
    {
        PlaySE(SE_SAVE);
        saveDialogCallback = SaveDialogCB_ReturnSuccess;
    }
    return SAVE_IN_PROGRESS;
}

static u8 SaveDialogCB_ReturnSuccess(void)
{
    if (!IsSEPlaying() && SaveDialogCheckForTimeoutOrKeypress())
    {
        sub_8071700();
        return SAVE_SUCCESS;
    }
    else
        return SAVE_IN_PROGRESS;
}

static u8 SaveDialogCB_SaveError(void)
{
    if (Menu_UpdateWindowText())
    {
        PlaySE(SE_BOO);
        saveDialogCallback = SaveDialogCB_ReturnError;
    }
    return SAVE_IN_PROGRESS;
}

static u8 SaveDialogCB_ReturnError(void)
{
    if (!SaveDialogCheckForTimeoutAndKeypress())
        return SAVE_IN_PROGRESS;
    else
    {
        sub_8071700();
        return SAVE_ERROR;
    }
}

static void sub_80719F0(void)
{
    TransferPlttBuffer();
}

static bool32 sub_80719FC(u8 *step)
{
    switch (*step)
    {
    case 0:
        REG_DISPCNT = 0;
        SetVBlankCallback(NULL);
        ScanlineEffect_Stop();
        DmaClear16(3, PLTT, PLTT_SIZE);
        DmaFill16Large(3, 0, (void *)(VRAM + 0x0), 0x18000, 0x1000);
        break;
    case 1:
        ResetSpriteData();
        ResetTasks();
        ResetPaletteFade();
        ScanlineEffect_Clear();
        break;
    case 2:
        Text_LoadWindowTemplate(&gMenuTextWindowTemplate);
        InitMenuWindow(&gMenuTextWindowTemplate);
        REG_DISPCNT = DISPCNT_MODE_0 | DISPCNT_BG0_ON;
        break;
    case 3:
    {
        u32 savedIme;

        BlendPalettes(0xFFFFFFFF, 16, RGB(0, 0, 0));
        SetVBlankCallback(sub_80719F0);
        savedIme = REG_IME;
        REG_IME = 0;
        REG_IE |= 1;
        REG_IME = savedIme;
        break;
    }
    case 4:
        return TRUE;
    }
    (*step)++;
    return FALSE;
}

void sub_8071B28(void)
{
    if (sub_80719FC(&gMain.state))
    {
        CreateTask(Task_8071B64, 0x50);
        SetMainCallback2(sub_8071B54);
    }
}

static void sub_8071B54(void)
{
    RunTasks();
    UpdatePaletteFade();
}

static void Task_8071B64(u8 taskId)
{
    s16 *step = gTasks[taskId].data;

    if (!gPaletteFade.active)
    {
        switch (*step)
        {
        case 0:
            Menu_DisplayDialogueFrame();
            Menu_PrintText(gSystemText_Saving, 2, 15);
            BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB(0, 0, 0));
            (*step)++;
            break;
        case 1:
            SetSecretBase2Field_9_AndHideBG();
            sub_8125E2C();
            (*step)++;
            break;
        case 2:
            if (!sub_8125E6C())
                break;
            ClearSecretBase2Field_9_2();
            (*step)++;
            break;
        case 3:
            BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB(0, 0, 0));
            (*step)++;
            break;
        case 4:
            SetMainCallback2(gMain.savedCallback);
            DestroyTask(taskId);
            break;
        }
    }
}

static void ArrowCallback(struct Sprite *sprite)
{
        StartSpriteAnimIfDifferent(sprite, 0);
}
