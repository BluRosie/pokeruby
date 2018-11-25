EverGrandeCity_Corridor4_MapBorder::
	.incbin "data/layouts/EverGrandeCity_Corridor4/border.bin"

EverGrandeCity_Corridor4_MapBlockdata::
	.incbin "data/layouts/EverGrandeCity_Corridor4/map.bin"

	.align 2
EverGrandeCity_Corridor4_Layout::
	.4byte 0xb
	.4byte 0x22
	.4byte EverGrandeCity_Corridor4_MapBorder
	.4byte EverGrandeCity_Corridor4_MapBlockdata
	.4byte gTileset_Building
	.4byte gTileset_EliteFour

