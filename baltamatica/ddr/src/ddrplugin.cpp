#include "ddrplugin.hpp"
#include <mlpack.hpp>

static bexfun_info_t function_list[]=
{
    {"ddr::pca",pca,nullptr},
    {"ddr::kpca",kpca,nullptr},
    {"ddr::tsne",tsne,nullptr},
    {"ddr::umap",umap,nullptr},
    {"ddr::mnf",mnf,nullptr},
    {"",nullptr,nullptr}
};

bexfun_info_t *bxPluginFunctions()
{
    return function_list;
}

int bxPluginInit(int nrhs,const bxArray *prhs[])
{
    return 0;
}

int bxPluginFini()
{
    return 0;
}

void pca(int nlhs, bxArray *plhs[], int nrhs, const bxArray *prhs[])
{
    if (nrhs < 2) throw std::invalid_argument("输入参数过少");
    if (nrhs > 2) throw std::invalid_argument("输入参数过多");
    if (nlhs != 2) throw std::invalid_argument("输出参数个数错误");

    if (!bxIsDouble(prhs[0]) || bxGetNumberOfDimensions(prhs[0]) != 2)
        throw std::invalid_argument("仅支持 double 类型的矩阵");
    if (!bxIsDouble(prhs[1]) || bxGetNumberOfElements(prhs[1]) != 1)
        throw std::invalid_argument("目标维度应为数值标量");

    size_t n = bxGetM(prhs[0]);  // 样本数
    size_t d = bxGetN(prhs[0]);  // 特征维度
    const double* Xraw = bxGetDoublesRO(prhs[0]);
    size_t r = static_cast<size_t>(bxGetScalar(prhs[1]));

    // 正确转换数据布局: 从行主序转换为列主序，并转置为 d x n 格式
    arma::mat X(d, n);  // MLpack期望d×n矩阵(每列是一个样本)
    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j < d; ++j)
            X(j, i) = Xraw[i * d + j];

    // 输出矩阵
    plhs[0] = bxCreateDoubleMatrix(n, r, bxREAL);  // Y: n x r
    plhs[1] = bxCreateDoubleMatrix(r, 1, bxREAL);  // eigVals: r x 1
    double* Yptr = bxGetDoublesRW(plhs[0]);
    double* eigptr = bxGetDoublesRW(plhs[1]);
    
    // 使用中间变量存储结果
    arma::mat Y_mlpack;
    arma::vec eigVals_mlpack;
    
    mlpack::pca::PCA pca;
    pca.Apply(X, Y_mlpack, eigVals_mlpack);
    
    // 将结果转换回行主序 (n x r)
    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j < r; ++j)
            Yptr[i * r + j] = Y_mlpack(j, i);  // 转置并复制
    
    // 复制特征值
    for (size_t i = 0; i < r; ++i)
        eigptr[i] = eigVals_mlpack(i);
}

void kpca(int nlhs, bxArray *plhs[], int nrhs, const bxArray *prhs[])
{
    if (nrhs < 2) throw std::invalid_argument("输入参数过少");
    if (nrhs > 4) throw std::invalid_argument("输入参数过多");
    if (nlhs != 2) throw std::invalid_argument("输出参数个数错误");

    if (!bxIsDouble(prhs[0]) || bxGetNumberOfDimensions(prhs[0]) != 2)
        throw std::invalid_argument("仅支持 double 类型的矩阵");
    if (!bxIsDouble(prhs[1]) || bxGetNumberOfElements(prhs[1]) != 1)
        throw std::invalid_argument("目标维度应为数值标量");

    size_t n = bxGetM(prhs[0]);  // 样本数
    size_t d = bxGetN(prhs[0]);  // 特征维度
    const double* Xraw = bxGetDoublesRO(prhs[0]);
    size_t r = static_cast<size_t>(bxGetScalar(prhs[1]));

    std::string kernelType = "rbf";
    
    if (nrhs >= 3) kernelType = std::string(baltam::bxAsString(prhs[2]));

    // 正确转换数据布局
    arma::mat X(d, n);
    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j < d; ++j)
            X(j, i) = Xraw[i * d + j];

    // 输出矩阵
    plhs[0] = bxCreateDoubleMatrix(n, r, bxREAL);  // Y: n x r
    plhs[1] = bxCreateDoubleMatrix(r, 1, bxREAL);  // eigVals: r x 1
    double* Yptr = bxGetDoublesRW(plhs[0]);
    double* eigptr = bxGetDoublesRW(plhs[1]);
    
    // 使用中间变量存储结果
    arma::mat Y_mlpack;
    arma::vec eigVals_mlpack;
    
    try {
        if (kernelType == "rbf") {
            mlpack::kernel::GaussianKernel kernel;
            mlpack::kpca::KernelPCA<mlpack::kernel::GaussianKernel> kpca(kernel);
            kpca.Apply(X, Y_mlpack, eigVals_mlpack);
        } else if (kernelType == "poly") {
            mlpack::kernel::PolynomialKernel kernel;
            mlpack::kpca::KernelPCA<mlpack::kernel::PolynomialKernel> kpca(kernel);
            kpca.Apply(X, Y_mlpack, eigVals_mlpack);
        } else {
            throw std::invalid_argument("不支持的核类型");
        }
        
        // 将结果转换回行主序 (n x r)
        for (size_t i = 0; i < n; ++i)
            for (size_t j = 0; j < r; ++j)
                Yptr[i * r + j] = Y_mlpack(j, i);
        
        // 复制特征值
        for (size_t i = 0; i < r; ++i)
            eigptr[i] = eigVals_mlpack(i);
            
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("KPCA计算失败: ") + e.what());
    }
}

void tsne(int nlhs,bxArray *plhs[],int nrhs,const bxArray *prhs[])
{
    /*
        Y = tsne(X,nd,perplexity=30,theta=0.5)
    */
    if(nrhs<2)
        throw std::invalid_argument("输入参数过少");
    if(nrhs>4)
        throw std::invalid_argument("输入参数过多");
    if(nlhs>1)
        throw std::invalid_argument("输出参数过多");
    TSNE func;
    if(!bxIsDouble(prhs[0])||bxGetNumberOfDimensions(prhs[0])!=2)
        throw std::invalid_argument("仅支持 double 类型的矩阵");
    if(!bxIsScalar(prhs[1]))
        throw std::invalid_argument("目标维度应为数值标量");
    baSize n=bxGetM(prhs[0]);
    baSize ndin=bxGetN(prhs[0]);
    baSize ndout=bxGetScalar(prhs[1]);
    std::vector<double> dataT(n*ndin);
    {
        const double *X=bxGetDoublesRO(prhs[0]);
        for(baSize i=0;i<n;++i)
            for(baSize j=0;j<ndin;++j)
                dataT[i*ndin+j]=X[j*n+i];
    }
    std::vector<double> Yrow(n*ndout);
    double perplexity=30;
    double theta=0.5;
    if(nrhs>=3)
    {
        if(!bxIsScalar(prhs[2]))
            throw std::invalid_argument("困惑度应为数值标量");
        perplexity=bxGetScalar(prhs[2]);
    }
    if(nrhs==4)
    {
        if(!bxIsScalar(prhs[3]))
            throw std::invalid_argument("近似度应为数值标量");
        theta=bxGetScalar(prhs[3]);
    }
    func.run(dataT.data(),n,ndin,Yrow.data(),ndout,perplexity,theta,42,true);
    plhs[0] = bxCreateDoubleMatrix(n,ndout,bxREAL);
    double* Ycol = bxGetDoublesRW(plhs[0]);
    for(baSize i=0;i<n;++i)
        for(baSize j=0;j<ndout;++j)
            Ycol[j*n+i]=Yrow[i*ndout+j];
}

void umap(int nlhs, bxArray *plhs[], int nrhs, const bxArray *prhs[])
{
    /*
        Y = umap(X, nd, n_neighbors=15, min_dist=0.1, n_epochs=500)
    */
    if (nrhs < 2)
        throw std::invalid_argument("输入参数过少");
    if (nrhs > 5)
        throw std::invalid_argument("输入参数过多");
    if (nlhs > 1)
        throw std::invalid_argument("输出参数过多");

    if (!bxIsDouble(prhs[0]) || bxGetNumberOfDimensions(prhs[0]) != 2)
        throw std::invalid_argument("仅支持 double 类型的矩阵");
    if (!bxIsScalar(prhs[1]))
        throw std::invalid_argument("目标维度应为数值标量");

    baSize n = bxGetM(prhs[0]);        // 样本数
    baSize ndin = bxGetN(prhs[0]);     // 输入维度
    baSize ndout = bxGetScalar(prhs[1]); // 输出维度

    // 转置数据到 column-major（features 在前，样本在后）
    std::vector<double> dataT(n * ndin);
    {
        const double *X = bxGetDoublesRO(prhs[0]);
        for (baSize i = 0; i < n; ++i)
            for (baSize j = 0; j < ndin; ++j)
                dataT[i*ndin+j]=X[j*n+i];
    }

    // 默认参数
    int n_neighbors = 15;
    double min_dist = 0.1;
    int n_epochs = -1;

    if (nrhs >= 3) {
        if (!bxIsScalar(prhs[2]))
            throw std::invalid_argument("邻居数应为数值标量");
        n_neighbors = static_cast<int>(bxGetScalar(prhs[2]));
    }
    if (nrhs >= 4) {
        if (!bxIsScalar(prhs[3]))
            throw std::invalid_argument("最小距离应为数值标量");
        min_dist = bxGetScalar(prhs[3]);
    }
    if (nrhs == 5) {
        if (!bxIsScalar(prhs[4]))
            throw std::invalid_argument("迭代次数应为数值标量");
        n_epochs = static_cast<int>(bxGetScalar(prhs[4]));
    }

    // 输出 embedding (row-major)
    plhs[0] = bxCreateDoubleMatrix(n, ndout, bxREAL);
    double *Ycol = bxGetDoublesRW(plhs[0]);

    // 内部 embedding (column-major, dim x n)
    std::vector<double> Yembed(n * ndout);

    // UMAP 参数
    umappp::Options opt;
    opt.num_neighbors = n_neighbors;
    opt.min_dist = min_dist;
    opt.num_epochs = n_epochs;

    // 构建 knn builder
    knncolle::VptreeBuilder<int, double, double> builder(std::make_shared<knncolle::EuclideanDistance<double, double> >());

    // 初始化并运行
    auto status = umappp::initialize(
        ndin,
        n,
        dataT.data(),
        builder,
        ndout,
        Yembed.data(),
        opt
    );
    status.run();

    // 转换为 (n x ndout) row-major 输出
    for (baSize i = 0; i < n; ++i)
        for (baSize j = 0; j < ndout; ++j)
            Ycol[j*n+i]=Yembed[i*ndout+j];
}

void mnf(int nlhs, bxArray *plhs[], int nrhs, const bxArray *prhs[])
{
    /*
        [W, H] = mnf(X, nd, max_iter=100)
    */
    if (nrhs < 2)
        throw std::invalid_argument("输入参数过少");
    if (nrhs > 3)
        throw std::invalid_argument("输入参数过多");
    if (nlhs != 2)
        throw std::invalid_argument("输出参数个数错误");

    if (!bxIsDouble(prhs[0]) || bxGetNumberOfDimensions(prhs[0]) != 2)
        throw std::invalid_argument("仅支持 double 类型的矩阵");
    if (!bxIsDouble(prhs[1]) || bxGetNumberOfElements(prhs[1]) != 1)
        throw std::invalid_argument("目标维度应为数值标量");

    // 输入矩阵
    size_t n = bxGetM(prhs[0]);
    size_t m = bxGetN(prhs[0]);
    double *X = bxGetDoubles(prhs[0]);
    size_t r = static_cast<size_t>(bxGetScalar(prhs[1]));

    // 可选迭代次数
    size_t max_iter = 100;
    if (nrhs == 3)
        max_iter = static_cast<size_t>(bxGetScalar(prhs[2]));

    // 构建输出矩阵 W (n x r) 和 H (r x m)
    plhs[0] = bxCreateDoubleMatrix(n, r, bxREAL);
    plhs[1] = bxCreateDoubleMatrix(r, m, bxREAL);
    double *Wptr = bxGetDoubles(plhs[0]);
    double *Hptr = bxGetDoubles(plhs[1]);

    arma::mat W(Wptr, n, r, false, true); // false = 不copy, true = 允许析构时释放内存
    arma::mat H(Hptr, r, m, false, true);

    // 使用 mlpack NMF ALS Factorizer
    mlpack::NMFALSFactorizer nmf;
    nmf.Apply(arma::mat(X, n, m, false, true), r, W, H);
}