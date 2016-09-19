#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hdf5.h>
#include "gfast_traceBuffer.h"
#include "iscl/log/log.h"

/*!
 * @brief Closes the HDF5 file on h5trace
 *
 * @param[inout] h5trace    on input holds the HDF5 file handle and
 *                          h5traceBuffer properties. 
 *                          on output h5tracebuffer information is reset
 *                          and the underlying HDF5 file with the traces
 *                          is closed.
 *
 * @result 0 indicates success
 * 
 */ 
int traceBuffer_h5_finalize(struct h5traceBuffer_struct *h5trace)
{
    const char *fcnm = "traceBuffer_h5_finalize\0";
    herr_t status;
    int i, ierr;
    ierr = 0;
    if (!h5trace->linit)
    {
        log_errorF("%s: Error h5trace never initialized\n", fcnm);
        ierr = 1;
        return ierr;
    }
    if (h5trace->ntraces > 0 && h5trace->traces != NULL)
    {
        for (i=0; i<h5trace->ntraces; i++)
        {
            if (h5trace->traces[i].groupName != NULL)
            {
                free(h5trace->traces[i].groupName);
            }
            if (h5trace->traces[i].buffer1 != NULL)
            {
                free(h5trace->traces[i].buffer1);
            }
            if (h5trace->traces[i].buffer2 != NULL)
            {
                free(h5trace->traces[i].buffer2);
            }
        }
        free(h5trace->traces);
    }
    status = H5Fclose(h5trace->fileID);
    if (status != 0)
    {
        log_errorF("%s: Error closing file\n", fcnm);
        ierr = 1; 
    }
    memset(h5trace, 0, sizeof(struct h5traceBuffer_struct));
    return ierr;
}