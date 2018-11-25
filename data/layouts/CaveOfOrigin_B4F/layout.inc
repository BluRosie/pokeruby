CaveOfOrigin_B4F_MapBorder::
	.incbin "data/layouts/CaveOfOrigin_B4F/border.bin"

CaveOfOrigin_B4F_MapBlockdata::
	.incbin "data/layouts/CaveOfOrigin_B4F/map.bin"

	.align 2
CaveOfOrigin_B4F_Layout::
	.4byte 0x13
	.4byte 0x13
	.4byte CaveOfOrigin_B4F_MapBorder
	.4byte CaveOfOrigin_B4F_MapBlockdata
	.4byte gTileset_General
	.4byte gTileset_Cave

