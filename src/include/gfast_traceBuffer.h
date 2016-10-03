#ifndef _gfast_tracebuffer_h__
#define _gfast_tracebuffer_h__ 1
#if defined WINNT || defined WIN32 || defined WIN64
#include <windows.h>
#include <limits.h>
#else
#include <linux/limits.h>
#endif
#include <stdbool.h>
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <hdf5.h>
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#include "gfast_struct.h"
#include "gfast_config.h"
#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#ifdef GFAST_USE_EW
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <transport.h>
#include <earthworm.h>
#include <trace_buf.h>
#ifdef __clang__
#pragma clang diagnostic pop
#endif
int WaveMsg2MakeLocal( TRACE2_HEADER* wvmsg );
struct ewRing_struct
{
    char ewRingName[512];  /*!< Earthworm ring name to which we will connect */
    SHM_INFO region;       /*!< Earthworm shared memory region corresponding to
                                the earthworm ring */
    MSG_LOGO *getLogo;     /*!< Logos to scrounge from the ring [nlogo] */
    long ringKey;          /*!< Ring key number */
    short nlogo;           /*!< Number of logos */
    bool linit;            /*!< True if the structure is initialized.
                                False if the structure is not initialized. */
    unsigned msWait;       /*!< microseconds to wait after reading ring */
    unsigned char
       traceBuffer2Type;   /*!< traceBuffer2type earthworm type */ 
    unsigned char
       heartBeatType;      /*!< earthworm heartbeat type */ 
    unsigned char
       errorType;          /*!< earthworm error type */
    unsigned char  
       instLocalID;        /*!< earthworm local instrument ID type */
    unsigned char
       instWildcardID;     /*!< instrument wildcard ID */
    unsigned char
       modWildcardID;      /*!< module wildcard ID */
};
#else
#ifndef MAX_TRACEBUF_SIZ
#define MAX_TRACEBUF_SIZ 4096
#endif
struct ewRing_struct
{
    bool linit;            /*!< Bogus value so that compilation proceeds */ 
};
#endif

struct h5trace_struct
{
    char netw[64];        /*!< Network name for forming earthworm requests */
    char stnm[64];        /*!< Station name for forming earthworm requests */
    char chan[64];        /*!< Channel name for forming earthworm requests */
    char loc[64];         /*!< Location code for forming earthworm requests */
    char *groupName;      /*!< Full path to HDF5 data group (null terminated) */
    char *metaGroupName;  /*!< Full path to HDF5 metadata group name (null
                               terminated) */    
    //double *buffer1;      /*!< Dataset 1 [maxpts] - TODO - delete its in h5 */
    //double *buffer2;      /*!< Dataset 2 [maxpts] - TODO - delete its in h5 */
    double *data;         /*!< Data to be copied onto other structure [ncopy] */
    double t1;            /*!< Start time of data (UTC-seconds) */
    //double t1beg;         /*!< Epochal start time of buffer1 (UTC-seconds) - TODO - delete */
    //double t2beg;         /*!< Epochal start time of buffer2 (UTC-seconds) - TODO - delete */
    double slat;          /*!< Station latitude (degrees) */
    double slon;          /*!< Station longitude (degrees) */
    double selev;         /*!< Station elevation above sea level (m) */
    double dt;            /*!< Sampling period (seconds) */
    int idest;            /*!< Maps this trace back to the appropriate
                               three-component data stream */
    int maxpts;           /*!< Max number of points in data buffers */
    int npts1;            /*!< Number of points in buffer 1 - TODO - delete */
    int npts2;            /*!< Number of points in buffer 2 - TODO - delete */
    int ncopy;            /*!< Number of points to copy from Earthworm traceBuffer
                               to HDF5 or number of points to copy from HDF5 to
                               GFAST data buffer */
};

struct h5traceBuffer_struct
{
    struct h5trace_struct *traces; /*!< HDF5 trace data structure */
    hid_t fileID;                  /*!< HDF5 file handle */
    int ntraces;                   /*!< Number of traces to collect */
    bool linit;                    /*!< True if the structure has been
                                        initialized */
};

#ifdef __cplusplus
extern "C"
{
#endif


/* Classify the result of earthworm tport_copyfrom */
int traceBuffer_ewrr_classifyGetRetval(const int retval);
/* Finalize earthworm ring reader */
int traceBuffer_ewrr_finalize(struct ewRing_struct *ringInfo);
/* Flush an earthworm ring */
int traceBuffer_ewrr_flushRing(struct ewRing_struct *ringInfo);
/* Read tracebuf2 messages from the ring */
int traceBuffer_ewrr_getTraceBuf2Messages(const int maxMessages,
                                          const bool showWarnings,
                                          struct ewRing_struct *ringInfo,
                                          int *nRead,
                                          char *msgs);
/* Initialize the earthworm ring reader connection */
int traceBuffer_ewrr_initialize(const char *configFile,
                                const char *ewRing,
                                const int msWait,
                                struct ewRing_struct *ringInfo);

/* Classify return value from Earthworm get transport call */
int traceBuffer_ewrr_classifyGetRetval(const int retval);
/* Flush an earthworm ring */
int traceBuffer_ewrr_flushRing(struct ewRing_struct *ringInfo);
/* Finalize earthworm ring reader */
int traceBuffer_ewrr_finalize(struct ewRing_struct *ringInfo);
/* Unpack messages */
int traceBuffer_ewrr_unpackTraceBuf2Messages(
    const int nRead,
    const char *msgs,
    struct h5traceBuffer_struct *h5traces);

/* Sets data in h5 file */
int traceBuffer_h5_setData(struct h5traceBuffer_struct *h5traceBuffer);
/* Copies the trace buffer to the GFAST structure */
int traceBuffer_h5_copyTraceBufferToGFAST(
    struct h5traceBuffer_struct *traceBuffer,
    struct GFAST_data_struct *gps_data);
/* Initialize tracebuffer structure from GFAST */
int traceBuffer_h5_setTraceBufferFromGFAST(
    const double bufflen,
    struct GFAST_data_struct gps_data,
    struct h5traceBuffer_struct *traceBuffer);
/* Close the HDF5 file with the traces */
int traceBuffer_h5_finalize(struct h5traceBuffer_struct *h5trace);
/* Get data from the HDF5 file */
int traceBuffer_h5_getData(const double t1, const double t2,
                           struct h5traceBuffer_struct *h5traceBuffer);
/* Get a double array */
int traceBuffer_h5_getDoubleArray(const hid_t groupID,
                                  const int i1, const int i2, 
                                  const char *citem,
                                  const double traceNaN, 
                                  const int nwork,
                                  double *__restrict__ work);
/* Get scalar data from the HDF5 file */
int traceBuffer_h5_getScalars(const hid_t groupID,
                              const int intNaN,
                              const double doubleNaN,
                              int *maxpts,
                              double *dt, double *ts1, double *ts2);
/* Initialize the HDF5 tracebuffer structure */
int traceBuffer_h5_initialize(const int job,
                              const bool linMemory,
                              const char *h5dir,
                              const char *h5file,
                              struct h5traceBuffer_struct *h5traceBuffer);
/* Sets the HDF5 file name */
int traceBuffer_h5_setFileName(const char *h5dir,
                               const char *h5file,
                               char h5name[PATH_MAX]);
/* Set scalar data */
int traceBuffer_h5_setDoubleScalar(const hid_t groupID,
                                   const char *citem,
                                   const double scalar);
int traceBuffer_h5_setIntegerScalar(const hid_t groupID,
                                    const char *citem,
                                    const int scalar);

#define GFAST_traceBuffer_h5_copyTraceBufferToGFAST(...)       \
              traceBuffer_h5_copyTraceBufferToGFAST(__VA_ARGS__)
#define GFAST_traceBuffer_h5_finalize(...)       \
              traceBuffer_h5_finalize(__VA_ARGS__)
#define GFAST_traceBuffer_h5_getData(...)       \
              traceBuffer_h5_getData(__VA_ARGS__)
#define GFAST_traceBuffer_h5_getDoubleArray(...)       \
              traceBuffer_h5_getDoubleArray(__VA_ARGS__)
#define GFAST_traceBuffer_h5_getScalars(...)       \
              traceBuffer_h5_getScalars(__VA_ARGS__)
#define GFAST_traceBuffer_h5_initialize(...)       \
              traceBuffer_h5_initialize(__VA_ARGS__)
#define GFAST_traceBuffer_h5_setFileName(...)       \
              traceBuffer_h5_setFileName(__VA_ARGS__)
#define GFAST_traceBuffer_h5_setDoubleScalar(...)       \
              traceBuffer_h5_setDoubleScalar(__VA_ARGS__)
#define GFAST_traceBuffer_h5_setIntegerScalar(...)       \
              traceBuffer_h5_setIntegerScalar(__VA_ARGS__)
#define GFAST_traceBuffer_h5_setTraceBufferFromGFAST(...)       \
              traceBuffer_h5_setTraceBufferFromGFAST(__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* _gfast_tracebuffer_h__ */
