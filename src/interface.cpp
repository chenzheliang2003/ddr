#include "interface.hpp"

static bexfun_info_t function_list[]=
{
    {"ddr::pca",pca,nullptr},
    {"ddr::tsne",tsne,nullptr},
    {"ddr::umap",umap,nullptr},
    {"",nullptr,nullptr}
};

bexfun_info_t *bxPluginFunctions()
{
    return function_list;
}

int bxPluginInit(int nrhs,const bxArray *prhs[])
{
    baltam::bxAddCXXClass<bxPCA>("DDR");
    return 0;
}

int bxPluginFini()
{
    return 0;
}

bxPCA::bxPCA(const PCA &object):
PCA(object){}

baltam::extern_obj_base *bxPCA::dup() const
{
    return new bxPCA(*this);
}

std::string bxPCA::classname() const
{
    return "PCA";
}

std::string bxPCA::to_string() const
{
    std::stringstream ss;
    ss<<"PCA 对象, 包含如下性质:"<<std::endl;
    ss<<"均值:"<<std::endl;
    ss<<"[";
    int n=mean.cols();
    for(int i=0;i<n;++i)
    {
        ss<<mean(i);
        if(i<n-1)
            ss<<", ";
    }
    ss<<"],"<<std::endl;
    ss<<"成分:"<<std::endl;
    ss<<"[";
    n=components.rows();
    int m=components.cols();
    for(int i=0;i<n;++i)
    {
        for(int j=0;j<m;++j)
        {
            ss<<components(i,j);
            if(j<m-1)
                ss<<", ";
        }
        if(i<n-1)
            ss<<"; "<<std::endl;
    }
    ss<<"]."<<std::endl;
    return ss.str();
}

int bxPCA::ID=0;

void pca(int nlhs,bxArray *plhs[],int nrhs,const bxArray *prhs[])
{
    /*
        Y = pca(X, tol, svdOption="thin")
        Y = pca(X, nd, svdOption="thin")
        [Y, model] = pca(__)
        [Y, model, explaination] = pca(__)
    */
    if(nrhs<2)
        throw std::invalid_argument("输入参数过少");
    if(nrhs>3)
        throw std::invalid_argument("输入参数过多");
    if(nlhs>3)
        throw std::invalid_argument("输出参数过多");
    PCA func;
    if(!bxIsDouble(prhs[0])||bxGetNumberOfDimensions(prhs[0])!=2)
        throw std::invalid_argument("仅支持 double 类型的矩阵");
    baSize n=bxGetN(prhs[0]),m=bxGetM(prhs[0]);
    const double *data=bxGetDoublesRO(prhs[0]);
    Eigen::MatrixXd matrix(n,m);
    for(baSize i=0;i<n;++i)
        for(baSize j=0;j<m;++j)
            matrix(i,j)=data[i*m+j];
    if(!bxIsScalar(prhs[1]))
        throw std::invalid_argument("容差或维度应为数值标量");
    double tol_or_nd=bxGetScalar(prhs[1]);
    if(nrhs==3)
    {
        if(!bxIsString(prhs[2])||bxGetNumberOfDimensions(prhs[2])!=2||bxGetN(prhs[2])!=2||bxGetM(prhs[2])!=2)
            throw std::invalid_argument("SVD 分解选项应为字符串标量");
        const std::string &option=bxGetString(prhs[2],0);
        if(option=="thin")
            func.set_svd_option(PCA::THIN);
        else if(option=="full")
            func.set_svd_option(PCA::FULL);
        else
            throw std::invalid_argument("SVD 分解选项应为 \"thin\", \"full\" 之一");
    }
    if(tol_or_nd>0&&tol_or_nd<1)
        func.fit(matrix,tol_or_nd);
    else if(tol_or_nd>=1&&tol_or_nd==std::floor(tol_or_nd))
        func.fit(matrix,static_cast<int>(tol_or_nd));
    Eigen::MatrixXd reduced=func.transform(matrix);
    n=reduced.rows();
    m=reduced.cols();
    plhs[0]=bxCreateDoubleMatrix(n,m,bxREAL);
    double *ret_data=bxGetDoublesRW(plhs[0]);
    for(int i=0;i<n;++i)
        for(int j=0;j<m;++j)
            ret_data[i*m+j]=reduced(i,j);
    if(nlhs>=2)
        plhs[1]=baltam::bxCreateExtObj<bxPCA>(func);
    if(nlhs==3)
        plhs[2]=bxCreateDoubleScalar(func.get_explained());
}

void pcarec(int nlhs,bxArray *plhs[],int nrhs,const bxArray *prhs[])
{

}

void tsne(int nlhs,bxArray *plhs[],int nrhs,const bxArray *prhs[])
{

}

void umap(int nlhs,bxArray *plhs[],int nrhs,const bxArray *prhs[])
{

}