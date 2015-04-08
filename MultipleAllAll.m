function W = MultipleAllAll(all_D, compactB)

% num_sub_dic_each_partition = size(compactB, 1) / numel(all_D);
% for i = 1 : numel(all_D)
%     subD = all_D{i};
%     if ~isempty(find(isnan(subD), 1)) || ...
%             ~isempty(find(isinf(subD), 1))
%         save('tmp', 'all_D', 'compactB');
%         error('dfsdf');
%     end
%     
%     idx_start = (i - 1) * num_sub_dic_each_partition + 1;
%     idx_end = idx_start + idx_start - 1;
%     if ~isempty(find(compactB(idx_start : idx_end, :) < 0 | ...
%         compactB(idx_start : idx_end, :) >= size(subD, 2), 1));
%     save('tmp', 'all_D', 'compactB');
%         error('dfsdf');
%     end
% end


    W = RelatedFunctions2(1, all_D, compactB);
    
%     if ~isempty(find(isnan(W), 1)) || ...
%             ~isempty(find(isinf(W), 1))
%         save('tmp', 'all_D', 'compactB');
%         error('dfsdf');
%     end
    return;



num_partitions = numel(all_D);

dim = 0;
for i = 1 : num_partitions
    dim = dim + size(all_D{i}, 1);
end

num_point = size(compactB, 2);

num_sub_dic_each_partition = size(compactB, 1) / num_partitions;

W = zeros(dim, num_point);

idx_start = 1;
for i = 1 : numel(all_D)
    subD = all_D{i};
    dic_dim = size(subD, 1);
    
    idx_end = dic_dim + idx_start - 1;
    
    W(idx_start : idx_end, :) = reconstruct_subD(subD, ...
        compactB((i - 1) * num_sub_dic_each_partition + 1 : ...
        i * num_sub_dic_each_partition, :));
    
    idx_start = idx_end + 1;
end

% dic_dim = size(all_D{1}, 1);
% dic_num = numel(all_D);
%
% dim = dic_num * dic_dim;
%
% num_point = size(all_B{1}, 2);
%
% W = zeros(dim, num_point);
%
% for i = 1 : numel(all_D)
%     idx_start = (i - 1) * dic_dim + 1;
%     idx_end = i * dic_dim;
%
%     W(idx_start : idx_end, :) = all_D{i} * full(all_B{i});
% end