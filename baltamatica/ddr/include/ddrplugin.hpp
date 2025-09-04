#include "bex/bex.hpp"
#include "ddr.hpp"

#ifdef __cplusplus
extern "C"
{
#endif

BEX_EXPORT bexfun_info_t *bxPluginFunctions();

BEX_EXPORT int bxPluginInit(int nrhs,const bxArray *prhs[]);

BEX_EXPORT int bxPluginFini();

#ifdef __cplusplus
}
#endif

void pca(int nlhs,bxArray *plhs[],int nrhs,const bxArray *prhs[]);

void kpca(int nlhs,bxArray *plhs[],int nrhs,const bxArray *prhs[]);

void tsne(int nlhs,bxArray *plhs[],int nrhs,const bxArray *prhs[]);

void umap(int nlhs,bxArray *plhs[],int nrhs,const bxArray *prhs[]);

void mnf(int nlhs,bxArray *plhs[],int nrhs,const bxArray *prhs[]);