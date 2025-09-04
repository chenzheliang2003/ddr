#include <iostream>
#include <vector>
#include <random>
#include <limits>
#include <cmath>
#include <memory>

#include "ddr.hpp"

int main() {
    // 设置数据维度和观察数量
    int nobs = 100;   // 数据点数
    int ndim = 5;     // 每个数据点的维度
    int k = 10;       // 最近邻的数量

    // 随机生成数据
    std::vector<double> data(nobs * ndim);
    std::mt19937_64 rng(nobs * k);  // 随机数生成器
    std::uniform_real_distribution<> dist(0, 1);

    for (int o = 0; o < nobs; ++o) {
        double offset = (o % 2 == 1 ? 10 : 0);  // 创建两个人群，一群在 x 轴偏移量为 0，另一群为 10
        for (int d = 0; d < ndim; ++d) {
            data[d + o * ndim] = dist(rng) + (d == 0 ? offset : 0); // 数据分布在二维空间
        }
    }

    // 构建 k-近邻索引
    std::shared_ptr<knncolle::Builder<int, double, double>> builder;
    builder.reset(new knncolle::VptreeBuilder<int, double, double>(std::make_shared<knncolle::EuclideanDistance<double, double>>()));
    auto index = builder->build_unique(knncolle::SimpleMatrix<int, double>(ndim, nobs, data.data()));
    auto searcher = index->initialize();

    // 查找最近邻
    umappp::NeighborList<int, double> neighbors(nobs);
    for (int i = 0; i < nobs; ++i) {
        std::vector<int> indices;
        std::vector<double> distances;
        searcher->search(i, k, &indices, &distances);
        size_t actual_k = indices.size();
        for (size_t x = 0; x < actual_k; ++x) {
            neighbors[i].emplace_back(indices[x], distances[x]);
        }
    }

    // 设置 UMAP 配置
    umappp::Options options;
    options.num_neighbors = k;  // 设置邻居数
    options.num_threads = 2;    // 使用 2 个线程
    options.initialize = umappp::InitializeMethod::SPECTRAL;  // 初始化方法：谱初始化

    // 初始化嵌入空间
    int outdim = 2;  // 降维目标为 2 维
    std::vector<double> output(nobs * outdim);
    
    // 执行初始化
    auto status = umappp::initialize(neighbors, outdim, output.data(), options);

    // 输出一些基本信息
    std::cout << "Initial Epoch: " << status.epoch() << std::endl;
    std::cout << "Total Epochs: " << status.num_epochs() << std::endl;
    std::cout << "Number of Observations: " << status.num_observations() << std::endl;

    // 运行 UMAP 算法，执行 500 次迭代
    status.run();
    std::cout << "Final Epoch: " << status.epoch() << std::endl;

    // 打印降维后的结果，确保没有 NaN 值
    for (const auto& val : output) {
        if (std::isnan(val)) {
            std::cout << "NaN detected in output!" << std::endl;
            return -1;
        }
    }

    // 简单的检查，查看两个群体是否有分离
    std::vector<double> min_odd(outdim, std::numeric_limits<double>::infinity());
    std::vector<double> max_odd(outdim, -std::numeric_limits<double>::infinity());
    auto min_even = min_odd;
    auto max_even = max_odd;
    for (int i = 0; i < nobs; ++i) {
        auto& min = (i % 2 == 0 ? min_even : min_odd);
        auto& max = (i % 2 == 0 ? max_even : max_odd);
        for (int d = 0; d < outdim; ++d) {
            auto val = output[outdim * i + d];
            min[d] = std::min(min[d], val);
            max[d] = std::max(max[d], val);
        }
    }

    if (max_even[0] < min_odd[0] || max_odd[0] < min_even[0] || max_even[1] < min_odd[1] || max_odd[1] < min_even[1]) {
        std::cout << "Groups seem to be separated in the embedding space!" << std::endl;
    } else {
        std::cout << "Groups are not well separated!" << std::endl;
    }

    return 0;
}
