#include <stdbool.h>
#include <limits.h>
#include "gfast_enum.h"

#ifndef __GFAST_STRUCT_H__
#define __GFAST_STRUCT_H__

struct GFAST_props_struct
{
    char streamfile[PATH_MAX];  /*!< File of streams to include in the
                                     processing. */
    char dtfile[PATH_MAX];      /*!< File with site names and sampling periods
                                     if initializing from file */
    char siteposfile[PATH_MAX]; /*!< File of all the site locations.  The
                                     format used is currently from the SOPAC
                                     SECTOR web service. */
    char eewsfile[PATH_MAX];    /*!< File to output the results of the ActiveMQ
                                     listener from the seismic warning file. */
    char eewgfile[PATH_MAX];    /*!< File to output the results of G-FAST */
    char syndriver[PATH_MAX];   /*!< The synthetic mode driver file. */
    char synoutput[PATH_MAX];   /*!< The synthetic mode output file. */
    char syndata_dir[PATH_MAX]; /*!< Synthetic data directory */
    char syndata_pre[PATH_MAX]; /*!< Synthetic data prefix (LX) */
    char AMQhost[512];          /*!< ActiveMQ hostname to access ElarmS messages
                                     (siren). */
    char AMQtopic[512];         /*!< ActiveMQ topic to access ElarmS messages
                                     (eew.alg.elarms.data). */
    char AMQuser[512];          /*!< ActiveMQ username to access ElarmS
                                     messages */
    char AMQpassword[512];      /*!< ActiveMQ password to access ElarmS
                                     messages */
    char RMQhost[512];          /*!< RabbitMQ hostname to acceess processed GPS
                                     positions.  PANGA separates this into two 
                                     distinct processing groups, CWU-ppp and
                                     UNAVCO-rtx */
    char RMQtopic[512];         /*!< RabbitMQ topic to access processed GPS
                                     positions (www.panga.org) */
    char RMQuser[512];          /*!< RabbitMQ username to acess processed GPS
                                     positions */
    char RMQpassword[512];      /*!< RabbitMQ password to access processed GPS
                                     positions */
    char RMQexchange[512];      /*!< RabbitMQ exchange to access processed GPS
                                     positions (nev-cor) */
    double dt_default;          /*!< Default sampling period (s) for GPS
                                     stations */
    double pgd_dist_tol;        /*!< PGD source station distance
                                     tolerance (km) */
    double pgd_dist_def;        /*!< PGD default station distance (km) if
                                     d < pgd_dist_tol */
    double bufflen;             /*!< The number of seconds to keep in the data
                                     buffers */
    double processingTime;      /*!< Max processing time (s) after origin time
                                     which GFAST module will declare an event 
                                     done. This must be less than bufflen
                                     less than or equal to the synthetic
                                     runtime */
    double eqDefaultDepth;      /*!< Default earthquake depth (km) to be applied
                                     to shakeAlert structure */
    double synthetic_runtime;   /*!< Simulation runtime (s) for offline mode */
    double pgd_window_vel;      /*!< Velocity used in determining if enough
                                     data has arrived at a station in PGD
                                     inversion */
    double cmt_window_vel;      /*!< Velocity used in determining if enough
                                     data has arrived at a station in CMT
                                     inversion */
    double cmt_window_avg;      /*!< Window length (s) over which data positions
                                     are averaged in CMT inversion */
    double ff_window_avg;       /*!< Window length (s) over which data positions
                                     are averaged in FF inversion */
    double ff_window_vel;       /*!< Velocity used in determining if enough
                                     data has arrived at a station in CMT
                                     inversion */
    double ff_flen_pct;         /*!< Fault length safety factor */
    double ff_fwid_pct;         /*!< Fault width safety factor */
    int AMQport;                /*!< ActiveMQ port to access ElarmS messages 
                                    (61620). */
    int RMQport;                /*!< RabbitMQ port to access processed GPS
                                     positions (5672) */
    int utm_zone;               /*!< UTM zone.  If this is -12345 then will 
                                     extract the UTM zone from the event
                                     origin. */
    int pgd_min_sites;          /*!< Minimum number of sites to proceed with
                                     PGD estimation */
    int cmt_min_sites;          /*!< Minimum number of sites to proceed with
                                     CMT estimation */
    int ff_min_sites;           /*!< Minimum number of sites to proceed with
                                     FF estimation */
    int ff_nstr;                /*!< Number of fault patches along strike */
    int ff_ndip;                /*!< Number of fault patches down dip */
    int ff_nfp;                 /*!< Number of fault planes considered in
                                     inversion */
    int pgd_ngridSearch_deps;   /*!< Number of depths in PGD grid-search */
    int cmt_ngridSearch_deps;   /*!< Number of depths in CMT grid-search */
    int verbose;                /*!< Controls verbosity - errors will always
                                     be output.
                                      = 1 -> Output generic information
                                      = 2 -> Output generic information and
                                             warnings 
                                      = 3 -> Output generic information,
                                             warnings, and debug information
                                             and debug information. */
    bool lremove_disp0;         /*!< Remove the (u,n,e) component at the 
                                     origin time from the displacement
                                     streams */
    bool ldeviatoric_cmt;       /*!< If true then the CMT inversion is 
                                     constrained to purely deviatoric sources.
                                     Otherwise, all 6 moment tensor terms
                                     are inverted for. */
    enum opmode_type opmode;    /*!< GFAST operation mode (realtime, 
                                     playback, offline) */
    enum dtinit_type dt_init;   /*!< Defines how to initialize GPS sampling
                                     period */
    enum locinit_type loc_init; /*!< Defines how to initialize GPS locations */
};


struct GFAST_faultPlane_struct
{
    double *fault_lon;  /*!< Fault patch longitudes (degrees).  The size is
                             [nstr x ndip] with leading dimension nstr */
    double *fault_lat;  /*!< Fault patch latitudes (degrees).  The size is
                             [nstr x ndip] with leading dimension nstr */
    double *fault_xutm; /*!< Fault easting UTM (m).  The size is
                             [nstr x ndip] with leading dimension nstr */
    double *fault_yutm; /*!< Fault northing UTM (m).  The size is
                             [nstr x ndip] with leading dimension nstr */
    double *fault_alt;  /*!< Depth at fault patch (km) The size is 
                             [nstr x ndip] with leading dimension nstr */
    double *strike;     /*!< Strike angles of fault patches (degrees)
                             [nstr x ndip] with leading dimension nstr */
    double *dip;        /*!< Dip angles of fault patches (degrees).  The size
                             is [nstr x ndip] with leading dimension nstr */
    double *length;     /*!< Length of fault patches (m).  The size is
                             [nstr x ndip] with leading dimension nstr */
    double *width;      /*!< Width of fault patches (m).  The size is 
                             [nstr x ndip] with leading dimension nstr */
    double *sslip;      /*!< Strike-slip along each fault patch.  The size
                             is [nstr x ndip] with leading dimension nstr */
    double *dslip;      /*!< Dip-slip along each fault patch.  The size
                             is [nstr x ndip] with leading dimension nstr */
    double *EN;         /*!< Modeled east displacements [nobs] */
    double *NN;         /*!< Modeled north displacements [nobs] */
    double *UN;         /*!< Modeled vertical displacements [nobs] */
    double *Einp;       /*!< Observed input east displacements [nobs] */
    double *Ninp;       /*!< Observed input north displacements [nobs] */
    double *Uinp;       /*!< Observed input vertical displacements [nobs] */
    int maxobs;         /*!< Max number of allowable observations */
    int nobs;           /*!< Number of observations (3 x number_of_sites) */
    int nstr;           /*!< Number of fault patches along strike */
    int ndip;           /*!< Number of fault patches down dip */
};

struct GFAST_ffResults_struct
{
    struct GFAST_faultPlane_struct *fp;   /*!< Fault planes [nfp] */
    double *vr;                           /*!< Variance reduction on ifp'th
                                               fault plane [nfp] */
    double *Mw;                           /*!< Moment magnitude on ifp'th
                                               fault plane [nfp] */
    double *str;                          /*!< Strike of ifp'th fault
                                               (degrees) [nfp] */
    double *dip;                          /*!< Dip on ifp'th fault plane 
                                               (degrees) [nfp] */
    double SA_lat;                        /*!< Source latitude (degrees) */
    double SA_lon;                        /*!< Source longitude (degrees) */
    double SA_dep;                        /*!< Source depth (km) */
    double SA_mag;                        /*!< Source magnitude (Mw) */
    int preferred_fault_plane;            /*!< Preferred fault plane */
    int nfp;                              /*!< Number of fault planes */
};

struct GFAST_cmtResults_struct
{
    double *objfn;     /*!< Objective function at all depths */
    double *mts;       /*!< Holds the NED moment tensor terms packed
                            \f$ \{m_{xx}, m_{yy}, m_{zz},
                                  m_{xy}, m_{xz}, m_{yz} \} \f$.  [6*ndeps] */
    double *str1;      /*!< Strike on first nodal plane for
                            all depths [ndeps] */
    double *str2;      /*!< Strike on second nodal plane for
                            all depths [ndeps] */
    double *dip1;      /*!< Dip on first nodal plane for all depths [ndeps] */
    double *dip2;      /*!< Dip on second nodal plane for all depths [ndeps] */
    double *rak1;      /*!< Rake on first nodal plane for all depths [ndeps] */
    double *rak2;      /*!< Rake on second nodal plane for all depths [ndeps] */
    double *Mw;        /*!< Moment magnitude for all depths [ndeps] */
    double *srcDepths; /*!< Source depths in moment tensor inversion grid
                            search (km) [ndeps] */
    int opt_indx;      /*!< Optimal index in depth grid search [0, ndeps) */
    int ndeps;         /*!< Number of depths in grid search */
};

struct GFAST_pgdResults_struct
{
    double *mpgd;      /*!< PGD magnitude at id'th depth [ndeps] */
    double *mpgd_vr;   /*!< PGD variance reduction at id'th depth [ndeps] */
    double *srcDepths; /*!< PGD source depths in grid search (km) [ndeps] */
    bool *lsiteUsed;   /*!< If true then then the isite'th from the 
                            site list was used in the PGD estimation [nsites] */
    int ndeps;         /*!< Number of depths in PGD estimation */ 
    int nsites;        /*!< Should equal GFAST_data_struct's stream_length */
};

struct GFAST_strongMotion_struct
{
    double *z;      /*!< Vertical channel [npts] */
    double *n;      /*!< North (radial, 2) channel [npts] */
    double *e;      /*!< East (transverse, 3) channel [npts] */
    int npts;       /*!< Number of points */
    double dt;      /*!< Sampling period (s) */
};

struct GFAST_collocatedData_struct
{
    struct GFAST_strongMotion_struct sm;  /*!< Collocated strong motion data
                                               structure */ 
    char site[64];    /*!< Name of site */
    double *ubuff;    /*!< Up position buffer (m?).  If any sample is 
                           not known it should be a NAN. [maxpts] */
    double *nbuff;    /*!< North position buffer (m?) [maxpts].  If any sample
                           is not known it should be a NAN. */
    double *ebuff;    /*!< East position buffer (m?) [maxpts].  If any sample
                           is not known it should be a NAN. */
    double *tbuff;    /*!< Epochal time buffer (s) [maxpts] */ 
    double epoch;     /*!< Epoch time (seconds) corresponding to first sample 
                           of u, n, and e traces */
    double dt;        /*!< Sampling period (seconds). */
    double sta_lat;   /*!< Station latitude [-90,90] (degrees) */
    double sta_lon;   /*!< Station longitude [0,360] (degrees) */
    double sta_alt;   /*!< Station altitude (m) */
    int maxpts;       /*!< Max number of poitns in buffer.  This is 
                           computed from the site sampling period and
                           the GFAST_parm_struct's bufflen */
    int npts;         /*!< Number of points in time series.  This cannot
                           exceed maxpts */
    bool lcollocated; /*!< True -> station is collocated with a strong
                                   motion station.
                           False -> station is not collocated with a strong
                                   motion station. */
    bool lskip_pgd;   /*!< True  -> This site is ignored during the PGD
                                    estimation.
                           False -> Will  attempt to use this site during
                                    the PGD estimation. */
    bool lskip_cmt;   /*!< True  -> this site is ignored during the CMT 
                                    inversion. 
                           False -> Will attempt to use this site during the
                                    CMT inversion */
    bool lskip_ff;    /*!< True  -> this site is ignored during the finite
                                    fault inversion
                           False -> Will attempt to use this during the 
                                    finite fault inversion */
};

struct GFAST_data_struct
{
    struct GFAST_collocatedData_struct *data;  /*!< Collocated data structure
                                                    [stream_length] */
    int stream_length;                         /*!< Number of streams */
};

struct GFAST_shakeAlert_struct
{
    char eventid[128];  /*!< Event ID */
    double lat;         /*!< Event latitude (degrees) */
    double lon;         /*!< Event longitude (degrees) */
    double dep;         /*!< Event depth (kilometers) */ 
    double mag;         /*!< Event magnitude */
    double time;        /*!< Event epochal time (s) */
};

struct GFAST_activeEvents_struct
{
    struct GFAST_shakeAlert_struct *SA; /*<! Shake alert structure with 
                                             requisite event info [nev] */
    int nev;                            /*!< Number of events */ 
};

#endif /* __GFAST_STRUCT_H__ */
