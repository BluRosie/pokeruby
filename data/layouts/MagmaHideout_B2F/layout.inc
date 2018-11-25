MagmaHideout_B2F_MapBorder::
	.incbin "data/layouts/MagmaHideout_B2F/border.bin"

MagmaHideout_B2F_MapBlockdata::
	.incbin "data/layouts/MagmaHideout_B2F/map.bin"

	.align 2
MagmaHideout_B2F_Layout::
	.4byte 0x22
	.4byte 0x18
	.4byte MagmaHideout_B2F_MapBorder
	.4byte MagmaHideout_B2F_MapBlockdata
	.4byte gTileset_General
	.4byte gTileset_Facility

