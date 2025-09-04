%% 加载数据
load mnist.mat;
test.images = test.images(:, :, 1:2000);   % 只取前 2000 张
[nrow, ncol, nsamples] = size(test.images);

% 转换为样本 x 特征矩阵
X = reshape(test.images, nrow*ncol, nsamples)';

%% PCA 测试
r = 20;  % 输出维度
[Y_pca, eigVals_pca] = ddr::pca(X, r);

% 重构数据（近似）: X ≈ Y * V'
reconstruction_pca = Y_pca * Y_pca' * X;
reconstructionError_pca = norm(X - reconstruction_pca, 'fro');
fprintf('PCA 重构误差: %.6f\n', reconstructionError_pca);

%% KPCA 测试
kernelType = 'rbf';
param = 1.0;   % sigma
[Y_kpca, eigVals_kpca] = ddr::kpca(X, r, kernelType, param);

projectionError_kpca = norm(Y_kpca, 'fro');
fprintf('KPCA 投影 norm: %.6f\n', projectionError_kpca);
