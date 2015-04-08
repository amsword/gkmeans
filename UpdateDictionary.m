function all_D = UpdateDictionary(Z, compactB, sub_dim_start_idx, ...
    num_sub_dic_each_partition, sub_dic_size_each_partition)

num_partitions = numel(sub_dim_start_idx) - 1;
all_D = cell(num_partitions, 1);

all_words_each_partition = sub_dic_size_each_partition * num_sub_dic_each_partition;

for i = 1 : num_partitions
    idx_dim_start = sub_dim_start_idx(i);
    idx_dim_end = sub_dim_start_idx(i + 1)  - 1;
    
    subZ = Z(idx_dim_start : idx_dim_end, :);
    subB = compactB((i - 1) * num_sub_dic_each_partition + 1 : ...
        i * num_sub_dic_each_partition, :);
   
    all_D{i} = solve_Z_DB(subZ, subB, all_words_each_partition);
    
end

end

function D = solve_Z_DB(subZ, subB, all_words_each_partition)

lambda = 10^(-5);
[zbt, bbt] = mexZBt2(subZ, subB, all_words_each_partition);

D = zbt  / (double(bbt) + lambda * eye(all_words_each_partition));

return;

[U S V] = svd(full(subB * subB'));

diag_value = diag(S);
ind = diag_value > 10^-5;

if ~find(~ind)
    D = (subZ * subB')  / (subB * subB');
else
    selected_diag_value = diag_value(ind);
    non_zero_rhs = subZ * subB' * V;
    hat_D1 = non_zero_rhs(:, ind) * ...
        diag(selected_diag_value .^ -1);
    
    D = subZ(:, randperm(size(subZ, 2), size(subB, 1)));
    hat_D = D * U;
    hat_D2 = hat_D(:, ~ind);
    hat_D(:, ind) = hat_D1;
    hat_D(:, ~ind) = hat_D2;
    D = hat_D * U';
end

return;
s = sum(subB, 2);
ind = full((s ~= 0));

D = zeros(size(subZ, 1), size(subB, 1));

size_split = size(subB, 1) / num_split;


D(:, ~ind) = subZ(:, randperm(size(subZ, 2), sum(ind == 0)));


for idx_split = 1 : num_split
    ind_empty = false(size(subB, 1), 1);
    idx_start = (idx_split - 1) * size_split + 1;
    idx_end = idx_split * size_split;
    
    ind_empty(idx_start : idx_end) = ~ind(idx_start : idx_end);
    
    empty_pos = (ind_empty == 1);
    empty_count = sum(empty_pos);
    if empty_count > 0
        X = subZ(:, randperm(size(subZ, 2), empty_count));
        for i = 1 : (idx_split - 1)
            idx_d_start = (i - 1) * size_split + 1;
            idx_d_end = i * size_split;
            
            distance_2_dic = sqdist(X, D(:, idx_d_start : idx_d_end));
            [useless_var, idx] = min(distance_2_dic, [], 2);
            X = X - D(:, idx + idx_d_start - 1);
        end        
        D(:, ind_empty) = X;
    end
end
end
