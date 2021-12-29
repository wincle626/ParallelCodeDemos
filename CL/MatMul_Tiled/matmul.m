function matmul()
    A = randi(10, 4096, 1024);
    B = randi(10, 1024, 256);
    C = A * B;
    writematrix(A,'A.csv','Delimiter','tab');
    writematrix(B,'B.csv','Delimiter','tab');
    writematrix(C,'C.csv','Delimiter','tab');
end