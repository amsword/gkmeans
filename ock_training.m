function [ock_output_model, compactB]  = ock_training(...
    Xtraining, opt_input_ock)
%%
begin_time = tic;
default_num_partitions = 4;
default_sub_dic_size_each_partition = 256;
if isfield(opt_input_ock, 'all_D')
    all_D = opt_input_ock.all_D;
    default_num_partitions = numel(all_D);
    default_sub_dic_size_each_partition = size(all_D{1}, 2);
end

if isfield(opt_input_ock, 'num_sub_dic_each_partition')
    num_sub_dic_each_partition = opt_input_ock.num_sub_dic_each_partition;
else
    num_sub_dic_each_partition = 1;
end

if isfield(opt_input_ock, 'func_tol')
    func_tol = opt_input_ock.func_tol;
else
    func_tol = 10^-4;
end

if isfield(opt_input_ock, 'sub_dic_size_each_partition')
    sub_dic_size_each_partition = opt_input_ock.sub_dic_size_each_partition;
else
    sub_dic_size_each_partition = default_sub_dic_size_each_partition;
end

if isfield(opt_input_ock, 'num_partitions')
    num_partitions = opt_input_ock.num_partitions;
else
    num_partitions = default_num_partitions;
end

if isfield(opt_input_ock, 'max_iter')
    max_iter = opt_input_ock.max_iter;
else
    max_iter = 100;
end

if isfield(opt_input_ock, 'is_optimize_R')
    is_optimize_R = opt_input_ock.is_optimize_R;
else
    is_optimize_R = true;
end

if num_partitions == 1
    is_optimize_R = false;
end


dic_size_each_partitions = num_sub_dic_each_partition * sub_dic_size_each_partition;

%% set partition information
dim = size(Xtraining, 1);
[sub_dim_start_idx, sub_dim_lengths] = dim_split(dim, num_partitions);

%% intialize R
if isfield(opt_input_ock, 'init_R') && is_optimize_R
    R = opt_input_ock.init_R;
else
    R = eye(dim, dim);
end

%% initialize D
if ~isfield(opt_input_ock, 'init_all_D')
    all_D = RandomInitDictionary(Xtraining, sub_dim_start_idx, dic_size_each_partitions);
%    all_D = RandomInitDictionary2(Xtraining, sub_dim_start_idx, dic_size_each_partitions, num_sub_dic_each_partition);
else
    all_D = opt_input_ock.init_all_D;
end
ock_output_model.init_all_D = all_D;

%% initialize B
compactB = [];

clear para_encode;
para_encode.num_grouped = opt_input_ock.num_grouped;
para_encode.encoding_type = opt_input_ock.encoding_type;
para_encode.num_can = opt_input_ock.num_can;
if isfield(opt_input_ock, 'is_ock')
    para_encode.is_ock = opt_input_ock.is_ock;
end
%
if isfield(opt_input_ock, 'init_compactB')
    compactB = opt_input_ock.init_compactB;
    all_D = UpdateDictionary(Xtraining, compactB, sub_dim_start_idx, ...
        num_sub_dic_each_partition, sub_dic_size_each_partition);
else
    para_encode.is_initialize = 1;
    compactB = UpdateRepresentation(...
        R' * Xtraining, all_D, [], num_sub_dic_each_partition, ...
        para_encode);
end
para_encode.is_initialize = 1;

%%
obj = zeros(3 * max_iter, 1);
i = 1;
total_time = 0;
if ~is_optimize_R
    assert(sum(sum(abs(R - eye(dim, dim)))) < 10^-9);
end

%%
for iter = 1 : max_iter
    [num2str(iter) '/' num2str(max_iter)]
    
    tic;
    if is_optimize_R
        R = UpdateRotation(Xtraining, all_D, compactB);
        Z = R' * Xtraining;
    else
        Z = Xtraining;
    end
    
    obj(i) = DistortionLoss(Z, all_D, compactB);
    i = i + 1;
  
    all_D = UpdateDictionary(Z, compactB, sub_dim_start_idx, ...
        num_sub_dic_each_partition, sub_dic_size_each_partition);
  
    obj(i) = DistortionLoss(Z, all_D, compactB);
    
    i = i + 1;
    
    compactB = UpdateRepresentation(...
        Z, all_D, compactB, num_sub_dic_each_partition, para_encode);
    
    obj(i) = DistortionLoss(Z, all_D, compactB);
%     assert(obj(i) <= obj(i - 1) + 0.0001)
    i = i + 1;

    time_iter = toc;
    total_time = total_time + time_iter;
    fprintf('time/iter: %f. Left: %f. obj: %f\n', ...
        time_iter,  time_iter * (max_iter - iter), ...
        obj(i - 1));
    
    if iter > 1
        if abs(obj(i - 1) - obj(i - 4)) < func_tol
            break;
        end
    end

end

end_time = toc(begin_time);
ock_output_model.is_optimize_R = is_optimize_R;
ock_output_model.R = R;
ock_output_model.training_time = end_time;
ock_output_model.all_D = all_D;
ock_output_model.obj = obj(1 : 3 * iter);
ock_output_model.num_sub_dic_each_partition = num_sub_dic_each_partition;
ock_output_model.sub_dim_start_idx = sub_dim_start_idx;
ock_output_model.num_grouped = opt_input_ock.num_grouped;
ock_output_model.para_encode = para_encode;
if isfield(opt_input_ock, 'is_ock')
    ock_output_model.is_ock = opt_input_ock.is_ock;
end
