#include "global.h"
#include "rtc.h"
#include "string_util.h"
#include "text.h"

static u16 sErrorStatus;
static struct SiiRtcInfo sRtc;
static u8 sProbeResult;
static u16 sSavedIme;

struct Time gLocalTime;

static const struct SiiRtcInfo sRtcDummy = {0, MONTH_JAN, 1}; // 2000 Jan 1

static const s32 sNumDaysInMonths[12] =
{
    31,
    28,
    31,
    30,
    31,
    30,
    31,
    31,
    30,
    31,
    30,
    31,
};

void RtcDisableInterrupts()
{
    sSavedIme = REG_IME;
    REG_IME = 0;
}

void RtcRestoreInterrupts()
{
    REG_IME = sSavedIme;
}

u32 ConvertBcdToBinary(u8 bcd)
{
    if (bcd > 0x9F)
        return 0xFF;

    if ((bcd & 0xF) <= 9)
        return (10 * ((bcd >> 4) & 0xF)) + (bcd & 0xF);
    else
        return 0xFF;
}

bool8 IsLeapYear(u8 year)
{
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
        return TRUE;

    return FALSE;
}

static u8 GetMonthBasedOnDays(u8 year, u16 days) 
{
    u8 i;
    u8 month;

    for (i = 0; i < 12; i++) 
    {
        if (days > sNumDaysInMonths[i]) // if day isn't in this month
        {
            days -= sNumDaysInMonths[i];
            month++;
            if (IsLeapYear(year) && month == MONTH_FEB)
                days--;
        }
        else
            return month;
    }

    return 0xFF;
}

u16 ConvertDateToDayCount(u8 year, u8 month, u8 day)
{
    s32 i;
    u16 dayCount = 0;

    // The fix was to use "i >= 0" as the condition instead of "i > 0".
    for (i = year - 1; i >= 0; i--)
    {
        dayCount += 365;

        if (IsLeapYear(i) == TRUE)
            dayCount++;
    }

    for (i = 0; i < month - 1; i++)
        dayCount += sNumDaysInMonths[i];

    if (month > MONTH_FEB && IsLeapYear(year) == TRUE)
        dayCount++;

    dayCount += day;

    return dayCount;
}

u16 RtcGetDayCount(struct SiiRtcInfo *rtc)
{
    
    u8 year = gSaveBlock2.playTimeYears; //ConvertBcdToBinary(rtc->year);
    u8 month = GetMonthBasedOnDays(gSaveBlock2.playTimeYears, gSaveBlock2.playTimeDays); //ConvertBcdToBinary(rtc->month);
    u8 day = gSaveBlock2.playTimeDays; //ConvertBcdToBinary(rtc->day);
    return ConvertDateToDayCount(year, month, day);
}

void RtcInit()
{
    sErrorStatus = 0;

    RtcDisableInterrupts();
    SiiRtcUnprotect();
    sProbeResult = SiiRtcProbe();
    RtcRestoreInterrupts();

    if (!(sProbeResult & 0xF))
    {
        sErrorStatus = RTC_INIT_ERROR;
        return;
    }

    if (sProbeResult & 0xF0)
        sErrorStatus = RTC_INIT_WARNING;
    else
        sErrorStatus = 0;

    RtcGetRawInfo(&sRtc);
    sErrorStatus = RtcCheckInfo(&sRtc);
}

u16 RtcGetErrorStatus()
{
    return sErrorStatus;
}

void RtcGetInfo(struct SiiRtcInfo *rtc)
{
    if (sErrorStatus & RTC_ERR_FLAG_MASK)
        *rtc = sRtcDummy;
    else
        RtcGetRawInfo(rtc);
}

void RtcGetDateTime(struct SiiRtcInfo *rtc)
{
    RtcDisableInterrupts();
    SiiRtcGetDateTime(rtc);
    RtcRestoreInterrupts();
}

void RtcGetStatus(struct SiiRtcInfo *rtc)
{
    RtcDisableInterrupts();
    SiiRtcGetStatus(rtc);
    RtcRestoreInterrupts();
}

void RtcGetRawInfo(struct SiiRtcInfo *rtc)
{
    RtcGetStatus(rtc);
    RtcGetDateTime(rtc);
}

u16 RtcCheckInfo(struct SiiRtcInfo *rtc)
{
    u16 errorFlags = 0;
    s32 year;
    s32 month;
    s32 value;

    if (rtc->status & SIIRTCINFO_POWER)
        errorFlags |= RTC_ERR_POWER_FAILURE;

    if (!(rtc->status & SIIRTCINFO_24HOUR))
        errorFlags |= RTC_ERR_12HOUR_CLOCK;

    year = gSaveBlock2.playTimeYears; //ConvertBcdToBinary(rtc->year);

    if (year == 0xFF)
        errorFlags |= RTC_ERR_INVALID_YEAR;

    month = GetMonthBasedOnDays(gSaveBlock2.playTimeYears, gSaveBlock2.playTimeDays); //ConvertBcdToBinary(rtc->month);

    if (month == 0xFF || month == 0 || month > 12)
        errorFlags |= RTC_ERR_INVALID_MONTH;

    value = gSaveBlock2.playTimeDays;

    if (value == 0xFF)
        errorFlags |= RTC_ERR_INVALID_DAY;

    value = gSaveBlock2.playTimeHours; //ConvertBcdToBinary(rtc->hour);

    if (value > 24)
        errorFlags |= RTC_ERR_INVALID_HOUR;

    value = gSaveBlock2.playTimeMinutes; //ConvertBcdToBinary(rtc->minute);

    if (value > 60)
        errorFlags |= RTC_ERR_INVALID_MINUTE;

    value = gSaveBlock2.playTimeSeconds; //ConvertBcdToBinary(rtc->second);

    if (value > 60)
        errorFlags |= RTC_ERR_INVALID_SECOND;

    return errorFlags;
}

void RtcReset()
{
    RtcDisableInterrupts();
    SiiRtcReset();
    RtcRestoreInterrupts();
}

void FormatDecimalTime(u8 *dest, s32 hour, s32 minute, s32 second)
{
    dest = ConvertIntToDecimalStringN(dest, hour, STR_CONV_MODE_LEADING_ZEROS, 2);
    *dest++ = CHAR_COLON;
    dest = ConvertIntToDecimalStringN(dest, minute, STR_CONV_MODE_LEADING_ZEROS, 2);
    *dest++ = CHAR_COLON;
    dest = ConvertIntToDecimalStringN(dest, second, STR_CONV_MODE_LEADING_ZEROS, 2);
    *dest = EOS;
}

void FormatHexTime(u8 *dest, s32 hour, s32 minute, s32 second)
{
    dest = ConvertIntToHexStringN(dest, hour, STR_CONV_MODE_LEADING_ZEROS, 2);
    *dest++ = CHAR_COLON;
    dest = ConvertIntToHexStringN(dest, minute, STR_CONV_MODE_LEADING_ZEROS, 2);
    *dest++ = CHAR_COLON;
    dest = ConvertIntToHexStringN(dest, second, STR_CONV_MODE_LEADING_ZEROS, 2);
    *dest = EOS;
}

void FormatHexRtcTime(u8 *dest)
{
    FormatHexTime(dest, sRtc.hour, sRtc.minute, sRtc.second);
}

void FormatDecimalDate(u8 *dest, s32 year, s32 month, s32 day)
{
    dest = ConvertIntToDecimalStringN(dest, year, STR_CONV_MODE_LEADING_ZEROS, 4);
    *dest++ = CHAR_HYPHEN;
    dest = ConvertIntToDecimalStringN(dest, month, STR_CONV_MODE_LEADING_ZEROS, 2);
    *dest++ = CHAR_HYPHEN;
    dest = ConvertIntToDecimalStringN(dest, day, STR_CONV_MODE_LEADING_ZEROS, 2);
    *dest = EOS;
}

void FormatHexDate(u8 *dest, s32 year, s32 month, s32 day)
{
    dest = ConvertIntToHexStringN(dest, year, STR_CONV_MODE_LEADING_ZEROS, 4);
    *dest++ = CHAR_HYPHEN;
    dest = ConvertIntToHexStringN(dest, month, STR_CONV_MODE_LEADING_ZEROS, 2);
    *dest++ = CHAR_HYPHEN;
    dest = ConvertIntToHexStringN(dest, day, STR_CONV_MODE_LEADING_ZEROS, 2);
    *dest = EOS;
}

#if DEBUG

void debug_sub_800987C(u8 *dest)
{
    FormatHexDate(dest, sRtc.year, sRtc.month, sRtc.day);
}

void debug_sub_8009894(u8 *dest)
{
    u16 dayCount;

    dayCount = RtcGetDayCount(&sRtc);
    ConvertIntToDecimalStringN(dest, dayCount, STR_CONV_MODE_RIGHT_ALIGN, 4);
}

void debug_sub_80098B8(u8 *dest)
{
    ConvertIntToHexStringN(dest, sRtc.status, STR_CONV_MODE_LEADING_ZEROS, 2);
}

#endif

void RtcCalcTimeDifference(struct SiiRtcInfo *rtc, struct Time *result, struct Time *t)
{
    u16 days = RtcGetDayCount(rtc);
    result->seconds = ConvertBcdToBinary(rtc->second) - t->seconds;
    result->minutes = ConvertBcdToBinary(rtc->minute) - t->minutes;
    result->hours = ConvertBcdToBinary(rtc->hour) - t->hours;
    result->days = days - t->days;

    if (result->seconds < 0)
    {
        result->seconds += 60;
        --result->minutes;
    }

    if (result->minutes < 0)
    {
        result->minutes += 60;
        --result->hours;
    }

    if (result->hours < 0)
    {
        result->hours += 24;
        --result->days;
    }
}

void RtcCalcLocalTime()
{
    //RtcGetInfo(&sRtc);
    //RtcCalcTimeDifference(&sRtc, &gLocalTime, &gSaveBlock2.localTimeOffset);
    gLocalTime.days = gSaveBlock2.playTimeDays;
    gLocalTime.hours = gSaveBlock2.playTimeHours;
    gLocalTime.minutes = gSaveBlock2.playTimeMinutes;
    gLocalTime.seconds = gSaveBlock2.playTimeSeconds;
}

void RtcInitLocalTimeOffset(s32 hour, s32 minute)
{
    RtcCalcLocalTimeOffset(0, hour, minute, 0);
}

void RtcCalcLocalTimeOffset(s32 days, s32 hours, s32 minutes, s32 seconds)
{
    gLocalTime.days = days;
    gLocalTime.hours = hours;
    gLocalTime.minutes = minutes;
    gLocalTime.seconds = seconds;
    RtcGetInfo(&sRtc);
    RtcCalcTimeDifference(&sRtc, &gSaveBlock2.localTimeOffset, &gLocalTime);
}

void CalcTimeDifference(struct Time *result, struct Time *t1, struct Time *t2)
{
    result->seconds = t2->seconds - t1->seconds;
    result->minutes = t2->minutes - t1->minutes;
    result->hours = t2->hours - t1->hours;
    result->days = t2->days - t1->days;

    if (result->seconds < 0)
    {
        result->seconds += 60;
        --result->minutes;
    }

    if (result->minutes < 0)
    {
        result->minutes += 60;
        --result->hours;
    }

    if (result->hours < 0)
    {
        result->hours += 24;
        --result->days;
    }
}

u32 RtcGetMinuteCount()
{
    RtcGetInfo(&sRtc);
    return (24 * 60) * RtcGetDayCount(&sRtc) + 60 * sRtc.hour + sRtc.minute;
}

#if DEBUG
void debug_sub_8009A60()
{
    RtcGetRawInfo(&sRtc);
}
#endif
