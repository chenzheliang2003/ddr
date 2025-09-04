#include <iostream>
#include <vector>
#include "ddr.hpp"

int main() {
    // 假设我们有一个简单的 6 个样本，每个样本是 3 维的输入数据（N=6, D=3）
    int N = 6;  // 样本数量
    int D = 3;  // 每个样本的维度
    double X[6 * 3] = {  // 6 个样本，每个样本 3 个特征
        1.0, 2.0, 3.0,
        1.5, 2.5, 3.5,
        3.0, 3.5, 4.0,
        5.0, 5.5, 6.0,
        7.0, 7.5, 8.0,
        9.0, 9.5, 10.0
    };

    int no_dims = 2;  // 降维目标维度
    double perplexity = 1.0;  // t-SNE 的 perplexity 参数
    double theta = 0.5;  // t-SNE 的 theta 参数（用于加速）
    unsigned int seed = 42;  // 随机种子
    bool verbose = true;  // 是否显示进度

    double Y[6 * 2];  // 存储降维后的数据（6 个样本，2 维）

    // 创建 TSNE 类的实例
    TSNE tsne;

    // 执行 t-SNE 降维
    tsne.run(X, N, D, Y, no_dims, perplexity, theta, seed, verbose);

    // 输出降维后的结果
    std::cout << "t-SNE 结果 (降到 2 维):\n";
    for (int i = 0; i < N; ++i) {
        std::cout << "样本 " << i + 1 << ": (" << Y[i * no_dims] << ", " << Y[i * no_dims + 1] << ")\n";
    }

    return 0;
}
