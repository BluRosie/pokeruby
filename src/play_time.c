#include "global.h"
#include "play_time.h"
#include "clock.h"
#include "berry.h"
#include "dewford_trend.h"
#include "event_data.h"
#include "field_specials.h"
#include "field_weather.h"
#include "lottery_corner.h"
#include "main.h"
#include "overworld.h"
#include "rtc.h"
#include "time_events.h"
#include "tv.h"
#include "wallclock.h"

enum
{
    STOPPED,
    RUNNING,
    MAXED_OUT
};

static u8 sPlayTimeCounterState;

void PlayTimeCounter_Reset()
{
    sPlayTimeCounterState = STOPPED;

    gSaveBlock2.playTimeHours = 0;
    gSaveBlock2.playTimeMinutes = 0;
    gSaveBlock2.playTimeSeconds = 0;
    gSaveBlock2.playTimeVBlanks = 0;
}

void PlayTimeCounter_Start()
{
    sPlayTimeCounterState = RUNNING;

    if (gSaveBlock2.playTimeHours > 999)
        PlayTimeCounter_SetToMax();
}

void PlayTimeCounter_Stop()
{
    sPlayTimeCounterState = STOPPED;
}

static void DailyUpdates()
{
    ClearDailyFlags();
    UpdateDewfordTrendPerDay(1);
    UpdateTVShowsPerDay(1);
    UpdateWeatherPerDay(1);
    UpdatePartyPokerusTime(1);
    UpdateMirageRnd(1);
    UpdateBirchState(1);
    SetShoalItemFlag(1);
    SetRandomLotteryNumber(1);
}

void PlayTimeCounter_Update()
{
    if (sPlayTimeCounterState == RUNNING)
    {
        gSaveBlock2.playTimeVBlanks++;

        if (gSaveBlock2.playTimeVBlanks > VBLANKS_PER_SECOND - 1)
        {
            gSaveBlock2.playTimeVBlanks = 0;
            gSaveBlock2.playTimeSeconds++;

            if (gSaveBlock2.playTimeSeconds > SECONDS_PER_MINUTE - 1)
            {
                gSaveBlock2.playTimeSeconds = 0;
                gSaveBlock2.playTimeMinutes++;

                if (gSaveBlock2.playTimeMinutes > MINUTES_PER_HOUR - 1)
                {
                    gSaveBlock2.playTimeMinutes = 0;
                    gSaveBlock2.playTimeHours++;
                    BerryTreeTimeUpdate(1);

                    if (gSaveBlock2.playTimeHours > HOURS_PER_DAY - 1)
                    {
                        gSaveBlock2.playTimeHours = 0;
                        gSaveBlock2.playTimeDays++;
                        DailyUpdates();

                        if (gSaveBlock2.playTimeDays > DAYS_PER_YEAR - 1)
                        {
                            gSaveBlock2.playTimeDays = 0;

                            if (gSaveBlock2.playTimeYears < 255)
                                gSaveBlock2.playTimeYears++;
                            else
                                sPlayTimeCounterState = MAXED_OUT;
                        }
                    }
                }
            }
        }
    }
}

void PlayTimeCounter_SetToMax() 
{
    sPlayTimeCounterState = MAXED_OUT;

    gSaveBlock2.playTimeHours = 999;
    gSaveBlock2.playTimeMinutes = 59;
    gSaveBlock2.playTimeSeconds = 59;
    gSaveBlock2.playTimeVBlanks = 59;
}
