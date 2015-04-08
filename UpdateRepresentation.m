function [new_compactB] = UpdateRepresentation(...
    Z, all_D, old_compact_subB, num_sub_dic_each_partition, ...
    para_encode)


if ~para_encode.is_initialize
    para_encode.old_codes = old_compact_subB;
end

new_compactB = MatchingPersuitFull3(...
    Z, all_D, num_sub_dic_each_partition, para_encode);

return;



num_partitions = numel(all_D);
num_point = size(Z, 2);
new_compactB = zeros(num_sub_dic_each_partition * num_partitions, ...
    num_point, 'int16');

idx_dim_start = 1;
for i = 1 : num_partitions
    subD = all_D{i};
    sub_dim = size(subD, 1);
    
    idx_dim_end = idx_dim_start + sub_dim - 1;
    
    subZ = Z(idx_dim_start : idx_dim_end, :);
    
    new_compact_SubB = mexMatchingPersuit(subZ, subD, num_sub_dic_each_partition, opt_input_encode);
    
    reconstructed = reconstruct_subD(subD, new_compact_SubB);
    new_error = sum((subZ - reconstructed) .^ 2, 1);
    
    if num_sub_dic_each_partition >= 0 ...
            && ~isempty(old_compact_subB) ...
            && num_sub_dic_each_partition ~= 1
        reconstructed2 = reconstruct_subD(subD, old_compact_subB);
        original_error = sum((subZ - reconstructed2) .^ 2, 1);
        no_need_update = new_error > original_error;
        if sum(no_need_update) ~= 0
            warning('here');
        end
        new_compact_SubB(:, no_need_update) = old_compact_subB(:, no_need_update);
    end
    new_compactB((i - 1) * num_sub_dic_each_partition + 1 : ...
        i * num_sub_dic_each_partition, :) = new_compact_SubB;
    idx_dim_start = idx_dim_end + 1;
end