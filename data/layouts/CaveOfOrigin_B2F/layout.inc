CaveOfOrigin_B2F_MapBorder::
	.incbin "data/layouts/CaveOfOrigin_B2F/border.bin"

CaveOfOrigin_B2F_MapBlockdata::
	.incbin "data/layouts/CaveOfOrigin_B2F/map.bin"

	.align 2
CaveOfOrigin_B2F_Layout::
	.4byte 0x15
	.4byte 0x15
	.4byte CaveOfOrigin_B2F_MapBorder
	.4byte CaveOfOrigin_B2F_MapBlockdata
	.4byte gTileset_General
	.4byte gTileset_Cave

