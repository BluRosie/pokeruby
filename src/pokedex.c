#include "global.h"
#include "gba/m4a_internal.h"
#include "pokedex.h"
#include "battle.h"
#include "data2.h"
#include "decompress.h"
#include "event_data.h"
#include "graphics.h"
#include "m4a.h"
#include "main.h"
#include "menu.h"
#include "menu_cursor.h"
#include "palette.h"
#include "pokedex_area_screen.h"
#include "pokedex_cry_screen.h"
#include "pokemon.h"
#include "pokemon_icon.h"
#include "random.h"
#include "overworld.h"
#include "constants/songs.h"
#include "sound.h"
#include "constants/species.h"
#include "string_util.h"
#include "strings.h"
#include "task.h"
#include "trig.h"
#include "scanline_effect.h"
#include "ewram.h"

struct PokedexListItem
{
    u16 dexNum;
    u16 seen:1;
    u16 owned:1;
};

struct PokedexView
{
    struct PokedexListItem unk0[NATIONAL_DEX_COUNT];
    u16 unk608;
    u8 unk60A_1:1;
    u8 unk60A_2:1;
    u8 unk60B;
    u16 pokemonListCount;
    u16 selectedPokemon;
    u16 unk610;
    u16 dexMode;
    u16 unk614;
    u16 dexOrder;
    u16 unk618;
    u16 seenPokemon;
    u16 ownedPokemon;
    u16 spriteIds[4]; // for scrolling mons
    u16 selectedMonSpriteId;
    u16 changeInSelectedNamePixelPos; // for updating which mon is selected graphically (pixels)
    u16 unk62A;
    u8 selectedNamePixelPosition; // which mon is selected graphically (in pixels from the top)
    u8 unk62D;
    u8 iterationsOfScrolling; // directly manipulated by the function
    u8 scrollDirection;
    s16 unk630;
    s16 unk632;
    u16 pixelsToScrollPerIteration;
    u16 iterationsOfScrollingConstant; // used to call the function
    u16 timeHeldDPad;
    u16 unk63A[4];
    u8 filler642[8];
    u8 currentScreen;
    u8 unk64B;
    u8 unk64C_1:1;
    u8 selectedScreen;
    u8 descriptionPageNum;
    u8 unk64F;
    u8 menuIsOpen;      //menuIsOpen
    u8 unk651;
    u16 menuCursorPos;     //Menu cursor position
    s16 menuY;     //Menu Y position (inverted because we use REG_BG0VOFS for this)
    u8 unk656[8];
    u8 unk65E[8];
};

enum
{
    DEX_MODE_HOENN,
    DEX_MODE_NATIONAL
};

enum
{
    PAGE_SCREEN,
    AREA_SCREEN,
    CRY_SCREEN,
    SIZE_SCREEN
};

enum
{
    MAIN_DEXPAGE,
    PAGE_DEXPAGE,
    DEXPAGE_2,
    RESULTS_DEXPAGE,
    DEXPAGE_4,
    AREA_DEXPAGE,
    CRY_DEXPAGE,
    SIZE_DEXPAGE
};

struct UnknownStruct2
{
    const u8 *text1;
    const u8 *text2;
};

struct UnknownStruct1
{
    const struct UnknownStruct2 *unk0;
    u8 unk4;
    u8 unk5;
    u16 unk6;
};

struct UnknownStruct3
{
    const u8 *text;
    u8 unk4;
    u8 unk5;
    u8 unk6;
};

struct UnknownStruct4
{
    const u8 *text;
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u8 unk7;
    u8 unk8;
    u8 unk9;
};

extern struct MusicPlayerInfo gMPlayInfo_BGM;
extern u8 gReservedSpritePaletteCount;
extern struct SpriteTemplate gUnknown_02024E8C;
extern u8 gUnknown_03005E98;
extern const u8 gPokedexMenu_Gfx[];
extern const u8 gUnknown_08E96738[];
extern const u8 gPokedexListLayout[];
extern const u8 gPokedexListResultsLayout[];
extern const u8 gUnknown_08E9C6DC[];
extern const u8 gPokedexPageLayout[];
extern const u8 gUnknown_08E96ACC[];
extern const u8 gUnknown_08E96B58[];
extern const u16 gPokedexMenu_Pal[];
extern const u16 gPokedexMenu2_Pal[];
extern const struct CompressedSpriteSheet gTrainerFrontPicTable[];
extern const struct MonCoords gTrainerFrontPicCoords[];
extern const struct PokedexEntry gPokedexEntries[];
extern const u8 gPokedexMenuSearch_Gfx[];
extern const u8 gUnknown_08E96D2C[];
extern const u16 gPokedexMenuSearch_Pal[];
extern const u8 gTypeNames[][7];
extern const u8 gPokedexMenu2_Gfx[];

extern void SafeLoadMonIconPalette(u16 species);

static EWRAM_DATA struct PokedexView *gPokedexView = NULL;
static EWRAM_DATA u16 gUnknown_0202FFB8 = 0;
static EWRAM_DATA u8 gUnknown_0202FFBA = 0;
static EWRAM_DATA struct PokedexListItem *gUnknown_0202FFBC = NULL;

u8 gUnknown_03005CE8;
IntrCallback gUnknown_03005CEC;

static u8 sub_8091E3C(void);

static const u16 sPokedexSearchPalette[] = INCBIN_U16("graphics/pokedex/search.gbapal");
static const u16 sNationalPokedexPalette[] = INCBIN_U16("graphics/pokedex/national.gbapal");
const u8 gEmptySpacce_839F7FC[0xA4] = {0};
static const u8 gUnknown_0839F8A0[] = INCBIN_U8("graphics/pokedex/pokedex_cry_layout.bin.lz");
static const u8 gUnknown_0839F988[] = INCBIN_U8("graphics/pokedex/pokedex_size_layout.bin.lz");
#if ENGLISH
static const u8 gUnknown_0839FA7C[] = INCBIN_U8("graphics/pokedex/noball.4bpp.lz");
#elif GERMAN
extern const u8 gUnknown_0839FA7C[];
#endif

#include "data/pokedex_orders.h"
static const struct OamData gOamData_83A0404 =
{
    .y = 160,
    .affineMode = 0,
    .objMode = 0,
    .mosaic = 0,
    .bpp = 0,
    .shape = 0,
    .x = 0,
    .matrixNum = 0,
    .size = 0,
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0,
};
static const struct OamData gOamData_83A040C =
{
    .y = 160,
    .affineMode = 0,
    .objMode = 0,
    .mosaic = 0,
    .bpp = 0,
    .shape = 1,
    .x = 0,
    .matrixNum = 0,
    .size = 0,
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0,
};
static const struct OamData gOamData_83A0414 =
{
    .y = 160,
    .affineMode = 0,
    .objMode = 0,
    .mosaic = 0,
    .bpp = 0,
    .shape = 1,
    .x = 0,
    .matrixNum = 0,
    .size = 3,
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0,
};
static const struct OamData gOamData_83A041C =
{
    .y = 160,
    .affineMode = 0,
    .objMode = 2,
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
static const struct OamData gOamData_83A0424 =
{
    .y = 160,
    .affineMode = 0,
    .objMode = 0,
    .mosaic = 0,
    .bpp = 0,
    .shape = 1,
    .x = 0,
    .matrixNum = 0,
    .size = 3,
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0,
};
static const struct OamData gOamData_83A042C =
{
    .y = 160,
    .affineMode = 0,
    .objMode = 0,
    .mosaic = 0,
    .bpp = 0,
    .shape = 2,
    .x = 0,
    .matrixNum = 0,
    .size = 0,
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0,
};
static const union AnimCmd gSpriteAnim_83A0434[] =
{
    ANIMCMD_FRAME(3, 30),
    ANIMCMD_END,
};
static const union AnimCmd gSpriteAnim_83A043C[] =
{
    ANIMCMD_FRAME(1, 30),
    ANIMCMD_END,
};
static const union AnimCmd gSpriteAnim_83A0444[] =
{
    ANIMCMD_FRAME(16, 30),
    ANIMCMD_END,
};
static const union AnimCmd gSpriteAnim_83A044C[] =
{
    ANIMCMD_FRAME(32, 30),
    ANIMCMD_END,
};
static const union AnimCmd gSpriteAnim_83A0454[] =
{
    ANIMCMD_FRAME(64, 30),
    ANIMCMD_END,
};
static const union AnimCmd gSpriteAnim_83A045C[] =
{
    ANIMCMD_FRAME(96, 30),
    ANIMCMD_END,
};
static const union AnimCmd gSpriteAnim_83A0464[] =
{
    ANIMCMD_FRAME(128, 30),
    ANIMCMD_END,
};
static const union AnimCmd gSpriteAnim_83A046C[] =
{
    ANIMCMD_FRAME(160, 30),
    ANIMCMD_END,
};
static const union AnimCmd gSpriteAnim_83A0474[] =
{
    ANIMCMD_FRAME(192, 30),
    ANIMCMD_END,
};
static const union AnimCmd gSpriteAnim_83A047C[] =
{
    ANIMCMD_FRAME(224, 30),
    ANIMCMD_END,
};
static const union AnimCmd gSpriteAnim_83A0484[] =
{
    ANIMCMD_FRAME(226, 30),
    ANIMCMD_END,
};
static const union AnimCmd gSpriteAnim_83A048C[] =
{
    ANIMCMD_FRAME(228, 30),
    ANIMCMD_END,
};
static const union AnimCmd gSpriteAnim_83A0494[] =
{
    ANIMCMD_FRAME(230, 30),
    ANIMCMD_END,
};
static const union AnimCmd gSpriteAnim_83A049C[] =
{
    ANIMCMD_FRAME(232, 30),
    ANIMCMD_END,
};
static const union AnimCmd gSpriteAnim_83A04A4[] =
{
    ANIMCMD_FRAME(234, 30),
    ANIMCMD_END,
};
static const union AnimCmd gSpriteAnim_83A04AC[] =
{
    ANIMCMD_FRAME(236, 30),
    ANIMCMD_END,
};
static const union AnimCmd gSpriteAnim_83A04B4[] =
{
    ANIMCMD_FRAME(238, 30),
    ANIMCMD_END,
};
static const union AnimCmd gSpriteAnim_83A04BC[] =
{
    ANIMCMD_FRAME(240, 30),
    ANIMCMD_END,
};
static const union AnimCmd gSpriteAnim_83A04C4[] =
{
    ANIMCMD_FRAME(242, 30),
    ANIMCMD_END,
};
static const union AnimCmd gSpriteAnim_83A04CC[] =
{
    ANIMCMD_FRAME(4, 30),
    ANIMCMD_END,
};
static const union AnimCmd *const gSpriteAnimTable_83A04D4[] =
{
    gSpriteAnim_83A0434,
};
static const union AnimCmd *const gSpriteAnimTable_83A04D8[] =
{
    gSpriteAnim_83A043C,
};
static const union AnimCmd *const gSpriteAnimTable_83A04DC[] =
{
    gSpriteAnim_83A0444,
};
static const union AnimCmd *const gSpriteAnimTable_83A04E0[] =
{
    gSpriteAnim_83A044C,
    gSpriteAnim_83A0454,
    gSpriteAnim_83A045C,
    gSpriteAnim_83A0464,
};
static const union AnimCmd *const gSpriteAnimTable_83A04F0[] =
{
    gSpriteAnim_83A046C,
    gSpriteAnim_83A0474,
};
static const union AnimCmd *const gSpriteAnimTable_83A04F8[] =
{
    gSpriteAnim_83A047C,
    gSpriteAnim_83A0484,
    gSpriteAnim_83A048C,
    gSpriteAnim_83A0494,
    gSpriteAnim_83A049C,
    gSpriteAnim_83A04A4,
    gSpriteAnim_83A04AC,
    gSpriteAnim_83A04B4,
    gSpriteAnim_83A04BC,
    gSpriteAnim_83A04C4,
};
static const union AnimCmd *const gSpriteAnimTable_83A0520[] =
{
    gSpriteAnim_83A04CC,
};
static void UpdateCursorPositionMainPage(struct Sprite *);
static const struct SpriteTemplate gSpriteTemplate_MainPageScrollBar =
{
    .tileTag = 4096,
    .paletteTag = 4096,
    .oam = &gOamData_83A0404,
    .anims = gSpriteAnimTable_83A04D4,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = UpdateCursorPositionMainPage,
};
static void sub_808EF8C(struct Sprite *);
static const struct SpriteTemplate gSpriteTemplate_RedArrow =
{
    .tileTag = 4096,
    .paletteTag = 4096,
    .oam = &gOamData_83A040C,
    .anims = gSpriteAnimTable_83A04D8,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = sub_808EF8C,
};
static void sub_808F08C(struct Sprite *);
static const struct SpriteTemplate gSpriteTemplate_BottomLeftText =
{
    .tileTag = 4096,
    .paletteTag = 4096,
    .oam = &gOamData_83A0414,
    .anims = gSpriteAnimTable_83A04E0,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = sub_808F08C,
};
static void sub_808F0B4(struct Sprite *);
static const struct SpriteTemplate gSpriteTemplate_TransparentBarForBall =
{
    .tileTag = 4096,
    .paletteTag = 4096,
    .oam = &gOamData_83A041C,
    .anims = gSpriteAnimTable_83A04DC,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = sub_808F0B4,
};
static void sub_808ED94(struct Sprite *);
static const struct SpriteTemplate gSpriteTemplate_SeenOwnText =
{
    .tileTag = 4096,
    .paletteTag = 4096,
    .oam = &gOamData_83A0424,
    .anims = gSpriteAnimTable_83A04F0,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = sub_808ED94,
};
static const struct SpriteTemplate gSpriteTemplate_SeenOwnNumbers =
{
    .tileTag = 4096,
    .paletteTag = 4096,
    .oam = &gOamData_83A042C,
    .anims = gSpriteAnimTable_83A04F8,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = sub_808ED94,
};
static void sub_808F168(struct Sprite *);
static const struct SpriteTemplate gSpriteTemplate_83A05B4 =
{
    .tileTag = 4096,
    .paletteTag = 4096,
    .oam = &gOamData_83A042C,
    .anims = gSpriteAnimTable_83A0520,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = sub_808F168,
};
static const struct CompressedSpriteSheet gUnknown_083A05CC[] =
{
    {gPokedexMenu2_Gfx, 0x1F00, 0x1000},
    {NULL, 0, 0},
};
static const struct SpritePalette gUnknown_083A05DC[] =
{
    {gPokedexMenu_Pal, 0x1000},
    {NULL, 0},
};

static const u8 gPixelsPerScrollIteration[] = { 2, 4, 8, 16, 32}; // notice that gPixelsPerScrollIteration[i] * gScrollingIterations[i] = 32
static const u8 gScrollingIterations[] =      {16, 8, 4,  2,  1};

const u8 gEmptySpacce_83A05F6[] = {0, 0};  // Padding, maybe?
static const u8 gUnknown_083A05F8[] = _("");
#if ENGLISH
#include "data/pokedex_entries_en.h"
#elif GERMAN
#include "data/pokedex_entries_de.h"
#endif
static const u16 gUnknown_083B4EC4[16] = {0};

static const u8 *const sMonFootprintTable[] =
{
    [SPECIES_NONE] = gMonFootprint_Bulbasaur,
    [SPECIES_BULBASAUR] = gMonFootprint_Bulbasaur,
    [SPECIES_IVYSAUR] = gMonFootprint_Ivysaur,
    [SPECIES_VENUSAUR] = gMonFootprint_Venusaur,
    [SPECIES_CHARMANDER] = gMonFootprint_Charmander,
    [SPECIES_CHARMELEON] = gMonFootprint_Charmeleon,
    [SPECIES_CHARIZARD] = gMonFootprint_Charizard,
    [SPECIES_SQUIRTLE] = gMonFootprint_Squirtle,
    [SPECIES_WARTORTLE] = gMonFootprint_Wartortle,
    [SPECIES_BLASTOISE] = gMonFootprint_Blastoise,
    [SPECIES_CATERPIE] = gMonFootprint_Caterpie,
    [SPECIES_METAPOD] = gMonFootprint_Metapod,
    [SPECIES_BUTTERFREE] = gMonFootprint_Butterfree,
    [SPECIES_WEEDLE] = gMonFootprint_Weedle,
    [SPECIES_KAKUNA] = gMonFootprint_Kakuna,
    [SPECIES_BEEDRILL] = gMonFootprint_Beedrill,
    [SPECIES_PIDGEY] = gMonFootprint_Pidgey,
    [SPECIES_PIDGEOTTO] = gMonFootprint_Pidgeotto,
    [SPECIES_PIDGEOT] = gMonFootprint_Pidgeot,
    [SPECIES_RATTATA] = gMonFootprint_Rattata,
    [SPECIES_RATICATE] = gMonFootprint_Raticate,
    [SPECIES_SPEAROW] = gMonFootprint_Spearow,
    [SPECIES_FEAROW] = gMonFootprint_Fearow,
    [SPECIES_EKANS] = gMonFootprint_Ekans,
    [SPECIES_ARBOK] = gMonFootprint_Arbok,
    [SPECIES_PIKACHU] = gMonFootprint_Pikachu,
    [SPECIES_RAICHU] = gMonFootprint_Raichu,
    [SPECIES_SANDSHREW] = gMonFootprint_Sandshrew,
    [SPECIES_SANDSLASH] = gMonFootprint_Sandslash,
    [SPECIES_NIDORAN_F] = gMonFootprint_NidoranF,
    [SPECIES_NIDORINA] = gMonFootprint_Nidorina,
    [SPECIES_NIDOQUEEN] = gMonFootprint_Nidoqueen,
    [SPECIES_NIDORAN_M] = gMonFootprint_NidoranM,
    [SPECIES_NIDORINO] = gMonFootprint_Nidorino,
    [SPECIES_NIDOKING] = gMonFootprint_Nidoking,
    [SPECIES_CLEFAIRY] = gMonFootprint_Clefairy,
    [SPECIES_CLEFABLE] = gMonFootprint_Clefable,
    [SPECIES_VULPIX] = gMonFootprint_Vulpix,
    [SPECIES_NINETALES] = gMonFootprint_Ninetales,
    [SPECIES_JIGGLYPUFF] = gMonFootprint_Jigglypuff,
    [SPECIES_WIGGLYTUFF] = gMonFootprint_Wigglytuff,
    [SPECIES_ZUBAT] = gMonFootprint_Zubat,
    [SPECIES_GOLBAT] = gMonFootprint_Golbat,
    [SPECIES_ODDISH] = gMonFootprint_Oddish,
    [SPECIES_GLOOM] = gMonFootprint_Gloom,
    [SPECIES_VILEPLUME] = gMonFootprint_Vileplume,
    [SPECIES_PARAS] = gMonFootprint_Paras,
    [SPECIES_PARASECT] = gMonFootprint_Parasect,
    [SPECIES_VENONAT] = gMonFootprint_Venonat,
    [SPECIES_VENOMOTH] = gMonFootprint_Venomoth,
    [SPECIES_DIGLETT] = gMonFootprint_Diglett,
    [SPECIES_DUGTRIO] = gMonFootprint_Dugtrio,
    [SPECIES_MEOWTH] = gMonFootprint_Meowth,
    [SPECIES_PERSIAN] = gMonFootprint_Persian,
    [SPECIES_PSYDUCK] = gMonFootprint_Psyduck,
    [SPECIES_GOLDUCK] = gMonFootprint_Golduck,
    [SPECIES_MANKEY] = gMonFootprint_Mankey,
    [SPECIES_PRIMEAPE] = gMonFootprint_Primeape,
    [SPECIES_GROWLITHE] = gMonFootprint_Growlithe,
    [SPECIES_ARCANINE] = gMonFootprint_Arcanine,
    [SPECIES_POLIWAG] = gMonFootprint_Poliwag,
    [SPECIES_POLIWHIRL] = gMonFootprint_Poliwhirl,
    [SPECIES_POLIWRATH] = gMonFootprint_Poliwrath,
    [SPECIES_ABRA] = gMonFootprint_Abra,
    [SPECIES_KADABRA] = gMonFootprint_Kadabra,
    [SPECIES_ALAKAZAM] = gMonFootprint_Alakazam,
    [SPECIES_MACHOP] = gMonFootprint_Machop,
    [SPECIES_MACHOKE] = gMonFootprint_Machoke,
    [SPECIES_MACHAMP] = gMonFootprint_Machamp,
    [SPECIES_BELLSPROUT] = gMonFootprint_Bellsprout,
    [SPECIES_WEEPINBELL] = gMonFootprint_Weepinbell,
    [SPECIES_VICTREEBEL] = gMonFootprint_Victreebel,
    [SPECIES_TENTACOOL] = gMonFootprint_Tentacool,
    [SPECIES_TENTACRUEL] = gMonFootprint_Tentacruel,
    [SPECIES_GEODUDE] = gMonFootprint_Geodude,
    [SPECIES_GRAVELER] = gMonFootprint_Graveler,
    [SPECIES_GOLEM] = gMonFootprint_Golem,
    [SPECIES_PONYTA] = gMonFootprint_Ponyta,
    [SPECIES_RAPIDASH] = gMonFootprint_Rapidash,
    [SPECIES_SLOWPOKE] = gMonFootprint_Slowpoke,
    [SPECIES_SLOWBRO] = gMonFootprint_Slowbro,
    [SPECIES_MAGNEMITE] = gMonFootprint_Magnemite,
    [SPECIES_MAGNETON] = gMonFootprint_Magneton,
    [SPECIES_FARFETCHD] = gMonFootprint_Farfetchd,
    [SPECIES_DODUO] = gMonFootprint_Doduo,
    [SPECIES_DODRIO] = gMonFootprint_Dodrio,
    [SPECIES_SEEL] = gMonFootprint_Seel,
    [SPECIES_DEWGONG] = gMonFootprint_Dewgong,
    [SPECIES_GRIMER] = gMonFootprint_Grimer,
    [SPECIES_MUK] = gMonFootprint_Muk,
    [SPECIES_SHELLDER] = gMonFootprint_Shellder,
    [SPECIES_CLOYSTER] = gMonFootprint_Cloyster,
    [SPECIES_GASTLY] = gMonFootprint_Gastly,
    [SPECIES_HAUNTER] = gMonFootprint_Haunter,
    [SPECIES_GENGAR] = gMonFootprint_Gengar,
    [SPECIES_ONIX] = gMonFootprint_Onix,
    [SPECIES_DROWZEE] = gMonFootprint_Drowzee,
    [SPECIES_HYPNO] = gMonFootprint_Hypno,
    [SPECIES_KRABBY] = gMonFootprint_Krabby,
    [SPECIES_KINGLER] = gMonFootprint_Kingler,
    [SPECIES_VOLTORB] = gMonFootprint_Voltorb,
    [SPECIES_ELECTRODE] = gMonFootprint_Electrode,
    [SPECIES_EXEGGCUTE] = gMonFootprint_Exeggcute,
    [SPECIES_EXEGGUTOR] = gMonFootprint_Exeggutor,
    [SPECIES_CUBONE] = gMonFootprint_Cubone,
    [SPECIES_MAROWAK] = gMonFootprint_Marowak,
    [SPECIES_HITMONLEE] = gMonFootprint_Hitmonlee,
    [SPECIES_HITMONCHAN] = gMonFootprint_Hitmonchan,
    [SPECIES_LICKITUNG] = gMonFootprint_Lickitung,
    [SPECIES_KOFFING] = gMonFootprint_Koffing,
    [SPECIES_WEEZING] = gMonFootprint_Weezing,
    [SPECIES_RHYHORN] = gMonFootprint_Rhyhorn,
    [SPECIES_RHYDON] = gMonFootprint_Rhydon,
    [SPECIES_CHANSEY] = gMonFootprint_Chansey,
    [SPECIES_TANGELA] = gMonFootprint_Tangela,
    [SPECIES_KANGASKHAN] = gMonFootprint_Kangaskhan,
    [SPECIES_HORSEA] = gMonFootprint_Horsea,
    [SPECIES_SEADRA] = gMonFootprint_Seadra,
    [SPECIES_GOLDEEN] = gMonFootprint_Goldeen,
    [SPECIES_SEAKING] = gMonFootprint_Seaking,
    [SPECIES_STARYU] = gMonFootprint_Staryu,
    [SPECIES_STARMIE] = gMonFootprint_Starmie,
    [SPECIES_MR_MIME] = gMonFootprint_Mrmime,
    [SPECIES_SCYTHER] = gMonFootprint_Scyther,
    [SPECIES_JYNX] = gMonFootprint_Jynx,
    [SPECIES_ELECTABUZZ] = gMonFootprint_Electabuzz,
    [SPECIES_MAGMAR] = gMonFootprint_Magmar,
    [SPECIES_PINSIR] = gMonFootprint_Pinsir,
    [SPECIES_TAUROS] = gMonFootprint_Tauros,
    [SPECIES_MAGIKARP] = gMonFootprint_Magikarp,
    [SPECIES_GYARADOS] = gMonFootprint_Gyarados,
    [SPECIES_LAPRAS] = gMonFootprint_Lapras,
    [SPECIES_DITTO] = gMonFootprint_Ditto,
    [SPECIES_EEVEE] = gMonFootprint_Eevee,
    [SPECIES_VAPOREON] = gMonFootprint_Vaporeon,
    [SPECIES_JOLTEON] = gMonFootprint_Jolteon,
    [SPECIES_FLAREON] = gMonFootprint_Flareon,
    [SPECIES_PORYGON] = gMonFootprint_Porygon,
    [SPECIES_OMANYTE] = gMonFootprint_Omanyte,
    [SPECIES_OMASTAR] = gMonFootprint_Omastar,
    [SPECIES_KABUTO] = gMonFootprint_Kabuto,
    [SPECIES_KABUTOPS] = gMonFootprint_Kabutops,
    [SPECIES_AERODACTYL] = gMonFootprint_Aerodactyl,
    [SPECIES_SNORLAX] = gMonFootprint_Snorlax,
    [SPECIES_ARTICUNO] = gMonFootprint_Articuno,
    [SPECIES_ZAPDOS] = gMonFootprint_Zapdos,
    [SPECIES_MOLTRES] = gMonFootprint_Moltres,
    [SPECIES_DRATINI] = gMonFootprint_Dratini,
    [SPECIES_DRAGONAIR] = gMonFootprint_Dragonair,
    [SPECIES_DRAGONITE] = gMonFootprint_Dragonite,
    [SPECIES_MEWTWO] = gMonFootprint_Mewtwo,
    [SPECIES_MEW] = gMonFootprint_Mew,
    [SPECIES_CHIKORITA] = gMonFootprint_Chikorita,
    [SPECIES_BAYLEEF] = gMonFootprint_Bayleef,
    [SPECIES_MEGANIUM] = gMonFootprint_Meganium,
    [SPECIES_CYNDAQUIL] = gMonFootprint_Cyndaquil,
    [SPECIES_QUILAVA] = gMonFootprint_Quilava,
    [SPECIES_TYPHLOSION] = gMonFootprint_Typhlosion,
    [SPECIES_TOTODILE] = gMonFootprint_Totodile,
    [SPECIES_CROCONAW] = gMonFootprint_Croconaw,
    [SPECIES_FERALIGATR] = gMonFootprint_Feraligatr,
    [SPECIES_SENTRET] = gMonFootprint_Sentret,
    [SPECIES_FURRET] = gMonFootprint_Furret,
    [SPECIES_HOOTHOOT] = gMonFootprint_Hoothoot,
    [SPECIES_NOCTOWL] = gMonFootprint_Noctowl,
    [SPECIES_LEDYBA] = gMonFootprint_Ledyba,
    [SPECIES_LEDIAN] = gMonFootprint_Ledian,
    [SPECIES_SPINARAK] = gMonFootprint_Spinarak,
    [SPECIES_ARIADOS] = gMonFootprint_Ariados,
    [SPECIES_CROBAT] = gMonFootprint_Crobat,
    [SPECIES_CHINCHOU] = gMonFootprint_Chinchou,
    [SPECIES_LANTURN] = gMonFootprint_Lanturn,
    [SPECIES_PICHU] = gMonFootprint_Pichu,
    [SPECIES_CLEFFA] = gMonFootprint_Cleffa,
    [SPECIES_IGGLYBUFF] = gMonFootprint_Igglybuff,
    [SPECIES_TOGEPI] = gMonFootprint_Togepi,
    [SPECIES_TOGETIC] = gMonFootprint_Togetic,
    [SPECIES_NATU] = gMonFootprint_Natu,
    [SPECIES_XATU] = gMonFootprint_Xatu,
    [SPECIES_MAREEP] = gMonFootprint_Mareep,
    [SPECIES_FLAAFFY] = gMonFootprint_Flaaffy,
    [SPECIES_AMPHAROS] = gMonFootprint_Ampharos,
    [SPECIES_BELLOSSOM] = gMonFootprint_Bellossom,
    [SPECIES_MARILL] = gMonFootprint_Marill,
    [SPECIES_AZUMARILL] = gMonFootprint_Azumarill,
    [SPECIES_SUDOWOODO] = gMonFootprint_Sudowoodo,
    [SPECIES_POLITOED] = gMonFootprint_Politoed,
    [SPECIES_HOPPIP] = gMonFootprint_Hoppip,
    [SPECIES_SKIPLOOM] = gMonFootprint_Skiploom,
    [SPECIES_JUMPLUFF] = gMonFootprint_Jumpluff,
    [SPECIES_AIPOM] = gMonFootprint_Aipom,
    [SPECIES_SUNKERN] = gMonFootprint_Sunkern,
    [SPECIES_SUNFLORA] = gMonFootprint_Sunflora,
    [SPECIES_YANMA] = gMonFootprint_Yanma,
    [SPECIES_WOOPER] = gMonFootprint_Wooper,
    [SPECIES_QUAGSIRE] = gMonFootprint_Quagsire,
    [SPECIES_ESPEON] = gMonFootprint_Espeon,
    [SPECIES_UMBREON] = gMonFootprint_Umbreon,
    [SPECIES_MURKROW] = gMonFootprint_Murkrow,
    [SPECIES_SLOWKING] = gMonFootprint_Slowking,
    [SPECIES_MISDREAVUS] = gMonFootprint_Misdreavus,
    [SPECIES_UNOWN] = gMonFootprint_Unown,
    [SPECIES_WOBBUFFET] = gMonFootprint_Wobbuffet,
    [SPECIES_GIRAFARIG] = gMonFootprint_Girafarig,
    [SPECIES_PINECO] = gMonFootprint_Pineco,
    [SPECIES_FORRETRESS] = gMonFootprint_Forretress,
    [SPECIES_DUNSPARCE] = gMonFootprint_Dunsparce,
    [SPECIES_GLIGAR] = gMonFootprint_Gligar,
    [SPECIES_STEELIX] = gMonFootprint_Steelix,
    [SPECIES_SNUBBULL] = gMonFootprint_Snubbull,
    [SPECIES_GRANBULL] = gMonFootprint_Granbull,
    [SPECIES_QWILFISH] = gMonFootprint_Qwilfish,
    [SPECIES_SCIZOR] = gMonFootprint_Scizor,
    [SPECIES_SHUCKLE] = gMonFootprint_Shuckle,
    [SPECIES_HERACROSS] = gMonFootprint_Heracross,
    [SPECIES_SNEASEL] = gMonFootprint_Sneasel,
    [SPECIES_TEDDIURSA] = gMonFootprint_Teddiursa,
    [SPECIES_URSARING] = gMonFootprint_Ursaring,
    [SPECIES_SLUGMA] = gMonFootprint_Slugma,
    [SPECIES_MAGCARGO] = gMonFootprint_Magcargo,
    [SPECIES_SWINUB] = gMonFootprint_Swinub,
    [SPECIES_PILOSWINE] = gMonFootprint_Piloswine,
    [SPECIES_CORSOLA] = gMonFootprint_Corsola,
    [SPECIES_REMORAID] = gMonFootprint_Remoraid,
    [SPECIES_OCTILLERY] = gMonFootprint_Octillery,
    [SPECIES_DELIBIRD] = gMonFootprint_Delibird,
    [SPECIES_MANTINE] = gMonFootprint_Mantine,
    [SPECIES_SKARMORY] = gMonFootprint_Skarmory,
    [SPECIES_HOUNDOUR] = gMonFootprint_Houndour,
    [SPECIES_HOUNDOOM] = gMonFootprint_Houndoom,
    [SPECIES_KINGDRA] = gMonFootprint_Kingdra,
    [SPECIES_PHANPY] = gMonFootprint_Phanpy,
    [SPECIES_DONPHAN] = gMonFootprint_Donphan,
    [SPECIES_PORYGON2] = gMonFootprint_Porygon2,
    [SPECIES_STANTLER] = gMonFootprint_Stantler,
    [SPECIES_SMEARGLE] = gMonFootprint_Smeargle,
    [SPECIES_TYROGUE] = gMonFootprint_Tyrogue,
    [SPECIES_HITMONTOP] = gMonFootprint_Hitmontop,
    [SPECIES_SMOOCHUM] = gMonFootprint_Smoochum,
    [SPECIES_ELEKID] = gMonFootprint_Elekid,
    [SPECIES_MAGBY] = gMonFootprint_Magby,
    [SPECIES_MILTANK] = gMonFootprint_Miltank,
    [SPECIES_BLISSEY] = gMonFootprint_Blissey,
    [SPECIES_RAIKOU] = gMonFootprint_Raikou,
    [SPECIES_ENTEI] = gMonFootprint_Entei,
    [SPECIES_SUICUNE] = gMonFootprint_Suicune,
    [SPECIES_LARVITAR] = gMonFootprint_Larvitar,
    [SPECIES_PUPITAR] = gMonFootprint_Pupitar,
    [SPECIES_TYRANITAR] = gMonFootprint_Tyranitar,
    [SPECIES_LUGIA] = gMonFootprint_Lugia,
    [SPECIES_HO_OH] = gMonFootprint_HoOh,
    [SPECIES_CELEBI] = gMonFootprint_Celebi,
    [SPECIES_TREECKO] = gMonFootprint_Treecko,
    [SPECIES_GROVYLE] = gMonFootprint_Grovyle,
    [SPECIES_SCEPTILE] = gMonFootprint_Sceptile,
    [SPECIES_TORCHIC] = gMonFootprint_Torchic,
    [SPECIES_COMBUSKEN] = gMonFootprint_Combusken,
    [SPECIES_BLAZIKEN] = gMonFootprint_Blaziken,
    [SPECIES_MUDKIP] = gMonFootprint_Mudkip,
    [SPECIES_MARSHTOMP] = gMonFootprint_Marshtomp,
    [SPECIES_SWAMPERT] = gMonFootprint_Swampert,
    [SPECIES_POOCHYENA] = gMonFootprint_Poochyena,
    [SPECIES_MIGHTYENA] = gMonFootprint_Mightyena,
    [SPECIES_ZIGZAGOON] = gMonFootprint_Zigzagoon,
    [SPECIES_LINOONE] = gMonFootprint_Linoone,
    [SPECIES_WURMPLE] = gMonFootprint_Wurmple,
    [SPECIES_SILCOON] = gMonFootprint_Silcoon,
    [SPECIES_BEAUTIFLY] = gMonFootprint_Beautifly,
    [SPECIES_CASCOON] = gMonFootprint_Cascoon,
    [SPECIES_DUSTOX] = gMonFootprint_Dustox,
    [SPECIES_LOTAD] = gMonFootprint_Lotad,
    [SPECIES_LOMBRE] = gMonFootprint_Lombre,
    [SPECIES_LUDICOLO] = gMonFootprint_Ludicolo,
    [SPECIES_SEEDOT] = gMonFootprint_Seedot,
    [SPECIES_NUZLEAF] = gMonFootprint_Nuzleaf,
    [SPECIES_SHIFTRY] = gMonFootprint_Shiftry,
    [SPECIES_NINCADA] = gMonFootprint_Nincada,
    [SPECIES_NINJASK] = gMonFootprint_Ninjask,
    [SPECIES_SHEDINJA] = gMonFootprint_Shedinja,
    [SPECIES_TAILLOW] = gMonFootprint_Taillow,
    [SPECIES_SWELLOW] = gMonFootprint_Swellow,
    [SPECIES_SHROOMISH] = gMonFootprint_Shroomish,
    [SPECIES_BRELOOM] = gMonFootprint_Breloom,
    [SPECIES_SPINDA] = gMonFootprint_Spinda,
    [SPECIES_WINGULL] = gMonFootprint_Wingull,
    [SPECIES_PELIPPER] = gMonFootprint_Pelipper,
    [SPECIES_SURSKIT] = gMonFootprint_Surskit,
    [SPECIES_MASQUERAIN] = gMonFootprint_Masquerain,
    [SPECIES_WAILMER] = gMonFootprint_Wailmer,
    [SPECIES_WAILORD] = gMonFootprint_Wailord,
    [SPECIES_SKITTY] = gMonFootprint_Skitty,
    [SPECIES_DELCATTY] = gMonFootprint_Delcatty,
    [SPECIES_KECLEON] = gMonFootprint_Kecleon,
    [SPECIES_BALTOY] = gMonFootprint_Baltoy,
    [SPECIES_CLAYDOL] = gMonFootprint_Claydol,
    [SPECIES_NOSEPASS] = gMonFootprint_Nosepass,
    [SPECIES_TORKOAL] = gMonFootprint_Torkoal,
    [SPECIES_SABLEYE] = gMonFootprint_Sableye,
    [SPECIES_BARBOACH] = gMonFootprint_Barboach,
    [SPECIES_WHISCASH] = gMonFootprint_Whiscash,
    [SPECIES_LUVDISC] = gMonFootprint_Luvdisc,
    [SPECIES_CORPHISH] = gMonFootprint_Corphish,
    [SPECIES_CRAWDAUNT] = gMonFootprint_Crawdaunt,
    [SPECIES_FEEBAS] = gMonFootprint_Feebas,
    [SPECIES_MILOTIC] = gMonFootprint_Milotic,
    [SPECIES_CARVANHA] = gMonFootprint_Carvanha,
    [SPECIES_SHARPEDO] = gMonFootprint_Sharpedo,
    [SPECIES_TRAPINCH] = gMonFootprint_Trapinch,
    [SPECIES_VIBRAVA] = gMonFootprint_Vibrava,
    [SPECIES_FLYGON] = gMonFootprint_Flygon,
    [SPECIES_MAKUHITA] = gMonFootprint_Makuhita,
    [SPECIES_HARIYAMA] = gMonFootprint_Hariyama,
    [SPECIES_ELECTRIKE] = gMonFootprint_Electrike,
    [SPECIES_MANECTRIC] = gMonFootprint_Manectric,
    [SPECIES_NUMEL] = gMonFootprint_Numel,
    [SPECIES_CAMERUPT] = gMonFootprint_Camerupt,
    [SPECIES_SPHEAL] = gMonFootprint_Spheal,
    [SPECIES_SEALEO] = gMonFootprint_Sealeo,
    [SPECIES_WALREIN] = gMonFootprint_Walrein,
    [SPECIES_CACNEA] = gMonFootprint_Cacnea,
    [SPECIES_CACTURNE] = gMonFootprint_Cacturne,
    [SPECIES_SNORUNT] = gMonFootprint_Snorunt,
    [SPECIES_GLALIE] = gMonFootprint_Glalie,
    [SPECIES_LUNATONE] = gMonFootprint_Lunatone,
    [SPECIES_SOLROCK] = gMonFootprint_Solrock,
    [SPECIES_AZURILL] = gMonFootprint_Azurill,
    [SPECIES_SPOINK] = gMonFootprint_Spoink,
    [SPECIES_GRUMPIG] = gMonFootprint_Grumpig,
    [SPECIES_PLUSLE] = gMonFootprint_Plusle,
    [SPECIES_MINUN] = gMonFootprint_Minun,
    [SPECIES_MAWILE] = gMonFootprint_Mawile,
    [SPECIES_MEDITITE] = gMonFootprint_Meditite,
    [SPECIES_MEDICHAM] = gMonFootprint_Medicham,
    [SPECIES_SWABLU] = gMonFootprint_Swablu,
    [SPECIES_ALTARIA] = gMonFootprint_Altaria,
    [SPECIES_WYNAUT] = gMonFootprint_Wynaut,
    [SPECIES_DUSKULL] = gMonFootprint_Duskull,
    [SPECIES_DUSCLOPS] = gMonFootprint_Dusclops,
    [SPECIES_ROSELIA] = gMonFootprint_Roselia,
    [SPECIES_SLAKOTH] = gMonFootprint_Slakoth,
    [SPECIES_VIGOROTH] = gMonFootprint_Vigoroth,
    [SPECIES_SLAKING] = gMonFootprint_Slaking,
    [SPECIES_GULPIN] = gMonFootprint_Gulpin,
    [SPECIES_SWALOT] = gMonFootprint_Swalot,
    [SPECIES_TROPIUS] = gMonFootprint_Tropius,
    [SPECIES_WHISMUR] = gMonFootprint_Whismur,
    [SPECIES_LOUDRED] = gMonFootprint_Loudred,
    [SPECIES_EXPLOUD] = gMonFootprint_Exploud,
    [SPECIES_CLAMPERL] = gMonFootprint_Clamperl,
    [SPECIES_HUNTAIL] = gMonFootprint_Huntail,
    [SPECIES_GOREBYSS] = gMonFootprint_Gorebyss,
    [SPECIES_ABSOL] = gMonFootprint_Absol,
    [SPECIES_SHUPPET] = gMonFootprint_Shuppet,
    [SPECIES_BANETTE] = gMonFootprint_Banette,
    [SPECIES_SEVIPER] = gMonFootprint_Seviper,
    [SPECIES_ZANGOOSE] = gMonFootprint_Zangoose,
    [SPECIES_RELICANTH] = gMonFootprint_Relicanth,
    [SPECIES_ARON] = gMonFootprint_Aron,
    [SPECIES_LAIRON] = gMonFootprint_Lairon,
    [SPECIES_AGGRON] = gMonFootprint_Aggron,
    [SPECIES_CASTFORM] = gMonFootprint_Castform,
    [SPECIES_VOLBEAT] = gMonFootprint_Volbeat,
    [SPECIES_ILLUMISE] = gMonFootprint_Illumise,
    [SPECIES_LILEEP] = gMonFootprint_Lileep,
    [SPECIES_CRADILY] = gMonFootprint_Cradily,
    [SPECIES_ANORITH] = gMonFootprint_Anorith,
    [SPECIES_ARMALDO] = gMonFootprint_Armaldo,
    [SPECIES_RALTS] = gMonFootprint_Ralts,
    [SPECIES_KIRLIA] = gMonFootprint_Kirlia,
    [SPECIES_GARDEVOIR] = gMonFootprint_Gardevoir,
    [SPECIES_BAGON] = gMonFootprint_Bagon,
    [SPECIES_SHELGON] = gMonFootprint_Shelgon,
    [SPECIES_SALAMENCE] = gMonFootprint_Salamence,
    [SPECIES_BELDUM] = gMonFootprint_Beldum,
    [SPECIES_METANG] = gMonFootprint_Metang,
    [SPECIES_METAGROSS] = gMonFootprint_Metagross,
    [SPECIES_REGIROCK] = gMonFootprint_Regirock,
    [SPECIES_REGICE] = gMonFootprint_Regice,
    [SPECIES_REGISTEEL] = gMonFootprint_Registeel,
    [SPECIES_KYOGRE] = gMonFootprint_Kyogre,
    [SPECIES_GROUDON] = gMonFootprint_Groudon,
    [SPECIES_RAYQUAZA] = gMonFootprint_Rayquaza,
    [SPECIES_LATIAS] = gMonFootprint_Latias,
    [SPECIES_LATIOS] = gMonFootprint_Latios,
    [SPECIES_JIRACHI] = gMonFootprint_Jirachi,
    [SPECIES_DEOXYS] = gMonFootprint_Deoxys,
    [SPECIES_CHIMECHO] = gMonFootprint_Chimecho,
    [SPECIES_EGG] = gMonFootprint_Bulbasaur,
};
static const u8 gUnknown_083B5558[] = _("{CLEAR_TO 0}");
const u8 gUnknown_083B555C[] = INCBIN_U8("graphics/unknown/unknown_3B555C.bin");
static const struct OamData gOamData_83B557C =
{
    .y = 0,
    .affineMode = 0,
    .objMode = 0,
    .mosaic = 0,
    .bpp = 0,
    .shape = 0,
    .x = 0,
    .matrixNum = 0,
    .size = 3,
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0,
};
static void *const gUnknown_083B5584[] =
{
    ePokedexPalAddr1,
    ePokedexPalAddr2,
    ePokedexPalAddr3,
    ePokedexPalAddr4,
};
static const struct SpriteFrameImage gSpriteImageTable_83B5594[] =
{
    {ewram8000, 0x800},
    {ewram8800, 0x800},
    {ewram9000, 0x800},
    {ewram9800, 0x800},
    {ewramA000, 0x800},
    {ewramA800, 0x800},
    {ewramB000, 0x800},
    {ewramB800, 0x800},
    {ewramC000, 0x800},
    {ewramC800, 0x800},
    {ewramD000, 0x800},
    {ewramD800, 0x800},
    {ewramE000, 0x800},
    {ewramE800, 0x800},
    {ewramF000, 0x800},
    {ewramF800, 0x800},
};
static const struct SpriteFrameImage gSpriteImageTable_83B5614[] =
{
    {ewramC000, 0x800},
    {ewramC800, 0x800},
    {ewramD000, 0x800},
    {ewramD800, 0x800},
    {ewramE000, 0x800},
    {ewramE800, 0x800},
    {ewramF000, 0x800},
    {ewramF800, 0x800},
    {ewram10000, 0x800},
    {ewram10800, 0x800},
    {ewram11000, 0x800},
    {ewram11800, 0x800},
    {ewram12000, 0x800},
    {ewram12800, 0x800},
    {ewram13000, 0x800},
    {ewram13800, 0x800},
};
static const struct SpriteFrameImage gSpriteImageTable_83B5694[] =
{
    {ewram10000, 0x800},
    {ewram10800, 0x800},
    {ewram11000, 0x800},
    {ewram11800, 0x800},
    {ewram12000, 0x800},
    {ewram12800, 0x800},
    {ewram13000, 0x800},
    {ewram13800, 0x800},
    {ewram14000, 0x800},
    {ewram14800, 0x800},
    {ewram15000, 0x800},
    {ewram15800, 0x800},
    {ewram16000_2, 0x800},
    {ewram16800, 0x800},
    {ewram17000, 0x800},
    {ewram17800_2, 0x800},
};
static const struct SpriteFrameImage gSpriteImageTable_83B5714[] =
{
    {ewram14000, 0x800},
    {ewram14800, 0x800},
    {ewram15000, 0x800},
    {ewram15800, 0x800},
    {ewram16000_2, 0x800},
    {ewram16800, 0x800},
    {ewram17000, 0x800},
    {ewram17800_2, 0x800},
    {ewram18000_2, 0x800},
    {ewram18800, 0x800},
    {ewram19000, 0x800},
    {ewram19800, 0x800},
    {ewram1A000, 0x800},
    {ewram1A800, 0x800},
    {ewram1B000_2, 0x800},
    {ewram1B800, 0x800},
};
static const struct SpriteFrameImage *const gUnknown_083B5794[] =
{
    gSpriteImageTable_83B5594,
    gSpriteImageTable_83B5614,
    gSpriteImageTable_83B5694,
    gSpriteImageTable_83B5714,
};
static void nullsub_59(struct Sprite *);
static const struct SpriteTemplate gUnknown_083B57A4 =
{
    .tileTag = 0xFFFF,
    .paletteTag = 0,
    .oam = &gOamData_83B557C,
    .anims = NULL,
    .images = gSpriteImageTable_83B5594,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = nullsub_59,
};
static const u8 gUnknown_083B57BC[][4] =
{
    {0,      0, 0,      0},
    {CHAR_A, 3, CHAR_a, 3},
    {CHAR_D, 3, CHAR_d, 3},
    {CHAR_G, 3, CHAR_g, 3},
    {CHAR_J, 3, CHAR_j, 3},
    {CHAR_M, 3, CHAR_m, 3},
    {CHAR_P, 3, CHAR_p, 3},
    {CHAR_S, 3, CHAR_s, 3},
    {CHAR_V, 3, CHAR_v, 3},
    {CHAR_Y, 2, CHAR_y, 2},
};
static const struct UnknownStruct3 gUnknown_083B57E4[] =
{
    {DexText_SearchForPoke, 0,  0, 5},
    {DexText_SwitchDex,     6,  0, 5},
    {DexText_ReturnToDex,  12,  0, 5},
};
static const struct UnknownStruct4 gUnknown_083B57FC[] =
{
    {DexText_ListByABC,           0,  2,  5,  5,  2, 12},
    {DexText_ListByColor,         0,  4,  5,  5,  4, 12},
    {DexText_ListByType,          0,  6,  5,  5,  6,  6},
    {DexText_ListByType,          0,  6,  5, 11,  6,  6},
    {DexText_SelectDexList,       0,  8,  5,  5,  8, 12},
    {DexText_SelectDexMode,       0, 10,  5,  5, 10, 12},
    {DexText_ExecuteSearchSwitch, 0, 12,  5,  0,  0,  0},
};
static const u8 gUnknown_083B5850[][4] =
{
    {0xFF, 0xFF, 0xFF,    1},
    {0xFF, 0xFF,    0,    2},
    {0xFF,    3,    1,    4},
    {   2, 0xFF,    1,    4},
    {0xFF, 0xFF,    2,    5},
    {0xFF, 0xFF,    4,    6},
    {0xFF, 0xFF,    5, 0xFF},
};
static const u8 gUnknown_083B586C[][4] =
{
    {0xFF, 0xFF, 0xFF, 0xFF},
    {0xFF, 0xFF, 0xFF, 0xFF},
    {0xFF, 0xFF, 0xFF, 0xFF},
    {0xFF, 0xFF, 0xFF, 0xFF},
    {0xFF, 0xFF, 0xFF,    5},
    {0xFF, 0xFF,    4,    6},
    {0xFF, 0xFF,    5, 0xFF},
};
static const u8 gUnknown_083B5888[][4] =
{
    {0xFF, 0xFF, 0xFF,    1},
    {0xFF, 0xFF,    0,    2},
    {0xFF,    3,    1,    4},
    {   2, 0xFF,    1,    4},
    {0xFF, 0xFF,    2,    6},
    {0xFF, 0xFF, 0xFF, 0xFF},
    {0xFF, 0xFF,    4, 0xFF},
};
static const u8 gUnknown_083B58A4[][4] =
{
    {0xFF, 0xFF, 0xFF, 0xFF},
    {0xFF, 0xFF, 0xFF, 0xFF},
    {0xFF, 0xFF, 0xFF, 0xFF},
    {0xFF, 0xFF, 0xFF, 0xFF},
    {0xFF, 0xFF, 0xFF,    6},
    {0xFF, 0xFF, 0xFF, 0xFF},
    {0xFF, 0xFF,    4, 0xFF},
};
static const struct UnknownStruct2 gUnknown_083B58C0[] =
{
    {DexText_HoennDex2,    DexText_HoennDex},
    {DexText_NationalDex2, DexText_NationalDex},
    {NULL, NULL},
};
static const struct UnknownStruct2 gUnknown_083B58D8[] =
{
    {DexText_ListByNumber,          DexText_NumericalMode},
    {DexText_ListByABC2,            DexText_ABCMode},
    {DexText_ListByHeavyToLightest, DexText_HeaviestMode},
    {DexText_ListByLightToHeaviest, DexText_LightestMode},
    {DexText_ListByTallToSmallest,  DexText_TallestMode},
    {DexText_ListBySmallToTallest,  DexText_SmallestMode},
    {NULL, NULL},
};
static const struct UnknownStruct2 gUnknown_083B5910[] =
{
    {DexText_Terminator5, DexText_DontSpecify},
    {DexText_Terminator5, DexText_ABC},
    {DexText_Terminator5, DexText_DEF},
    {DexText_Terminator5, DexText_GHI},
    {DexText_Terminator5, DexText_JKL},
    {DexText_Terminator5, DexText_MNO},
    {DexText_Terminator5, DexText_PQR},
    {DexText_Terminator5, DexText_STU},
    {DexText_Terminator5, DexText_VWX},
    {DexText_Terminator5, DexText_YZ},
    {NULL, NULL},
};
static const struct UnknownStruct2 gUnknown_083B5968[] =
{
    {DexText_Terminator5, DexText_DontSpecify},
    {DexText_Terminator5, DexText_Red},
    {DexText_Terminator5, DexText_Blue},
    {DexText_Terminator5, DexText_Yellow},
    {DexText_Terminator5, DexText_Green},
    {DexText_Terminator5, DexText_Black},
    {DexText_Terminator5, DexText_Brown},
    {DexText_Terminator5, DexText_Purple},
    {DexText_Terminator5, DexText_Gray},
    {DexText_Terminator5, DexText_White},
    {DexText_Terminator5, DexText_Pink},
    {NULL, NULL},
};
static const struct UnknownStruct2 gUnknown_083B59C8[] =
{
    {DexText_Terminator5, DexText_None},
    {DexText_Terminator5, gTypeNames[TYPE_NORMAL]},
    {DexText_Terminator5, gTypeNames[TYPE_FIGHTING]},
    {DexText_Terminator5, gTypeNames[TYPE_FLYING]},
    {DexText_Terminator5, gTypeNames[TYPE_POISON]},
    {DexText_Terminator5, gTypeNames[TYPE_GROUND]},
    {DexText_Terminator5, gTypeNames[TYPE_ROCK]},
    {DexText_Terminator5, gTypeNames[TYPE_BUG]},
    {DexText_Terminator5, gTypeNames[TYPE_GHOST]},
    {DexText_Terminator5, gTypeNames[TYPE_STEEL]},
    {DexText_Terminator5, gTypeNames[TYPE_FIRE]},
    {DexText_Terminator5, gTypeNames[TYPE_WATER]},
    {DexText_Terminator5, gTypeNames[TYPE_GRASS]},
    {DexText_Terminator5, gTypeNames[TYPE_ELECTRIC]},
    {DexText_Terminator5, gTypeNames[TYPE_PSYCHIC]},
    {DexText_Terminator5, gTypeNames[TYPE_ICE]},
    {DexText_Terminator5, gTypeNames[TYPE_DRAGON]},
    {DexText_Terminator5, gTypeNames[TYPE_DARK]},
    {DexText_Terminator5, gTypeNames[TYPE_FAIRY]},
    {NULL, NULL},
};
static const u8 gUnknown_083B5A60[] = {0, 1};
static const u8 gUnknown_083B5A62[] = {0, 1, 2, 3, 4, 5};
static const u8 gUnknown_083B5A68[] = {0xFF, TYPE_NORMAL, TYPE_FIGHTING, TYPE_FLYING, TYPE_POISON, TYPE_GROUND, TYPE_ROCK, TYPE_BUG, TYPE_GHOST, TYPE_STEEL, TYPE_FIRE, TYPE_WATER, TYPE_GRASS, TYPE_ELECTRIC, TYPE_PSYCHIC, TYPE_ICE, TYPE_DRAGON, TYPE_DARK, TYPE_FAIRY};
static const struct UnknownStruct1 gUnknown_083B5A7C[] =
{
    {gUnknown_083B5910,  6,  7, 10},
    {gUnknown_083B5968,  8,  9, 11},
    {gUnknown_083B59C8, 10, 11, NUMBER_OF_MON_TYPES},
    {gUnknown_083B59C8, 12, 13, NUMBER_OF_MON_TYPES},
    {gUnknown_083B58D8,  4,  5,  6},
    {gUnknown_083B58C0,  2,  3,  2},
};
static const u8 gUnknown_083B5AAC[] = _("{STR_VAR_1}{CLEAR_TO 43}");
static const u8 gUnknown_083B5AB2[] = _("{STR_VAR_1}{CLEAR_TO 96}");

static void MainCB(void);
static void Task_PokedexShowMainScreen(u8 taskId);
static void Task_PokedexMainScreen(u8 taskId);
static void WaitForScrollMonSpritesMainPage(u8 taskId);
static void Task_PokedexMainScreenMenu(u8 taskId);
static void sub_808CA64(u8 taskId);
static void sub_808CAE4(u8 taskId);
static void sub_808CB8C(u8 taskId);
static void Task_ClosePokedex(u8 taskId);
static void sub_808CCC4(u8 taskId);
static void Task_PokedexResultsScreen(u8 taskId);
static void WaitForScrollMonSpritesResultsScreen(u8 taskId);
static void Task_PokedexResultsScreenMenu(u8 taskId);
static void sub_808D118(u8 taskId);
static void sub_808D198(u8 taskId);
static void Task_PokedexResultsScreenReturnToMainScreen(u8 taskId);
static void Task_PokedexResultsScreenExitPokedex(u8 taskId);
static bool8 DisplayMainScreen(u8 screen);
static void sub_808D640(void);
static void SortPokedex(u8 dexMode, u8 sortMode);
static void PrintDexNumberSlots(u16 index, u8 stringLength, u8 c, u16 d);
static void PrintCaughtBall(u16 owned, u8 b, u8 c, u16 d);
static u8 PrintMonNameMainPage(u16 num, u8 b, u8 c);
static void FreeVRAMSpaceForMonNameText(u8 a, u8 b, u16 c);
static void DrawSurroundingMons(u16 num, u16 b);
static bool8 ScrollMonSprites(u8 direction, u8 pixels, u8 iterations);
static u16 ProcessDPadInputsMainPage(u16 currentMon, u16 b);
static void SetSelectedSpriteId(void);
static u8 HandleUpDownInputPageScreen(void);
static u8 DestroySpritesForSkips(void);
static u16 ParseDexNumberForSprite(u16 num);
static u32 SetUpMonPic(u16 num, s16 x, s16 y);
static void DrawCursorsAndText(u8 a);
static void MoveMonPosForDesc(struct Sprite *sprite);
static void sub_808EE28(struct Sprite *sprite);
static u8 sub_808F210(struct PokedexListItem *, u8);
static bool8 sub_808F250(u8 taskId);
static u8 sub_808F284(struct PokedexListItem *, u8);
static void Task_InitPageScreenMultistep(u8 taskId);
static void Task_PageScreenProcessInput(u8 taskId);
static void sub_808F888(u8 taskId);
static void Task_ClosePageScreen(u8 taskId);
static void Task_InitAreaScreenMultistep(u8 taskId);
static void Task_AreaScreenProcessInput(u8 taskId);
static void sub_808FA00(u8 taskId);
static void Task_InitCryScreenMultistep(u8 taskId);
static void Task_CryScreenProcessInput(u8 taskId);
static void sub_808FFBC(u8 taskId);
static void sub_8090040(u8 a);
static void Task_InitSizeScreenMultistep(u8 taskId);
static void Task_SizeScreenProcessInput(u8 taskId);
static void sub_8090498(u8 taskId);
static void sub_80904FC(u16 a);
static void sub_8090540(u16 a);
static void sub_8090584(u8 a, u16 b);
static void sub_8090644(u8 a, u16 b);
static void DisplayDexForNewMon(u8);
static void sub_8090A3C(u8);
static void sub_8090B8C(u8);
static void sub_8090C28(struct Sprite *);
static void InitBGAndObj(u16);
static void sub_8091154(u16 order, u8, u8);
static u8 sub_80911C8(u16 num, u8, u8);
static u8 sub_8091260(u16 num, u8, u8, u8);
static void sub_8091304(const u8 *name, u8, u8);
static void sub_8091458(u16 height, u8 i, u8 i1);
static void sub_8091564(u16 weight, u8 i, u8 i1);
void PrintFootprints(u16, u16, u16);
static void sub_80917CC(u16 i, u16 i1);
static u16 GetSlotNumbersFromMinMax(u8, u16, u16, u16);
u16 DisplayMonPic(u16 num, s16 x, s16 y, u16 paletteNum);
static void sub_8091E54(u8);
static void sub_809204C(u8);
static void sub_809207C(u8);
static void sub_809217C(u8);
static void HandleInputSearchScreen(u8);
static void sub_80923FC(u8);
static void sub_80924A4(u8);
static void sub_8092508(u8);
static void sub_80925CC(u8);
static void sub_8092644(u8);
static void sub_80927B8(u8);
static void sub_80927F0(u8);
static void sub_8092AB0(u8);
static void sub_8092AD4(u8, u8);
static void sub_8092B68(u8);
static void sub_8092C8C(u8);
static void sub_8092D78(u8);
static u8 sub_8092E10(u8, u8);
static void sub_8092EB0(u8);
static void sub_809308C(u8);

// mon pic positions for displaying the image

#define X_POSITION_FOR_DESCRIPTION_PAGE 48
#define Y_POSITION_FOR_DESCRIPTION_PAGE 56

#define X_POSITION_FOR_CRY_PAGE 48
#define Y_POSITION_FOR_CRY_PAGE 56

#define X_POSITION_FOR_SCROLLING 96
#define Y_POSITION_FOR_SCROLLING 80

// various text positions

#define DESCRIPTION_X_POSITION 1
#define DESCRIPTION_Y_POSITION 12

void ResetPokedex(void)
{
    u16 i;

    gUnknown_0202FFB8 = 0;
    gUnknown_0202FFBA = 0x40;
    gUnknown_03005CE8 = 0;
    gSaveBlock2.pokedex.unknown1 = 0;
    gSaveBlock2.pokedex.order = 0;
    gSaveBlock2.pokedex.nationalMagic = 0;
    gSaveBlock2.pokedex.unknown2 = 0;
    gSaveBlock2.pokedex.unownPersonality = 0;
    gSaveBlock2.pokedex.spindaPersonality = 0;
    gSaveBlock2.pokedex.unknown3 = 0;
    DisableNationalPokedex();
    for (i = 0; i < DEX_FLAGS_NO; i++)
    {
        gSaveBlock2.pokedex.owned[i] = 0;
        gSaveBlock2.pokedex.seen[i] = 0;
    }
}

void sub_808C0A0(void)
{
    gUnknown_0202FFB8 = 0;
    gUnknown_0202FFBA = 0x40;
}

static void sub_808C0B8(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void ClearPokedexView(struct PokedexView *pokedexView)
{
    u16 i;

    for (i = 0; i < NATIONAL_DEX_COUNT; i++)
    {
        pokedexView->unk0[i].dexNum |= 0xFFFF;
        pokedexView->unk0[i].seen = 0;
        pokedexView->unk0[i].owned = 0;
    }
    pokedexView->unk608 = 0;
    pokedexView->unk60A_1 = 0;
    pokedexView->unk60A_2 = 0;
    pokedexView->pokemonListCount = 0;
    pokedexView->selectedPokemon = 0;
    pokedexView->unk610 = 0;
    pokedexView->dexMode = 0;
    pokedexView->unk614 = 0;
    pokedexView->dexOrder = 0;
    pokedexView->unk618 = 0;
    pokedexView->seenPokemon = 0;
    pokedexView->ownedPokemon = 0;
    for (i = 0; i <= 3; i++)
        pokedexView->spriteIds[i] |= 0xFFFF;
    pokedexView->changeInSelectedNamePixelPos = 0;
    pokedexView->unk62A = 0;
    pokedexView->selectedNamePixelPosition = 0;
    pokedexView->unk62D = 0;
    pokedexView->iterationsOfScrolling = 0;
    pokedexView->scrollDirection = 0;
    pokedexView->unk630 = 0;
    pokedexView->unk632 = 0;
    pokedexView->pixelsToScrollPerIteration = 0;
    pokedexView->iterationsOfScrollingConstant = 0;
    pokedexView->timeHeldDPad = 0;
    for (i = 0; i <= 3; i++)
        pokedexView->unk63A[i] = 0;
    pokedexView->currentScreen = MAIN_DEXPAGE;
    pokedexView->unk64B = 0;
    pokedexView->unk64C_1 = 0;
    pokedexView->selectedScreen = 0;
    pokedexView->unk64F = 0;
    pokedexView->menuIsOpen = 0;
    pokedexView->menuCursorPos = 0;
    pokedexView->menuY = 0;
    for (i = 0; i <= 7; i++)
        pokedexView->unk656[i] = 0;
    for (i = 0; i <= 7; i++)
        pokedexView->unk65E[i] = 0;
}

void CB2_InitPokedex(void)
{
    switch (gMain.state)
    {
    case 0:
    default:
        SetVBlankCallback(NULL);
        InitBGAndObj(0); // init everything
        DmaFill16Large(3, 0, (void *)(VRAM + 0x0), VRAM_SIZE, 0x1000);
        DmaClear32(3, OAM, OAM_SIZE);
        DmaClear16(3, PLTT, PLTT_SIZE);
        gMain.state = 1;
        break;
    case 1:
        ScanlineEffect_Stop();
        ResetTasks();
        ResetSpriteData();
        ResetPaletteFade();
        FreeAllSpritePalettes();
        gReservedSpritePaletteCount = 8;
        Text_LoadWindowTemplate(&gWindowTemplate_81E7048);
        InitMenuWindow(&gWindowTemplate_81E7048);
        gMain.state++;
        break;
    case 2:
        switch (Random() & 3)
        {
        case 0:
        default:
            gPokedexView = ePokedexView1;
            break;
        case 1:
            gPokedexView = ePokedexView2;
            break;
        case 2:
            gPokedexView = ePokedexView3;
            break;
        case 3:
            gPokedexView = ePokedexView4;
            break;
        }
        ClearPokedexView(gPokedexView);
        CreateTask(Task_PokedexShowMainScreen, 0);
        gPokedexView->dexMode = gSaveBlock2.pokedex.unknown1;
        if (!IsNationalPokedexEnabled())
            gPokedexView->dexMode = DEX_MODE_HOENN;
        gPokedexView->dexOrder = gSaveBlock2.pokedex.order;
        gPokedexView->selectedPokemon = gUnknown_0202FFB8;
        gPokedexView->selectedNamePixelPosition = gUnknown_0202FFBA;
        gPokedexView->selectedScreen = PAGE_SCREEN;
        gPokedexView->descriptionPageNum = 0;
        if (!IsNationalPokedexEnabled())
        {
            gPokedexView->seenPokemon = GetHoennPokedexCount(0);
            gPokedexView->ownedPokemon = GetHoennPokedexCount(1);
        }
        else
        {
            gPokedexView->seenPokemon = GetNationalPokedexCount(0);
            gPokedexView->ownedPokemon = GetNationalPokedexCount(1);
        }
        gPokedexView->unk62D = 8;
        gMain.state++;
        break;
    case 3:
        {
            u16 savedIme;

            savedIme = REG_IME;
            REG_IME = 0;
            REG_IE |= 1;
            REG_IME = savedIme;
            REG_DISPSTAT |= 8;
            SetVBlankCallback(sub_808C0B8);
            SetMainCallback2(MainCB);
            SortPokedex(gPokedexView->dexMode, gPokedexView->dexOrder);
            m4aMPlayVolumeControl(&gMPlayInfo_BGM, 0xFFFF, 0x80);
        }
        break;
    }
}

u8 unref_sub_808C540(void (*func)(u8))
{
    u16 savedIme;
    u8 taskId;

    SetVBlankCallback(NULL);
    InitBGAndObj(0x200); // init all but bg1
    ScanlineEffect_Stop();
    ResetTasks();
    ResetPaletteFade();
    savedIme = REG_IME;
    REG_IME = 0;
    REG_IE |= 1;
    REG_IME = savedIme;
    REG_DISPSTAT |= 0x8;
    SetVBlankCallback(sub_808C0B8);
    SetMainCallback2(MainCB);
    if (!gPokedexView->unk64B)
        taskId = CreateTask(sub_808CAE4, 0);
    else
        taskId = CreateTask(sub_808D198, 0);
    gTasks[taskId].data[0] = CreateTask(func, 0);
    return gTasks[taskId].data[0];
}

static void MainCB(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void Task_PokedexShowMainScreen(u8 taskId)
{
    gPokedexView->unk64C_1 = 0;
    if (DisplayMainScreen(MAIN_DEXPAGE))
        gTasks[taskId].func = Task_PokedexMainScreen;
}

//Hide menu and process input on main screen
void Task_PokedexMainScreen(u8 taskId)
{
    REG_BG0VOFS = gPokedexView->menuY;

    //If menu is open, slide it down, off screen
    if (gPokedexView->menuY)
        gPokedexView->menuY -= 8;
    else
    {
        if ((gMain.newKeys & A_BUTTON) && gPokedexView->unk0[gPokedexView->selectedPokemon].seen)
        {
            SetSelectedSpriteId();
            BeginNormalPaletteFade(
              ~(1 << (gSprites[gPokedexView->selectedMonSpriteId].oam.paletteNum + 16)),
              0, 0, 16, RGB(0, 0, 0));
            gSprites[gPokedexView->selectedMonSpriteId].callback = MoveMonPosForDesc;
            gTasks[taskId].func = sub_808CA64;
            PlaySE(SE_PIN);
        }
        else if (gMain.newKeys & START_BUTTON)
        {
            //Open menu
            gPokedexView->menuY = 0;
            gPokedexView->menuIsOpen = 1;
            gPokedexView->menuCursorPos = 0;
            gTasks[taskId].func = Task_PokedexMainScreenMenu;
            PlaySE(SE_SELECT);
        }
        else if (gMain.newKeys & SELECT_BUTTON)
        {
            PlaySE(SE_SELECT);
            BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB(0, 0, 0));
            gTasks[taskId].data[0] = sub_8091E3C();
            gPokedexView->unk64F = 0;
            gPokedexView->unk62A = gPokedexView->selectedNamePixelPosition;
            gPokedexView->unk610 = gPokedexView->selectedPokemon;
            gPokedexView->unk614 = gPokedexView->dexMode;
            gPokedexView->unk618 = gPokedexView->dexOrder;
            gTasks[taskId].func = sub_808CB8C;
            PlaySE(SE_PC_LOGON);
        }
        else if (gMain.newKeys & B_BUTTON)
        {
            BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB(0, 0, 0));
            gTasks[taskId].func = Task_ClosePokedex;
            PlaySE(SE_PC_OFF);
        }
        else
        {
            gPokedexView->selectedPokemon = ProcessDPadInputsMainPage(gPokedexView->selectedPokemon, 0xE);
            if (gPokedexView->iterationsOfScrolling)
                gTasks[taskId].func = WaitForScrollMonSpritesMainPage;
        }
    }
}

static void WaitForScrollMonSpritesMainPage(u8 taskId)
{
    if (ScrollMonSprites(gPokedexView->scrollDirection, gPokedexView->pixelsToScrollPerIteration, gPokedexView->iterationsOfScrollingConstant))
        gTasks[taskId].func = Task_PokedexMainScreen;
}

//Bring up menu and process menu input
static void Task_PokedexMainScreenMenu(u8 taskId)
{
    REG_BG0VOFS = gPokedexView->menuY;

    //If menu is not open, slide it up, on screen
    if (gPokedexView->menuY != 80)
        gPokedexView->menuY += 8;
    else
    {
        if (gMain.newKeys & A_BUTTON)
        {
            switch (gPokedexView->menuCursorPos)
            {
            case 0: //BACK TO LIST
            default:
                gMain.newKeys |= START_BUTTON;  //Exit menu
                break;
            case 1: //LIST TOP
                gPokedexView->selectedPokemon = 0;
                gPokedexView->selectedNamePixelPosition = 0x40; // start at top plus 4 positions (because the blanks above)
                DestroySpritesForSkips();
                DrawSurroundingMons(gPokedexView->selectedPokemon, 0xE);
                gMain.newKeys |= START_BUTTON;  //Exit menu
                break;
            case 2: //LIST BOTTOM
                gPokedexView->selectedPokemon = gPokedexView->pokemonListCount - 1;
                gPokedexView->selectedNamePixelPosition = (gPokedexView->pokemonListCount - 1) * 16 + 0x40; // start at bottom
                DestroySpritesForSkips();
                DrawSurroundingMons(gPokedexView->selectedPokemon, 0xE);
                gMain.newKeys |= START_BUTTON;  //Exit menu
                break;
            case 3: //CLOSE POKEDEX
                BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB(0, 0, 0));
                gTasks[taskId].func = Task_ClosePokedex;
                PlaySE(SE_PC_OFF);
                break;
            }
        }

        //Exit menu when Start or B is pressed
        if (gMain.newKeys & (START_BUTTON | B_BUTTON))
        {
            gPokedexView->menuIsOpen = 0;
            gTasks[taskId].func = Task_PokedexMainScreen;
            PlaySE(SE_SELECT);
        }
        else if ((gMain.newAndRepeatedKeys & DPAD_UP) && gPokedexView->menuCursorPos != 0)
        {
            gPokedexView->menuCursorPos--;
            PlaySE(SE_SELECT);
        }
        else if ((gMain.newAndRepeatedKeys & DPAD_DOWN) && gPokedexView->menuCursorPos <= 2)
        {
            gPokedexView->menuCursorPos++;
            PlaySE(SE_SELECT);
        }
    }
}

static void sub_808CA64(u8 taskId)
{
    if (gSprites[gPokedexView->selectedMonSpriteId].pos1.x == X_POSITION_FOR_DESCRIPTION_PAGE
     && gSprites[gPokedexView->selectedMonSpriteId].pos1.y == Y_POSITION_FOR_DESCRIPTION_PAGE)
    {
        gPokedexView->unk64B = gPokedexView->currentScreen;
        gTasks[taskId].data[0] = sub_808F210(&gPokedexView->unk0[gPokedexView->selectedPokemon], gPokedexView->selectedMonSpriteId);
        gTasks[taskId].func = sub_808CAE4;
    }
}

static void sub_808CAE4(u8 taskId)
{
    if (gTasks[gTasks[taskId].data[0]].isActive)
    {
        if (gPokedexView->currentScreen == PAGE_DEXPAGE && !sub_808F250(gTasks[taskId].data[0]) && HandleUpDownInputPageScreen())
            sub_808F284(&gPokedexView->unk0[gPokedexView->selectedPokemon], gTasks[taskId].data[0]);
    }
    else
    {
        gUnknown_0202FFB8 = gPokedexView->selectedPokemon;
        gUnknown_0202FFBA = gPokedexView->selectedNamePixelPosition;
        gTasks[taskId].func = Task_PokedexShowMainScreen;
    }
}

static void sub_808CB8C(u8 taskId)
{
    bool8 isActive = gTasks[gTasks[taskId].data[0]].isActive;

    if (!isActive)
    {
        if (gPokedexView->unk64F != 0)
        {
            gPokedexView->selectedPokemon = isActive;
            gPokedexView->selectedNamePixelPosition = 0x40;
            gTasks[taskId].func = sub_808CCC4;
        }
        else
        {
            gPokedexView->selectedNamePixelPosition = gPokedexView->unk62A;
            gPokedexView->selectedPokemon = gPokedexView->unk610;
            gPokedexView->dexMode = gPokedexView->unk614;
            if (!IsNationalPokedexEnabled())
                gPokedexView->dexMode = DEX_MODE_HOENN;
            gPokedexView->dexOrder = gPokedexView->unk618;
            gTasks[taskId].func = Task_PokedexShowMainScreen;
        }
    }
}

static void Task_ClosePokedex(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        gSaveBlock2.pokedex.unknown1 = gPokedexView->dexMode;
        if (!IsNationalPokedexEnabled())
            gSaveBlock2.pokedex.unknown1 = 0;
        gSaveBlock2.pokedex.order = gPokedexView->dexOrder;
        DestroyTask(taskId);
        SetMainCallback2(c2_exit_to_overworld_1_sub_8080DEC);
        m4aMPlayVolumeControl(&gMPlayInfo_BGM, 0xFFFF, 0x100);
    }
}

static void sub_808CCC4(u8 taskId)
{
    gPokedexView->unk64C_1 = 1;
    if (DisplayMainScreen(RESULTS_DEXPAGE))
        gTasks[taskId].func = Task_PokedexResultsScreen;
}

static void Task_PokedexResultsScreen(u8 taskId)
{
    REG_BG0VOFS = gPokedexView->menuY;

    if (gPokedexView->menuY)
    {
        gPokedexView->menuY -= 8;
    }
    else
    {
        if ((gMain.newKeys & A_BUTTON) && gPokedexView->unk0[gPokedexView->selectedPokemon].seen)
        {
            u32 excludedPalettes;

            SetSelectedSpriteId();
            excludedPalettes = (1 << (gSprites[gPokedexView->selectedMonSpriteId].oam.paletteNum + 16));
            gSprites[gPokedexView->selectedMonSpriteId].callback = MoveMonPosForDesc;
            BeginNormalPaletteFade(~excludedPalettes, 0, 0, 16, RGB(0, 0, 0));
            gTasks[taskId].func = sub_808D118;
            PlaySE(SE_PIN);
        }
        else if (gMain.newKeys & START_BUTTON)
        {
            gPokedexView->menuY = 0;
            gPokedexView->menuIsOpen = 1;
            gPokedexView->menuCursorPos = 0;
            gTasks[taskId].func = Task_PokedexResultsScreenMenu;
            PlaySE(SE_SELECT);
        }
        else if (gMain.newKeys & SELECT_BUTTON)
        {
            BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB(0, 0, 0));
            gTasks[taskId].data[0] = sub_8091E3C();
            gPokedexView->unk64F = 0;
            gTasks[taskId].func = sub_808CB8C;
            PlaySE(SE_PC_LOGON);
        }
        else if (gMain.newKeys & B_BUTTON)
        {
            BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB(0, 0, 0));
            gTasks[taskId].func = Task_PokedexResultsScreenReturnToMainScreen;
            PlaySE(SE_PC_OFF);
        }
        else
        {
            gPokedexView->selectedPokemon = ProcessDPadInputsMainPage(gPokedexView->selectedPokemon, 0xE);
            if (gPokedexView->iterationsOfScrolling)
                gTasks[taskId].func = WaitForScrollMonSpritesResultsScreen;
        }
    }
}

static void WaitForScrollMonSpritesResultsScreen(u8 taskId)
{
    if (ScrollMonSprites(gPokedexView->scrollDirection, gPokedexView->pixelsToScrollPerIteration, gPokedexView->iterationsOfScrollingConstant))
        gTasks[taskId].func = Task_PokedexResultsScreen;
}

static void Task_PokedexResultsScreenMenu(u8 taskId)
{
    REG_BG0VOFS = gPokedexView->menuY;

    if (gPokedexView->menuY != 96)
    {
        gPokedexView->menuY += 8;
    }
    else
    {
        if (gMain.newKeys & A_BUTTON)
        {
            switch (gPokedexView->menuCursorPos)
            {
            case 0: //BACK TO LIST
            default:
                gMain.newKeys |= START_BUTTON;
                break;
            case 1: //LIST TOP
                gPokedexView->selectedPokemon = 0;
                gPokedexView->selectedNamePixelPosition = 0x40;
                DestroySpritesForSkips();
                DrawSurroundingMons(gPokedexView->selectedPokemon, 0xE);
                gMain.newKeys |= START_BUTTON;
                break;
            case 2: //LIST BOTTOM
                gPokedexView->selectedPokemon = gPokedexView->pokemonListCount - 1;
                gPokedexView->selectedNamePixelPosition = (gPokedexView->pokemonListCount - 1) * 16 + 0x40;
                DestroySpritesForSkips();
                DrawSurroundingMons(gPokedexView->selectedPokemon, 0xE);
                gMain.newKeys |= START_BUTTON;
                break;
            case 3: //BACK TO POKEDEX
                BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB(0, 0, 0));
                gTasks[taskId].func = Task_PokedexResultsScreenReturnToMainScreen;
                PlaySE(SE_TRACK_DOOR);
                break;
            case 4: //CLOSE POKEDEX
                BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB(0, 0, 0));
                gTasks[taskId].func = Task_PokedexResultsScreenExitPokedex;
                PlaySE(SE_PC_OFF);
                break;
            }
        }

        //Exit menu when Start or B is pressed
        if (gMain.newKeys & (START_BUTTON | B_BUTTON))
        {
            gPokedexView->menuIsOpen = 0;
            gTasks[taskId].func = Task_PokedexResultsScreen;
            PlaySE(SE_SELECT);
        }
        else if ((gMain.newAndRepeatedKeys & DPAD_UP) && gPokedexView->menuCursorPos)
        {
            gPokedexView->menuCursorPos--;
            PlaySE(SE_SELECT);
        }
        else if ((gMain.newAndRepeatedKeys & DPAD_DOWN) && gPokedexView->menuCursorPos <= 3)
        {
            gPokedexView->menuCursorPos++;
            PlaySE(SE_SELECT);
        }
    }
}

static void sub_808D118(u8 taskId)
{
    if (gSprites[gPokedexView->selectedMonSpriteId].pos1.x == X_POSITION_FOR_DESCRIPTION_PAGE
     && gSprites[gPokedexView->selectedMonSpriteId].pos1.y == Y_POSITION_FOR_DESCRIPTION_PAGE)
    {
        gPokedexView->unk64B = gPokedexView->currentScreen;
        gTasks[taskId].data[0] = sub_808F210(&gPokedexView->unk0[gPokedexView->selectedPokemon], gPokedexView->selectedMonSpriteId);
        gTasks[taskId].func = sub_808D198;
    }
}

static void sub_808D198(u8 taskId)
{
    if (gTasks[gTasks[taskId].data[0]].isActive)
    {
        if (gPokedexView->currentScreen == PAGE_DEXPAGE && !sub_808F250(gTasks[taskId].data[0]) && HandleUpDownInputPageScreen())
            sub_808F284(&gPokedexView->unk0[gPokedexView->selectedPokemon], gTasks[taskId].data[0]);
    }
    else
    {
        gTasks[taskId].func = sub_808CCC4;
    }
}

static void Task_PokedexResultsScreenReturnToMainScreen(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        gPokedexView->selectedNamePixelPosition = gPokedexView->unk62A;
        gPokedexView->selectedPokemon = gPokedexView->unk610;
        gPokedexView->dexMode = gPokedexView->unk614;
        if (!IsNationalPokedexEnabled())
            gPokedexView->dexMode = DEX_MODE_HOENN;
        gPokedexView->dexOrder = gPokedexView->unk618;
        gTasks[taskId].func = Task_PokedexShowMainScreen;
    }
}

static void Task_PokedexResultsScreenExitPokedex(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        gPokedexView->selectedNamePixelPosition = gPokedexView->unk62A;
        gPokedexView->selectedPokemon = gPokedexView->unk610;
        gPokedexView->dexMode = gPokedexView->unk614;
        if (!IsNationalPokedexEnabled())
            gPokedexView->dexMode = DEX_MODE_HOENN;
        gPokedexView->dexOrder = gPokedexView->unk618;
        gTasks[taskId].func = Task_ClosePokedex;
    }
}

static bool8 DisplayMainScreen(u8 screen)
{
    switch (gMain.state)
    {
    case 0:
    default:
        if (gPaletteFade.active)
            return 0;
        SetVBlankCallback(NULL);
        gPokedexView->currentScreen = screen;
        InitBGAndObj(0); // init everything
        REG_BG2VOFS = gPokedexView->unk62D;
        LZ77UnCompVram(gPokedexMenu_Gfx, (void *)(VRAM));
        LZ77UnCompVram(gUnknown_08E96738, (void *)(VRAM + 0x6800));
        LZ77UnCompVram(gUnknown_08E9C6DC, (void *)(VRAM + 0x7800));
        DmaClear16(3, VRAM + 0x6000, 0x500);
        if (screen == MAIN_DEXPAGE)
            LZ77UnCompVram(gPokedexListLayout, (void *)(VRAM + 0x6500));
        else
            LZ77UnCompVram(gPokedexListResultsLayout, (void *)(VRAM + 0x6500));
        ResetPaletteFade();
        if (screen == MAIN_DEXPAGE)
            gPokedexView->unk64C_1 = 0;
        else
            gPokedexView->unk64C_1 = 1;
        sub_808D640();
        gMain.state = 1;
        break;
    case 1:
        ResetSpriteData();
        FreeAllSpritePalettes();
        gReservedSpritePaletteCount = 8;
        LoadCompressedObjectPic(&gUnknown_083A05CC[0]);
        LoadSpritePalettes(gUnknown_083A05DC);
        DrawCursorsAndText(screen);
        gMain.state++;
        break;
    case 2:
        Text_LoadWindowTemplate(&gWindowTemplate_81E7048);
        InitMenuWindow(&gWindowTemplate_81E7048);
        LZ77UnCompVram(gUnknown_0839FA7C, (void *)(VRAM + 0xFF80));
        gMain.state++;
        break;
    case 3:
        if (screen == MAIN_DEXPAGE)
            SortPokedex(gPokedexView->dexMode, gPokedexView->dexOrder);
        DrawSurroundingMons(gPokedexView->selectedPokemon, 0xE);
        gPokedexView->menuIsOpen = 0;
        gPokedexView->menuY = 0;
        gMain.state++;
        break;
    case 4:
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB(0, 0, 0));
        SetVBlankCallback(sub_808C0B8);
        gMain.state++;
        break;
    case 5:
        REG_WININ = 0x3F3F;
        REG_WINOUT = 0x1D3F;
        REG_WIN0H = 0;
        REG_WIN0V = 0;
        REG_WIN1H = 0;
        REG_WIN1V = 0;
        REG_BLDCNT = 0;
        REG_BLDALPHA = 0;
        REG_BLDY = 0;
        REG_BG3CNT = BGCNT_PRIORITY(3) | BGCNT_CHARBASE(0) | BGCNT_SCREENBASE(15) | BGCNT_16COLOR | BGCNT_TXT256x256;
        REG_BG1CNT = BGCNT_PRIORITY(1) | BGCNT_CHARBASE(0) | BGCNT_SCREENBASE(13) | BGCNT_16COLOR | BGCNT_TXT256x256;
        REG_BG0CNT = BGCNT_PRIORITY(0) | BGCNT_CHARBASE(0) | BGCNT_SCREENBASE(12) | BGCNT_16COLOR | BGCNT_TXT256x256;
        REG_DISPCNT = DISPCNT_MODE_0 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG_ALL_ON | DISPCNT_OBJ_ON | DISPCNT_OBJWIN_ON;
        gMain.state++;
        break;
    case 6:
        if (!gPaletteFade.active)
        {
            gMain.state = 0;
            return TRUE;
        }
        break;
    }
    return FALSE;
}

static void sub_808D640(void)
{
    if (gPokedexView->unk64C_1)
        LoadPalette(sPokedexSearchPalette + 1, 1, sizeof(sPokedexSearchPalette) - sizeof(u16));
    else if (!IsNationalPokedexEnabled())
        LoadPalette(gPokedexMenu_Pal + 1, 1, 0xBE);
    else
        LoadPalette(sNationalPokedexPalette + 1, 1, sizeof(sNationalPokedexPalette) - sizeof(u16));
}

static void SortPokedex(u8 dexMode, u8 sortMode)
{
    u16 vars[3]; //I have no idea why three regular variables are stored in an array, but whatever.
    s16 i;

    gPokedexView->pokemonListCount = 0;

    switch (dexMode)
    {
    default:
    case DEX_MODE_HOENN:
        vars[0] = HOENN_DEX_COUNT;
        vars[1] = 1;
        break;
    case DEX_MODE_NATIONAL:
        if (IsNationalPokedexEnabled())
        {
            vars[0] = NATIONAL_DEX_COUNT;
            vars[1] = 0;
        }
        else
        {
            vars[0] = HOENN_DEX_COUNT;
            vars[1] = 1;
        }
        break;
    }

    switch (sortMode)
    {
    case 0:
        if (vars[1])
        {
            for (i = 0; i < vars[0]; i++)
            {
                vars[2] = HoennToNationalOrder(i + 1);
                gPokedexView->unk0[i].dexNum = vars[2];
                gPokedexView->unk0[i].seen = GetSetPokedexFlag(vars[2], 0);
                gPokedexView->unk0[i].owned = GetSetPokedexFlag(vars[2], 1);
                if (gPokedexView->unk0[i].seen)
                    gPokedexView->pokemonListCount = i + 1;
            }
        }
        else
        {
            bool32 r10;
            s16 r5;

            r10 = r5 = i = 0;
            for (i = 0; i < vars[0]; i++)
            {
                vars[2] = i + 1;
                if (GetSetPokedexFlag(vars[2], 0))
                    r10 = 1;
                if (r10)
                {
                    asm("");    //Needed to match for some reason
                    gPokedexView->unk0[r5].dexNum = vars[2];
                    gPokedexView->unk0[r5].seen = GetSetPokedexFlag(vars[2], 0);
                    gPokedexView->unk0[r5].owned = GetSetPokedexFlag(vars[2], 1);
                    if (gPokedexView->unk0[r5].seen)
                        gPokedexView->pokemonListCount = r5 + 1;
                    r5++;
                }
            }
        }
        break;
    case 1:
        for (i = 0; i < POKEMON_SLOTS_NUMBER - 1; i++)
        {
            vars[2] = gPokedexOrder_Alphabetical[i];

            if (NationalToHoennOrder(vars[2]) <= vars[0] && GetSetPokedexFlag(vars[2], 0))
            {
                gPokedexView->unk0[gPokedexView->pokemonListCount].dexNum = vars[2];
                gPokedexView->unk0[gPokedexView->pokemonListCount].seen = 1;
                gPokedexView->unk0[gPokedexView->pokemonListCount].owned = GetSetPokedexFlag(vars[2], 1);
                gPokedexView->pokemonListCount++;
            }
        }
        break;
    case 2:
        for (i = NATIONAL_DEX_COUNT - 1; i >= 0; i--)
        {
            vars[2] = gPokedexOrder_Weight[i];

            if (NationalToHoennOrder(vars[2]) <= vars[0] && GetSetPokedexFlag(vars[2], 1))
            {
                gPokedexView->unk0[gPokedexView->pokemonListCount].dexNum = vars[2];
                gPokedexView->unk0[gPokedexView->pokemonListCount].seen = 1;
                gPokedexView->unk0[gPokedexView->pokemonListCount].owned = 1;
                gPokedexView->pokemonListCount++;
            }
        }
        break;
    case 3:
        for (i = 0; i < NATIONAL_DEX_COUNT; i++)
        {
            vars[2] = gPokedexOrder_Weight[i];

            if (NationalToHoennOrder(vars[2]) <= vars[0] && GetSetPokedexFlag(vars[2], 1))
            {
                gPokedexView->unk0[gPokedexView->pokemonListCount].dexNum = vars[2];
                gPokedexView->unk0[gPokedexView->pokemonListCount].seen = 1;
                gPokedexView->unk0[gPokedexView->pokemonListCount].owned = 1;
                gPokedexView->pokemonListCount++;
            }
        }
        break;
    case 4:
        for (i = NATIONAL_DEX_COUNT - 1; i >=0; i--)
        {
            vars[2] = gPokedexOrder_Height[i];

            if (NationalToHoennOrder(vars[2]) <= vars[0] && GetSetPokedexFlag(vars[2], 1))
            {
                gPokedexView->unk0[gPokedexView->pokemonListCount].dexNum = vars[2];
                gPokedexView->unk0[gPokedexView->pokemonListCount].seen = 1;
                gPokedexView->unk0[gPokedexView->pokemonListCount].owned = 1;
                gPokedexView->pokemonListCount++;
            }
        }
        break;
    case 5:
        for (i = 0; i < NATIONAL_DEX_COUNT; i++)
        {
            vars[2] = gPokedexOrder_Height[i];

            if (NationalToHoennOrder(vars[2]) <= vars[0] && GetSetPokedexFlag(vars[2], 1))
            {
                gPokedexView->unk0[gPokedexView->pokemonListCount].dexNum = vars[2];
                gPokedexView->unk0[gPokedexView->pokemonListCount].seen = 1;
                gPokedexView->unk0[gPokedexView->pokemonListCount].owned = 1;
                gPokedexView->pokemonListCount++;
            }
        }
        break;
    }

    for (i = gPokedexView->pokemonListCount; i < NATIONAL_DEX_COUNT; i++)
    {
        gPokedexView->unk0[i].dexNum |= 0xFFFF;
        gPokedexView->unk0[i].seen = 0;
        gPokedexView->unk0[i].owned = 0;
    }
}

static void PrintOnScreenSlots(u8 a, u16 currentIndex, u16 c)
{
    s16 index;
    u16 i;
    u16 r2;

    switch (a)
    {
    case 0: // called from DrawSurroundingMons
    default:
        index = currentIndex - 5;
        for (i = 0; i <= 10; i++) // draw 10 separate names?
        {
            if (index < 0 || index >= NATIONAL_DEX_COUNT || gPokedexView->unk0[index].dexNum == 0xFFFF)
            {
                FreeVRAMSpaceForMonNameText(0x11, i * 2, c); // show nothing
            }
            else
            {
                FreeVRAMSpaceForMonNameText(0x11, i * 2, c);
                if (gPokedexView->unk0[index].seen)
                {
                    PrintDexNumberSlots(index, 0x12, i * 2, c);
                    PrintCaughtBall(gPokedexView->unk0[index].owned, 0x11, i * 2, c);
                    PrintMonNameMainPage(gPokedexView->unk0[index].dexNum, 0x17, i * 2);
                }
                else
                {
                    PrintDexNumberSlots(index, 0x12, i * 2, c);
                    PrintCaughtBall(0, 0x11, i * 2, c);
                    PrintMonNameMainPage(0, 0x17, i * 2);
                }
            }
            index++;
        }
        break;
    case 1: // called when moving pokedex up
        index = currentIndex - 5;
        if (index < 0 || index >= NATIONAL_DEX_COUNT || gPokedexView->unk0[index].dexNum == 0xFFFF)
        {
            FreeVRAMSpaceForMonNameText(0x11, gPokedexView->unk630 * 2, c);
        }
        else
        {
            FreeVRAMSpaceForMonNameText(0x11, gPokedexView->unk630 * 2, c);
            if (gPokedexView->unk0[index].seen)
            {
                PrintDexNumberSlots(index, 0x12, gPokedexView->unk630 * 2, c);
                PrintCaughtBall(gPokedexView->unk0[index].owned, 0x11, gPokedexView->unk630 * 2, c);
                PrintMonNameMainPage(gPokedexView->unk0[index].dexNum, 0x17, gPokedexView->unk630 * 2);
            }
            else
            {
                PrintDexNumberSlots(index, 0x12, gPokedexView->unk630 * 2, c);
                PrintCaughtBall(0, 0x11, gPokedexView->unk630 * 2, c);
                PrintMonNameMainPage(0, 0x17, gPokedexView->unk630 * 2);
            }
        }
        break;
    case 2: // called when moving pokedex down
        index = currentIndex + 5;
        r2 = gPokedexView->unk630 + 10;
        if (r2 > 15)
            r2 -= 16;
        if (index < 0 || index >= NATIONAL_DEX_COUNT || gPokedexView->unk0[index].dexNum == 0xFFFF)
            FreeVRAMSpaceForMonNameText(0x11, r2 * 2, c);
        else
        {
            FreeVRAMSpaceForMonNameText(0x11, r2 * 2, c);
            if (gPokedexView->unk0[index].seen)
            {
                PrintDexNumberSlots(index, 0x12, r2 * 2, c);
                PrintCaughtBall(gPokedexView->unk0[index].owned, 0x11, r2 * 2, c);
                PrintMonNameMainPage(gPokedexView->unk0[index].dexNum, 0x17, r2 * 2);
            }
            else
            {
                PrintDexNumberSlots(index, 0x12, r2 * 2, c);
                PrintCaughtBall(0, 0x11, r2 * 2, c);
                PrintMonNameMainPage(0, 0x17, r2 * 2);
            }
        }
        break;
    }
}

static void PrintDexNumberSlots(u16 index, u8 stringLength, u8 c, u16 d) // stringlength?
{
    u8 text[4];
    u16 unk[2];
    u16 r7 = gPokedexView->unk0[index].dexNum;

    if (gPokedexView->dexMode == DEX_MODE_HOENN)
        r7 = NationalToHoennOrder(r7);
    unk[0] = 0x3FC;
    unk[1] = 0x3FD;
    text[0] = CHAR_0 + r7 / 100;        // hundreds-place number
    text[1] = CHAR_0 + (r7 % 100) / 10; // tens-place number
    text[2] = CHAR_0 + (r7 % 100) % 10; // ones-place number
    text[3] = EOS;
    *(u16 *)(VRAM + d * 0x800 +  c      * 0x40 + stringLength * 2) = unk[0];
    *(u16 *)(VRAM + d * 0x800 + (c + 1) * 0x40 + stringLength * 2) = unk[1];
    Menu_PrintText(text, stringLength - 15, c);
}

static void PrintCaughtBall(u16 owned, u8 b, u8 c, u16 d)
{
    u16 unk[2];

    if (owned)
    {
        unk[0] = 0x3FE;
        unk[1] = 0x3FF;
    }
    else
    {
        unk[0] = 0;
        unk[1] = 0;
    }
    *(u16 *)(VRAM + d * 0x800 +  c      * 0x40 + b * 2) = unk[0];
    *(u16 *)(VRAM + d * 0x800 + (c + 1) * 0x40 + b * 2) = unk[1];
}

static u8 PrintMonNameMainPage(u16 num, u8 b, u8 c)
{
    u8 text[10];
    u8 i;

    for (i = 0; i < 10; i++)
        text[i] = CHAR_SPACE;
    text[i] = EOS;

    num = NationalPokedexNumToSpecies(num);
    switch (num)
    {
    default:
        for (i = 0; gSpeciesNames[num][i] != EOS && i < 10; i++)
            text[i] = gSpeciesNames[num][i];
        break;
    case 0:
        for (i = 0; i < 10; i++)
            text[i] = CHAR_HYPHEN;
        break;
    }
    Menu_PrintTextPixelCoords(text, (b - 0x11) * 8 + 0xFC, c * 8, 0);
    return i;
}

static void FreeVRAMSpaceForMonNameText(u8 a, u8 b, u16 c)
{
    u8 i;

    for (i = 0; i < 12; i++)
    {
        *(u16 *)(VRAM + c * 0x800 +  b *      64 + (a + i) * 2) = 0;
        *(u16 *)(VRAM + c * 0x800 + (b + 1) * 64 + (a + i) * 2) = 0;
    }
}



static void DrawSurroundingMons(u16 num, u16 b)
{
    u8 i;
    u16 dexNum;
    u8 spriteId;

    for (i = 0; i < 4; i++)
        gPokedexView->spriteIds[i] = 0xFFFF;
    gPokedexView->selectedMonSpriteId = 0xFFFF;
    PrintOnScreenSlots(0, num, b);
    REG_BG2VOFS = gPokedexView->unk62D;

    dexNum = ParseDexNumberForSprite(num - 1);
    if (dexNum != 0xFFFF)
    {
        spriteId = SetUpMonPic(dexNum, X_POSITION_FOR_SCROLLING, Y_POSITION_FOR_SCROLLING);
        gSprites[spriteId].callback = sub_808EE28;
        gSprites[spriteId].data[5] = -32;
    }

    dexNum = ParseDexNumberForSprite(num);
    if (dexNum != 0xFFFF)
    {
        spriteId = SetUpMonPic(dexNum, X_POSITION_FOR_SCROLLING, Y_POSITION_FOR_SCROLLING);
        gSprites[spriteId].callback = sub_808EE28;
        gSprites[spriteId].data[5] = 0;
    }

    dexNum = ParseDexNumberForSprite(num + 1);
    if (dexNum != 0xFFFF)
    {
        spriteId = SetUpMonPic(dexNum, X_POSITION_FOR_SCROLLING, Y_POSITION_FOR_SCROLLING);
        gSprites[spriteId].callback = sub_808EE28;
        gSprites[spriteId].data[5] = 32;
    }

    gPokedexView->unk630 = 0;
    gPokedexView->unk632 = 0;
}

static bool8 ScrollMonSprites(u8 direction, u8 pixels, u8 iterations)
{
    u16 i;
    u8 foo;

    if (gPokedexView->iterationsOfScrolling)
    {
        gPokedexView->iterationsOfScrolling--;
        switch (direction)
        {
        case 1: // move up
            for (i = 0; i < 4; i++)
            {
                if (gPokedexView->spriteIds[i] != 0xFFFF)
                    gSprites[gPokedexView->spriteIds[i]].data[5] += pixels;
            }
            foo = 16 * (iterations - gPokedexView->iterationsOfScrolling) / iterations;
            REG_BG2VOFS = gPokedexView->unk62D + gPokedexView->unk632 * 16 - foo;
            gPokedexView->selectedNamePixelPosition -= gPokedexView->changeInSelectedNamePixelPos;
            break;
        case 2: // move down
            for (i = 0; i < 4; i++)
            {
                if (gPokedexView->spriteIds[i] != 0xFFFF)
                    gSprites[gPokedexView->spriteIds[i]].data[5] -= pixels;
            }
            foo = 16 * (iterations - gPokedexView->iterationsOfScrolling) / iterations;
            REG_BG2VOFS = gPokedexView->unk62D + gPokedexView->unk632 * 16 + foo;
            gPokedexView->selectedNamePixelPosition += gPokedexView->changeInSelectedNamePixelPos;
            break;
        }
        return FALSE;
    }
    else
    {
        REG_BG2VOFS = gPokedexView->unk62D + gPokedexView->unk630 * 16;
        return TRUE;
    }
}

static void MatchPrintedNameWithPosition(u8 direction, u16 b) // why the fuck is it 15
{
    u16 unk;
    u8 spriteId;

    gPokedexView->unk632 = gPokedexView->unk630;
    switch (direction)
    {
    case 1: // move up
        unk = ParseDexNumberForSprite(b - 1);
        if (unk != 0xFFFF)
        {
            spriteId = SetUpMonPic(unk, X_POSITION_FOR_SCROLLING, Y_POSITION_FOR_SCROLLING);
            gSprites[spriteId].callback = sub_808EE28;
            gSprites[spriteId].data[5] = -64;
        }
        if (gPokedexView->unk630 > 0)
            gPokedexView->unk630--;
        else
            gPokedexView->unk630 = 15;
        break;
    case 2: // move down
        unk = ParseDexNumberForSprite(b + 1);
        if (unk != 0xFFFF)
        {
            spriteId = SetUpMonPic(unk, X_POSITION_FOR_SCROLLING, Y_POSITION_FOR_SCROLLING);
            gSprites[spriteId].callback = sub_808EE28;
            gSprites[spriteId].data[5] = 64;
        }
        if (gPokedexView->unk630 < 15)
            gPokedexView->unk630++;
        else
            gPokedexView->unk630 = 0;
        break;
    }
}


// a - dexNum
// b - 0xE
static u16 ProcessDPadInputsMainPage(u16 a, u16 b)
{
    u8 r3;
    u8 r5;
    u8 i;
    u16 r6;
    u8 r10 = 0;

    if (!((gMain.heldKeys & DPAD_UP) && (a > 0)))
    {
        if (!((gMain.heldKeys & DPAD_DOWN) && (a < gPokedexView->pokemonListCount - 1)))
        {
            if ((gMain.newKeys & DPAD_LEFT) && (a > 0))
            {
                r6 = a;
                for (i = 0; i < 7; i++)
                    a = GetSlotNumbersFromMinMax(1, a, 0, gPokedexView->pokemonListCount - 1); // move the pokedex up 7
                gPokedexView->selectedNamePixelPosition += 16 * (a - r6);
                DestroySpritesForSkips();
                DrawSurroundingMons(a, 0xE);
                PlaySE(0x6D);
            }
            else if ((gMain.newKeys & DPAD_RIGHT) && (a < gPokedexView->pokemonListCount - 1))
            {
                r6 = a;
                for (i = 0; i < 7; i++)
                    a = GetSlotNumbersFromMinMax(0, a, 0, gPokedexView->pokemonListCount - 1); // move the pokedex down 7
                gPokedexView->selectedNamePixelPosition += (a - r6) * 16;
                DestroySpritesForSkips();
                DrawSurroundingMons(a, 0xE);
                PlaySE(0x6D);
            }
          _0808E5A2:
            if (r10 == 0)
            {
                gPokedexView->timeHeldDPad = 0;
                return a;
            }
        }
        else
        {
            r10 = 2;
            a = GetSlotNumbersFromMinMax(0, a, 0, gPokedexView->pokemonListCount - 1); // move the pokedex down 1
            MatchPrintedNameWithPosition(2, a);
            PrintOnScreenSlots(2, a, b);
            PlaySE(0x6C);
            goto _0808E5A2;
        }
    }
    else
    {
        r10 = 1;
        a = GetSlotNumbersFromMinMax(1, a, 0, gPokedexView->pokemonListCount - 1); // move the pokedex up 1
        MatchPrintedNameWithPosition(1, a);
        PrintOnScreenSlots(1, a, b);
        PlaySE(0x6C);
        goto _0808E5A2;
    }

    if (gPokedexView->timeHeldDPad <= 15)
    {
        r5 = gPixelsPerScrollIteration[gPokedexView->timeHeldDPad / 4];
        r3 = gScrollingIterations[gPokedexView->timeHeldDPad / 4];
        gPokedexView->iterationsOfScrolling = r3;
        gPokedexView->iterationsOfScrollingConstant = r3;
        gPokedexView->pixelsToScrollPerIteration = r5;
        gPokedexView->scrollDirection = r10;
        gPokedexView->changeInSelectedNamePixelPos = r5 / 2;
        ScrollMonSprites(gPokedexView->scrollDirection, gPokedexView->pixelsToScrollPerIteration, gPokedexView->iterationsOfScrollingConstant);
    }
    else
    {
        r6 = a;
        if (gMain.heldKeys & DPAD_UP)
        {
            for (i = 0; i < 7; i++)
                a = GetSlotNumbersFromMinMax(1, a, 0, gPokedexView->pokemonListCount - 1); // move the pokedex up 7
        }
        else if (gMain.heldKeys & DPAD_DOWN)
        {
            for (i = 0; i < 7; i++)
                a = GetSlotNumbersFromMinMax(0, a, 0, gPokedexView->pokemonListCount - 1); // move the pokedex down 7
        }
        gPokedexView->selectedNamePixelPosition += (a - r6) * 16;
        DestroySpritesForSkips();
        DrawSurroundingMons(a, 0xE);
    }

    if (gPokedexView->timeHeldDPad <= 19)
        gPokedexView->timeHeldDPad++;
    return a;
}

static void SetSelectedSpriteId(void)
{
    u16 i;

    for (i = 0; i < 4; i++)
    {
        u16 spriteId = gPokedexView->spriteIds[i];

        if (gSprites[spriteId].pos2.x == 0 && gSprites[spriteId].pos2.y == 0 && spriteId != 0xFFFF)
            gPokedexView->selectedMonSpriteId = spriteId;
    }
}

static u8 HandleUpDownInputPageScreen(void)
{
    u16 temp;
    u16 currentMon = gPokedexView->selectedPokemon;

    if ((gMain.newKeys & DPAD_UP) && currentMon)
    {
        temp = currentMon;
        while (temp != 0)
        {
            temp = GetSlotNumbersFromMinMax(1, temp, 0, gPokedexView->pokemonListCount - 1);

            if (gPokedexView->unk0[temp].seen)
            {
                currentMon = temp;
                break;
            }
        }

        if (gPokedexView->selectedPokemon == currentMon)
            return 0;
        else
        {
            gPokedexView->selectedPokemon = currentMon;
            gPokedexView->selectedNamePixelPosition -= 16;
            return 1;
        }
    }
    else if ((gMain.newKeys & DPAD_DOWN) && currentMon < gPokedexView->pokemonListCount - 1)
    {
        temp = currentMon;
        while (temp < gPokedexView->pokemonListCount - 1)
        {
            temp = GetSlotNumbersFromMinMax(0, temp, 0, gPokedexView->pokemonListCount - 1);

            if (gPokedexView->unk0[temp].seen)
            {
                currentMon = temp;
                break;
            }
        }

        if (gPokedexView->selectedPokemon == currentMon)
            return 0;
        else
        {
            gPokedexView->selectedPokemon = currentMon;
            gPokedexView->selectedNamePixelPosition += 16;
            return 1;
        }
    }
    return 0;
}

static u8 DestroySpritesForSkips(void)
{
    u16 i;

    for (i = 0; i < 4; i++)
    {
        if (gPokedexView->spriteIds[i] != 0xFFFF)
        {
            DestroySprite(&gSprites[gPokedexView->spriteIds[i]]);
            gPokedexView->spriteIds[i] |= 0xFFFF;
        }
    }
    return 0;
}

static u16 ParseDexNumberForSprite(u16 num)
{
    if (num >= NATIONAL_DEX_COUNT || gPokedexView->unk0[num].dexNum == 0xFFFF)
        return 0xFFFF;
    else if (gPokedexView->unk0[num].seen)
        return gPokedexView->unk0[num].dexNum;
    else
        return 0;
}

static u32 SetUpMonPic(u16 num, s16 x, s16 y)
{
    u8 palette;

    for (palette = 0; palette < 4; palette++)
    {
        if (gPokedexView->spriteIds[palette] == 0xFFFF)
        {
            u8 spriteId = DisplayMonPic(num, x, y, palette);

            gSprites[spriteId].oam.affineMode = 1;
            gSprites[spriteId].oam.priority = 3;
            gSprites[spriteId].data[0] = 0;
            gSprites[spriteId].data[1] = palette;
            gSprites[spriteId].data[2] = NationalPokedexNumToSpecies(num);
            gPokedexView->spriteIds[palette] = spriteId;
            return spriteId;
        }
    }
    return 0xFFFF;
}

static void DrawCursorsAndText(u8 screen)
{
    u8 spriteId;
    u16 r5;

    spriteId = CreateSprite(&gSpriteTemplate_RedArrow, 184, 4, 0);
    gSprites[spriteId].data[1] = 0;

    spriteId = CreateSprite(&gSpriteTemplate_RedArrow, 184, 156, 0);
    gSprites[spriteId].data[1] = 1;
    gSprites[spriteId].vFlip = TRUE;

    CreateSprite(&gSpriteTemplate_MainPageScrollBar, 234, 20, 0);
    CreateSprite(&gSpriteTemplate_BottomLeftText, 16, 138, 0); // start button

    spriteId = CreateSprite(&gSpriteTemplate_BottomLeftText, 48, 138, 0);
    StartSpriteAnim(&gSprites[spriteId], 3); // menu text

    spriteId = CreateSprite(&gSpriteTemplate_BottomLeftText, 16, 158, 0);
    StartSpriteAnim(&gSprites[spriteId], 2); // select button
    gSprites[spriteId].data[2] = 0x80;

    spriteId = CreateSprite(&gSpriteTemplate_BottomLeftText, 48, 158, 0);
    StartSpriteAnim(&gSprites[spriteId], 1); // search text

    spriteId = CreateSprite(&gSpriteTemplate_TransparentBarForBall, 0, 80, 2);
    gSprites[spriteId].oam.affineMode = 1;
    gSprites[spriteId].oam.matrixNum = 30;
    gSprites[spriteId].data[0] = 0x1E;
    gSprites[spriteId].data[1] = 0;

    spriteId = CreateSprite(&gSpriteTemplate_TransparentBarForBall, 0, 80, 2);
    gSprites[spriteId].oam.affineMode = 1;
    gSprites[spriteId].oam.matrixNum = 31;
    gSprites[spriteId].data[0] = 0x1F;
    gSprites[spriteId].data[1] = 0x80;

    if (screen == MAIN_DEXPAGE)
    {
        u32 _a;

        CreateSprite(&gSpriteTemplate_SeenOwnText, 32, 40, 1); // seen text

        spriteId = CreateSprite(&gSpriteTemplate_SeenOwnText, 32, 72, 1);
        StartSpriteAnim(&gSprites[spriteId], 1); // own text
        _a = 0;

        spriteId = CreateSprite(&gSpriteTemplate_SeenOwnNumbers, 28, 48, 1);
        r5 = gPokedexView->seenPokemon / 100; // hundreds-place seen number
        StartSpriteAnim(&gSprites[spriteId], r5);
        if (r5 != 0)
            _a = 1;
        else
            gSprites[spriteId].invisible = TRUE;

        spriteId = CreateSprite(&gSpriteTemplate_SeenOwnNumbers, 34, 48, 1);
        r5 = (gPokedexView->seenPokemon % 100) / 10; // tens-place seen number
        if (r5 != 0 || _a != 0)
            StartSpriteAnim(&gSprites[spriteId], r5);
        else
            gSprites[spriteId].invisible = TRUE;

        spriteId = CreateSprite(&gSpriteTemplate_SeenOwnNumbers, 40, 48, 1);
        r5 = (gPokedexView->seenPokemon % 100) % 10; // ones-place seen number
        StartSpriteAnim(&gSprites[spriteId], r5);
        _a = 0;

        spriteId = CreateSprite(&gSpriteTemplate_SeenOwnNumbers, 28, 80, 1);
        r5 = gPokedexView->ownedPokemon / 100; // hundreds-place owned number
        StartSpriteAnim(&gSprites[spriteId], r5);
        if (r5 != 0)
            _a = 1;
        else
            gSprites[spriteId].invisible = TRUE;

        spriteId = CreateSprite(&gSpriteTemplate_SeenOwnNumbers, 34, 80, 1);
        r5 = (gPokedexView->ownedPokemon % 100) / 10; // tens-place owned number
        if (r5 != 0 || _a != 0)
            StartSpriteAnim(&gSprites[spriteId], r5);
        else
            gSprites[spriteId].invisible = TRUE;

        spriteId = CreateSprite(&gSpriteTemplate_SeenOwnNumbers, 40, 80, 1);
        r5 = (gPokedexView->ownedPokemon % 100) % 10; // ones-place owned number
        StartSpriteAnim(&gSprites[spriteId], r5);

        spriteId = CreateSprite(&gSpriteTemplate_83A05B4, 140, 96, 1);
        gSprites[spriteId].invisible = TRUE;
    }
    else
    {
        spriteId = CreateSprite(&gSpriteTemplate_83A05B4, 140, 80, 1); // create nothing essentially
        gSprites[spriteId].invisible = TRUE;
    }
}

static void nullsub_58(struct Sprite *sprite)
{
}

static void sub_808ED94(struct Sprite *sprite)
{
    if (gPokedexView->currentScreen != MAIN_DEXPAGE)
        DestroySprite(sprite);
}

static void MoveMonPosForDesc(struct Sprite *sprite)
{
    sprite->oam.priority = 0;
    sprite->oam.affineMode = 0;
    sprite->pos2.x = 0;
    sprite->pos2.y = 0;
    if (sprite->pos1.x != X_POSITION_FOR_DESCRIPTION_PAGE || sprite->pos1.y != Y_POSITION_FOR_DESCRIPTION_PAGE)
    {
        if (sprite->pos1.x > X_POSITION_FOR_DESCRIPTION_PAGE)
            sprite->pos1.x--;
        if (sprite->pos1.x < X_POSITION_FOR_DESCRIPTION_PAGE)
            sprite->pos1.x++;

        if (sprite->pos1.y > Y_POSITION_FOR_DESCRIPTION_PAGE)
            sprite->pos1.y--;
        if (sprite->pos1.y < Y_POSITION_FOR_DESCRIPTION_PAGE)
            sprite->pos1.y++;
    }
    else
    {
        sprite->callback = nullsub_58;
    }
}

static void sub_808EE28(struct Sprite *sprite)
{
    u8 data1 = sprite->data[1];

    if (gPokedexView->currentScreen != MAIN_DEXPAGE && gPokedexView->currentScreen != RESULTS_DEXPAGE)
    {
        DestroySprite(sprite);
        gPokedexView->spriteIds[data1] = 0xFFFF;
    }
    else
    {
        u32 var;

        sprite->pos2.y = gSineTable[(u8)sprite->data[5]] * 76 / 256;
        var = 0x10000 / gSineTable[sprite->data[5] + 0x40];
        if (var > 0xFFFF)
            var = 0xFFFF;
        SetOamMatrix(sprite->data[1] + 1, 0x100, 0, 0, var);
        sprite->oam.matrixNum = data1 + 1;

        if (sprite->data[5] > -64 && sprite->data[5] < 64)
        {
            sprite->invisible = FALSE;
            sprite->data[0] = 1;
        }
        else
        {
            sprite->invisible = TRUE;
        }

        if ((sprite->data[5] <= -64 || sprite->data[5] >= 64) && sprite->data[0] != 0)
        {
            DestroySprite(sprite);
            gPokedexView->spriteIds[data1] = 0xFFFF;
        }
    }
}

#define CURSOR_RANGE_Y 120

static void UpdateCursorPositionMainPage(struct Sprite *sprite)
{
    if (gPokedexView->currentScreen != MAIN_DEXPAGE && gPokedexView->currentScreen != RESULTS_DEXPAGE)
        DestroySprite(sprite);
    else
        sprite->pos2.y = gPokedexView->selectedPokemon * CURSOR_RANGE_Y / (gPokedexView->pokemonListCount - 1);
}

static void sub_808EF8C(struct Sprite *sprite)
{
    if (gPokedexView->currentScreen != MAIN_DEXPAGE && gPokedexView->currentScreen != RESULTS_DEXPAGE)
    {
        DestroySprite(sprite);
    }
    else
    {
        u8 r0;

        if (sprite->data[1] != 0)
        {
            if (gPokedexView->selectedPokemon == gPokedexView->pokemonListCount - 1)
                sprite->invisible = TRUE;
            else
                sprite->invisible = FALSE;
            r0 = sprite->data[2];
        }
        else
        {
            if (gPokedexView->selectedPokemon == 0)
                sprite->invisible = TRUE;
            else
                sprite->invisible = FALSE;
            r0 = sprite->data[2] - 128;
        }
        sprite->pos2.y = gSineTable[r0] / 64;
        sprite->data[2] = sprite->data[2] + 8;
        if (gPokedexView->menuIsOpen == 0 && gPokedexView->menuY == 0 && sprite->invisible == 0)
            sprite->invisible = FALSE;
        else
            sprite->invisible = TRUE;
    }
}

static void sub_808F08C(struct Sprite *sprite)
{
    if (gPokedexView->currentScreen != MAIN_DEXPAGE && gPokedexView->currentScreen != RESULTS_DEXPAGE)
        DestroySprite(sprite);
}

static void sub_808F0B4(struct Sprite *sprite)
{
    if (gPokedexView->currentScreen != MAIN_DEXPAGE && gPokedexView->currentScreen != RESULTS_DEXPAGE)
    {
        DestroySprite(sprite);
    }
    else
    {
        u8 val;
        s16 r3;
        s16 r0;

        val = gPokedexView->selectedNamePixelPosition + sprite->data[1];
        r3 = gSineTable[val];
        r0 = gSineTable[val + 0x40];
        SetOamMatrix(sprite->data[0], r0, r3, -r3, r0);

        val = gPokedexView->selectedNamePixelPosition + (sprite->data[1] + 0x40);
        r3 = gSineTable[val];
        r0 = gSineTable[val + 0x40];
        sprite->pos2.x = r0 * 40 / 256;
        sprite->pos2.y = r3 * 40 / 256;
    }
}

static void sub_808F168(struct Sprite *sprite)
{
    if (gPokedexView->currentScreen != MAIN_DEXPAGE && gPokedexView->currentScreen != RESULTS_DEXPAGE)
    {
        DestroySprite(sprite);
    }
    else
    {
        u16 r1 = gPokedexView->currentScreen == 0 ? 80 : 96;

        if (gPokedexView->menuIsOpen != 0 && gPokedexView->menuY == r1)
        {
            sprite->invisible = FALSE;
            sprite->pos2.y = gPokedexView->menuCursorPos * 16;
            sprite->pos2.x = gSineTable[(u8)sprite->data[2]] / 64;
            sprite->data[2] += 8;
        }
        else
        {
            sprite->invisible = TRUE;
        }
    }
}

static u8 sub_808F210(struct PokedexListItem *item, u8 b)
{
    u8 taskId;

    gUnknown_0202FFBC = item;
    taskId = CreateTask(Task_InitPageScreenMultistep, 0);
    gTasks[taskId].data[0] = 0;
    gTasks[taskId].data[1] = 1;
    gTasks[taskId].data[2] = 0;
    gTasks[taskId].data[3] = 0;
    gTasks[taskId].data[4] = b;
    return taskId;
}

static bool8 sub_808F250(u8 taskId)
{
    if (gTasks[taskId].data[0] == 0 && gTasks[taskId].func == Task_PageScreenProcessInput)
        return FALSE;
    else
        return TRUE;
}

static u8 sub_808F284(struct PokedexListItem *item, u8 b)
{
    gUnknown_0202FFBC = item;
    gTasks[b].data[0] = 1;
    gTasks[b].data[1] = 0;
    gTasks[b].data[2] = 0;
    gTasks[b].data[3] = 0;
    return b;
}

#if ENGLISH
#define CATEGORY_LEFT (11)
#elif GERMAN
#define CATEGORY_LEFT (16)
#endif

static void Task_InitPageScreenMultistep(u8 taskId)
{
    switch (gMain.state)
    {
    case 0:
    default:
        if (!gPaletteFade.active)
        {
            u16 r2;

            gPokedexView->currentScreen = PAGE_DEXPAGE;
            gPokedexView->descriptionPageNum = 0;
            gUnknown_03005CEC = gMain.vblankCallback;
            SetVBlankCallback(NULL);
            r2 = 0;
            if (gTasks[taskId].data[1] != 0)
                r2 += 0x1000;
            if (gTasks[taskId].data[2] != 0)
                r2 |= 0x200;
            InitBGAndObj(r2);
            gMain.state = 1;
        }
        break;
    case 1:
        LZ77UnCompVram(gPokedexMenu_Gfx, (void *)VRAM);
        LZ77UnCompVram(gPokedexPageLayout, (void *)(VRAM + 0x7800));
        PrintFootprints(gUnknown_0202FFBC->dexNum, 2, 0x3FC);
        gMain.state++;
        break;
    case 2:
        sub_80904FC(0xD);
        sub_8090584(gPokedexView->selectedScreen, 0xD);
        sub_808D640();
        gMain.state++;
        break;
    case 3:
        Text_LoadWindowTemplate(&gWindowTemplate_81E7064);
        InitMenuWindow(&gWindowTemplate_81E7064);
        gMain.state++;
        break;
    case 4:
        if (gPokedexView->dexMode == DEX_MODE_HOENN)
            sub_8091154(NationalToHoennOrder(gUnknown_0202FFBC->dexNum), 0xD, 3);
        else
            sub_8091154(gUnknown_0202FFBC->dexNum, 0xD, 3);
        sub_80911C8(gUnknown_0202FFBC->dexNum, 0x10, 3);
        Menu_PrintText(gDexText_UnknownPoke, CATEGORY_LEFT, 5);
        Menu_PrintText(gDexText_UnknownHeight, 16, 7);
        Menu_PrintText(gDexText_UnknownWeight, 16, 9);
        if (gUnknown_0202FFBC->owned)
        {
            sub_8091304(gPokedexEntries[gUnknown_0202FFBC->dexNum].categoryName, CATEGORY_LEFT, 5);
            sub_8091458(gPokedexEntries[gUnknown_0202FFBC->dexNum].height, 16, 7);
            sub_8091564(gPokedexEntries[gUnknown_0202FFBC->dexNum].weight, 16, 9);
            Menu_PrintText(gPokedexEntries[gUnknown_0202FFBC->dexNum].description, DESCRIPTION_X_POSITION, DESCRIPTION_Y_POSITION);
            sub_80917CC(14, 0x3FC);
        }
        else
        {
            Menu_PrintText(gUnknown_083A05F8, 2, 13);
            LoadPalette(gPlttBufferUnfaded + 1, 0x31, 0x1E);
        }
        gMain.state++;
        break;
    case 5:
        if (gTasks[taskId].data[1] == 0)
        {
            gTasks[taskId].data[4] = (u16)DisplayMonPic(gUnknown_0202FFBC->dexNum, X_POSITION_FOR_DESCRIPTION_PAGE, Y_POSITION_FOR_DESCRIPTION_PAGE, 0);
            gSprites[gTasks[taskId].data[4]].oam.priority = 0;
        }
        gMain.state++;
        break;
    case 6:
        {
            u32 excludedPalettes = 0;

            if (gTasks[taskId].data[2] != 0)
                excludedPalettes = 0x14;
            if (gTasks[taskId].data[1] != 0)
                excludedPalettes |= (1 << (gSprites[gTasks[taskId].data[4]].oam.paletteNum + 16));
            BeginNormalPaletteFade(~excludedPalettes, 0, 16, 0, RGB(0, 0, 0));
            SetVBlankCallback(gUnknown_03005CEC);
            gMain.state++;
        }
        break;
    case 7:
        REG_BLDCNT = 0;
        REG_BLDALPHA = 0;
        REG_BLDY = 0;
        REG_BG3CNT = BGCNT_PRIORITY(3) | BGCNT_CHARBASE(0) | BGCNT_SCREENBASE(15) | BGCNT_16COLOR | BGCNT_TXT256x256;
        REG_BG1CNT = BGCNT_PRIORITY(0) | BGCNT_CHARBASE(0) | BGCNT_SCREENBASE(13) | BGCNT_16COLOR | BGCNT_TXT256x256;
        REG_DISPCNT = DISPCNT_MODE_0 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON | DISPCNT_OBJ_ON;
        gMain.state++;
        break;
    case 8:
        if (!gPaletteFade.active)
        {
            gMain.state++;
            if (gTasks[taskId].data[3] == 0)
            {
                StopCryAndClearCrySongs();
                PlayCry2(NationalPokedexNumToSpecies(gUnknown_0202FFBC->dexNum), 0, 0x7D, 0xA);
            }
            else
            {
                gMain.state++;
            }
        }
        break;
    case 9:
        if (!IsCryPlayingOrClearCrySongs())
            gMain.state++;
        break;
    case 10:
        gTasks[taskId].data[0] = 0;
        gTasks[taskId].data[1] = 0;
        gTasks[taskId].data[2] = 1;
        gTasks[taskId].data[3] = 1;
        gTasks[taskId].func = Task_PageScreenProcessInput;
        gMain.state = 0;
        break;
    }
}

static void Task_PageScreenProcessInput(u8 taskId)
{
    if (gTasks[taskId].data[0] != 0)
    {
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB(0, 0, 0));
        gTasks[taskId].func = sub_808F888;
        PlaySE(SE_Z_SCROLL);
        return;
    }
    if (gMain.newKeys & B_BUTTON)
    {
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB(0, 0, 0));
        gTasks[taskId].func = Task_ClosePageScreen;
        PlaySE(SE_PC_OFF);
        return;
    }
    if (gMain.newKeys & A_BUTTON)
    {
        switch (gPokedexView->selectedScreen)
        {
        case PAGE_SCREEN:
            break;
        case AREA_SCREEN:
            BeginNormalPaletteFade(0xFFFFFFEB, 0, 0, 16, RGB(0, 0, 0));
            gTasks[taskId].func = Task_InitAreaScreenMultistep;
            PlaySE(SE_PIN);
            break;
        case CRY_SCREEN:
            BeginNormalPaletteFade(0xFFFFFFEB, 0, 0, 16, RGB(0, 0, 0));
            gTasks[taskId].func = Task_InitCryScreenMultistep;
            PlaySE(SE_PIN);
            break;
        case SIZE_SCREEN:
            if (!gUnknown_0202FFBC->owned)
            {
                PlaySE(SE_HAZURE);
            }
            else
            {
                BeginNormalPaletteFade(0xFFFFFFEB, 0, 0, 16, RGB(0, 0, 0));
                gTasks[taskId].func = Task_InitSizeScreenMultistep;
                PlaySE(SE_PIN);
            }
            break;
        }
        return;
    }
    if (((gMain.newKeys & DPAD_LEFT)
     || ((gMain.newKeys & L_BUTTON) && gSaveBlock2.optionsButtonMode == OPTIONS_BUTTON_MODE_LR))
     && gPokedexView->selectedScreen > 0)
    {
        gPokedexView->selectedScreen--;
        sub_8090584(gPokedexView->selectedScreen, 0xD);
        PlaySE(SE_Z_PAGE);
        return;
    }
    if (((gMain.newKeys & DPAD_RIGHT)
     || ((gMain.newKeys & R_BUTTON) && gSaveBlock2.optionsButtonMode == OPTIONS_BUTTON_MODE_LR))
     && gPokedexView->selectedScreen < 3)
    {
        gPokedexView->selectedScreen++;
        sub_8090584(gPokedexView->selectedScreen, 0xD);
        PlaySE(SE_Z_PAGE);
        return;
    }
}

static void sub_808F888(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_InitPageScreenMultistep;
}

static void Task_ClosePageScreen(u8 taskId)
{
    if (!gPaletteFade.active)
        DestroyTask(taskId);
}

static void Task_InitAreaScreenMultistep(u8 taskId)
{
    switch (gMain.state)
    {
    case 0:
    default:
        if (!gPaletteFade.active)
        {
            gPokedexView->currentScreen = AREA_DEXPAGE;
            gUnknown_03005CEC = gMain.vblankCallback;
            SetVBlankCallback(NULL);
            InitBGAndObj(0x200); // init all but bg1
            gPokedexView->selectedScreen = AREA_SCREEN;
            gMain.state = 1;
        }
        break;
    case 1:
        sub_8090540(0xD);
        sub_8090644(1, 0xD);
        sub_808D640();
        REG_BG1CNT = BGCNT_PRIORITY(0) | BGCNT_CHARBASE(0) | BGCNT_SCREENBASE(13) | BGCNT_16COLOR | BGCNT_TXT256x256;
        gMain.state++;
        break;
    case 2:
        ShowPokedexAreaScreen(NationalPokedexNumToSpecies(gUnknown_0202FFBC->dexNum), &gPokedexView->unk64F);
        SetVBlankCallback(gUnknown_03005CEC);
        gPokedexView->unk64F = 0;
        gMain.state = 0;
        gTasks[taskId].func = Task_AreaScreenProcessInput;
        break;
    }
}

static void Task_AreaScreenProcessInput(u8 taskId)
{
    if (gPokedexView->unk64F != 0)
        gTasks[taskId].func = sub_808FA00;
}

static void sub_808FA00(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        switch (gPokedexView->unk64F)
        {
        case 1:
        default:
            gTasks[taskId].func = Task_InitPageScreenMultistep;
            break;
        case 2:
            gTasks[taskId].func = Task_InitCryScreenMultistep;
            break;
        }
    }
}

static void Task_InitCryScreenMultistep(u8 taskId)
{
    switch (gMain.state)
    {
    case 0:
    default:
        if (!gPaletteFade.active)
        {
            m4aMPlayStop(&gMPlayInfo_BGM);
            gPokedexView->currentScreen = CRY_DEXPAGE;
            gUnknown_03005CEC = gMain.vblankCallback;
            SetVBlankCallback(NULL);
            InitBGAndObj(0x200); // init all but bg1
            gPokedexView->selectedScreen = CRY_SCREEN;
            gMain.state = 1;
        }
        break;
    case 1:
        LZ77UnCompVram(gPokedexMenu_Gfx, (void *)VRAM);
        LZ77UnCompVram(gUnknown_0839F8A0, (void *)(VRAM + 0x7000));
        gMain.state++;
        break;
    case 2:
        sub_8090540(0xD);
        sub_8090644(2, 0xD);
        sub_808D640();
        DmaClear16(3, (void *)(VRAM + 0xF800), 0x500);
        gMain.state++;
        break;
    case 3:
        Text_LoadWindowTemplate(&gWindowTemplate_81E702C);
        InitMenuWindow(&gWindowTemplate_81E702C);
        ResetPaletteFade();
        gMain.state++;
        break;
    case 4:
        Menu_PrintText(gDexText_CryOf, 10, 4);
        sub_8091260(gUnknown_0202FFBC->dexNum, 10, 6, 2);
        gMain.state++;
        break;
    case 5:
        gTasks[taskId].data[4] = DisplayMonPic(gUnknown_0202FFBC->dexNum, X_POSITION_FOR_CRY_PAGE, Y_POSITION_FOR_CRY_PAGE, 0);
        gSprites[gTasks[taskId].data[4]].oam.priority = 0;
        gUnknown_03005E98 = 0;
        gMain.state++;
        break;
    case 6:
        {
            struct CryRelatedStruct sp8;

            sp8.unk0 = 0x4020;
            sp8.unk2 = 0x1F;
            sp8.paletteNo = 8;
            sp8.yPos = 0x1E;
            sp8.xPos = 0xC;
            if (sub_8119E3C(&sp8, 0) != 0)
            {
                gMain.state++;
                gUnknown_03005E98 = 0;
            }
        }
        break;
    case 7:
        {
            struct CryRelatedStruct sp10;

            sp10.unk0 = 0x3000;
            sp10.unk2 = 0xE;
            sp10.paletteNo = 9;
            sp10.xPos = 0x12;
            sp10.yPos = 3;
            if (ShowPokedexCryScreen(&sp10, 1) != 0)
                gMain.state++;
        }
        break;
    case 8:
        BeginNormalPaletteFade(0xFFFFFFEB, 0, 16, 0, RGB(0, 0, 0));
        SetVBlankCallback(gUnknown_03005CEC);
        gMain.state++;
        break;
    case 9:
        REG_BLDCNT = 0;
        REG_BLDALPHA = 0;
        REG_BLDY = 0;
        REG_BG2CNT = BGCNT_PRIORITY(2) | BGCNT_CHARBASE(0) | BGCNT_SCREENBASE(14) | BGCNT_16COLOR | BGCNT_TXT256x256;
        REG_BG0CNT = BGCNT_PRIORITY(3) | BGCNT_CHARBASE(1) | BGCNT_SCREENBASE(31) | BGCNT_16COLOR | BGCNT_TXT256x256;
        REG_BG1CNT = BGCNT_PRIORITY(0) | BGCNT_CHARBASE(0) | BGCNT_SCREENBASE(13) | BGCNT_16COLOR | BGCNT_TXT256x256;
        REG_DISPCNT = DISPCNT_MODE_0 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG_ALL_ON | DISPCNT_OBJ_ON;
        gMain.state++;
        break;
    case 10:
        gPokedexView->unk64F = 0;
        gMain.state = 0;
        gTasks[taskId].func = Task_CryScreenProcessInput;
        break;
    }
}

static void Task_CryScreenProcessInput(u8 taskId)
{
    sub_8119F88(0);

    if (IsCryPlaying())
        sub_8090040(1);
    else
        sub_8090040(0);

    if (gMain.newKeys & A_BUTTON)
    {
        sub_8090040(1);
        sub_811A050(NationalPokedexNumToSpecies(gUnknown_0202FFBC->dexNum));
        return;
    }
    else if (!gPaletteFade.active)
    {
        if (gMain.newKeys & B_BUTTON)
        {
            BeginNormalPaletteFade(0xFFFFFFEB, 0, 0, 16, RGB(0, 0, 0));
            m4aMPlayContinue(&gMPlayInfo_BGM);
            gPokedexView->unk64F = 1;
            gTasks[taskId].func = sub_808FFBC;
            PlaySE(SE_PC_OFF);
            return;
        }
        if ((gMain.newKeys & DPAD_LEFT)
         || ((gMain.newKeys & L_BUTTON) && gSaveBlock2.optionsButtonMode == OPTIONS_BUTTON_MODE_LR))
        {
            BeginNormalPaletteFade(0xFFFFFFEB, 0, 0, 16, RGB(0, 0, 0));
            m4aMPlayContinue(&gMPlayInfo_BGM);
            gPokedexView->unk64F = 2;
            gTasks[taskId].func = sub_808FFBC;
            PlaySE(SE_Z_PAGE);
            return;
        }
        if ((gMain.newKeys & DPAD_RIGHT)
         || ((gMain.newKeys & R_BUTTON) && gSaveBlock2.optionsButtonMode == OPTIONS_BUTTON_MODE_LR))
        {
            if (!gUnknown_0202FFBC->owned)
            {
                PlaySE(SE_HAZURE);
            }
            else
            {
                BeginNormalPaletteFade(0xFFFFFFEB, 0, 0, 16, RGB(0, 0, 0));
                m4aMPlayContinue(&gMPlayInfo_BGM);
                gPokedexView->unk64F = 3;
                gTasks[taskId].func = sub_808FFBC;
                PlaySE(SE_Z_PAGE);
            }
            return;
        }
    }
}

static void sub_808FFBC(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        DestroyCryMeterNeedleSprite();
        switch (gPokedexView->unk64F)
        {
        default:
        case 1:
            gTasks[taskId].func = Task_InitPageScreenMultistep;
            break;
        case 2:
            gTasks[taskId].func = Task_InitAreaScreenMultistep;
            break;
        case 3:
            gTasks[taskId].func = Task_InitSizeScreenMultistep;
            break;
        }
    }
}

static void sub_8090040(u8 a)
{
    u16 unk;

    if (a != 0)
        unk = 0x392;
    else
        unk = 0x2AF;
    LoadPalette(&unk, 0x5D, 2);
}

static void Task_InitSizeScreenMultistep(u8 taskId)
{
    u8 spriteId;

    switch (gMain.state)
    {
    default:
    case 0:
        if (!gPaletteFade.active)
        {
            gPokedexView->currentScreen = SIZE_DEXPAGE;
            gUnknown_03005CEC = gMain.vblankCallback;
            SetVBlankCallback(NULL);
            InitBGAndObj(0x200); // init all but bg1
            gPokedexView->selectedScreen = SIZE_SCREEN;
            gMain.state = 1;
        }
        break;
    case 1:
        LZ77UnCompVram(gPokedexMenu_Gfx, (void *)VRAM);
        LZ77UnCompVram(gUnknown_0839F988, (void *)(VRAM + 0x7000));
        gMain.state++;
        break;
    case 2:
        sub_8090540(0xD);
        sub_8090644(3, 0xD);
        sub_808D640();
        gMain.state++;
        break;
    case 3:
        {
            u8 string[40];  //I hope this is the correct size

            Text_LoadWindowTemplate(&gWindowTemplate_81E702C);
            InitMenuWindow(&gWindowTemplate_81E702C);
            string[0] = EOS;
            StringAppend(string, gDexText_SizeComparedTo);
            StringAppend(string, gSaveBlock2.playerName);
            MenuPrint_Centered(string, 3, 15, 0xC0);
            gMain.state++;
        }
        break;
    case 4:
        ResetPaletteFade();
        gMain.state++;
        break;
    case 5:
        spriteId = sub_8091A4C(gSaveBlock2.playerGender, 152, 56, 0);
        gSprites[spriteId].oam.affineMode = 1;
        gSprites[spriteId].oam.matrixNum = 1;
        gSprites[spriteId].oam.priority = 0;
        gSprites[spriteId].pos2.y = gPokedexEntries[gUnknown_0202FFBC->dexNum].trainerOffset;
        SetOamMatrix(1, gPokedexEntries[gUnknown_0202FFBC->dexNum].trainerScale, 0, 0, gPokedexEntries[gUnknown_0202FFBC->dexNum].trainerScale);
        LoadPalette(gUnknown_083B4EC4, (gSprites[spriteId].oam.paletteNum + 16) * 16, sizeof(gUnknown_083B4EC4));
        gMain.state++;
        break;
    case 6:
        spriteId = DisplayMonPic(gUnknown_0202FFBC->dexNum, 88, 56, 1);
        gSprites[spriteId].oam.affineMode = 1;
        gSprites[spriteId].oam.matrixNum = 2;
        gSprites[spriteId].oam.priority = 0;
        gSprites[spriteId].pos2.y = gPokedexEntries[gUnknown_0202FFBC->dexNum].pokemonOffset;
        SetOamMatrix(2, gPokedexEntries[gUnknown_0202FFBC->dexNum].pokemonScale, 0, 0, gPokedexEntries[gUnknown_0202FFBC->dexNum].pokemonScale);
        LoadPalette(gUnknown_083B4EC4, (gSprites[spriteId].oam.paletteNum + 16) * 16, sizeof(gUnknown_083B4EC4));
        gMain.state++;
        break;
    case 7:
        BeginNormalPaletteFade(0xFFFFFFEB, 0, 16, 0, RGB(0, 0, 0));
        SetVBlankCallback(gUnknown_03005CEC);
        gMain.state++;
        break;
    case 8:
        REG_BLDCNT = 0;
        REG_BLDALPHA = 0;
        REG_BLDY = 0;
        REG_BG2CNT = BGCNT_PRIORITY(3) | BGCNT_CHARBASE(0) | BGCNT_SCREENBASE(14) | BGCNT_16COLOR | BGCNT_TXT256x256;
        REG_DISPCNT = DISPCNT_MODE_0 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON | DISPCNT_OBJ_ON;
        gMain.state++;
        break;
    case 9:
        if (!gPaletteFade.active)
        {
            gPokedexView->unk64F = 0;
            gMain.state = 0;
            gTasks[taskId].func = Task_SizeScreenProcessInput;
        }
        break;
    }
}

static void Task_SizeScreenProcessInput(u8 taskId)
{
    if (gMain.newKeys & B_BUTTON)
    {
        BeginNormalPaletteFade(0xFFFFFFEB, 0, 0, 16, RGB(0, 0, 0));
        gPokedexView->unk64F = 1;
        gTasks[taskId].func = sub_8090498;
        PlaySE(SE_PC_OFF);
    }
    else if ((gMain.newKeys & DPAD_LEFT)
     || ((gMain.newKeys & L_BUTTON) && gSaveBlock2.optionsButtonMode == OPTIONS_BUTTON_MODE_LR))
    {
        BeginNormalPaletteFade(0xFFFFFFEB, 0, 0, 16, RGB(0, 0, 0));
        gPokedexView->unk64F = 2;
        gTasks[taskId].func = sub_8090498;
        PlaySE(SE_Z_PAGE);
    }
}

static void sub_8090498(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        switch (gPokedexView->unk64F)
        {
        default:
        case 1:
            gTasks[taskId].func = Task_InitPageScreenMultistep;
            break;
        case 2:
            gTasks[taskId].func = Task_InitCryScreenMultistep;
            break;
        }
    }
}

static void sub_80904FC(u16 a)
{
    LZ77UnCompVram(gUnknown_08E96ACC, (void *)(VRAM + a * 0x800));
    DmaClear16(3, (void *)(VRAM + a * 0x800 + 0xC0), 0x440);
}

static void sub_8090540(u16 a)
{
    LZ77UnCompVram(gUnknown_08E96B58, (void *)(VRAM + a * 0x800));
    DmaClear16(3, (void *)(VRAM + a * 0x800 + 0xC0), 0x440);
}

#ifdef NONMATCHING
static void sub_8090584(u8 a, u16 b)
{
    u8 i;   //r1
    u8 j;   //r3
    u32 r6;
    register u8 r7;

    for (i = 0; i < 4; i++)
    {
        r7 = i * 5 + 1;
        r6 = 0x4000;

        if (i == a)
            r6 = 0x2000;

        for (j = 0; j < 5; j++)
        {
            u32 r0 = b * 0x800 + (r7 + j) * 2;
            u8 *ptr;

            ptr = (void *)VRAM;
            *(u16 *)(ptr + r0) = (*(u16 *)(ptr + r0) & 0xFFF) | r6;
            ptr = (void *)VRAM + 0x40;
            *(u16 *)(ptr + r0) = (*(u16 *)(ptr + r0) & 0xFFF) | r6;
        }
    }
    r6 = 0x4000;
    for (j = 0; j < 5; j++)
    {
        u32 r0 = b * 0x800 + j * 2;
        u8 *ptr;

        ptr = (void *)VRAM + 0x32;
        *(u16 *)(ptr + r0) = (*(u16 *)(ptr + r0) & 0xFFF) | r6;
        ptr = (void *)VRAM + 0x72;
        *(u16 *)(ptr + r0) = (*(u16 *)(ptr + r0) & 0xFFF) | r6;
    }
}
#else
NAKED
static void sub_8090584(u8 a, u16 b)
{
    asm(".syntax unified\n\
    push {r4-r7,lr}\n\
    mov r7, r10\n\
    mov r6, r9\n\
    mov r5, r8\n\
    push {r5-r7}\n\
    lsls r0, 24\n\
    lsrs r0, 24\n\
    mov r10, r0\n\
    lsls r1, 16\n\
    lsrs r1, 16\n\
    mov r9, r1\n\
    movs r1, 0\n\
_0809059C:\n\
    lsls r0, r1, 2\n\
    adds r0, r1\n\
    adds r0, 0x1\n\
    lsls r0, 24\n\
    lsrs r7, r0, 24\n\
    movs r6, 0x80\n\
    lsls r6, 7\n\
    cmp r1, r10\n\
    bne _080905B2\n\
    movs r6, 0x80\n\
    lsls r6, 6\n\
_080905B2:\n\
    movs r3, 0\n\
    mov r0, r9\n\
    lsls r0, 11\n\
    mov r12, r0\n\
    adds r1, 0x1\n\
    mov r8, r1\n\
    mov r5, r12\n\
    ldr r4, _08090634 @ =0x00000fff\n\
_080905C2:\n\
    adds r0, r7, r3\n\
    lsls r0, 1\n\
    adds r0, r5, r0\n\
    movs r2, 0xC0\n\
    lsls r2, 19\n\
    adds r1, r0, r2\n\
    ldrh r2, [r1]\n\
    ands r2, r4\n\
    orrs r2, r6\n\
    strh r2, [r1]\n\
    ldr r1, _08090638 @ =0x06000040\n\
    adds r0, r1\n\
    ldrh r2, [r0]\n\
    ands r2, r4\n\
    orrs r2, r6\n\
    strh r2, [r0]\n\
    adds r0, r3, 0x1\n\
    lsls r0, 24\n\
    lsrs r3, r0, 24\n\
    cmp r3, 0x4\n\
    bls _080905C2\n\
    mov r2, r8\n\
    lsls r0, r2, 24\n\
    lsrs r1, r0, 24\n\
    cmp r1, 0x3\n\
    bls _0809059C\n\
    movs r6, 0x80\n\
    lsls r6, 7\n\
    movs r3, 0\n\
    mov r5, r12\n\
    ldr r4, _08090634 @ =0x00000fff\n\
_08090600:\n\
    lsls r0, r3, 1\n\
    adds r0, r5, r0\n\
    ldr r2, _0809063C @ =0x06000032\n\
    adds r1, r0, r2\n\
    ldrh r2, [r1]\n\
    ands r2, r4\n\
    orrs r2, r6\n\
    strh r2, [r1]\n\
    ldr r1, _08090640 @ =0x06000072\n\
    adds r0, r1\n\
    ldrh r2, [r0]\n\
    ands r2, r4\n\
    orrs r2, r6\n\
    strh r2, [r0]\n\
    adds r0, r3, 0x1\n\
    lsls r0, 24\n\
    lsrs r3, r0, 24\n\
    cmp r3, 0x4\n\
    bls _08090600\n\
    pop {r3-r5}\n\
    mov r8, r3\n\
    mov r9, r4\n\
    mov r10, r5\n\
    pop {r4-r7}\n\
    pop {r0}\n\
    bx r0\n\
    .align 2, 0\n\
_08090634: .4byte 0x00000fff\n\
_08090638: .4byte 0x06000040\n\
_0809063C: .4byte 0x06000032\n\
_08090640: .4byte 0x06000072\n\
    .syntax divided\n");
}
#endif

//Nope, can't get this one to match, either.
#ifdef NONMATCHING
static void sub_8090644(u8 a, u16 b)
{
    u8 i;
    u8 j;

    for (i = 0; i < 4; i++)
    {
        u8 r8 = i * 5 + 1;
        u32 r5;

        if (i == a || i == 0)
            r5 = 0x2000;
        else if (a != 0)
            r5 = 0x4000;

        for (j = 0; j < 5; j++)
        {
            u16 (*vramData)[0x400];

            vramData = (u16 (*)[])VRAM;
            vramData[b][r8 + j] = (vramData[b][r8 + j] & 0xFFF) | r5;
            vramData = (u16 (*)[])(VRAM + 0x40);
            vramData[b][r8 + j] = (vramData[b][r8 + j] & 0xFFF) | r5;
        }
    }

    for (j = 0; j < 5; j++)
    {
        u16 (*vramData)[0x400];

        vramData = (u16 (*)[])(VRAM + 0x32);
        vramData[b][j] = (vramData[b][j] & 0xFFF) | 0x4000;
        vramData = (u16 (*)[])(VRAM + 0x72);
        vramData[b][j] = (vramData[b][j] & 0xFFF) | 0x4000;
    }
}
#else
NAKED
static void sub_8090644(u8 a, u16 b)
{
    asm(".syntax unified\n\
    push {r4-r7,lr}\n\
    mov r7, r10\n\
    mov r6, r9\n\
    mov r5, r8\n\
    push {r5-r7}\n\
    lsls r0, 24\n\
    lsrs r0, 24\n\
    mov r10, r0\n\
    lsls r1, 16\n\
    lsrs r1, 16\n\
    mov r9, r1\n\
    movs r1, 0\n\
_0809065C:\n\
    lsls r0, r1, 2\n\
    adds r0, r1\n\
    adds r0, 0x1\n\
    lsls r0, 24\n\
    lsrs r0, 24\n\
    mov r8, r0\n\
    cmp r1, r10\n\
    beq _08090670\n\
    cmp r1, 0\n\
    bne _08090676\n\
_08090670:\n\
    movs r5, 0x80\n\
    lsls r5, 6\n\
    b _0809067A\n\
_08090676:\n\
    movs r5, 0x80\n\
    lsls r5, 7\n\
_0809067A:\n\
    movs r3, 0\n\
    mov r0, r9\n\
    lsls r7, r0, 11\n\
    adds r1, 0x1\n\
    mov r12, r1\n\
    adds r6, r7, 0\n\
    ldr r4, _080906FC @ =0x00000fff\n\
_08090688:\n\
    mov r1, r8\n\
    adds r0, r1, r3\n\
    lsls r0, 1\n\
    adds r0, r6, r0\n\
    movs r2, 0xC0\n\
    lsls r2, 19\n\
    adds r1, r0, r2\n\
    ldrh r2, [r1]\n\
    ands r2, r4\n\
    orrs r2, r5\n\
    strh r2, [r1]\n\
    ldr r1, _08090700 @ =0x06000040\n\
    adds r0, r1\n\
    ldrh r2, [r0]\n\
    ands r2, r4\n\
    orrs r2, r5\n\
    strh r2, [r0]\n\
    adds r0, r3, 0x1\n\
    lsls r0, 24\n\
    lsrs r3, r0, 24\n\
    cmp r3, 0x4\n\
    bls _08090688\n\
    mov r2, r12\n\
    lsls r0, r2, 24\n\
    lsrs r1, r0, 24\n\
    cmp r1, 0x3\n\
    bls _0809065C\n\
    movs r5, 0x80\n\
    lsls r5, 7\n\
    movs r3, 0\n\
    adds r6, r7, 0\n\
    ldr r4, _080906FC @ =0x00000fff\n\
_080906C8:\n\
    lsls r0, r3, 1\n\
    adds r0, r6, r0\n\
    ldr r2, _08090704 @ =0x06000032\n\
    adds r1, r0, r2\n\
    ldrh r2, [r1]\n\
    ands r2, r4\n\
    orrs r2, r5\n\
    strh r2, [r1]\n\
    ldr r1, _08090708 @ =0x06000072\n\
    adds r0, r1\n\
    ldrh r2, [r0]\n\
    ands r2, r4\n\
    orrs r2, r5\n\
    strh r2, [r0]\n\
    adds r0, r3, 0x1\n\
    lsls r0, 24\n\
    lsrs r3, r0, 24\n\
    cmp r3, 0x4\n\
    bls _080906C8\n\
    pop {r3-r5}\n\
    mov r8, r3\n\
    mov r9, r4\n\
    mov r10, r5\n\
    pop {r4-r7}\n\
    pop {r0}\n\
    bx r0\n\
    .align 2, 0\n\
_080906FC: .4byte 0x00000fff\n\
_08090700: .4byte 0x06000040\n\
_08090704: .4byte 0x06000032\n\
_08090708: .4byte 0x06000072\n\
    .syntax divided\n");
}
#endif

u8 PrepareDexDisplayForCaughtMon(u16 dexNum, u32 b, u32 c)
{
    u8 taskId = CreateTask(DisplayDexForNewMon, 0);

    gTasks[taskId].data[0] = 0;
    gTasks[taskId].data[1] = dexNum;
    gTasks[taskId].data[12] = b;
    gTasks[taskId].data[13] = b >> 16;
    gTasks[taskId].data[14] = c;
    gTasks[taskId].data[15] = c >> 16;
    return taskId;
}

static void DisplayDexForNewMon(u8 taskId)
{
    u8 spriteId;
    u16 dexNum = gTasks[taskId].data[1];
    //u16 i;

    switch (gTasks[taskId].data[0])
    {
    case 0:
    default:
        if (!gPaletteFade.active)
        {
            gUnknown_03005CEC = gMain.vblankCallback;
            SetVBlankCallback(NULL);
            InitBGAndObj(0x100); // init all but bg0
            gTasks[taskId].data[0] = 1;
        }
        break;
    case 1:
        LZ77UnCompVram(gPokedexMenu_Gfx, (void *)(VRAM + 0x4000));
        LZ77UnCompVram(gPokedexPageLayout, (void *)(VRAM + 0x7800));
        PrintFootprints(gTasks[taskId].data[1], 2, 0x3FC);
        ResetPaletteFade();
        LoadPalette(gPokedexMenu_Pal + 1, 0x21, 0x9E);
        gTasks[taskId].data[0]++;
        break;
    case 2:
        Text_LoadWindowTemplate(&gWindowTemplate_81E7064);
        InitMenuWindow(&gWindowTemplate_81E7064);
        DmaClear16(3, (void *)(VRAM + 0xC000), 0x200);
        gTasks[taskId].data[0]++;
        break;
    case 3:
        MenuPrint_Centered(gDexText_RegisterComplete, 2, 0, 0xD0);
        if (!IsNationalPokedexEnabled())
            sub_8091154(NationalToHoennOrder(dexNum), 13, 3);
        else
            sub_8091154(dexNum, 13, 3);
        sub_80911C8(dexNum, 16, 3);
        Menu_PrintText(gDexText_UnknownPoke, CATEGORY_LEFT, 5);
        Menu_PrintText(gDexText_UnknownHeight, 16, 7);
        Menu_PrintText(gDexText_UnknownWeight, 16, 9);
        sub_8091304(gPokedexEntries[dexNum].categoryName, CATEGORY_LEFT, 5);
        sub_8091458(gPokedexEntries[dexNum].height, 16, 7);
        sub_8091564(gPokedexEntries[dexNum].weight, 16, 9);
        Menu_PrintText(gPokedexEntries[dexNum].description, DESCRIPTION_X_POSITION, DESCRIPTION_Y_POSITION);
        sub_80917CC(14, 0x3FC);
        gTasks[taskId].data[0]++;
        break;
    case 4:
        spriteId = DisplayMonPic(dexNum, X_POSITION_FOR_DESCRIPTION_PAGE, Y_POSITION_FOR_DESCRIPTION_PAGE, 0);
        gSprites[spriteId].oam.priority = 0;
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB(0, 0, 0));
        SetVBlankCallback(gUnknown_03005CEC);
        gTasks[taskId].data[3] = spriteId;
        gTasks[taskId].data[0]++;
        break;
    case 5:
        REG_BLDCNT = 0;
        REG_BLDALPHA = 0;
        REG_BLDY = 0;
        REG_BG3CNT = BGCNT_PRIORITY(3) | BGCNT_CHARBASE(1) | BGCNT_SCREENBASE(15) | BGCNT_16COLOR | BGCNT_TXT256x256;
        REG_DISPCNT = DISPCNT_MODE_0 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG2_ON | DISPCNT_BG3_ON | DISPCNT_OBJ_ON;
        gTasks[taskId].data[0]++;
        break;
    case 6:
        if (!gPaletteFade.active)
        {
            PlayCry1(NationalPokedexNumToSpecies(dexNum), 0);
            gTasks[taskId].data[2] = 0;
            gTasks[taskId].data[4] = 0;
            gTasks[taskId].func = sub_8090A3C;
        }
        break;
    }
}

static void sub_8090A3C(u8 taskId)
{
    if (gMain.newKeys & B_BUTTON)
    {
        BeginNormalPaletteFade(0xFFFC, 0, 0, 16, RGB(0, 0, 0));
        gSprites[gTasks[taskId].data[3]].callback = sub_8090C28;
        gTasks[taskId].func = sub_8090B8C;
        return;
    }
    gTasks[taskId].data[2]++;
    if (gTasks[taskId].data[2] & 0x10)
        LoadPalette(gPokedexMenu_Pal + 1, 0x51, 14);
    else
        LoadPalette(gPokedexMenu2_Pal + 1, 0x51, 14);
}

static void sub_8090B8C(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        u16 species;
        u32 otId;
        u32 personality;
        u8 paletteNum;
        const u8 *lzPaletteData;

        REG_DISPCNT = DISPCNT_MODE_0 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG3_ON | DISPCNT_OBJ_ON;
        CpuCopy16(gBattleTextboxTilemap, (void *)(VRAM + 0xC000), 0x1000);
        ApplyPlayerChosenFrameToBattleMenu();
        species = NationalPokedexNumToSpecies(gTasks[taskId].data[1]);
        otId = ((u16)gTasks[taskId].data[13] << 16) | (u16)gTasks[taskId].data[12];
        personality = ((u16)gTasks[taskId].data[15] << 16) | (u16)gTasks[taskId].data[14];
        paletteNum = gSprites[gTasks[taskId].data[3]].oam.paletteNum;
        lzPaletteData = GetMonSpritePalFromOtIdPersonality(species, otId, personality);
        LoadCompressedPalette(lzPaletteData, 0x100 | paletteNum * 16, 32);
        DestroyTask(taskId);
    }
}

static void sub_8090C28(struct Sprite *sprite)
{
    if (sprite->pos1.x < 0x78)
        sprite->pos1.x += 2;
    if (sprite->pos1.x > 0x78)
        sprite->pos1.x -= 2;

    if (sprite->pos1.y < 0x50)
        sprite->pos1.y += 1;
    if (sprite->pos1.y > 0x50)
        sprite->pos1.y -= 1;
}

const u8 *GetPokemonCategory(u16 dexNum)
{
    return gPokedexEntries[dexNum].categoryName;
}

u16 GetPokedexHeightWeight(u16 dexNum, u8 data)
{
    switch (data)
    {
    case 0:  // height
        return gPokedexEntries[dexNum].height;
    case 1:  // weight
        return gPokedexEntries[dexNum].weight;
    default:
        return 1;
    }
}

s8 GetSetPokedexFlag(u16 nationalDexNo, u8 caseID)
{
    u8 index;
    u8 bit;
    u8 mask;
    s8 retVal;
    int i = 0;

    nationalDexNo--;
    index = nationalDexNo / 8;
    bit = nationalDexNo % 8;
    mask = 1 << bit;
    retVal = 0;
    switch (caseID)
    {
    case FLAG_GET_SEEN:
        retVal = (gSaveBlock2.pokedex.seen[index] & mask) != 0;
        break;
    case FLAG_GET_CAUGHT:
        retVal = (gSaveBlock2.pokedex.owned[index] & mask) != 0;
        break;
    case FLAG_SET_SEEN:
        gSaveBlock2.pokedex.seen[index] |= mask;
        break;
    case FLAG_SET_CAUGHT:
        gSaveBlock2.pokedex.owned[index] |= mask;
        break;
    case FLAG_SET_ALL:
        for (i = 0; i < DEX_FLAGS_NO; i++)
        {
            gSaveBlock2.pokedex.seen[i] = 0xFF;
            gSaveBlock2.pokedex.owned[i] = 0xFF;
        }
        break;
    }
    return retVal;
}

u16 GetNationalPokedexCount(u8 caseID)
{
    u16 count = 0;
    u16 i;

    for (i = 0; i < NATIONAL_DEX_COUNT; i++)
    {
        switch (caseID)
        {
        case FLAG_GET_SEEN:
            if (GetSetPokedexFlag(i + 1, FLAG_GET_SEEN))
                count++;
            break;
        case FLAG_GET_CAUGHT:
            if (GetSetPokedexFlag(i + 1, FLAG_GET_CAUGHT))
                count++;
            break;
        }
    }
    return count;
}

u16 GetHoennPokedexCount(u8 caseID)
{
    u16 count = 0;
    u16 i;

    for (i = 0; i < HOENN_DEX_COUNT; i++)
    {
        switch (caseID)
        {
        case FLAG_GET_SEEN:
            if (GetSetPokedexFlag(HoennToNationalOrder(i + 1), FLAG_GET_SEEN))
                count++;
            break;
        case FLAG_GET_CAUGHT:
            if (GetSetPokedexFlag(HoennToNationalOrder(i + 1), FLAG_GET_CAUGHT))
                count++;
            break;
        }
    }
    return count;
}

bool8 CompletedHoennPokedex(void)
{
    u16 i;

    for (i = 0; i < 200; i++)
    {
        if (!GetSetPokedexFlag(HoennToNationalOrder(i + 1), FLAG_GET_CAUGHT))
            return FALSE;
    }
    return TRUE;
}

bool16 CompletedNationalPokedex(void)
{
    u16 i;

    // BUG: This function indexes pokemon checks by 0, but adds
    // 1 before passing to GetSetPokedexFlag. Normally, this is
    // fine, because GetSetPokedexFlag subtracts by 1 to get the
    // array index value, but since the array is 0 indexed
    // starting with Bulbasaur, values passed actually means that
    // dex entries 152 (Chikorita) and 252 (Treecko) are skipped.
    // Because an earlier Hoenn Dex check prevented Treecko from
    // being skippable, it means that Chikorita is not required
    // to obtain the National Diploma. This was fixed in Emerald.
    for (i = 0; i < 150; i++)
    {
        if (GetSetPokedexFlag(i + 1, 1) == 0)
            return FALSE;
    }
    for (i = 152; i < 250; i++)
    {
        if (GetSetPokedexFlag(i + 1, 1) == 0)
            return FALSE;
    }
    for (i = 252; i < 384; i++)
    {
        if (GetSetPokedexFlag(i + 1, 1) == 0)
            return FALSE;
    }
    return TRUE;
}

static void InitBGAndObj(u16 a)
{
    if (!(a & 0x100))
    {
        REG_DISPCNT &= 0xFEFF;
        REG_BG0CNT = 0;
        REG_BG0HOFS = 0;
        REG_BG0VOFS = 0;
    }
    if (!(a & 0x200))
    {
        REG_DISPCNT &= 0xFDFF;
        REG_BG1CNT = 0;
        REG_BG1HOFS = 0;
        REG_BG1VOFS = 0;
    }
    if (!(a & 0x400))
    {
        REG_DISPCNT &= 0xFBFF;
        REG_BG2CNT = 0;
        REG_BG2HOFS = 0;
        REG_BG2VOFS = 0;
    }
    if (!(a & 0x800))
    {
        REG_DISPCNT &= 0xF7FF;
        REG_BG3CNT = 0;
        REG_BG3HOFS = 0;
        REG_BG3VOFS = 0;
    }
    if (!(a & 0x1000))
    {
        REG_DISPCNT &= 0xEFFF;
        ResetSpriteData();
        FreeAllSpritePalettes();
        gReservedSpritePaletteCount = 8;
    }
}

static void sub_8091154(u16 order, u8 b, u8 c)
{
    u8 str[4];

    str[0] = CHAR_0 + order / 100;
    str[1] = CHAR_0 + (order % 100) / 10;
    str[2] = CHAR_0 + (order % 100) % 10;
    str[3] = EOS;
    Menu_PrintText(str, b, c);
}

static u8 sub_80911C8(u16 num, u8 b, u8 c)
{
    u8 str[11];
    u8 i;

    for (i = 0; i < 11; i++)
        str[i] = EOS;
    num = NationalPokedexNumToSpecies(num);
    switch (num)
    {
    default:
        for (i = 0; gSpeciesNames[num][i] != EOS && i < 10; i++)
            str[i] = gSpeciesNames[num][i];
        break;
    case 0:
        for (i = 0; i < 10; i++)
            str[i] = 0xAE;
        break;
    }
    Menu_PrintText(str, b, c);
    return i;
}

static u8 sub_8091260(u16 num, u8 b, u8 c, u8 d)
{
    u8 str[40];
    u8 *end;
    u8 i;

    end = StringCopy(str, gUnknown_083B5558);
    str[2] = d;
    num = NationalPokedexNumToSpecies(num);
    switch (num)
    {
    default:
        for (i = 0; gSpeciesNames[num][i] != EOS && i < 10; i++)
            end[i] = gSpeciesNames[num][i];
        break;
    case 0:
        for (i = 0; i < 10; i++)
            end[i] = 0xAE;
        break;
    }
    end[i] = EOS;
    Menu_PrintText(str, b, c);
    return i;
}

static void sub_8091304(const u8 *name, u8 left, u8 top)
{
    u8 str[32];
    u8 i;
#if ENGLISH
    u8 j;
#endif

    for (i = 0; name[i] != EOS && i < 11; i++)
        str[i] = name[i];
#if ENGLISH
    for (j = 0; gDexText_UnknownPoke[j] == 0xAC || gDexText_UnknownPoke[j] == 0; j++)
        ;
    j--;
    while (gDexText_UnknownPoke[j] != EOS)
        str[i++] = gDexText_UnknownPoke[j++];
#endif
    str[i] = EOS;
    sub_8072B80(str, left, top, gDexText_UnknownPoke);
}

#if ENGLISH
void unref_sub_80913A4(u16 a, u8 left, u8 top)
{
    u8 str[6];
    bool8 outputted = FALSE;
    u8 result;

    result = a / 1000;
    if (result == 0)
    {
        str[0] = CHAR_SPACE;
        outputted = FALSE;
    }
    else
    {
        str[0] = CHAR_0 + result;
        outputted = TRUE;
    }

    result = (a % 1000) / 100;
    if (result == 0 && !outputted)
    {
        str[1] = CHAR_SPACE;
        outputted = FALSE;
    }
    else
    {
        str[1] = CHAR_0 + result;
        outputted = TRUE;
    }

    str[2] = CHAR_0 + ((a % 1000) % 100) / 10;
    str[3] = CHAR_PERIOD;
    str[4] = CHAR_0 + ((a % 1000) % 100) % 10;
    str[5] = EOS;
    Menu_PrintText(str, left, top);
}
#elif GERMAN
void unref_sub_80913A4(u16 arg0, u8 left, u8 top) {
    u8 buffer[8];
    int offset;
    u8 result;

    u8 r6 = 0;
    offset = 0;


    buffer[r6++] = 0xFC;
    buffer[r6++] = 0x13;
    r6++;

    result = (arg0 / 1000);
    if (result == 0)
    {
        offset = 6;
    }
    else
    {
        buffer[r6++] = result + CHAR_0;
    }


    result = (arg0 % 1000) / 100;

    if (result == 0 && offset != 0)
    {
        offset += 6;
    }
    else
    {
        buffer[r6++] = result + CHAR_0;
    }

    buffer[r6++] = (((arg0 % 1000) % 100) / 10) + CHAR_0;
    buffer[r6++] = CHAR_COMMA;
    buffer[r6++] = (((arg0 % 1000) % 100) % 10) + CHAR_0;

    buffer[r6++] = EOS;
    buffer[2] = offset;
    Menu_PrintText(buffer, left, top);
}
#endif

#ifdef UNITS_IMPERIAL
#define CHAR_PRIME (0xB4)
#define CHAR_DOUBLE_PRIME (0xB2)
static void sub_8091458(u16 height, u8 left, u8 top)
{
    u8 buffer[16];
    u32 inches, feet;
    u8 i = 0;

    inches = (height * 10000) / 254;
    if (inches % 10 >= 5)
        inches += 10;
    feet = inches / 120;
    inches = (inches - (feet * 120)) / 10;

    buffer[i++] = EXT_CTRL_CODE_BEGIN;
    buffer[i++] = 0x13;
    if (feet / 10 == 0)
    {
        buffer[i++] = 18;
        buffer[i++] = feet + CHAR_0;
    }
    else
    {
        buffer[i++] = 12;
        buffer[i++] = feet / 10 + CHAR_0;
        buffer[i++] = (feet % 10) + CHAR_0;
    }
    buffer[i++] = CHAR_PRIME;
    buffer[i++] = (inches / 10) + CHAR_0;
    buffer[i++] = (inches % 10) + CHAR_0;
    buffer[i++] = CHAR_DOUBLE_PRIME;
    buffer[i++] = EOS;
    Menu_PrintText(buffer, left, top);
}
#else
static void sub_8091458(u16 height, u8 left, u8 top)
{
    unref_sub_80913A4(height, left, top);
}
#endif

#ifdef UNITS_IMPERIAL
static void sub_8091564(u16 weight, u8 left, u8 top)
{
    u8 buffer[16];
    u32 lbs;
    u8 i = 0;
    bool8 output;

    lbs = (weight * 100000) / 4536;
    if (lbs % 10 >= 5)
        lbs += 10;
    output = FALSE;

    buffer[i] = (lbs / 100000) + CHAR_0;
    if (buffer[i] == CHAR_0 && output == FALSE)
    {
        buffer[i++] = CHAR_SPACE;
        buffer[i++] = CHAR_SPACE;
    }
    else
    {
        output = TRUE;
        i++;
    }

    lbs = (lbs % 100000);
    buffer[i] = (lbs / 10000) + CHAR_0;
    if (buffer[i] == CHAR_0 && output == FALSE)
    {
        buffer[i++] = CHAR_SPACE;
        buffer[i++] = CHAR_SPACE;
    }
    else
    {
        output = TRUE;
        i++;
    }

    lbs = (lbs % 10000);
    buffer[i] = (lbs / 1000) + CHAR_0;
    if (buffer[i] == CHAR_0 && output == FALSE)
    {
        buffer[i++] = CHAR_SPACE;
        buffer[i++] = CHAR_SPACE;
    }
    else
    {
        output = TRUE;
        i++;
    }
    lbs = (lbs % 1000);
    buffer[i++] = (lbs / 100) + CHAR_0;
    lbs = (lbs % 100);
    buffer[i++] = CHAR_PERIOD;
    buffer[i++] = (lbs / 10) + CHAR_0;
    buffer[i++] = CHAR_SPACE;
    buffer[i++] = CHAR_l;
    buffer[i++] = CHAR_b;
    buffer[i++] = CHAR_s;
    buffer[i++] = CHAR_PERIOD;
    buffer[i++] = EOS;
    Menu_PrintText(buffer, left, top);
}
#else
static void sub_8091564(u16 arg0, u8 left, u8 top)
{
    unref_sub_80913A4(arg0, left, top);
}
#endif

void PrintFootprints(u16 num, u16 b, u16 c)
{
    u8 arr[0x80];
    u16 i;
    u16 j;
    const u8 *r12;
    u16 r7;
    u8 r3;

    r12 = gMonFootprint_Rayquaza;

    for (r7 = 0, i = 0; i < 32; i++)
    {
        r3 = r12[i];
        for (j = 0; j < 4; j++)
        {
            u32 r1 = j * 2;
            s32 r2 = (r3 >> r1) & 1;

            if (r3 & (2 << r1))
                r2 |= 0x10;

// Needed to match
#ifndef NONMATCHING
            asm("");asm("");asm("");asm("");asm("");
#endif

            arr[r7] = r2;
            r7++;
        }
    }
    CpuCopy16(arr, (u16 *)(VRAM + b * 0x4000 + c * 0x20), 0x80);

    SafeLoadMonIconPalette(num);
    CreateMonIcon(num, sub_809D62C, 208, 68, 3, 0);
}

static void sub_80917CC(u16 a, u16 b)
{
    *(u16 *)(VRAM + a * 0x800 + 0x232) = 0xF000 + b + 0;
    *(u16 *)(VRAM + a * 0x800 + 0x234) = 0xF000 + b + 1;
    *(u16 *)(VRAM + a * 0x800 + 0x272) = 0xF000 + b + 2;
    *(u16 *)(VRAM + a * 0x800 + 0x274) = 0xF000 + b + 3;
}

static u16 GetSlotNumbersFromMinMax(u8 a, u16 b, u16 c, u16 d)
{
    switch (a)
    {
    case 1:
        if (b > c)
            b--;
        break;
    case 0:
        if (b < d)
            b++;
        break;
    case 3:
        if (b > c)
            b--;
        else
            b = d;
        break;
    case 2:
        if (b < d)
            b++;
        else
            b = c;
        break;
    }
    return b;
}

static void nullsub_59(struct Sprite *sprite)
{
}

static void sub_8091878(u16 a, u8 b)
{
    gUnknown_02024E8C = gUnknown_083B57A4;
    gUnknown_02024E8C.paletteTag = a;
    gUnknown_02024E8C.images = gUnknown_083B5794[b];
    gUnknown_02024E8C.anims = gSpriteAnimTable_81E7C64;
}

static void sub_80918B0(u16 a, u8 b)
{
    gUnknown_02024E8C = gUnknown_083B57A4;
    gUnknown_02024E8C.paletteTag = a;
    gUnknown_02024E8C.images = gUnknown_083B5794[b];
    gUnknown_02024E8C.anims = gUnknown_081EC2A4[0];
}

u16 DisplayMonPic(u16 num, s16 x, s16 y, u16 paletteNum)
{
    u8 spriteId;

    num = NationalPokedexNumToSpecies(num);
    switch (num)
    {
    default:
        DecompressPicFromTable_2(
          &gMonFrontPicTable[num],
          gMonFrontPicCoords[num].coords,
          gMonFrontPicCoords[num].y_offset,
          (void*)ewram_addr,
          gUnknown_083B5584[paletteNum],
          num);
        break;
    case SPECIES_SPINDA:
        LoadSpecialPokePic(
          &gMonFrontPicTable[num],
          gMonFrontPicCoords[num].coords,
          gMonFrontPicCoords[num].y_offset,
          ewram_addr,
          gUnknown_083B5584[paletteNum],
          num,
          gSaveBlock2.pokedex.spindaPersonality,
          1);
        break;
    case SPECIES_UNOWN:
        LoadSpecialPokePic(
          &gMonFrontPicTable[num],
          gMonFrontPicCoords[num].coords,
          gMonFrontPicCoords[num].y_offset,
          ewram_addr,
          gUnknown_083B5584[paletteNum],
          num,
          gSaveBlock2.pokedex.unownPersonality,
          1);
        break;
    }
    LoadCompressedPalette(gMonPaletteTable[num].data, 0x100 + paletteNum * 16, 32);
    sub_8091878(paletteNum, paletteNum);
    spriteId = CreateSprite(&gUnknown_02024E8C, x, y, 0);
    gSprites[spriteId].oam.paletteNum = paletteNum;
    return spriteId;
}

u16 sub_8091A4C(u16 gender, s16 x, s16 y, u16 paletteNum)
{
    u8 spriteId;

    DecompressPicFromTable_2(
      &gTrainerFrontPicTable[gender],
      gTrainerFrontPicCoords[gender].coords,
      gTrainerFrontPicCoords[gender].y_offset,
      (void*)ewram_addr,
      gUnknown_083B5584[0],
      gender);
    sub_80918B0(gender, 0);
    spriteId = CreateSprite(&gUnknown_02024E8C, x, y, 0);
    gSprites[spriteId].oam.paletteNum = paletteNum;
    return spriteId;
}

int sub_8091AF8(u8 a, u8 b, u8 abcGroup, u8 bodyColor, u8 type1, u8 type2)
{
    u16 species;
    u16 i;
    u16 resultsCount;
    u8 types[2];

    SortPokedex(a, b);

    for (i = 0, resultsCount = 0; i < NATIONAL_DEX_COUNT; i++)
    {
        if (gPokedexView->unk0[i].seen)
        {
            gPokedexView->unk0[resultsCount] = gPokedexView->unk0[i];
            resultsCount++;
        }
    }
    gPokedexView->pokemonListCount = resultsCount;

    // Search by name
    if (abcGroup != 0xFF)
    {
        for (i = 0, resultsCount = 0; i < gPokedexView->pokemonListCount; i++)
        {
            u8 r3;

            species = NationalPokedexNumToSpecies(gPokedexView->unk0[i].dexNum);
            r3 = gSpeciesNames[species][0];
            if ((r3 >= gUnknown_083B57BC[abcGroup][0] && r3 < gUnknown_083B57BC[abcGroup][0] + gUnknown_083B57BC[abcGroup][1])
             || (r3 >= gUnknown_083B57BC[abcGroup][2] && r3 < gUnknown_083B57BC[abcGroup][2] + gUnknown_083B57BC[abcGroup][3]))
            {
                gPokedexView->unk0[resultsCount] = gPokedexView->unk0[i];
                resultsCount++;
            }
        }
        gPokedexView->pokemonListCount = resultsCount;
    }

    // Search by body color
    if (bodyColor != 0xFF)
    {
        for (i = 0, resultsCount = 0; i < gPokedexView->pokemonListCount; i++)
        {
            species = NationalPokedexNumToSpecies(gPokedexView->unk0[i].dexNum);

            if (bodyColor == gBaseStats[species].bodyColor)
            {
                gPokedexView->unk0[resultsCount] = gPokedexView->unk0[i];
                resultsCount++;
            }
        }
        gPokedexView->pokemonListCount = resultsCount;
    }

    // Search by type
    if (type1 != 0xFF || type2 != 0xFF)
    {
        if (type1 == 0xFF)
        {
            type1 = type2;
            type2 = 0xFF;
        }

        if (type2 == 0xFF)
        {
            for (i = 0, resultsCount = 0; i < gPokedexView->pokemonListCount; i++)
            {
                if (gPokedexView->unk0[i].owned)
                {
                    species = NationalPokedexNumToSpecies(gPokedexView->unk0[i].dexNum);

                    types[0] = gBaseStats[species].type1;
                    types[1] = gBaseStats[species].type2;
                    if (types[0] == type1 || types[1] == type1)
                    {
                        gPokedexView->unk0[resultsCount] = gPokedexView->unk0[i];
                        resultsCount++;
                    }
                }
            }
        }
        else
        {
            for (i = 0, resultsCount = 0; i < gPokedexView->pokemonListCount; i++)
            {
                if (gPokedexView->unk0[i].owned)
                {
                    species = NationalPokedexNumToSpecies(gPokedexView->unk0[i].dexNum);

                    types[0] = gBaseStats[species].type1;
                    types[1] = gBaseStats[species].type2;
                    if ((types[0] == type1 && types[1] == type2) || (types[0] == type2 && types[1] == type1))
                    {
                        gPokedexView->unk0[resultsCount] = gPokedexView->unk0[i];
                        resultsCount++;
                    }
                }
            }
        }
        gPokedexView->pokemonListCount = resultsCount;
    }

    if (gPokedexView->pokemonListCount != 0)
    {
        for (i = gPokedexView->pokemonListCount; i < NATIONAL_DEX_COUNT; i++)
        {
            gPokedexView->unk0[i].dexNum = 0xFFFF;
            gPokedexView->unk0[i].seen = FALSE;
            gPokedexView->unk0[i].owned = FALSE;

        }
    }

    return resultsCount;
}

#if ENGLISH
#define SUB_8091E20_WIDTH (208)
#elif GERMAN
#define SUB_8091E20_WIDTH (216)
#endif

void sub_8091E20(const u8 *str)
{
    sub_8072AB0(str, 9, 120, SUB_8091E20_WIDTH, 32, 1);
}

u8 sub_8091E3C(void)
{
    return CreateTask(sub_8091E54, 0);
}

static void sub_8091E54(u8 taskId)
{
    u16 i;

    switch (gMain.state)
    {
    default:
    case 0:
        if (!gPaletteFade.active)
        {
            gPokedexView->currentScreen = DEXPAGE_2;
            InitBGAndObj(0); // init everything
            LZ77UnCompVram(gPokedexMenuSearch_Gfx, (void *)VRAM);
            LZ77UnCompVram(gUnknown_08E96D2C, (void *)(VRAM + 0x7800));
            LoadPalette(gPokedexMenuSearch_Pal + 1, 1, 0x7E);
            if (!IsNationalPokedexEnabled())
            {
                for (i = 0; i < 17; i++)
                {
                    ((u16 *)(VRAM + 0x7A80))[i] = ((u16 *)(VRAM + 0x7B00))[i];
                    ((u16 *)(VRAM + 0x7AC0))[i] = ((u16 *)(VRAM + 0x7B40))[i];
                    ((u16 *)(VRAM + 0x7B00))[i] = 1;
                    ((u16 *)(VRAM + 0x7B40))[i] = 1;
                }
            }
            gMain.state = 1;
        }
        break;
    case 1:
        Text_LoadWindowTemplate(&gWindowTemplate_81E7064);
        InitMenuWindow(&gWindowTemplate_81E7064);
        LoadCompressedObjectPic(&gUnknown_083A05CC[0]);
        LoadSpritePalettes(gUnknown_083A05DC);
        sub_809308C(taskId);
        for (i = 0; i < 16; i++)
            gTasks[taskId].data[i] = 0;
        sub_8092EB0(taskId);
        sub_8092AB0(0);
        sub_8092B68(taskId);
        gMain.state++;
        break;
    case 2:
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB(0, 0, 0));
        gMain.state++;
        break;
    case 3:
        REG_BG3CNT = 0x0F03;
        REG_DISPCNT = 0x1C40;
        gMain.state++;
        break;
    case 4:
        if (!gPaletteFade.active)
        {
            gTasks[taskId].func = sub_809204C;
            gMain.state = 0;
        }
        break;
    }
}

static void sub_809204C(u8 taskId)
{
    sub_8092AB0(gTasks[taskId].data[0]);
    sub_8092B68(taskId);
    gTasks[taskId].func = sub_809207C;
}

static void sub_809207C(u8 taskId)
{
    if (gMain.newKeys & B_BUTTON)
    {
        PlaySE(SE_PC_OFF);
        gTasks[taskId].func = sub_80927B8;
        return;
    }
    if (gMain.newKeys & A_BUTTON)
    {
        switch (gTasks[taskId].data[0])
        {
        case 0:
            PlaySE(SE_PIN);
            gTasks[taskId].data[1] = 0;
            gTasks[taskId].func = sub_809217C;
            break;
        case 1:
            PlaySE(SE_PIN);
            gTasks[taskId].data[1] = 4;
            gTasks[taskId].func = sub_809217C;
            break;
        case 2:
            PlaySE(SE_PC_OFF);
            gTasks[taskId].func = sub_80927B8;
            break;
        }
        return;
    }
    if ((gMain.newKeys & DPAD_LEFT) && gTasks[taskId].data[0] > 0)
    {
        PlaySE(SE_Z_PAGE);
        gTasks[taskId].data[0]--;
        sub_8092AB0(gTasks[taskId].data[0]);
    }
    if ((gMain.newKeys & DPAD_RIGHT) && gTasks[taskId].data[0] < 2)
    {
        PlaySE(SE_Z_PAGE);
        gTasks[taskId].data[0]++;
        sub_8092AB0(gTasks[taskId].data[0]);
    }
}

static void sub_809217C(u8 taskId)
{
    sub_8092AD4(gTasks[taskId].data[0], gTasks[taskId].data[1]);
    sub_8092B68(taskId);
    gTasks[taskId].func = HandleInputSearchScreen;
}

static void HandleInputSearchScreen(u8 taskId) // 
{
    const u8 (*r6)[4];

    if (gTasks[taskId].data[0] != 0)
    {
        if (!IsNationalPokedexEnabled())
            r6 = gUnknown_083B58A4;
        else
            r6 = gUnknown_083B586C;
    }
    else
    {
        if (!IsNationalPokedexEnabled())
            r6 = gUnknown_083B5888;
        else
            r6 = gUnknown_083B5850;
    }

    if (gMain.newKeys & B_BUTTON)
    {
        PlaySE(SE_BOWA);
        sub_8092EB0(taskId);
        gTasks[taskId].func = sub_809204C;
        return;
    }
    if (gMain.newKeys & A_BUTTON)
    {
        if (gTasks[taskId].data[1] == 6)
        {
            if (gTasks[taskId].data[0] != 0)
            {
                gUnknown_0202FFBA = 0x40;
                gPokedexView->unk62A = 0x40;
                gUnknown_0202FFB8 = 0;
                gPokedexView->unk610 = 0;
                gSaveBlock2.pokedex.unknown1 = sub_8092E10(taskId, 5);
                if (!IsNationalPokedexEnabled())
                    gSaveBlock2.pokedex.unknown1 = 0;
                gPokedexView->unk614 = gSaveBlock2.pokedex.unknown1;
                gSaveBlock2.pokedex.order = sub_8092E10(taskId, 4);
                gPokedexView->unk618 = gSaveBlock2.pokedex.order;
                PlaySE(SE_PC_OFF);
                gTasks[taskId].func = sub_80927B8;
            }
            else
            {
                sub_8091E20(gDexText_Searching);
                gTasks[taskId].func = sub_80923FC;
                PlaySE(SE_Z_SEARCH);
            }
        }
        else
        {
            PlaySE(SE_PIN);
            gTasks[taskId].func = sub_80925CC;
        }
        return;
    }

    if ((gMain.newKeys & DPAD_LEFT) && r6[gTasks[taskId].data[1]][0] != 0xFF)
    {
        PlaySE(SE_SELECT);
        gTasks[taskId].data[1] = r6[gTasks[taskId].data[1]][0];
        sub_8092AD4(gTasks[taskId].data[0], gTasks[taskId].data[1]);
    }
    if ((gMain.newKeys & DPAD_RIGHT) && r6[gTasks[taskId].data[1]][1] != 0xFF)
    {
        PlaySE(SE_SELECT);
        gTasks[taskId].data[1] = r6[gTasks[taskId].data[1]][1];
        sub_8092AD4(gTasks[taskId].data[0], gTasks[taskId].data[1]);
    }
    if ((gMain.newKeys & DPAD_UP) && r6[gTasks[taskId].data[1]][2] != 0xFF)
    {
        PlaySE(SE_SELECT);
        gTasks[taskId].data[1] = r6[gTasks[taskId].data[1]][2];
        sub_8092AD4(gTasks[taskId].data[0], gTasks[taskId].data[1]);
    }
    if ((gMain.newKeys & DPAD_DOWN) && r6[gTasks[taskId].data[1]][3] != 0xFF)
    {
        PlaySE(SE_SELECT);
        gTasks[taskId].data[1] = r6[gTasks[taskId].data[1]][3];
        sub_8092AD4(gTasks[taskId].data[0], gTasks[taskId].data[1]);
    }
}

static void sub_80923FC(u8 taskId)
{
    u8 r10 = sub_8092E10(taskId, 5);
    u8 r9 = sub_8092E10(taskId, 4);
    u8 r8 = sub_8092E10(taskId, 0);
    u8 r6 = sub_8092E10(taskId, 1);
    u8 r4 = sub_8092E10(taskId, 2); // type1
    u8 r0 = sub_8092E10(taskId, 3); // type2

    sub_8091AF8(r10, r9, r8, r6, r4, r0);
    gTasks[taskId].func = sub_80924A4;
}

static void sub_80924A4(u8 taskId)
{
    if (!IsSEPlaying())
    {
        if (gPokedexView->pokemonListCount != 0)
        {
            PlaySE(SE_SEIKAI);
            sub_8091E20(gDexText_SearchComplete);
        }
        else
        {
            PlaySE(SE_HAZURE);
            sub_8091E20(gDexText_NoMatching);
        }
        gTasks[taskId].func = sub_8092508;
    }
}

static void sub_8092508(u8 taskId)
{
    if (gMain.newKeys & A_BUTTON)
    {
        if (gPokedexView->pokemonListCount != 0)
        {
            gPokedexView->unk64F = 1;
            gPokedexView->dexMode = sub_8092E10(taskId, 5);
            gPokedexView->dexOrder = sub_8092E10(taskId, 4);
            gTasks[taskId].func = sub_80927B8;
            PlaySE(SE_PC_OFF);
        }
        else
        {
            gTasks[taskId].func = sub_809217C;
            PlaySE(SE_BOWA);
        }
    }
}

static void sub_80925B4(u16 a, int unused)
{
    MenuCursor_SetPos814AD7C(0x90, (a * 2 + 1) * 8);
}

static void sub_80925CC(u8 taskId)
{
    u8 r0;
    u16 *p1;
    u16 *p2;

    sub_8092C8C(0);
    r0 = gTasks[taskId].data[1];
    p1 = &gTasks[taskId].data[gUnknown_083B5A7C[r0].unk4];
    p2 = &gTasks[taskId].data[gUnknown_083B5A7C[r0].unk5];
    gTasks[taskId].data[14] = *p1;
    gTasks[taskId].data[15] = *p2;
    sub_8092D78(taskId);
    CreateBlendedOutlineCursor(16, 0xFFFF, 12, 0x2D9F, 11);
    sub_80925B4(*p1, 1);
    gTasks[taskId].func = sub_8092644;
}

static void sub_8092644(u8 taskId)
{
    u8 r1;
    const struct UnknownStruct2 *r8;
    u16 *p1;
    u16 *p2;
    u16 numOptions;
    bool8 r3;

    r1 = gTasks[taskId].data[1];
    r8 = gUnknown_083B5A7C[r1].unk0;
    p1 = &gTasks[taskId].data[gUnknown_083B5A7C[r1].unk4];
    p2 = &gTasks[taskId].data[gUnknown_083B5A7C[r1].unk5];
    numOptions = gUnknown_083B5A7C[r1].unk6 - 1;

    if (gMain.newKeys & A_BUTTON)
    {
        sub_814ADC8();
        PlaySE(SE_PIN);
        Menu_EraseWindowRect(18, 1, 28, 12);
        sub_8092C8C(1);
        gTasks[taskId].func = sub_809217C;
        return;
    }
    if (gMain.newKeys & B_BUTTON)
    {
        sub_814ADC8();
        PlaySE(SE_BOWA);
        Menu_EraseWindowRect(18, 1, 28, 12);
        sub_8092C8C(1);
        *p1 = gTasks[taskId].data[14];
        *p2 = gTasks[taskId].data[15];
        gTasks[taskId].func = sub_809217C;
        return;
    }
    r3 = FALSE;
    if (gMain.newAndRepeatedKeys & DPAD_UP)
    {
        if (*p1 != 0)
        {
            sub_80925B4(*p1, 0);
            (*p1)--;
            sub_80925B4(*p1, 1);
            r3 = TRUE;
        }
        else if (*p2 != 0)
        {
            (*p2)--;
            sub_8092D78(taskId);
            sub_80925B4(*p1, 1);
            r3 = TRUE;
        }
        if (r3)
        {
            PlaySE(SE_SELECT);
            sub_8091E20(r8[*p1 + *p2].text1);
        }
        return;
    }
    if (gMain.newAndRepeatedKeys & DPAD_DOWN)
    {
        if (*p1 < 5 && *p1 < numOptions) // scroll down w/o scrolling text
        {
            sub_80925B4(*p1, 0);
            (*p1)++;
            sub_80925B4(*p1, 1);
            r3 = TRUE;
        }
        else if (numOptions > 5 && *p2 < numOptions - 5) // scroll down
        {
            (*p2)++;
            sub_8092D78(taskId);
            sub_80925B4(5, 1);
            r3 = TRUE;
        }
        if (r3)
        {
            PlaySE(SE_SELECT);
            sub_8091E20(r8[*p1 + *p2].text1);
        }
        return;
    }
}

static void sub_80927B8(u8 taskId)
{
    BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB(0, 0, 0));
    gTasks[taskId].func = sub_80927F0;
}

static void sub_80927F0(u8 taskId)
{
    if (!gPaletteFade.active)
        DestroyTask(taskId);
}

#ifdef NONMATCHING
void sub_8092810(u8 a, u8 b, u8 c, u8 d)
{
    u16 i;

    for (i = 0; i < d; i++)
    {
        ((u16 *)VRAM)[15 * 0x400 + c * 32 + i + b] &= 0xFFF;
        ((u16 *)VRAM)[15 * 0x400 + c * 32 + i + b] |= a << 12;

        ((u16 *)VRAM)[15 * 0x400 + (c + 1) * 32 + i + b] &= 0xFFF;
        ((u16 *)VRAM)[15 * 0x400 + (c + 1) * 32 + i + b] |= a << 12;
    }
}
#else
NAKED
void sub_8092810(u8 a, u8 b, u8 c, u8 d)
{
    asm(".syntax unified\n\
    push {r4-r7,lr}\n\
    lsls r0, 24\n\
    lsrs r0, 24\n\
    lsls r1, 24\n\
    lsrs r1, 24\n\
    mov r12, r1\n\
    lsls r2, 24\n\
    lsrs r1, r2, 24\n\
    lsls r3, 24\n\
    lsrs r5, r3, 8\n\
    movs r3, 0\n\
    cmp r5, 0\n\
    beq _0809285A\n\
    lsls r7, r1, 6\n\
    ldr r6, _08092860 @ =0x00000fff\n\
    lsls r4, r0, 12\n\
_08092830:\n\
    mov r0, r12\n\
    adds r1, r0, r3\n\
    lsls r1, 1\n\
    adds r1, r7, r1\n\
    ldr r0, _08092864 @ =0x06007800\n\
    adds r2, r1, r0\n\
    ldrh r0, [r2]\n\
    ands r0, r6\n\
    orrs r0, r4\n\
    strh r0, [r2]\n\
    ldr r0, _08092868 @ =0x06007840\n\
    adds r1, r0\n\
    ldrh r0, [r1]\n\
    ands r0, r6\n\
    orrs r0, r4\n\
    strh r0, [r1]\n\
    adds r0, r3, 0x1\n\
    lsls r0, 16\n\
    lsrs r3, r0, 16\n\
    cmp r0, r5\n\
    bcc _08092830\n\
_0809285A:\n\
    pop {r4-r7}\n\
    pop {r0}\n\
    bx r0\n\
    .align 2, 0\n\
_08092860: .4byte 0x00000fff\n\
_08092864: .4byte 0x06007800\n\
_08092868: .4byte 0x06007840\n\
    .syntax divided\n");
}
#endif

static void sub_809286C(u8 a, u8 b, u8 c)
{
    u8 r5 = (b & 1) | ((c & 1) << 1);

    switch (a)
    {
    case 0:
    case 1:
    case 2:
        sub_8092810(r5, gUnknown_083B57E4[a].unk4, gUnknown_083B57E4[a].unk5, gUnknown_083B57E4[a].unk6);
        break;
    case 3:
    case 4:
    case 7:
    case 8:
        sub_8092810(r5, gUnknown_083B57FC[a - 3].unk4, gUnknown_083B57FC[a - 3].unk5, gUnknown_083B57FC[a - 3].unk6);
        // fall through
    case 5:
    case 6:
        sub_8092810(r5, gUnknown_083B57FC[a - 3].unk7, gUnknown_083B57FC[a - 3].unk8, gUnknown_083B57FC[a - 3].unk9);
        break;
    case 10:
        sub_8092810(r5, gUnknown_083B57FC[2].unk4, gUnknown_083B57FC[2].unk5, gUnknown_083B57FC[2].unk6);
        break;
    case 9:
        if (!IsNationalPokedexEnabled())
            sub_8092810(r5, gUnknown_083B57FC[a - 3].unk4, gUnknown_083B57FC[a - 3].unk5 - 2, gUnknown_083B57FC[a - 3].unk6);
        else
            sub_8092810(r5, gUnknown_083B57FC[a - 3].unk4, gUnknown_083B57FC[a - 3].unk5, gUnknown_083B57FC[a - 3].unk6);
        break;
    }
}

static void sub_8092964(u8 a)
{
    switch (a)
    {
    case 0:
        sub_809286C(0, 0, 0);
        sub_809286C(1, 1, 0);
        sub_809286C(2, 1, 0);
        sub_809286C(3, 1, 0);
        sub_809286C(4, 1, 0);
        sub_809286C(10, 1, 0);
        sub_809286C(5, 1, 0);
        sub_809286C(6, 1, 0);
        sub_809286C(7, 1, 0);
        sub_809286C(8, 1, 0);
        sub_809286C(9, 1, 0);
        break;
    case 1:
        sub_809286C(0, 1, 0);
        sub_809286C(1, 0, 0);
        sub_809286C(2, 1, 0);
        sub_809286C(3, 1, 1);
        sub_809286C(4, 1, 1);
        sub_809286C(10, 1, 1);
        sub_809286C(5, 1, 1);
        sub_809286C(6, 1, 1);
        sub_809286C(7, 1, 0);
        sub_809286C(8, 1, 0);
        sub_809286C(9, 1, 0);
        break;
    case 2:
        sub_809286C(0, 1, 0);
        sub_809286C(1, 1, 0);
        sub_809286C(2, 0, 0);
        sub_809286C(3, 1, 1);
        sub_809286C(4, 1, 1);
        sub_809286C(10, 1, 1);
        sub_809286C(5, 1, 1);
        sub_809286C(6, 1, 1);
        sub_809286C(7, 1, 1);
        sub_809286C(8, 1, 1);
        sub_809286C(9, 1, 1);
        break;
    }
}

static void sub_8092AB0(u8 a)
{
    sub_8092964(a);
    sub_8091E20(gUnknown_083B57E4[a].text);
}

static void sub_8092AD4(u8 a, u8 b)
{
    sub_8092964(a);
    switch (b)
    {
    case 0:
        sub_809286C(3, 0, 0);
        break;
    case 1:
        sub_809286C(4, 0, 0);
        break;
    case 2:
        sub_809286C(10, 0, 0);
        sub_809286C(5, 0, 0);
        break;
    case 3:
        sub_809286C(10, 0, 0);
        sub_809286C(6, 0, 0);
        break;
    case 4:
        sub_809286C(7, 0, 0);
        break;
    case 5:
        sub_809286C(8, 0, 0);
        break;
    case 6:
        sub_809286C(9, 0, 0);
        break;
    }
    sub_8091E20(gUnknown_083B57FC[b].text);
}

static void sub_8092B68(u8 taskId)
{
    u16 var;

    var = gTasks[taskId].data[6] + gTasks[taskId].data[7];
    StringCopy(gStringVar1, gUnknown_083B5910[var].text2);
    Menu_PrintTextPixelCoords(gUnknown_083B5AB2, 45, 16, 1);

    var = gTasks[taskId].data[8] + gTasks[taskId].data[9];
    StringCopy(gStringVar1, gUnknown_083B5968[var].text2);
    Menu_PrintTextPixelCoords(gUnknown_083B5AB2, 45, 32, 1);

    var = gTasks[taskId].data[10] + gTasks[taskId].data[11];
    StringCopy(gStringVar1, gUnknown_083B59C8[var].text2);
    Menu_PrintTextPixelCoords(gUnknown_083B5AAC, 45, 48, 1);

    var = gTasks[taskId].data[12] + gTasks[taskId].data[13];
    StringCopy(gStringVar1, gUnknown_083B59C8[var].text2);
    Menu_PrintTextPixelCoords(gUnknown_083B5AAC, 93, 48, 1);

    var = gTasks[taskId].data[4] + gTasks[taskId].data[5];
    StringCopy(gStringVar1, gUnknown_083B58D8[var].text2);
    Menu_PrintTextPixelCoords(gUnknown_083B5AB2, 45, 64, 1);

    if (IsNationalPokedexEnabled())
    {
        var = gTasks[taskId].data[2] + gTasks[taskId].data[3];
        StringCopy(gStringVar1, gUnknown_083B58C0[var].text2);
        Menu_PrintTextPixelCoords(gUnknown_083B5AB2, 45, 80, 1);
    }
}

static void sub_8092C8C(u8 a)
{
    u16 i;
    u16 j;

    if (a == 0)
    {
        *((u16 *)(VRAM + 0x7800 + 0x22)) = 0xC0B;
        for (i = 0x12; i < 0x1D; i++)
            *((u16 *)(VRAM + 0x7800 + i * 2)) = 0x80D;
        *((u16 *)(VRAM + 0x7800 + 0x3A)) = 0x80B;
        for (j = 1; j < 13; j++)
        {
            *((u16 *)(VRAM + 0x7800 + 0x22 + j * 64)) = 0x40A;
            for (i = 0x12; i < 0x1D; i++)
                *((u16 *)(VRAM + 0x7800 + j * 64 + i * 2)) = 2;
            *((u16 *)(VRAM + 0x7800 + 0x3A + j * 64)) = 0xA;
        }
        *((u16 *)(VRAM + 0x7800 + 0x362)) = 0x40B;
        for (i = 0x12; i < 0x1D; i++)
            *((u16 *)(VRAM + 0x7800 + 0x340 + i * 2)) = 0xD;
        *((u16 *)(VRAM + 0x7800 + 0x37A)) = 0xB;
    }
    else
    {
        for (j = 0; j < 14; j++)
        {
            for (i = 0x11; i < 0x1E; i++)
            {
                *((u16 *)(VRAM + 0x7800 + j * 64 + i * 2)) = 0x4F;
            }
        }
    }
}

static void sub_8092D78(u8 taskId)
{
    const struct UnknownStruct2 *r6 = gUnknown_083B5A7C[gTasks[taskId].data[1]].unk0;
    const u16 *r8 = &gTasks[taskId].data[gUnknown_083B5A7C[gTasks[taskId].data[1]].unk4];
    const u16 *r7 = &gTasks[taskId].data[gUnknown_083B5A7C[gTasks[taskId].data[1]].unk5];
    u16 i;
    u16 j;

    Menu_EraseWindowRect(18, 1, 28, 12);
    for (i = 0, j = *r7; i < 6 && r6[j].text2 != NULL; i++, j++)
    {
        Menu_PrintText(r6[j].text2, 18, i * 2 + 1);
    }
    sub_8091E20(r6[*r8 + *r7].text1);
}

static u8 sub_8092E10(u8 taskId, u8 b)
{
    const u16 *ptr1 = &gTasks[taskId].data[gUnknown_083B5A7C[b].unk4];
    const u16 *ptr2 = &gTasks[taskId].data[gUnknown_083B5A7C[b].unk5];
    u16 r2 = *ptr1 + *ptr2;

    switch (b)
    {
    default:
        return 0;
    case 5:
        return gUnknown_083B5A60[r2];
    case 4:
        return gUnknown_083B5A62[r2];
    case 0:
        if (r2 == 0)
            return 0xFF;
        else
            return r2;
    case 1:
        if (r2 == 0)
            return 0xFF;
        else
            return r2 - 1;
    case 2: // type 1
    case 3: // type 2
        return gUnknown_083B5A68[r2];
    }
}

static void sub_8092EB0(u8 taskId)
{
    u16 r3;

    switch (gPokedexView->unk614)
    {
    default:
    case 0:
        r3 = 0;
        break;
    case 1:
        r3 = 1;
        break;
    }
    gTasks[taskId].data[2] = r3;

    switch (gPokedexView->unk618)
    {
    default:
    case 0:
        r3 = 0;
        break;
    case 1:
        r3 = 1;
        break;
    case 2:
        r3 = 2;
        break;
    case 3:
        r3 = 3;
        break;
    case 4:
        r3 = 4;
        break;
    case 5:
        r3 = 5;
        break;
    }
    gTasks[taskId].data[4] = r3;
}

static bool8 sub_8092F44(u8 taskId)
{
    u8 val1 = gTasks[taskId].data[1];
    const u16 *ptr = &gTasks[taskId].data[gUnknown_083B5A7C[val1].unk5];
    u16 val2 = gUnknown_083B5A7C[val1].unk6 - 1;

    if (val2 > 5 && *ptr != 0)
        return FALSE;
    else
        return TRUE;
}

static bool8 sub_8092F8C(u8 taskId)
{
    u8 val1 = gTasks[taskId].data[1];
    const u16 *ptr = &gTasks[taskId].data[gUnknown_083B5A7C[val1].unk5];
    u16 val2 = gUnknown_083B5A7C[val1].unk6 - 1;

    if (val2 > 5 && *ptr < val2 - 5)
        return FALSE;
    else
        return TRUE;
}

static void sub_8092FD8(struct Sprite *sprite)
{
    if (gTasks[sprite->data[0]].func == sub_8092644)
    {
        u8 val;

        if (sprite->data[1] != 0)
        {
            if (sub_8092F8C(sprite->data[0]))
                sprite->invisible = TRUE;
            else
                sprite->invisible = FALSE;
        }
        else
        {
            if (sub_8092F44(sprite->data[0]))
                sprite->invisible = TRUE;
            else
                sprite->invisible = FALSE;
        }
        val = sprite->data[2] + sprite->data[1] * 128;
        sprite->pos2.y = gSineTable[val] / 128;
        sprite->data[2] += 8;
    }
    else
    {
        sprite->invisible = TRUE;
    }
}

static void sub_809308C(u8 taskId)
{
    u8 spriteId;

    spriteId = CreateSprite(&gSpriteTemplate_RedArrow, 184, 4, 0);
    gSprites[spriteId].data[0] = taskId;
    gSprites[spriteId].data[1] = 0;
    gSprites[spriteId].callback = sub_8092FD8;

    spriteId = CreateSprite(&gSpriteTemplate_RedArrow, 184, 108, 0);
    gSprites[spriteId].data[0] = taskId;
    gSprites[spriteId].data[1] = 1;
    gSprites[spriteId].vFlip = TRUE;
    gSprites[spriteId].callback = sub_8092FD8;
}
