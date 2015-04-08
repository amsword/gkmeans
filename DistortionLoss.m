function y = DistortionLoss(Z, all_D, compactB)

%  D = ConstructD(all_D);
y = RelatedFunctions2(0, Z, all_D, compactB);
return;
if size(compactB, 1) == numel(all_D)
    y = RelatedFunctions(0, Z, all_D, compactB);
    return;
else
    
    return;
    
end


W = MultipleAllAll(all_D, compactB);

y = sum(sum((Z - W) .^ 2));
%
% y = 0;
%
% batch_size = 1000;
% batch_num = ceil(size(Xtraining, 2) / batch_size);
%
% for i = 1 : batch_num
%     idx_start = (i - 1) * batch_size + 1;
%     idx_end = i * batch_size;
%     y = y + sum(sum(Xtraining(:, ))
% end