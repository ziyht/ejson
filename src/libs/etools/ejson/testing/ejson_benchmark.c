//
// Created by ziyht on 17-2-15.
//


#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <inttypes.h>

#define  EUTILS_LLOG

#include "eutils.h"
#include "ejson.h"

#include "ebench.h"

void obj_bench_init(ebench_p b)
{
    b->prvt.p = ejson_new(EOBJ, 0);
}

void obj_bench_add(ebench_p b)
{
    u64 scale = b->scale;

    ejson e = b->prvt.p;

    for(u64 i = 0; i < scale; i++)
    {
        ejson_addT(e, ullstr(i), ETRUE);
    }
}

void obj_bench_find(ebench_p b)
{
    u64 scale = b->scale;

    ejson e = b->prvt.p;

    for(u64 i = 0; i < scale; i++)
    {
        if(!ejson_r(e, ullstr(i)))
        {
            llog_err("'%d' can not find in json");
            abort();
        }
    }
}

void obj_bench_free(ebench_p b)
{
    ejson_free(b->prvt.p);
}

void arr_bench_init(ebench_p b)
{
    b->prvt.p = ejson_new(EARR, 0);
}

void arr_bench_add(ebench_p b)
{
    u64 scale = b->scale;

    ejson e = b->prvt.p;

    for(u64 i = 0; i < scale; i++)
    {
        ejson_addT(e, 0, ETRUE);
    }
}

void arr_bench_find(ebench_p b)
{
    u64 scale = b->scale;

    ejson e = b->prvt.p;

    for(u64 i = 0; i < scale; i++)
    {
        if(!ejson_i(e, (uint)i))
        {
            llog_err("%" PRId64 " can not find in json", );
            abort();
        }
    }
}

void arr_bench_free(ebench_p b)
{
    ejson_free(b->prvt.p);
}

#define CALLGRIND 0


int ejson_benchmark(int argc, char* argv[])
{
    ebench b;

    b = ebench_new("ejson obj operating");
    ebench_addStep (b, "init", obj_bench_init);
    ebench_addOprt (b, "add" , obj_bench_add);
    ebench_addOprt (b, "find", obj_bench_find);
    ebench_addOprt (b, "free", obj_bench_free);
    ebench_addScale(b,   50000);
    ebench_addScale(b,  500000);
    ebench_addScale(b, 5000000);

    b = ebench_new("ejson arr operating");
    ebench_addStep (b, "init", arr_bench_init);
    ebench_addOprt (b, "add" , arr_bench_add);
    ebench_addOprt (b, "find", arr_bench_find);
    ebench_addOprt (b, "free", arr_bench_free);
    ebench_addScale(b,   50000);
    ebench_addScale(b,  500000);
    ebench_addScale(b, 5000000);

    ebench_exec();

    ebench_showResult();

    ebench_release();

    return ETEST_OK;
}

