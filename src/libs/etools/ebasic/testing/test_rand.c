﻿/**
* this file is create by subsystem 'ETest' from EMake framework
*/

#include <etest.h>

#include <math.h>

#include "eutils.h"

static int rand_verify()
{
    e_srand(0);

    long arr[100] = {38,7719,21238,2437,8855,11797,8365,32285,10450,30612,5853,28100,1142,281,20537,15921,8945,26285,2997,14680,20976,31891,21655,25906,18457,1323,28881,2240,9725,32278,2446,590,840,18587,16907,21237,23611,12617,12456,867,29533,6878,28223,17887,31597,20584,12212,31111,7578,17066,7629,29404,12279,13505,24388,11649,12329,7176,2331,19264,22114,14136,26928,1102,21652,8404,24337,27856,5598,24772,14097,13213,4683,16703,15260,15942,2747,27375,28871,18004,16673,3152,11819,23504,239,4186,2804,28937,3023,10335,20533,21393,16020,11574,25983,13961,624,7065,27569,12830};

    for(int i = 0; i < 100; i++)
    {
        eexpect_num(e_rand(), arr[i]);
    }

    return ETEST_OK;
}

static int lrand48_verify()
{
    e_srand48(0);

    long arr[100] = {366850414,1610402240,206956554,1869309841,1239749840,1687491058,1486475625,791919534,1876694714,1600079540,957876296,759625531,1573073931,558822492,846739434,1668143718,1814699139,1236495745,1536607432,178250261,978877010,236108985,1170868265,838993021,1221027898,2059579517,1863412570,350446804,591650915,559121193,1984478347,936136734,1695354299,274055456,176535366,2020013348,54921735,331165453,820730508,332294532,1136734859,1883017796,924731022,566734305,673439058,1653759302,230620165,1655800634,1514395931,469525091,1635940142,884147033,1393344992,1997065850,1078935654,1476267618,936498180,1306316252,1238150237,1358544779,995199000,1357733199,296986106,2063219434,310042954,959244590,697039819,2045658729,769194090,855145646,217500455,2051030999,2114448794,1236886331,1859535587,321856025,1952385682,1398554113,137140141,2050845566,2075033485,1686941574,1729078280,1226757936,606848212,2067062644,1244417508,938146757,806243045,1983002741,61631449,1651114229,1552859968,1269097015,914588672,1378991719,1603660749,148216207,114450030,1662825892};

    for(int i = 0; i < 100; i++)
    {
        // printf("%d,", e_lrand48());
        eexpect_num(e_lrand48(), arr[i]);
    }

    return ETEST_OK;
}

static int mrand48_verify()
{
    e_srand48(0);

    long arr[100] = {733700828,-1074162815,413913109,-556347614,-1815467615,-919985179,-1322016045,1583839069,-541577867,-1094808216,1915752592,1519251063,-1148819433,1117644984,1693478868,-958679860,-665569017,-1821975806,-1221752431,356500522,1957754020,472217971,-1953230765,1677986043,-1852911499,-175808261,-568142155,700893609,1183301830,1118242387,-326010602,1872273469,-904258697,548110913,353070733,-254940599,109843471,662330906,1641461017,664589064,-2021497577,-528931704,1849462044,1133468611,1346878117,-987448691,461240330,-983366027,-1266175434,939050183,-1023087011,1768294066,-1508277311,-300835595,-2137095988,-1342432060,1872996360,-1682334791,-1818666821,-1577877738,1990398000,-1579500897,593972212,-168528427,620085908,1918489181,1394079639,-203649837,1538388181,1710291292,435000911,-192905297,-66069707,-1821194633,-575896122,643712051,-390195931,-1497859070,274280282,-193276164,-144900325,-921084147,-836810736,-1841451424,1213696425,-160842008,-1806132280,1876293515,1612486091,-328961814,123262898,-992738837,-1189247360,-1756773266,1829177344,-1536983858,-1087645797,296432414,228900061,-969315512};

    for(int i = 0; i < 100; i++)
    {
        // printf("%d,", e_mrand48());
        eexpect_num(e_mrand48(), arr[i]);
    }

    return ETEST_OK;
}

static int drand48_verify()
{
    e_srand48(0);

    f64 arr[100] = {0.17082803610628971569,0.74990198048496381489,0.09637165562356742043,0.87046522702707562757,0.57730350679510777923,0.78579925883967405298,0.69219415345864021560,0.36876626992042105257,0.87390407686180893165,0.74509509845006505202,0.44604590909311170321,0.35372820309337527078,0.73251963200253911168,0.26022200108288018328,0.39429377492387729376,0.77678995122561644848,0.84503513758028603320,0.57578820048277634669,0.71553859516866324952,0.08300424607387668630,0.45582512865975743921,0.10994681418064544687,0.54522802381657342607,0.39068657064867551298,0.56858542321444716094,0.95906644948837538323,0.86771909645913680720,0.16318951025235861607,0.27550892686831573997,0.26036109487206715585,0.92409474186916540361,0.43592263710268497334,0.78946086555209049607,0.12761701681174031364,0.08220568604043521077,0.94064201644784617429,0.02557492625301804878,0.15421093132782459634,0.38218242527815604603,0.15473669966669234555,0.52933341811698042534,0.87684849108325124689,0.43061143833839921058,0.26390622634207971942,0.31359449902321401282,0.77009168585472309587,0.10739088305410504631,0.77104225519564550950,0.70519555889444873742,0.21863965870775814437,0.76179399285599558311,0.41171304557895638254,0.64882682292911297850,0.92995625490736699703,0.50241856559867059673,0.68744067942886744049,0.43609094814732785039,0.60830090794974012169,0.57655863363536852262,0.63262171071406925194,0.46342564762874260964,0.63224378961008653732,0.13829493264932679608,0.96076141792519109686,0.14437500108784107056,0.44668307106778115667,0.32458445984537931395,0.95258407759246921387,0.35818391048117703690,0.39820822249224363532,0.10128154223238183818,0.95508573565287946394,0.98461694767107488246,0.57597008145148720359,0.86591373538525928666,0.14987589123277444969,0.90915043028165953842,0.65125250864004158302,0.06386085476492198154,0.95499938656245575430,0.96626276427565827021,0.78554338521396971373,0.80516481789500105037,0.57125367979575258914,0.28258571995424475176,0.96255105196796364453,0.57947705894630630041,0.43685862697056876414,0.37543617453727406996,0.92340760919302056209,0.02869938006311301137,0.76885997771550051993,0.72310677171758541704,0.59096934981296556089,0.42588853857983721696,0.64214305908301838599,0.74676272910062735377,0.06901854981410338041,0.05329494862236217045,0.77431364557134685356};

    for(int i = 0; i < 100; i++)
    {
        // printf("%.20f,", e_drand48());
        eexpect_num(e_drand48(), arr[i]);
    }

    return ETEST_OK;
}

int test_rand(int argc, char* argv[])
{
    (void)argc; (void)argv;

    ETEST_RUN( rand_verify() );
    ETEST_RUN( lrand48_verify() );
    ETEST_RUN( mrand48_verify() );
    ETEST_RUN( drand48_verify() );

    return ETEST_OK;
}
