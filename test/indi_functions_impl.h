#ifndef INDI_FUNCTIONS_IMPL_TESTING
#define INDI_FUNCTIONS_IMPL_TESTING

#include <indiapi.h>
#include <indidevapi.h>
void 	ISGetProperties (const char *dev) {}
void 	ISNewText (const char *dev, const char *name, char *texts[], char *names[], int n) {}
void 	ISNewNumber (const char *dev, const char *name, double *doubles, char *names[], int n) {}
void 	ISNewSwitch (const char *dev, const char *name, ISState *states, char *names[], int n) {}
void 	ISNewBLOB (const char *dev, const char *name, int sizes[], int blobsizes[], char *blobs[], char *formats[], char *names[], int n) {}
void 	ISSnoopDevice (XMLEle *root) {}


#endif
