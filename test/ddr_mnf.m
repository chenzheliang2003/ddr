load mnist.mat;
test.images = test.images(:, :, 1:2000);
[nrow, ncol, nsamples] = size(test.images);
X = reshape(test.images, nrow*ncol, nsamples)';
[W, H] = mnf_plugin(X,20);
reconstructionError = norm(X - W*H, 'fro');
fprintf('重构误差: %.6f\n', reconstructionError);

for i = 1:10
    img = reshape(H(i,:), [nrow,ncol]);
    dip::figure;
    imshow(img, []);
    dip::title(sprintf('基 %d', i));
end