#include "ddrplugin.hpp"
#include <mlpack.hpp>

static bexfun_info_t function_list[] =
    {
        {"pca_plugin", pca, nullptr},
        {"kpca_plugin", kpca, nullptr},
        {"tsne_plugin", tsne, nullptr},
        {"umap_plugin", umap, nullptr},
        {"mnf_plugin", mnf, nullptr},
        {"", nullptr, nullptr}};

bexfun_info_t *bxPluginFunctions()
{
    return function_list;
}

int bxPluginInit(int nrhs, const bxArray *prhs[])
{
    return 0;
}

int bxPluginFini()
{
    return 0;
}

typedef std::unordered_map<std::string, const bxArray *> Parameters;

Parameters getParameters(int nrhs, const bxArray *prhs[], int current)
{
    if (nrhs - current & 1)
        throw std::invalid_argument("参数个数错误.");
    Parameters ret;
    while (current < nrhs)
    {
        ret[baltam::bxAsString(prhs[current])] = prhs[current + 1];
        current += 2;
    }
    return ret;
}

bool isScalar(const bxArray *array)
{
    return bxGetNumberOfDimensions(array) == 2 && bxGetN(array) == 1 && bxGetM(array) == 1 && bxIsScalar(array);
}

double getScalar(const bxArray *array)
{
    if (!isScalar(array))
        throw std::invalid_argument("此处应为数值标量.");
    return bxGetScalar(array);
}

void pca(int nlhs, bxArray *plhs[], int nrhs, const bxArray *prhs[])
{
    /*
        [Y, EigVals] = pca(X, NumDimensions)
        
        输入:
            X               | double, n×d 矩阵, 每行是一个样本
            NumDimensions   | scalar, 输出的降维维度 r
        
        输出:
            Y               | double, n×r 矩阵, 降维后的结果
            EigVals         | double, r×1 向量, 对应的特征值
    */
    if (nrhs < 2)
        throw std::invalid_argument("输入参数过少.");
    if (nrhs > 2)
        throw std::invalid_argument("输入参数过多.");
    if (nlhs != 2)
        throw std::invalid_argument("输出参数个数错误.");

    if (!bxIsDouble(prhs[0]) || bxGetNumberOfDimensions(prhs[0]) != 2)
        throw std::invalid_argument("仅支持 double 类型的矩阵.");
    if (!bxIsDouble(prhs[1]) || bxGetNumberOfElements(prhs[1]) != 1)
        throw std::invalid_argument("目标维度应为数值标量.");

    size_t n = bxGetM(prhs[0]);
    size_t d = bxGetN(prhs[0]);
    const double *Xraw = bxGetDoublesRO(prhs[0]);
    size_t r = static_cast<size_t>(bxGetScalar(prhs[1]));

    arma::mat X(d, n);
    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j < d; ++j)
            X(j, i) = Xraw[i + n * j];

    plhs[0] = bxCreateDoubleMatrix(n, r, bxREAL);
    plhs[1] = bxCreateDoubleMatrix(r, 1, bxREAL);
    double *Yptr = bxGetDoublesRW(plhs[0]);
    double *eigptr = bxGetDoublesRW(plhs[1]);

    arma::mat Y_mlpack;
    arma::vec eigVals_mlpack;

    mlpack::pca::PCA pca;
    pca.Apply(X, Y_mlpack, eigVals_mlpack);

    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j < r; ++j)
            Yptr[i + n * j] = Y_mlpack(j, i);

    for (size_t i = 0; i < r; ++i)
        eigptr[i] = eigVals_mlpack(i);
}

void kpca(int nlhs, bxArray *plhs[], int nrhs, const bxArray *prhs[])
{
    /*
    [Y, EigVals] = kpca(X, NumDimensions, KernelType)
    
    输入:
        X               | double, n×d 矩阵
        NumDimensions   | scalar, 输出的降维维度 r
        KernelType      | string, 核函数类型, 可选: "rbf" (默认), "poly"
    
    输出:
        Y               | double, n×r 矩阵, 降维后的结果
        EigVals         | double, r×1 向量, 对应的特征值
    */
    if (nrhs < 2)
        throw std::invalid_argument("输入参数过少");
    if (nrhs > 4)
        throw std::invalid_argument("输入参数过多");
    if (nlhs != 2)
        throw std::invalid_argument("输出参数个数错误");

    if (!bxIsDouble(prhs[0]) || bxGetNumberOfDimensions(prhs[0]) != 2)
        throw std::invalid_argument("仅支持 double 类型的矩阵");
    if (!bxIsDouble(prhs[1]) || bxGetNumberOfElements(prhs[1]) != 1)
        throw std::invalid_argument("目标维度应为数值标量");

    size_t n = bxGetM(prhs[0]);
    size_t d = bxGetN(prhs[0]);
    const double *Xraw = bxGetDoublesRO(prhs[0]);
    size_t r = static_cast<size_t>(bxGetScalar(prhs[1]));

    std::string kernelType = "rbf";

    if (nrhs >= 3)
        kernelType = std::string(baltam::bxAsString(prhs[2]));

    arma::mat X(d, n);
    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j < d; ++j)
            X(j, i) = Xraw[i + n * j];

    plhs[0] = bxCreateDoubleMatrix(n, r, bxREAL);
    plhs[1] = bxCreateDoubleMatrix(r, 1, bxREAL);
    double *Yptr = bxGetDoublesRW(plhs[0]);
    double *eigptr = bxGetDoublesRW(plhs[1]);

    arma::mat Y_mlpack;
    arma::vec eigVals_mlpack;

    try
    {
        if (kernelType == "rbf")
        {
            mlpack::kernel::GaussianKernel kernel;
            mlpack::kpca::KernelPCA<mlpack::kernel::GaussianKernel> kpca(kernel);
            kpca.Apply(X, Y_mlpack, eigVals_mlpack);
        }
        else if (kernelType == "poly")
        {
            mlpack::kernel::PolynomialKernel kernel;
            mlpack::kpca::KernelPCA<mlpack::kernel::PolynomialKernel> kpca(kernel);
            kpca.Apply(X, Y_mlpack, eigVals_mlpack);
        }
        else
        {
            throw std::invalid_argument("不支持的核类型");
        }

        for (size_t i = 0; i < n; ++i)
            for (size_t j = 0; j < r; ++j)
                Yptr[i + n * j] = Y_mlpack(j, i);

        for (size_t i = 0; i < r; ++i)
            eigptr[i] = eigVals_mlpack(i);
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error(std::string("KPCA计算失败: ") + e.what());
    }
}

void tsne(int nlhs, bxArray *plhs[], int nrhs, const bxArray *prhs[])
{
    /*
        [Y, KL, Converged] = tsne(X, Name, Value, ...)
        
        输入:
            X               | double, n×d 矩阵
            Name/Value 可选参数:
                "Perplexity"       | scalar, 默认 30
                "NumDimensions"    | scalar, 输出维度, 2 或 3
                "Algorithm"        | string, "barneshut"(默认) 或 "exact"
                "MaxIter"          | scalar, 最大迭代次数, 默认 1000
                "Exaggeration"     | scalar, early exaggeration 因子, 默认 4
                "Theta"            | scalar, barnes-hut 近似参数, 默认 0.5
                "Distance"         | string, "Euclidean"(默认) 或 "Manhattan"
        
        输出:
            Y               | double, n×ndout 矩阵, 降维结果
            KL              | scalar, 最终 KL 散度值
            Converged       | scalar, 是否已达到最大迭代次数 (0/1)
    */
    if (nrhs < 1)
        throw std::invalid_argument("输入参数过少");
    if (nlhs > 3)
        throw std::invalid_argument("输出参数过多");

    if (!bxIsDouble(prhs[0]) || bxGetNumberOfDimensions(prhs[0]) != 2)
        throw std::invalid_argument("仅支持 double 类型的矩阵");

    qdtsne::Options opt;

    Parameters params = getParameters(nrhs, prhs, 1);
    baSize n = bxGetM(prhs[0]);
    baSize ndin = bxGetN(prhs[0]);
    int ndout = 2;
    if (params["NumDimensions"])
    {
        ndout = getScalar(params["NumDimensions"]);
        if (ndout != 2 && ndout != 3)
            throw std::runtime_error("输出维度应为 2 或 3");
    }
    if (params["Perplexity"])
        opt.perplexity = getScalar(params["Perplexity"]);
    if (params["MaxIter"])
        opt.max_iterations = getScalar(params["MaxIter"]);
    if (params["Exaggeration"])
        opt.exaggeration_factor = getScalar(params["Exaggeration"]);
    if (params["Algorithm"])
    {
        const std::string &algorithm = baltam::bxAsString(params["Algorithm"]);
        if (algorithm == "exact")
            opt.theta = 0;
        else if (algorithm != "barneshut")
            throw std::runtime_error("算法参数应为 \"exact\" 或 \"barneshut\"");
    }
    if (params["Theta"])
        opt.theta = getScalar(params["Theta"]);

    std::vector<double> dataT(n * ndin);
    {
        const double *X = bxGetDoublesRO(prhs[0]);
        for (baSize i = 0; i < n; ++i)
            for (baSize j = 0; j < ndin; ++j)
                dataT[i * ndin + j] = X[j * n + i];
    }
    plhs[0] = bxCreateDoubleMatrix(n, ndout, bxREAL);
    double *Ycol = bxGetDoublesRW(plhs[0]);
    std::vector<double> Yembed;
    knncolle::VptreeBuilder<int, double, double> builder(std::make_shared<knncolle::EuclideanDistance<double, double>>());
    if (params["Distance"])
    {
        const std::string &distance = baltam::bxAsString(params["Distance"]);
        if (distance == "Euclidean")
            builder = knncolle::VptreeBuilder<int, double, double>(std::make_shared<knncolle::EuclideanDistance<double, double>>());
        else if (distance == "Manhattan")
            builder = knncolle::VptreeBuilder<int, double, double>(std::make_shared<knncolle::ManhattanDistance<double, double>>());
        else
            throw std::runtime_error("距离参数应为 \"Euclidean\" 或 \"Manhattan\"");
    }
    auto status1 = qdtsne::initialize<2>(static_cast<int>(ndin), static_cast<int>(n), dataT.data(), builder, opt);
    auto status2 = qdtsne::initialize<3>(static_cast<int>(ndin), static_cast<int>(n), dataT.data(), builder, opt);
    if (ndout == 2)
    {
        Yembed = qdtsne::initialize_random<2>(n);
        status1.run(Yembed.data());
    }
    else
    {
        Yembed = qdtsne::initialize_random<3>(n);
        status2.run(Yembed.data());
    }

    for (baSize i = 0; i < n; ++i)
        for (baSize j = 0; j < ndout; ++j)
            Ycol[j * n + i] = Yembed[i * ndout + j];

    if (nlhs >= 2)
    {
        double KL = 0.0;
        const int nn = (ndout == 2 ? status1.num_observations() : status2.num_observations());
        const auto &nbrs = (ndout == 2 ? status1.get_neighbors() : status2.get_neighbors());

        std::vector<double> q(nn * nn, 0.0);
        double sum_q = 0.0;

        for (int i = 0; i < nn; ++i)
        {
            for (const auto &x : nbrs[i])
            {
                int j = x.first;
                double sqdist = 0.0;
                for (int d = 0; d < ndout; ++d)
                {
                    double delta = Yembed[d + i * ndout] - Yembed[d + j * ndout];
                    sqdist += delta * delta;
                }
                q[i * nn + j] = 1.0 / (1.0 + sqdist);
                sum_q += q[i * nn + j];
            }
        }

        for (int i = 0; i < nn; ++i)
        {
            for (const auto &x : nbrs[i])
            {
                int j = x.first;
                q[i * nn + j] /= sum_q;
            }
        }

        for (int i = 0; i < nn; ++i)
        {
            for (const auto &x : nbrs[i])
            {
                int j = x.first;
                double pij = x.second;
                double qij = q[i * nn + j];
                KL += pij * std::log(pij / qij);
            }
        }

        plhs[1] = bxCreateDoubleScalar(KL);
    }
    if (nlhs == 3)
        if (ndout == 2)
            plhs[2] = bxCreateDoubleScalar(status1.iteration() != status1.max_iterations());
        else
            plhs[2] = bxCreateDoubleScalar(status2.iteration() != status2.max_iterations());
}

void umap(int nlhs, bxArray *plhs[], int nrhs, const bxArray *prhs[])
{
    /*
        Y = umap(X, NumDimensions, NNeighbors, MinDist, NEpochs)
        
        输入:
            X               | double, n×d 矩阵
            NumDimensions   | scalar, 输出维度 r
            NNeighbors      | scalar, 邻居数, 默认 15
            MinDist         | scalar, 最小距离, 默认 0.1
            NEpochs         | scalar, 迭代次数, 默认 500
        
        输出:
            Y               | double, n×r 矩阵, 降维结果
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

    baSize n = bxGetM(prhs[0]);
    baSize ndin = bxGetN(prhs[0]);
    baSize ndout = bxGetScalar(prhs[1]);

    std::vector<double> dataT(n * ndin);
    {
        const double *X = bxGetDoublesRO(prhs[0]);
        for (baSize i = 0; i < n; ++i)
            for (baSize j = 0; j < ndin; ++j)
                dataT[i * ndin + j] = X[j * n + i];
    }

    int n_neighbors = 15;
    double min_dist = 0.1;
    int n_epochs = -1;

    if (nrhs >= 3)
    {
        if (!bxIsScalar(prhs[2]))
            throw std::invalid_argument("邻居数应为数值标量");
        n_neighbors = static_cast<int>(bxGetScalar(prhs[2]));
    }
    if (nrhs >= 4)
    {
        if (!bxIsScalar(prhs[3]))
            throw std::invalid_argument("最小距离应为数值标量");
        min_dist = bxGetScalar(prhs[3]);
    }
    if (nrhs == 5)
    {
        if (!bxIsScalar(prhs[4]))
            throw std::invalid_argument("迭代次数应为数值标量");
        n_epochs = static_cast<int>(bxGetScalar(prhs[4]));
    }

    plhs[0] = bxCreateDoubleMatrix(n, ndout, bxREAL);
    double *Ycol = bxGetDoublesRW(plhs[0]);

    std::vector<double> Yembed(n * ndout);

    umappp::Options opt;
    opt.num_neighbors = n_neighbors;
    opt.min_dist = min_dist;
    opt.num_epochs = n_epochs;

    knncolle::VptreeBuilder<int, double, double> builder(std::make_shared<knncolle::EuclideanDistance<double, double>>());

    auto status = umappp::initialize(
        ndin,
        n,
        dataT.data(),
        builder,
        ndout,
        Yembed.data(),
        opt);
    status.run();

    for (baSize i = 0; i < n; ++i)
        for (baSize j = 0; j < ndout; ++j)
            Ycol[j * n + i] = Yembed[i * ndout + j];
}

void mnf(int nlhs, bxArray *plhs[], int nrhs, const bxArray *prhs[])
{
    /*
        [W, H] = mnf(X, NumComponents, MaxIter)
        
        输入:
            X               | double, n×m 矩阵
            NumComponents   | scalar, 分解的潜在维度 r
            MaxIter         | scalar, 最大迭代次数, 默认 100
        
        输出:
            W               | double, n×r 矩阵, 分解结果
            H               | double, r×m 矩阵, 分解结果
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
    size_t n = bxGetM(prhs[0]);
    size_t m = bxGetN(prhs[0]);
    double *X = bxGetDoubles(prhs[0]);
    size_t r = static_cast<size_t>(bxGetScalar(prhs[1]));

    size_t max_iter = 100;
    if (nrhs == 3)
        max_iter = static_cast<size_t>(bxGetScalar(prhs[2]));

    plhs[0] = bxCreateDoubleMatrix(n, r, bxREAL);
    plhs[1] = bxCreateDoubleMatrix(r, m, bxREAL);
    double *Wptr = bxGetDoubles(plhs[0]);
    double *Hptr = bxGetDoubles(plhs[1]);

    arma::mat W(Wptr, n, r, false, true);
    arma::mat H(Hptr, r, m, false, true);

    mlpack::NMFALSFactorizer nmf;
    nmf.Apply(arma::mat(X, n, m, false, true), r, W, H);
}