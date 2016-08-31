#include <limits.h>
#include <hdf5.h>
#include "gfast_struct.h"

#ifndef __GFAST_HDF5_H__
#define __GFAST_HDF5_H__

enum data2h5_enum
{
    COPY_DATA_TO_H5 = 0,  /*!< Copies data structure to HDF5 structure */
    COPY_H5_TO_DATA = 1,  /*!< Copies HDF5 structure to data structure */
};

struct h5_peakDisplacementData_struct
{
    hvl_t stnm;
    hvl_t pd;
    hvl_t wt;
    hvl_t sta_lat;
    hvl_t sta_lon;
    hvl_t sta_alt;
    hvl_t lmask;
    hvl_t lactive;
    int nsites;
};

struct h5_pgdResults_struct
{
    hvl_t mpgd;
    hvl_t mpgd_vr;
    hvl_t dep_vr_pgd;
    hvl_t UP;
    hvl_t UPinp;
    hvl_t srcDepths;
    hvl_t srdist;
    hvl_t iqr;
    hvl_t lsiteUsed;
    int ndeps;
    int nsites;
};

struct h5_cmtResults_struct
{
    hvl_t l2;
    hvl_t pct_dc;
    hvl_t objfn;
    hvl_t mts;
    hvl_t str1;
    hvl_t str2;
    hvl_t dip1;
    hvl_t dip2;
    hvl_t rak1;
    hvl_t rak2;
    hvl_t Mw;
    hvl_t srcDepths;
    hvl_t EN;
    hvl_t NN;
    hvl_t UN;
    hvl_t Einp;
    hvl_t Ninp;
    hvl_t Uinp;
    hvl_t lsiteUsed;
    int opt_indx;
    int ndeps;
    int nsites;
};

struct h5_offsetData_struct
{
    hvl_t stnm;
    hvl_t ubuff;
    hvl_t nbuff;
    hvl_t ebuff;
    hvl_t wtu;
    hvl_t wtn;
    hvl_t wte;
    hvl_t sta_lat;
    hvl_t sta_lon;
    hvl_t sta_alt;
    hvl_t lmask;
    hvl_t lactive;
    int nsites;
};

struct h5_faultPlane_struct
{
    hvl_t lon_vtx;
    hvl_t lat_vtx;
    hvl_t dep_vtx;
    hvl_t fault_xutm;
    hvl_t fault_yutm;
    hvl_t fault_alt;
    hvl_t strike;
    hvl_t dip;
    hvl_t length;
    hvl_t width;
    hvl_t sslip;
    hvl_t dslip;
    hvl_t sslip_unc;
    hvl_t dslip_unc;
    hvl_t EN;
    hvl_t NN;
    hvl_t UN;
    hvl_t Einp;
    hvl_t Ninp;
    hvl_t Uinp;
    hvl_t fault_ptr;
    int maxobs;
    int nsites_used;
    int nstr;
    int ndip;
};

struct h5_ffResults_struct
{
    hvl_t fp;
    hvl_t vr;
    hvl_t Mw;
    hvl_t str;
    hvl_t dip;
    double SA_lat;
    double SA_lon;
    double SA_dep;
    double SA_mag;
    int preferred_fault_plane;
    int nfp;
};

struct h5_hypocenter_struct
{
    char eventid[128]; /*!< Event ID */
    double lat;        /*!< Event latitude (degrees) */
    double lon;        /*!< Event longitude (degrees) */
    double dep;        /*!< Event depth (kilometers) */ 
    double mag;        /*!< Event magnitude */
    double time;       /*!< Event epochal time (s) */
};

#ifdef __cplusplus
extern "C"
{
#endif

int hdf5_copy__cmtResults(const enum data2h5_enum job,
                          struct GFAST_cmtResults_struct *cmt,
                          struct h5_cmtResults_struct *h5_cmt);
int hdf5_copy__faultPlane(const enum data2h5_enum job,
                          struct GFAST_faultPlane_struct *fp,
                          struct h5_faultPlane_struct *h5_fp);
int hdf5_copy__ffResults(const enum data2h5_enum job,
                         struct GFAST_ffResults_struct *ff,
                         struct h5_ffResults_struct *h5_ff);
int hdf5_copy__hypocenter(const enum data2h5_enum job,
                          struct GFAST_shakeAlert_struct *hypo,
                          struct h5_hypocenter_struct *h5_hypo);
int hdf5_copy__offsetData(const enum data2h5_enum job,
                          struct GFAST_offsetData_struct *offset_data,
                          struct h5_offsetData_struct *h5_offset_data);
int hdf5_copy__peakDisplacementData(
    const enum data2h5_enum job,
    struct GFAST_peakDisplacementData_struct *pgd_data,
    struct h5_peakDisplacementData_struct *h5_pgd_data);
int hdf5_copy__pgdResults(const enum data2h5_enum job,
                          struct GFAST_pgdResults_struct *pgd,
                          struct h5_pgdResults_struct *h5_pgd);

herr_t hdf5_createType__cmtResults(hid_t group_id);
herr_t hdf5_createType__faultPlane(hid_t group_id);
herr_t hdf5_createType__ffResults(hid_t group_id);
herr_t hdf5_createType__hypocenter(hid_t group_id);
herr_t hdf5_createType__offsetData(hid_t group_id);
herr_t hdf5_createType__peakDisplacementData(hid_t group_id);
herr_t hdf5_createType__pgdResults(hid_t group_id);

int hdf5_initialize(const char *adir,
                    const char *evid,
                    const char *propfilename);


int hdf5_memory__freeCMTResults(struct h5_cmtResults_struct *cmt);
int hdf5_memory__freeFaultPlane(struct h5_faultPlane_struct *fp);
int hdf5_memory__freeFFResults(struct h5_ffResults_struct *ff);
int hdf5_memory__freeOffsetData(
    struct h5_offsetData_struct *h5_offset_data);
int hdf5_memory__freePGDData(
    struct h5_peakDisplacementData_struct *h5_pgd_data);
int hdf5_memory__freePGDResults(struct h5_pgdResults_struct *pgd);

int hdf5_setFileName(const char *adir,
                     const char *evid, 
                     char fname[PATH_MAX]);

int hdf5_update__cmt(const char *adir,
                     const char *evid,
                     const int h5k,
                     struct GFAST_offsetData_struct cmt_data,
                     struct GFAST_cmtResults_struct cmt);
int hdf5_update__ff(const char *adir,
                    const char *evid,
                    const int h5k,
                    struct GFAST_offsetData_struct ff_data,
                    struct GFAST_ffResults_struct ff);
int hdf5_update__hypocenter(const char *adir,
                            const char *evid,
                            const int h5k,
                            struct GFAST_shakeAlert_struct hypo);
int hdf5_update__getIteration(const char *adir,
                              const char *evid,
                              const double epoch);
int hdf5_update__pgd(const char *adir,
                     const char *evid,
                     const int h5k,
                     struct GFAST_peakDisplacementData_struct pgd_data,
                     struct GFAST_pgdResults_struct pgd);



//----------------------------------------------------------------------------//
//                            hdf5 convenience functions                      //
//----------------------------------------------------------------------------//
hid_t h5_open_rdonly(const char *flname);
hid_t h5_open_rdwt(const char *flname);
int h5_close(const hid_t file_id);
int h5_write_array__float(const char *dset_name, const hid_t file_id,
                          const int n, const float *x);
int h5_write_array__double(const char *dset_name, const hid_t file_id,
                           const int n, const double *x);
int h5_write_array__int(const char *dset_name, const hid_t file_id,
                        const int n, const int *x);
int h5_write_array__chars(const char *citem_chr, const hid_t file_id,
                          const int n, const char **c);
int h5_read_array__double(const char *dset_name, const hid_t file_id,
                          const int nref, double *x);
int h5_read_array__float(const char *dset_name, const hid_t file_id,
                         const int nref, float *x);
int h5_read_array__int(const char *dset_name, const hid_t file_id,
                       const int nref, int *x);
int h5_write_attribute__double(const char *citem, const hid_t hdf5_id,
                               const int n, const double *attr_data);
int h5_write_attribute__int(const char *citem, const hid_t hdf5_id,
                            const int n, const int *attr_data);
int h5_write_attribute__char(const char *citem, const hid_t hdf5_id,
                             const int n, const char **cattr);
int h5_n_group_members(const char *group_name, const hid_t file_id);
int h5_get_array_size(const hid_t file_id, const char *citem);
bool h5_item_exists(const hid_t file_id, const char *citem_in);
hid_t h5_create_group(const hid_t file_id, const char *cgroup);

#define GFAST_hdf5_setFileName(...)       \
              hdf5_setFileName(__VA_ARGS__)

#define GFAST_hdf5_copy__cmtResults(...)       \
              hdf5_copy__cmtResults(__VA_ARGS__)
#define GFAST_hdf5_copy__faultPlane(...)       \
              hdf5_copy__faultPlane(__VA_ARGS__)
#define GFAST_hdf5_copy__ffResults(...)       \
              hdf5_copy__ffResults(__VA_ARGS__)
#define GFAST_hdf5_copy__hypocenter(...)       \
              hdf5_copy__hypocenter(__VA_ARGS__)
#define GFAST_hdf5_copy__offsetData(...)       \
              hdf5_copy__offsetData(__VA_ARGS__)
#define GFAST_hdf5_copy__peakDisplacementData(...)       \
              hdf5_copy__peakDisplacementData(__VA_ARGS__)
#define GFAST_hdf5_copy__pgdResults(...)       \
              hdf5_copy__pgdResults(__VA_ARGS__)

#define GFAST_hdf5_createType__cmtResults(...)       \
              hdf5_createType__cmtResults(__VA_ARGS__)
#define GFAST_hdf5_createType__faultPlane(...)       \
              hdf5_createType__faultPlane(__VA_ARGS__)
#define GFAST_hdf5_createType__ffResults(...)       \
              hdf5_createType__ffResults(__VA_ARGS__)
#define GFAST_hdf5_createType__hypocenter(...)       \
              hdf5_createType__hypocenter(__VA_ARGS__)
#define GFAST_hdf5_createType__offsetData(...)       \
              hdf5_createType__offsetData(__VA_ARGS__)
#define GFAST_hdf5_createType__peakDisplacementData(...)       \
              hdf5_createType__peakDisplacementData(__VA_ARGS__)
#define GFAST_hdf5_createType__pgdResults(...)       \
              hdf5_createType__pgdResults(__VA_ARGS__)

#define GFAST_hdf5_initialize(...)       \
              hdf5_initialize(__VA_ARGS__)
#define GFAST_hdf5_memory__freeCMTResults(...)       \
              hdf5_memory__freeCMTResults(__VA_ARGS__)
#define GFAST_hdf5_memory__freePGDData(...)       \
              hdf5_memory__freePGDData(__VA_ARGS__)
#define GFAST_hdf5_memory__freePGDResults(...)       \
              hdf5_memory__freePGDResults(__VA_ARGS__)
#define GFAST_hdf5_memory__freeFaultPlane(...)       \
              hdf5_memory__freeFaultPlane(__VA_ARGS__)
#define GFAST_hdf5_memory__freeFFResults(...)       \
              hdf5_memory__freeFFResults(__VA_ARGS__)
#define GFAST_hdf5_memory__freeOffsetData(...)       \
              hdf5_memory__freeOffsetData(__VA_ARGS__)
#define GFAST_hdf5_update__cmt(...)       \
              hdf5_update__cmt(__VA_ARGS__)
#define GFAST_hdf5_update__ff(...)       \
              hdf5_update__ff(__VA_ARGS__)
#define GFAST_hdf5_update__getIteration(...)            \
              hdf5_update__getIteration(__VA_ARGS__)
#define GFAST_hdf5_update__hypocenter(...)       \
              hdf5_update__hypocenter(__VA_ARGS__)
#define GFAST_hdf5_update__pgd(...)            \
              hdf5_update__pgd(__VA_ARGS__)



#ifdef __cplusplus
}
#endif
#endif /* __GFAST_HDF5_H__ */