function [init_R, init_all_D] = construct_init_dictionary(R, old_all_D)


old_num_partitions = numel(old_all_D);

num_rows = 0;
num_cols = 0;
for i = 1 : old_num_partitions
    num_rows = num_rows + size(old_all_D{i}, 1);
    num_cols = num_cols + size(old_all_D{i}, 2);
end

new_num_partitions = old_num_partitions / 2;
init_all_D = cell(new_num_partitions, 1);

for i = 1 : new_num_partitions
    old_1 = (i - 1) * 2 + 1;
    old_2 = old_1 + 1;
    
    sub_rows = size(old_all_D{old_1}, 1) + size(old_all_D{old_2}, 1);
    sub_cols = size(old_all_D{old_1}, 2) + size(old_all_D{old_2}, 2);
    subD = zeros(sub_rows, sub_cols);
    subD(1 : size(old_all_D{old_1}, 1), ...
        1 : size(old_all_D{old_1}, 2)) = old_all_D{old_1};
    subD(size(old_all_D{old_1}, 1) + 1 : end, ...
        size(old_all_D{old_1}, 2) + 1 : end) = old_all_D{old_2};
    init_all_D{i} = subD;
end


if new_num_partitions == 1
    init_all_D = {R * init_all_D{1}};
    init_R = eye(num_rows);
else
    init_R = R;
end


