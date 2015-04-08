function all_D = RandomInitDictionary(Xtraining, sub_dim_start_idx, dic_size_each_partitions)

num_point = size(Xtraining, 2);
num_partitions = numel(sub_dim_start_idx) - 1;
all_D = cell(num_partitions, 1);
rp = randperm(num_point);
rp = rp(1 : dic_size_each_partitions);

for i = 1 : num_partitions
    idx_dim_start = sub_dim_start_idx(i);
    idx_dim_end = sub_dim_start_idx(i + 1) - 1;
    
    all_D{i} = Xtraining(idx_dim_start : idx_dim_end, rp);
end