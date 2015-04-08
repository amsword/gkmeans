function [mat_compact_B] = UpdateRepresentationCompact(...
    Xbase, ock_model)

all_D = ock_model.all_D;
num_sub_dic_each_partition = ock_model.num_sub_dic_each_partition;

if ock_model.is_optimize_R
    Z = ock_model.R' * Xbase;
else
    Z= Xbase;
end

if isfield(ock_model, 'para_encode')
    para_encode = ock_model.para_encode;
end

para_encode.is_initialize = 1;
para_encode.num_grouped  = ock_model.num_grouped;

if isfield(ock_model, 'is_ock')
    para_encode.is_ock = ock_model.is_ock;
end
para_encode
mat_compact_B = UpdateRepresentation(...
    Z, all_D, [], num_sub_dic_each_partition, ...
    para_encode);


% there is no need to conduct the following.
% para_encode.is_initialize = 0;
% % 
% mat_compact_B = UpdateRepresentation(...
%     Z, all_D, mat_compact_B, ...
%     num_sub_dic_each_partition, para_encode);

return;




if num_sub_dic_each_partition == 1
    method = 0;
else
    method = 102;
end

num_point = size(Xtraining, 2);

mat_compact_B = zeros(numel(all_D) * num_sub_dic_each_partition, num_point, 'int16');

idx_dim_start = 1;
for i = 1 : numel(all_D)
    subD = all_D{i};
    sub_dim = size(subD, 1);
    
    idx_dim_end = idx_dim_start + sub_dim - 1;
    subZ = Z(idx_dim_start : idx_dim_end, :);
    idx_dim_start = idx_dim_end + 1;
    
    new_compact_SubB = mexMatchingPersuit(subZ, subD, num_sub_dic_each_partition, method);

    mat_compact_B((i - 1)* num_sub_dic_each_partition + 1 : i * num_sub_dic_each_partition, : ) = ...
        new_compact_SubB;
end