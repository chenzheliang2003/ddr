load fisheriris.mat;
[Y, loss] = tsne_plugin(meas, 'Algorithm', 'exact', 'MaxIter', 300);
loss
figure;
gscatter(Y(:,1),Y(:,2),species)
title("鸢尾花数据集");

%%

load mnist.mat;
test.images = test.images(:, :, 1:2000);
[nrow, ncol, nsamples] = size(test.images);

X = reshape(test.images, nrow*ncol, nsamples)';
r = 50;
[X_pca, eigVals_pca] = pca_plugin(X, r);
[Y, loss] = tsne_plugin(X_pca, 'perplexity', 50);
loss
labels_str = arrayfun(@num2str, test.labels(1:2000), 'UniformOutput', false);
figure;
gscatter(Y(:,1),Y(:,2),labels_str)
title("MNIST 数据集");

%%

