#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gfast_core.h"
#include "iscl/log/log.h"
#include "iscl/memory/memory.h"
#include "iscl/os/os.h"

/*!
 * @brief Initializes the GPS data streams
 *
 * @param[in] props      holds the GFAST properties
 *
 * @param[out] gps_data  this should have been pre-initialized to NULL.
 *                       on successful output holds the GPS data structure,
 *                       metadata such (station SNCL's, locations, instrument
 *                       sampling period) and requisite memory.
 *
 * @result 0 indicates success
 *
 * @author Ben Baker (ISTI)
 *
 */
int core_data_initialize(struct GFAST_props_struct props,
                         struct GFAST_data_struct *gps_data)
{
    const char *fcnm = "core_data_initialize\0";
    int ierr, k, mpts;
    // Get the sites to be used
    ierr = 0;
    if (props.verbose > 0){log_infoF("%s: Initializing metadata...\n", fcnm);}
    ierr = core_data_readSiteFile(props.sitefile, gps_data);
    if (ierr != 0)
    {
        log_errorF("%s: Error reading sites file!\n", fcnm);
        return -1;
    }
    // Set the buffer lengths
    for (k=0; k<gps_data->stream_length; k++)
    {
        mpts = (int) (props.bufflen/gps_data->data[k].dt + 0.5) + 1;
        gps_data->data[k].maxpts = mpts;
        gps_data->data[k].ubuff = ISCL_memory_calloc__double(mpts);
        gps_data->data[k].nbuff = ISCL_memory_calloc__double(mpts);
        gps_data->data[k].ebuff = ISCL_memory_calloc__double(mpts);
    }
    return 0;
}
