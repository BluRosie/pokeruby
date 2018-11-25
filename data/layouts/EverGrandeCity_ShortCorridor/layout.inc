EverGrandeCity_ShortCorridor_MapBorder::
	.incbin "data/layouts/EverGrandeCity_ShortCorridor/border.bin"

EverGrandeCity_ShortCorridor_MapBlockdata::
	.incbin "data/layouts/EverGrandeCity_ShortCorridor/map.bin"

	.align 2
EverGrandeCity_ShortCorridor_Layout::
	.4byte 0xb
	.4byte 0xd
	.4byte EverGrandeCity_ShortCorridor_MapBorder
	.4byte EverGrandeCity_ShortCorridor_MapBlockdata
	.4byte gTileset_Building
	.4byte gTileset_EliteFour

