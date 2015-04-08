function R = UpdateRotation(Xtraining, all_D, compactB)


% D = ConstructD(all_D);
% 
% W = D * B;

W = MultipleAllAll(all_D, compactB);

[svd_U, svd_Sigma, svd_V] = svd(Xtraining * W');
R = svd_U * svd_V';
