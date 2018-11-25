CaveOfOrigin_B3F_MapBorder::
	.incbin "data/layouts/CaveOfOrigin_B3F/border.bin"

CaveOfOrigin_B3F_MapBlockdata::
	.incbin "data/layouts/CaveOfOrigin_B3F/map.bin"

	.align 2
CaveOfOrigin_B3F_Layout::
	.4byte 0x13
	.4byte 0x15
	.4byte CaveOfOrigin_B3F_MapBorder
	.4byte CaveOfOrigin_B3F_MapBlockdata
	.4byte gTileset_General
	.4byte gTileset_Cave

