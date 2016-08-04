#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hdf5.h>
#include <cblas.h>
#include "gfast_hdf5.h"
#include "iscl/log/log.h"

//============================================================================//
int hdf5_copy__peakDisplacementData(
    const enum data2h5_enum job,
    struct GFAST_peakDisplacementData_struct *pgd_data,
    struct h5_peakDisplacementData_struct *h5_pgd_data)
{
    const char *fcnm = "hdf5_copyPeakDisplacementData\0";
    int *lactiveTemp, *lmaskTemp, i, ierr, nsites;
    char *ctemp;
    //------------------------------------------------------------------------//
    ierr = 0;
    if (job == COPY_DATA_TO_H5)
    {
        // Make sure there is something to do
        nsites = pgd_data->nsites;
        if (nsites < 1)
        {
            log_errorF("%s: No sites!\n", fcnm);
            ierr = 1;
            return ierr;
        }

        h5_pgd_data->pd.len = nsites;
        h5_pgd_data->pd.p = (double *)calloc(nsites, sizeof(double));
        cblas_dcopy(nsites, pgd_data->pd, 1, h5_pgd_data->pd.p, 1);

        h5_pgd_data->wt.len = nsites;
        h5_pgd_data->wt.p = (double *)calloc(nsites, sizeof(double));
        cblas_dcopy(nsites, pgd_data->wt, 1, h5_pgd_data->wt.p, 1);

        h5_pgd_data->sta_lat.len = nsites;
        h5_pgd_data->sta_lat.p = (double *)calloc(nsites, sizeof(double));
        cblas_dcopy(nsites, pgd_data->sta_lat, 1, h5_pgd_data->sta_lat.p, 1);

        h5_pgd_data->sta_lon.len = nsites;
        h5_pgd_data->sta_lon.p = (double *)calloc(nsites, sizeof(double));
        cblas_dcopy(nsites, pgd_data->sta_lon, 1, h5_pgd_data->sta_lon.p, 1);

        h5_pgd_data->sta_alt.len = nsites;
        h5_pgd_data->sta_alt.p = (double *)calloc(nsites, sizeof(double));
        cblas_dcopy(nsites, pgd_data->sta_alt, 1, h5_pgd_data->sta_alt.p, 1);

        h5_pgd_data->stnm.len = 64*nsites;
        ctemp = (char *)calloc(64*nsites, sizeof(char));

        h5_pgd_data->lactive.len = nsites;
        lactiveTemp = (int *)calloc(nsites, sizeof(int));

        h5_pgd_data->lmask.len = nsites;
        lmaskTemp = (int *)calloc(nsites, sizeof(int));
        for (i=0; i<nsites; i++)
        {
            lactiveTemp[i] = pgd_data->lactive[i];
            lmaskTemp[i] = pgd_data->lmask[i];
            strcpy(&ctemp[i*64], pgd_data->stnm[i]);
        }
        h5_pgd_data->stnm.p = ctemp;
        h5_pgd_data->lactive.p = lactiveTemp;
        h5_pgd_data->lmask.p = lmaskTemp;
    }
    else if (job == COPY_H5_TO_DATA)
    {
        printf("%s: Not yet done\n", fcnm);
        ierr = 1;
    }
    else
    {
        printf("%s: Invalid job\n", fcnm);
        ierr = 1;
    }
    return ierr;
}
//============================================================================//
/*!
 * @brief Copies PGD results structure to/from HDF5 PGD results structure
 *
 * @param[in] job        if job = COPY_DATA_TO_H5 then copy pgd -> h5_pgd
 *                       if job = COPY_H5_TO_DATA then copy h5_pgd -> pgd
 *
 * @param[inout] pgd     if job = COPY_DATA_TO_H5 then on input this is the
 *                       structure to copy to h5_pgd
 *                       if job = COPY_H5_TO_DATA then on output this is the
 *                       copied h5_pgd structure 
 * @param[inout] h5_pgd  if job = COPY_DATA_TO_H5 then on output this is the
 *                       HDF5 version of pgd
 *                       if job = COPY_DATA_TO_H5 then on input this is the
 *                       structure to copy to pgd 
 *
 * @author Ben Baker, ISTI
 *
 * @bug COPY_H5_TO_DATA not yet done
 *
 */
int hdf5_copy__pgdResults(const enum data2h5_enum job,
                          struct GFAST_pgdResults_struct *pgd,
                          struct h5_pgdResults_struct *h5_pgd)
{
    const char *fcnm = "hdf5_copyPGDResults\0";
    int *lsiteUsedTemp, i, ierr, ndeps, nsites;
    //------------------------------------------------------------------------//
    ierr = 0;
    if (job == COPY_DATA_TO_H5)
    {
        memset(h5_pgd, 0, sizeof(struct h5_pgdResults_struct));
        // Make sure there is something to do
        ndeps = pgd->ndeps;
        nsites = pgd->nsites;
        if (ndeps < 1 || nsites < 1)
        {
            if (nsites < 1){log_errorF("%s: No sites!\n", fcnm);}
            if (ndeps < 1){log_errorF("%s: No depths!\n", fcnm);}
            ierr = 1;
            return ierr;
        }

        h5_pgd->mpgd.len = ndeps;
        h5_pgd->mpgd.p = (double *)calloc(ndeps, sizeof(double));
        cblas_dcopy(ndeps, pgd->mpgd, 1, h5_pgd->mpgd.p, 1);

        h5_pgd->mpgd_vr.len = ndeps;
        h5_pgd->mpgd_vr.p = (double *)calloc(ndeps, sizeof(double));
        cblas_dcopy(ndeps, pgd->mpgd_vr, 1, h5_pgd->mpgd_vr.p, 1);

        h5_pgd->dep_vr_pgd.len = ndeps;
        h5_pgd->dep_vr_pgd.p = (double *)calloc(ndeps, sizeof(double));
        cblas_dcopy(ndeps, pgd->dep_vr_pgd, 1, h5_pgd->dep_vr_pgd.p, 1);

        h5_pgd->UP.len = nsites*ndeps;
        h5_pgd->UP.p = (double *)calloc(nsites*ndeps, sizeof(double));
        cblas_dcopy(nsites*ndeps, pgd->UP, 1, h5_pgd->UP.p, 1);

        h5_pgd->UPinp.len = nsites;
        h5_pgd->UPinp.p = (double *)calloc(nsites, sizeof(double));
        cblas_dcopy(nsites, pgd->UPinp, 1, h5_pgd->UPinp.p, 1);

        h5_pgd->srcDepths.len = ndeps;
        h5_pgd->srcDepths.p = (double *)calloc(ndeps, sizeof(double)); 
        cblas_dcopy(ndeps, pgd->srcDepths, 1, h5_pgd->srcDepths.p, 1);

        h5_pgd->iqr75_25.len = ndeps;
        h5_pgd->iqr75_25.p = (double *)calloc(ndeps, sizeof(double));
        cblas_dcopy(ndeps, pgd->iqr75_25, 1, h5_pgd->iqr75_25.p, 1);

        h5_pgd->lsiteUsed.len = nsites;
        lsiteUsedTemp = (int *)calloc(nsites, sizeof(int));
        for (i=0; i<nsites; i++)
        {
            lsiteUsedTemp[i] = (int) pgd->lsiteUsed[i];
        }
        h5_pgd->lsiteUsed.p = lsiteUsedTemp;

        h5_pgd->ndeps = ndeps;
        h5_pgd->nsites = nsites; 
    }
    else if (job == COPY_H5_TO_DATA)
    {
        log_errorF("%s: Error not yet done\n", fcnm);
        ierr = 1;
    }
    else
    {
        log_errorF("%s: Invalid job\n", fcnm);
        ierr = 1;
    }
    return ierr;
}
//============================================================================//
/*!
 * @brief Copies CMT results structure to/from HDF5 CMT results structure
 *
 * @param[in] job        if job = COPY_DATA_TO_H5 then copy cmt -> h5_cmt
 *                       if job = COPY_H5_TO_DATA then copy h5_cmt -> cmt 
 *
 * @param[inout] cmt     if job = COPY_DATA_TO_H5 then on input this is the
 *                       structure to copy to h5_cmt
 *                       if job = COPY_H5_TO_DATA then on output this is the
 *                       copied h5_cmt structure 
 * @param[inout] h5_cmt  if job = COPY_DATA_TO_H5 then on output this is the
 *                       HDF5 version of cmt 
 *                       if job = COPY_DATA_TO_H5 then on input this is the
 *                       structure to copy to cmt
 *
 * @author Ben Baker, ISTI
 *
 * @bug COPY_H5_TO_DATA not yet done
 *
 */
int hdf5_copy__cmtResults(const enum data2h5_enum job,
                          struct GFAST_cmtResults_struct *cmt,
                          struct h5_cmtResults_struct *h5_cmt)
{
    const char *fcnm = "hdf5_copy__cmtResults\0";
    int *lsiteUsedTemp, i, ierr, ndeps, nsites;
    //------------------------------------------------------------------------//
    ierr = 0;
    if (job == COPY_DATA_TO_H5)
    {
        memset(h5_cmt, 0, sizeof(struct h5_cmtResults_struct));
        // Make sure there is something to do
        ndeps = cmt->ndeps;
        nsites = cmt->nsites;
        if (ndeps < 1 || nsites < 1)
        {
            if (nsites < 1){log_errorF("%s: No sites!\n", fcnm);}
            if (ndeps < 1){log_errorF("%s: No depths!\n", fcnm);}
            ierr = 1;
            return ierr;
        }
        h5_cmt->objfn.len = ndeps;
        h5_cmt->objfn.p = (double *)calloc(ndeps, sizeof(double));
        cblas_dcopy(ndeps, cmt->objfn, 1, h5_cmt->objfn.p, 1);

        h5_cmt->mts.len = 6*ndeps;
        h5_cmt->mts.p = (double *)calloc(6*ndeps, sizeof(double));
        cblas_dcopy(6*ndeps, cmt->mts, 1, h5_cmt->mts.p, 1);

        h5_cmt->str1.len = ndeps;
        h5_cmt->str1.p = (double *)calloc(ndeps, sizeof(double));
        cblas_dcopy(ndeps, cmt->str1, 1, h5_cmt->str1.p, 1);

        h5_cmt->str2.len = ndeps;
        h5_cmt->str2.p = (double *)calloc(ndeps, sizeof(double));
        cblas_dcopy(ndeps, cmt->str2, 1, h5_cmt->str2.p, 1);

        h5_cmt->dip1.len = ndeps;
        h5_cmt->dip1.p = (double *)calloc(ndeps, sizeof(double));
        cblas_dcopy(ndeps, cmt->dip1, 1, h5_cmt->dip1.p, 1);

        h5_cmt->dip2.len = ndeps;
        h5_cmt->dip2.p = (double *)calloc(ndeps, sizeof(double));
        cblas_dcopy(ndeps, cmt->dip2, 1, h5_cmt->dip2.p, 1);

        h5_cmt->rak1.len = ndeps;
        h5_cmt->rak1.p = (double *)calloc(ndeps, sizeof(double));
        cblas_dcopy(ndeps, cmt->rak1, 1, h5_cmt->rak1.p, 1);

        h5_cmt->rak2.len = ndeps;
        h5_cmt->rak2.p = (double *)calloc(ndeps, sizeof(double));
        cblas_dcopy(ndeps, cmt->rak2, 1, h5_cmt->rak2.p, 1);

        h5_cmt->Mw.len = ndeps;
        h5_cmt->Mw.p = (double *)calloc(ndeps, sizeof(double));
        cblas_dcopy(ndeps, cmt->Mw, 1, h5_cmt->Mw.p, 1);

        h5_cmt->srcDepths.len = ndeps;
        h5_cmt->srcDepths.p = (double *)calloc(ndeps, sizeof(double));
        cblas_dcopy(ndeps, cmt->srcDepths, 1, h5_cmt->srcDepths.p, 1); 

        h5_cmt->EN.len = nsites*ndeps;
        h5_cmt->EN.p = (double *)calloc(nsites*ndeps, sizeof(double));
        cblas_dcopy(nsites*ndeps, cmt->EN, 1, h5_cmt->EN.p, 1); 

        h5_cmt->NN.len = nsites*ndeps;
        h5_cmt->NN.p = (double *)calloc(nsites*ndeps, sizeof(double));
        cblas_dcopy(nsites*ndeps, cmt->NN, 1, h5_cmt->NN.p, 1);

        h5_cmt->UN.len = nsites*ndeps;
        h5_cmt->UN.p = (double *)calloc(nsites*ndeps, sizeof(double));
        cblas_dcopy(nsites*ndeps, cmt->UN, 1, h5_cmt->UN.p, 1);

        h5_cmt->Einp.len = nsites;
        h5_cmt->Einp.p = (double *)calloc(nsites, sizeof(double));
        cblas_dcopy(nsites, cmt->EN, 1, h5_cmt->Einp.p, 1); 

        h5_cmt->Ninp.len = nsites;
        h5_cmt->Ninp.p = (double *)calloc(nsites, sizeof(double));
        cblas_dcopy(nsites, cmt->NN, 1, h5_cmt->Ninp.p, 1); 

        h5_cmt->Uinp.len = nsites;
        h5_cmt->Uinp.p = (double *)calloc(nsites, sizeof(double));
        cblas_dcopy(nsites, cmt->UN, 1, h5_cmt->Uinp.p, 1);

        h5_cmt->lsiteUsed.len = nsites;
        lsiteUsedTemp = (int *)calloc(nsites, sizeof(int));
        for (i=0; i<nsites; i++)
        {
            lsiteUsedTemp[i] = (int) cmt->lsiteUsed[i];
        }
        h5_cmt->lsiteUsed.p = lsiteUsedTemp;


        h5_cmt->opt_indx = cmt->opt_indx;
        h5_cmt->ndeps = ndeps;
        h5_cmt->nsites = nsites;
    }
    else if (job == COPY_H5_TO_DATA)
    {
        log_errorF("%s: Error not yet done\n", fcnm);
        ierr = 1;
    }
    else
    {
        log_errorF("%s: Invalid job\n", fcnm);
        ierr = 1;
    }
    return ierr;
}
//============================================================================//

int hdf5_copy__faultPlane(const enum data2h5_enum job,
                          struct GFAST_faultPlane_struct *fp,
                          struct h5_faultPlane_struct *h5_fp)
{
    const char *fcnm = "hdf5_copy__faultPlane\0";
    int *itemp, i, ierr, ndip, nfp, nsites, nstr;
    //------------------------------------------------------------------------//
    ierr = 0;
    if (job == COPY_DATA_TO_H5)
    {
        memset(h5_fp, 0, sizeof(struct h5_faultPlane_struct));
        // Make sure there is something to do
        nstr = fp->nstr;
        ndip = fp->ndip;
        nsites = fp->nsites_used;
        if (ndip < 1 || nsites < 1 || nstr < 1)
        {
            if (nsites < 1){log_errorF("%s: No sites!\n", fcnm);}
            if (ndip < 1){log_errorF("%s: No faults down dip!\n", fcnm);}
            if (nstr < 1){log_errorF("%s: No faults along strike!\n", fcnm);}
            ierr = 1;
            return ierr;
        }
        nfp = nstr*ndip; // number of fault patches

        h5_fp->lon_vtx.len = 4*nfp;
        h5_fp->lon_vtx.p = (double *)calloc(4*nfp, sizeof(double));
        cblas_dcopy(4*nfp, fp->lon_vtx, 1, h5_fp->lon_vtx.p, 1);

        h5_fp->lat_vtx.len = 4*nfp;
        h5_fp->lat_vtx.p = (double *)calloc(4*nfp, sizeof(double));
        cblas_dcopy(4*nfp, fp->lat_vtx, 1, h5_fp->lat_vtx.p, 1);

        h5_fp->dep_vtx.len = 4*nfp;
        h5_fp->dep_vtx.p = (double *)calloc(4*nfp, sizeof(double));
        cblas_dcopy(4*nfp, fp->dep_vtx, 1, h5_fp->dep_vtx.p, 1);

        h5_fp->fault_xutm.len = nfp;
        h5_fp->fault_xutm.p = (double *)calloc(nfp, sizeof(double));
        cblas_dcopy(nfp, fp->fault_xutm, 1, h5_fp->fault_xutm.p, 1);

        h5_fp->fault_yutm.len = nfp;
        h5_fp->fault_yutm.p = (double *)calloc(nfp, sizeof(double));
        cblas_dcopy(nfp, fp->fault_yutm, 1, h5_fp->fault_yutm.p, 1);

        h5_fp->fault_alt.len = nfp;
        h5_fp->fault_alt.p = (double *)calloc(nfp, sizeof(double));
        cblas_dcopy(nfp, fp->fault_alt, 1, h5_fp->fault_alt.p, 1);

        h5_fp->strike.len = nfp;
        h5_fp->strike.p = (double *)calloc(nfp, sizeof(double));
        cblas_dcopy(nfp, fp->strike, 1, h5_fp->strike.p, 1);

        h5_fp->dip.len = nfp;
        h5_fp->dip.p = (double *)calloc(nfp, sizeof(double));
        cblas_dcopy(nfp, fp->dip, 1, h5_fp->dip.p, 1);

        h5_fp->length.len = nfp;
        h5_fp->length.p = (double *)calloc(nfp, sizeof(double));
        cblas_dcopy(nfp, fp->length, 1, h5_fp->length.p, 1);

        h5_fp->width.len = nfp;
        h5_fp->width.p = (double *)calloc(nfp, sizeof(double));
        cblas_dcopy(nfp, fp->width, 1, h5_fp->width.p, 1);

        h5_fp->sslip.len = nfp;
        h5_fp->sslip.p = (double *)calloc(nfp, sizeof(double));
        cblas_dcopy(nfp, fp->sslip, 1, h5_fp->sslip.p, 1);

        h5_fp->dslip.len = nfp;
        h5_fp->dslip.p = (double *)calloc(nfp, sizeof(double));
        cblas_dcopy(nfp, fp->dslip, 1, h5_fp->dslip.p, 1);

        h5_fp->sslip_unc.len = nfp;
        h5_fp->sslip_unc.p = (double *)calloc(nfp, sizeof(double));
        cblas_dcopy(nfp, fp->sslip_unc, 1, h5_fp->sslip_unc.p, 1);

        h5_fp->dslip_unc.len = nfp;
        h5_fp->dslip_unc.p = (double *)calloc(nfp, sizeof(double));
        cblas_dcopy(nfp, fp->dslip_unc, 1, h5_fp->dslip_unc.p, 1);

        h5_fp->EN.len = nsites;
        h5_fp->EN.p = (double *)calloc(nsites, sizeof(double));
        cblas_dcopy(nsites, fp->EN, 1, h5_fp->EN.p, 1);

        h5_fp->NN.len = nsites;
        h5_fp->NN.p = (double *)calloc(nsites, sizeof(double));
        cblas_dcopy(nsites, fp->NN, 1, h5_fp->NN.p, 1);

        h5_fp->UN.len = nsites;
        h5_fp->UN.p = (double *)calloc(nsites, sizeof(double));
        cblas_dcopy(nsites, fp->UN, 1, h5_fp->UN.p, 1);

        h5_fp->Einp.len = nsites;
        h5_fp->Einp.p = (double *)calloc(nsites, sizeof(double));
        cblas_dcopy(nsites, fp->Einp, 1, h5_fp->Einp.p, 1);

        h5_fp->Ninp.len = nsites;
        h5_fp->Ninp.p = (double *)calloc(nsites, sizeof(double));
        cblas_dcopy(nsites, fp->Ninp, 1, h5_fp->Ninp.p, 1);

        h5_fp->Uinp.len = nsites;
        h5_fp->Uinp.p = (double *)calloc(nsites, sizeof(double));
        cblas_dcopy(nsites, fp->Uinp, 1, h5_fp->Uinp.p, 1);

        h5_fp->fault_ptr.len = nfp + 1;
        itemp = (int *)calloc(nfp+1, sizeof(int));
        for (i=0; i<nfp+1; i++)
        {
            itemp[i] = fp->fault_ptr[i]; 
        } 
        h5_fp->fault_ptr.p = itemp;

        h5_fp->maxobs = fp->maxobs;
        h5_fp->nsites_used = fp->nsites_used;
        h5_fp->nstr = fp->nstr;
        h5_fp->ndip = fp->ndip;
    }
    else if (job == COPY_H5_TO_DATA)
    {
        log_errorF("%s: Error not yet done\n", fcnm);
        ierr = 1;
    }
    else
    {
        log_errorF("%s: Invalid job\n", fcnm);
        ierr = 1;
    }
    return ierr;
}
//============================================================================//
/*!
 * @brief Copies finite fault results structure to/from HDF5 finite fault
 *        results structure
 *
 * @param[in] job        if job = COPY_DATA_TO_H5 then copy ff -> h5_ff
 *                       if job = COPY_H5_TO_DATA then copy h5_ff -> ff
 *
 * @param[inout] ff      if job = COPY_DATA_TO_H5 then on input this is the
 *                       structure to copy to h5_ff
 *                       if job = COPY_H5_TO_DATA then on output this is the
 *                       copied h5_ff structure 
 * @param[inout] h5_ff   if job = COPY_DATA_TO_H5 then on output this is the
 *                       HDF5 version of ff
 *                       if job = COPY_DATA_TO_H5 then on input this is the
 *                       structure to copy to ff 
 *
 * @author Ben Baker, ISTI
 *
 * @bug COPY_H5_TO_DATA not yet done
 *
 */
int hdf5_copy__ffResults(const enum data2h5_enum job,
                         struct GFAST_ffResults_struct *ff,
                         struct h5_ffResults_struct *h5_ff)
{
    const char *fcnm = "hdf5_copyFFResults\0";
    struct h5_faultPlane_struct *h5_fp = NULL;
    int i, ierr, nfp;
    //------------------------------------------------------------------------//
    ierr = 0;
    if (job == COPY_DATA_TO_H5)
    {
        nfp = ff->nfp;
        if (nfp < 0)
        {
            log_errorF("%s: Error no fault planes!\n", fcnm);
            return -1;
        }

        h5_ff->fp.len = nfp;
        h5_fp = (struct h5_faultPlane_struct *)
                calloc(nfp, sizeof(struct h5_faultPlane_struct));
        for (i=0; i<nfp; i++)
        {
            ierr = GFAST_hdf5_copy__faultPlane(job, &ff->fp[i], &h5_fp[i]);
        }
        h5_ff->fp.p = h5_fp;

        h5_ff->vr.len = nfp;
        h5_ff->vr.p = (double *)calloc(nfp, sizeof(double));
        cblas_dcopy(nfp, ff->vr, 1, h5_ff->vr.p, 1); 

        h5_ff->Mw.len = nfp;
        h5_ff->Mw.p = (double *)calloc(nfp, sizeof(double));
        cblas_dcopy(nfp, ff->Mw, 1, h5_ff->Mw.p, 1);

        h5_ff->str.len = nfp;
        h5_ff->str.p = (double *)calloc(nfp, sizeof(double));
        cblas_dcopy(nfp, ff->str, 1, h5_ff->str.p, 1);

        h5_ff->dip.len = nfp;
        h5_ff->dip.p = (double *)calloc(nfp, sizeof(double));
        cblas_dcopy(nfp, ff->dip, 1, h5_ff->dip.p, 1);

        h5_ff->SA_lat = ff->SA_lat;
        h5_ff->SA_lon = ff->SA_lon;
        h5_ff->SA_dep = ff->SA_dep;
        h5_ff->SA_mag = ff->SA_mag;
        h5_ff->preferred_fault_plane = ff->preferred_fault_plane;
        h5_ff->nfp = ff->nfp;

    }   
    else if (job == COPY_H5_TO_DATA)
    {
        log_errorF("%s: not yet done\n", fcnm);
        ierr = 1;
    }
    else
    {
        log_errorF("%s: Invalid job\n", fcnm);
        ierr = 1;
    }
    return ierr;
}
