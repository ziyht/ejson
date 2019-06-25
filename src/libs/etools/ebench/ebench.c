/// =====================================================================================
///
///       Filename:  ebench.h
///
///    Description:  easy benchmark tools
///
///        Version:  1.0
///        Created:  06/18/2019 04:38:34 PM
///       Revision:  none
///       Compiler:  gcc
///        Depends:  ejson, estr
///
///         Author:  Haitao Yang, joyhaitao@foxmail.com
///        Company:
///
/// =====================================================================================

#include "stdio.h"

#include "ecompat.h"
#include "etype.h"
#include "ejson.h"
#include "eutils.h"
#include "etime.h"

#include "ebench.h"

#define EBENCH_VERSION "ebench 1.0.1"

#define TITLE " EBENCHMARK "

static char* t_title_map[] = {
    "OPRT" ,
    "SCALE",
    "COST" ,
    "TPO",
    "OPS"  ,
};

enum HDR{
    HDR_START__ = 0,
    HDR_NAME  = 0,
    HDR_SCALE,
    HDR_COST,
    HDR_TPO,
    HDR_OPS,
    HDR_END__
};

#define T_TITLE_CNT (sizeof(t_title_map) / sizeof(char*))

typedef struct ebench_oprt_s
{
    estr      name;
    ebench_cb oprt;

    bool      is_step;
}ebench_oprt_t, * ebench_oprt_p;

static void __ebench_oprt_release(ebench_oprt_p p)
{
    estr_free(p->name);
}

static void __ebench_oprt_release_cb(eobj o, eval prvt) { __ebench_oprt_release(EOBJ_VALR(o)); }

typedef struct __ebench_s{

    estr        name;
    uint        scale;
    eval        prvt;

    i64         start;
    i64         end;

    ejson       oprts;
    ejson       scales;
    ejson       results;
}__ebench_t, * __ebench_p;

typedef struct ebench_result_s
{
    __ebench_p    bench;
    ebench_oprt_p oprt;

    uint   scale;

    i64    start;
    i64    end;

    f64    cost;
    char*  cost_unit;
    f64    tpo;
    char*  tpo_unit;
    f64    ops;

    bool   need_gap;

}ebench_result_t, * ebench_result_p;

static void __ebench_release(__ebench_p b)
{
    estr_free(b->name);

    ejson_freeEx(b->oprts, __ebench_oprt_release_cb, EVAL_ZORE);

    ejson_free(b->scales);
    ejson_free(b->results);

    memset(b, 0, sizeof(*b));
}

static void __ebench_release_cb(eobj o, eval prvt) { __ebench_release(EOBJ_VALR(o)); }

typedef struct __ebench_man_s{

    ejson cases;

    //! table
    uint  t_cols_len[T_TITLE_CNT];
    uint  t_t_len1;
    uint  t_t_len2;
    uint  t_row_len;

    estr  result;

}__ebench_man_t;

static __ebench_man_t __ebench_man;

static void __ebench_man_init()
{
    if(!__ebench_man.cases)
    {
        __ebench_man.cases = ejson_new(EOBJ, EVAL_ZORE);

        for(uint i = 0; i < T_TITLE_CNT; i++)
        {
            __ebench_man.t_cols_len[i] = strlen(t_title_map[i]);
        }
    }
}

static void __ebench_man_release()
{
    ejson_freeEx(__ebench_man.cases, __ebench_release_cb, EVAL_ZORE);
    estr_free(__ebench_man.result);

    memset(&__ebench_man, 0, sizeof(__ebench_man));
}

static __ebench_p __ebench_man_new_bench(constr name)
{
    __ebench_p out;

    __ebench_man_init();

    out = ejson_rValR(__ebench_man.cases, name);

    if(!out)
    {
        eobj o = ejson_addR(__ebench_man.cases, name, sizeof(__ebench_t));

        out = EOBJ_VALR(o);

        estr_wrtS(out->name, name);

        out->oprts   = ejson_new(EOBJ, EVAL_ZORE);
        out->scales  = ejson_new(EOBJ, EVAL_ZORE);
        out->results = ejson_new(EARR, EVAL_ZORE);
    }
    else
        out = 0;

    return out;
}

static __ebench_p __ebench_man_get_bench(constr name)
{
    __ebench_man_init();

    return ejson_rValR(__ebench_man.cases, name);
}

static void __ebench_man_exec_cases()
{
    ebench_result_p result = 0;

    printf("executing start...\n"); fflush(stdout);

    ejson_foreach_s(__ebench_man.cases, o)
    {
        __ebench_p bench = EOBJ_VALR(o);

        if(ejson_isEmpty(bench->oprts))
        {
            printf(" case '%s' have no oprts, skipped\n", bench->name);
            continue;
        }

        if(ejson_isEmpty(bench->scales))
        {
            printf(" case '%s' have no scales, skipped\n", bench->name);
            continue;
        }

        ejson_clear(bench->results);

        printf("benchmark: %s\n", bench->name); fflush(stdout);
        int i = 0;
        ejson_foreach_s(bench->scales, oscale)
        {
            bench->scale = (uint)EOBJ_VALI(oscale);

            ejson_foreach_s(bench->oprts, ooprt)
            {
                int all = ejson_size(bench->scales) * ejson_size(bench->oprts);
                int len = strlen(llstr(all));

                i++;

                ebench_oprt_p oprt = EOBJ_VALR(ooprt);

                if(oprt->is_step)
                {
                    printf("  (%*d/%*d)step: %s...", len, i, len, all, oprt->name); fflush(stdout);
                    oprt->oprt((ebench_p)bench);
                    printf(" ok\n");fflush(stdout);

                    if(i % ejson_size(bench->oprts) == 0 && result)
                        result->need_gap = 1;

                    continue;
                }

                printf("  (%*d/%*d)oprt: %s...", len, i, len, all, oprt->name); fflush(stdout);

                result = EOBJ_VALR(ejson_addR(bench->results, 0, sizeof(*result)));

                result->oprt  = oprt;
                result->bench = bench;
                result->scale = bench->scale;

                result->start = clock() * (1000000000ll / CLOCKS_PER_SEC);
                oprt->oprt((ebench_p)bench);
                result->end   = clock() * (1000000000ll / CLOCKS_PER_SEC);

                printf(" ok\n");fflush(stdout);

                if(i % ejson_size(bench->oprts) == 0)
                    result->need_gap = 1;
            }
        }
    }

    printf("executing over.\n\n"); fflush(stdout);
}

static void __ebench_man_cal_results()
{
    estr buf = 0;

    printf("cal result..."); fflush(stdout);

    ejson_foreach_s(__ebench_man.cases, o)
    {
        __ebench_p bench = EOBJ_VALR(o);

        if(ejson_isEmpty(bench->oprts))
        {
            continue;
        }

        if(ejson_isEmpty(bench->scales))
        {
            continue;
        }

        ejson_foreach_s(bench->results, oresult)
        {
            ebench_result_p result = EOBJ_VALR(oresult);

            result->cost  = (f64)(result->end  - result->start);
            result->tpo   = result->cost / result->scale;
            result->ops   = 1e9 / result->tpo;

            result->cost_unit = "ns";
            result->tpo_unit  = "ns";

            if(result->cost > 1000.0)
            {
                result->cost     /= 1000.0;
                result->cost_unit = "us";
            }
            if(result->cost > 1000.0)
            {
                result->cost     /= 1000.0;
                result->cost_unit = "ms";
            }

            if(__ebench_man.t_cols_len[HDR_NAME] < estr_len(result->oprt->name))
                __ebench_man.t_cols_len[HDR_NAME] = (uint)estr_len(result->oprt->name);

            estr_wrtP(buf, "%" PRId64, result->scale);
            if(__ebench_man.t_cols_len[HDR_SCALE] < estr_len(buf))
                __ebench_man.t_cols_len[HDR_SCALE] = (uint)estr_len(buf);

            estr_wrtP(buf, "%.2f%s", result->cost, result->cost_unit);
            if(__ebench_man.t_cols_len[HDR_COST] < estr_len(buf))
                __ebench_man.t_cols_len[HDR_COST] = (uint)estr_len(buf);

            estr_wrtP(buf, "%.2f%s", result->tpo, result->tpo_unit);
            if(__ebench_man.t_cols_len[HDR_TPO] < estr_len(buf))
                __ebench_man.t_cols_len[HDR_TPO] = (uint)estr_len(buf);

            estr_wrtP(buf, "%.2f", result->ops);
            if(__ebench_man.t_cols_len[HDR_OPS] < estr_len(buf))
                __ebench_man.t_cols_len[HDR_OPS] = (uint)estr_len(buf);
        }
    }

    __ebench_man.t_row_len = HDR_END__ - 1; // all space len between col
    for(int i = 0; i < HDR_END__; i++)
    {
        __ebench_man.t_row_len += __ebench_man.t_cols_len[i];
    }

    {
        uint title_len =  sizeof(TITLE) - 1;

        if(__ebench_man.t_row_len > title_len)
        {
            __ebench_man.t_t_len1 = (__ebench_man.t_row_len - title_len) / 2;
            __ebench_man.t_t_len2 = __ebench_man.t_row_len - title_len - __ebench_man.t_t_len1;
        }
        else
        {
            __ebench_man.t_t_len1 = 0;
            __ebench_man.t_t_len2 = 0;
        }
    }

    estr_free(buf);

    printf("ok\n"); fflush(stdout);
}

static void __ebench_man_format_result()
{
    estr_clear(__ebench_man.result);
    estr* buf = &__ebench_man.result;

    ejson_foreach_s(__ebench_man.cases, o)
    {
        __ebench_p bench = EOBJ_VALR(o);

        if(ejson_isEmpty(bench->oprts))
        {
            continue;
        }

        if(ejson_isEmpty(bench->scales))
        {
            continue;
        }

        estr_catS(*buf, "\n");
        estr_catC(*buf, '=', __ebench_man.t_t_len1);
        estr_catS(*buf, TITLE);
        estr_catC(*buf, '=', __ebench_man.t_t_len2);
        estr_catS(*buf, "\n");

        estr_catP(*buf, "%s\n", bench->name);
        estr_catC(*buf, '=', __ebench_man.t_row_len);
        estr_catS(*buf, "\n");

        estr_catP(*buf, "%-*s ", __ebench_man.t_cols_len[0], t_title_map[0]);
        for(uint i = 1; i < T_TITLE_CNT ; i++)
        {
            estr_catP(*buf, "%*s ", __ebench_man.t_cols_len[i], t_title_map[i]);
        }
        estr_catS(*buf, "\n");

        for(uint i = 0; i < T_TITLE_CNT ; i++)
        {
            estr_catC(*buf, '-', __ebench_man.t_cols_len[i]);
            estr_catS(*buf, " ");
        }
        estr_catS(*buf, "\n");

        ejson_foreach_s(bench->results, oresult)
        {
            ebench_result_p result = EOBJ_VALR(oresult);

            estr_catP(*buf, "%-*s "        , __ebench_man.t_cols_len[HDR_NAME] , result->oprt->name);
            estr_catP(*buf, "%*d "         , __ebench_man.t_cols_len[HDR_SCALE], result->scale     );
            estr_catP(*buf, "%*.2f%s "     , (int)(__ebench_man.t_cols_len[HDR_COST] - strlen(result->cost_unit)),  result->cost, result->cost_unit);
            estr_catP(*buf, "%*.2f%s "     , (int)(__ebench_man.t_cols_len[HDR_TPO]  - strlen(result->tpo_unit )),  result->tpo , result->tpo_unit);
            estr_catP(*buf, "%*.2f"        , __ebench_man.t_cols_len[HDR_OPS], result->ops);
            estr_catS(*buf, "\n");

            if(result->need_gap)
                estr_catS(*buf, "\n");
        }
    }

    __ebench_man.result = *buf;
}

ebench ebench_new(constr name)
{
    return (ebench)__ebench_man_new_bench(name);
}

ebench ebench_get(constr name)
{
    return (ebench)__ebench_man_get_bench(name);
}

#define _CHECK_NULL(p) do{if(!p){printf("%s:'%s' can not be null, abort\n", __FUNCTION__, #p); abort();}}while(0)

ebench ebench_addOprt(ebench b, constr name, ebench_cb oprt)
{
    _CHECK_NULL(b); _CHECK_NULL(name); _CHECK_NULL(oprt);

    __ebench_p bench = (__ebench_p)(b);

    ebench_oprt_p _oprt = ejson_rValR(bench->oprts, name);
    if(!_oprt)
    {
        eobj o = ejson_addR(bench->oprts, name, sizeof(*_oprt));

        _oprt = EOBJ_VALR(o);

        estr_wrtS(_oprt->name, name);
        _oprt->oprt = oprt;
    }
    else
    {
        printf("oprt '%s' already exists in benchmark '%s'\n", name, bench->name);
        abort();
    }

    return b;
}

ebench ebench_addStep(ebench b, constr name, ebench_cb oprt)
{
    _CHECK_NULL(b); _CHECK_NULL(name); _CHECK_NULL(oprt);

    __ebench_p bench = (__ebench_p)(b);

    ebench_oprt_p _oprt = ejson_rValR(bench->oprts, name);
    if(!_oprt)
    {
        eobj o = ejson_addR(bench->oprts, name, sizeof(*_oprt));

        _oprt = EOBJ_VALR(o);

        estr_wrtS(_oprt->name, name);
        _oprt->oprt    = oprt;
        _oprt->is_step = 1;
    }
    else
    {
        printf("step '%s' already exists in benchmark '%s'\n", name, bench->name);
        abort();
    }

    return b;
}

ebench ebench_addScale (ebench b, uint scale)
{
    _CHECK_NULL(b); _CHECK_NULL(scale);

    __ebench_p bench = (__ebench_p)(b);

    ejson_addI(bench->scales, ullstr(scale), scale);

    return b;
}

void ebench_exec()
{
    if(ejson_isEmpty(__ebench_man.cases))
    {
        puts("no benchmark cases found...");

        return;
    }

    __ebench_man_exec_cases();
    __ebench_man_cal_results();
    __ebench_man_format_result();
}

void   ebench_showResult()
{
    puts(__ebench_man.result);
}

constr ebench_getResult()
{
    return __ebench_man.result;
}

void ebench_release()
{
    __ebench_man_release();
}
