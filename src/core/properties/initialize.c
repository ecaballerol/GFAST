#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iniparser.h>
#include "gfast_core.h"
#include "iscl/os/os.h"
#include "iscl/log/log.h"
/*!
 * @brief Initializes the GFAST properties (parameter) structure
 *
 * @param[in] propfilename   name of properties file
 * @param[in] opmode         GFAST operational mode
 *
 * @param[out] props         on successful exit holds the GFAST properties
 *                           structure
 *
 * @author Brendan Crowell (crowellb@uw.edu) and Ben Baker (benbaker@isti.com)
 *
 * @result 0 indicates success
 *
 */
int core_properties_initialize(const char *propfilename,
                               const enum opmode_type opmode,
                               struct GFAST_props_struct *props)
{
    const char *fcnm = "core_properties_initialize\0";
    const char *s;
    int ierr; 
    dictionary *ini;
    //------------------------------------------------------------------------//
    // Require the properties file exists
    ierr =-1;
    memset(props, 0, sizeof(struct GFAST_props_struct));
    props->opmode = opmode;
    if (!os_path_isfile(propfilename))
    {
        log_errorF("%s: Properties file: %s does not exist\n",
                   fcnm, propfilename);
        return ierr;
    }
    // Load the ini file
    ierr = 1;
    ini = iniparser_load(propfilename);
    //-------------------------GFAST General Parameters-----------------------//
    s = iniparser_getstring(ini, "general:sitefile\0", "GFAST_streams.txt\0");
    strcpy(props->sitefile, s);
    if (!os_path_isfile(props->sitefile))
    {
        log_errorF("%s: Cannot find station list %s\n", fcnm, props->sitefile);
        return -1;
    }
    props->bufflen = iniparser_getdouble(ini, "general:bufflen\0", 1800.0);
    if (props->bufflen <= 0.0)
    {
        log_errorF("%s: Buffer lengths must be positive!\n", fcnm);
        goto ERROR;
    }
    if (props->opmode == OFFLINE)
    {
        s = iniparser_getstring(ini, "general:eewsfile\0", NULL);
        if (s == NULL)
        {
            log_errorF("%s: Could not find decision module XML file!\n", fcnm);
            goto ERROR;
        }
        strcpy(props->eewsfile, s);
        s = iniparser_getstring(ini, "general:synthetic_data_directory\0",
                                "./"); 
        if (strlen(s) > 0)
        {
            strcpy(props->syndata_dir, s);
            if (!os_path_isdir(props->syndata_dir))
            {
                log_errorF("%s: Synthetic data directory %s doesn't exist\n",
                           fcnm, props->syndata_dir);
                goto ERROR;
            }
        }
        else
        {
            strcpy(props->syndata_dir, "./\0");
        }
        s = iniparser_getstring(ini, "general:synthetic_data_prefix\0", "LX\0");
        if (strlen(s) > 0){strcpy(props->syndata_pre, s);}
    }
    // UTM zone
    props->utm_zone = iniparser_getint(ini, "general:utm_zone\0", -12345);
    if (props->utm_zone < 0 || props->utm_zone > 60)
    {
        if (props->utm_zone !=-12345)
        {
            log_warnF("%s: UTM zone %d is invalid estimating from hypocenter\n",
                   fcnm, props->utm_zone);
            props->utm_zone =-12345;
        } 
    }
    // Verbosity
    props->verbose = iniparser_getint(ini, "general:verbose\0", 2);
    // Sampling period
    props->dt_default = iniparser_getdouble(ini, "general:dt_default\0", 1.0);
    if (props->dt_default <= 0.0)
    {
        log_warnF("%s: Default sampling period %f invalid; defaulting to %f!\n",
                  fcnm, props->dt_init, 1.0);
        props->dt_default = 1.0; 
    }
    props->dt_init = iniparser_getint(ini, "general:dt_init\0", 3);
    if (props->opmode != OFFLINE)
    {
        if (props->dt_init != INIT_DT_FROM_TRACEBUF){
            log_warnF("%s: Obtaining sampling period from tracebuf\n", fcnm);
            props->dt_init = 3;
        }
    }
    if (props->opmode == OFFLINE)
    {
        // Make sure the EEW XML file exists
        if (!os_path_isfile(props->eewsfile))
        {
            log_errorF("%s: Cannot find EEW XML file!\n",
                       fcnm, props->eewsfile);
            goto ERROR;
        }
        // Figure out how to initialize sampling period
        if (props->dt_init == INIT_DT_FROM_FILE)
        {
            props->dt_init = iniparser_getint(ini, "general:dt_init\0", 1);
            if (s == NULL)
            {
                log_errorF("%s: Must specify sitefile!\n", fcnm);
                goto ERROR; 
            }
        }
        else if (props->dt_init == INIT_DT_FROM_SAC)
        {
            props->dt_init = INIT_DT_FROM_SAC;
        }
        else
        {
            if (props->dt_init != INIT_DT_FROM_DEFAULT)
            {
                log_warnF("%s: Setting dt from default\n", fcnm);
                props->dt_init = INIT_DT_FROM_DEFAULT;
            }
        }
    }
    else
    {
        if (props->dt_init != INIT_DT_FROM_TRACEBUF)
        {
            log_warnF("%s: Will get GPS sampling period from trace buffer!\n",
                      fcnm);
            props->dt_init = INIT_DT_FROM_TRACEBUF;
        }
    }
    // Wait time
    props->waitTime = 1.0;
    if (props->opmode == REAL_TIME_EEW || props->opmode == REAL_TIME_PTWC)
    {
        props->waitTime = iniparser_getdouble(ini, "general:waitTime\0", 1.0);
        if (props->waitTime < 0.0)
        {
            log_errorF("%s: Invalid wait time %f!\n", fcnm, props->waitTime);
            goto ERROR;
        } 
    }        
    // Location initialization
    props->loc_init = iniparser_getint(ini, "general:loc_init\0", 1);
    if (props->opmode == OFFLINE)
    {
        if (props->loc_init == INIT_LOCS_FROM_TRACEBUF)
        {
            log_errorF("%s: offline can't initialize locations from tracebuf\n",
                       fcnm);
            goto ERROR;
        }
    }
    // Synthetic runtime
    if (props->opmode == OFFLINE)
    {
        props->synthetic_runtime
           = iniparser_getdouble(ini, "general:synthetic_runtime\0", 0.0); 
    } 
    // Processing time
    props->processingTime
       = iniparser_getdouble(ini, "general:processing_time\0", 300.0);
    if (props->processingTime > props->bufflen)
    {
        log_errorF("%s: Error processing time cannot exceed buffer length\n",
                   fcnm);
        goto ERROR;
    }
    // Default earthquake depth
    props->eqDefaultDepth
        = iniparser_getdouble(ini, "general:default_event_depth\0", 8.0);
    if (props->eqDefaultDepth < 0.0)
    {
        log_errorF("%s: Error default earthquake depth must be positive %f\n",
                   fcnm, props->eqDefaultDepth);
        goto ERROR;
    }
    // H5 archive directory
    s = iniparser_getstring(ini, "general:h5ArchiveDirectory\0", NULL);
    if (s == NULL)
    {
        strcpy(props->h5ArchiveDir, "./\0");
    }
    else
    {
        strcpy(props->h5ArchiveDir, s);
        if (!os_path_isdir(props->h5ArchiveDir))
        {
            log_warnF("%s: Archive directory %s doesn't exist\n",
                       fcnm, props->h5ArchiveDir);
            log_warnF("%s: Will use current working directory\n", fcnm);
            memset(props->h5ArchiveDir, 0, sizeof(props->h5ArchiveDir));
            strcpy(props->h5ArchiveDir, "./\0");
        }
    }
    //------------------------------PGD Parameters----------------------------//
    props->pgd_props.verbose = props->verbose;
    props->pgd_props.utm_zone = props->utm_zone;
    props->pgd_props.dist_tol = iniparser_getdouble(ini, "PGD:dist_tolerance\0",
                                                    6.0);
    if (props->pgd_props.dist_tol < 0.0)
    {
        log_errorF("%s: Error ndistance tolerance %f cannot be negative\n",
                   fcnm, props->pgd_props.dist_tol);
        goto ERROR;
    }
    props->pgd_props.disp_def = iniparser_getdouble(ini, "PGD:disp_default\0",
                                                    0.01);
    if (props->pgd_props.disp_def <= 0.0)
    {
        log_errorF("%s: Error PGD distance default %f must be positive\n",
                   fcnm, props->pgd_props.disp_def);
        goto ERROR;
    }
    props->pgd_props.ngridSearch_deps
         = iniparser_getint(ini, "PGD:ndepths_in_pgd_gridSearch\0", 100);
    if (props->pgd_props.ngridSearch_deps < 1){
        log_errorF("%s: Error PGD grid search depths %d must be positive\n",
                   fcnm, props->pgd_props.ngridSearch_deps);
        goto ERROR;
    } 
    props->pgd_props.window_vel = iniparser_getdouble(ini,
                                                "PGD:pgd_window_vel\0", 3.0);
    if (props->pgd_props.window_vel <= 0.0){
        log_errorF("%s: Error window velocity must be positive!\n", fcnm);
        goto ERROR;
    }
    props->pgd_props.min_sites = iniparser_getint(ini, "PGD:pgd_min_sites\0",
                                                  4);
    if (props->pgd_props.min_sites < 1){
        log_errorF("%s: Error at least one site needed to estimate PGD!\n", 
                   fcnm);
        goto ERROR;
    }
    //----------------------------CMT Parameters------------------------------//
    props->cmt_props.verbose = props->verbose;
    props->cmt_props.utm_zone = props->utm_zone;
    props->cmt_props.ngridSearch_deps
         = iniparser_getint(ini, "CMT:ndepths_in_cmt_gridSearch\0", 100);
    if (props->cmt_props.ngridSearch_deps < 1)
    {
        log_errorF("%s: Error CMT grid search depths %d must be positive\n",
                   fcnm, props->cmt_props.ngridSearch_deps);
        goto ERROR;
    }
    props->cmt_props.min_sites
        = iniparser_getint(ini, "CMT:cmt_min_sites\0", 4);
    if (props->cmt_props.min_sites < 3)
    {
        log_errorF("%s: Error at least two sites needed to estimate CMT!\n",
                   fcnm);
        goto ERROR;
    }
    props->cmt_props.window_vel
        = iniparser_getdouble(ini, "CMT:cmt_window_vel\0", 2.0);
    if (props->cmt_props.window_vel <= 0.0)
    {
        log_errorF("%s: Error window velocity must be positive!\n", fcnm);
        goto ERROR;
    }
    props->cmt_props.window_avg
        = iniparser_getdouble(ini, "CMT:cmt_window_avg\0", 0.0);
    if (props->cmt_props.window_avg < 0.0)
    {   
        log_errorF("%s: Error window average time must be positive!\n", fcnm);
        goto ERROR;
    }
    props->cmt_props.ldeviatoric
        = iniparser_getboolean(ini, "CMT:ldeviatoric_cmt\0", true);
    if (!props->cmt_props.ldeviatoric)
    {
        log_errorF("%s: Error general CMT inversions not yet programmed\n",
                   fcnm);
        goto ERROR;
    }
    //------------------------------FF Parameters-----------------------------//
    props->ff_props.verbose = props->verbose;
    props->ff_props.utm_zone = props->utm_zone;
    props->ff_props.nfp
        = iniparser_getint(ini, "FF:ff_number_of_faultplanes\0", 2);
    if (props->ff_props.nfp != 2)
    {
        log_errorF("%s: Error only 2 fault planes considered in ff\n", fcnm);
        goto ERROR;
    }
    props->ff_props.nstr = iniparser_getint(ini, "FF:ff_nstr\0", 10);
    if (props->ff_props.nstr < 1)
    {
        log_errorF("%s: Error no fault patches along strike!\n", fcnm);
        goto ERROR;
    }
    props->ff_props.ndip = iniparser_getint(ini, "FF:ff_ndip\0", 5);
    if (props->ff_props.ndip < 1)
    {
        log_errorF("%s: Error no fault patches down dip!\n", fcnm);
        goto ERROR;
    }
    props->ff_props.min_sites = iniparser_getint(ini, "FF:ff_min_sites\0", 4);
    if (props->ff_props.min_sites < props->cmt_props.min_sites)
    {
        log_errorF("%s: Error FF needs at least as many sites as CMT\n", fcnm);
        goto ERROR;
    }
    props->ff_props.window_vel
        = iniparser_getdouble(ini, "FF:ff_window_vel\0", 3.0);
    if (props->ff_props.window_vel <= 0.0)
    {
        log_errorF("%s: Error window velocity must be positive!\n", fcnm);
        goto ERROR;
    }
    props->ff_props.window_avg
        = iniparser_getdouble(ini, "FF:ff_window_avg\0", 10.0);
    if (props->ff_props.window_avg <= 0.0)
    {
        log_errorF("%s: Error window average time must be positive!\n", fcnm);
        goto ERROR;
    }
    props->ff_props.flen_pct
        = iniparser_getdouble(ini, "FF:ff_flen_pct\0", 10.0);
    if (props->ff_props.flen_pct < 0.0)
    {
        log_errorF("%s: Error cannot shrink fault length\n", fcnm);
        goto ERROR;
    }
    props->ff_props.fwid_pct
        = iniparser_getdouble(ini, "FF:ff_fwid_pct\0", 10.0);
    if (props->ff_props.fwid_pct < 0.0)
    {
        log_errorF("%s: Error cannot shrink fault width\n", fcnm);
        goto ERROR;
    }
    //---------------------------ActiveMQ Parameters--------------------------//
    printf("%d\n", props->opmode);
    if (props->opmode == REAL_TIME_EEW) 
    {
        s = iniparser_getstring(ini, "ActiveMQ:host\0", NULL);
        if (s == NULL)
        {
            log_errorF("%s: Could not find ActiveMQ host!\n", fcnm);
            goto ERROR;
        }
        else
        {
            strcpy(props->activeMQ_props.host, s);
        }
        s = iniparser_getstring(ini, "ActiveMQ:user\0", NULL);
        if (s == NULL)
        {
            log_errorF("%s: Could not find ActiveMQ user!\n", fcnm);
            goto ERROR;
        }
        else
        {
            strcpy(props->activeMQ_props.user, s);
        }
        s = iniparser_getstring(ini, "ActiveMQ:password\0", NULL);
        if (s == NULL)
        {
            log_errorF("%s: Could not find password!\n", fcnm);
            goto ERROR;
        }
        else
        {
             strcpy(props->activeMQ_props.password, s);
        }
        s = iniparser_getstring(ini, "ActiveMQ:originTopic\0", NULL);
        if (s == NULL)
        {
            log_errorF("%s: Could not find activeMQ originTopic\n", fcnm);
            goto ERROR;
        }
        else
        {
            strcpy(props->activeMQ_props.originTopic, s);
        }
        s = iniparser_getstring(ini, "ActiveMQ:destinationTopic\0", NULL);
        if (s == NULL)
        {
            log_errorF("%s: Could not find ActiveMQ destinationTopic!\n", fcnm);
            goto ERROR;
        }
        else
        {
            strcpy(props->activeMQ_props.destinationTopic, s); 
        }
        props->activeMQ_props.port = iniparser_getint(ini, "ActiveMQ:port\0",
                                                      -12345);
        if (props->activeMQ_props.port ==-12345)
        {
            log_errorF("%s: Could not find activeMQ port\n", fcnm);
            goto ERROR;
        }
        props->activeMQ_props.msReconnect
             = iniparser_getint(ini, "ActiveMQ:msReconnect\0", 500);
        if (props->activeMQ_props.msReconnect < 0)
        {
            log_warnF("%s: Overriding msReconnect to 500\n", fcnm);
            props->activeMQ_props.msReconnect = 500;
        }
        props->activeMQ_props.maxAttempts
             = iniparser_getint(ini, "ActiveMQ:maxAttempts\0", 5);
        if (props->activeMQ_props.maxAttempts < 0)
        {
            log_warnF("%s: Overriding maxAttempts to 5\n", fcnm);
            props->activeMQ_props.maxAttempts = 5;
        }
        props->activeMQ_props.msWaitForMessage 
             = iniparser_getint(ini, "ActiveMQ:msWaitForMessage\0", 1);
        if (props->activeMQ_props.msWaitForMessage < 0)
        {
            log_warnF("%s: ActiveMQ could hang indefinitely, overriding to 1\n",
                      fcnm);
            props->activeMQ_props.msWaitForMessage = 1;
        }
    } // End check on need for ActiveMQ
    //----------------------------RabbitMQ Parameters-------------------------//
/*
    s = iniparser_getstring(ini, "RabbitMQ:RMQhost\0", NULL);
    if (s == NULL){
        log_errorF("%s: Could not find RMQhost!\n", fcnm);
        goto ERROR;
    }else{
        strcpy(props->RMQhost, s); 
    }   
    props->RMQport = iniparser_getint(ini, "RabbitMQ:RMQport\0", -12345);
    if (props->RMQport ==-12345){
        log_errorF("%s: Could not find RMQport\n", fcnm);
        goto ERROR;
    }   
    s = iniparser_getstring(ini, "RabbitMQ:RMQtopic\0", NULL);
    if (s == NULL){
        log_errorF("%s: Could not find RMQtopic!\n", fcnm);
        goto ERROR;
    }else{
        strcpy(props->RMQtopic, s); 
    }   
    s = iniparser_getstring(ini, "RabbitMQ:RMQuser\0", NULL);
    if (s == NULL){
        log_errorF("%s: Could not find RMQuser!\n", fcnm);
        goto ERROR;
    }else{
        strcpy(props->RMQuser, s); 
    }   
    s = iniparser_getstring(ini, "RabbitMQ:RMQpassword\0", NULL);
    if (s == NULL){
        log_errorF("%s: Could not find RMQpassword!\n", fcnm);
        goto ERROR;
    }else{
        strcpy(props->RMQpassword, s); 
    }
    s = iniparser_getstring(ini, "RabbitMQ:RMQexchange\0", NULL);
    if (s == NULL){
        log_errorF("%s: Could not find RMQexchange!\n", fcnm);
        goto ERROR;
    }else{
        strcpy(props->RMQexchange, s);
    }
*/
    // Success!
    ierr = 0;
ERROR:;
    // Free the ini file
    iniparser_freedict(ini);
    return ierr;
}
