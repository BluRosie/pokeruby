BattleTower_Outside_MapBorder::
	.incbin "data/layouts/BattleTower_Outside/border.bin"

BattleTower_Outside_MapBlockdata::
	.incbin "data/layouts/BattleTower_Outside/map.bin"

	.align 2
BattleTower_Outside_Layout::
	.4byte 0x1d
	.4byte 0x1e
	.4byte BattleTower_Outside_MapBorder
	.4byte BattleTower_Outside_MapBlockdata
	.4byte gTileset_General
	.4byte gTileset_Dewford

