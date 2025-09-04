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

class bxPCA:public PCA,public baltam::extern_obj_base
{
public:
    bxPCA(const PCA &object);

    baltam::extern_obj_base *dup() const override;

    std::string classname() const override;

    std::string to_string() const override;

public:
    static int ID;
};

void pca(int nlhs,bxArray *plhs[],int nrhs,const bxArray *prhs[]);

void pcarec(int nlhs,bxArray *plhs[],int nrhs,const bxArray *prhs[]);

void tsne(int nlhs,bxArray *plhs[],int nrhs,const bxArray *prhs[]);

void umap(int nlhs,bxArray *plhs[],int nrhs,const bxArray *prhs[]);