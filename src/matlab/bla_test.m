function bla_test(n)

if nargin < 1
    n = 100;
end

% random matrices
A = rand(n);
B = rand(n);
b = rand(n, 1);

% for gemv
tic;
for i = 1:50
    c = A * b;
end
toc;

tic;
for i = 1:50
    c = A' * b;
end
toc;

% for gemm
tic;
for i = 1:5
    C = A * B;
end
toc;

tic;
for i = 1:5
    C = A' * B;
end
toc;
