compile_c_code; % run only once
addpath('c_code/bin');

% please modify the following two variables to achieve different quantization schemes. 
num_sub_dic_each_partition = 4;
num_partitions = 1;

num_grouped = 2; % it can be modified to be 1. '2' corresponds better accuracy with longer time

% it is ok not to change the following variables
encoding_method = 'gkmeans';
num_can = 12;
num_can_encode = 12;
initialize_type = 'h_ockmeans'
h_ockmeans_initi_num_iter = 10; 
h_ockmeans_final_num_iter = 100 - log(num_sub_dic_each_partition) / log(2) * h_ockmeans_initi_num_iter;
assert(h_ockmeans_final_num_iter == floor(h_ockmeans_final_num_iter));
assert(h_ockmeans_final_num_iter > 0);
topks = [1];
sub_dic_size_each_partition = 256;

% please change the following routines to feed your training data
data_dim = 128;
num_training = 10000;
Xtraining = rand(data_dim, num_training);

clear opt_input_ock;
opt_input_ock.num_sub_dic_each_partition = num_sub_dic_each_partition;
opt_input_ock.num_partitions = num_partitions;
opt_input_ock.sub_dic_size_each_partition = sub_dic_size_each_partition;
opt_input_ock.num_grouped = num_grouped;
opt_input_ock.encoding_type = encoding_method;
opt_input_ock.num_can_train = num_can;
opt_input_ock.num_can = num_can;

% hirarchical initialization 
num_sub_problem = log(num_sub_dic_each_partition) / log(2);
num_sub_problem = floor(num_sub_problem);
assert(2 .^ (num_sub_problem) == num_sub_dic_each_partition);
clear opt_input_ock2;
opt_input_ock2.num_sub_dic_each_partition = 1;
opt_input_ock2.num_partitions = num_sub_dic_each_partition;
opt_input_ock2.sub_dic_size_each_partition = ...
sub_dic_size_each_partition;
opt_input_ock2.max_iter = h_ockmeans_initi_num_iter;
opt_input_ock2.num_grouped = num_grouped;
opt_input_ock2.encoding_type = encoding_method;
opt_input_ock2.num_can_train = num_can;
opt_input_ock2.num_can = num_can;

[sub_dim_start_idx] = dim_split(size(Xtraining, 1), opt_input_ock2.num_partitions);
init_all_D = RandomInitDictionary(Xtraining, sub_dim_start_idx, ...
    opt_input_ock2.sub_dic_size_each_partition * ...
    opt_input_ock2.num_sub_dic_each_partition);

opt_input_ock2.init_all_D = init_all_D;
all_ock_model = cell(num_sub_problem, 1);
t1 = tic;
for idx_sub_problem = 1 : num_sub_problem
    t2 = tic;
    ock_model = ock_training(Xtraining, opt_input_ock2);
    time_curr = toc(t2);
    all_ock_model{idx_sub_problem} = ock_model;

    opt_input_ock2.num_sub_dic_each_partition = ...
    opt_input_ock2.num_sub_dic_each_partition * 2;
    opt_input_ock2.num_partitions = opt_input_ock2.num_partitions / 2;
    [opt_input_ock2.init_R, opt_input_ock2.init_all_D] = construct_init_dictionary(...
        ock_model.R, ...
        ock_model.all_D);
end
time_h_init = toc(t1);


opt_input_ock.init_all_D = opt_input_ock2.init_all_D;
opt_input_ock.max_iter = h_ockmeans_final_num_iter;
x = tic;
ock_model = ock_training(Xtraining, opt_input_ock);
time_training = toc(x);

training_energy = sum(Xtraining(:) .^ 2);

% modify the following to feed your own database
num_base = 20000;
Xbase = rand(data_dim, num_base);
base_energy = sum(Xbase(:) .^ 2);

ock_model.para_encode.num_can_encode = num_can_encode;
ock_model.para_encode.num_can = num_can_encode;
x = tic;
mat_compact_B = ock_encoding(ock_model, Xbase);
time_encoding = toc(x);

% loss on the database
if ock_model.is_optimize_R
    Xbase = ock_model.R' * Xbase;
end
loss_base = DistortionLoss(Xbase, ock_model.all_D, mat_compact_B)

NBase = size(mat_compact_B, 2);
base_energy = sum(Xbase(:) .^ 2);

num_test = 1000;
Xtest = rand(data_dim, num_test); 
if ock_model.is_optimize_R
    Xtest = ock_model.R' * Xtest;
end


%% perform retrieval, you can change the number 100 to other numbers, it means
% how many data points are returned
tic;
retrieval_result = mexRetrievalSave(mat_compact_B, ...
    ock_model.all_D, ...
    Xtest, ...
    500);
time_retrieval = toc;

% get the ground truth. It is recommneded to calculate the ground truth once
% if it is used multiple times;
% if the data size is large, please calculate the ground truth batch by batch 
% to avoid the potential memory issue;
ground_truth_file = 'gt_file';
X1 = Xtest;
X2 = Xbase;
R = X1'*X2;
sqrX1 = sum(X1.^2);
sqrX2 = sum(X2.^2);
D = bsxfun(@plus, sqrX1', sqrX2);
D = real(sqrt(D-2*R));
[s_dist, s_idx] = sort(D, 2);
s_dist = s_dist';
s_idx = s_idx';
save_gnd(s_idx(1 : topks(end), :) - 1, s_dist(1 : topks(end), :), 'double', ground_truth_file);
% evaluate the result
tic;
rec = mexResultRecall(retrieval_result, ...
    ground_truth_file, ...
    int32(topks), ...
    size(Xbase, 2));
time_cost_recall = toc;
if size(rec, 2) == 1
    fprintf('rec is %f at %d point\n', rec(end), numel(rec));
end

