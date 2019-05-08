A = readmatrix('./data/Faces_easy-left.dat');
B = readmatrix('./data/airplanes-btm.dat');
C = readmatrix('./data/panda-left.dat');
D = readmatrix('./data/pyramid-btm.dat');
E = readmatrix('./data/sunflower-btm.dat');
F = readmatrix('./data/umbrella-left.dat');
G = readmatrix('./data/yin_yang-left.dat');
A = A / 255;
B = B / 255;
C = C / 255;
D = D / 255;
E = E / 255;
F = F / 255;
G = G / 255;
[r1, c1] = size(A);
[r2, c2] = size(B);
[r3, c3] = size(C);
[r4, c4] = size(D);
[r5, c5] = size(E);
[r6, c6] = size(F);
[r7, c7] = size(G);
counter = 0;
for i = 1:74:r1-63
    counter = counter + 1;
    imwrite(A(i:i+63, 1:138), sprintf("./data/figures/Face_easy-left/Face_easy-left-%d.jpg", counter));
end
counter = 0;
for i = 1:74:r2-63
    counter = counter + 1;
    imwrite(B(i:i+63, 1:138), sprintf("./data/figures/airplanes-btm/airplanes-btm-%d.jpg", counter));
end
counter = 0;
for i = 1:74:r3-63
    counter = counter + 1;
    imwrite(C(i:i+63, 1:138), sprintf("./data/figures/panda-left/panda-left-%d.jpg", counter));
end
counter = 0;
for i = 1:74:r4-63
    counter = counter + 1;
    imwrite(D(i:i+63, 1:138), sprintf("./data/figures/pyramid-btm/pyramid-btm-%d.jpg", counter));
end
counter = 0;
for i = 1:74:r5-63
    counter = counter + 1;
    imwrite(E(i:i+63, 1:138), sprintf("./data/figures/sunflower-btm/sunflower-btm-%d.jpg", counter));
end
counter = 0;
for i = 1:74:r6-63
    counter = counter + 1;
    imwrite(F(i:i+63, 1:138), sprintf("./data/figures/umbrella-left/umbrella-left-%d.jpg", counter));
end
counter = 0;
for i = 1:74:r7-63
    counter = counter + 1;
    imwrite(G(i:i+63, 1:138), sprintf("./data/figures/yin_yang-left/yin_yang-left-%d.jpg", counter));
end
