/// =====================================================================================
///
///       Filename:  etime.h
///
///    Description:  a compat time impletation for different platforms
///
///        Version:  1.0
///        Created:  06/11/2019 11:00:34 AM
///       Revision:  none
///       Compiler:  gcc
///
///         Author:  Haitao Yang, joyhaitao@foxmail.com
///        Company:
///
/// =====================================================================================

#include <time.h>

#include "etype.h"

typedef struct etm_s {
        int     tm_sec;         /* seconds after the minute [0-60] */
        int     tm_min;         /* minutes after the hour [0-59] */
        int     tm_hour;        /* hours since midnight [0-23] */
        int     tm_mday;        /* day of the month [1-31] */
        int     tm_mon;         /* months since January [0-11] */
        int     tm_year;        /* years since 1900 */
        int     tm_wday;        /* days since Sunday [0-6] */
        int     tm_yday;        /* days since January 1 [0-365] */
        int     tm_isdst;       /* Daylight Savings Time flag */
        long    tm_gmtoff;      /* offset from UTC in seconds */
        char    *tm_zone;       /* timezone abbreviation */
        long    tm_nsec;        /* nanoseconds */
}etm;

i64 e_nowns();
i64 e_nowms();
i64 e_nows ();

//! get a str of time in format like this:
//!     20130603150702.352432467
//!
//!  the output len is limited by dlen - 1
//!
cstr e_nowstr(cstr desc, int dlen);             // YYYYMMDDhhmmss.[9]ns
cstr e_secstr(cstr desc, int dlen, time_t sec); // YYYYMMDDhhmmss

/**
 *
 *  etimer_strfstr()
 *      input                  output [%F %T]
 *      17:43:03               1900-01-00 17:43:03
 *      17:43:03 2018          2018-01-00 17:43:03
 *      Jun 17:43:03 2018      2018-06-00 17:43:03
 *      Jun 27 17:43:03 2018   2018-06-27 17:43:03
 *
 *
 *  etimer_strfsec()
 *      input                  output [%F %T]
 *      31723506(1y2d4h5m6s)   1971-01-03 12:05:06
 *      1530092583             2018-06-27 17:43:03
 *      1                      1970-01-01 08:00:01
 *
 *  etimer_strpsec()
 *      input                  output
 *      Jun 27 17:43:03 2018   1530092583
 */
cstr e_strfstr(cstr dest, int dlen, constr dfmt, constr from, constr ffmt); // format to time str from another formated time str
cstr e_strfsec(cstr dest, int dlen, constr dfmt, time_t sec );              // format to time str from a timestamp(sec)
i64  e_strpsec(constr from, constr ffmt);                                   // parse a time str to timestamp(sec)
i64  e_strpms (constr from, constr ffmt);                                   // parse a time str to timestamp(ms)
/** ---------------------------
%a 星期几的简写
%A 星期几的全称
%b 月分的简写
%B 月份的全称
%c 标准的日期的时间串
%C 年份的后两位数字
%d 十进制表示的每月的第几天
%D 月/天/年
%e 在两字符域中，十进制表示的每月的第几天
%F 年-月-日
%g 年份的后两位数字，使用基于周的年
%G 年分，使用基于周的年
%h 简写的月份名
%H 24小时制的小时
%I 12小时制的小时
%j 十进制表示的每年的第几天
%m 十进制表示的月份
%M 十时制表示的分钟数
%n 新行符
%p 本地的AM或PM的等价显示
%r 12小时的时间
%R 显示小时和分钟：hh:mm
%S 十进制的秒数
%t 水平制表符
%T 显示时分秒：hh:mm:ss
%u 每周的第几天，星期一为第一天 （值从0到6，星期一为0）
%U 第年的第几周，把星期日做为第一天（值从0到53）
%V 每年的第几周，使用基于周的年
%w 十进制表示的星期几（值从0到6，星期天为0）
%W 每年的第几周，把星期一做为第一天（值从0到53）
%x 标准的日期串
%X 标准的时间串
%y 不带世纪的十进制年份（值从0到99）
%Y 带世纪部分的十进制年份
%z，%Z 时区名称，如果不能得到时区名称则返回空字符。
%% 百分号
--------------------------- */

/**
 *   etimer_elapsefstr
 *      input                       output
 *      367 04:05:06 1 [%j %T %Y]   2y 002d 04:05:06 [%Yy %jd %T]
 *      367 04:05:06 1 [%j %T %Y]   732d 04:05:06    [%jd %T]
 *      367 04:05:06 1 [%j %T %Y]   2y 52:05:06      [%Yy %T]
 *      367 04:05:06 1 [%j %T %Y]   17572:05:06      [%T]
 *
 *  etimer_elapsefsec
 *      input                       output
 *      31723506(1y2d4h5m6s)        1y 002d 04:05:06 [%Yy %jd %T]
 *      31723506                    367d 04:05:06    [%jd %T]
 *      31723506                    1y 52:05:06      [%Yy %T]
 *      31723506                    8812:05:06       [%T]
 *      1                           0y 000d 00:00:01 [%Yy %jd %T]
 *
 *  etimer_elapsepsec
 *      input                       output
 *   1  2 04:05:06 1 [%j %T %Y]     31723506   use %j as days, not yday likely in tm, this case is specific if only using %Y %j %T in ffmt
 *   2  Jan 0 00:00:03 0000         -1
 *   3  Jan 1 00:00:03 0000         3
 *   4  Jan 2 02:00:03 0000         93603
 */
cstr e_elapsefstr(cstr dest, int dlen, constr dfmt, constr from, constr ffmt);  // format to elapse str from a time str
cstr e_elapsefsec(cstr dest, int dlen, constr dfmt, i64 sec);                   // format to elapse str from a sec timestamp
i64  e_elapsepsec(constr from, constr ffmt);                                    // parse a elapse str to passed seconds
i64  e_elapsepms (constr from, constr ffmt);                                    // parse a elapse str to passed ms
