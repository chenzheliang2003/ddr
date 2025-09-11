load mnist.mat;
test.images = test.images(:, :, 1:2000);
[nrow, ncol, nsamples] = size(test.images);
X = reshape(test.images, nrow*ncol, nsamples)';
Y = umap_plugin(X,2);
labels_str = arrayfun(@num2str, test.labels(1:2000), 'UniformOutput', false);
gscatter(Y(:,1), Y(:,2), labels_str);