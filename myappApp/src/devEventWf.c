/*************************************************************************\
 *  * * Copyright (c) 2008 UChicago Argonne LLC, as Operator of Argonne
 *   * *     National Laboratory.
 *    * * Copyright (c) 2002 The Regents of the University of California, as
 *     * *     Operator of Los Alamos National Laboratory.
 *      * * EPICS BASE is distributed subject to a Software License Agreement found
 *       * * in file LICENSE that is included with this distribution.
 *        * \*************************************************************************/

/*
 *  *  *      Original Authors: Bob Dalesio and Marty Kraimer
 *   *   *      Date: 6-1-90
 *    *    */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "alarm.h"
#include "dbDefs.h"
#include "dbAccess.h"
#include "dbEvent.h"
#include "recGbl.h"
#include "devSup.h"
#include "waveformRecord.h"
#include "epicsExport.h"
#include "devEventWf.h"
#include "cantProceed.h"

/* Create the dset for devWfSoft */
static long init_record(waveformRecord *prec);
static long read_wf(waveformRecord *prec);

struct {
    long      number;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN read_wf;
} devEventWf = {
    5,
    NULL,
    NULL,
    init_record,
    NULL,
    read_wf
};
struct devicePvt {
    int count;
    epicsOldString *buffer;
};
epicsExportAddress(dset, devEventWf);

static long init_record(waveformRecord *prec)
{
    /* INP must be CONSTANT, PV_LINK, DB_LINK or CA_LINK*/
    struct devicePvt* pvt;
	switch (prec->inp.type) {
    case CONSTANT:
        prec->nord = 0;
        break;
    case PV_LINK:
    case DB_LINK:
        pvt = (struct devicePvt*)malloc(sizeof(struct devicePvt));
        pvt->buffer = calloc(prec->nelm, dbValueSize(prec->ftvl));
        pvt->count = 0;
        prec->dpvt = pvt;
//	printf("%s pvt->buffer is %p\n",prec->name,pvt->buffer);
    case CA_LINK:
        break;
    default:
        recGblRecordError(S_db_badField, (void *)prec,
            "devWfSoft (init_record) Illegal INP field");
        return(S_db_badField);
    }
    return 0;
}

static long read_wf(waveformRecord *prec)
{
    struct devicePvt *pvt=prec->dpvt;
    long nRequest = prec->nelm;
    char timestr[32];
    int i = 0;
    epicsOldString inpval = "";
//    inpval = callocMustSucceed(1, 40*sizeof(char), "inpval calloc failed");

    dbGetLink(&prec->inp, prec->ftvl, &inpval, 0, 0);

    recGblGetTimeStamp(prec);
//    printf("%d, %d\n", (prec->time).secPastEpoch, (prec->time).nsec);
    epicsTimeToStrftime(timestr, sizeof(timestr),
                             "%Y/%m/%d %H:%M:%S.%06f" , &prec->time);
//    strcat(inpval,"-");
//    strcat(inpval,timestr);
//	printf("%s inpval is %s\n", prec->name, inpval);
//	printf("%s addr is %p\n",prec->name,((epicsOldString *)pvt->buffer));
//	printf("%s addr is %p\n",prec->name,((epicsOldString *)pvt->buffer)+1);
    if(pvt->count < nRequest) {
            snprintf(((epicsOldString *)pvt->buffer)[pvt->count],40,"%s",inpval);
//		printf("count is %d\t str is %s\n", pvt->count, ((epicsOldString *)pvt->buffer)[pvt->count]);
		pvt->count++;
//		prec->nord = 0;
    }else if(pvt->count >= nRequest) {
            for(i = 0; i < nRequest; i++){
                    snprintf(((epicsOldString *)prec->bptr)[i],40,"%s",((epicsOldString *)pvt->buffer)[i]);
            }
            snprintf(((epicsOldString *)pvt->buffer)[0],40,"%s",inpval);
            pvt->count = 1;
	    prec->nord = nRequest;
	    prec->udf = FALSE;
    }
    
/**
    if (nRequest > 0) {
		prec->nord = nRequest;
        if (prec->tsel.type == CONSTANT &&
            prec->tse == epicsTimeEventDeviceTime)
            dbGetTimeStamp(&prec->inp, &prec->time);
    }
**/
//    prec->udf = FALSE;
    return 0;
}

