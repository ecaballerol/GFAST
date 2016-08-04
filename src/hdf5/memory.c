#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gfast_hdf5.h"
#include "iscl/log/log.h"

int hdf5_memory__freePGDResults(struct h5_pgdResults_struct *pgd)
{
    if (pgd->mpgd.p       != NULL){free(pgd->mpgd.p);}
    if (pgd->mpgd_vr.p    != NULL){free(pgd->mpgd_vr.p);}
    if (pgd->dep_vr_pgd.p != NULL){free(pgd->dep_vr_pgd.p);}
    if (pgd->UP.p         != NULL){free(pgd->UP.p);}
    if (pgd->UPinp.p      != NULL){free(pgd->UPinp.p);}
    if (pgd->srcDepths.p  != NULL){free(pgd->srcDepths.p);}
    if (pgd->iqr75_25.p   != NULL){free(pgd->iqr75_25.p);}
    if (pgd->lsiteUsed.p  != NULL){free(pgd->lsiteUsed.p);}
    memset(pgd, 0, sizeof(struct h5_pgdResults_struct));
    return 0;
}

int hdf5_memory__freeCMTResults(struct h5_cmtResults_struct *cmt)
{
    if (cmt->objfn.p     != NULL){free(cmt->objfn.p);}
    if (cmt->mts.p       != NULL){free(cmt->mts.p);}
    if (cmt->str1.p      != NULL){free(cmt->str1.p);}
    if (cmt->str2.p      != NULL){free(cmt->str2.p);}
    if (cmt->dip1.p      != NULL){free(cmt->dip1.p);}
    if (cmt->dip2.p      != NULL){free(cmt->dip2.p);}
    if (cmt->rak1.p      != NULL){free(cmt->rak1.p);}
    if (cmt->rak2.p      != NULL){free(cmt->rak2.p);}
    if (cmt->Mw.p        != NULL){free(cmt->Mw.p);}
    if (cmt->srcDepths.p != NULL){free(cmt->srcDepths.p);}
    if (cmt->EN.p        != NULL){free(cmt->EN.p);}
    if (cmt->NN.p        != NULL){free(cmt->NN.p);}
    if (cmt->UN.p        != NULL){free(cmt->UN.p);} 
    if (cmt->Einp.p      != NULL){free(cmt->Einp.p);}
    if (cmt->Ninp.p      != NULL){free(cmt->Ninp.p);}
    if (cmt->Uinp.p      != NULL){free(cmt->Uinp.p);}
    if (cmt->lsiteUsed.p != NULL){free(cmt->lsiteUsed.p);}
    memset(cmt, 0, sizeof(struct h5_cmtResults_struct));
    return 0;
}

int hdf5_memory__freeFaultPlane(struct h5_faultPlane_struct *fp)
{
    if (fp->lon_vtx.p    != NULL){free(fp->lon_vtx.p);}
    if (fp->lat_vtx.p    != NULL){free(fp->lat_vtx.p);}
    if (fp->dep_vtx.p    != NULL){free(fp->dep_vtx.p);}
    if (fp->fault_xutm.p != NULL){free(fp->fault_xutm.p);}
    if (fp->fault_yutm.p != NULL){free(fp->fault_yutm.p);}
    if (fp->fault_alt.p  != NULL){free(fp->fault_alt.p);}
    if (fp->strike.p     != NULL){free(fp->strike.p);}
    if (fp->dip.p        != NULL){free(fp->dip.p);}
    if (fp->length.p     != NULL){free(fp->length.p);}
    if (fp->width.p      != NULL){free(fp->width.p);}
    if (fp->sslip.p      != NULL){free(fp->sslip.p);}
    if (fp->dslip.p      != NULL){free(fp->dslip.p);}
    if (fp->sslip_unc.p  != NULL){free(fp->sslip_unc.p);}
    if (fp->dslip_unc.p  != NULL){free(fp->dslip_unc.p);}
    if (fp->EN.p         != NULL){free(fp->EN.p);}
    if (fp->NN.p         != NULL){free(fp->NN.p);}
    if (fp->UN.p         != NULL){free(fp->UN.p);}
    if (fp->Einp.p       != NULL){free(fp->Einp.p);}
    if (fp->Ninp.p       != NULL){free(fp->Ninp.p);}
    if (fp->Uinp.p       != NULL){free(fp->Uinp.p);}
    if (fp->fault_ptr.p  != NULL){free(fp->fault_ptr.p);}
    memset(fp, 0, sizeof(struct h5_faultPlane_struct));
    return 0;
}

int hdf5_memory__freeFFResults(struct h5_ffResults_struct *ff)
{
    struct h5_faultPlane_struct *fp = NULL;
    int i;
    if (ff->fp.p != NULL)
    {
        fp = (struct h5_faultPlane_struct *) ff->fp.p;
        for (i=0; i<ff->fp.len; i++)
        {
            GFAST_hdf5_memory__freeFaultPlane(&fp[i]);
        }
        free(ff->fp.p);
        fp = NULL;
    }
    if (ff->vr.p  != NULL){free(ff->vr.p);}
    if (ff->Mw.p  != NULL){free(ff->Mw.p);}
    if (ff->str.p != NULL){free(ff->str.p);}
    if (ff->dip.p != NULL){free(ff->dip.p);}
    memset(ff, 0, sizeof(struct h5_ffResults_struct));
    return 0;
}
