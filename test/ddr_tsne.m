load fisheriris.mat;
Y = ddr::tsne(meas,2);
gscatter(Y(:,1),Y(:,2),species)