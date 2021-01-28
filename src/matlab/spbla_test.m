function spbla_test(filename)

load(filename, 'spA');

[m, n] = size(spA);
b = rand(n, 1);
c = zeros(m, 1);

% SpMV
tic;
for i=1:50
    c = spA * b;
end
toc;

tic;
for i=1:50
    b = spA' * c;
end
toc;