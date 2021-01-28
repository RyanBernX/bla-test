sparsity = [0.02, 0.05, 0.10, 0.14];
ndim = [10000, 20000];

for i = ndim
    for s = sparsity
        fprintf('creating sparse matrix %d x %d (sparsity = %d%%)\n', i, i, floor(100 * s));
        spA = sprandn(i, i, s);
        filename = sprintf('../../data/X%d_%d.mat', i, floor(100 * s));
        save(filename, 'spA');
    end
end