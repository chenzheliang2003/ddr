#include "ddr.hpp"
#include <iostream>
#include <cmath> // for fabs

int main() {
    try {
        // 读取数据（使用值而非引用）
        Eigen::MatrixXd data = read_csv("../../../examples/hald.csv");
        std::cout << "原始数据维度: " << data.rows() << " 行, " 
                  << data.cols() << " 列\n";
        
        PCA pca;
        
        // 拟合模型并降维
        pca.fit(data, 0.05); // 保留95%方差
        Eigen::MatrixXd reduced = pca.transform(data);
        std::cout << "降维后维度: " << reduced.rows() << " 行, " 
                  << reduced.cols() << " 列\n";

        std::cout << "解释性: " << pca.get_explained() << std::endl;
        
        // 保存降维结果
        write_csv(reduced, "../../../examples/reduced.csv");
        
        // 重构数据
        Eigen::MatrixXd reconstructed = pca.reconstruct(reduced);
        std::cout << "重构后维度: " << reconstructed.rows() << " 行, " 
                  << reconstructed.cols() << " 列\n";
        
        // 保存重构结果
        write_csv(reconstructed, "../../../examples/reconstructed.csv");
        
        // 计算重构误差
        double error = (data - reconstructed).array().abs().mean();
        std::cout << "平均重构误差: " << error << "\n";
        
        // 验证重构精度
        double max_error = (data - reconstructed).array().abs().maxCoeff();
        double min_value = data.array().abs().minCoeff();
        std::cout << "最大绝对误差: " << max_error << "\n";
        
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}